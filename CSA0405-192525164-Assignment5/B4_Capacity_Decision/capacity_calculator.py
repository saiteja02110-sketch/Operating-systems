#!/usr/bin/env python3
"""B4 capacity estimate for the University Result Portal.

Assumptions (from the case study, illustrative - replace the per-container
benchmark with a real measured value before production use):
- 150,000 students in a 2-hour (7,200 s) result-day window, one request each
- peak = 3 x average
- one container serves 25 requests/s (illustrative benchmark only)
"""
import math

students = 150_000
window_seconds = 2 * 60 * 60  # 7200
peak_factor = 3
per_container_rps = 25

average_rps = students / window_seconds
peak_rps = average_rps * peak_factor
containers_needed = math.ceil(peak_rps / per_container_rps)

print("University Result Portal - capacity estimate")
print(f"Students in window: {students}")
print(f"Window: {window_seconds} seconds (2 hours)")
print(f"Average request rate: {students}/{window_seconds} = {average_rps:.2f} requests/s")
print(f"Peak request rate (3x average): {peak_rps:.1f} requests/s")
print(f"Assumed capacity per container: {per_container_rps} requests/s (illustrative)")
print(f"Containers needed at peak: ceil({peak_rps:.1f} / {per_container_rps}) = {containers_needed}")
print("Recommended replicas: 4-5 (peak requirement plus redundancy/headroom)")
