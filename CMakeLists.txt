cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(klogger)

add_executable(${PROJECT_NAME} src/main.cpp src/logger.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
  "src"
)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)

