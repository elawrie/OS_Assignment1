
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// // TODO: MAKE FUNCTION PROTOTYPES 

// // initialize global variables
// int initial_memory_size;
// int num_processes = 0;
// struct MemoryBlock *head;
// struct MemoryBlock *initial_block;
// struct MemoryBlock *temp;
// struct MemoryBlock *temp2;
// int last_address_space;
// int space_requested;

// // NEW 
// // typedef struct hole {
// //     int start_address;
// //     int end_address;
// //     int space_avail;
// //     int valid_bit;
// //     char status[10];
// // } Hole;

// // NEW 
// // typedef struct process {
// //     char process_name[10];
// //     int process_size;
// //     int start_address;
// //     int end_address;
// // } Process;

// // structure to represent a block of memory
// struct MemoryBlock {
//     int start_address;
//     int end_address;
//     int space_avail;
//     char status[10];
//     struct MemoryBlock *next;
// };

// // NEW 
// // Hole *hole_array[100];
// // Process *process_array[100];
// // void *memory
// // int num_holes;

// // function to condense all holes into one big hole 
// void condense_holes() {
//     temp = head;
//     while (temp->next != NULL && temp->next->next != NULL) {
//         if (strcmp(temp->next->status, "Unused") == 0 && strcmp(temp->next->next->status, "Unused") == 0) {
//             temp->next->next->start_address = temp->next->start_address;
//             temp->next->space_avail = temp->next->space_avail + temp->next->next->space_avail;
//             temp->next->end_address = temp->next->next->end_address;
//             struct MemoryBlock *temp_next_next = temp->next->next;
//             temp->next->next = temp_next_next->next;
//             if (temp_next_next->next != NULL) {
//                 temp_next_next = temp->next;
//             }
//             free(temp_next_next);
//         } else {
//             temp = temp->next;
//         }
//     }
// }

// // initialize memory block 
// void initialize_memory(int size) {

//     // // NEW 
//     // memory = malloc(size);
//     // hole_array[0] = malloc(sizeof(Hole));
//     // hole_array[0]->start_address = 0;
//     // hole_array[0]->end_address = size - 1;
//     // hole_array[0]->space_avail = size;
//     // hole_array[0]->valid_bit = 1;
//     // num_holes = 1;



//     head = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
//     initial_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
//     if (head == NULL) {
//         printf("Memory allocation failed.\n");
//         exit(1);
//     }

//     head->start_address = -1;
//     head->end_address = -1;
//     head->space_avail = size;
//     head->next = initial_block;
//     strcpy(head->status, "Unused");

//     strcpy(initial_block->status, "Unused");
//     initial_block->start_address = 0;
//     initial_block->end_address = size;
//     initial_block->space_avail = size;
//     initial_block->next = NULL;

//     last_address_space = size;
// }

// // implement first fit request functionality
// int first_fit_request(char *process_name, int size) {

//     // // NEW 
//     // int index = 0;
//     // int bytes_allocated = -1; // return -1 if no space found 
//     // while(index < num_holes) {
//     //     if (hole_array[index]->size >= size) {
//     //         // create a new block
//     //         process_array[num_processes] = malloc(sizeof(Process));
//     //         block_array[num_processes]->size = size;
//     //         block_array[num_processes]->start_address = hole_array[index]->start_address;
//     //         block_array[num_processes]->process_name = strdup(process_name);
//     //         // change end address 
//     //         ++num_processes;
//     //         hole_array[index]->start_address += size;
//     //         hole_array[index]->size -= size;
//     //         // check if the hole is now 0 (if the size of the process is equal to the size of the hole it replaced)
//     //         if (hole_array[index]->size == 0) {
//     //             hole_array[index]->valid_bit = 0;
//     //             --num_holes;
//     //             // shrink hole list 
//     //             // iterate through the remaining holes - 1 
//     //             // happens if you fully get rid of one hole 
//     //             for (int i = index; i < num_holes - 1; ++i) {
//     //                 hole_array[i] = hole_array[i + 1];
//     //             }
//     //         }
//     //         bytes_allocated = size;
//     //         break;
//     //     }
//     //     ++index; 
//     // }
//     // return bytes_allocated;




//     temp = head;
//     int leftover_space = 0;

