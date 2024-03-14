#include <stdio.h>
#include <stdlib.h>
#include "list.h"  // include the task header file
#include "cpu.h"

// create the head of the list 
struct node* head = NULL;

// variables for statistics 
float avgTurnaroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;
int size = 0;

// make the TID for each process
int tid = 0;

// function to add the tasks to the list based on priority
void add(char *name, int priority, int burst) {
    ++size;
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = __sync_fetch_and_add(&newTask->tid,1);
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&head, newTask);
}

// function to pick the next task with the highest priority
Task* pickNextTask(struct node **head) {
    if (*head == NULL) {
        // if the list is empty, return NULL
        return NULL;
    }

    // calculate waiting time for all remaining tasks
    struct node *curr = *head;
    while (curr->next != NULL && curr->next != NULL) {
        curr->next->task->waitingTime += curr->task->burst;
        curr = curr->next;
    }

    // rind the task with the highest priority
    struct node *current = *head;
    struct node *highestPriorityNode = *head;

    while (current != NULL) {
        if (current->task->priority > highestPriorityNode->task->priority) {
            highestPriorityNode = current;
        }
        current = current->next;
    }

    Task* nextTask = highestPriorityNode->task;

    // remove the node from the list
    delete(head, nextTask);

    return nextTask;
}

// function to schedule the tasks 
void schedule() {
    Task* task;
    while ((task = pickNextTask(&head)) != NULL) {
        // execute the task 
        run(task, task->burst);
        // increment statistics
        avgWaitingTime += task->waitingTime;
        avgTurnaroundTime += task->burst;
        avgResponseTime += task->waitingTime;
        free(task);
    }

    // print average statistics 
    printf("Average Turnaround Time: %.2f\n", (avgTurnaroundTime + avgWaitingTime) / size);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime / size);
    printf("Average Response Time: %.2f\n", avgResponseTime / size);
}
