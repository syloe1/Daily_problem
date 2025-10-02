cmake_minimum_required(VERSION 3.10)
project(MemoryPoolTest)

# 设置C++标准为C++11，支持线程和移动语义等特性
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找线程库（多线程测试需要）
find_package(Threads REQUIRED)

# 收集所有源文件
set(SOURCE_FILES
    MemoryPool.cpp
    test.cpp
    test_multi.cpp
)

# 收集头文件（用于IDE显示）
set(HEADER_FILES
    MemoryPool.h
)

# 创建单线程测试可执行文件
add_executable(test_single test.cpp MemoryPool.cpp)
target_link_libraries(test_single PRIVATE Threads::Threads)

# 创建多线程测试可执行文件
add_executable(test_multi test_multi.cpp MemoryPool.cpp)
target_link_libraries(test_multi PRIVATE Threads::Threads)

# 可选：如果使用IDE（如VS Code、CLion），将头文件添加到项目中
source_group("Headers" FILES ${HEADER_FILES})
    