//     while (temp->next != NULL) {
//         if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
//             head->space_avail = head->space_avail - size;

//             strcpy(temp->next->status, process_name);
//             temp->next->end_address = temp->next->start_address + size - 1;

//             leftover_space = temp->next->space_avail - size;

//             if (leftover_space > 0) {
//                 struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
//                 strcpy(new_block->status, "Unused");
//                 new_block->space_avail = leftover_space;
//                 temp->next->space_avail = size;
//                 new_block->start_address = temp->next->end_address + 1;
//                 new_block->end_address = new_block->start_address + leftover_space - 1;
//                 if (new_block->end_address > last_address_space) {
//                     new_block->end_address = last_address_space;
//                 }
//                 new_block->next = temp->next->next;
//                 temp->next->next = new_block;
//             }
//             return;
//         } 
//     else {
//         temp = temp->next;
//     }  
//     }
//     // if no space found 
//     printf("There is no space available for process %s.\n", process_name);
// }

// // implement best fit request functionality
// void best_fit_request(char *process_name, int size) {
//     temp = head;
//     int leftover_space = 0;
//     int best_fit = last_address_space;
//     struct MemoryBlock *best_fit_block = NULL;

//     // loop through memory blocks to find best fit hole (big enough to fit process)
//     while (temp->next != NULL) {
//         // print temp variable status 
//         printf("Temp status: %s\n", temp->next->status);
//         if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
//             printf("Space available: %d\n", temp->next->space_avail);
//             // print the status of the next block
//             printf("Next block status: %s\n", temp->next->status);
//             if (temp->next->space_avail < best_fit) {
//                 // print im here for debugging 
//                 printf("Im here\n");
//                 // print the space available 
//                 best_fit = temp->next->space_avail;
//                 best_fit_block = temp;
//             }
//         }
//         temp = temp->next;
//     }

//     if (best_fit_block != NULL) {
//         head->space_avail = head->space_avail - size;

//         strcpy(best_fit_block->next->status, process_name);
//         best_fit_block->next->end_address = best_fit_block->next->start_address + size - 1;

//         leftover_space = best_fit_block->next->space_avail - size;

//         if (leftover_space > 0) {
//             struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
//             strcpy(new_block->status, "Unused");
//             new_block->space_avail = leftover_space;
//             best_fit_block->next->space_avail = size;
//             new_block->start_address = best_fit_block->next->end_address + 1;
//             new_block->end_address = new_block->start_address + leftover_space - 1;
//             if (new_block->end_address > last_address_space) {
//                 new_block->end_address = last_address_space;
//             }
//             new_block->next = best_fit_block->next->next;
//             best_fit_block->next->next = new_block;
//         }
//     } else {
//         printf("There is no space available for process %s.\n", process_name);
//     }
// }

// // implement worst fit request functionality
// void worst_fit_request(char *process_name, int size) {
//     temp = head;
//     int leftover_space = 0;
//     int worst_fit = 0;
//     struct MemoryBlock *worst_fit_block = NULL;

//     // loop through memory blocks to find worst fit hole (big enough to fit process)
//     while (temp->next != NULL) {
//         // print temp variable status 
//         printf("Temp status: %s\n", temp->next->status);
//         if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
//             printf("Space available: %d\n", temp->next->space_avail);
//             // print the status of the next block
//             printf("Next block status: %s\n", temp->next->status);
//             if (temp->next->space_avail > worst_fit) {
//                 // print im here for debugging 
//                 printf("Im here\n");
//                 // print the space available 
//                 worst_fit = temp->next->space_avail;
//                 worst_fit_block = temp;
//             }
//         }
//         temp = temp->next;
//     }

//     if (worst_fit_block != NULL) {
//         head->space_avail = head->space_avail - size;

//         strcpy(worst_fit_block->next->status, process_name);
//         worst_fit_block->next->end_address = worst_fit_block->next->start_address + size - 1;

//         leftover_space = worst_fit_block->next->space_avail - size;

