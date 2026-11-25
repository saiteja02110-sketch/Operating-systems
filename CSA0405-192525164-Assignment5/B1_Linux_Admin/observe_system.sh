#!/usr/bin/env bash
# observe_system.sh - observe processes, memory, disk and network sockets.
# Commands are read-only; failures (e.g. missing tools) do not stop the script.
echo "== Processes and CPU (ps aux --sort=-%cpu | head) =="
ps aux --sort=-%cpu | head || true
echo
echo "== Memory (free -h) =="
free -h || true
echo
echo "== Disk and filesystems (df -h) =="
df -h || true
echo
echo "== Network sockets (ss -tulpen) =="
ss -tulpen || ss -tulpn || true
