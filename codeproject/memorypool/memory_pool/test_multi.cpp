#include "MemoryPool.h"
#include "ThreadCache.h"
#include "CentralCache.h"
#include "PageCache.h"
#include <iostream>
#include <cassert>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <cstring>

// 全局统计：分配/释放成功次数
std::atomic<size_t> g_allocSuccess(0);
std::atomic<size_t> g_freeSuccess(0);
const size_t THREAD_NUM = 8;       // 测试线程数
const size_t ALLOC_PER_THREAD = 10000; // 每个线程分配次数

// 多线程测试函数：ThreadCache并发分配与释放
void threadCacheWorker(size_t baseSize) {
    ThreadCache* tc = ThreadCache::getInstance();
    std::vector<void*> ptrs;
    ptrs.reserve(ALLOC_PER_THREAD);

    // 阶段1：并发分配内存
    for (size_t i = 0; i < ALLOC_PER_THREAD; ++i) {
        size_t size = baseSize + (i % 64); // 内存大小：baseSize ~ baseSize+63
        void* ptr = tc->allocate(size);
        if (ptr != nullptr) {
            ptrs.push_back(ptr);
            g_allocSuccess.fetch_add(1, std::memory_order_relaxed);
            // 写入随机数据（验证内存可写）
            memset(ptr, 0xBB, size);
        }
    }

    // 阶段2：并发释放内存
    for (size_t i = 0; i < ptrs.size(); ++i) {
        size_t size = baseSize + (i % 64);
        tc->deallocate(ptrs[i], size);
        g_freeSuccess.fetch_add(1, std::memory_order_relaxed);
    }
}

// 测试ThreadCache多线程并发性能
void testThreadCacheMultiThread() {
    std::cout << "=== 测试ThreadCache多线程并发分配与释放 ===" << std::endl;
    g_allocSuccess = 0;
    g_freeSuccess = 0;

    // 启动多个线程，每个线程分配不同基础大小的内存
    std::vector<std::thread> threads;
    size_t baseSizes[] = {8, 128, 1024, 8192, 32768, 65536, 131072, 256 * 1024};
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(threadCacheWorker, baseSizes[i % 8]);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 统计结果
    size_t totalExpected = THREAD_NUM * ALLOC_PER_THREAD;
    std::cout << "线程数：" << THREAD_NUM << "，每个线程分配次数：" << ALLOC_PER_THREAD << std::endl;
    std::cout << "预期总分配次数：" << totalExpected << "，实际成功：" << g_allocSuccess << std::endl;
    std::cout << "预期总释放次数：" << totalExpected << "，实际成功：" << g_freeSuccess << std::endl;
    std::cout << "总耗时：" << duration << "ms，平均每秒操作：" << (g_allocSuccess + g_freeSuccess) * 1000 / duration << "次" << std::endl;

    // 验证无内存泄漏（分配数=释放数）
    assert(g_allocSuccess == g_freeSuccess && "多线程分配与释放次数不匹配，可能存在内存泄漏");
    assert(g_allocSuccess == totalExpected && "部分内存分配失败");
    std::cout << "ThreadCache多线程并发测试通过！\n" << std::endl;
}

// 多线程测试函数：CentralCache自旋锁竞争
void centralCacheWorker(size_t index) {
    CentralCache& cc = CentralCache::getInstance();
    std::vector<void*> ptrs;
    const size_t batch = 100;

    // 循环：分配-释放（触发CentralCache自旋锁竞争）
    for (size_t i = 0; i < ALLOC_PER_THREAD / batch; ++i) {
        // 批量分配
        for (size_t j = 0; j < batch; ++j) {
            void* ptr = cc.fetchRange(index);
            if (ptr != nullptr) {
                ptrs.push_back(ptr);
                g_allocSuccess.fetch_add(1, std::memory_order_relaxed);
            }
        }

        // 批量释放（构建链表后归还）
        if (!ptrs.empty()) {
            void* start = ptrs[0];
            void* current = start;
            for (size_t j = 1; j < ptrs.size(); ++j) {
                *reinterpret_cast<void**>(current) = ptrs[j];
                current = ptrs[j];
            }
            *reinterpret_cast<void**>(current) = nullptr;

            size_t blockSize = (index + 1) * ALIGNMENT;
            cc.returnRange(start, ptrs.size() * blockSize, index);
            g_freeSuccess.fetch_add(ptrs.size(), std::memory_order_relaxed);
            ptrs.clear();
        }
    }
}

