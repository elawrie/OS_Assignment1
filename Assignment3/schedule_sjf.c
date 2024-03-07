#include <stdio.h>
#include <stdlib.h>
#include "list.h"  // include the task header file

// create the head of the list 
struct node* head = NULL;

// make the TID for each process
int tid = 0;

// function to add the tasks to the list 
// void add(char *name, int priority, int burst) {
//     // printf("GOES INTO ADD\n");
//     Task *newTask = malloc(sizeof(Task));
//     newTask->name = name;
//     newTask->tid = tid++;
//     newTask->priority = priority;
//     newTask->burst = burst;
//     // printf("BEFORE LOOP\n");
//     // while(head->task->burst <= newTask->burst) {
//     //     printf("HEY FATTY");
//     //     head = head->next;
//     // }
//     while(head != NULL && head->task->burst <= newTask->burst) {
//         // printf("HEY FATTY\n");
//         head = head->next;
//     }
//     insert(&head, newTask);
// }
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = tid++;
    newTask->priority = priority;
    newTask->burst = burst;
    
    // If the list is empty or the new task has the shortest burst time, make it the new head
    if (head == NULL || burst < head->task->burst) {
        struct node *newNode = malloc(sizeof(struct node));
        newNode->task = newTask;
        newNode->next = head;
        head = newNode;
        return;
    }
    
    // Otherwise, find the correct position to insert the new task
    struct node *current = head;
    while (current->next != NULL && burst >= current->next->task->burst) {
        current = current->next;
    }
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = current->next;
    current->next = newNode;
}


Task* pickNextTask(struct node **head) {
    if (*head == NULL) {
        // If the list is empty, return NULL
        return NULL;
    }
    Task* nextTask = (*head)->task;
    struct node *temp = *head;
    // Move the head to the next node
    *head = (*head)->next;
    // Free the memory of the node (optional, depending on your design)
    free(temp);

    return nextTask;
}




// function to schedule the tasks 
void schedule() {    
    traverse(head);
    Task* task;
    while ((task = pickNextTask(&head)) != NULL) {
    // Execute the task (you might want to call your CPU run function here)
        printf("Executing task: %s\n", task->name);
    // You can perform other operations related to the execution

    // Free the memory of the task if needed (depends on your design)
        free(task);
    }
}