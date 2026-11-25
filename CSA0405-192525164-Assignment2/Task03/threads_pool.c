/* Task03 - Threads: thread-pool speed-up for 64 camera frames
 * Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
 * Compile: gcc -std=c11 -Wall -Wextra -pthread threads_pool.c -o threads_pool
 *
 * 64 frames are processed by a pool of W worker threads (W = 1, 4, 8, 16).
 * Workers take frame numbers from a shared counter protected by a mutex.
 * Each frame does the same fixed CPU work, so elapsed time should fall
 * as workers increase, until the CPU cores are saturated (Amdahl /
 * core limit), after which extra threads add overhead without speed-up.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define FRAMES 64
#define WORK 3000000L   /* fixed sum-loop iterations per frame */

static int next_frame;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static volatile long total_checksum;

static void process_frame(int f)
{
    long s = 0;
    for (long k = 0; k < WORK; k++) s += (k + f) % 7;
    __sync_fetch_and_add(&total_checksum, s & 1); /* keep work from being optimised away */
}

static void *worker(void *arg)
{
    (void)arg;
    for (;;) {
        pthread_mutex_lock(&lock);
        int f = next_frame++;
        pthread_mutex_unlock(&lock);
        if (f >= FRAMES) break;
        process_frame(f);
    }
    return NULL;
}

static double run_pool(int workers)
{
    pthread_t th[32];
    next_frame = 0;
    struct timespec a, b;
    clock_gettime(CLOCK_MONOTONIC, &a);
    for (int i = 0; i < workers; i++) pthread_create(&th[i], NULL, worker, NULL);
    for (int i = 0; i < workers; i++) pthread_join(th[i], NULL);
    clock_gettime(CLOCK_MONOTONIC, &b);
    return (b.tv_sec - a.tv_sec) * 1000.0 + (b.tv_nsec - a.tv_nsec) / 1e6;
}

int main(void)
{
    int pools[] = {1, 4, 8, 16};
    double base = 0;
    printf("processing %d camera frames, fixed work per frame\n", FRAMES);
    for (int i = 0; i < 4; i++) {
        double ms = run_pool(pools[i]);
        if (i == 0) base = ms;
        printf("pool = %2d threads: %8.2f ms  (speed-up vs 1 thread: %.2fx)\n",
               pools[i], ms, base / ms);
    }
    return 0;
}
