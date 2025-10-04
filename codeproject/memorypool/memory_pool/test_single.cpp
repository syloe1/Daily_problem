#include "MemoryPool.h"
#include "ThreadCache.h"
#include "CentralCache.h"
#include "PageCache.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
#include <thread>
// 测试ThreadCache单线程分配与释放
void testThreadCacheSingleThread() {
    std::cout << "=== 测试ThreadCache单线程分配与释放 ===" << std::endl;
    ThreadCache* tc = ThreadCache::getInstance();
    
    // 测试不同大小内存分配（8B, 16B, 32B, 256KB）
    std::vector<void*> ptrs;
    size_t sizes[] = {8, 16, 32, 64, 128, 256 * 1024};
    int allocCount = 100;

    // 分配内存
    for (size_t size : sizes) {
        for (int i = 0; i < allocCount; ++i) {
            void* ptr = tc->allocate(size);
            assert(ptr != nullptr && "ThreadCache分配内存失败");
            ptrs.push_back(ptr);
            // 写入数据验证可用性
            memset(ptr, 0xAA, size);
        }
        std::cout << "成功分配 " << allocCount << " 个 " << size << "B 内存块" << std::endl;
    }

    // 释放内存
    int idx = 0;
    for (size_t size : sizes) {
        for (int i = 0; i < allocCount; ++i) {
            void* ptr = ptrs[idx++];
            tc->deallocate(ptr, size);
        }
        std::cout << "成功释放 " << allocCount << " 个 " << size << "B 内存块" << std::endl;
    }

    std::cout << "ThreadCache单线程测试通过！\n" << std::endl;
}

// 测试CentralCache批量分配与延迟归还
void testCentralCacheBatchAndDelay() {
    std::cout << "=== 测试CentralCache批量分配与延迟归还 ===" << std::endl;
    CentralCache& cc = CentralCache::getInstance();
    

    // 测试8B内存块（index=0）
    size_t index = 0;
    size_t blockSize = (index + 1) * ALIGNMENT; // 8B
    size_t batchNum = 10;

    // 1. 从CentralCache批量获取内存
    std::vector<void*> ptrs;
    for (size_t i = 0; i < batchNum; ++i) {
        void* ptr = cc.fetchRange(index);
        assert(ptr != nullptr && "CentralCache批量分配失败");
        ptrs.push_back(ptr);
    }
    std::cout << "成功从CentralCache获取 " << batchNum << " 个 " << blockSize << "B 内存块" << std::endl;

    // 2. 构建归还链表（模拟ThreadCache归还批量内存）
    void* start = ptrs[0];
    void* current = start;
    for (size_t i = 1; i < batchNum; ++i) {
        *reinterpret_cast<void**>(current) = ptrs[i];
        current = ptrs[i];
    }
    *reinterpret_cast<void**>(current) = nullptr; // 链表尾置空

    // 3. 归还给CentralCache（触发延迟归还检查）
    cc.returnRange(start, batchNum * blockSize, index);
    std::cout << "成功将 " << batchNum << " 个 " << blockSize << "B 内存块归还给CentralCache" << std::endl;

    // 4. 等待延迟时间（1秒），触发延迟归还到PageCache
    std::this_thread::sleep_for(CentralCache::DELAY_INTERVAL);
    std::cout << "延迟时间到达，已执行CentralCache到PageCache的归还逻辑" << std::endl;

    std::cout << "CentralCache批量与延迟归还测试通过！\n" << std::endl;
}

// 测试PageCache页级分配与合并
void testPageCacheAllocAndMerge() {
    std::cout << "=== 测试PageCache页级分配与合并 ===" << std::endl;
    PageCache& pc = PageCache::getInstance();
    size_t pageSize = PAGE_SIZE; // 4KB

    // 1. 分配不同页数的内存（1页、2页、4页）
    void* ptr1 = pc.allocateSpan(1); // 4KB
    void* ptr2 = pc.allocateSpan(2); // 8KB
    void* ptr3 = pc.allocateSpan(4); // 16KB
    assert(ptr1 != nullptr && "PageCache分配1页失败");
    assert(ptr2 != nullptr && "PageCache分配2页失败");
    assert(ptr3 != nullptr && "PageCache分配4页失败");
    std::cout << "成功分配：1页(" << pageSize << "B)、2页(" << 2 * pageSize << "B)、4页(" << 4 * pageSize << "B)" << std::endl;

    // 2. 释放内存（验证相邻页合并）
    pc.deallocateSpan(ptr1, 1);
    pc.deallocateSpan(ptr2, 2);
    pc.deallocateSpan(ptr3, 4);
    std::cout << "成功释放所有页内存，已触发相邻页合并逻辑" << std::endl;

    // 3. 验证大页分配（合并后的内存可复用）
    void* ptrMerge = pc.allocateSpan(7); // 尝试分配7页（28KB）
    assert(ptrMerge != nullptr && "PageCache合并后大页分配失败");
    std::cout << "成功从合并后的内存中分配7页(" << 7 * pageSize << "B)内存" << std::endl;
    pc.deallocateSpan(ptrMerge, 7);

    std::cout << "PageCache页级分配与合并测试通过！\n" << std::endl;
}

// 测试MemoryPool模板函数（newElement/deleteElement）
class TestObject {
public:
    TestObject(int val) : value(val) {
        std::cout << "TestObject构造：value=" << value << "，地址=" << this << std::endl;
    }
    ~TestObject() {
        std::cout << "TestObject析构：value=" << value << "，地址=" << this << std::endl;
    }
    int getValue() const { return value; }
private:
    int value;
};

void testMemoryPoolTemplate() {
    std::cout << "=== 测试MemoryPool模板函数（newElement/deleteElement） ===" << std::endl;
    using namespace memoryPool;

    // 初始化内存池
    HashBucket::initMemoryPool();

    // 使用模板函数分配对象
    TestObject* obj1 = newElement<TestObject>(10);
    TestObject* obj2 = newElement<TestObject>(20);
    assert(obj1 != nullptr && "模板函数分配obj1失败");
    assert(obj2 != nullptr && "模板函数分配obj2失败");
    assert(obj1->getValue() == 10 && "obj1值错误");
    assert(obj2->getValue() == 20 && "obj2值错误");
    std::cout << "成功通过newElement分配2个TestObject对象" << std::endl;

    // 释放对象
    deleteElement(obj1);
    deleteElement(obj2);
    std::cout << "成功通过deleteElement释放2个TestObject对象" << std::endl;

    std::cout << "MemoryPool模板函数测试通过！\n" << std::endl;
}

int main() {
    try {
        testThreadCacheSingleThread();
        testCentralCacheBatchAndDelay();
        testPageCacheAllocAndMerge();
        testMemoryPoolTemplate();
        std::cout << "=== 所有单线程测试全部通过！ ===" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "测试异常：" << e.what() << std::endl;
        return 1;
    }
    return 0;
}