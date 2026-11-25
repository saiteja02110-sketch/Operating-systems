# B4 — Final Engineering Decision

## Capacity estimate (see capacity_calculator.py)
- Average: 150,000 / 7,200 s = 20.83 requests/s (assumes one portal request per student;
  real sessions create multiple requests, so measure requests per session too).
- Peak (illustrative 3× factor): 62.5 requests/s.
- At an illustrative 25 requests/s per container: ceil(62.5 / 25) = 3 containers.
- With redundancy and safety headroom: **4–5 replicas** recommended.
The 25 requests/s figure is an example assumption, not a measurement — replace it with the
actual benchmark, and account for login, database queries, cache misses, request mix and
network latency.

## Options evaluated
| Option | Evaluation |
|---|---|
| Own servers + VMs | Control over hardware and data; predictable local costs. Limited capacity, hardware failure risk, slower expansion. |
| Own servers + containers/Kubernetes | Efficient use of existing hardware, quick app scaling; multi-node setup improves resilience, but local capacity still has a ceiling. |
| Public cloud | Rapid capacity increase, managed database and load balancer, pay-as-used; cost and data-governance/vendor dependency must be monitored. |
| Hybrid (recommended) | Keep steady workloads and private data controls in campus infrastructure; burst the portal web tier to cloud for result day. Connect securely; use tested backup/restore. |

## Recommendation — hybrid
Hybrid cloud with containerised web instances and a managed database (or a carefully
managed database tier). Use a load balancer, autoscaling, cache, private networking,
monitoring, backups and a tested failover plan. Before production: load-test with realistic
traffic, review security, verify database bottlenecks and estimate cloud spend.

Alternative — public cloud only: scales quickly and avoids buying servers for a short
peak; scale up around the event and down afterwards. The provider operates the
infrastructure, but the university remains responsible for access controls, encryption,
retention, legal compliance and data governance.

## Practical extension (B4.1)
A second service (e.g. a learning-management demo) gets its own limits — in Compose as a
separate service definition, in Kubernetes as a separate Deployment/Service, e.g.
requests cpu 100m / memory 128Mi, limits cpu 250m / memory 256Mi. Verify with
`docker stats` or the cluster metrics.

## Conclusion
Linux provides efficient administration and observability; VMs offer OS isolation and
consolidation; containers make the portal portable and fast to scale; Kubernetes supports
replica management, recovery and scaling across nodes. The hybrid, autoscaled design is a
practical balance of availability, cost and control — validated with actual measurements
rather than assumed benchmark values.
