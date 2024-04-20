
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure to represent a block of memory
typedef struct {
    int start_address;
    int size;
    char status[10];
    struct MemoryBlock *next;
} MemoryBlock;

// structure to represent a process
typedef struct {
    int start_address;
    int size;
    char name[10];
} Process;

// initialize global variables
int initial_memory_size;
int num_processes = 0;
struct MemoryBlock *memory = NULL;
struct Process *processes = NULL;

// initialize memory block 
void initialize_memory(int size) {
    memory = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    if (memory == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    memory[0].start_address = 0;
    memory[0].size = size;
    strcpy(memory[0].status, "Unused");
}

// implement request functionality 
void request_memory(char *process_name, int size, char strategy) {
    int i;
    int index = -1;
    

    // check if inputted size is valid 
    if (size <= 0 || size > initial_memory_size) {
        printf("Invalid memory request size.\n");
        --num_processes;
        return;
    }

    // implement first fit strategy
    if (strategy == 'F') {
        // check if process already exists
        for (i = 0; i < num_processes; i++) {
            if (strcmp(processes[i].name, process_name) == 0) {
                printf("Process %s already exists.\n", process_name);
                --num_processes;
                return;
            }
        }

        // find the first hole that fits the process
        for (i = 0; i < num_processes; i++) {
            if (strcmp(memory[i].status, "Unused") == 0 && memory[i].size >= size) {
                index = i;
                break;
            }
        }

        // check if no hole is found
        if (index == -1) {
            printf("Memory allocation failed: no hole found.\n");
            --num_processes;
            return;
        }

        // print out all elements in memory array 
            for (i = 0; i < num_processes + 1; i++) {
                printf("Memory[%d]: start_address = %d, size = %d, status = %s\n", i, memory[i].start_address, memory[i].size, memory[i].status);
            }

        // allocate memory for the process
        processes[index].size = size;
        strcpy(processes[index].name, process_name);
        processes[index].start_address = memory[index].start_address;
        strcpy(memory[index].status, "Used");
        // print new process's index 
        printf("Process %s created with size %d starting at address %d.\n", process_name, size, memory[index].start_address);

        // update the hole (account for process being smaller than hole found)
        if (processes[index].size < memory[index].size) {
            // memory = (MemoryBlock *)realloc(memory, (num_processes + 1) * sizeof(MemoryBlock));
            struct MemoryBlock new_memory = (MemoryBlock *)malloc(sizeof(MemoryBlock));
            memory->next = &new_memory;
            new_memory.start_address = size + 1;
            new_memory.size = memory[index].size - size;
            memory[index].size = size;
            strcpy(new_memory.status, "Unused");

            // print out all elements in memory array 
            for (i = 0; i < num_processes + 1; i++) {
                printf("Memory[%d]: start_address = %d, size = %d, status = %s\n", i, memory[i].start_address, memory[i].size, memory[i].status);
            }

        } else {
            strcpy(memory[index].status, "Used");
        }
    } 
    // implement 
    
    
    
    else {
        printf("Invalid strategy.\n");
    }
}

// implement report status function 
void report_status() {
    int i;
    printf("Memory Status\n");
    printf("-------------\n");
    for (i = 0; i <= num_processes; i++) {
        // printf("STATUS: %s\n", memory[i].status);
        if (strcmp(memory[i].status, "Unused") == 0) {
            // printf("Hole: %d bytes starting at address %d\n", memory[i].size, memory[i].start_address);
            // print out hole like [start_address, size]
            printf("Hole: ");
            printf("Starting Address %d, Size %d\n", memory[i].start_address, memory[i].size);
        }
        else {
            // print out hole like [start_address, size]
            printf("Process %s: ", processes[i].name);
            printf("Starting Address %d, Size %d\n", processes[i].start_address, processes[i].size);
            // printf("Process %s: %d bytes starting at address %d\n", processes[i].name, processes[i].size, processes[i].start_address);
            
        }
    }
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
    processes = (Process *)realloc(processes, (num_processes) * sizeof(Process));

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
            // release_memory(process_name);
        } else if (strcmp(command, "C") == 0) {
            printf("Compact Memory\n");
            // compact_memory();
        } else if (strcmp(command, "STAT") == 0) {
            printf("Report Status\n");
            report_status();
        } else if (strcmp(command, "X") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }
    free(memory);
    free(processes);
}