# CSA0405 – Operating Systems – Assignment 2 (CO2)

**Name:** M Sai Teja
**Register Number:** 192525164
**Course:** CSA0405 Operating Systems
**Assignment:** Assignment 2 – CO2: Process Management and Deadlock Handling (50 marks)

**GitHub Repository:** (paste your GitHub URL here after uploading)

## Environment (where these outputs were captured)

- Compiler: gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
- Python: 3.12.3
- OS: Ubuntu 24.04.5 LTS, Linux x86_64
- Compile standard used for all C programs: `gcc -std=c11 -Wall -Wextra -pthread`

> Note: run the programs on your own Linux system before final submission
> and commit each task separately (at least one commit per task), as the
> assignment requires individual execution and a progressive commit history.
> Timings (Task03, Task04, Task06) are machine-dependent and will differ.

## How to run everything

```bash
bash run_all.sh
```

This compiles and runs all six tasks and refreshes each task's `output.txt`.

## Task-by-task

### Task01 – Processes and Process States (`Task01/fork_demo.c`)
```bash
gcc -std=c11 -Wall -Wextra -pthread Task01/fork_demo.c -o Task01/fork_demo
./Task01/fork_demo
# observe processes with: ps aux | grep fork_demo ; ls /proc/<pid>/ ; cat /proc/<pid>/status
```
Result: the parent (exam server) creates 4 child session processes with
`fork()`; each child prints its PID/PPID and running state, finishes, and
the parent reaps all 4 – no zombies.

### Task02 – CPU Scheduling (`Task02/rr_scheduling.c`, `scheduling_calc.py`)
```bash
gcc -std=c11 -Wall -Wextra -pthread Task02/rr_scheduling.c -o Task02/rr_scheduling
./Task02/rr_scheduling
python3 Task02/scheduling_calc.py
```
Result (jobs P1–P4, arrival 0/1/2/3, burst 8/4/9/5): Round Robin q=3 gives
Gantt `P1(0-3) P2(3-6) P3(6-9) P4(9-12) P1(12-15) P2(15-16) P3(16-19)
P4(19-21) P1(21-23) P3(23-26)`, average TAT = 20.00, average WT = 13.50.
Calculations: FCFS avg TAT 15.25 / WT 8.75; SJF 14.25 / 7.75; SRTF 13.00 /
6.50; RR q=1: 19.00 / 12.50 (26 slices), q=5: 17.50 / 11.00 (6 slices),
q=20: 15.25 / 8.75 (4 slices, same as FCFS).

### Task03 – Threads (`Task03/threads_pool.c`, `Task03/race_demo.c`)
```bash
gcc -std=c11 -Wall -Wextra -pthread Task03/threads_pool.c -o Task03/threads_pool
gcc -std=c11 -Wall -Wextra -pthread Task03/race_demo.c -o Task03/race_demo
./Task03/threads_pool && ./Task03/race_demo
```
Result: 64 frames processed with pools of 1/4/8/16 threads, timed with
`clock_gettime` (real times in `output.txt`; speed-up plateaus once the
available cores are saturated). Race demo: without a mutex the shared
counter ends below 1,000,000 (lost updates); with a mutex it is exactly
1,000,000.

### Task04 – Inter-Process Communication (`Task04/pipe_ipc.c`, `shared_memory_ipc.c`, `transfer_bench.py`)
```bash
gcc -std=c11 -Wall -Wextra -pthread Task04/pipe_ipc.c -o Task04/pipe_ipc
gcc -std=c11 -Wall -Wextra -pthread Task04/shared_memory_ipc.c -o Task04/shared_memory_ipc
./Task04/pipe_ipc && ./Task04/shared_memory_ipc
python3 Task04/transfer_bench.py
```
Result: both pipe and shared-memory versions print
`analyser: 6 readings, average 35.5, max 41` / `reader: done`.
The pipe signals end-of-data when the parent closes the write end
(child's `read()` returns 0); shared memory needs the extra
process-shared mutex + ready flag for synchronisation.
Transfer benchmark records real measured pipe times for 1 MB, 10 MB and
100 MB in `output.txt`.

### Task05 – Synchronisation (`Task05/bounded_buffer.c`, `bounded_buffer_multi.c`, `run_100.sh`)
```bash
gcc -std=c11 -Wall -Wextra -pthread Task05/bounded_buffer.c -o Task05/bounded_buffer
gcc -std=c11 -Wall -Wextra -pthread Task05/bounded_buffer_multi.c -o Task05/bounded_buffer_multi
./Task05/bounded_buffer
bash Task05/run_100.sh
./Task05/bounded_buffer_multi
```
Result: 1 producer / 1 consumer gives `items consumed: 20, sum = 210
(expected 210)`; the 100-run check prints `100/100 runs sum=210`;
4 producers / 4 consumers consume all 100 items exactly once
(count 100, sum 5050).

### Task06 – Deadlock and Banker's Algorithm (`Task06/bankers.c`, `bankers_bench.py`)
```bash
gcc -std=c11 -Wall -Wextra -pthread Task06/bankers.c -o Task06/bankers
./Task06/bankers
python3 Task06/bankers_bench.py
```
Result:
```
initial state safe: P1 P3 P4 P0 P2
P1 request: granted, safe sequence P1 P3 P4 P0 P2
P4 request: must wait (not available)
P0 request: denied (unsafe)
```
The benchmark times the safety algorithm for n = 10, 100 and 1000
processes (real measured times in `output.txt`). O(m·n²) is the
worst case; in the synthetic safe state used here most processes finish
in the first sweep, so measured growth is gentler – a state where only
one process can finish per sweep would approach O(m·n²).
