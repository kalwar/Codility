# you can write to stdout for debugging purposes, e.g.
# print("this is a debug message")

def pows(n):
    two = 0
    while n and n % 2 == 0:
        n //= 2
        two += 1
    
    five = 0
    while n and n % 5 == 0:
        n //= 5
        five += 1
    
    return (two, five)

def score(choices):
    a, b = sum(x[0] for x in choices), sum(x[1] for x in choices)
    
    return min(a, b), max(a, b)

from collections import defaultdict

def solution(A):
    seen = defaultdict(int)
    seen2 = defaultdict(int)
    seen3 = defaultdict(int)
    for a, b in map(pows, A):
        for i, v in seen2.items():
            seen3[i + a] = max(seen3[i + a], seen2[i] + b)
        for i, v in seen.items():
            seen2[i + a] = max(seen2[i + a], seen[i] + b)
        for i in range(a+1):
            seen[i] = max(seen[i], b)
            
    res = 0
    for i, v in seen3.items():
        res = max(res, min(i, v))
    return res
