#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "cpu.h"

// create the head of the list 
struct node* head = NULL;

// variables for statistics
float avgTurnaroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;
int size = 0;

int tid = 0;

void add(char *name, int priority, int burst) {
    ++size;
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = tid++;
    newTask->priority = priority;
    newTask->burst = burst;
    
    // if the list is empty or the new task has the shortest burst time, make it the new head
    if (head == NULL || burst < head->task->burst) {
        struct node *newNode = malloc(sizeof(struct node));
        newNode->task = newTask;
        newNode->next = head;
        newTask->tid = __sync_fetch_and_add(&newTask->tid,1);
        head = newNode;
        return;
    }
    
    // otherwise, find the correct position to insert the new task
    struct node *current = head;
    while (current->next != NULL && burst >= current->next->task->burst) {
        current = current->next;
    }
    
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = current->next;
    current->next = newNode;
}


// pick the next task 
Task* pickNextTask(struct node **head) {
    if (*head == NULL) {
        // if the list is empty, return NULL
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
    free(temp);

    return nextTask;
}

// schedule the tasks
void schedule() {    
    traverse(head);
    printf("HEAD: %s", head->task->name);
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

        free(task);
    }

    // print average statistics 
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime / size);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime / size);

    printf("Average Response Time: %.2f\n", avgResponseTime / size);
}