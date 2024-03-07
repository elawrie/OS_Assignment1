// higher value is higher priority 

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
    while(head->task->priority >= newTask->burst) {
        head = head->next;
    }
    insert(&head, newTask);

    // do logic for algos in the ADD FUNCTION
    // insert into list , put more logic into the add function to put the nodes in the correct order for more complicated algos 
    // ex. shortest job first --> put the tasks in the list BASED ON THEIR CPU BURST 

}

// function to pick the next task
Task* pickNextTask() {
    struct node *temp;
    Task* task = head->task;
    temp = head->next; 

    // while loop temp != null
    // if task burst from temp is less than the task burst that you got, then pick it! (which task burst is less)
    // save the one with the lower burst
    // find minumum in the list 
    // increment the temp pointer (temp = temp->next)
    // return the minimum that you found 
}



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