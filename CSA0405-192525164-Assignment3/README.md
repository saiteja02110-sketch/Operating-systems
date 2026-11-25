# CSA0405 — Operating Systems — Assignment 3 (CO3)

**GitHub Repository:** (paste your GitHub URL here after uploading)

| Field | Details |
|---|---|
| Name | M Sai Teja |
| Register Number | 192525164 |
| Course Code | CSA0405 |
| Course | Operating Systems |
| Assignment | Assignment 3 — Memory Management, Linking, Paging, Segmentation, Virtual Memory and Storage |

## Environment
- OS: Linux
- Compiler: gcc (see output files / `gcc --version`)
- Python: python3 (for Task04 EAT calculation, Task05 and Task06)
- Build: `gcc -std=c11 -Wall -Wextra` unless noted below

## Repository structure
- `Task01/` — Linking, loading and address binding: `hello.c`, `crypto_demo.c` (shared library), `runtime_load.c` (dlopen/dlsym)
- `Task02/` — Paging: `paging_translation.c`
- `Task03/` — Segmentation: `segment_translation.c`
- `Task04/` — Virtual memory / demand paging: `memory_touch.c`, `page_fault_eat.py`
- `Task05/` — Page replacement: `page_replacement.py` (FIFO, LRU, Optimal, Belady's anomaly)
- `Task06/` — Disk management: `raid_calculator.py`
- `Task07/` — Memory-management design / locking: `lock_loop.c` (mlock)
- Each task folder also contains `output.txt` — the actual terminal output captured when the programs were run.

## How to compile and run everything
```bash
chmod +x run_all.sh
./run_all.sh
```

## How to run each task individually
```bash
# Task01
cd Task01
gcc -Wall -Wextra hello.c -o hello_dynamic && ./hello_dynamic
gcc -fPIC -shared crypto_demo.c -o libcrypto_demo.so
gcc -Wall -Wextra runtime_load.c -o runtime_load -ldl && ./runtime_load

# Task02
cd ../Task02 && gcc -std=c11 -Wall -Wextra paging_translation.c -o paging_translation && ./paging_translation

# Task03
cd ../Task03 && gcc -std=c11 -Wall -Wextra segment_translation.c -o segment_translation && ./segment_translation

# Task04
cd ../Task04 && gcc -std=c11 -Wall -Wextra memory_touch.c -o memory_touch && ./memory_touch
python3 page_fault_eat.py

# Task05
cd ../Task05 && python3 page_replacement.py

# Task06
cd ../Task06 && python3 raid_calculator.py

# Task07
cd ../Task07 && gcc -O2 -Wall lock_loop.c -o lock_loop && ./lock_loop
```

## Results summary
- Task01: dynamic executable runs; runtime loader loads `libcrypto_demo.so`, resolves `encrypt_demo` and prints `Result: 93` (7 XOR 0x5A).
- Task02: logical addresses 0, 1500, 3100, 6200, 9000, 2049 translate to physical 5120, 2524, 7196, 3128, invalid address, and page fault respectively.
- Task03: offsets below each segment limit translate as base+offset; an offset equal to the limit is rejected.
- Task04: sequential page-touch completes; demand-paging EAT is 8100 ns at fault rate 1/1000, 180 ns at 1/100000, 108 ns at 1/1000000; at most ~1 fault per 800,000 accesses keeps slowdown within 10%.
- Task05: reference string faults — 3 frames: FIFO 15, LRU 12, Optimal 9; 4 frames: FIFO 10, LRU 8, Optimal 8. Belady string FIFO: 9 faults with 3 frames, 10 with 4 frames.
- Task06: 6 x 4 TB disks — RAID 0: 24 TB, RAID 1: 12 TB, RAID 5: 20 TB, RAID 6: 16 TB, RAID 10: 12 TB usable.
- Task07: 1 MB buffer is touched, locked with mlock, and the locked loop prints its checksum.

Note: run the programs on your own Linux system as well and replace/keep the `output.txt` files with your own terminal outputs before final submission, as the assignment requires individual execution.
