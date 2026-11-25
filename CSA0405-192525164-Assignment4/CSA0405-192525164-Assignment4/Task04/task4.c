#include <stdio.h>
#include <stdlib.h>

#define N 8
#define MAXCYL 199

static int req[N] = {98, 183, 37, 122, 14, 124, 65, 67};

static int cmp_int(const void *a, const void *b) {
    return *(const int *)a - *(const int *)b;
}

/* Print "NAME head -> order... = total" and return the total movement,
   where movement is measured from `head` through every entry in order. */
static int show(const char *name, const int order[], int n, int head) {
    int total = 0, pos = head;
    printf("%-7s %d", name, head);
    for (int i = 0; i < n; i++) {
        printf(" -> %d", order[i]);
        total += abs(order[i] - pos);
        pos = order[i];
    }
    printf("  = %d cylinders\n", total);
    return total;
}

int main(void) {
    int head = 53;
    int order[N + 2], n;

    /* FCFS: arrival order */
    show("FCFS", req, N, head);

    /* SSTF: repeatedly pick the nearest unserviced request */
    {
        int done[N] = {0}, pos = head;
        for (int k = 0; k < N; k++) {
            int best = -1, dist = MAXCYL + 1;
            for (int i = 0; i < N; i++) {
                if (done[i]) continue;
                int d = abs(req[i] - pos);
                if (d < dist) { dist = d; best = i; }
            }
            done[best] = 1;
            order[k] = req[best];
            pos = req[best];
        }
        show("SSTF", order, N, head);
    }

    /* Sorted queue shared by the SCAN family */
    int s[N];
    for (int i = 0; i < N; i++) s[i] = req[i];
    qsort(s, N, sizeof s[0], cmp_int);

    /* SCAN: head starts toward cylinder 0, goes to 0, then reverses up */
    n = 0;
    for (int i = N - 1; i >= 0; i--) if (s[i] < head) order[n++] = s[i];
    order[n++] = 0;
    for (int i = 0; i < N; i++) if (s[i] > head) order[n++] = s[i];
    show("SCAN", order, n, head);

    /* LOOK: like SCAN but reverses at the last request, not at 0 */
    n = 0;
    for (int i = N - 1; i >= 0; i--) if (s[i] < head) order[n++] = s[i];
    for (int i = 0; i < N; i++) if (s[i] > head) order[n++] = s[i];
    show("LOOK", order, n, head);

    /* C-SCAN: service upward only, go to 199, jump to 0, continue up */
    n = 0;
    for (int i = 0; i < N; i++) if (s[i] > head) order[n++] = s[i];
    order[n++] = MAXCYL;
    order[n++] = 0;
    for (int i = 0; i < N; i++) if (s[i] < head) order[n++] = s[i];
    show("C-SCAN", order, n, head);

    /* C-LOOK: like C-SCAN but wraps from the highest to the lowest request */
    n = 0;
    for (int i = 0; i < N; i++) if (s[i] > head) order[n++] = s[i];
    for (int i = 0; i < N; i++) if (s[i] < head) order[n++] = s[i];
    show("C-LOOK", order, n, head);

    return 0;
}