//         if (leftover_space > 0) {
//             struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
//             strcpy(new_block->status, "Unused");
//             new_block->space_avail = leftover_space;
//             worst_fit_block->next->space_avail = size;
//             new_block->start_address = worst_fit_block->next->end_address + 1;
//             new_block->end_address = new_block->start_address + leftover_space - 1;
//             if (new_block->end_address > last_address_space) {
//                 new_block->end_address = last_address_space;
//             }
//             new_block->next = worst_fit_block->next->next;
//             worst_fit_block->next->next = new_block;
//         }
//     } else {
//         printf("There is no space available for process %s.\n", process_name);
//     }
// }

// // implement request functionality 
// // NEW: change size to be passed in as a string (change to int when calling strategy functions)
// void request_memory(char *process_name, char *size, char *strategy) {
//     if (*strategy == 'F') {
//         if (first_fit_request(process_name, atoi(size)) != -1) {
//             // NEW 
//             // IT WORKED 
//         }
//     } else if (*strategy == 'B') {
//         best_fit_request(process_name, atoi(size));
//     } else if (*strategy == 'W') {
//         worst_fit_request(process_name, atoi(size));
//     } else {
//         printf("Invalid strategy.\n");
//     }
// }

// // implement release functionality
// void release_memory(char *process_name) {
//     temp = head;
//     struct MemoryBlock *prev = head;
//     while (temp->next != NULL) {
//         if (strcmp(temp->next->status, process_name) == 0) {
//             strcpy(temp->next->status, "Unused");
//             temp->next->space_avail = temp->next->end_address - temp->next->start_address + 1;
//             head->space_avail = head->space_avail + temp->next->space_avail;
//             // print status of next block 
//             // printf("Status of next block after the one i removed lol: %s\n", temp->next->next->status);
//             // check if next block is unused
//             if (strcmp(temp->next->next->status, "Unused") == 0) {
//                 // print temp start address and temp next start address
//                 printf("Temp start address: %d\n", temp->next->start_address);
//                 printf("Temp next start address: %d\n", temp->next->next->start_address);
//                 temp->next->next->start_address = temp->next->start_address;
//                 temp->next->space_avail = temp->next->space_avail + temp->next->next->space_avail;
//                 temp->next->end_address = temp->next->next->end_address;
//                 temp->next->next = temp->next->next->next;
//             }
//             // check if previous block is unused
//             if (prev != head && strcmp(prev->status, "Unused") == 0) {
//                 // print prev start address and temp next start address
//                 printf("Prev start address: %d\n", prev->start_address);
//                 printf("Temp next start address: %d\n", temp->next->start_address);
//                 prev->start_address = temp->next->start_address;
//                 prev->space_avail = prev->space_avail + temp->next->space_avail;
//                 prev->end_address = temp->next->end_address;
//                 prev->next = temp->next->next;
//             }
//             condense_holes();
//             return;
//         }
//         else {
//             prev = temp;
//             temp = temp->next;
//         }
//     }
//     // if process not found 
//     printf("Process %s not found.\n", process_name);
// }

// // implement compact holes functionality 
// void compact_memory() {
//     /*

//     loop through and find processes and holes
//     - put all processes into a new linked list 
//     - put all holes into one big hole 
//     - remove the item you find 

//     put the linked list of processes back into the memory block 

//     put the holes in at the back of the memory block 

//     change the start and end addresses of the processes and holes -- TODO 

//     go through each hole in the compaction list and move it up in the array (valid)

//     changing the start and end addresses of each element in the holes and process lists

//     TWO LIST (not linked, array of pointers)
//     - hole list to keep track of all holes (do this with everything, allocate all holes in the array)
//     - partition list that kept track of all allocated partitions (" ")
//     - length 100 

//     1. order holes
//     2. if two holes adjacent, compact 


//     */


//    // sort things (sort the holes)
//    // go through that list and move everything up 





// /*

// flag valid hole is available -- checking to see if the hole is actually available (see if it doesn't have a process in it)




// PSEUDOCODE 

// can have a helper function isAdjacent
// - if addresses of those two are right next to each other, return true (otherwise return false)

// order holes (put the holes list in order)

// set up a loop:
// while (compacting) 
//     break out of it 
//     check to see if the hole is valid 

//     keep track of predecessor, successor, beginning, and end (if theres any holes that come before)

