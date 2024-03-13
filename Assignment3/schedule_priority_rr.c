// #include <stdio.h>
// #include <stdlib.h>
// #include "list.h"
// #include "cpu.h"
// #include <string.h>


// // Create the head of the list
// // potentially create new struct node as a pointer for queues (array of max priority + 1), set that to null

// struct node* head = NULL;

// // Make the TID for each process
// int tid = 0;

// // Time Quantum for Round-Robin
// #define TIME_QUANTUM 10

// // Function to add tasks to the list
// void add(char *name, int priority, int burst) {
//     Task *newTask = malloc(sizeof(Task));
//     newTask->name = strdup(name);
//     newTask->tid = tid++;
//     newTask->priority = priority;
//     newTask->burst = burst;
//     insert(&head, newTask);
//     // insert new task into appropriate queue of tasks 
//     // insert (&queues[priority], newTask)
// }

// // Function to pick the next task
// // Function to pick the next task
// Task* pickNextTask() {
//     // next task points to temporary task 
//     // temp = new struct node 
//     // check if temp-> next == null
//     // if it is, then set temp equal to the queues of the current priority
//     // else, temp = temp-> next 
//     // return next task 

//     if (head == NULL) {
//         return NULL;
//     }

//     // Find the task with the highest priority
//     struct node *current = head;
//     struct node *highestPriorityNode = head;

//     while (current != NULL) {
//         if (current->task->priority > highestPriorityNode->task->priority) {
//             highestPriorityNode = current;
//         }
//         current = current->next;
//     }

//     Task* nextTask = highestPriorityNode->task;

//     // Remove the node from the list
//     delete(&head, nextTask);

//     // Cycle through tasks with the same priority in round-robin fashion
//     current = head;
//     while (current != NULL) {
//         if (current->task->priority == nextTask->priority && current->task->burst != 0) {
//             // Insert the task at the end of the list
//             // Task *tempTask = current->task;
//             // // print head
//             // // printf("HEAD OF LIST FATTY: %s", head->task->name);
//             // printf("current task name %s", current->task->name);
//             // printf("\nnext task name %s", nextTask->name);
//             delete(&head, current->task);
//             insert(&head, nextTask);
            

//             return current->task;
//         }
//         current = current->next;
//     }
//     // printf("next task %s: %d\n", nextTask->name, nextTask->burst);
//     return nextTask;
// }


// // Function to schedule tasks using priority and round-robin
// void schedule() {
//     // work is in here
//     /*
//     for loop: current priority = maxpriority, current priority >= minpriotiy, current priority--
//     while queue is not empty, check queues and check current priority
//     if its not equal to null, current = picknexttask() 


    
//     if need to reschedule is true, run the task with the current burst 

//     then delete the task out of the queue 

//     else:
//     then check time quantum 
//     RR PART 
//     if burst > time quantum, 
//         run the task and subtract the quantum 
//     else:
//         run the task and delete it 




//     the idea is to use multiple queues --> use an array 






//     */

//     Task *task;

//     // loop through and schedule tasks 
//     while ((task = pickNextTask()) != NULL) {
//         // printf("picked next task %s with remaining burst %d\n", task->name, task->burst);
//         // Check if the task has remaining burst before scheduling
//         if (task->burst > 0) {
//             // call the CPU run function 
//             int executionTime = (task->burst <= TIME_QUANTUM) ? task->burst : TIME_QUANTUM;
//             run(task, executionTime);
//             // printf("running task burst %d\n with execution time %d\n", task->burst, executionTime);

//             // Update the remaining burst for the task
//             task->burst -= executionTime;

//             // If the task still has remaining burst, insert it back into the list
//             if (task->burst > 0) {
//                 Task *tempTask = task;
//                 // print head
//                 // delete(&head, task);
//                 insert(&head, tempTask);
//                 // traverse(head);
//             } else {
//                 // Free the memory of the task if its burst is complete
//                 // printf("leaving task %s with remaining burst %d\n", task->name, task->burst);
                
//                 // printf("\nTASK GOT NOTHING: next task %s\n", pickNextTask()->name);
//                 // free(task);
             
//                 continue;

//             }
//         } else {
//             // printf("current task %s with remaining burst %d\n", task->name, task->burst);
//             continue;
//         }
//     }
// }

// /*
// need to reschedule, another function 

// check to see if the queues (array of queues) current priority != null
//     then return 1
// else return 0 





// */






// // two different queues
// // priority in one (put ones that dont have same priority), rr in another


// // keep track that higher priority always gets run before lower priority (when looping thru same priority w RR)


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
int numTasks = 0;

// create list of lists
struct node* lists[MAX_PRIORITY + 1] = {NULL};
struct node* tempNode = NULL;
int currPrio = 0;

// define TID for each process
int tid = 0;

// add a task to the list head
// this will act as task reference list
void add(char *name, int priority, int burst) {
    // create task
    Task* t = (Task*)malloc(sizeof(Task));
    t->name = name;
    t->tid = tid++;
    t->priority = priority;
    t->burst = burst;
    t->responseTime = -1; // -1 to indicate task hasn't started yet
    insert(&lists[priority], t);
}

// pick next task
Task* pickNextTask() {
    // if length of task lisk is 1, run until completion
    // if greater, run using round robin
    Task* t = tempNode->task;
    if (tempNode->next == NULL) {
        tempNode = lists[currPrio];
    } else {
        tempNode = tempNode->next;
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
                printf("\nresponse time for task %s: %d\n\n", task->name, totalExecTime);
                responseTotal += task->responseTime;
            }

            if(!ifReschedule()) {
                totalExecTime += task->burst;
                run(task, task->burst);
                ++numTasks;
                delete(&lists[currPrio], task);
            } else { // round robin condition
                if(task->burst > QUANTUM) {
                    run(task, QUANTUM);
                    task->burst -= QUANTUM;
                    totalExecTime += QUANTUM;
                    ++numTasks;
                } else {
                    run(task, task->burst);
                    // clean up so burst becomes 0
                    totalExecTime += task->burst;
                    task->burst = 0;
                    ++numTasks;
                    delete(&lists[currPrio], task);
                }
            }
        }
    }
    // // calculate average turnaround time, waiting time, and response time
    // printf("\nTotal Number of Tasks: %d\n", numTasks);
    // printf("Total Turnaround Time: %d\n", turnaroundTotal);
    printf("Average Response Time: %.2f\n", ((double)responseTotal / (double)numTasks));
}