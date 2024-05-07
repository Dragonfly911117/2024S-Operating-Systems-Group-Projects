#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int finish[NUMBER_OF_CUSTOMERS];
int request[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int release[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_mutex_t mutex;

int request_resources(int customer_num, int request[]) {
    // check safety
    pthread_mutex_lock(&mutex);
    int granted = 0;

    // copy shit
    int finish_copy[NUMBER_OF_CUSTOMERS];
    int available_copy[NUMBER_OF_RESOURCES];
    int allocation_copy[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    int need_copy[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        finish_copy[i] = finish[i];
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            allocation_copy[i][j] = allocation[i][j];
            need_copy[i][j] = need[i][j];
        }
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available_copy[i] = available[i];
    }
    // assume allocate
    finish_copy[customer_num] = 1;
    int is_pos = 1;
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available_copy[i] -= request[i];
        allocation_copy[customer_num][i] += request[i];
        need_copy[customer_num][i] -= request[i];
        if (need_copy[customer_num][i] > 0) {
            finish_copy[customer_num] = 0;
        }
        if (available_copy[i] < 0) {
            is_pos = 0;
            break;
        }
    }
    if (!is_pos) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    if (finish_copy[customer_num]) {
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available_copy[i] += allocation_copy[customer_num][i];
            allocation_copy[customer_num][i] = 0;
        }
    }
    pthread_mutex_unlock(&mutex);

    // check safety
    int found = 1;
    while (found) {
        found = 0;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish_copy[i]) {
                found = 1;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need_copy[i][j] > available_copy[j]) {
                        found = 0;
                        break;
                    }
                }
                if (found) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        available_copy[j] += allocation_copy[i][j];
                        finish_copy[i] = 1;
                    }
                }
            }
        }
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        if (!finish_copy[i]) {
            return 0;
        }
    }
    return 1;
}

void release_resources(int customer_num, int release[]) {
    // release resources
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        // need[customer_num][i] += release[i];
    }
    pthread_mutex_unlock(&mutex);
}

void allocate_resources(int customer_num, int request[]) {
    // allocate resources
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }
    pthread_mutex_unlock(&mutex);
}

void* customer(void* p_id) {
    int id = *((int*)p_id);
    while (!finish[id]) {
        // request
        int is_request = 0;
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            if (request[id][i] > 0) {
                is_request = 1;
                break;
            }
        }
        if (is_request) {
            if (request_resources(id, request[id])) {
                allocate_resources(id, request[id]);
                printf("Customer %d allocated\n", id);
            } else {
                printf("Customer %d denied\n", id);
            }
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                request[id][i] = 0;
            }
        }

        // release
        int is_release = 0;
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            if (release[id][i] > 0) {
                is_release = 1;
                break;
            }
        }
        if (is_release) {
            release_resources(id, release[id]);
            printf("Customer %d released\n", id);
            pthread_mutex_lock(&mutex);
            finish[id] = 1;
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                release[id][i] = 0;
                if (need[id][i] > 0 || allocation[id][i] > 0) {
                    finish[id] = 0;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    printf("Customer %d finished\n", id);
    pthread_exit(0);
}

int main(int argc, char** argv) {
    int all_finished = 0;

    // init
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        finish[i] = 0;
        available[i] = atoi(argv[i + 1]);
    }
    FILE* file = fopen("input.txt", "r");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            fscanf(file, "%d,", &maximum[i][j]);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
            request[i][j] = 0;
            release[i][j] = 0;
        }
    }
    fclose(file);

    // create threads
    pthread_t threads[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        int* p_id = (int*)malloc(sizeof(int));
        *p_id = i;
        pthread_create(&threads[i], NULL, customer, p_id);
    }

    while (!all_finished) {
        // take CLI input for request
        char cmd[3];
        memset(cmd, 0, 3);
        scanf("%s", cmd);

        // request
        if (cmd[1] == 'Q') {
            // input
            int id;
            scanf("%d", &id);
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                scanf("%d", &request[id][i]);
            }
        }

        // release
        if (cmd[1] == 'L') {
            // input
            int id;
            scanf("%d", &id);
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                scanf("%d", &release[id][i]);
            }
        }

        // print available
        if (cmd[0] == '*') {
            printf("Available: ");
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                printf("%d ", available[i]);
            }
            printf("\n");

            printf("Maximum: \n");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                printf("Customer %d: ", i);
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    printf("%d ", maximum[i][j]);
                }
                printf("\n");
            }
            printf("\n");

            printf("Allocation: \n");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                printf("Customer %d: ", i);
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    printf("%d ", allocation[i][j]);
                }
                printf("\n");
            }
            printf("\n");

            printf("Need: \n");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                printf("Customer %d: ", i);
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    printf("%d ", need[i][j]);
                }
                printf("\n");
            }
            printf("\n");

            printf("Finish: ");
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                printf("%d ", finish[i]);
            }
            printf("\n");
        }

        // check finish
        all_finished = 1;
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                all_finished = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    // join threads
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("All customers finished\n");
    return 0;
}
