cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)

project(klogger)
set(G3LOG g3log)

add_executable(${PROJECT_NAME} src/main.cpp src/logger.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "src")

target_link_libraries(${PROJECT_NAME} PRIVATE ${G3LOG})

add_subdirectory(third_party/k3log)
