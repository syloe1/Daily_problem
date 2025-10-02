#include "MemoryPool.h"
#include <iostream>
#include <cstring>

// 测试用结构体
struct TestObj {
    int a;
    double b;
    TestObj(int a_, double b_) : a(a_), b(b_) {}
    ~TestObj() { std::cout << "TestObj destroyed\n"; }
};

int main() {
    // 初始化内存池
    memoryPool::HashBucket::initMemoryPool();
    std::cout << "Memory pool initialized\n";

    // 测试小对象分配（使用内存池）
    TestObj* obj1 = memoryPool::newElement<TestObj>(10, 3.14);
    if (obj1) {
        std::cout << "obj1: a=" << obj1->a << ", b=" << obj1->b << "\n";
    }

    // 测试大对象分配（直接使用operator new）
    char* bigData = memoryPool::newElement<char>(1024);  // 1024字节 > MAX_SLOT_SIZE(512)
    if (bigData) {
        strcpy(bigData, "Test big data allocation");
        std::cout << "bigData: " << bigData << "\n";
    }

    // 释放对象
    memoryPool::deleteElement(obj1);
    memoryPool::deleteElement(bigData);

    return 0;
}