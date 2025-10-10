cmake_minimum_required(VERSION 3.30.0)
project(Project)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(simple-gui)
add_subdirectory(sandbox)

if(MSVC)
    # MSVC编译器选项
    add_compile_options(/W4)
else()
    # GCC/Clang编译器选项
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()