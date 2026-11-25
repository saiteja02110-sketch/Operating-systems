#include <stdio.h>
#define PAGE_SIZE 1024
#define PAGES 8
int page_table[PAGES] = {5,2,-1,7,0,-1,3,-1};
int main(void) {
    unsigned logical[] = {0,1500,3100,6200,9000,2049};
    for (int i=0;i<6;i++) {
        unsigned a=logical[i], p=a/PAGE_SIZE, d=a%PAGE_SIZE;
        printf("%4u -> page %u offset %4u : ",a,p,d);
        if (p>=PAGES) puts("invalid address");
        else if (page_table[p]==-1) puts("page fault");
        else printf("physical %u\n", page_table[p]*PAGE_SIZE+d);
    }
    return 0;
}
