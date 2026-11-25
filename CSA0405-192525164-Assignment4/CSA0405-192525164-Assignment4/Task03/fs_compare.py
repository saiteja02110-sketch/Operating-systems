#!/usr/bin/env python3
"""Modern file-system comparison and journaling modes for Task 3."""

ROWS = [
    ("FAT32", "File 4 GiB-1; volume varies", "No", "No native", "No / No"),
    ("NTFS", "Very large (implementation limits)", "Metadata journal", "Limited (Windows features)", "Integrity features / not general CoW"),
    ("ext4", "File up to 16 TiB; volume up to 1 EiB", "Yes", "No native", "Metadata checksums / no general CoW"),
    ("XFS", "Large-scale volumes and files", "Metadata journal", "No native", "Metadata CRC / no general CoW"),
    ("Btrfs", "Large (multi-PiB design limits)", "CoW transactional metadata", "Yes", "Checksums / CoW"),
    ("ZFS", "Very large pools and files", "Transactional intent log", "Yes", "Checksums / CoW"),
    ("APFS", "Large (64-bit addressing)", "Copy-on-write design", "Yes", "Metadata checksums / CoW"),
]

JOURNAL = [
    ("data=journal", "Journal file data and metadata", "Strongest protection; slower writes"),
    ("data=ordered", "Journal metadata; write data before related metadata commits", "Common default; balances speed and consistency"),
    ("data=writeback", "Journal metadata only, no data ordering", "Faster in some workloads; stale data may appear after a crash"),
]

CHOICES = [
    ("Pen drive shared Windows/Linux", "exFAT (or FAT32)", "Portable; exFAT avoids the FAT32 4 GiB file limit"),
    ("Linux database server", "ext4 or XFS", "Reliable journaling, mature tools, good performance"),
    ("Laptop", "NTFS (Windows) / APFS (macOS) / ext4 (Linux)", "Native OS file system and recovery tools"),
    ("Snapshot backup server", "Btrfs or ZFS", "Snapshots, checksums and copy-on-write"),
]

def main():
    print("File-system comparison (max file/volume | journal | snapshots | checksums/CoW):")
    for r in ROWS:
        print(f"  {r[0]:<6} | {r[1]} | {r[2]} | {r[3]} | {r[4]}")
    print()
    print("ext4 journaling modes:")
    for mode, behaviour, trade in JOURNAL:
        print(f"  {mode:<16} {behaviour} -- {trade}")
    print()
    print("Recommended choices:")
    for device, choice, reason in CHOICES:
        print(f"  {device}: {choice} -- {reason}")

if __name__ == "__main__":
    main()
