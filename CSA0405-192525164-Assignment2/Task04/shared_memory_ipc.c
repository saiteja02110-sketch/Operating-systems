/* Task04 - IPC with POSIX shared memory + a process-shared mutex flag
 * Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
 * Compile: gcc -std=c11 -Wall -Wextra -pthread shared_memory_ipc.c -o shared_memory_ipc
 *
 * Same weather-station exchange as pipe_ipc.c, but the readings are
 * placed in shared memory (no copy through the kernel pipe buffer).
 * A process-shared pthread mutex protects the ready flag: the parent
 * writes the data, sets ready = 1 under the mutex; the child polls
 * the flag under the same mutex, then reads the data directly.
 * Shared memory is faster but NEEDS this extra synchronisation -
 * with a pipe, the kernel's blocking read/write provides it for free.
 */
#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>

#define NREAD 6

typedef struct {
    pthread_mutex_t lock;   /* process-shared */
    int ready;              /* flag: data written */
    int count;
    int data[NREAD];
} shared_t;

int main(void)
{
    const char *name = "/weather_shm_sai_teja";
    int fd = shm_open(name, O_CREAT | O_RDWR, 0600);
    if (fd < 0) { perror("shm_open"); return 1; }
    if (ftruncate(fd, sizeof(shared_t)) != 0) { perror("ftruncate"); return 1; }
    shared_t *sh = mmap(NULL, sizeof(shared_t), PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);
    if (sh == MAP_FAILED) { perror("mmap"); return 1; }
    memset(sh, 0, sizeof *sh);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&sh->lock, &attr);

    pid_t pid = fork();
    if (pid == 0) {
        /* child (analyser): wait for the ready flag, then read directly */
        for (;;) {
            pthread_mutex_lock(&sh->lock);
            int r = sh->ready;
            pthread_mutex_unlock(&sh->lock);
            if (r) break;
            usleep(1000);
        }
        int count = sh->count, sum = 0, max = 0;
        for (int i = 0; i < count; i++) {
            sum += sh->data[i];
            if (sh->data[i] > max) max = sh->data[i];
        }
        printf("analyser: %d readings, average %.1f, max %d\n", count,
               count ? (double)sum / count : 0.0, max);
        fflush(stdout);
        _exit(0);
    }
    /* parent (sensor reader): write readings into shared memory */
    int readings[NREAD] = {31, 33, 36, 34, 41, 38};
    pthread_mutex_lock(&sh->lock);
    memcpy(sh->data, readings, sizeof readings);
    sh->count = NREAD;
    sh->ready = 1;
    pthread_mutex_unlock(&sh->lock);
    waitpid(pid, NULL, 0);
    printf("reader: done\n");
    pthread_mutex_destroy(&sh->lock);
    munmap(sh, sizeof *sh);
    close(fd);
    shm_unlink(name);
    return 0;
}
