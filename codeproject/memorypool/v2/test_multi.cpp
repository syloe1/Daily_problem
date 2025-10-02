#include "MemoryPool.h"
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

std::atomic<int> g_count(0);

void threadTask(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        int* p = memoryPool::newElement<int>(i);
        if (p != nullptr) {
            g_count.fetch_add(1, std::memory_order_relaxed);
            memoryPool::deleteElement(p);
        }
    }
}

int main() {
    memoryPool::HashBucket::initMemoryPool();
    const int THREAD_NUM = 8;
    const int ITER_PER_THREAD = 10000;
    std::vector<std::thread> threads;

    threads.reserve(THREAD_NUM);
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(threadTask, ITER_PER_THREAD);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Total operations: " << g_count << "\n";
    if (g_count == THREAD_NUM * ITER_PER_THREAD) {
        std::cout << "Thread safety test passed\n";
    } else {
        std::cout << "Thread safety test failed\n";
    }

    return 0;
}