# B3 — Bare Metal vs VM vs Container

Deploy the same sample Flask/Gunicorn portal in all three environments: bare metal uses
the host Linux directly; the VM adds a guest OS on KVM/VirtualBox; the container shares the
host kernel with an isolated process environment.

## Measurement procedure
- Start-up time: time the launch command, repeat 3 times.
- Memory: `free -h` before/after; `docker stats --no-stream` for the container.
- Disk / image size: `df -h`; `docker images` for the container image.
- HTTP throughput: `ab -n 1000 -c 50 http://127.0.0.1:8000/` (or wrk/hey), same duration
  and concurrency in all three environments. Record test conditions (CPU, RAM, OS,
  storage, network).

## Results — fill in from your own real tests (do not invent measurements)
| Metric | Bare metal | VM | Container |
|---|---|---|---|
| Start-up time | Record seconds | Record seconds | Record seconds |
| Memory used | Record RAM before/after | Guest + host overhead | Container usage + host |
| Disk / image size | OS + app files | Guest disk size | Image size |
| Requests per second | Measured ab/wrk/hey | Measured ab/wrk/hey | Measured ab/wrk/hey |

Central comparison table (fill with actual results):
| Deployment | Start-up | RAM | Size | RPS | Interpretation |
|---|---|---|---|---|---|
| Bare metal | ____ | ____ | ____ | ____ | Direct host performance |
| VM | ____ | ____ | ____ | ____ | Includes guest OS overhead |
| Container | ____ | ____ | ____ | ____ | Usually lightweight; shared kernel |

## Engineering trade-off
| Method | Advantages | Limitations |
|---|---|---|
| Bare metal | Lowest extra virtualisation layer; direct access to resources | Weak workload isolation; one OS failure affects services; manual scaling |
| VM | Separate guest OS; strong separation; flexible OS choices | More memory/disk overhead; VM start-up slower; scaling often slower |
| Container | Fast start-up, low overhead, portable image, easy replica scaling | Shared kernel; requires careful security, image management and orchestration |

Alternative — Docker Compose vs Kubernetes: Compose can scale replicas on one server, but
that server remains a failure domain and resource ceiling. Kubernetes schedules replicas
across multiple nodes and replaces failed pods, at the cost of cluster management and
monitoring. Compose suits small deployments; Kubernetes suits multi-node resilience and
autoscaling.

Apply/test commands:
```bash
kubectl apply -f deployment.yaml
kubectl get pods -o wide
kubectl delete pod <pod-name>   # a replacement pod is scheduled
kubectl scale deployment result-portal --replicas=6
```
