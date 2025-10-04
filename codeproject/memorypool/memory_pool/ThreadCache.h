// ThreadCache.h
#include <cstddef>
#include <array>
#include "Common.h"

class ThreadCache {
public: 
    ~ThreadCache();
    static ThreadCache* getInstance() {
        // 确保线程局部存储正确初始化
        thread_local ThreadCache instance;
        return &instance;
    }
    
    void* allocate(size_t size);
    void deallocate(void* ptr, size_t size);
    
private:
    ThreadCache() {
        // 初始化自由链表
        for (auto& list : freeList_) {
            list = nullptr;
        }
        // 初始化自由链表大小
        for (auto& size : freeListSize_) {
            size = 0;
        }
    }
    
    void* fetchFromCentralCache(size_t index);
    void returnToCentralCache(void* start, size_t size);
    bool shouldReturnToCentralCache(size_t index);
    
private:
    std::array<void*, FREE_LIST_SIZE> freeList_;
    std::array<size_t, FREE_LIST_SIZE> freeListSize_;
};