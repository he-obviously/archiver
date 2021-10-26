cmake_minimum_required(VERSION 3.16)
project(huffman_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(huffman_1 main.cpp constants.h compress.h compress.cpp read.cpp read.h prefix_tree.h prefix_tree.cpp bin_write.h bin_write.cpp decompress.h decompress.cpp bin_read.h bin_read.cpp)
