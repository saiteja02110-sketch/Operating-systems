/* Task03 - Race condition demo: shared counter with and without a mutex
 * Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
 * Compile: gcc -std=c11 -Wall -Wextra -pthread race_demo.c -o race_demo
 *
 * Two threads each increment a shared counter INCREMENTS times.
 * Without protection, increments are lost (read-modify-write races),
 * so the result is usually LESS than 2*INCREMENTS. With a mutex,
 * every increment is serialised and the result is exact.
 */
#include <stdio.h>
#include <pthread.h>

#define INCREMENTS 500000

static long counter;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *inc_unlocked(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS; i++) counter++;          /* race here */
    return NULL;
}

static void *inc_locked(void *arg)
{
    (void)arg;
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t a, b;
    counter = 0;
    pthread_create(&a, NULL, inc_unlocked, NULL);
    pthread_create(&b, NULL, inc_unlocked, NULL);
    pthread_join(a, NULL); pthread_join(b, NULL);
    printf("without mutex: counter = %ld (expected %d) - increments lost to races\n",
           counter, 2 * INCREMENTS);

    counter = 0;
    pthread_create(&a, NULL, inc_locked, NULL);
    pthread_create(&b, NULL, inc_locked, NULL);
    pthread_join(a, NULL); pthread_join(b, NULL);
    printf("with mutex:    counter = %ld (expected %d) - exact\n",
           counter, 2 * INCREMENTS);
    return 0;
}
