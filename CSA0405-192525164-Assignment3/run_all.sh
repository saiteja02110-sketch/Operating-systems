#!/bin/bash
# CSA0405-192525164-Assignment3 - compile and run all tasks
set -e
cd "$(dirname "$0")"
echo "=== Task01: Linking / Loading ==="
(cd Task01 && gcc -Wall -Wextra hello.c -o hello_dynamic && ./hello_dynamic && ls -lh hello_dynamic && ldd hello_dynamic || true)
(cd Task01 && gcc -fPIC -shared crypto_demo.c -o libcrypto_demo.so && gcc -Wall -Wextra runtime_load.c -o runtime_load -ldl && ./runtime_load)
echo "=== Task02: Paging ==="
(cd Task02 && gcc -std=c11 -Wall -Wextra paging_translation.c -o paging_translation && ./paging_translation)
echo "=== Task03: Segmentation ==="
(cd Task03 && gcc -std=c11 -Wall -Wextra segment_translation.c -o segment_translation && ./segment_translation)
echo "=== Task04: Virtual Memory / Demand Paging ==="
(cd Task04 && gcc -std=c11 -Wall -Wextra memory_touch.c -o memory_touch && ./memory_touch && python3 page_fault_eat.py)
echo "=== Task05: Page Replacement ==="
(cd Task05 && python3 page_replacement.py)
echo "=== Task06: Disk Management / RAID ==="
(cd Task06 && python3 raid_calculator.py)
echo "=== Task07: Memory Locking ==="
(cd Task07 && gcc -O2 -Wall lock_loop.c -o lock_loop && ./lock_loop)
echo "All tasks completed."
