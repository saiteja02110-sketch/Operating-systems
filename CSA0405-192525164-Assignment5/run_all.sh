#!/bin/bash
# CSA0405-192525164-Assignment5 - run everything runnable
# Design/docs tasks are printed/summarised; Docker and Kubernetes steps run
# only when those tools are available on the machine, and say so otherwise.
set -e
cd "$(dirname "$0")"
echo "=== Part A: Design (A1-A5) ==="
echo "Design document: PartA_Design/design.md"
grep '^## ' PartA_Design/design.md || true
echo
echo "=== B1: Linux Administration ==="
echo "--- disk-alert.sh (threshold 80%) ---"
bash B1_Linux_Admin/disk-alert.sh
echo "(disk-alert finished; no WARNING lines means no filesystem is >= 80% full)"
echo "--- log_analysis.sh ---"
bash B1_Linux_Admin/log_analysis.sh
echo "--- observe_system.sh ---"
bash B1_Linux_Admin/observe_system.sh
echo
echo "=== B2: Containers ==="
python3 -m py_compile B2_Containers/app.py && echo "app.py: Python syntax OK"
rm -rf B2_Containers/__pycache__
if command -v docker >/dev/null 2>&1; then
  (cd B2_Containers && docker build -t result-portal . && docker compose config)
else
  echo "Docker is not installed on this machine - Dockerfile and docker-compose.yml are included in B2_Containers/ but were NOT built or run here."
fi
echo
echo "=== B3: Deployment / Kubernetes ==="
echo "deployment.yaml and comparison.md are in B3_Deployment_Kubernetes/"
grep -E 'replicas:|cpu:|memory:|type:' B3_Deployment_Kubernetes/deployment.yaml || true
if command -v kubectl >/dev/null 2>&1; then
  kubectl apply --dry-run=client -f B3_Deployment_Kubernetes/deployment.yaml || true
else
  echo "kubectl is not installed on this machine - deployment.yaml was NOT applied here."
fi
echo
echo "=== B4: Capacity and Decision ==="
python3 B4_Capacity_Decision/capacity_calculator.py
echo
echo "All tasks completed."
