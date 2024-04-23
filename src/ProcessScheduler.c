#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int priority; // Only applicable for priority-based and priority with round-robin algorithms
    int remaining_time; // Used for algorithms like SJF and Round-Robin
} Process;

void FCFS(Process processes[], int n) {
    printf("--------------FCFS Scheduling--------------\n");

    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    printf("Process\t Waiting Time\t Turnaround Time\t Completed Time\n");
    for (int i = 0; i < n; i++) {
        total_waiting_time += current_time - processes[i].arrival_time;
        total_turnaround_time += current_time - processes[i].arrival_time + processes[i].burst_time;
        printf("%d\t %d\t\t %d\t\t\t %d\n", processes[i].process_id, current_time - processes[i].arrival_time,
               current_time - processes[i].arrival_time + processes[i].burst_time,
               current_time + processes[i].burst_time);
        current_time += processes[i].burst_time;
    }
    printf("Average waiting time: %.2f\n", (float) total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) total_turnaround_time / n);
    printf("--------------END--------------\n\n");
}

void SJF(Process processes[], int n) {
    printf("--------------SJF Scheduling--------------\n");

    int current_time = 0;
    int completed = 0;
    int completed_time[n];
    int first_arrival_time[n];
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed < n) {
        int shortest_task_index = -1;
        int shortest_burst_time = 9999;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time < shortest_burst_time &&
                processes[i].remaining_time > 0) {
                shortest_task_index = i;
                shortest_burst_time = processes[i].remaining_time;
            }
        }
        if (shortest_task_index == -1) {
            current_time++;
        } else {
            if (processes[shortest_task_index].remaining_time == processes[shortest_task_index].burst_time) {
                first_arrival_time[shortest_task_index] = current_time;
            }
            processes[shortest_task_index].remaining_time--;
            current_time++;
            if (processes[shortest_task_index].remaining_time == 0) {
                completed++;
                completed_time[shortest_task_index] = current_time;
                // printf("Task %d completed at time %d\n", processes[shortest_task_index].process_id, current_time);
            }
        }
    }

    int waiting_time[n];
    int turnaround_time[n];
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        waiting_time[i] = completed_time[i] - processes[i].arrival_time - processes[i].burst_time;
        turnaround_time[i] = completed_time[i] - first_arrival_time[i];
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }
    printf("Process\t Waiting Time\t Turnaround Time\t Completed Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t %d\t\t %d\t\t\t %d\n", processes[i].process_id, waiting_time[i], turnaround_time[i],
               completed_time[i]);
    }
    printf("Average waiting time: %.2f\n", (float) total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) total_turnaround_time / n);
    printf("--------------END--------------\n\n");
}

void PriorityBased(Process processes[], int n) {
    printf("--------------Priority Based--------------\n");

    int current_time = 0;
    int completed = 0;
    int completed_time[n];
    int first_arrival_time[n];
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed < n) {
        int current_task_index = -1;
        int current_priority = 9999;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].priority < current_priority &&
                processes[i].remaining_time > 0) {
                current_task_index = i;
                current_priority = processes[i].priority;
            }
        }
        if (current_task_index == -1) {
            current_time++;
        } else {
            if (processes[current_task_index].remaining_time == processes[current_task_index].burst_time) {
                first_arrival_time[current_task_index] = current_time;
            }
            processes[current_task_index].remaining_time--;
            current_time++;
            if (processes[current_task_index].remaining_time == 0) {
                completed++;
                completed_time[current_task_index] = current_time;
                // printf("Task %d completed at time %d\n", processes[current_task_index].process_id, current_time);
            }
        }
    }

    int waiting_time[n];
    int turnaround_time[n];
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        waiting_time[i] = completed_time[i] - processes[i].arrival_time - processes[i].burst_time;
        turnaround_time[i] = completed_time[i] - first_arrival_time[i];
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }
    printf("Process\t Waiting Time\t Turnaround Time\t Completed Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t %d\t\t %d\t\t\t %d\n", processes[i].process_id, waiting_time[i], turnaround_time[i],
               completed_time[i]);
    }
    printf("Average waiting time: %.2f\n", (float) total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) total_turnaround_time / n);
    printf("--------------END--------------\n\n");
}

