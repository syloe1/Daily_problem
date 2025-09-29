#include "MemoryPool.h"
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

std::atomic<int> g_count(0);  // 原子变量计数，验证并发安全性

// 线程任务：分配并释放对象
void threadTask(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        // 分配小对象
        int* p = memoryPool::newElement<int>(i);
        if (p) {
            g_count++;
            memoryPool::deleteElement(p);
        }
    }
}

int main() {
    memoryPool::HashBucket::initMemoryPool();
    const int THREAD_NUM = 8;
    const int ITER_PER_THREAD = 10000;
    std::vector<std::thread> threads;

    // 启动多线程
    for (int i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(threadTask, ITER_PER_THREAD);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    // 验证计数是否正确（应为 THREAD_NUM * ITER_PER_THREAD）
    std::cout << "Total operations: " << g_count << "\n";
    if (g_count == THREAD_NUM * ITER_PER_THREAD) {
        std::cout << "Thread safety test passed\n";
    } else {
        std::cout << "Thread safety test failed\n";
    }

    return 0;
}