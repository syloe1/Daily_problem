#include "MemoryPool.h"
#include <cstdlib>
#include <cstring>

namespace memoryPool {
    // 构造函数
    MemoryPool::MemoryPool(size_t BlockSize)
        : BlockSize_(BlockSize),
          SlotMetaSize_(sizeof(Slot::next)),
          SlotTotalSize_(sizeof(Slot)),
          UserDataSize_(0),
          firstBlock_(nullptr),
          curSlot_(nullptr),
          freeList_(nullptr),
          lastSlot_(nullptr) {}

    // 析构函数
    MemoryPool::~MemoryPool() {
        Slot* cur = firstBlock_;
        while (cur != nullptr) {
            Slot* nxt = cur->next.load(std::memory_order_seq_cst);
            // 释放当前块（确保是operator new分配的合法地址）
            operator delete(reinterpret_cast<void*>(cur));
            cur = nxt;
        }
    }

    // 初始化内存池（设置用户数据大小）
    void MemoryPool::init(size_t userSize) {
        assert(userSize > 0 && userSize <= MAX_SLOT_SIZE);
        UserDataSize_ = userSize;
        // 重置所有状态
        firstBlock_ = nullptr;
        curSlot_ = nullptr;
        freeList_.store(nullptr, std::memory_order_seq_cst);
        lastSlot_ = nullptr;
    }

    // 无锁队列入队（将Slot加入空闲链表）
    bool MemoryPool::pushFreeList(Slot* slot) {
        assert(slot != nullptr);
        // 检查Slot地址是否对齐（避免非法指针）
        assert(reinterpret_cast<size_t>(slot) % ATOMIC_PTR_SIZE == 0 && "Slot address not aligned");
        
        while (true) {
            Slot* oldHead = freeList_.load(std::memory_order_acquire);
            // 设置新节点的next为旧表头
            slot->next.store(oldHead, std::memory_order_release);
            // CAS交换：成功则入队完成
            if (freeList_.compare_exchange_weak(
                    oldHead, slot,
                    std::memory_order_release,
                    std::memory_order_relaxed)) {
                return true;
            }
            // 失败则重试（可添加yield优化，此处省略以简化）
        }
    }

    // 无锁队列出队（从空闲链表取Slot）
    Slot* MemoryPool::popFreeList() {
        while (true) {
            Slot* oldHead = freeList_.load(std::memory_order_acquire);
            if (oldHead == nullptr) {
                return nullptr;  // 空闲链表为空
            }

            // 读取旧表头的next指针
            Slot* newHead = oldHead->next.load(std::memory_order_acquire);
            // 二次检查：确保oldHead仍在表头（避免ABA问题变种）
            if (freeList_.load(std::memory_order_acquire) != oldHead) {
                continue;
            }

            // CAS交换：成功则出队完成
            if (freeList_.compare_exchange_weak(
                    oldHead, newHead,
                    std::memory_order_acquire,
                    std::memory_order_relaxed)) {
                return oldHead;
            }
            // 失败则重试
        }
    }

    // 内存分配（核心函数，修复语法和作用域错误）
    void* MemoryPool::allocate() {
        // 1. 优先从无锁空闲链表获取
        Slot* slot = popFreeList();
        if (slot != nullptr) {
            assert(UserDataSize_ <= MAX_SLOT_SIZE && "User data size exceeds limit");
            return slot->userData;  // 返回用户数据区地址
        }

        // 2. 加锁分配新内存块
        std::lock_guard<std::mutex> lock(mutexForBlock_);
        
        // 检查当前块是否用尽，需分配新块
        if (curSlot_ == nullptr || curSlot_ > lastSlot_) {
            allocateNewBlock();
        }
        // 极端情况：新块分配失败
        if (curSlot_ == nullptr || curSlot_ > lastSlot_) {
            return nullptr;
        }

        // 3. 取当前可用Slot，更新curSlot_（修复：resSlot在作用域内声明）
        Slot* resSlot = curSlot_;  // 正确位置：在使用前声明，且在加锁块内
        curSlot_ = reinterpret_cast<Slot*>(
            reinterpret_cast<char*>(curSlot_) + SlotTotalSize_
        );

        // 4. 返回用户数据区地址
        return resSlot->userData;
    }

    // 内存释放（反推Slot地址，加入空闲链表）
    void MemoryPool::deallocate(void* userPtr) {
        if (userPtr == nullptr) return;

        // 反推Slot元数据地址（利用offsetof获取userData在Slot中的偏移）
        size_t slotOffset = offsetof(Slot, userData);
        Slot* slot = reinterpret_cast<Slot*>(
            reinterpret_cast<char*>(userPtr) - slotOffset
        );

        // 合法性检查：Slot地址对齐 + 在合法块内
        assert(reinterpret_cast<size_t>(slot) % ATOMIC_PTR_SIZE == 0 && "Slot address not aligned");
        size_t blockAddr = reinterpret_cast<size_t>(slot) & ~(BlockSize_ - 1);  // 块起始地址
        assert(blockAddr + BlockSize_ > reinterpret_cast<size_t>(slot) && "Slot not in any block");

        // 重置next指针（避免用户数据污染）
        slot->next.store(nullptr, std::memory_order_release);

        // 加入空闲链表
        pushFreeList(slot);
    }
    
    // 分配新内存块（修复块内Slot计数逻辑）
    void MemoryPool::allocateNewBlock() {
        // 1. 分配新块（大小为BlockSize_）
        void* newBlock = operator new(BlockSize_);
        assert(newBlock != nullptr && "Memory allocation failed");
        // 2. 清零内存块，确保所有Slot的next指针初始为nullptr
        memset(newBlock, 0, BlockSize_);

        // 3. 计算块内可容纳的Slot数量（避免越界）
        size_t slotCount = BlockSize_ / SlotTotalSize_;
        assert(slotCount >= 1 && "Block size too small for at least one Slot");

        // 4. 将新块加入块链表（头插法）
        Slot* blockHead = reinterpret_cast<Slot*>(newBlock);
        blockHead->next.store(firstBlock_, std::memory_order_release);
        firstBlock_ = blockHead;

        // 5. 计算块内第一个和最后一个Slot的地址
        Slot* firstSlot = blockHead;
        lastSlot_ = reinterpret_cast<Slot*>(
            reinterpret_cast<char*>(newBlock) + (slotCount - 1) * SlotTotalSize_
        );

        // 6. 设置当前可用Slot
        curSlot_ = firstSlot;
    }

    // 内存对齐计算（保持不变）
    size_t MemoryPool::padPointer(char* p, size_t align) {
        size_t addr = reinterpret_cast<size_t>(p);
        size_t remainder = addr % align;
        return (remainder == 0) ? 0 : (align - remainder);
    }

    // 初始化所有内存池（HashBucket静态函数）
    void HashBucket::initMemoryPool() {
        for (int i = 0; i < MEMORY_POOL_NUM; ++i) {
            size_t userSize = (i + 1) * SLOT_BASE_SIZE;
            getMemoryPool(i).init(userSize);
        }
    }

    // 获取内存池单例（HashBucket静态函数）
    MemoryPool& HashBucket::getMemoryPool(int idx) {
        assert(idx >= 0 && idx < MEMORY_POOL_NUM);
        static MemoryPool memoryPool[MEMORY_POOL_NUM];
        return memoryPool[idx];
    }
}  // namespace memoryPool