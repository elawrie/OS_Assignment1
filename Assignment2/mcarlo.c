/* generates a double precision random number */
double random_double() {
    return random() / ((double)RAND_MAX + 1);
}

/* check for points inside circle */
for (i = 0; i < npoints; i++) {
    /* generate random numbers between -1.0 and +1.0 (exclusive) */
    x = random_double() * 2.0 - 1.0;
    y = random_double() * 2.0 - 1.0;
    if (sqrt(x*x + y*y) < 1.0 )
        ++hit_count;
}
