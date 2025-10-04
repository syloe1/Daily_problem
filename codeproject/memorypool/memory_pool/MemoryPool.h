#pragma once
#include <cstdint>
#include <atomic>
#include <mutex>
#include <cassert>

namespace memoryPool {
    #define MEMORY_POOL_NUM 64
    #define SLOT_BASE_SIZE 8
    #define MAX_SLOT_SIZE 512
    #define ATOMIC_PTR_SIZE sizeof(std::atomic<void*>)

    // 修复1：Slot结构——元数据与用户数据严格隔离，用户数据区大小固定
    struct Slot {
        std::atomic<Slot*> next;  // 元数据：无锁队列指针（8字节）
        char userData[MAX_SLOT_SIZE];  // 用户数据区：最大512字节（覆盖所有可能的用户请求）
    };

    class MemoryPool {
    public:
        MemoryPool(size_t BlockSize = 4096);
        ~MemoryPool();
        void init(size_t userSize);  // userSize：用户请求的内存大小（≤MAX_SLOT_SIZE）
        void* allocate();
        void deallocate(void* ptr);

    private:
        void allocateNewBlock();
        size_t padPointer(char* p, size_t align);
        bool pushFreeList(Slot* slot);
        Slot* popFreeList();

    private:
        size_t BlockSize_;          // 内存块总大小（默认4096字节）
        size_t SlotMetaSize_;       // 元数据大小：sizeof(Slot::next) = 8字节
        size_t SlotTotalSize_;      // 单个Slot总大小：sizeof(Slot) = 8 + 512 = 520字节（固定）
        size_t UserDataSize_;       // 用户实际请求的内存大小（≤MAX_SLOT_SIZE）
        Slot* firstBlock_;          // 块链表头
        Slot* curSlot_;             // 当前可用Slot
        std::atomic<Slot*> freeList_;  // 无锁空闲链表
        Slot* lastSlot_;            // 块内最后一个Slot
        std::mutex mutexForBlock_;  // 保护块分配
    };

    class HashBucket {
    public:
        static void initMemoryPool();
        static MemoryPool& getMemoryPool(int idx);
        static void* useMemory(size_t size) {
            if (size <= 0 || size > MAX_SLOT_SIZE) {
                return operator new(size);
            }
            // 计算匹配的内存池索引（用户数据大小，不含元数据）
            int idx = static_cast<int>(((size + SLOT_BASE_SIZE - 1) / SLOT_BASE_SIZE) - 1);
            idx = (idx < 0) ? 0 : (idx >= MEMORY_POOL_NUM ? MEMORY_POOL_NUM - 1 : idx);
            return getMemoryPool(idx).allocate();
        }
        static void freeMemory(void* ptr, size_t size) {
            if (!ptr) return;
            if (size <= 0 || size > MAX_SLOT_SIZE) {
                operator delete(ptr);
                return;
            }
            int idx = static_cast<int>(((size + SLOT_BASE_SIZE - 1) / SLOT_BASE_SIZE) - 1);
            idx = (idx < 0) ? 0 : (idx >= MEMORY_POOL_NUM ? MEMORY_POOL_NUM - 1 : idx);
            getMemoryPool(idx).deallocate(ptr);
        }

        template<typename T, typename... Args>
        friend T* newElement(Args&&... args);
        template<typename T>
        friend void deleteElement(T* p);
    };
    
    // 修复2：模板函数（用户数据与Slot元数据分离）
    template<typename T, typename... Args>
    T* newElement(Args&&... args) {
        size_t userSize = sizeof(T);
        // 从内存池获取Slot的用户数据区地址
        void* userPtr = HashBucket::useMemory(userSize);
        if (userPtr == nullptr) {
            return nullptr;
        }
        // 在用户数据区构造对象
        return new(userPtr) T(std::forward<Args>(args)...);
    }

    template<typename T>
    void deleteElement(T* p) {
        if (p == nullptr) return;
        p->~T();  // 析构对象
        HashBucket::freeMemory(p, sizeof(T));
    }
}  // namespace memoryPool