//     loop through the number of holes 
//         check to see if the hole is valid (check predecessor, hole list of the one before)
//         find a valid successor 
//         check to see if the value you have looped through is equal to number of holes -- if YES, then you're done (if count == numHoles)
//             break 



// */

    



//     temp2 = head->next;
//     // new memory block for all processes
//     struct MemoryBlock *new_mem_head = NULL;
//     bool found_hole = false;
//     bool found_process = false;
//     // new memory block for all holes
//     struct MemoryBlock *holes_head = NULL;

//     while (temp2->next != NULL) {
//         // check if current item is a hole (status = unused)
//         if (strcmp(temp2->next->status, "Unused") == 0 && !found_hole) {
//             holes_head = temp2->next;
//             found_hole = true;
//             temp2 = temp2->next;
//             continue;

//         }
//         else if (!found_process) {
//             // add process to new memory block linked list
//             new_mem_head = temp2->next;
//             found_process = true;
//             temp2 = temp2->next;
//             continue;
//         }
//         if (found_hole) {
//             // check if current item is a hole (status = unused)
//             if (strcmp(temp2->next->status, "Unused") == 0) {
//                 // add holes to the end of the new memory block linked list
//                 // print temp next start address
//                 printf("Temp next start address: %d\n", temp2->next->start_address);
//                 // print temp next end address
//                 printf("Temp next end address: %d\n", temp2->next->end_address);
//                 holes_head->next = temp2->next;
//                 // how to increment the current hole without losing access to the head later on???

//                 // holes_head->next = holes_head->next->next;
//             }
//         }
//         if (found_process) {
//             // add processes to the end of the new memory block linked list
//             new_mem_head->next = temp2->next;
//             // new_mem_head->next = new_mem_head->next->next;
//         }

//         printf("Temp start address: %d\n", temp2->start_address);
//         // printf("Temp next start address: %d\n", temp2->next->start_address);

//         // print temp status
        
//     }
//     // change head to point to the new memory block linked list
//     head = new_mem_head;
//     // loop through until the end of processes list 
//     while (new_mem_head->next != NULL) {
//         // print out the new memory block linked list 
//         printf("New memory block: %s\n", new_mem_head->status);
//         new_mem_head = new_mem_head->next;
//     }
//     // add holes to the end of the new memory block linked list
//     new_mem_head->next = holes_head;
  
    
//     // print out the new memory head 
//     printf("New memory head: %s\n", head->status);
//     condense_holes();
// }

// // implement report status function 
// void report_status() {

//     // NEW 
//     for (int i = 0; i < num_processes; ++i) {
//         printf("Block size: %d, Start Address %d, Process ID: %s\n", process_list[i]->size, process_list[i]->start_address, process_list[i]->process_name);
//     }
//     for (int j = 0; j < num_holes; ++j) {
//         printf("Block size: %d, Start Address %d, Valid Bit: %d\n", hole_list[j]->size, hole_list[j]->start_address, hole_list[j]->valid);
//     }





//     temp = head;
//     printf("Memory Block:\n\n");
//     while (temp->next != NULL) {
//         printf("Start Address: %d\n", temp->next->start_address);
//         printf("End Address: %d\n", temp->next->end_address);
//         printf("Size: %d\n", temp->next->space_avail);
//         // check if hole 
//         if (strcmp(temp->next->status, "Unused") == 0) {
//             printf("Status: %s\n", temp->next->status);
//         }
//         else {
//             printf("Status: Process %s\n", temp->next->status);
//         }
//         printf("\n");
//         temp = temp->next;
//     }
//     printf("\n");
// }

// int main(int argc, char *argv[]) {
//     // check for correct amount of parameters 
//     if (argc != 2) {
//         printf("Usage: ./allocator <memory_size>\n");
//         return -1;
//     }
    
//     initial_memory_size = atoi(argv[1]);

//     // check for valid parameter
//     if (initial_memory_size <= 0) {
//         printf("Invalid memory size.\n");
//         return 1;
//     }

//     // initialize the contiguous memory block
//     initialize_memory(initial_memory_size);

//     char command[10];
//     char process_name[10];
//     int size;
//     char strategy;

//     // run logic for memory allocation simulation 
//     while (1) {
//         printf("allocator> ");
//         scanf("%s", command);
        

