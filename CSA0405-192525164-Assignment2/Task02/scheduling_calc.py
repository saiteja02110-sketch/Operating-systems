#!/usr/bin/env python3
"""Task02 - Scheduling calculations for FCFS, SJF, SRTF and Round Robin.
Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
Jobs: P1..P4, arrival {0,1,2,3}, burst {8,4,9,5}.
Round Robin uses the same queue logic as rr_scheduling.c: newly arrived
processes are added BEFORE the current process is requeued.
"""
NAMES = ["P1", "P2", "P3", "P4"]
ARR = [0, 1, 2, 3]
BURST = [8, 4, 9, 5]
N = 4


def report(label, finish):
    tat = [finish[i] - ARR[i] for i in range(N)]
    wt = [tat[i] - BURST[i] for i in range(N)]
    print(f"{label}: finish={finish} avg TAT = {sum(tat)/N:.2f}, avg WT = {sum(wt)/N:.2f}")
    return sum(tat) / N, sum(wt) / N


def fcfs():
    t, finish = 0, [0] * N
    for i in sorted(range(N), key=lambda x: (ARR[x], x)):
        t = max(t, ARR[i]) + BURST[i]
        finish[i] = t
    return finish


def sjf():
    t, done, finish = 0, set(), [0] * N
    while len(done) < N:
        ready = [i for i in range(N) if i not in done and ARR[i] <= t]
        if not ready:
            t = min(ARR[i] for i in range(N) if i not in done)
            continue
        p = min(ready, key=lambda x: (BURST[x], ARR[x], x))
        t += BURST[p]
        finish[p] = t
        done.add(p)
    return finish


def srtf():
    t, left, finish, cur = 0, BURST[:], [0] * N, -1
    done = 0
    while done < N:
        ready = [i for i in range(N) if left[i] > 0 and ARR[i] <= t]
        if not ready:
            t = min(ARR[i] for i in range(N) if left[i] > 0)
            continue
        p = min(ready, key=lambda x: (left[x], ARR[x], x))
        left[p] -= 1
        t += 1
        cur = p
        if left[p] == 0:
            finish[p] = t
            done += 1
    return finish


def rr(quantum):
    left, finish = BURST[:], [0] * N
    queue, added = [0], [False] * N
    added[0] = True
    t, done, slices = 0, 0, 0
    while done < N:
        p = queue.pop(0)
        run = min(quantum, left[p])
        t += run
        left[p] -= run
        slices += 1
        for i in range(N):
            if not added[i] and ARR[i] <= t:
                queue.append(i)
                added[i] = True
        if left[p] > 0:
            queue.append(p)
        else:
            finish[p] = t
            done += 1
    return finish, slices


if __name__ == "__main__":
    report("FCFS", fcfs())
    report("SJF (non-preemptive)", sjf())
    report("SRTF (preemptive SJF)", srtf())
    for q in (1, 3, 5, 20):
        f, s = rr(q)
        tat = [f[i] - ARR[i] for i in range(N)]
        wt = [tat[i] - BURST[i] for i in range(N)]
        print(f"Round Robin q={q}: avg TAT = {sum(tat)/N:.2f}, avg WT = {sum(wt)/N:.2f}, slices (context runs) = {s}")
