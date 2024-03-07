#include <stdio.h>
#include <stdlib.h>
#include "list.h"  // include the task header file

// create the head of the list 
struct node* head = NULL;

// make the TID for each process
int tid = 0;

// function to add the tasks to the list 
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = tid++;
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&head, newTask);
}

// function to pick the next task
Task* pickNextTask() {
    // goes through the list and picks the next one sequentially for this type 
}

// do logic for algos in the ADD FUNCTION
// insert into list , put more logic into the add function to put the nodes in the correct order for more complicated algos 
// ex. shortest job first --> put the tasks in the list BASED ON THEIR CPU BURST 

// function to schedule the tasks 
void schedule() {

    Task* task;
    // loop through and schedule tasks 
    while (task != NULL) {
        task = pickNextTask();
        // call the CPU run function 
        run(task, task->burst);
        // increments head to the next 
        delete(&head, task);
    }
}

// void add(Task tasks[], int n) {
//     int waiting_time = 0, turnaround_time = 0;

//     printf("Task\tWaiting Time\tTurnaround Time\n");

//     for (int i = 0; i < n; i++) {
//         turnaround_time += tasks[i].burst;
//         printf("%s\t%d\t\t%d\n", tasks[i].name, waiting_time, turnaround_time);
//         waiting_time += tasks[i].burst;
//     }

//     printf("\nAverage Waiting Time: %.2f\n", (float)waiting_time / n);
//     printf("Average Turnaround Time: %.2f\n", (float)turnaround_time / n);
// }

// int schedule() {
//     int n;

//     printf("Enter the number of tasks: ");
//     scanf("%d", &n);

//     Task tasks[n];

//     // Input task details
//     for (int i = 0; i < n; i++) {
//         tasks[i].tid = i + 1;
//         printf("Enter name for task %d: ", i + 1);
//         scanf("%ms", &tasks[i].name);  // %ms is used for dynamically allocating memory for string
//         printf("Enter priority for task %d: ", i + 1);
//         scanf("%d", &tasks[i].priority);
//         printf("Enter burst time for task %d: ", i + 1);
//         scanf("%d", &tasks[i].burst);
//     }

//     // Sort tasks based on priority (FCFS for priority scheduling)
//     for (int i = 0; i < n - 1; i++) {
//         for (int j = 0; j < n - i - 1; j++) {
//             if (tasks[j].priority > tasks[j + 1].priority) {
//                 Task temp = tasks[j];
//                 tasks[j] = tasks[j + 1];
//                 tasks[j + 1] = temp;
//             }
//         }
//     }

//     // Perform FCFS Scheduling
//     add(tasks, n);

//     // Free dynamically allocated memory for task names
//     for (int i = 0; i < n; i++) {
//         free(tasks[i].name);
//     }

//     return 0;
// }


// add your "add" code for the scheduler which is what the driver code will compile and use 
// main is in driver -- call the add function that you write in each 

// main pieces: make add and schedule functions for each scheduling algorithm