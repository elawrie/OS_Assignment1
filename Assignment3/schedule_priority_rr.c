#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "cpu.h"
#include "schedulers.h"

// variables for total turnaround time, waiting time, and response time
int turnaroundTotal = 0;
int waitingTotal = 0;
int responseTotal = 0;
int totalExecTime = 0;
int size = 0;

// create list of lists
struct node* lists[MAX_PRIORITY + 1] = {NULL};
struct node* tempNode = NULL;
int currPrio = 0;

// define TID for each process
int tid = 0;

// add a task to the list
void add(char *name, int priority, int burst) {
    // create task
    Task* t = (Task*)malloc(sizeof(Task));
    t->name = name;
    t->tid = tid++;
    t->priority = priority;
    t->burst = burst;
    turnaroundTotal += burst;
    t->waitingTime = 0;
    t->responseTime = -1; // -1 to indicate task hasn't started yet
    ++size;
    insert(&lists[priority], t);
}

// pick next task
Task* pickNextTask() {

    // calculate waiting time for all remaining tasks
    struct node *curr = lists[tempNode->task->priority];
    for (int i = MAX_PRIORITY; i >= MIN_PRIORITY; i--) {
        while (curr->next != NULL && curr->task->priority) {
            curr->next->task->waitingTime += curr->task->burst;
            curr = curr->next;
        }
    }

    // if length of task lisk is 1, run until completion
    // if greater, run using round robin
    Task* t = tempNode->task;
    if (tempNode->next == NULL) {
        tempNode = lists[currPrio];
    } else {
        tempNode = tempNode->next;
    }

    // update waiting time for all tasks in the list
    struct node *current = tempNode;
    while (current != NULL) {
        current->task->waitingTime += QUANTUM;
        current = current->next;
    }

    return t;
}

int ifReschedule() {
    if(lists[currPrio]->next != NULL) {
        return 1;
    }
    return 0;
}

// invoke the scheduler
void schedule() {
    Task *task;
    for (currPrio = MAX_PRIORITY; currPrio >= MIN_PRIORITY; currPrio--) {
        tempNode = lists[currPrio];
        while(lists[currPrio] != NULL) {
            task = pickNextTask();

            // check if task has been picked yet?
            if (task->responseTime == -1) {
                task->responseTime = totalExecTime;
                responseTotal += task->responseTime;
            }
            // if no round robin
            if(!ifReschedule()) {
                totalExecTime += task->burst;
                run(task, task->burst);
                delete(&lists[currPrio], task);

                // calculate waiting time
                waitingTotal += task->waitingTime;
            } 
            // round robin condition
            else { 
                tempNode->task->waitingTime += task->burst;
                
                if(task->burst > QUANTUM) {
                    run(task, QUANTUM);
                    task->burst -= QUANTUM;
                    totalExecTime += QUANTUM;
                } 
                else {
                    run(task, task->burst);
                    // calculate waiting time
                    waitingTotal += task->waitingTime;
                    totalExecTime += task->burst;
                    task->burst = 0;
                    delete(&lists[currPrio], task);
                }
            }
        }
    }
    // calculate average statistics 
    printf("Average Turnaround Time: %.2f\n", (((double)turnaroundTotal + waitingTotal) / (double)size));
    printf("Average Response Time: %.2f\n", ((double)responseTotal / (double)size));
    printf("Average Waiting Time: %.2f\n", ((double)waitingTotal / (double)size));
}