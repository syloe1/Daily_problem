#include "PageCache.h"
#include <sys/mman.h>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <thread>
#include <stdexcept>
PageCache::~PageCache() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 释放所有通过systemAlloc分配的内存
    for (auto& pair : spanMap_) {
        Span* span = pair.second;
        if (span && span->pageAddr) {
            systemFree(span->pageAddr, span->numPages);
        }
        delete span;
    }
    spanMap_.clear();
    
    // 清空空闲链表
    freeSpans_.clear();
}
void* PageCache::allocateSpan(size_t numPages) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(numPages == 0) return nullptr;
    std::cout<<"THread "<<std::this_thread::get_id()<<" allocating"<< numPages<<" pages"<<std::endl;
    // 查找合适的空闲span
    auto it = freeSpans_.lower_bound(numPages);
    if (it != freeSpans_.end()) {
        Span* span = it->second;

        // 将取出的span从原有的空闲链表中移除
        if (span->next) {
            freeSpans_[it->first] = span->next;
        } else {
            freeSpans_.erase(it);
        }

        // 如果span大于需要的numPages则进行分割
        if (span->numPages > numPages) {
            Span* newSpan = new Span;
            newSpan->pageAddr = static_cast<char*>(span->pageAddr) + 
                                numPages * PAGE_SIZE;
            newSpan->numPages = span->numPages - numPages;
            newSpan->next = nullptr;

            // 将超出部分放回空闲列表头部
            auto& list = freeSpans_[newSpan->numPages];
            newSpan->next = list;
            list = newSpan;

            span->numPages = numPages;
        }

        // 记录span信息用于回收
        spanMap_[span->pageAddr] = span;
        return span->pageAddr;
    }

    // 没有合适的span，向系统申请
    void* memory = systemAlloc(numPages);
    if (!memory) {
        throw std::bad_alloc();
    }

    // 创建新的span
    Span* span = new Span;
    span->pageAddr = memory;
    span->numPages = numPages;
    span->next = nullptr;

    // 记录span信息用于回收
    spanMap_[memory] = span;
    return memory;
}

void PageCache::deallocateSpan(void* ptr, size_t numPages) {
    if (!ptr) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    if(numPages == 0) return ;
    std::cout << "Thread " << std::this_thread::get_id() << " deallocating " << numPages 
              << " pages at " << ptr << std::endl;
    // 查找对应的span
    auto it = spanMap_.find(ptr);
    if (it == spanMap_.end()) {
        // 如果没有找到对应的span，直接释放给系统
        systemFree(ptr, numPages);
        return;
    }

    Span* span = it->second;

    // 尝试合并后面的span
    void* nextAddr = static_cast<char*>(ptr) + numPages * PAGE_SIZE;
    auto nextIt = spanMap_.find(nextAddr);
    
    if (nextIt != spanMap_.end()) {
        Span* nextSpan = nextIt->second;
        
        // 检查nextSpan是否在空闲链表中
        bool found = false;
        auto freeIt = freeSpans_.find(nextSpan->numPages);
        if (freeIt != freeSpans_.end()) {
            Span* nextList = freeIt->second;
            
            // 检查是否是头节点
            if (nextList == nextSpan) {
                freeSpans_[nextSpan->numPages] = nextSpan->next;
                found = true;
            } else if (nextList) {
                Span* prev = nextList;
                while (prev->next) {
                    if (prev->next == nextSpan) {   
                        // 将nextSpan从空闲链表中移除
                        prev->next = nextSpan->next;
                        found = true;
                        break;
                    }
                    prev = prev->next;
                }
            }
        }

        // 合并span
        if (found) {
            span->numPages += nextSpan->numPages;
            spanMap_.erase(nextAddr);
            delete nextSpan;
        }
    }

    // 尝试合并前面的span
    if (ptr != nullptr) {
        void* prevAddr = static_cast<char*>(ptr) - PAGE_SIZE;
        auto prevIt = spanMap_.find(prevAddr);
        
        if (prevIt != spanMap_.end()) {
            Span* prevSpan = prevIt->second;
            
            // 检查prevSpan是否在空闲链表中
            bool found = false;
            auto freeIt = freeSpans_.find(prevSpan->numPages);
            if (freeIt != freeSpans_.end()) {
                Span* prevList = freeIt->second;
                
                // 检查是否是头节点
                if (prevList == prevSpan) {
                    freeSpans_[prevSpan->numPages] = prevSpan->next;
                    found = true;
                } else if (prevList) {
                    Span* prevPrev = prevList;
                    while (prevPrev->next) {
                        if (prevPrev->next == prevSpan) {
                            // 将prevSpan从空闲链表中移除
                            prevPrev->next = prevSpan->next;
                            found = true;
                            break;
                        }
                        prevPrev = prevPrev->next;
                    }
                }
            }
            
            // 合并span
            if (found) {
                prevSpan->numPages += span->numPages;
                spanMap_.erase(ptr);
                delete span;
                span = prevSpan;
                ptr = prevSpan->pageAddr;
            }
        }
    }

    // 将合并后的span通过头插法插入空闲列表
    auto& list = freeSpans_[span->numPages];
    span->next = list;
    list = span;
    
    // 更新spanMap中的指针
    spanMap_[ptr] = span;
}

void* PageCache::systemAlloc(size_t numPages) {
    size_t size = numPages * PAGE_SIZE;

    // 使用mmap分配内存
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return nullptr;
    }

    // 清零内存
    memset(ptr, 0, size);
    return ptr;
}

void PageCache::systemFree(void* ptr, size_t numPages) {
    if (!ptr) return;
    
    size_t size = numPages * PAGE_SIZE;
    munmap(ptr, size);
}