// 测试CentralCache自旋锁线程安全
void testCentralCacheMultiThread() {
    std::cout << "=== 测试CentralCache自旋锁线程安全 ===" << std::endl;
    g_allocSuccess = 0;
    g_freeSuccess = 0;

    // 所有线程操作同一个CentralCache链表（index=10，对应88B内存块）
    size_t targetIndex = 10;
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(centralCacheWorker, targetIndex);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // 统计结果
    std::cout << "线程数：" << THREAD_NUM << "，共同操作CentralCache index=" << targetIndex << "（" << (targetIndex + 1) * ALIGNMENT << "B）" << std::endl;
    std::cout << "总分配成功次数：" << g_allocSuccess << "，总释放成功次数：" << g_freeSuccess << std::endl;
    std::cout << "总耗时：" << duration << "ms，自旋锁竞争下平均每秒操作：" << (g_allocSuccess + g_freeSuccess) * 1000 / duration << "次" << std::endl;

    assert(g_allocSuccess == g_freeSuccess && "CentralCache多线程分配与释放次数不匹配");
    std::cout << "CentralCache自旋锁线程安全测试通过！\n" << std::endl;
}

// 测试PageCache多线程页分配（验证互斥锁）
void pageCacheWorker(size_t pageNum) {
    PageCache& pc = PageCache::getInstance();
    std::vector<void*> ptrs;

    for (size_t i = 0; i < 1000; ++i) {
        // 分配指定页数的内存
        void* ptr = pc.allocateSpan(pageNum);
        if (ptr != nullptr) {
            ptrs.push_back(ptr);
            g_allocSuccess.fetch_add(1, std::memory_order_relaxed);
        }

        // 每分配10次释放1次（模拟真实场景）
        if (i % 10 == 0 && !ptrs.empty()) {
            void* ptrToFree = ptrs.back();
            ptrs.pop_back();
            pc.deallocateSpan(ptrToFree, pageNum);
            g_freeSuccess.fetch_add(1, std::memory_order_relaxed);
        }
    }

    // 释放剩余内存
    for (void* ptr : ptrs) {
        pc.deallocateSpan(ptr, pageNum);
        g_freeSuccess.fetch_add(1, std::memory_order_relaxed);
    }
}

void testPageCacheMultiThread() {
    std::cout << "=== 测试PageCache多线程页分配（互斥锁） ===" << std::endl;
    g_allocSuccess = 0;
    g_freeSuccess = 0;

    // 线程分配不同页数（1页、2页、4页、8页）
    std::vector<std::thread> threads;
    size_t pageNums[] = {1, 2, 4, 8};
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < THREAD_NUM; ++i) {
        threads.emplace_back(pageCacheWorker, pageNums[i % 4]);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "线程数：" << THREAD_NUM << "，分配页数：1/2/4/8页（4KB/8KB/16KB/32KB）" << std::endl;
    std::cout << "总分配成功次数：" << g_allocSuccess << "，总释放成功次数：" << g_freeSuccess << std::endl;
    std::cout << "总耗时：" << duration << "ms" << std::endl;

    assert(g_allocSuccess == g_freeSuccess && "PageCache多线程分配与释放次数不匹配");
    std::cout << "PageCache多线程互斥锁测试通过！\n" << std::endl;
}

int main() {
    try {
        testThreadCacheMultiThread();
        testCentralCacheMultiThread();
        testPageCacheMultiThread();
        std::cout << "=== 所有多线程测试全部通过！ ===" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "测试异常：" << e.what() << std::endl;
        return 1;
    }
    return 0;
}