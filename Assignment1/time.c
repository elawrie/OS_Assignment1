#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>

// main
int main(int argc, char** argv){
    
    // error checking -- make sure user enters a valid command
    if (argc != 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return -1;
    }

    // size of shared memory
    const int SIZE = 4096;

    // name of shared memory object
    const char *name = "/os";

    // shared memory file descriptor
    int shm_fd;

    // pointer to shared memory object
    struct timeval* ptr; 
    struct timeval current;

    // open the shared memory object 
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    // error checking for invalid file name / shared memory
    if (shm_fd == -1) {
        printf("shared memory failed\n");
        exit(EXIT_FAILURE);
    }

    // configure size of shared memory object
    ftruncate(shm_fd, SIZE);
    
    // memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // check for error in creating shared memory
    if (ptr == MAP_FAILED) {
        close(shm_fd);
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    pid_t pid;

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            // child process

            // get starting time before fuction call 
            gettimeofday(&current,NULL);
    
            // copy starting time into shared memory object
            memcpy(ptr, &current, sizeof(struct timeval));

            // execute the system call 
            execlp(argv[1], argv[1], NULL);
            
            break;
        default:
            // parent process
            
            // wait for the child to finish
            waitpid(pid, NULL, 0);

            // get time when system call is finished (child is finished)
            struct timeval endtime;
            gettimeofday(&endtime,NULL);

            // remove the shared memory object
            shm_unlink(name);

            // Getting the time difference in seconds
            struct timeval time_passed;
            time_passed.tv_sec = endtime.tv_sec - ptr->tv_sec;

            // Calculating microsecond difference 
            if (endtime.tv_usec < ptr->tv_usec) {
                time_passed.tv_sec--;
                time_passed.tv_usec = 1000000 + endtime.tv_usec - ptr->tv_usec;
            } else {
                time_passed.tv_usec = endtime.tv_usec - ptr->tv_usec;
            }

            // print the time elapsed result
            printf("Elapsed time: %ld.%ld seconds\n", time_passed.tv_sec, (long)time_passed.tv_usec);

            break;
    }

    return 0;
}