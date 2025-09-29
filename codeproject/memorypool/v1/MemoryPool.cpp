#include "MemoryPool.h"
#include <cstdlib>  // for operator new/delete

namespace memoryPool {
    MemoryPool::MemoryPool(size_t BlockSize)
        : BlockSize_(BlockSize)
    {}

    MemoryPool::~MemoryPool() {
        Slot* cur = firstBlock_;
        while (cur) {
            Slot* nxt = cur->next;
            operator delete(reinterpret_cast<void*>(cur));
            cur = nxt;
        }
    }

    void MemoryPool::init(size_t size) {
        assert(size > 0);
        SlotSize_ = size;
        firstBlock_ = nullptr;
        curSlot_ = nullptr;
        freeList_ = nullptr;
        lastSlot_ = nullptr;
    }

    void* MemoryPool::allocate() {
        // 优先从空闲列表分配
        if (freeList_ != nullptr) {
            std::lock_guard<std::mutex> lock(mutexForFreeList_);
            if (freeList_ != nullptr) {
                Slot* tmp = freeList_;
                freeList_ = freeList_->next;
                return tmp;
            }
        }

        // 从新块分配
        Slot* tmp;
        std::lock_guard<std::mutex> lock(mutexForBlock_);  // 修正：变量名一致性
        if (curSlot_ >= lastSlot_) {  // 修正：curSLot_ -> curSlot_
            allocateNewBlock();
        }
        tmp = curSlot_;
        // 修正：计算下一个slot的指针（按SlotSize_偏移）
        curSlot_ = reinterpret_cast<Slot*>(reinterpret_cast<char*>(curSlot_) + SlotSize_);
        return tmp;
    }

    void MemoryPool::deallocate(void* ptr) {  // 修正：MempryPool -> MemoryPool
        // 修正：删除未定义的pre判断，直接加入空闲列表
        std::lock_guard<std::mutex> lock(mutexForFreeList_);
        reinterpret_cast<Slot*>(ptr)->next = freeList_;
        freeList_ = reinterpret_cast<Slot*>(ptr);  // 修正：reinterpre_cast -> reinterpret_cast
    }

    void MemoryPool::allocateNewBlock() {
        void* newBlock = operator new(BlockSize_);  // 修正：NewBlock -> newBlock（小写规范）
        // 将新块加入块链表头部
        reinterpret_cast<Slot*>(newBlock)->next = firstBlock_;
        firstBlock_ = reinterpret_cast<Slot*>(newBlock);

        // 计算块内可用内存起始位置（跳过块头部的next指针）
        char* body = reinterpret_cast<char*>(newBlock) + sizeof(Slot*);  // 修正：newBLock -> newBlock
        size_t paddingSize = padPointer(body, SlotSize_);  // 对齐调整
        curSlot_ = reinterpret_cast<Slot*>(body + paddingSize);

        // 计算块内最后一个slot的位置
        lastSlot_ = reinterpret_cast<Slot*>(
            reinterpret_cast<size_t>(newBlock) + BlockSize_ - SlotSize_
        );  // 修正：reiniterpret_cast -> reinterpret_cast，去掉+1（避免越界）
    }

    size_t MemoryPool::padPointer(char* p, size_t align) {
        return (align - reinterpret_cast<size_t>(p) % align) % align;
    }

    void HashBucket::initMemoryPool() {
        for (int i = 0; i < MEMORY_POOL_NUM; i++) {
            getMemoryPool(i).init((i + 1) * SLOT_BASE_SIZE);
        }
    }

    // 单例模式：每个索引对应一个内存池实例
    MemoryPool& HashBucket::getMemoryPool(int idx) {
        static MemoryPool memoryPool[MEMORY_POOL_NUM];
        return memoryPool[idx];
    }
}