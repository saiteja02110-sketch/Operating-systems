#!/bin/bash
# CSA0405-192525164-Assignment4 - compile and run all tasks
set -e
cd "$(dirname "$0")"
echo "=== Task01: File-System Design Review ==="
(cd Task01 && python3 print_design.py)
echo "=== Task02: File Allocation and Free-Space Management ==="
(cd Task02 && gcc -std=c11 -Wall -Wextra task2.c -o task2 && ./task2 && python3 allocation_calc.py)
echo "=== Task03: Modern File Systems ==="
(cd Task03 && python3 fs_compare.py && bash inspect_fs.sh)
echo "=== Task04: Disk Scheduling Algorithms ==="
(cd Task04 && gcc -std=c11 -Wall -Wextra task4.c -o task4 && ./task4)
echo "=== Task05: Distributed and Cloud-Based Storage ==="
(cd Task05 && python3 storage_compare.py)
echo "=== Task06: Video-Streaming Storage Design ==="
(cd Task06 && python3 capacity_calculator.py)
echo "All tasks completed."
