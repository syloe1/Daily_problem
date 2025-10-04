void* fetchRange(size_t index) {
private:
    std::array<std::atomic<void*>, FREE_LIST_SIZE> centralFreeList_;
    std::array<atomic_flag, FREE_LIST_SIZE> locks_;
}