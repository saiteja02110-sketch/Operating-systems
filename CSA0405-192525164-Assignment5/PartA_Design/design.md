# Part A — Design (A1–A5): University Result Portal

Case study: a University Result Portal used by a few hundred users on normal days, but
about 150,000 students may access results during a two-hour result-day window. The current
single physical server is unreliable, so the design uses Linux, containerised application
instances, a managed database, load balancing and autoscaling.

## A1 — Linux System Components

### A1.1 Architecture
Layers, top to bottom: User / Web Portal → Shell & Utilities → System Libraries / APIs →
Linux Kernel → Hardware.

| Component | Role in the portal server |
|---|---|
| Kernel | Schedules CPU processes, manages virtual memory, file access, device drivers and the network stack. |
| System libraries | Provide standard interfaces such as libc so applications can use system calls. |
| Shell | Accepts commands and runs scripts for administration and automation. |
| Utilities | Tools such as `ps`, `grep`, `systemctl`, `df`, `ip` and `journalctl` help manage and diagnose the server. |

The kernel keeps processes isolated, allocates memory, manages files through file-system
drivers, controls devices through drivers, and handles network packets.

### A1.2 Process and Memory Management
| Feature | Evaluation |
|---|---|
| Completely Fair Scheduler (CFS) | Shares CPU time among runnable tasks using scheduling weights; responsive under competition, but CPU saturation still increases latency. |
| Page cache | Keeps recently accessed file data in RAM to reduce repeated disk reads; improves database/web performance and can be reclaimed when applications need memory. |
| Swap | Moves less-used pages to disk under RAM pressure; prevents some out-of-memory failures but is much slower than RAM, so heavy swapping causes latency. |
| /proc | Virtual file system exposing process information, CPU, memory and kernel state — low-cost live observability. |
| /sys | Exposes device, kernel and subsystem configuration and statistics. |

Evaluation: CFS suits general server workloads; use monitoring and resource limits so one
process cannot consume all CPU. Page cache improves performance; swap should be monitored,
not treated as extra fast RAM.

### A1.3 File System and Permissions
Single directory tree rooted at `/`: `/etc` configuration, `/var/log` logs, `/home` users,
`/usr` programs and libraries, `/tmp` temporary files, `/var` changing data. The Virtual
File System (VFS) gives a common interface to different file systems.

Permissions are read (r), write (w), execute (x), assigned separately to owner, group and
others. `setuid` lets an executable run with the file owner's effective identity and must be
used carefully. `sudo` lets an authorised user run selected commands with elevated
privileges, with policy and logging.

### Alternative considered: Windows Server
Linux is preferred for this Python portal: typically lower base memory/disk footprint
(minimal distribution, no GUI), automation via shell, systemd, cron and Ansible with text
configuration, and many distributions carry no per-server OS licence fee. Windows Server
offers PowerShell/Task Scheduler and integrates well where staff know it, but typically has
a larger base footprint and per-server/per-core licensing costs depending on edition.

## A2 — System Administration

### A2.1 Users, Services and Logs
| Account / service | Purpose |
|---|---|
| `portal` (service account) | Owns application files; no interactive login; least privilege. |
| `portal-admins` (group) | Limited group for approved administrators/maintenance. |
| `result-portal.service` | Starts Gunicorn under the `portal` user, restarts on failure. |
| journald | Collects system and service logs; logs are reviewed and retained. |

Monitor application errors, CPU, memory, disk, failed logins and service restarts. Never run
the web process as root.

### A2.2 Networking
Flow: Students → DNS → Load Balancer → Nginx / Reverse Proxy → App Instances → Managed DB.

| Setting | Design |
|---|---|
| IP addressing | Private static/reserved IP for servers; public traffic reaches only the load balancer. |
| DNS | `portal.university.example` points to the load balancer. |
| Reverse proxy | Nginx terminates TLS, forwards to app instances, can apply basic rate limits. |
| Firewall | Allow inbound SSH only from trusted admin networks, HTTP (redirect) and HTTPS; deny other inbound traffic. |
| SSH | Key-based login; disable password login where safe; restrict admin source IPs. |
| Database | Private network only; never expose the database port to the public internet. |

### A2.3 Automation
Automate backups, backup verification, log rotation, disk-space alerts, updates in
maintenance windows, service health checks and cleanup of old temporary files. Cron for
simple recurring jobs; systemd timers for service-aware scheduling.

```cron
0 * * * * /usr/local/sbin/disk-alert.sh
15 2 * * * /usr/local/sbin/portal-backup.sh
```

