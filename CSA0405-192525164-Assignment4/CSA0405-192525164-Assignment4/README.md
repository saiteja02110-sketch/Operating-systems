# CSA0405 — Operating Systems — Assignment 4 (CO4)

**GitHub Repository:** (paste your GitHub URL here after uploading)

| Field | Details |
|---|---|
| Name | M Sai Teja |
| Register Number | 192525164 |
| Course Code | CSA0405 |
| Course | Operating Systems |
| Assignment | Assignment 4 — Optimising File Storage and Disk Access (CO4) |

## Environment
- OS: Linux
- Compiler: gcc (see output files / `gcc --version`)
- Python: python3 (for Task01, Task02 calculations, Task03, Task05 and Task06)
- Build: `gcc -std=c11 -Wall -Wextra` unless noted below

## Repository structure
- `Task01/` — File-system design review: `design.md` (hierarchical directory design, naming rules, permissions), `print_design.py`
- `Task02/` — File allocation and free-space management: `task2.c` (first-fit contiguous + indexed allocation), `allocation_calc.py` (inode / bit-vector calculations)
- `Task03/` — Modern file systems: `fs_compare.py` (FAT32/NTFS/ext4/XFS/Btrfs/ZFS/APFS, journaling modes), `inspect_fs.sh` (df / stat / ls inspection commands)
- `Task04/` — Disk scheduling algorithms: `task4.c` (FCFS, SSTF, SCAN, LOOK, C-SCAN, C-LOOK)
- `Task05/` — Distributed and cloud-based storage: `storage_compare.py` (NFS vs HDFS/Ceph vs cloud object storage, replication and consistency)
- `Task06/` — Video-streaming storage design: `design.md` (architecture and design decisions), `capacity_calculator.py`
- Each task folder also contains `output.txt` — the actual terminal output captured when the programs were run.

## How to compile and run everything
```bash
chmod +x run_all.sh
./run_all.sh
```

## How to run each task individually
```bash
# Task01
cd Task01 && python3 print_design.py

# Task02
cd ../Task02 && gcc -std=c11 -Wall -Wextra task2.c -o task2 && ./task2
python3 allocation_calc.py

# Task03
cd ../Task03 && python3 fs_compare.py
bash inspect_fs.sh

# Task04
cd ../Task04 && gcc -std=c11 -Wall -Wextra task4.c -o task4 && ./task4

# Task05
cd ../Task05 && python3 storage_compare.py

# Task06
cd ../Task06 && python3 capacity_calculator.py
```

## Results summary
- Task01: a flat, permission-free directory is replaced by a hierarchical design — one private folder per register number (0700 directories, 0600 private files), group-shared project folders (2770), course folders, and administrator-managed backup.
- Task02: after allocating A(6), B(8), C(5), D(7) and releasing B and D, the disk is `AAAAAA........CCCCC.............`; a 10-block file E fits contiguously at block 19, and indexed allocation uses index block 6 with data blocks 7 8 9 10 11 12 13 29 30 31. An inode can address at most 1,074,791,436 blocks (≈ 4.00 TiB); a 4 TB disk needs a ≈ 116.4 MiB free-space bit vector (128 MiB for 4 TiB).
- Task03: journaling (especially ext4 `data=ordered` as the balanced default) and copy-on-write file systems (Btrfs/ZFS/APFS) protect consistency; choices: exFAT for a shared pen drive, ext4/XFS for a Linux database server, the native file system on a laptop, Btrfs/ZFS for a snapshot backup server.
- Task04: for head 53 and requests 98, 183, 37, 122, 14, 124, 65, 67 — total head movement: FCFS 640, SSTF 236, SCAN 236, LOOK 208, C-SCAN 382, C-LOOK 322 cylinders.
- Task05: NFS is simple but a single server is a bottleneck / single point of failure; HDFS/Ceph scale and replicate across nodes; cloud object storage is managed and multi-zone at usage-based cost. Three replicas tolerate a node or disk failure while another copy is served and the missing copy is repaired.
- Task06: hybrid design — HDD pool for the full catalog, SSD/RAM cache for hot videos, optional cloud object storage + CDN. Planning: 20,000 videos × 1 GB = 20 TB base, 60 TB with 3 copies, ≈ 72 TB with a 20% reserve; 200,000 students × 5 GB = 1 PB monthly transfer; 10,000 concurrent viewers × 5 Mbps = 50 Gbps peak; a 1 GB file has 262,144 blocks of 4 KiB, and seeking uses the segment index rather than traversing them.

Note: run the programs on your own Linux system as well and replace/keep the `output.txt` files with your own terminal outputs before final submission, as the assignment requires individual execution.
