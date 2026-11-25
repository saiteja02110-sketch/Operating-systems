#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE (1024*1024)
int main(void){
    char *buf = calloc(SIZE,1);
    if(!buf) return 1;
    for(size_t i=0;i<SIZE;i+=4096) buf[i]=1; /* touch pages before timing */
    if(mlock(buf,SIZE)!=0) perror("mlock"); else printf("mlock: %d bytes locked in memory\n", SIZE);
    volatile unsigned long s=0;
    for(int r=0;r<100000;r++) s += buf[(r*4096)%SIZE];
    printf("checksum=%lu\n", s);
    munlock(buf,SIZE);
    free(buf);
    return 0;
}