//         // NEW 
//         fgets(request, 16, stdin);
//         if (strncmp(request, "RQ", 2) == 0) {
//             process_name = strtok((request+2), " \n");
//             size = strtok(NULL, " \n");
//             strategy = strtok(NULL, " \n");
//             request_memory(process_name, size, strategy);
//         }


//         if (strcmp(command, "RQ") == 0) {
//             scanf("%s %d %c", process_name, &size, &strategy);
//             // increment num_processes
//             // num_processes++;
//             printf("Request Memory: %s %d %c\n", process_name, size, strategy);
//             request_memory(process_name, size, strategy);
//         } else if (strcmp(command, "RL") == 0) {
//             scanf("%s", process_name);
//             printf("Release Memory: %s\n", process_name);
//             release_memory(process_name);
//         } else if (strcmp(command, "C") == 0) {
//             printf("Compact Memory\n");
//             compact_memory();
//         } else if (strcmp(command, "STAT") == 0) {
//             printf("Report Status\n");
//             report_status();
//         } else if (strcmp(command, "X") == 0) {
//             break;
//         } else {
//             printf("Invalid command.\n");
//         }
//     }
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// initialize global variables
int initial_memory_size;
int num_processes = 0;
struct MemoryBlock *head;
struct MemoryBlock *initial_block;
struct MemoryBlock *temp;
struct MemoryBlock *prev;
// struct MemoryBlock *temp2;
int last_address_space;
int space_requested;

// structure to represent a block of memory
struct MemoryBlock {
    int start_address;
    int end_address;
    int space_avail;
    char status[10];
    struct MemoryBlock *next;
};

// function to condense all holes into one big hole 
void condense_holes() {
    temp = head;
    while (temp != NULL && temp->next != NULL) {
        if (strcmp(temp->status, "Unused") == 0 && strcmp(temp->next->status, "Unused") == 0) {
            temp->next->start_address = temp->start_address;
            temp->space_avail = temp->space_avail + temp->next->space_avail;
            temp->end_address = temp->next->end_address;
            struct MemoryBlock *temp_next = temp->next;
            temp->next = temp_next->next;
            if (temp_next->next != NULL) {
                temp_next = temp->next;
            }
            // free(temp_next);
        } else {
            temp = temp->next;
        }
    }
}

// initialize memory block 
void initialize_memory(int size) {
    head = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
    initial_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
    if (head == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    head->start_address = -1;
    head->end_address = -1;
    head->space_avail = size;
    head->next = initial_block;
    strcpy(head->status, "Unused");

    strcpy(initial_block->status, "Unused");
    initial_block->start_address = 0;
    initial_block->end_address = size;
    initial_block->space_avail = size;
    initial_block->next = NULL;

    last_address_space = size;
}

// implement first fit request functionality
void first_fit_request(char *process_name, int size) {
    temp = head;
    int leftover_space = 0;

    while (temp->next != NULL) {
        if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
            head->space_avail = head->space_avail - size;

            strcpy(temp->next->status, process_name);
            temp->next->end_address = temp->next->start_address + size - 1;

            leftover_space = temp->next->space_avail - size;

            if (leftover_space > 0) {
                struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
                strcpy(new_block->status, "Unused");
                new_block->space_avail = leftover_space;
                temp->next->space_avail = size;
                new_block->start_address = temp->next->end_address + 1;
                new_block->end_address = new_block->start_address + leftover_space - 1;
                if (new_block->end_address > last_address_space) {
                    new_block->end_address = last_address_space;
                }
                new_block->next = temp->next->next;
                temp->next->next = new_block;
            }
            return;
        } 
    else {
        temp = temp->next;
    }  
    }
    // if no space found 
    printf("There is no space available for process %s.\n", process_name);
}

