#include <stdio.h>
#include <stdlib.h>
#include "list.h"  // include the task header file
#include "cpu.h"

// create the head of the list 
struct node* head = NULL;

// make the TID for each process
int tid = 0;

// function to add the tasks to the list based on priority
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = name;
    newTask->tid = tid++;
    newTask->priority = priority;
    newTask->burst = burst;
    // Insert the task into the list based on priority
    insert(&head, newTask);
}

// function to pick the next task with the highest priority
Task* pickNextTask(struct node **head) {
    if (*head == NULL) {
        // If the list is empty, return NULL
        return NULL;
    }

    // Find the task with the highest priority
    struct node *current = *head;
    struct node *highestPriorityNode = *head;

    while (current != NULL) {
        if (current->task->priority > highestPriorityNode->task->priority) {
            highestPriorityNode = current;
        }
        current = current->next;
    }

    Task* nextTask = highestPriorityNode->task;

    // Remove the node from the list
    delete(head, nextTask);

    return nextTask;
}

// do logic for algos in the ADD FUNCTION
// insert into list , put more logic into the add function to put the nodes in the correct order for more complicated algos 
// ex. shortest job first --> put the tasks in the list BASED ON THEIR CPU BURST 

// function to schedule the tasks 
void schedule() {
    Task* task;
    while ((task = pickNextTask(&head)) != NULL) {
        // Execute the task (you might want to call your CPU run function here)
        printf("Executing task: %s\n", task->name);
        // You can perform other operations related to the execution

        // Free the memory of the task if needed (depends on your design)
        free(task);
    }
}
