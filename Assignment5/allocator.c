#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// initialize global variables
int initial_memory_size;
struct MemoryBlock *head;
struct MemoryBlock *initial_block;
struct MemoryBlock *temp;
struct MemoryBlock *prev;
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

// function prototypes
void condense_holes();
void initialize_memory(int size);
void first_fit_request(char *process_name, int size);
void best_fit_request(char *process_name, int size);
void worst_fit_request(char *process_name, int size);
void request_memory(char *process_name, int size, char strategy);
void release_memory(char *process_name);
void compact_memory();
void report_status();

// function to condense all holes into one big hole 
void condense_holes() {
    temp = head;
    while (temp != NULL && temp->next != NULL) {
        // if two adjacent blocks are holes 
        if (strcmp(temp->status, "Unused") == 0 && strcmp(temp->next->status, "Unused") == 0) {
            temp->next->start_address = temp->start_address;
            temp->space_avail = temp->space_avail + temp->next->space_avail;
            temp->end_address = temp->next->end_address;
            struct MemoryBlock *temp_next = temp->next;
            temp->next = temp_next->next;
            if (temp_next->next != NULL) {
                temp_next = temp->next;
            }
            free(temp_next);
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

    // Create memory block with specified size 
    strcpy(initial_block->status, "Unused");
    initial_block->start_address = 0;
    initial_block->end_address = size - 1;
    initial_block->space_avail = size;
    initial_block->next = NULL;

    last_address_space = size;
}

// implement first fit request functionality
void first_fit_request(char *process_name, int size) {
    temp = head;
    int leftover_space = 0;

    while (temp->next != NULL) {
        // find the first hole that is big enough to fit the process
        if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
            // decrease the total space available by the new process size
            head->space_avail = head->space_avail - size;
            
            // update status of hole ==> process name
            strcpy(temp->next->status, process_name);
            temp->next->end_address = temp->next->start_address + size - 1;

            leftover_space = temp->next->space_avail - size;

            // create a hole for the leftover space from the process being allocated
            if (leftover_space > 0) {
                struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
                strcpy(new_block->status, "Unused");
                new_block->space_avail = leftover_space;
                temp->next->space_avail = size;
                new_block->start_address = temp->next->end_address + 1;
                new_block->end_address = new_block->start_address + leftover_space - 1;
                // set the new block's address space to the last address space if at the end
                if (new_block->end_address >= last_address_space) {
                    new_block->end_address = last_address_space;
                }
                new_block->next = temp->next->next;
                temp->next->next = new_block;
            }

            return;
        } else {
            temp = temp->next;
        }  
    }
    // if not enough space for process size
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
        if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
            if (temp->next->space_avail <= best_fit) {
                best_fit = temp->next->space_avail;
                best_fit_block = temp;
            }
        }
        temp = temp->next;
    }

    // if best fit block is found
    if (best_fit_block != NULL) {
        head->space_avail = head->space_avail - size;

        strcpy(best_fit_block->next->status, process_name);
        best_fit_block->next->end_address = best_fit_block->next->start_address + size - 1;

        leftover_space = best_fit_block->next->space_avail - size;

        // create a new hole for leftover space left from process allocation
        if (leftover_space > 0) {
            struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
            strcpy(new_block->status, "Unused");
            new_block->space_avail = leftover_space;
            best_fit_block->next->space_avail = size;
            new_block->start_address = best_fit_block->next->end_address + 1;
            new_block->end_address = new_block->start_address + leftover_space - 1;
            if (new_block->end_address >= last_address_space) {
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
        if (strcmp(temp->next->status, "Unused") == 0 && temp->next->space_avail >= size) {
            if (temp->next->space_avail > worst_fit) {
                worst_fit = temp->next->space_avail;
                worst_fit_block = temp;
            }
        }
        temp = temp->next;
    }

    // if worst fit block is found
    if (worst_fit_block != NULL) {
        head->space_avail = head->space_avail - size;

        strcpy(worst_fit_block->next->status, process_name);
        worst_fit_block->next->end_address = worst_fit_block->next->start_address + size - 1;

        leftover_space = worst_fit_block->next->space_avail - size;

        // create a hole for the leftover space from the process being allocated
        if (leftover_space > 0) {
            struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
            strcpy(new_block->status, "Unused");
            new_block->space_avail = leftover_space;
            worst_fit_block->next->space_avail = size;
            new_block->start_address = worst_fit_block->next->end_address + 1;
            new_block->end_address = new_block->start_address + leftover_space - 1;
            if (new_block->end_address >= last_address_space) {
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
        // finding process within the memory block
        if (strcmp(temp->status, process_name) == 0) {
            // if process is the first item in the memory block
            if (strcmp(temp->status, head->next->status) == 0) {
                // if block next to the first process is a hole
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
                    return;
                }
            }

            // update the found process to a hole
            strcpy(temp->status, "Unused");

            // check if the next block is Unused ==> combine current hole with this one
            if (temp->next != NULL && (strcmp(temp->next->status, "Unused") == 0)) {
                temp->end_address = temp->next->end_address;
                temp->space_avail += temp->next->space_avail;
                temp->next->start_address = temp->start_address;
                temp->next->space_avail = temp->space_avail;
                if (temp->space_avail == initial_memory_size) {
                    head = temp;
                    return;
                }
                temp->next = temp->next->next;
            }

            // check if previous block is unused ==> combine current hole with this one
            if (strcmp(prev->status, "Unused") == 0) {
                // set start address to that of the previous block's
                temp->start_address = prev->start_address;
                // combine space available of both memory blocks
                temp->space_avail += prev->space_avail;
                // previous block's attributes will become those of the item after the current released process
                prev->end_address = temp->end_address;
                prev->next = temp->next;
                prev->space_avail = temp->space_avail;
                if (temp->space_avail == initial_memory_size) {
                    head->next = temp;
                    return;
                }
            }
            // condense any holes
            condense_holes();
            return;
        }
        else {
            // move onto next memory block
            prev = temp;
            if (temp->next == NULL) {
                break;
            }
            temp = temp->next;
        }
    }
    // if process was not found in entire memory block
    printf("Process %s not found.\n", process_name);
}

// implement compact holes functionality 
void compact_memory() {
    struct MemoryBlock *new_mem_head = NULL;
    struct MemoryBlock *last_process = new_mem_head;

    temp = head->next;

    int hole_size = 0;
    int process_size = 0;

    int start = 0;

    // find all processes 
    while (temp != NULL) {
        if (strcmp(temp->status, "Unused") != 0) {
            struct MemoryBlock *new_block = (struct MemoryBlock *)malloc(sizeof(struct MemoryBlock));
            strcpy(new_block->status, temp->status);
            new_block->space_avail = temp->space_avail;
            new_block->start_address = start;
            new_block->end_address = start + temp->space_avail - 1;
            start = new_block->end_address + 1;
            process_size += temp->space_avail;
            if (new_mem_head == NULL) {
                new_mem_head = new_block;
                temp = temp->next;
                continue;
            }
            last_process = new_mem_head;
            while (last_process->next != NULL) {
                last_process = last_process->next;
            }
            last_process->next = new_block;
        }
        else {
            // if the found hole is the initial empty memory block
            if (temp->space_avail == initial_memory_size) {
                return;
            }
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
    head->next = new_mem_head;
}

// implement report status function 
void report_status() {
    temp = head;
    printf("\nMemory Block:\n");
    printf("----------------------------\n");
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
        printf("----------------------------\n");
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
            printf("Request Memory: %s %d %c\n", process_name, size, strategy);
            request_memory(process_name, size, strategy);
        } else if (strcmp(command, "RL") == 0) {
            scanf("%s", process_name);
            printf("Release Memory: %s\n", process_name);
            release_memory(process_name);
        } else if (strcmp(command, "C") == 0) {
            printf("Compacted Memory\n");
            compact_memory();
        } else if (strcmp(command, "STAT") == 0) {
            report_status();
        } else if (strcmp(command, "X") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }
}