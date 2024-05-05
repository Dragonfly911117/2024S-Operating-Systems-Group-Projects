#include "../include/sudoku.h"

int sudoku[SIZE][SIZE] = {
        6, 3, 7, 1, 5, 9, 2, 4, 8,
        2, 8, 1, 3, 4, 7, 9, 5, 6,
        5, 9, 4, 2, 6, 8, 1, 7, 3,
        8, 1, 6, 5, 9, 2, 7, 3, 4,
        4, 2, 9, 7, 8, 3, 6, 1, 5,
        3, 7, 5, 6, 1, 4, 8, 2, 9,
        7, 4, 2, 9, 3, 6, 5, 8, 1,
        9, 5, 3, 8, 2, 1, 4, 6, 7,
        1, 6, 8, 4, 7, 5, 3, 9, 2
};

int main() {
    pthread_t threads[NUM_THREADS];
    parameters *params[NUM_THREADS];

    for (int i = 0; i < SIZE; i++) {
        // Create threads to validate rows, columns and blocks
        for (int j = 0; j < 3; j++) {
            params[i * 3 + j] = (parameters *) malloc(sizeof(parameters));
            params[i * 3 + j]->index = i;
            params[i * 3 + j]->type = j;
            pthread_create(&threads[i * 3 + j], NULL, validate, (void *) params[i * 3 + j]);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        void *result;
        pthread_join(threads[i], &result);
        if ((int) result == 0) {
            printf("Invalid Sudoku solution!\n");
            return 1;
        }
    }

    printf("Valid Sudoku solution!\n");

    return 0;
}

void *validate(void *param) {
    parameters *params = (parameters *) param;
    int index = params->index;
    int used[SIZE] = {0};
    for (int i = 0; i < SIZE; i++) {
        int num;
        if (params->type == ROW) {
            num = sudoku[index][i];
        } else if (params->type == COL) {
            num = sudoku[i][index];
        } else { // BLOCK
            num = sudoku[index / 3 * 3 + i / 3][index % 3 * 3 + i % 3];
        }
        if (num < 1 || num > SIZE || used[num - 1]) {
            fprintf(stderr, "Invalid Sudoku solution in %s %d\n",
                    params->type == ROW ? "row" : params->type == COL ? "column" : "block", index + 1);
            pthread_exit((void *) 0);
        }
        used[num - 1] = 1;
    }
    pthread_exit((void *) 1);
    return NULL;
}
