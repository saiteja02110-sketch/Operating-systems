# CSA0405 — Operating Systems — Assignment 5 (CO5)

**GitHub Repository:** (paste your GitHub URL here after uploading)

| Field | Details |
|---|---|
| Name | M Sai Teja |
| Register Number | 192525164 |
| Course Code | CSA0405 |
| Course | Operating Systems |
| Assignment | Assignment 5 — Linux, Containers and Virtualisation for Efficient Operations (CO5) |
| Case Study | University Result Portal |

## Environment
- OS: Linux
- Shell: bash (`bash --version`); scripts run with `bash <script>.sh`
- Python: python3 (B2 syntax check, B4 capacity calculator)
- Docker / Kubernetes: Dockerfile, docker-compose.yml and deployment.yaml are included;
  Docker and kubectl were **not available** on the machine where these outputs were
  captured, so no image build, container run or cluster apply was done there — run those
  on a machine with Docker/Kubernetes and add your own outputs (see `B2_Containers/output.txt`
  and `B3_Deployment_Kubernetes/output.txt`).

## Repository structure
- `PartA_Design/` — `design.md`: A1 Linux components, A2 administration (portal service
  account, systemd, networking/firewall, cron), A3 virtualisation, A4 containers
  (namespaces/cgroups), A5 cloud/Kubernetes concepts
- `B1_Linux_Admin/` — `disk-alert.sh` (warns at ≥ 80% disk use), `log_analysis.sh`
  (status counts, busiest client, total bytes for the embedded sample log),
  `observe_system.sh` (ps / free / df / ss observation), `result-portal.service`
  (systemd unit running as the dedicated `portal` user, `Restart=on-failure`)
- `B2_Containers/` — `app.py` (Flask portal), `requirements.txt`, `Dockerfile`
  (python:3.12-slim, non-root `portal` user, port 8000, Gunicorn), `docker-compose.yml`
  (portal + PostgreSQL 16 with persistent `dbdata` volume and resource limits)
- `B3_Deployment_Kubernetes/` — `deployment.yaml` (Deployment, 3 replicas, readiness
  probe on `/health`, LoadBalancer Service 80 → 8000), `comparison.md` (bare metal vs
  VM vs container — measurement fields left blank to fill from your own real tests)
- `B4_Capacity_Decision/` — `capacity_calculator.py`, `decision.md` (hybrid recommendation)
- Each folder also contains `output.txt` — the actual output captured when the runnable
  items were run on this machine.

## How to run everything
```bash
chmod +x run_all.sh
./run_all.sh
```

## How to run each part individually
```bash
# Part A - design document (read it)
cat PartA_Design/design.md

# B1
cd B1_Linux_Admin && bash disk-alert.sh && bash log_analysis.sh && bash observe_system.sh

# B2 (needs Python packages, and Docker for the container steps)
cd ../B2_Containers && python3 -m py_compile app.py
pip install -r requirements.txt   # then: gunicorn --bind 0.0.0.0:8000 app:app
docker build -t result-portal .   # on a machine with Docker
docker compose up -d              # portal + postgres; set POSTGRES_PASSWORD in a local .env first

# B3 (needs a Kubernetes cluster)
cd ../B3_Deployment_Kubernetes && kubectl apply -f deployment.yaml

# B4
cd ../B4_Capacity_Decision && python3 capacity_calculator.py
```

## Results summary
- Part A: layered Linux design for the portal; dedicated `portal` service account and
  systemd service; private database networking with a firewall allowing only SSH (admin
  networks), HTTP and HTTPS; type-1 KVM recommended for production virtualisation;
  containers (namespaces for isolation, cgroups for limits) for the stateless web tier;
  cloud load balancer + autoscaling pods + managed database for result day.
- B1: `disk-alert.sh` ran with exit 0 and printed no warnings on this machine (no mounted
  filesystem was ≥ 80% full — see `df` output in `output.txt`). `log_analysis.sh` on the
  embedded sample log produced: status counts **200: 4, 302: 1, 403: 1, 404: 1**; busiest
  client **10.0.0.5 with 4 requests**; **total bytes sent: 43200**. `observe_system.sh`
  captured real `ps`, `free -h`, `df -h` and `ss` output.
- B2: `app.py` passes the Python syntax check (`/` returns "University Result Portal",
  `/health` returns `{"status": "ok"}`). Flask is not installed on the capture machine and
  Docker is not available there, so the app was not served and no image was built there —
  this is recorded honestly in `B2_Containers/output.txt`.
- B3: `deployment.yaml` declares 3 replicas, requests 250m CPU / 256Mi memory, limits
  500m / 512Mi, a readiness probe on `/health:8000`, and a LoadBalancer Service
  (port 80 → 8000). It was not applied (no cluster here). The bare-metal/VM/container
  measurement table in `comparison.md` is deliberately left blank — fill it from real
  tests, do not invent measurements.
- B4: capacity — 150,000 students / 7,200 s = **20.83 requests/s average**; peak at 3× =
  **62.5 requests/s**; at an illustrative 25 requests/s per container, ceil(62.5/25) =
  **3 containers**, and **4–5 replicas are recommended** with headroom. Decision: **hybrid**
  — steady/private workloads on campus infrastructure, burst the containerised web tier
  to the cloud for result day, with load balancer, autoscaling, cache, monitoring and backups.

Note: run the programs on your own Linux system as well and replace/keep the `output.txt`
files with your own terminal outputs before final submission, as the assignment requires
individual execution. The database password in `docker-compose.yml` is a local-demo
placeholder — change it via a local, untracked `.env` file (`POSTGRES_PASSWORD`) and never
commit real credentials.
