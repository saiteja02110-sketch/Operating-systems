#!/usr/bin/env python3
"""RAID usable capacity for 6 x 4 TB disks."""
n, size = 6, 4
raw = n*size
print(f"Disks: {n} x {size} TB = {raw} TB raw")
print(f"RAID 0 : {raw} TB usable, fault tolerance: none")
print(f"RAID 1 : {raw//2} TB usable (mirrored pairs), tolerates one disk per mirror pair")
print(f"RAID 5 : {(n-1)*size} TB usable, tolerates 1 disk failure")
print(f"RAID 6 : {(n-2)*size} TB usable, tolerates 2 disk failures")
print(f"RAID 10: {raw//2} TB usable (striped mirrors), fast and resilient")
