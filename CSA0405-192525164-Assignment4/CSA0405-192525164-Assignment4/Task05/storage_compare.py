#!/usr/bin/env python3
"""Distributed and cloud storage comparison for Task 5."""

ROWS = [
    ("NFS",
     "Depends on server/export and protocol semantics; shared namespace",
     "Single server is a potential single point of failure unless HA is configured",
     "Good LAN access; network/server bottleneck",
     "Low to moderate; server and maintenance"),
    ("HDFS / Ceph",
     "Distributed metadata/data consistency model; depends on system and configuration",
     "Replication and failover improve availability",
     "Scales with nodes and parallelism",
     "Higher setup and operational complexity"),
    ("Cloud object storage",
     "Provider-defined strong/eventual consistency; check the service",
     "Multi-zone options and managed service",
     "Internet/network latency; scalable throughput",
     "Usage-based: storage, requests and transfer"),
]

COMPONENTS = [
    ("Device driver", "Translates OS requests into hardware-specific operations"),
    ("Buffering", "Temporarily holds data to handle speed differences"),
    ("Caching", "Keeps frequently accessed blocks in memory to reduce physical I/O"),
    ("Spooling", "Queues work for a device that handles one stream/job at a time"),
    ("Block-device interface", "Provides block-addressed read/write operations"),
]

def main():
    print("Storage options (consistency | availability | performance | cost):")
    for r in ROWS:
        print(f"  {r[0]}")
        print(f"    Consistency: {r[1]}")
        print(f"    Availability: {r[2]}")
        print(f"    Performance: {r[3]}")
        print(f"    Cost: {r[4]}")
    print()
    print("Replication: three replicas tolerate a node/disk failure - another")
    print("copy is served and the missing copy is repaired later - at the cost")
    print("of more storage and network bandwidth. Strong consistency makes")
    print("reads reflect completed writes but needs coordination; eventual")
    print("consistency allows temporary differences between replicas.")
    print()
    print("OS device-management components:")
    for name, role in COMPONENTS:
        print(f"  {name}: {role}")

if __name__ == "__main__":
    main()
