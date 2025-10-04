#pragma once
#include <cstddef>

constexpr size_t ALIGNMENT = 8;
constexpr size_t MAX_BYTES = 256 * 1024;
constexpr size_t FREE_LIST_SIZE = MAX_BYTES / ALIGNMENT;

class SizeClass {
public:
    static size_t getIndex(size_t bytes);
    static size_t roundUp(size_t bytes);
};