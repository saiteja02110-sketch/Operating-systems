/* Task05 - Bounded buffer, 4 producers + 4 consumers
 * Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
 * Compile: gcc -std=c11 -Wall -Wextra -pthread bounded_buffer_multi.c -o bounded_buffer_multi
 *
 * Each of the 4 producers produces a disjoint range of 25 items
 * (producer p makes items p*25+1 .. p*25+25), so the 100 items 1..100
 * are each produced exactly once. The 4 consumers consume 25 items
 * each (100 total). Correct run: consumed = 100, sum = 5050.
 * The mutex protects in/out AND the consumed count/sum; the empty
 * and full semaphores bound the buffer exactly as in the 1+1 case.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZE 5
#define PRODUCERS 4
#define CONSUMERS 4
#define PER 25
#define TOTAL (PRODUCERS * PER)          /* 100 items */
int buffer[SIZE], in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
long consumed_sum = 0;
int consumed_count = 0;

void *producer(void *arg)
{
    long p = (long)arg;
    for (int k = 1; k <= PER; k++) {
        int item = (int)(p * PER) + k;
        sem_wait(&empty);
        pthread_mutex_lock(&lock);
        buffer[in] = item;
        in = (in + 1) % SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&full);
    }
    return NULL;
}
void *consumer(void *arg)
{
    (void)arg;
    for (int k = 0; k < PER; k++) {
        sem_wait(&full);
        pthread_mutex_lock(&lock);
        int item = buffer[out];
        out = (out + 1) % SIZE;
        consumed_sum += item;
        consumed_count++;
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
    }
    return NULL;
}
int main(void)
{
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_t p[PRODUCERS], c[CONSUMERS];
    for (long i = 0; i < PRODUCERS; i++) pthread_create(&p[i], NULL, producer, (void *)i);
    for (int i = 0; i < CONSUMERS; i++) pthread_create(&c[i], NULL, consumer, NULL);
    for (int i = 0; i < PRODUCERS; i++) pthread_join(p[i], NULL);
    for (int i = 0; i < CONSUMERS; i++) pthread_join(c[i], NULL);
    long expected = (long)TOTAL * (TOTAL + 1) / 2;
    printf("multi (4 producers, 4 consumers): items consumed: %d, sum = %ld (expected %ld) - %s\n",
           consumed_count, consumed_sum, expected,
           (consumed_count == TOTAL && consumed_sum == expected) ? "CORRECT, every item consumed exactly once" : "ERROR");
    sem_destroy(&empty);
    sem_destroy(&full);
    return (consumed_count == TOTAL && consumed_sum == expected) ? 0 : 1;
}
