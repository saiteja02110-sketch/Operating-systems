#!/bin/bash
# Task 3 - inspect a real Linux file system (read-only commands only)
echo "--- df -h (mounted file systems and free space) ---"
df -h
echo "--- df -i (inode usage) ---"
df -i
echo "--- stat and ls -i on this script ---"
stat "$0" 2>/dev/null || stat inspect_fs.sh
ls -i "$0" 2>/dev/null || ls -i inspect_fs.sh
echo "--- current directory file system ---"
df -h .
