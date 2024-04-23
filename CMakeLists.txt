cmake_minimum_required(VERSION 3.25)

project(2024S-Operating-System-Group)

set(CMAKE_C_STANDARD  17)

set(SUDOKU_FILES
        include/sudoku.h
        src/sudoku.c
)

add_executable(sudoku_verifier ${SUDOKU_FILES})
target_compile_options(sudoku_verifier PRIVATE -DUSE_PTHREAD_EXIT)

set(MERGE_SORT_FILES
        include/merge_sort.h
        src/merge_sort.c
)

add_executable(merge_sort ${MERGE_SORT_FILES})

add_executable(processes_scheduler src/ProcessScheduler.c)
