#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// initialize global variables 
int page_table[256];
struct TLB {
    int page[16];
    int frame[16];
    int index;
};
int free_frame = 0;
int page_faults = 0;
int tlb_hits = 0;

// function prototypes
void get_page(int logical, int* page_table, struct TLB *tlb, int physical_memory[][256]);
int get_frame(int page_number, int physical_memory[][256]);

// function to get the page number
void get_page(int logical, int* page_table, struct TLB *tlb, int physical_memory[][256]) {
    // bit manipulation to get page number and offset
    int page_number = (logical >> 8) & 0xFF;
    int offset = logical & 0xFF;
    int frame_number = -1;
    bool tlb_hit = false;

    // check if page number is in TLB
    for (int i = 0; i < 16; i++) {
        if (tlb->page[i] == page_number) {
            frame_number = tlb->frame[i];
            tlb_hits++;
            tlb_hit = true;
            // if in TLB, stop searching
            break; 
        }
    }

    // check if page number is in page table
    if (frame_number == -1) {
        frame_number = page_table[page_number];
        // if frame was not found in page table, increment page fault
        if (frame_number == -1) {
            frame_number = get_frame(page_number, physical_memory);
            page_table[page_number] = frame_number;
            page_faults++;
        }
    }

    // update TLB
    if (!tlb_hit) {
        tlb->page[tlb->index] = page_number;
        tlb->frame[tlb->index] = frame_number;
        tlb->index = (tlb->index + 1) % 16;
    }

    // get the value from physical memory
    int value = (int8_t) physical_memory[frame_number][offset];
    printf("Virtual address: %d Physical address: %d Value: %d\n", logical, (frame_number << 8) | offset, value);
}

// function to get the frame number from backing store 
int get_frame(int page_number, int physical_memory[][256]) {
    // open the backing store file
    FILE* store = fopen("BACKING_STORE.bin", "rb");
    if (store == NULL) {
        printf("Error: Cannot open BACKING_STORE.bin\n");
        exit(1);
    }

    // seek to the correct page number
    if (fseek(store, page_number * 256, SEEK_SET) != 0) {
        printf("Error: Cannot seek to page number %d\n", page_number);
        exit(1);
    }

    // create a buffer 
    unsigned char buffer[256];

    // read the contents of the backing store
    if (fread(buffer, 1, 256, store) == 0) {
        printf("Error: Cannot read from BACKING_STORE.bin\n");
        exit(1);
    }

    // copy the values of each byte from the store to the corresponding location in the physical memory array 
    for (int i = 0; i < 256; i++) {
        physical_memory[free_frame][i] = buffer[i];
    }

    // close the backing store file
    fclose(store);

    // return and increment the frame number
    return free_frame++;
}

int main(int argc, char* argv[]) {

    // initialize variables
    int buffer;
    int lines = 0;
    // make the page table 
    memset(page_table, -1, sizeof(page_table));
    // make the TLB
    struct TLB tlb;
    memset(tlb.page, -1, sizeof(tlb.page));
    memset(tlb.frame, -1, sizeof(tlb.frame));
    tlb.index = 0;

    // create physical memory 
    int physical_memory[256][256];

    // check if user entered correct number of arguments
    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    // open user input file
    FILE* file = fopen(argv[1], "r");

    // check if file was opened successfully
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    // read the contents of the file 
    while (fscanf(file, "%d", &buffer) == 1) {
        get_page(buffer, page_table, &tlb, physical_memory);
        ++lines;
    }

    // calculate page fault rate
    double page_fault_rate = (double)page_faults / lines;

    // calculate TLB hit rate
    double tlb_hit_rate = (double)tlb_hits / lines;

    // print the results
    printf("Page Fault Rate: %.3f\n", page_fault_rate);
    printf("TLB Hit Rate: %.3f\n", tlb_hit_rate);

    // close the file
    fclose(file);

    return 0;
}
