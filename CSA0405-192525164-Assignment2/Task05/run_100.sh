#!/usr/bin/env bash
# Task05 - Run the 1-producer/1-consumer bounded buffer 100 times and
# verify the consumed sum is 210 every single time.
set -u
cd "$(dirname "$0")"
gcc -std=c11 -Wall -Wextra -pthread bounded_buffer.c -o bounded_buffer
pass=0
for i in $(seq 1 100); do
    out=$(./bounded_buffer)
    if [ "$out" = "items consumed: 20, sum = 210 (expected 210)" ]; then
        pass=$((pass + 1))
    else
        echo "run $i FAILED: $out"
    fi
done
echo "$pass/100 runs sum=210"
