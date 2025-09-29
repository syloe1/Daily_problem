#include <mutex>
#include <cassert>

namespace memoryPool {
    #define MEMORY_POOL_NUM 64
    #define SLOT_BASE_SIZE 8
    #define MAX_SLOT_SIZE 512

    struct Slot {
        Slot* next;
    };

    class MemoryPool {
    public:
        MemoryPool(size_t BlockSize = 4096);
        ~MemoryPool();

        void init(size_t);
        void* allocate();
        void deallocate(void*);
    private:
        void allocateNewBlock();
        size_t padPointer(char* p, size_t align);
    private:
        int BlockSize_;
        int SlotSize_;
        Slot* firstBlock_;  // 修正：添加下划线，与实现一致
        Slot* curSlot_;
        Slot* freeList_;
        Slot* lastSlot_;
        std::mutex mutexForFreeList_;
        std::mutex mutexForBlock_;  // 修正：原代码实现中是mutexForBlock_，头文件保持一致
    };

    class HashBucket {
    public:
        static void initMemoryPool();
        static MemoryPool& getMemoryPool(int idx);

        static void* useMemory(size_t size) {
            if (size <= 0) return nullptr;
            if (size > MAX_SLOT_SIZE) return operator new(size);
            return getMemoryPool(((size + 7) / SLOT_BASE_SIZE) - 1).allocate();
        }

        static void freeMemory(void* ptr, size_t size) {
            if (!ptr) return;
            if (size > MAX_SLOT_SIZE) {
                operator delete(ptr);
                return;
            }
            getMemoryPool(((size + 7) / SLOT_BASE_SIZE) - 1).deallocate(ptr);
        }

        template<typename T, typename... Args>
        friend T* newElement(Args&&... args);

        template<typename T>
        friend void deleteElement(T* p);
    };

    template<typename T, typename... Args>  // 修正：typename.. -> typename...
    T* newElement(Args&&... args) {
        T* p = reinterpret_cast<T*>(HashBucket::useMemory(sizeof(T)));  // 修正：先赋值
        if (p != nullptr) {  // 修正：单独判断
            new(p) T(std::forward<Args>(args)...);  // placement new
        }
        return p;
    }

    template<typename T>
    void deleteElement(T* p) {
        if (p) {
            p->~T();  // 显式调用析构函数
            HashBucket::freeMemory(reinterpret_cast<void*>(p), sizeof(T));
        }
    }
}
// namespace memoryPool  // 修正：拼写错误（mempool -> memoryPool）