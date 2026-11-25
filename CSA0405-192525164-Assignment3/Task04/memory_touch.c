#include <stdio.h>
#include <stdlib.h>
#define N (16*1024*1024)
int main(void){
    unsigned char *a = calloc(N,1);
    if(!a) return 1;
    for(size_t i=0;i<N;i+=4096) a[i]=1; /* write-touch each page */
    volatile unsigned long sum=0;
    for(size_t i=0;i<N;i+=4096) sum += a[i];
    printf("Sequential page sum=%lu\n", sum);
    printf("Touched %d pages of %d bytes each\n", N/4096, 4096);
    free(a);
    return 0;
}
