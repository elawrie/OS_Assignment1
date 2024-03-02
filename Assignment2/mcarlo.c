#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int hit_count = 0;

/* generates a double precision random number */
double random_double() {
    return random() / ((double)RAND_MAX + 1);
}

void *generate_points(void *arg) {
    int npoints = *((int *)arg);
    /* check for points inside circle */
    for (int i = 0; i < npoints; i++) {
        /* generate random numbers between -1.0 and +1.0 (exclusive) */
        double x = random_double() * 2.0 - 1.0;
        double y = random_double() * 2.0 - 1.0;
        // checking if points are inside the circle
        if (sqrt(x*x + y*y) < 1.0 )
            ++hit_count;
    }

    pthread_exit(NULL);
}

int main(int argc, char** argv){
    // error checking, make sure user inputs num points
    if (argc != 2) {
        printf("Usage: %s <number of points>\n", argv[0]);
        return -1;
    }

    int num_points = atoi(argv[1]);

    // check if user inputs positive integer
    if (num_points <= 0) {
        printf("Please provide a POSITIVE INTEGER\n");
        return -1;
    }

    // initialize thread IDs
    pthread_t tid_one, tid_two;

    // split number of points between two threads
    int tid_one_points = num_points / 2;
    int tid_two_points = num_points - tid_one_points;
    
    // one thread to calculate half points
    pthread_create(&tid_one, NULL, generate_points, (void *)&tid_one_points);
    
    // second thread to calculate other half points
    pthread_create(&tid_two, NULL, generate_points, (void *)&tid_two_points);

    // JOIN BOTH THREADS
    pthread_join(tid_one, NULL);
    pthread_join(tid_two, NULL);

    printf("HIT COUNT %d\n", hit_count);

    double pi = 4.0 * hit_count / num_points;
    printf("Estimated value of pi: %f\n", pi);

    return 0;

}