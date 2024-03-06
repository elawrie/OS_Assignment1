#include <stdio.h>
#include <stdlib.h>
#include "list.h"  // include the task header file

void add(Task tasks[], int n) {
    int waiting_time = 0, turnaround_time = 0;

    printf("Task\tWaiting Time\tTurnaround Time\n");

    for (int i = 0; i < n; i++) {
        turnaround_time += tasks[i].burst;
        printf("%s\t%d\t\t%d\n", tasks[i].name, waiting_time, turnaround_time);
        waiting_time += tasks[i].burst;
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)turnaround_time / n);
}

int schedule() {
    int n;

    printf("Enter the number of tasks: ");
    scanf("%d", &n);

    Task tasks[n];

    // Input task details
    for (int i = 0; i < n; i++) {
        tasks[i].tid = i + 1;
        printf("Enter name for task %d: ", i + 1);
        scanf("%ms", &tasks[i].name);  // %ms is used for dynamically allocating memory for string
        printf("Enter priority for task %d: ", i + 1);
        scanf("%d", &tasks[i].priority);
        printf("Enter burst time for task %d: ", i + 1);
        scanf("%d", &tasks[i].burst);
    }

    // Sort tasks based on priority (FCFS for priority scheduling)
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (tasks[j].priority > tasks[j + 1].priority) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }

    // Perform FCFS Scheduling
    add(tasks, n);

    // Free dynamically allocated memory for task names
    for (int i = 0; i < n; i++) {
        free(tasks[i].name);
    }

    return 0;
}