// implement best fit request functionality
void best_fit_request(char *process_name, int size) {
    temp = head;
    int leftover_space = 0;
    int best_fit = last_address_space;
    struct MemoryBlock *best_fit_block = NULL;

    // loop through memory blocks to find best fit hole (big enough to fit process)
    while (temp->next != NULL) {
        // print temp variable status 
        printf("Temp status: %s\n", temp->next->status);
        if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
            printf("Space available: %d\n", temp->next->space_avail);
            // print the status of the next block
            printf("Next block status: %s\n", temp->next->status);
            if (temp->next->space_avail < best_fit) {
                // print im here for debugging 
                printf("Im here\n");
                // print the space available 
                best_fit = temp->next->space_avail;
                best_fit_block = temp;
            }
        }
        temp = temp->next;
    }

    if (best_fit_block != NULL) {
        head->space_avail = head->space_avail - size;

        strcpy(best_fit_block->next->status, process_name);
        best_fit_block->next->end_address = best_fit_block->next->start_address + size - 1;

        leftover_space = best_fit_block->next->space_avail - size;

        if (leftover_space > 0) {
            struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
            strcpy(new_block->status, "Unused");
            new_block->space_avail = leftover_space;
            best_fit_block->next->space_avail = size;
            new_block->start_address = best_fit_block->next->end_address + 1;
            new_block->end_address = new_block->start_address + leftover_space - 1;
            if (new_block->end_address > last_address_space) {
                new_block->end_address = last_address_space;
            }
            new_block->next = best_fit_block->next->next;
            best_fit_block->next->next = new_block;
        }
    } else {
        printf("There is no space available for process %s.\n", process_name);
    }
}

// implement worst fit request functionality
void worst_fit_request(char *process_name, int size) {
    temp = head;
    int leftover_space = 0;
    int worst_fit = 0;
    struct MemoryBlock *worst_fit_block = NULL;

    // loop through memory blocks to find worst fit hole (big enough to fit process)
    while (temp->next != NULL) {
        // print temp variable status 
        printf("Temp status: %s\n", temp->next->status);
        if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
            printf("Space available: %d\n", temp->next->space_avail);
            // print the status of the next block
            printf("Next block status: %s\n", temp->next->status);
            if (temp->next->space_avail > worst_fit) {
                // print im here for debugging 
                printf("Im here\n");
                // print the space available 
                worst_fit = temp->next->space_avail;
                worst_fit_block = temp;
            }
        }
        temp = temp->next;
    }

    if (worst_fit_block != NULL) {
        head->space_avail = head->space_avail - size;

        strcpy(worst_fit_block->next->status, process_name);
        worst_fit_block->next->end_address = worst_fit_block->next->start_address + size - 1;

        leftover_space = worst_fit_block->next->space_avail - size;

        if (leftover_space > 0) {
            struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
            strcpy(new_block->status, "Unused");
            new_block->space_avail = leftover_space;
            worst_fit_block->next->space_avail = size;
            new_block->start_address = worst_fit_block->next->end_address + 1;
            new_block->end_address = new_block->start_address + leftover_space - 1;
            if (new_block->end_address > last_address_space) {
                new_block->end_address = last_address_space;
            }
            new_block->next = worst_fit_block->next->next;
            worst_fit_block->next->next = new_block;
        }
    } else {
        printf("There is no space available for process %s.\n", process_name);
    }
}

// implement request functionality 
void request_memory(char *process_name, int size, char strategy) {
    if (strategy == 'F') {
        first_fit_request(process_name, size);
    } else if (strategy == 'B') {
        best_fit_request(process_name, size);
    } else if (strategy == 'W') {
        worst_fit_request(process_name, size);
    } else {
        printf("Invalid strategy.\n");
    }
}

