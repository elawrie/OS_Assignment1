// check time quantum (10 in this case)
// first gets to run but only for 10, then all of them, get removed from the queue 
// decrement CPU burst value each time then run 
// DO THE ROUND ROBIN LOGIC IN THE SCHEDULE FUNCTION

// logic
// or check if burst is <= TQ (or greater than depending on desired logic)
// decrement burst 
// remove task from queue if its less than or equal to time quantum 

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