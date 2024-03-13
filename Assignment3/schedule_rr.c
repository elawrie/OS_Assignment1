// // check time quantum (10 in this case)
// // first gets to run but only for 10, then all of them, get removed from the queue 
// // decrement CPU burst value each time then run 
// // DO THE ROUND ROBIN LOGIC IN THE SCHEDULE FUNCTION

// // logic
// // or check if burst is <= TQ (or greater than depending on desired logic)
// // decrement burst 
// // remove task from queue if its less than or equal to time quantum 

// #include <stdio.h>
// #include <stdlib.h>
// #include "list.h"  // include the task header file
// #include "cpu.h"

// // // create the head of the list 
// // struct node* head = NULL;

// // // make the TID for each process
// // int tid = 0;

// // // function to add the tasks to the list 
// // void add(char *name, int priority, int burst) {
// //     Task *newTask = malloc(sizeof(Task));
// //     newTask->name = name;
// //     newTask->tid = tid++;
// //     newTask->priority = priority;
// //     newTask->burst = burst;
// //     insert(&head, newTask);
// // }

// // create list
// struct node* head = NULL;
// struct node *temp;

// // define TID for each process
// int tid = 0;

// // add a task to the list head
// void add(char *name, int priority, int burst) {
//     Task *t = (Task*)malloc(sizeof(Task));
//     t->name = name;
//     t->tid = tid++;
//     t->priority = priority;
//     t->burst = burst;
//     insert(&head, t);
// }

// // function to pick the next task
// // Task* pickNextTask() {
// //     // goes through the list and picks the next one sequentially for this type 
// // }

// // do logic for algos in the ADD FUNCTION
// // insert into list , put more logic into the add function to put the nodes in the correct order for more complicated algos 
// // ex. shortest job first --> put the tasks in the list BASED ON THEIR CPU BURST 

// // function to schedule the tasks 
// // void schedule() {

// //     Task* task;
// //     // loop through and schedule tasks 
// //     while (task != NULL) {
// //         task = pickNextTask();
// //         // call the CPU run function 
// //         run(task, task->burst);
// //         // increments head to the next 
// //         delete(&head, task);
// //     }
// // }



// // pick next task
// Task* pickNextTask() {
//     Task *t = temp->task;
//     // if only 1 node left
//     if (temp->next == NULL) {
//         temp = head;
//     } else {
//         temp = temp->next;
//     }
//     return t;
// }

// // invoke the scheduler
// void schedule() {
//     // loop through the task list
//     Task* task;
//     temp = head;
//     while (head != NULL) {
//         task = pickNextTask();
//         if(task->burst > QUANTUM) {
//             run(task, QUANTUM);
//             task->burst -= QUANTUM;
//         } else {
//             run(task, task->burst);
//             // clean up so burst becomes 0
//             task->burst = 0;
//             delete(&head, task);
//         }
//     }
// }




#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "cpu.h"

// priority not needed for FCFS
// #define MIN_PRIORITY 1
// #define MAX_PRIORITY 10

// create list
struct node* head = NULL;
struct node *temp;

float avgTurnaroundTime = 0, avgWaitingTime = 0, avgResponseTime = 0;
int size = 0;

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
        if(task->burst > QUANTUM) {
            run(task, QUANTUM);
            task->burst -= QUANTUM;
            avgTurnaroundTime += QUANTUM;
        } else {
            run(task, task->burst);
            // increment statistics
            avgWaitingTime += task->waitingTime;
            // avgResponseTime += task->waitingTime + task->burst;
            // clean up so burst becomes 0
            avgTurnaroundTime += task->burst;
            task->burst = 0;
            delete(&head, task);
        }
    }
    // print average statistics 
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime / size);
    printf("Average Waiting Time: %.2f\n", avgWaitingTime / size);
    printf("Average Response Time: %.2f\n", avgResponseTime / size);
}