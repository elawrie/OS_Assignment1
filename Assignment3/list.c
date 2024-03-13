/**
 * Various list operations
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "task.h"


// add a new task to the list of tasks
void insert(struct node **head, Task *newTask) {
    // add the new task to the list 
    struct node *newNode = malloc(sizeof(struct node));

    newNode->task = newTask;
    newNode->next = *head;
    *head = newNode;
}
// void insert(struct node **head, Task *newTask) {
//     // Create a new node for the new task
//     struct node *newNode = malloc(sizeof(struct node));
//     if (newNode == NULL) {
//         // Handle memory allocation failure
//         printf("Memory allocation failed.\n");
//         return;
//     }
//     newNode->task = newTask;
//     newNode->next = NULL;

//     // If the list is empty, set the new node as the head
//     if (*head == NULL) {
//         *head = newNode;
//     } else {
//         // Otherwise, traverse the list to the end
//         struct node *current = *head;
//         while (current->next != NULL) {
//             current = current->next;
//         }
//         // Add the new node at the end
//         current->next = newNode;
//     }
// }

// delete the selected task from the list
void delete(struct node **head, Task *task) {
    struct node *temp;
    struct node *prev;

    temp = *head;
    // special case - beginning of list
    if (strcmp(task->name,temp->task->name) == 0) {
        *head = (*head)->next;
    }
    else {
        // interior or last element in the list
        prev = *head;
        temp = temp->next;
        while (strcmp(task->name,temp->task->name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        prev->next = temp->next;
    }
}

// traverse the list
void traverse(struct node *head) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {
        printf("[%s] [%d] [%d]\n",temp->task->name, temp->task->priority, temp->task->burst);
        temp = temp->next;
    }
}
