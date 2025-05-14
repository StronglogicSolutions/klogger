cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(klogger)
find_package(fmt CONFIG REQUIRED)

add_library(klog STATIC src/logger.cpp)
add_library(klogger::klog ALIAS klog)

target_include_directories(klog PRIVATE "src")
target_link_libraries(klog PUBLIC fmt::fmt)

option(KLOGGER_BUILD_EXECUTABLE "Build the klogger executable" OFF)

if(KLOGGER_BUILD_EXECUTABLE)
    add_executable(klogger src/main.cpp)
    target_link_libraries(klogger PRIVATE klogger::klog)
    target_include_directories(klogger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
