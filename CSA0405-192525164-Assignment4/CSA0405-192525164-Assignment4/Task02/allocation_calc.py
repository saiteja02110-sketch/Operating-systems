#!/usr/bin/env python3
"""Allocation cost and free-space calculations for Task 2."""

BLOCK = 4096  # 4 KB

def main():
    # Blocks in one 4 GB file
    file_bytes = 4 * 1024**3
    print(f"4 GB file: {file_bytes} bytes / {BLOCK} = {file_bytes // BLOCK} blocks")

    # Inode capacity: 12 direct + single + double + triple indirect,
    # each indirect block holds 4096 / 4 = 1024 pointers
    per_indirect = BLOCK // 4
    max_blocks = 12 + per_indirect + per_indirect**2 + per_indirect**3
    print(f"Pointers per indirect block: {per_indirect}")
    print(f"Inode maximum: 12 + 1024 + 1024^2 + 1024^3 = {max_blocks} blocks")
    print(f"Inode maximum file size: {max_blocks * BLOCK} bytes "
          f"= {max_blocks * BLOCK / 1024**4:.2f} TiB")

    # Cost to reach the final block of the 4 GB file
    print("Cost to reach final block: contiguous ~1 data read; "
          "linked up to 1,048,576 traversals; indexed ~3 reads "
          "(double-indirect + leaf index + data)")

    # Free-space bit vector for a 4 TB disk
    for label, total in (("4 TB (decimal)", 4 * 1000**4), ("4 TiB", 4 * 1024**4)):
        blocks = total // BLOCK
        bitmap_bytes = -(-blocks // 8)  # round up: a partial byte still needs a byte
        print(f"{label}: {blocks} blocks, bit vector = {bitmap_bytes} bytes "
              f"= {bitmap_bytes / 1024**2:.1f} MiB")

if __name__ == "__main__":
    main()