// implement release functionality
void release_memory(char *process_name) {
    temp = head;
    prev = head;
    while (temp != NULL) {
        if (strcmp(temp->status, process_name) == 0) {
            if (strcmp(temp->status, head->next->status) == 0) {
                printf("HEAD DELETE\n");
                if (temp->next != NULL && strcmp(temp->next->status, "Unused") == 0) {
                    temp->end_address = temp->next->end_address;
                    temp->space_avail += temp->next->space_avail;
                    temp->next = temp->next->next;
                    head->next = temp;
                    strcpy(temp->status, "Unused");
                    return;
                }
                else {
                    strcpy(head->next->status, "Unused");
                    printf("HEAD DELETE WITH FAT PROCESS BEHIND IT YUFHGU\n");
                    return;
                }
                
            }
            strcpy(temp->status, "Unused");
            // temp->space_avail = temp->end_address - temp->start_address + 1; 
            // head->space_avail = head->space_avail + temp->next->space_avail;

            //  print status of next block 
            // printf("Status of next block after the one i removed lol: %s\n", temp->next->next->status);
            // check if next block is unused
            if (strcmp(temp->next->status, "Unused") == 0) {
                // print temp start address and temp next start address
                // printf("Temp start address: %d\n", temp->start_address);
                // printf("Temp next start address: %d\n", temp->next->start_address);
                temp->end_address = temp->next->end_address;
                temp->space_avail += temp->next->space_avail;
                temp->next->start_address = temp->start_address;
                temp->next->space_avail = temp->space_avail;
                // print temp space avail
                printf("Space avail after adding temp next space avail: %d\n", temp->space_avail);
                printf("next space avail value: %d\n", temp->next->space_avail);
                if (temp->space_avail == initial_memory_size) {
                    head = temp;
                    return;
                }
                // if (temp->next->next != NULL) {
                // print temp next 
                temp->next = temp->next->next;
                // }
            }
            // check if previous block is unused
            // prev != head && 
            if (strcmp(prev->status, "Unused") == 0) {
                // print prev start address and temp next start address
                // printf("Prev start address: %d\n", prev->start_address);
                // printf("Temp start address: %d\n", temp->start_address);
                temp->start_address = prev->start_address;
                temp->space_avail += prev->space_avail;
                // print prev space avail
                printf("CURRENT temp space avail after adding prev space avail: %d\n",  temp->space_avail);
                printf("previous space avail: %d\n", prev->space_avail);
                prev->end_address = temp->end_address;
                prev->next = temp->next;
                prev->space_avail = temp->space_avail;
                if (temp->space_avail == initial_memory_size) {
                    head->next = temp;
                    return;
                }
            }
            condense_holes();
            return;
        }
        else {
            prev = temp;
            if (temp->next == NULL) {
                break;
            }
            temp = temp->next;
            printf("PREV STATUS: %s\n", prev->status);
            printf("TEMP NEXT STATUS: %s\n", temp->status);
        }
    }
    // 
    // if process not found 
    printf("Process %s not found.\n", process_name);
}

