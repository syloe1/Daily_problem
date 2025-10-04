#include <cstddef>       // 包含size_t定义
#include <atomic>        // 包含std::atomic定义
#include <array>         // 包含std::array定义
#include <chrono>        // 包含std::chrono定义
#include "Common.h"
struct SpanTracker{
    std::atomic<void*> spanAddr{nullptr};
    std::atomic<size_t> numPages{0};
    std::atomic<size_t> blockCount{0};
    std::atomic<size_t> freeCount{0};
};

class CentralCache {
public:
    static const std::chrono::milliseconds DELAY_INTERVAL;
    static CentralCache& getInstance() {
        static CentralCache instance;
        return instance;
    }
    void* fetchRange(size_t index);
    void returnRange(void* start, size_t size, size_t index);
private:
    CentralCache();
    void* fetchFromPageCache(size_t size);
    SpanTracker* getSpanTracker(void* blockAddr);
    void updateSpanFreeCount(SpanTracker* tracker, size_t newFreeBlocks, size_t index);
private:
    std::array<std::atomic<void*>, FREE_LIST_SIZE> centralFreeList_;
    std::array<std::atomic_flag, FREE_LIST_SIZE> locks_;  // 修复成员名
    std::array<SpanTracker, 1024> spanTrackers_;
    std::atomic<size_t> spanCount_{0};
    static const size_t MAX_DELAY_COUNT = 48;
    std::array<std::atomic<size_t>, FREE_LIST_SIZE> delayCounts_;  // 修复成员名
    std::array<std::chrono::steady_clock::time_point,  FREE_LIST_SIZE> lastReturnTimes_;  // 修复成员名
    
    bool shouldPerformDelayedReturn(size_t index, size_t currentCount, std::chrono::steady_clock::time_point currentTime );
    void performDelayedReturn(size_t index);
};