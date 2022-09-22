cmake_minimum_required(VERSION 3.23)
project(ClangStaticAnalysis_Lib)

#===============================================================================
# 1. LOAD LLVM CONFIGURATION
#===============================================================================
# 设置有效的LLVM安装地址
set(LT_LLVM_INSTALL_DIR "/usr/local/opt/llvm@13" CACHE PATH "LLVM installation directory")

# 添加 LLVMConfig.cmake 路径到CMake
list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

# FIXME: This is a warkaround for #25. Remove once resolved and use
# find_package(LLVM 11.0.0 REQUIRED CONFIG) instead.
find_package(LLVM REQUIRED CONFIG)

# HelloWorld 需要LLVM头文件
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

#===============================================================================
# 2. LLVM-TUTOR BUILD CONFIGURATION
#===============================================================================
# LLVM基于Cpp14标准
set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

# LLVM 一般构建不需要 RTTI.
if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

#===============================================================================
# 3. ADD THE TARGET
#===============================================================================
add_library(HelloWorld SHARED library.cpp Analysis/SSALiveness.cpp Analysis/SSALiveness.h)

# 允许未定义符号在Darwin平台上共享Object (Linux平台上默认此操作)
target_link_libraries(HelloWorld
        "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")
