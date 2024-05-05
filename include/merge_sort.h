#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <stdlib.h>
#include <pthread.h>

#define SIZE 100
#define NUM_THREADS 20


typedef struct Node {
    int data;
    struct Node*next;
} node_t;


node_t *SortedMerge(node_t *a, node_t *b);

void FrontBackSplit(node_t *source,
                    node_t **frontRef, node_t **backRef);

void *MergeSort(void *arg);

void push(node_t **head_ref, int new_data);

#endif // MERGE_SORT_H
