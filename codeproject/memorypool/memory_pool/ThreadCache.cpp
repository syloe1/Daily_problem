// ThreadCache.cpp
#include "ThreadCache.h"
#include "CentralCache.h"
#include "Common.h"
#include "PageCache.h"
#include <cstdlib>
#include <stdexcept>
ThreadCache::~ThreadCache() {
    for(size_t i = 0; i < FREE_LIST_SIZE; i++) {
        if (freeList_[i] != nullptr) {
            void* start = freeList_[i];
            size_t blockSize = (i + 1) * ALIGNMENT;
            // 计算链表中的块数量
            size_t count = 0;
            void* current = start;
            while (current != nullptr) {
                count++;
                current = *reinterpret_cast<void**>(current);
            }
            // 返还给CentralCache
            CentralCache::getInstance().returnRange(start, count * blockSize, i);
        }
    }
}
void* ThreadCache::allocate(size_t size) {
    if (size == 0) {
        size = ALIGNMENT;
    }

    // 大内存直接使用PageCache
    if (size > MAX_BYTES) {
        size_t numPages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
        return PageCache::getInstance().allocateSpan(numPages);
    }

    // 小内存走内存池逻辑
    size_t index = getIndex(size);
    if (index >= FREE_LIST_SIZE) {
        void* ptr = malloc(size);
        if (!ptr) {
            throw std::bad_alloc();
        }
        return ptr;
    }

    // 从自由链表获取
    if (freeList_[index]) {
        void* ptr = freeList_[index];
        freeList_[index] = *reinterpret_cast<void**>(ptr);
        freeListSize_[index]--;
        return ptr;
    }

    // 从中心缓存获取
    return fetchFromCentralCache(index);
}

void ThreadCache::deallocate(void* ptr, size_t size) {
    if (size > MAX_BYTES) {
        // 大内存直接使用PageCache释放
        size_t numPages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
        PageCache::getInstance().deallocateSpan(ptr, numPages);
        return;
    }
    
    // 检查index是否在有效范围内
    size_t index = getIndex(size);
    if (index >= FREE_LIST_SIZE) {
        free(ptr);
        return;
    }
    
    // 小内存处理：加入自由链表
    *reinterpret_cast<void**>(ptr) = freeList_[index];
    freeList_[index] = ptr;
    freeListSize_[index]++;
    
    if (shouldReturnToCentralCache(index)) {
        returnToCentralCache(freeList_[index], size);
    }
}
bool ThreadCache::shouldReturnToCentralCache(size_t index) {
    size_t threshold = 256;
    return (freeListSize_[index] > threshold);  // 修复拼写错误 threshould -> threshold
}

void* ThreadCache::fetchFromCentralCache(size_t index) {
    void* start = CentralCache::getInstance().fetchRange(index);
    if (!start) return nullptr; 
    
    void* result = start;
    void* current = start;
    size_t batchNum = 0;
    
    // 计算批次数量
    while (current != nullptr) {
        batchNum++;
        current = *reinterpret_cast<void**>(current);
    }
    
    freeList_[index] = *reinterpret_cast<void**>(start);
    freeListSize_[index] += batchNum;
    return result;
}


void ThreadCache::returnToCentralCache(void* start, size_t size) {
    size_t index = getIndex(size);  // 从Common.h获取
    size_t alignedSize = (index + 1) * ALIGNMENT;  // 计算对齐后的大小
    size_t batchNum = freeListSize_[index];
    
    if (batchNum <= 1) return;
    
    size_t keepNum = std::max(batchNum / 4, size_t(1));
    size_t returnNum = batchNum - keepNum;
    
    char* current = static_cast<char*>(start);
    char* splitNode = current;
    
    for (size_t i = 0; i < keepNum; i++) {
        splitNode = reinterpret_cast<char*>(*reinterpret_cast<void**>(splitNode));
        if (splitNode == nullptr) {
            returnNum = batchNum - (i + 1);
            break;
        }
    }
    
    if (splitNode != nullptr) {
        void* nextNode = *reinterpret_cast<void**>(splitNode);
        *reinterpret_cast<void**>(splitNode) = nullptr;
        freeList_[index] = start;
        freeListSize_[index] = keepNum;
        
        if (returnNum > 0 && nextNode != nullptr) {
            CentralCache::getInstance().returnRange(nextNode, returnNum * alignedSize, index);
        }
    }
}