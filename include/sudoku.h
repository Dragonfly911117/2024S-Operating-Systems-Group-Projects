#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define NUM_THREADS 27 // 9 rows + 9 columns + 9 subgrids


extern int sudoku[SIZE][SIZE];

typedef enum {
    ROW,
    COL,
    BLOCK
} type_t;
typedef struct {
    int index;
    type_t type;

} parameters;

void *validate(void *param);

#endif // SUDOKU_H