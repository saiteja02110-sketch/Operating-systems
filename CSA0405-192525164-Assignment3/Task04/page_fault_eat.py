#!/usr/bin/env python3
"""Effective Access Time under demand paging.
memory access = 100 ns, page-fault service = 8 ms = 8,000,000 ns
EAT = (1-p)*100 + p*8,000,000
"""
MEM=100
FAULT=8_000_000
for label,p in [("1/1,000",1/1000),("1/100,000",1/100000),("1/1,000,000",1/1000000)]:
    eat=(1-p)*MEM + p*FAULT
    print(f"fault rate {label}: EAT = {eat:.0f} ns ({eat/MEM:.2f}x base)")
# max p for at most 10% slowdown: EAT <=110
pmax=10/(FAULT-MEM)
print(f"max fault rate for 10% slowdown: p <= {pmax:.3e} (about 1 fault per {1/pmax:,.0f} accesses)")
