#include <stdio.h>
typedef struct { const char *name; int base; int limit; } Segment;
static Segment segs[] = {{"Code",10000,4000},{"Data",20000,3000},{"Stack",30000,2000}};
static void check(int s, int offset) {
    Segment *g=&segs[s];
    if (offset < g->limit) printf("(%s, %d) -> physical %d\n", g->name, offset, g->base+offset);
    else printf("(%s, %d) -> invalid: offset >= limit %d (protection fault)\n", g->name, offset, g->limit);
}
int main(void){
    check(0,0); check(0,120); check(0,3999); check(0,4000);
    check(1,0); check(1,250); check(1,2999); check(1,3000);
    check(2,0); check(2,1999); check(2,2000);
    return 0;
}
