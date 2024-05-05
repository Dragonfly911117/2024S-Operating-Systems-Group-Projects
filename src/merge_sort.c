// Muti-threading version of merge_sort.c from https://www.geeksforgeeks.org/merge-sort-for-linked-list/
#include "../include/merge_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *MergeSort(void *arg) {
    node_t **headRef = (node_t **) arg;
    node_t *head = *headRef;
    node_t *a;
    node_t *b;

    if ((head == NULL) || (head->next == NULL)) {
        return NULL;
    }
    FrontBackSplit(head, &a, &b);

    pthread_t thread_a, thread_b;

    pthread_create(&thread_a, NULL, MergeSort, &a);
    pthread_create(&thread_b, NULL, MergeSort, &b);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    *headRef = SortedMerge(a, b);

    return NULL;
}

node_t *SortedMerge(node_t *a, node_t *b) {
    node_t *result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->data <= b->data) {
        result = a;
        result->next = SortedMerge(a->next, b);
    } else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return result;
}


void FrontBackSplit(node_t *source, node_t **frontRef, node_t **backRef) {
    node_t *fast;
    node_t *slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}


void printList(node_t *node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
}


void push(node_t **head_ref, int new_data) {
    node_t *new_node = (node_t *) malloc(sizeof(node_t));

    new_node->data = new_data;
    new_node->next = (*head_ref);

    (*head_ref) = new_node;
}

int main() {
    node_t *a = NULL;

    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        push(&a, rand() % SIZE);
    }

    printf("Unsorted Linked List is:\n");
    printList(a);

    pthread_t thread;
    pthread_create(&thread, NULL, MergeSort, &a);
    pthread_join(thread, NULL);

    printf("\nSorted Linked List is: \n");
    printList(a);
    printf("\n");

    return 0;
}