Alternative — Ansible vs shell scripts: shell scripts are fast and simple for one server,
but coordination, error handling and configuration drift grow across many hosts. Ansible
costs more to learn, but one idempotent playbook can target 20 servers consistently.
Recommendation: shell scripts for a few simple local tasks; Ansible as the portal grows.

## A3 — Virtualisation

### A3.1 Hypervisors
| Type | Examples | Description |
|---|---|---|
| Type 1 (bare metal) | KVM, VMware ESXi, Xen, Hyper-V | Runs on/near hardware and manages guest VMs; common for production servers. |
| Type 2 (hosted) | VirtualBox, VMware Workstation | Runs as an application on a host OS; convenient for development and labs. |

Full virtualisation presents virtual hardware so an unmodified guest OS can run.
Paravirtualisation uses guest-aware interfaces/drivers for efficiency. Intel VT-x and
AMD-V are CPU extensions supporting hardware-assisted virtualisation.

### A3.2 Benefits and Costs
Benefits: isolation between workloads; consolidation reduces physical-server count;
snapshots support testing and rollback; live migration can move VMs during maintenance.
Costs: CPU and I/O overhead; each VM needs RAM, disk and patching (licences may apply);
snapshots are not a substitute for independent backups; live migration needs compatible
hosts, network capacity and storage coordination.

Recommendation: type-1 KVM for production — efficient and suited to server workloads.
Avoid overcommitting memory during result-day traffic.
Alternative: Type-2 VirtualBox is easy for development/student labs, but the host OS is an
extra dependency and layer, so it is less suitable for a critical production portal.

## A4 — Containerisation

### A4.1 How Containers Work
Linux **namespaces** isolate views of resources (processes, network interfaces, mounts,
hostnames). **Control groups (cgroups)** limit and account for CPU, memory and other
resources. Images are assembled from read-only layers; a running container adds a writable
layer. Shared layers reduce duplication and make image distribution efficient. Containers
share the host Linux kernel.

### A4.2 Containers vs Virtual Machines
| Aspect | Containers | Virtual machines |
|---|---|---|
| Startup | Usually seconds or less | Often tens of seconds to minutes |
| Memory | Share host kernel; generally smaller per instance | Each VM runs a guest OS and consumes additional memory |
| Isolation | Process-level; kernel shared | Separate guest kernel; generally stronger boundary |
| Security | Needs careful privileges, image scanning, kernel hardening | Guest OS boundary gives more separation but still needs patching |
| Portal use | Good for scaling stateless web instances quickly | Good for legacy services or workloads needing a distinct OS |

Recommendation: containers for the Python portal web tier, database data stored
persistently and preferably on a managed database. Alternative (portal and database in
separate VMs) gives stronger isolation but slower provisioning and more memory per service.

## A5 — Cloud Deployment and Orchestration

### A5.1 Cloud Set-up
| Model | Meaning | Portal example |
|---|---|---|
| IaaS | Provider supplies virtual infrastructure; customer manages OS and software | Cloud VMs, networks and disks |
| PaaS | Provider manages runtime/platform; customer deploys app | Managed Python application platform |
| SaaS | Ready-to-use software delivered as a service | University consumes a hosted service instead of deploying its own portal |

Result-day architecture: Students → DNS + TLS → Cloud Load Balancer → Autoscaling Web
Pods → Managed DB, with Backups / Monitoring. Use private subnets for app and database, a
public load balancer, multiple portal instances, a managed database with backups and high
availability, object storage for static assets, monitoring and alerting. Keep the database
private. Scale replicas during result-day traffic; use a cache where appropriate after
validating data correctness.

### A5.2 Kubernetes
| Term | Explanation |
|---|---|
| Pod | Smallest deployable unit; one or more containers |
| Deployment | Declares desired number and version of pods; replaces failed pods |
| Service | Stable network endpoint for selected pods; distributes traffic |
| Replicas | Number of desired pod copies for availability and capacity |
| Resource requests | CPU/memory used for scheduling |
| Resource limits | Maximum CPU/memory a container may use |
| Horizontal Pod Autoscaler (HPA) | Adjusts replica count from metrics such as CPU utilisation |

Kubernetes improves resilience by replacing failed pods and helps scale web capacity. It
does not automatically guarantee database correctness or remove the need for backups,
monitoring and capacity planning.

Alternative: a fixed VM fleet sized for result day responds immediately but wastes
resources on ordinary days; autoscaling is more flexible but needs sensible min/max
replicas and warm-up testing.
