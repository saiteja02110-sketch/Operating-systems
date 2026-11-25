#!/usr/bin/env python3
"""Video-streaming capacity and throughput planning for Task 6."""

def main():
    # Catalog storage
    videos, avg_gb = 20000, 1
    base_tb = videos * avg_gb / 1000
    print(f"Catalog: {videos} videos x {avg_gb} GB = {base_tb:.0f} TB base storage")
    copies = 3
    raw_tb = base_tb * copies
    print(f"With {copies} copies: {raw_tb:.0f} TB raw")
    print(f"With 20% reserve: {raw_tb * 1.2:.0f} TB raw planned")

    # Monthly transfer
    students, per_student_gb = 200000, 5
    monthly_gb = students * per_student_gb
    print(f"Monthly transfer: {students} students x {per_student_gb} GB "
          f"= {monthly_gb} GB = {monthly_gb / 1000000:.0f} PB")

    # Peak throughput
    concurrent, mbps = 10000, 5
    peak = concurrent * mbps
    print(f"Peak throughput: {concurrent} concurrent x {mbps} Mbps "
          f"= {peak} Mbps = {peak // 1000} Gbps")

    # Blocks in a 1 GB file with 4 KiB blocks
    blocks = 1024**3 // 4096
    print(f"Blocks in a 1 GB file (4 KiB blocks): {blocks}")
    print("Seeking uses the segment index/offset, not a traversal of all blocks.")

if __name__ == "__main__":
    main()
