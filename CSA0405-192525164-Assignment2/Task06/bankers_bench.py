#!/usr/bin/env python3
"""Task06 - Measure: time the Banker's safety algorithm for n = 10, 100, 1000.
Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
Prediction (from the question): safety check is O(m * n^2) - with m fixed,
time should grow roughly with n^2 (100x processes -> ~10,000x time in
theory, before constant/cache effects). Synthetic safe states: each
process needs at most the available work as the algorithm proceeds.
Times are real measured times (best of several runs), printed in us/ms.
"""
import random
import time

M = 3


def make_state(n):
    random.seed(42)
    avail = [50, 50, 50]
    alloc = [[random.randint(0, 3) for _ in range(M)] for _ in range(n)]
    maxm = [[alloc[i][r] + random.randint(0, 3) for r in range(M)] for i in range(n)]
    return avail, alloc, maxm


def is_safe(avail, alloc, maxm, n):
    work = avail[:]
    finish = [False] * n
    count = 0
    while count < n:
        found = False
        for p in range(n):
            if finish[p]:
                continue
            if all(maxm[p][r] - alloc[p][r] <= work[r] for r in range(M)):
                for r in range(M):
                    work[r] += alloc[p][r]
                finish[p] = True
                count += 1
                found = True
        if not found:
            return False
    return True


if __name__ == "__main__":
    print("Banker's safety algorithm timing (m = 3 resource types):")
    for n, reps in ((10, 200), (100, 20), (1000, 3)):
        avail, alloc, maxm = make_state(n)
        assert is_safe(avail, alloc, maxm, n), "synthetic state must be safe"
        best = None
        for _ in range(reps):
            a = time.perf_counter()
            is_safe(avail, alloc, maxm, n)
            dt = time.perf_counter() - a
            best = dt if best is None else min(best, dt)
        print(f"n = {n:>5} processes: {best*1000:10.3f} ms per safety check "
              f"({best*1e6:10.1f} us), best of {reps} runs")
    print("Growth check: O(m*n^2) is the WORST case (many sweeps needed).")
    print("In this synthetic safe state almost every process can finish in the")
    print("first sweep, so the measured growth is much gentler (closer to O(m*n)).")
    print("A state where only one process can finish per sweep would approach O(m*n^2).")
