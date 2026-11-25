#!/usr/bin/env python3
"""Task04 - Growth investigation: time to transfer data through a pipe.
Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
A child process drains the pipe and counts bytes; the parent writes
the full payload and closes the pipe (EOF). Times are real measured
wall-clock times on the machine running this script.
"""
import os
import sys
import time

CHUNK = 65536


def transfer(size):
    r, w = os.pipe()
    pid = os.fork()
    if pid == 0:
        os.close(w)
        total = 0
        while True:
            data = os.read(r, CHUNK)
            if not data:
                break
            total += len(data)
        os.close(r)
        os._exit(0 if total == size else 1)
    os.close(r)
    block = b"x" * CHUNK
    start = time.perf_counter()
    sent = 0
    while sent < size:
        n = min(CHUNK, size - sent)
        os.write(w, block[:n])
        sent += n
    os.close(w)
    _, status = os.waitpid(pid, 0)
    elapsed = time.perf_counter() - start
    ok = os.WIFEXITED(status) and os.WEXITSTATUS(status) == 0
    return elapsed, ok


if __name__ == "__main__":
    for mb in (1, 10, 100):
        size = mb * 1024 * 1024
        elapsed, ok = transfer(size)
        print(f"pipe transfer {mb:>3} MB: {elapsed*1000:8.2f} ms "
              f"({mb/elapsed:7.1f} MB/s), bytes verified: {ok}")
        sys.stdout.flush()
