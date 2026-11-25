#!/usr/bin/env bash
# CSA0405-192525164-Assignment2 - compile and run every task.
# Student: M Sai Teja, Reg No: 192525164, Course: CSA0405 Operating Systems
# Each task's real output is saved to that task folder's output.txt
set -e
cd "$(dirname "$0")"
CFLAGS="-std=c11 -Wall -Wextra -pthread"

echo "=== Task01 - Processes (fork) ==="
gcc $CFLAGS Task01/fork_demo.c -o Task01/fork_demo
./Task01/fork_demo 2>&1 | tee Task01/output.txt

echo "=== Task02 - CPU Scheduling (Round Robin + calculations) ==="
gcc $CFLAGS Task02/rr_scheduling.c -o Task02/rr_scheduling
{ ./Task02/rr_scheduling; echo; python3 Task02/scheduling_calc.py; } 2>&1 | tee Task02/output.txt

echo "=== Task03 - Threads (pool speed-up + race demo) ==="
gcc $CFLAGS Task03/threads_pool.c -o Task03/threads_pool
gcc $CFLAGS Task03/race_demo.c -o Task03/race_demo
{ ./Task03/threads_pool; echo; ./Task03/race_demo; } 2>&1 | tee Task03/output.txt

echo "=== Task04 - IPC (pipe, shared memory, transfer benchmark) ==="
gcc $CFLAGS Task04/pipe_ipc.c -o Task04/pipe_ipc
gcc $CFLAGS Task04/shared_memory_ipc.c -o Task04/shared_memory_ipc
{ echo "--- pipe ---"; ./Task04/pipe_ipc; echo "--- shared memory ---"; ./Task04/shared_memory_ipc; echo "--- transfer benchmark ---"; python3 Task04/transfer_bench.py; } 2>&1 | tee Task04/output.txt

echo "=== Task05 - Synchronisation (bounded buffer) ==="
gcc $CFLAGS Task05/bounded_buffer.c -o Task05/bounded_buffer
gcc $CFLAGS Task05/bounded_buffer_multi.c -o Task05/bounded_buffer_multi
{ ./Task05/bounded_buffer; bash Task05/run_100.sh; ./Task05/bounded_buffer_multi; } 2>&1 | tee Task05/output.txt

echo "=== Task06 - Banker's Algorithm (+ timing) ==="
gcc $CFLAGS Task06/bankers.c -o Task06/bankers
{ ./Task06/bankers; echo; python3 Task06/bankers_bench.py; } 2>&1 | tee Task06/output.txt

echo "All tasks completed."
