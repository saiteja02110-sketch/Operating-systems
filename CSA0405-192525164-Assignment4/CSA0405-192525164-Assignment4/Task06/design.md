# Task 6 - Video-Streaming Storage Design

## Workload
Uploads are write-once, large and sequential. Playback is sequential with
buffering. Seeks are random access to an arbitrary minute. Popular (hot)
videos get many reads, especially in the first week. Expired content must
be deleted and storage audited.

Design targets (proposed, not measured): time-to-first-frame under
2 seconds on the campus network for cached content; seek response under
1 second; monitor rebuffering and cache-hit ratio.

## Architecture
```
Student clients
   |  HTTPS
Load balancer / streaming service
   |
   +-- SSD cache: popular videos, segment index and metadata
   +-- HDD storage pool: full video library (large sequential reads)
   +-- Optional cloud object storage + CDN: overflow / disaster recovery
Replication + checksums + monitoring across the pool
```
Videos are stored as immutable files using segmented streaming
(HLS/DASH), so a seek reads the segment index and then only the requested
segment - it never reads all preceding blocks.

## Stage 3 - critical design decisions
| Decision | Proposed choice | Reason |
|---|---|---|
| Block size | 4 KiB file-system blocks; streaming segments of a few seconds | Small blocks suit metadata/files; segment size is a streaming-layer choice |
| File system | ext4 or XFS on Linux server volumes | Mature, journalled, good sequential performance |
| Allocation | Contiguous / extent-based, managed by the file system | Efficient large sequential reads |
| Scheduler | mq-deadline or none, depending on SSD/HDD and tests | SSDs have no seek cost; HDDs benefit from deadline scheduling |
| Caching | RAM + SSD cache for popular / first-week videos | Hot videos served without HDD reads |
| Replication | At least 2 copies for critical content; 3 where the availability target warrants it | Survive failures, with a capacity trade-off |

## Alternatives
| Design | Advantages | Limitations |
|---|---|---|
| A: HDD pool + SSD cache | Lower storage cost; sequential reads efficient; hot videos on SSD | HDD random-seek latency; cache must be managed |
| B: SSD-only | Fast seeks and high IOPS; simpler caching layer | Higher purchase and capacity cost |
| C: Cloud object storage + CDN | Provider handles durability and scaling; edge caching helps remote students | Egress cost can dominate at high transfer volumes; less direct control |

## Recommendation
Start with the hybrid design A: an HDD pool for the full catalog, SSD/RAM
caching for popular videos, and a cloud/CDN option for peak demand or
remote students. Keep videos immutable, use segmented streaming for
efficient seeking, replicate critical metadata, and compare the hybrid
with SSD-only under the same measured workload before final purchase.
See capacity_calculator.py for the capacity and throughput planning.
