cmake_minimum_required(VERSION 3.15)
project(CoinChange2)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3")

set(PROJECT_SOURCE_DIR src)

add_executable(CoinChange2 main.cpp)