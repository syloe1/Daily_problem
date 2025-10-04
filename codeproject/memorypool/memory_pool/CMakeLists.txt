cmake_minimum_required(VERSION 3.10)
project(MemoryPoolTest)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项（根据需要调整）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

# 收集所有源文件
set(SOURCES
    MemoryPool.cpp
    CentralCache.cpp
    PageCache.cpp
    ThreadCache.cpp
)

# 收集所有头文件（用于 IDE 识别）
set(HEADERS
    MemoryPool.h
    CentralCache.h
    Central.h
    PageCache.h
    ThreadCache.h
    Common.h
    SizeClass.h
)

# 创建静态库（将内存池相关代码打包）
add_library(memory_pool STATIC ${SOURCES} ${HEADERS})

# 链接 pthread 库（用于多线程支持）
target_link_libraries(memory_pool pthread)

# 编译单线程测试程序
add_executable(test_single test_single.cpp)
target_link_libraries(test_single memory_pool)

# 编译多线程测试程序
add_executable(test_multi test_multi.cpp)
target_link_libraries(test_multi memory_pool)
    