// implement compact holes functionality 
void compact_memory() {
    /*

    loop through and find processes and holes
    - put all processes into a new linked list 
    - put all holes into one big hole 
    - remove the item you find 

    put the linked list of processes back into the memory block 

    put the holes in at the back of the memory block 

    */
    // temp2 = head->next;
    // // new memory block for all processes
    // struct MemoryBlock *new_mem_head = NULL;
    // bool found_hole = false;
    // bool found_process = false;
    // // new memory block for all holes
    // struct MemoryBlock *holes_head = NULL;


    // create a temporary list (second list)
    // when you find a valid process, add it to the front 

    // copy it over

    // bring it back

    // overwrite old one 

    struct MemoryBlock *new_mem_head = NULL;
    struct MemoryBlock *last_process = new_mem_head;

    temp = head->next;

    int hole_size = 0;
    int process_size = 0;


    int start = 0;
    // int end = 0;

    // find all processes 
    while (temp != NULL) {
        if (strcmp(temp->status, "Unused") != 0) {
            // print if process found 
            printf("Process found: %s\n", temp->status);
            struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
            strcpy(new_block->status, temp->status);
            new_block->space_avail = temp->space_avail;
            new_block->start_address = start;
            new_block->end_address = start + temp->space_avail - 1;
            start = new_block->end_address + 1;
            process_size += temp->space_avail;
            // end += temp->space_avail;
            if (new_mem_head == NULL) {
                new_mem_head = new_block;
                temp = temp->next;
                continue;
            }
            last_process = new_mem_head;
            while (last_process->next != NULL) {
                last_process = last_process->next;
            }
            printf("Made a new head: %s\n", temp->status);
            last_process->next = new_block;
        }
        else {
            printf("Hole found: %d\n", temp->space_avail);
            hole_size += temp->space_avail;
        }
        temp = temp->next;
    }
    // if no holes were found, return 
    if (hole_size == 0) {
        printf("No holes found.\n");
        return;
    }
    // create a new hole block and put it at the end of the memory block
    struct MemoryBlock *new_hole = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
    strcpy(new_hole->status, "Unused");
    new_hole->space_avail = hole_size;
    new_hole->start_address = process_size;
    new_hole->end_address = initial_memory_size - 1;
    // add hole to back of memory block 
    temp = new_mem_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_hole;
    // print if put in hole 
    printf("Put in hole: %d\n", hole_size);

    head->next = new_mem_head;

    // print the head 
    printf("NEW HEAD: %s\n", head->status);





    // while (temp2->next != NULL) {
    //     // check if current item is a hole (status = unused)
    //     if (strcmp(temp2->next->status, "Unused") == 0 && !found_hole) {
    //         holes_head = temp2->next;
    //         found_hole = true;
    //         temp2 = temp2->next;
    //         continue;

    //     }
    //     else if (!found_process) {
    //         // add process to new memory block linked list
    //         new_mem_head = temp2->next;
    //         found_process = true;
    //         temp2 = temp2->next;
    //         continue;
    //     }
    //     if (found_hole) {
    //         // check if current item is a hole (status = unused)
    //         if (strcmp(temp2->next->status, "Unused") == 0) {
    //             // add holes to the end of the new memory block linked list
    //             // print temp next start address
    //             printf("Temp next start address: %d\n", temp2->next->start_address);
    //             // print temp next end address
    //             printf("Temp next end address: %d\n", temp2->next->end_address);
    //             holes_head->next = temp2->next;
    //             // how to increment the current hole without losing access to the head later on???

    //             // holes_head->next = holes_head->next->next;
    //         }
    //     }
    //     if (found_process) {
    //         // add processes to the end of the new memory block linked list
    //         new_mem_head->next = temp2->next;
    //         // new_mem_head->next = new_mem_head->next->next;
    //     }

    //     printf("Temp start address: %d\n", temp2->start_address);
    //     // printf("Temp next start address: %d\n", temp2->next->start_address);


    //     // print temp status
        
    // }
    // // change head to point to the new memory block linked list
    // head = new_mem_head;
    // // loop through until the end of processes list 
    // while (new_mem_head->next != NULL) {
    //     // print out the new memory block linked list 
    //     printf("New memory block: %s\n", new_mem_head->status);
    //     new_mem_head = new_mem_head->next;
    // }
    // // add holes to the end of the new memory block linked list
    // new_mem_head->next = holes_head;
  
    
    // // print out the new memory head 
    // printf("New memory head: %s\n", head->status);
    // condense_holes();
}

// implement report status function 
void report_status() {
    temp = head;
    printf("Memory Block:\n\n");
    while (temp->next != NULL) {
        printf("Start Address: %d\n", temp->next->start_address);
        printf("End Address: %d\n", temp->next->end_address);
        printf("Size: %d\n", temp->next->space_avail);
        // check if hole 
        if (strcmp(temp->next->status, "Unused") == 0) {
            printf("Status: %s\n", temp->next->status);
        }
        else {
            printf("Status: Process %s\n", temp->next->status);
        }
        printf("\n");
        temp = temp->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    // check for correct amount of parameters 
    if (argc != 2) {
        printf("Usage: ./allocator <memory_size>\n");
        return 1;
    }
    
    initial_memory_size = atoi(argv[1]);

    // check for valid parameter
    if (initial_memory_size <= 0) {
        printf("Invalid memory size.\n");
        return 1;
    }

    // initialize the contiguous memory block
    initialize_memory(initial_memory_size);

    char command[10];
    char process_name[10];
    int size;
    char strategy;

    // run logic for memory allocation simulation 
    while (1) {
        printf("allocator> ");
        scanf("%s", command);

        if (strcmp(command, "RQ") == 0) {
            scanf("%s %d %c", process_name, &size, &strategy);
            // increment num_processes
            num_processes++;
            printf("Request Memory: %s %d %c\n", process_name, size, strategy);
            request_memory(process_name, size, strategy);
        } else if (strcmp(command, "RL") == 0) {
            scanf("%s", process_name);
            printf("Release Memory: %s\n", process_name);
            release_memory(process_name);
        } else if (strcmp(command, "C") == 0) {
            printf("Compact Memory\n");
            compact_memory();
        } else if (strcmp(command, "STAT") == 0) {
            printf("Report Status\n");
            report_status();
        } else if (strcmp(command, "X") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }
}