cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(klogger)
find_package(fmt CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/logger.cpp)
add_library(klog STATIC src/logger.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_include_directories(klog PRIVATE "src")
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
target_link_libraries(klog PUBLIC fmt::fmt)

