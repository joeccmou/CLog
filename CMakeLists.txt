# CMakeList.txt: Logging 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

set(This Logging)

project(${This} C CXX)


# TODO: 如有需要，请添加测试并安装目标。
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Headers 
    Logging.h
)

set(Sources 
    Logging.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})

add_subdirectory(test)