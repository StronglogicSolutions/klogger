cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)
project(klogger)

add_definitions(-DFMT_HEADER_ONLY)
set(SPDLIB "${CMAKE_CURRENT_SOURCE_DIR}/src/libspdlog.a")
add_executable(${PROJECT_NAME} src/main.cpp src/logger.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_libraries(${PROJECT_NAME} PRIVATE ${SPDLIB})
