#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a block of memory
typedef struct {
    int start_address;
    int size;
    char status[10];
} MemoryBlock;

// Structure to represent a process
typedef struct {
    char name[10];
    int start_address;
    int size;
} Process;

// Function prototypes
void initialize_memory(int size);
void request_memory(char *process_name, int size, char strategy);
void release_memory(char *process_name);
void compact_memory();
void report_status();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./allocator <memory_size>\n");
        return 1;
    }

    int initial_memory_size = atoi(argv[1]);
    if (initial_memory_size <= 0) {
        printf("Invalid memory size.\n");
        return 1;
    }

    initialize_memory(initial_memory_size);

    char command[10];
    char process_name[10];
    int size;
    char strategy;

    while (1) {
        printf("allocator> ");
        scanf("%s", command);

        if (strcmp(command, "RQ") == 0) {
            scanf("%s %d %c", process_name, &size, &strategy);
            request_memory(process_name, size, strategy);
        } else if (strcmp(command, "RL") == 0) {
            scanf("%s", process_name);
            release_memory(process_name);
        } else if (strcmp(command, "C") == 0) {
            compact_memory();
        } else if (strcmp(command, "STAT") == 0) {
            report_status();
        } else if (strcmp(command, "X") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}

// Global variables
MemoryBlock *memory = NULL;
Process *processes = NULL;
int num_processes = 0;
int max_memory_size = 0;

// Initialize memory with one hole representing the whole memory
void initialize_memory(int size) {
    memory = (MemoryBlock *)malloc(sizeof(MemoryBlock));
    if (memory == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    memory[0].start_address = 0;
    memory[0].size = size;
    strcpy(memory[0].status, "Unused");

    max_memory_size = size;
}

// Request memory for a process using the specified strategy
void request_memory(char *process_name, int size, char strategy) {
    int i;
    int index = -1;
    if (size <= 0) {
        printf("Invalid memory request size.\n");
        return;
    }

    for (i = 0; i < num_processes; i++) {
        if (strcmp(processes[i].name, process_name) == 0) {
            printf("Process '%s' already has memory allocated.\n", process_name);
            return;
        }
    }

    for (i = 0; i < max_memory_size; i++) {
        if (strcmp(memory[i].status, "Unused") == 0 && memory[i].size >= size) {
            if (strategy == 'F' || (strategy == 'B' && (index == -1 || memory[i].size < memory[index].size)) ||
                (strategy == 'W' && (index == -1 || memory[i].size > memory[index].size))) {
                index = i;
            }
        }
    }

    if (index == -1) {
        printf("Insufficient memory to allocate.\n");
        return;
    }

    processes = (Process *)realloc(processes, (num_processes + 1) * sizeof(Process));
    if (processes == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    processes[num_processes].start_address = memory[index].start_address;
    processes[num_processes].size = size;
    strcpy(processes[num_processes].name, process_name);

    if (memory[index].size == size) {
        strcpy(memory[index].status, process_name);
    } else {
        memory = (MemoryBlock *)realloc(memory, (max_memory_size + 1) * sizeof(MemoryBlock));
        if (memory == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }

        memory[num_processes].start_address = memory[index].start_address;
        memory[num_processes].size = size;
        strcpy(memory[num_processes].status, process_name);

        memory[index].start_address += size;
        memory[index].size -= size;
        index = num_processes;
    }

    num_processes++;
    printf("Memory allocated for process '%s'.\n", process_name);
}

// Release memory allocated to a process
void release_memory(char *process_name) {
    int i, j;
    for (i = 0; i < num_processes; i++) {
        if (strcmp(processes[i].name, process_name) == 0) {
            for (j = 0; j < max_memory_size; j++) {
                if (memory[j].start_address + memory[j].size == processes[i].start_address ||
                    memory[j].start_address == processes[i].start_address + processes[i].size) {
                    memory[j].size += processes[i].size;
                    break;
                }
            }
            strcpy(memory[j].status, "Unused");
            printf("Memory released for process '%s'.\n", process_name);
            break;
        }
    }
    if (i == num_processes) {
        printf("Process '%s' has not allocated any memory.\n", process_name);
    } else {
        for (; i < num_processes - 1; i++) {
            processes[i] = processes[i + 1];
        }
        num_processes--;
    }
}

// Compact the memory by combining unused memory blocks into one
void compact_memory() {
    int i, j;
    for (i = 0; i < max_memory_size; i++) {
        if (strcmp(memory[i].status, "Unused") == 0) {
            for (j = i + 1; j < max_memory_size; j++) {
                if (strcmp(memory[j].status, "Unused") != 0) {
                    memory[i].size += memory[j].size;
                    strcpy(memory[j].status, "Unused");
                    break;
                }
            }
            if (j == max_memory_size) {
                break;
            }
        }
    }
    printf("Memory compacted.\n");
}

// Report the status of memory
void report_status() {
    printf("Memory Status:\n");
    int i;
    for (i = 0; i < max_memory_size; i++) {
        if (strcmp(memory[i].status, "Unused") == 0) {
            int j;
            for (j = i + 1; j < max_memory_size; j++) {
                if (strcmp(memory[j].status, "Unused") != 0) {
                    printf("Addresses [%d:%d] Unused\n", memory[i].start_address, memory[j - 1].start_address + memory[j - 1].size - 1);
                    i = j - 1;
                    break;
                }
            }
            if (j == max_memory_size) {
                printf("Addresses [%d:%d] Unused\n", memory[i].start_address, memory[i].start_address + memory[i].size - 1);
                break;
            }
        } else {
            printf("Addresses [%d:%d] Process %s\n", memory[i].start_address, memory[i].start_address + memory[i].size - 1, memory[i].status);
        }
    }
    if (i == max_memory_size) {
        printf("Addresses [%d:%d] Unused\n", memory[i - 1].start_address + memory[i - 1].size, max_memory_size - 1);
    }

    for (i = 0; i < num_processes; i++) {
        printf("Addresses [%d:%d] Process %s\n", processes[i].start_address, processes[i].start_address + processes[i].size - 1, processes[i].name);
    }
}
