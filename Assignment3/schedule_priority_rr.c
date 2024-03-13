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
int size = 0;

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
    turnaroundTotal += burst;
    t->responseTime = -1; // -1 to indicate task hasn't started yet
    ++size;
    insert(&lists[priority], t);
}

// void incrementWaitTime(struct node *head, int waitTime) {
//     struct node* current = head;
//     while (current->next != NULL) {
//         // increment wait time
//         current->next->task->waitingTime += waitTime;
//         current = current->next;
//     }
//     printf("NULL\n");
// }

// pick next task
Task* pickNextTask() {

    // calculate waiting time for all remaining tasks
    struct node *curr = lists[tempNode->task->priority]; // 8 , [5,4], 1, ... 
    // struct node *curr = tempNode;
    for (int i = MAX_PRIORITY; i >= MIN_PRIORITY; i--) {
        incrementWaitTime(lists[curr->task->priority], curr->task->burst);
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
            
            // print task
            printf("task name AT TOP PLSSS: %s\n", task->name);
            // curr node print
            printf("curr node name: %s\n", tempNode->task->name);

            // check if task has been picked yet?
            if (task->responseTime == -1) {
                task->responseTime = totalExecTime;
                // printf("\nresponse time for task %s: %d\n\n", task->name, totalExecTime);
                responseTotal += task->responseTime;
            }
            // if no round robin
            if(!ifReschedule()) {
                totalExecTime += task->burst;
                run(task, task->burst);
                delete(&lists[currPrio], task);
                // print current task waiting time and name
                printf("current task WAITING TIMEEE: %d\n", task->waitingTime);
                printf("current task NAME: %s\n", task->name);
                // add waiting time to current node 

                // calculate waiting time
                waitingTotal += task->waitingTime;
            } else { // round robin condition
                
                // print out current task name
                printf("current task NAME: %s\n", task->name);
                // calculate individual waiting time 
                tempNode->task->waitingTime += task->burst;
                
                if(task->burst > QUANTUM) {
                    run(task, QUANTUM);
                    task->burst -= QUANTUM;
                    totalExecTime += QUANTUM;
                } else {
                    run(task, task->burst);
                    // calculate waiting time
                    waitingTotal += task->waitingTime;
                    printf("current task WAITING TIMEEE: %d\n", task->waitingTime);
                    printf("current task NAME: %s\n", task->name);
                    // clean up so burst becomes 0
                    totalExecTime += task->burst;
                    task->burst = 0;
                    delete(&lists[currPrio], task);
                }
            }
            printf("IM HERE\n");
        }
    }
    // calculate average statistics 
    // printf("\nTotal Number of Tasks: %d\n", size);
    // printf("Total Turnaround Time: %d\n", turnaroundTotal);
    // print total response 
    printf("Total Response Time: %d\n", responseTotal);
    // print size 
    printf("Size: %d\n", size);
    printf("Average Response Time: %.2f\n", ((double)responseTotal / (double)size));
    // print total waiting time
    printf("Total Waiting Time: %d\n", waitingTotal);
    // print average waiting time
    printf("Average Waiting Time: %.2f\n", ((double)waitingTotal / (double)size));
}