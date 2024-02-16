#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct Range {
    int start;
    int end;
};

pthread_mutex_t lock;
//This is the array size and does not need to be the same as the
// amount of checked numbers, but as many as the primenumbers
int primes[10000]; // Adjust the size as needed
int primeIndex = 0;

//Checks if a given number is a prime number
//Returns true if it is a prime number, false if it is not
bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }

    for (int j = 2; j * j <= num; ++j) {
        if (num % j == 0) {
            return false; // Not prime
        }
    }
    return true; // Prime
}

//Finds all prime numbers in a given range
//Returns nothing, but adds the prime numbers to the global array
void* findPrimes(void* args) {
    struct Range* range = (struct Range*) args;
    for (int i = range->start; i <= range->end; ++i) {
        if (isPrime(i)) {
            pthread_mutex_lock(&lock);
            if (primeIndex < sizeof(primes) / sizeof(primes[0])) { // Check for overflow
                primes[primeIndex++] = i;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

//Compares two integers
//Returns 0 if they are equal, a negative number if a is less than b,
// and a positive number if a is greater than b
int compare(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//Main function
//Here you set a minimum and a maximum value for the range of numbers to check
//You also set the amount of threads to use
int main() {
    int L = 1;
    int R = 1000000;
    //Dont have more than 4 on mac ;-(
    int numThreads = 4;

    //Create threads and ranges
    pthread_t threads[numThreads];
    struct Range ranges[numThreads];
    int threadRange = (R - L + 1) / numThreads;

    //Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    //Create threads
    for (int i = 0; i < numThreads; ++i) {
        ranges[i].start = L + i * threadRange;
        ranges[i].end = (i == numThreads - 1) ? R : ranges[i].start + threadRange - 1;
        pthread_create(&threads[i], NULL, findPrimes, &ranges[i]);
    }

    //Wait for threads to finish
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    //Sort the array of primes and print them
    qsort(primes, primeIndex, sizeof(int), compare);
    for (int i = 0; i < primeIndex; ++i) {
        printf("%d ", primes[i]);
    }

    //Destroy the mutex
    pthread_mutex_destroy(&lock);
    return 0;
}