void RoundRobin(Process processes[], int n, int quantum) {
    printf("--------------Round Robin--------------\n");
    int current_time = 0;
    int completed_count = 0;
    int current_id = 0;
    int is_idle = 1;
    int q = 0;
    int completed_time[n];
    int first_arrival_time[n];
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed_count < n) {
        if (is_idle || q == quantum) {
            //next job
            is_idle = 1;
            q = 0;
            for (int i = (current_time > 0); i < n + (current_time > 0); i++) {
                if (processes[(current_id + i) % n].remaining_time == 0 ||
                    processes[(current_id + i) % n].arrival_time > current_time) {
                    continue;
                } else {
                    current_id = (current_id + i) % n;
                    is_idle = 0;
                    break;
                }
            }
        }
        if (is_idle) {
            current_time++;
        } else {
            if (processes[current_id].remaining_time == processes[current_id].burst_time) {
                first_arrival_time[current_id] = current_time;
            }
            processes[current_id].remaining_time--;
            current_time++;
            q++;
            if (processes[current_id].remaining_time == 0) {
                completed_count++;
                // printf("Task %d completed at time %d\n", processes[current_id].process_id, current_time);
                completed_time[current_id] = current_time;
                is_idle = 1;
                q = 0;
            }
        }
    }

    int waiting_time[n];
    int turnaround_time[n];
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        waiting_time[i] = completed_time[i] - processes[i].arrival_time - processes[i].burst_time;
        turnaround_time[i] = completed_time[i] - first_arrival_time[i];
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }
    printf("Process\t Waiting Time\t Turnaround Time\t Completed Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t %d\t\t %d\t\t\t %d\n", processes[i].process_id, waiting_time[i], turnaround_time[i],
               completed_time[i]);
    }
    printf("Average waiting time: %.2f\n", (float) total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) total_turnaround_time / n);
    printf("--------------END--------------\n\n");
}

void RoundRobinPriority(Process processes[], int n, int quantum_time) {
    printf("--------------Priority With Round Robin--------------\n");

    int current_time = 0;
    int completed = 0;
    int completed_time[n];
    int first_arrival_time[n];
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }
    int current_task_index = -1;
    int current_priority = 9999;
    int q = 0;
    while (completed < n) {
        // find lowest priority task
        int lowest_priority_task_index = -1;
        int lowest_priority = 9999;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].priority < lowest_priority &&
                processes[i].remaining_time > 0) {
                lowest_priority_task_index = i;
                lowest_priority = processes[i].priority;
            }
        }

        if (current_priority > lowest_priority) {
            // there's a new arrival task with higher priority
            current_task_index = lowest_priority_task_index;
            current_priority = lowest_priority;
            q = 0;
        } else if (q == quantum_time) {
            // quantum time is up
            q = 0;
            for (int i = 1; i < n + 1; i++) {
                if (processes[(current_task_index + i) % n].arrival_time <= current_time &&
                    processes[(current_task_index + i) % n].remaining_time > 0 &&
                    current_priority == processes[(current_task_index + i) % n].priority) {
                    current_task_index = (current_task_index + i) % n;
                    break;
                }
            }
        } else if (lowest_priority == 9999) {
            current_task_index = -1;
        }

        if (current_task_index == -1) {
            current_time++;
        } else {
            if (processes[current_task_index].remaining_time == processes[current_task_index].burst_time) {
                first_arrival_time[current_task_index] = current_time;
            }
            current_time++;
            processes[current_task_index].remaining_time--;
            q++;
            if (processes[current_task_index].remaining_time == 0) {
                completed++;
                completed_time[current_task_index] = current_time;
                // printf("Task %d completed at time %d\n", processes[current_task_index].process_id, current_time);
                current_task_index = -1;
                current_priority = 9999;
                q = 0;
            }
        }
    }

    int waiting_time[n];
    int turnaround_time[n];
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        waiting_time[i] = completed_time[i] - processes[i].arrival_time - processes[i].burst_time;
        turnaround_time[i] = completed_time[i] - first_arrival_time[i];
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }
    printf("Process\t Waiting Time\t Turnaround Time\t Completed Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t %d\t\t %d\t\t\t %d\n", processes[i].process_id, waiting_time[i], turnaround_time[i],
               completed_time[i]);
    }
    printf("Average waiting time: %.2f\n", (float) total_waiting_time / n);
    printf("Average turnaround time: %.2f\n", (float) total_turnaround_time / n);
    printf("--------------END--------------\n\n");
}


int main() {
    Process processes[] = {
            {1, 0, 10, 2, 10},
            {2, 1, 5,  1, 5},
            {3, 2, 8,  3, 8},
            {4, 3, 3,  2, 3}
    };
    int quantum = 2;
    int n = sizeof(processes) / sizeof(processes[0]);

    // Sort processes based on arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                // Swap processes
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
    printf("Process\t Arrival Time\t Burst Time\t Priority\t Remaining Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t %d\t\t %d\t\t %d\t\t %d\n", processes[i].process_id, processes[i].arrival_time,
               processes[i].burst_time, processes[i].priority, processes[i].remaining_time);
    }

    FCFS(processes, n);
    SJF(processes, n);
    PriorityBased(processes, n);
    RoundRobin(processes, n, quantum);
    RoundRobinPriority(processes, n, quantum);
    return 0;
}
