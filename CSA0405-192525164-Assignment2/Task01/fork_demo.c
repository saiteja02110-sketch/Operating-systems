/* Task01 - Processes and Process States
 * Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
 *
 * Creates one child process per simulated student session with fork().
 * Each child prints its PID, its parent's PID and the state it passes
 * through (new -> ready -> running -> terminated). The parent waits
 * for every child with waitpid() so no zombies are left behind.
 *
 * Observe live with:
 *   ps aux | grep fork_demo          (process list)
 *   ls /proc/<pid>/                   (per-process kernel information)
 *   cat /proc/<pid>/status            (state, PPid, memory of a process)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SESSIONS 4

int main(void)
{
    printf("exam server (parent) PID = %d\n", getpid());
    fflush(stdout); /* flush before fork so children do not reprint buffered lines */
    for (int i = 0; i < SESSIONS; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); return 1; }
        if (pid == 0) {
            /* child: one student session */
            printf("session %d: child PID = %d, PPID = %d, state = running\n",
                   i + 1, getpid(), getppid());
            fflush(stdout);
            /* simulate a little session work */
            long s = 0;
            for (long k = 0; k < 500000; k++) s += k;
            printf("session %d: child PID = %d finished work (sum=%ld), state = terminated\n",
                   i + 1, getpid(), s);
            fflush(stdout);
            _exit(0);
        }
        printf("parent: created session %d as child PID = %d (state = new -> ready)\n",
               i + 1, pid);
        fflush(stdout);
    }
    int reaped = 0;
    while (wait(NULL) > 0) reaped++;
    printf("parent: all %d child processes reaped, no zombies left\n", reaped);
    return 0;
}
