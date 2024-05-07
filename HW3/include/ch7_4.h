#ifndef HW3_CH7_4_H
#define HW3_CH7_4_H

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for sleep
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>


#define BUFFER_SIZE 5
#define STATUS_CHECK_INTERVAL 3


typedef int buffer_item;

extern buffer_item buffer[BUFFER_SIZE];
extern pthread_mutex_t mutex;
extern sem_t empty;
extern sem_t full;

int myRand(void);

_Noreturn void* statusCheck(void*);

void print_info();

int insert_item(buffer_item item);

int remove_item(buffer_item* item);

void* producer(void*);

void* consumer(void*);

#endif //HW3_CH7_4_H
