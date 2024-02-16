# Prime Number Finder using Multithreading

This program is designed to find prime numbers within a specified range using multiple threads. It demonstrates the use of POSIX threads (pthreads) in C for parallel processing.

## Code Structure

### Includes and Definitions

\```c
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
\```

- `stdio.h` for standard I/O operations.
- `pthread.h` for threading capabilities.
- `stdbool.h` for boolean data types.
- `stdlib.h` for the `qsort` function.

### Global Variables

\```c
pthread_mutex_t lock;
int primes[100]; // Adjust the size as needed
int primeIndex = 0;
\```

- `pthread_mutex_t lock`: A mutex lock to control access to shared resources.
- `primes[100]`: An array to store prime numbers.
- `primeIndex`: Index to keep track of the next position in the `primes` array.

### Helper Function: `isPrime`

\```c
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
\```

- Checks if a number is prime.

### Thread Function: `findPrimes`

\```c
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
\```

- Each thread executes this function.
- Finds primes in the assigned range and stores them in the global `primes` array.

### Comparison Function for `qsort`

\```c
int compare(const void * a, const void * b) {
return ( *(int*)a - *(int*)b );
}
\```

- Used by `qsort` to sort integers.

### Main Function

\```c
int main() {
int L = 1;
int R = 100;
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
\```

- Initializes variables, creates threads, and starts them.
- Joins threads and prints sorted prime numbers.

## Program Flow

1. **Initialization**
    - Define the range (`L` to `R`) and number of threads.
    - Divide the range equally among the threads.

2. **Thread Creation and Execution**
    - For each thread, define its range and start it.
    - Each thread executes `findPrimes`, which checks for primes in its subrange and stores them.

3. **Finalizing Threads**
    - Wait for all threads to complete (`pthread_join`).
    - Sort the collected prime numbers.

4. **Output**
    - Print the sorted prime numbers.
    - Destroy the mutex lock.

## Synchronization

- Mutex locks are used to prevent simultaneous write access to the `primes` array by multiple threads.

## Sorting and Printing

- After all threads have completed, the main thread sorts the prime numbers using `qsort` and prints them.
