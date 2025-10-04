// Common.h
#ifndef COMMON_H
#define COMMON_H

#include <cstddef>

// 内存对齐粒度
#define ALIGNMENT 8
// 最大可管理的小块内存大小
// Common.h
constexpr size_t MAX_BYTES = 256 * 1024;  // 确保至少256KB
constexpr size_t FREE_LIST_SIZE = 20;     // 确保索引不会越界

// 计算内存大小对应的自由链表索引
static inline size_t getIndex(size_t bytes) {
    if (bytes <= ALIGNMENT) return 0;
    return (bytes + ALIGNMENT - 1) / ALIGNMENT - 1;
}

#endif // COMMON_H