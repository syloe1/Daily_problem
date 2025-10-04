#include "CentralCache.h"
#include "PageCache.h"
#include "Common.h"
#include <chrono>
#include <unordered_map>
#include <thread>
#include <stdexcept>

// 初始化静态成员
const std::chrono::milliseconds CentralCache::DELAY_INTERVAL(1000);

CentralCache::CentralCache() {
    // 初始化中央缓存自由链表
    for (auto& node : centralFreeList_) {
        node.store(nullptr, std::memory_order_relaxed);
    }
    
    // 初始化自旋锁
    for (auto& lock : locks_) {
        lock.clear(std::memory_order_relaxed);
    }
    
    // 初始化延迟计数
    for (auto& count : delayCounts_) {
        count.store(0, std::memory_order_relaxed);
    }
    
    // 初始化最后归还时间
    for (auto& time : lastReturnTimes_) {
        time = std::chrono::steady_clock::now();
    }
    
    spanCount_.store(0, std::memory_order_relaxed);
}

// 从中央缓存获取一定数量的内存块
void* CentralCache::fetchRange(size_t index) {
    if (index >= FREE_LIST_SIZE) {
        throw std::out_of_range("CentralCache::fetchRange - index out of range");
    }
    
    // 获取自旋锁
    while (locks_[index].test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    
    void* result = nullptr;
    try {
        // 尝试从现有链表获取
        result = centralFreeList_[index].load(std::memory_order_relaxed);
        
        if (!result) {
            // 缓存为空，从PageCache获取
            size_t size = (index + 1) * ALIGNMENT;
            result = fetchFromPageCache(size);
            
            if (!result) {
                throw std::bad_alloc();
            }
            
            char* start = static_cast<char*>(result);
            const size_t SPAN_PAGES = 8;  // 固定8页为一个span
            size_t numPages = (size <= SPAN_PAGES * PAGE_SIZE) ? 
                              SPAN_PAGES : (size + PAGE_SIZE - 1) / PAGE_SIZE;
            size_t blockNum = (numPages * PAGE_SIZE) / size;
            
            if (blockNum > 1) {
                // 将内存块链接成链表
                for (size_t i = 1; i < blockNum; ++i) {
                    void* current = start + (i - 1) * size;
                    void* next = start + i * size;
                    *reinterpret_cast<void**>(current) = next;
                }
                *reinterpret_cast<void**>(start + (blockNum - 1) * size) = nullptr;
                
                // 保留一个块作为结果，其余加入中央缓存
                void* next = *reinterpret_cast<void**>(result);
                *reinterpret_cast<void**>(result) = nullptr;
                centralFreeList_[index].store(next, std::memory_order_release);
                
                // 记录span信息
                size_t trackerIndex = spanCount_.fetch_add(1, std::memory_order_relaxed);
                if (trackerIndex < spanTrackers_.size()) {
                    spanTrackers_[trackerIndex].spanAddr.store(start, std::memory_order_release);
                    spanTrackers_[trackerIndex].numPages.store(numPages, std::memory_order_release);
                    spanTrackers_[trackerIndex].blockCount.store(blockNum, std::memory_order_release);
                    spanTrackers_[trackerIndex].freeCount.store(blockNum - 1, std::memory_order_release);
                }
            }
        } else {
            // 从现有链表中取出第一个块
            void* next = *reinterpret_cast<void**>(result);
            *reinterpret_cast<void**>(result) = nullptr;
            centralFreeList_[index].store(next, std::memory_order_release);
            
            // 更新span的空闲计数
            SpanTracker* tracker = getSpanTracker(result);
            if (tracker) {
                tracker->freeCount.fetch_sub(1, std::memory_order_release);
            }
        }
    } catch (...) {
        // 释放锁并重新抛出异常
        locks_[index].clear(std::memory_order_release);
        throw;
    }
    
    // 释放锁
    locks_[index].clear(std::memory_order_release);
    return result;
}

// 将内存块归还给中央缓存
void CentralCache::returnRange(void* start, size_t size, size_t index) {
    if (!start || index >= FREE_LIST_SIZE) {
        return;
    }
    
    size_t blockSize = (index + 1) * ALIGNMENT;
    size_t blockCount = size / blockSize;
    
    // 获取自旋锁
    while (locks_[index].test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    
    try {
        // 将归还的内存块链接到中央缓存
        void* end = start;
        size_t count = 1;
        while (*reinterpret_cast<void**>(end) != nullptr && count < blockCount) {
            end = *reinterpret_cast<void**>(end);
            count++;
        }
        
        // 插入到中央缓存链表头部
        void* current = centralFreeList_[index].load(std::memory_order_relaxed);
        *reinterpret_cast<void**>(end) = current;
        centralFreeList_[index].store(start, std::memory_order_release);
        
        // 更新延迟计数和检查是否需要延迟归还
        size_t currentCount = delayCounts_[index].fetch_add(1, std::memory_order_relaxed);
        auto currentTime = std::chrono::steady_clock::now();
        
        if (shouldPerformDelayedReturn(index, currentCount, currentTime)) {
            performDelayedReturn(index);
        }
    } catch (...) {
        locks_[index].clear(std::memory_order_release);
        throw;
    }
    
    locks_[index].clear(std::memory_order_release);
}

// 判断是否需要执行延迟归还
bool CentralCache::shouldPerformDelayedReturn(size_t index, size_t currentCount,
                                             std::chrono::steady_clock::time_point currentTime) {
    if (index >= FREE_LIST_SIZE) {
        return false;
    }
    
    // 两种情况需要执行延迟归还：计数达到阈值或时间间隔达到阈值
    if (currentCount >= MAX_DELAY_COUNT) {
        return true;
    }
    
    auto lastTime = lastReturnTimes_[index];
    return (currentTime - lastTime) >= DELAY_INTERVAL;
}

// 执行延迟归还操作，将内存从中央缓存归还给页缓存
void CentralCache::performDelayedReturn(size_t index) {
    if (index >= FREE_LIST_SIZE) {
        return;
    }
    
    // 重置延迟计数和时间
    delayCounts_[index].store(0, std::memory_order_relaxed);
    lastReturnTimes_[index] = std::chrono::steady_clock::now();
    
    // 统计每个span的空闲块数量
    std::unordered_map<SpanTracker*, size_t> spanFreeCounts;
    void* currentBlock = centralFreeList_[index].load(std::memory_order_relaxed);
    
    while (currentBlock) {
        SpanTracker* tracker = getSpanTracker(currentBlock);
        if (tracker) {
            spanFreeCounts[tracker]++;
        }
        currentBlock = *reinterpret_cast<void**>(currentBlock);
    }
    
    // 更新每个span的空闲计数并检查是否可以归还
    for (const auto& pair : spanFreeCounts) {
        updateSpanFreeCount(pair.first, pair.second, index);
    }
}

// 更新span的空闲块计数，如果所有块都空闲则归还给页缓存
void CentralCache::updateSpanFreeCount(SpanTracker* tracker, size_t newFreeBlocks, size_t index) {
    if (!tracker || index >= FREE_LIST_SIZE) {
        return;
    }
    
    // 更新空闲计数
    size_t oldFreeCount = tracker->freeCount.load(std::memory_order_relaxed);
    size_t newFreeCount = oldFreeCount + newFreeBlocks;
    tracker->freeCount.store(newFreeCount, std::memory_order_release);
    
    // 如果所有块都空闲，归还给PageCache
    if (newFreeCount == tracker->blockCount.load(std::memory_order_relaxed)) {
        void* spanAddr = tracker->spanAddr.load(std::memory_order_relaxed);
        size_t numPages = tracker->numPages.load(std::memory_order_relaxed);
        
        if (!spanAddr || numPages == 0) {
            return;
        }
        
        // 从中央缓存链表中移除属于该span的所有块
        void* head = centralFreeList_[index].load(std::memory_order_relaxed);
        void* newHead = nullptr;
        void* prev = nullptr;
        void* current = head;
        
        while (current) {
            void* next = *reinterpret_cast<void**>(current);
            // 检查当前块是否属于该span
            if (current >= spanAddr &&
                current < static_cast<char*>(spanAddr) + numPages * PAGE_SIZE) {
                // 从链表中移除
                if (prev) {
                    *reinterpret_cast<void**>(prev) = next;
                } else {
                    newHead = next;
                }
            } else {
                prev = current;
            }
            current = next;
        }
        
        // 更新中央缓存链表头
        centralFreeList_[index].store(newHead, std::memory_order_release);
        
        // 归还给PageCache
        PageCache::getInstance().deallocateSpan(spanAddr, numPages);
    }
}

// 从页缓存获取内存
void* CentralCache::fetchFromPageCache(size_t size) {
    if (size == 0) {
        return nullptr;
    }
    
    const size_t SPAN_PAGES = 8;
    size_t numPages;
    
    // 计算需要的页数
    if (size <= SPAN_PAGES * PAGE_SIZE) {
        // 小内存块使用固定大小的span
        numPages = SPAN_PAGES;
    } else {
        // 大内存块按实际需求分配
        numPages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    }
    
    return PageCache::getInstance().allocateSpan(numPages);
}

// 查找内存块所属的span跟踪器
SpanTracker* CentralCache::getSpanTracker(void* blockAddr) {
    if (!blockAddr) {
        return nullptr;
    }
    
    // 遍历所有span跟踪器查找所属span
    size_t count = spanCount_.load(std::memory_order_relaxed);
    for (size_t i = 0; i < count && i < spanTrackers_.size(); ++i) {
        void* spanAddr = spanTrackers_[i].spanAddr.load(std::memory_order_relaxed);
        size_t numPages = spanTrackers_[i].numPages.load(std::memory_order_relaxed);
        
        if (spanAddr && numPages > 0) {
            // 检查blockAddr是否在当前span范围内
            if (blockAddr >= spanAddr &&
                blockAddr < static_cast<char*>(spanAddr) + numPages * PAGE_SIZE) {
                return &spanTrackers_[i];
            }
        }
    }
    
    return nullptr;
}
