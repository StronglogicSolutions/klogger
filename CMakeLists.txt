cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)

project(klogger)

set(G3LOG "${CMAKE_CURRENT_SOURCE_DIR}/third_party/g3log/build/libg3log.a")

add_executable(${PROJECT_NAME} src/main.cpp src/logger.cpp)
add_definitions(-fpermissive)
target_include_directories(${PROJECT_NAME} PRIVATE "src")

target_link_libraries(${PROJECT_NAME} PRIVATE ${G3LOG})
