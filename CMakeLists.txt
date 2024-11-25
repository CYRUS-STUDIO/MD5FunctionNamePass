cmake_minimum_required(VERSION 3.20)
project(MD5FunctionNamePass)

# 设置 LLVM 安装目录的路径，LLVM_DIR 指向 LLVM 的 cmake 配置目录。
set(LLVM_DIR "D:/Projects/llvm-project/build/lib/cmake/llvm")

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(LLVMConfig)

set(CMAKE_CXX_STANDARD 17)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# 添加自定义 Pass
add_library(MD5FunctionNamePass SHARED
        MD5FunctionNamePass.cpp
        hello.c
)

# 链接 LLVM 支持库
llvm_map_components_to_libnames(LLVM_LIBS core support)
target_link_libraries(MD5FunctionNamePass ${LLVM_LIBS})


if (WIN32)
    set_target_properties(MD5FunctionNamePass PROPERTIES LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/export.def")
endif ()
