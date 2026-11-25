#!/usr/bin/env bash
# disk-alert.sh - warn about filesystems at or above the threshold
set -euo pipefail
threshold=80
while read -r use mount; do
  pct=${use%%%}
  if [[ "$pct" =~ ^[0-9]+$ ]] && (( pct >= threshold )); then
    echo "WARNING: $mount is ${pct}% full"
  fi
done < <(df -P -h | awk 'NR>1 {print $5, $6}')
