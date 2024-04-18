
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
struct MemoryBlock *temp2;
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
    while (temp->next != NULL && temp->next->next != NULL) {
        if (strcmp(temp->next->status, "Unused") == 0 && strcmp(temp->next->next->status, "Unused") == 0) {
            temp->next->next->start_address = temp->next->start_address;
            temp->next->space_avail = temp->next->space_avail + temp->next->next->space_avail;
            temp->next->end_address = temp->next->next->end_address;
            struct MemoryBlock *temp_next_next = temp->next->next;
            temp->next->next = temp_next_next->next;
            if (temp_next_next->next != NULL) {
                temp_next_next = temp->next;
            }
            free(temp_next_next);
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
    struct MemoryBlock *prev = head;
    while (temp->next != NULL) {
        if (strcmp(temp->next->status, process_name) == 0) {
            strcpy(temp->next->status, "Unused");
            temp->next->space_avail = temp->next->end_address - temp->next->start_address + 1;
            head->space_avail = head->space_avail + temp->next->space_avail;
            // print status of next block 
            // printf("Status of next block after the one i removed lol: %s\n", temp->next->next->status);
            // check if next block is unused
            if (strcmp(temp->next->next->status, "Unused") == 0) {
                // print temp start address and temp next start address
                printf("Temp start address: %d\n", temp->next->start_address);
                printf("Temp next start address: %d\n", temp->next->next->start_address);
                temp->next->next->start_address = temp->next->start_address;
                temp->next->space_avail = temp->next->space_avail + temp->next->next->space_avail;
                temp->next->end_address = temp->next->next->end_address;
                temp->next->next = temp->next->next->next;
            }
            // check if previous block is unused
            if (prev != head && strcmp(prev->status, "Unused") == 0) {
                // print prev start address and temp next start address
                printf("Prev start address: %d\n", prev->start_address);
                printf("Temp next start address: %d\n", temp->next->start_address);
                prev->start_address = temp->next->start_address;
                prev->space_avail = prev->space_avail + temp->next->space_avail;
                prev->end_address = temp->next->end_address;
                prev->next = temp->next->next;
            }
            condense_holes();
            return;
        }
        else {
            prev = temp;
            temp = temp->next;
        }
    }
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
    temp2 = head->next;
    // new memory block for all processes
    struct MemoryBlock *new_mem_head = NULL;
    bool found_hole = false;
    bool found_process = false;
    // new memory block for all holes
    struct MemoryBlock *holes_head = NULL;

    while (temp2->next != NULL) {
        // check if current item is a hole (status = unused)
        if (strcmp(temp2->next->status, "Unused") == 0 && !found_hole) {
            holes_head = temp2->next;
            found_hole = true;
            temp2 = temp2->next;
            continue;

        }
        else if (!found_process) {
            // add process to new memory block linked list
            new_mem_head = temp2->next;
            found_process = true;
            temp2 = temp2->next;
            continue;
        }
        if (found_hole) {
            // check if current item is a hole (status = unused)
            if (strcmp(temp2->next->status, "Unused") == 0) {
                // add holes to the end of the new memory block linked list
                // print temp next start address
                printf("Temp next start address: %d\n", temp2->next->start_address);
                // print temp next end address
                printf("Temp next end address: %d\n", temp2->next->end_address);
                holes_head->next = temp2->next;
                // how to increment the current hole without losing access to the head later on???

                // holes_head->next = holes_head->next->next;
            }
        }
        if (found_process) {
            // add processes to the end of the new memory block linked list
            new_mem_head->next = temp2->next;
            // new_mem_head->next = new_mem_head->next->next;
        }

        printf("Temp start address: %d\n", temp2->start_address);
        // printf("Temp next start address: %d\n", temp2->next->start_address);

        // printf("Temp status: %s\n", temp2->status);
        // printf("Temp NEXT BEECH status: %s\n", temp2->next->status);

        // printf("Temp next next status: %s\n", temp2->next->next->status);
        // printf("Temp NEXT next BEECH status: %s\n", temp2->next->next->status);

        // print temp status
        
    }
    // change head to point to the new memory block linked list
    head = new_mem_head;
    // loop through until the end of processes list 
    while (new_mem_head->next != NULL) {
        // print out the new memory block linked list 
        printf("New memory block: %s\n", new_mem_head->status);
        new_mem_head = new_mem_head->next;
    }
    // add holes to the end of the new memory block linked list
    new_mem_head->next = holes_head;
  
    
    // print out the new memory head 
    printf("New memory head: %s\n", head->status);
    condense_holes();
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