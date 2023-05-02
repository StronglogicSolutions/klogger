cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)

project(klogger)

set(G3LOG "${CMAKE_CURRENT_SOURCE_DIR}/third_party/g3log/libg3log.a")
set(G3_SHARED_LIB OFF)
add_executable(${PROJECT_NAME} src/main.cpp src/logger.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "src")

target_link_libraries(${PROJECT_NAME} PRIVATE ${G3LOG})

add_subdirectory(third_party/g3log)
