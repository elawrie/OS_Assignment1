#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>

// set up general stuff in a c program 

// do the program logic

// use code snippit provided:
// struct timeval current;
// gettimeofday(&current,NULL);
// current.tv sec represents seconds
// current.tv usec represents microseconds

// do error handling (make sure user enters a valid command)

// fart

// main
int main(int argc, char** argv){
    // error checking -- make sure user enters a valid command
    if (argc != 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return -1;
    }

    // use shared memory to communicate between parent and child (send current time to parent) 
    // use posix shared memory

    // size of shared memory
    const int SIZE = 4096;

    // name of shared memory object
    const char *name = "/os";

    // shared memory file descriptor
    int shm_fd;

    // pointer to shared memory object
    void *ptr; 

    // open the shared memory object 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    // error checking for invalid file name 
    if (shm_fd == -1) {
        // perror("shm_open");
        printf("shared memory failed\n");
        exit(EXIT_FAILURE);
    }
    // // check for memory overwrite errors

    // print the pid 
    printf("pid: %d\n", getpid());

    ftruncate(shm_fd, SIZE);

    // wait 10 seconds 
    // sleep(10);

    // me trying to debug but it flopped so 

    // FILE *fp;
    // char path[1024];

    // // Open the /proc/self/maps file
    // fp = fopen("/proc/self/maps", "r");
    // if (fp == NULL) {
    //     perror("Error opening /proc/self/maps");
    //     return 1;
    // }

    // // Read and print each line
    // while (fgets(path, sizeof(path), fp) != NULL) {
    //     printf("%s", path);
    // }

    // // Close the file
    // fclose(fp);

    // print out the values of the arguments before calling mmap
    printf("shm_fd: %d, SIZE: %d\n", shm_fd, SIZE);
    
    // memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // check for error in creating shared memory
    if (ptr == MAP_FAILED) {
        close(shm_fd);
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // write to shared memory object 

    // fork a child process

    pid_t pid;
    char *message;
    int n;

    printf("fork program starting\n");
    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            message = "This is the child";
            n = 5;
            // get time of fuction call
            struct timeval current;
            gettimeofday(&current,NULL);
            // printf("Starting time: %ld\n", current.tv_sec);
            // // put the time into the shared memory object
            // sprintf(ptr, "%ld", current.tv_sec);

            printf("Starting time: %ld.%06d\n", current.tv_sec, current.tv_usec);
            // put the time into the shared memory object
            sprintf(ptr, "%ld.%06d", current.tv_sec, current.tv_usec);

            // execute the system call 
            execlp(argv[1], argv[1], NULL);
            break;
        default:
            message = "This is the parent";
            n = 3;
            
            // wait for the child to finish
            waitpid(pid, NULL, 0);
            // get time when system call is finished
            struct timeval endtime;
            gettimeofday(&endtime,NULL);
            // printf("ENDING time: %ld.%06d\n", endtime.tv_sec, endtime.tv_usec);

            // get the time from the shared memory object

            // this is giving a seg fault 
            printf("start time passed in %s\n", (char *)ptr);
            
            // remove the shared memory object
            shm_unlink(name);

            // calculate time elapsed during system call
            // int timeElapsed = endtime.tv_sec - atoi((char *)ptr);
            // printf("Time elapsed: %d\n", timeElapsed);

            char timeString[30]; 

            // format the time string and store it in the variable timeString
            snprintf(timeString, sizeof(timeString), "%ld.%06d", endtime.tv_sec, endtime.tv_usec);

            printf("END TIME: %s", timeString);

            // convert strings to doubles
            double number1 = strtod((char *)ptr, NULL);
            double number2 = strtod(timeString, NULL);

            double result = number2 - number1;

            printf("\nResult: %f\n", result);

            // printf("time elapsed: %s, timeElapsed")

            break;
    }

    // printf("running ps with execlp\n");
    
    // execlp("ps", "ps", "ax", 0);

    // read from the shared memory object
    // printf("%s", (char *)ptr);

    

    // return 0;
    
    // remove the shared memory object
    // free(ptr);
    // ptr = NULL;
    // free(message);
    // message = NULL;

    return 0;
}