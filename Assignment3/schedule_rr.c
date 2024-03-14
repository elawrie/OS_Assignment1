#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "cpu.h"

// create list
struct node* head = NULL;
struct node *temp;

// variables for statistics
float avgTurnaroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;
int size = 0;
int totalExecTime = 0;


// define TID for each process
int tid = 0;

// add a task to the list head
void add(char *name, int priority, int burst) {
    ++size;
    Task *newTask = (Task*)malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = __sync_fetch_and_add(&newTask->tid,1);
    newTask->priority = priority;
    newTask->burst = burst;
    avgTurnaroundTime += burst;
    newTask->responseTime = -1; // -1 to indicate task hasn't started yet
    insert(&head, newTask);
}

// pick next task
Task* pickNextTask() {

    // calculate waiting time for all remaining tasks
    struct node *curr = head;
    while (curr->next != NULL && curr->next != NULL) {
        curr->next->task->waitingTime += curr->task->burst;
        curr = curr->next;
    }

    Task *newTask = temp->task;
    // if only 1 node left
    if (temp->next == NULL) {
        temp = head;
    } else {
        temp = temp->next;
    }

    return newTask;
}

// invoke the scheduler
void schedule() {
    // loop through the task list
    Task* task;
    temp = head;
    while (head != NULL) {
        task = pickNextTask();

        // checking if task has been picked yet to start
        if (task->responseTime == -1) {
            task->responseTime = totalExecTime;
            avgResponseTime += task->responseTime;
        }

        if(task->burst > QUANTUM) {
            run(task, QUANTUM);
            task->burst -= QUANTUM;
            totalExecTime += QUANTUM; // increment every time a task is executed
        } else {
            run(task, task->burst);
            // increment statistics
            avgWaitingTime += task->waitingTime;
            totalExecTime += task->burst; 
            task->burst = 0;
            delete(&head, task);
        }
    }
    // print average statistics 
    printf("Average Turnaround Time: %.2f\n", (avgTurnaroundTime + avgWaitingTime) / size);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime / size);
    printf("Average Response Time: %.2f\n", avgResponseTime / size);
}