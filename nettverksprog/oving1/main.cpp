#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

struct Range {
    int start;
    int end;
};

pthread_mutex_t lock;
int primes[100]; // Adjust the size as needed
int primeIndex = 0;

bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }

    for (int j = 2; j <= num / 2; ++j) {
        if (num % j == 0) {
            return false; // Not prime
        }
    }
    return true; // Prime
}

void* findPrimes(void* args) {
    struct Range* range = (struct Range*) args;
    for (int i = range->start; i <= range->end; ++i) {
        if (isPrime(i)) {
            pthread_mutex_lock(&lock);
            printf("%d ", i);
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int compare(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

int main() {
    //Choose the range for the prime numbers
    int L = 1;
    int R = 100;
    //Choose the number of threads
    int numThreads = 4;

    pthread_t threads[numThreads];
    struct Range ranges[numThreads];
    int threadRange = (R - L + 1) / numThreads;

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < numThreads; ++i) {
        ranges[i].start = L + i * threadRange;
        ranges[i].end = (i == numThreads - 1) ? R : ranges[i].start + threadRange - 1;
        pthread_create(&threads[i], NULL, findPrimes, &ranges[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    qsort(primes, primeIndex, sizeof(int), compare);
    for (int i = 0; i < primeIndex; ++i) {
        printf("%d ", primes[i]);
    }

    pthread_mutex_destroy(&lock);
    return 0;
}




