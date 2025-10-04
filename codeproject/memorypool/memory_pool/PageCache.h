// PageCache.h
#include <cstddef>
#include <map>
#include <mutex>

static const size_t PAGE_SIZE = 4096;

class PageCache {
public:
    static PageCache& getInstance() {
        static PageCache instance;
        return instance;
    }

    void* allocateSpan(size_t numPages);
    void deallocateSpan(void* ptr, size_t numPages);

private:
    PageCache() = default;
    ~PageCache();

    void* systemAlloc(size_t numPages);
    void systemFree(void* ptr, size_t numPages);

    struct Span {
        void* pageAddr = nullptr;
        size_t numPages = 0;
        Span* next = nullptr;
    };

    std::map<size_t, Span*> freeSpans_;
    std::map<void*, Span*> spanMap_;
    std::mutex mutex_;
};