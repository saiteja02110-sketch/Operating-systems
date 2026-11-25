/* Task04 - IPC with a pipe (Complete the Code, TODO 1 completed)
 * Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
 * Compile: gcc -std=c11 -Wall -Wextra -pthread pipe_ipc.c -o pipe_ipc
 * The parent (sensor reader) sends readings to the child (analyser)
 * through a pipe. Closing the write end makes the child's read()
 * return 0, which is how the pipe signals end of data.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); return 1; }
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[1]); /* child only reads */
        int r, count = 0, sum = 0, max = 0;
        /* TODO 1 completed: read one int at a time until read returns 0 */
        while (read(fd[0], &r, sizeof r) > 0) {
            count++;
            sum += r;
            if (r > max) max = r;
        }
        printf("analyser: %d readings, average %.1f, max %d\n", count,
               count ? (double)sum / count : 0.0, max);
        close(fd[0]);
        fflush(stdout);
        _exit(0);
    }
    close(fd[0]); /* parent only writes */
    int readings[] = {31, 33, 36, 34, 41, 38};
    for (int i = 0; i < 6; i++)
        if (write(fd[1], &readings[i], sizeof readings[i]) < 0) perror("write");
    close(fd[1]); /* signals end of data to the child */
    waitpid(pid, NULL, 0);
    printf("reader: done\n");
    return 0;
}
