#!/usr/bin/env python3
"""FIFO, LRU and Optimal page replacement."""

def fifo(ref, frames):
    mem, faults = [], 0
    for p in ref:
        if p in mem: continue
        faults+=1
        if len(mem)<frames: mem.append(p)
        else: mem.pop(0); mem.append(p)
    return faults

def lru(ref, frames):
    mem, faults, recent = [], 0, []
    for p in ref:
        if p in mem:
            recent.remove(p); recent.append(p); continue
        faults+=1
        if len(mem)<frames:
            mem.append(p)
        else:
            victim=recent.pop(0); mem.remove(victim); mem.append(p)
        recent.append(p)
    return faults

def optimal(ref, frames):
    mem, faults = [], 0
    for i,p in enumerate(ref):
        if p in mem: continue
        faults+=1
        if len(mem)<frames:
            mem.append(p)
        else:
            def next_use(x):
                try: return ref[i+1:].index(x)
                except ValueError: return 10**9
            victim=max(mem, key=next_use)
            mem.remove(victim); mem.append(p)
    return faults

REF=[7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1]
BELADY=[1,2,3,4,1,2,5,1,2,3,4,5]
print("Reference string:", REF)
for frames in (3,4):
    print(f"{frames} frames: FIFO {fifo(REF,frames)}, LRU {lru(REF,frames)}, Optimal {optimal(REF,frames)}")
print("Belady string:", BELADY)
for frames in (3,4):
    print(f"Belady FIFO with {frames} frames: {fifo(BELADY,frames)} faults")
