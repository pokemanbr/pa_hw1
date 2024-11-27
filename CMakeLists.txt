cmake_minimum_required(VERSION 3.31)

project(ParallelQuicksort)

set(CMAKE_CXX_STANDARD 17)

find_package(TBB REQUIRED)

add_executable(parallel_quicksort sorts.cpp)

target_link_libraries(parallel_quicksort TBB::tbb)