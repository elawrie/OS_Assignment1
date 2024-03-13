#include <stdio.h>
#include <stdlib.h>
#include "list.h"  // include the task header file
#include "cpu.h"

// create the head of the list 
struct node* head = NULL;
// struct node* tempHead;
float avgTurnaroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;


// make the TID for each process
int tid = 0;

int size = 0;

// function to add the tasks to the list 
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    ++size;
    newTask->name = name;
    newTask->tid = __sync_fetch_and_add(&newTask->tid,1);
    newTask->priority = priority;
    newTask->burst = burst;
    // if (head == NULL) {
    //     tempHead->task = newTask;
    // }
    insert(&head, newTask);
}

// function to pick the next task
// Task* pickNextTask() {
    // // goes through the list and picks the next one sequentially for this type 
    // if (head == NULL) {
    //     // if the list is empty, return NULL
    //     return NULL;
    // }

    // struct node *nextTask = head->task;

    // return nextTask;
     // error handling for when the list is empty
//     if (head == NULL) {
//         return NULL;
//     }
//     struct task *nextTask = head->task;
//     head = head->next;
//     return nextTask;
// }
Task* pickNextTask(struct node **head) {
    if (*head == NULL) {
        // If the list is empty, return NULL
        return NULL;
    }

    // calculate waiting time for all remaining tasks
    struct node *current = *head;
    while (current->next != NULL) {
        current->next->task->waitingTime += current->task->burst;
        current = current->next;
    }

    Task* nextTask = (*head)->task;
    struct node *temp = *head;

    // Move the head to the next node
    *head = (*head)->next;

    // Free the memory of the node (optional, depending on your design)
    free(temp);

    return nextTask;
}

// do logic for algos in the ADD FUNCTION
// insert into list , put more logic into the add function to put the nodes in the correct order for more complicated algos 
// ex. shortest job first --> put the tasks in the list BASED ON THEIR CPU BURST 

// function to schedule the tasks 
void schedule() {
    // struct task *task = head->task;
    // // loop through and schedule tasks 
    // while (task != NULL) {
    //     task = pickNextTask();
    //     // call the CPU run function 
    //     run(task, task->burst);
    //     // increments head to the next 
    //     delete(&head, task);
    // }
    // traverse(head);
    // printf("HEAD: %s", head->task->name);
    Task* task;
    while ((task = pickNextTask(&head)) != NULL) {
    // Execute the task (you might want to call your CPU run function here)
        printf("Executing task: %s\n", task->name);
        // increment statistics
        avgWaitingTime += task->waitingTime;
        avgTurnaroundTime += task->burst;

        // add wait time to turnaround
        avgTurnaroundTime += task->waitingTime;
        
        // print task waiting time 
        // printf("Waiting Time: %d\n", task->waitingTime);
        avgResponseTime += task->waitingTime;
        // print task response time
        // printf("Response Time: %d\n", task->waitingTime + task->burst);
    // You can perform other operations related to the execution

    // Free the memory of the task if needed (depends on your design)
        free(task);
    }
    // calculate average statistics
    // average response time is the turnaround time (burst) plus the wait time 
    // struct node *current = tempHead;
    // printf("HEAD: %s", tempHead->task->name);
    // while (current != NULL) {
    //     size++;
    //     avgTurnaroundTime += current->task->burst;
    //     avgWaitingTime += current->task->waitingTime;
    //     avgResponseTime += current->task->waitingTime + current->task->burst;
    //     current = current->next;
    // }



    // turnaround is wait + burst 
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime / size);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime / size);
    // print avg waiting time
    // printf("Average Waiting Time TRUST: %.2f\n", avgWaitingTime);
    // print avg response time
    // printf("Average Response Time TRUST: %.2f\n", avgResponseTime);
    // when it first gets to run 
    // have to look at RR time for RR (quantum)
    // look at it in txt file 
    printf("Average Response Time: %.2f\n", avgResponseTime / size);

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