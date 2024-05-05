#include "../include/ch7_4.h"


int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sleep time> <# of producers> <# of consumers>\n", argv[0]);
        return 1;
    }

    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = -1;
    }

    for (int i = 0; i < num_producers; i++)
        pthread_create(&producers[i], NULL, producer, NULL);

    for (int i = 0; i < num_consumers; i++)
        pthread_create(&consumers[i], NULL, consumer, NULL);

    pthread_create(NULL, NULL, statusCheck, NULL);

    sleep(sleep_time);

    for (int i = 0; i < num_producers; i++)
        pthread_cancel(producers[i]);
    for (int i = 0; i < num_consumers; i++)
        pthread_cancel(consumers[i]);

    pthread_mutex_destroy(&mutex);
    print_info();
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;


int myRand(void) {
    static unsigned long long next = 1;
    static const unsigned long a = 1103515245;
    static const unsigned long c = 12345;
    static const unsigned long m = 2147483648;
    next = (a * next + c) % m;
    return (int) (next);
}

void print_info() {
    char output[256];
    int full_temp;
    int empty_temp;

    sem_getvalue(&empty, &empty_temp);
    sem_getvalue(&full, &full_temp);
    sprintf(output, "==========================================================\n");
    sprintf(output + strlen(output), "Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++)
        sprintf(output + strlen(output), "%d ", buffer[i]);
    sprintf(output + strlen(output), "\n");
    sprintf(output + strlen(output), "full: %d\n", abs(full_temp));
    sprintf(output + strlen(output), "empty: %d\n", abs(empty_temp));
    sprintf(output + strlen(output), "==========================================================\n");

    printf("%s", output);
}

_Noreturn void* statusCheck(void*) {
    while (1) {
        print_info();
        sleep(STATUS_CHECK_INTERVAL);
    }
}

int insert_item(buffer_item item) {
    static int in = 0;
    if (buffer[in] != -1) {
        return -1;
    }

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    return 0;
}

int remove_item(buffer_item* item) {
    static int out = 0;
    if (buffer[out] == -1) {
        return -1;
    }
    *item = buffer[out];
    buffer[out] = -1;
    out = (out + 1) % BUFFER_SIZE;
    return 0;
}

void* producer(void*) {
    buffer_item item;
    while (1) {
        sleep(myRand() % 2 + 1);
        item = myRand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        int ret = insert_item(item);
        pthread_mutex_unlock(&mutex);
        if (ret == -1) {
            fprintf(stderr, "Error: Producer failed to insert item\n");
        } else {
            printf("Producer produced %d\n", item);
            sem_post(&full);
        }
    }
}

void* consumer(void*) {
    buffer_item item;
    while (1) {
        sleep(myRand() % 5 + 1);
        while (sem_wait(&full) != 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("sem_wait");
            exit(1);
        }
        pthread_mutex_lock(&mutex);

        int ret = remove_item(&item);
        pthread_mutex_unlock(&mutex);
        if (ret == -1) {
            fprintf(stderr, "Error: Consumer failed to remove item\n");
        } else {
            printf("Consumer consumed %d\n", item);
            sem_post(&empty);
        }
    }
}

