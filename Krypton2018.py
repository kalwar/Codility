# you can write to stdout for debugging purposes, e.g.
# print("this is a debug message")

def facs(n):
    a, b = 0, 0
    
    if n == 0:
        return 1, 1
    while n and n % 2 == 0:
        n //= 2
        a += 1

    while n and n % 5 == 0:
        n //= 5
        b += 1
    return (a, b)                

def solution(A):
    n = len(A)
    zero = False
    for i in range(n):
        for j in range(n):
            if (A[i][j] == 0): zero = True
            A[i][j] = facs(A[i][j])
    
    for j in range(1, n):
        A[0][j] = (A[0][j-1][0] + A[0][j][0], A[0][j-1][1] + A[0][j][1])
        A[j][0] = (A[j-1][0][0] + A[j][0][0], A[j-1][0][1] + A[j][0][1])
    
    for i in range(1, n):
        for j in range(1, n):
            A[i][j] = (min(A[i-1][j][0], A[i][j-1][0]) + A[i][j][0],
                       min(A[i-1][j][1], A[i][j-1][1]) + A[i][j][1])
    
    res = min(A[-1][-1])
    if zero: res = min(res, 1)
    return res
