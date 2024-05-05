cmake_minimum_required(VERSION 3.25)

project(HW3)

set(CMAKE_C_STANDARD 17)

set(Ch8_FILES
    src/ch8.c
)

set(Ch7_FILES
    src/ch7_4.c
    include/ch7_4.h
)

add_executable(CH7_4 ${Ch7_FILES})
add_executable(CH8 ${Ch8_FILES})

target_compile_options(CH7_4 PRIVATE -DUSE_PTHREAD_EXIT)
target_compile_options(CH8 PRIVATE -DUSE_PTHREAD_EXIT)
