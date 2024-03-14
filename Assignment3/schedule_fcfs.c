#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "cpu.h"

// create the head of the list 
struct node* head = NULL;

// create variables to store the average statistics
float avgTurnaroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;


// make the TID for each process
int tid = 0;
// size of the list 
int size = 0;

// function to add the tasks to the list 
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    ++size;
    newTask->name = name;
    newTask->tid = __sync_fetch_and_add(&newTask->tid,1);
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&head, newTask);
}

// function to pick the next task
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

    // move the head to the next node
    *head = (*head)->next;

    // free the memory of the node (optional, depending on your design)
    free(temp);

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
        // add wait time to turnaround
        avgTurnaroundTime += task->waitingTime;
        
        // free the task
        free(task);
    }

    // print average statistics 
    printf("Average Turnaround Time: %.2f\n", (avgTurnaroundTime) / size);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime / size);
    printf("Average Response Time: %.2f\n", avgResponseTime / size);

}