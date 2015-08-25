# you can use print for debugging purposes, e.g.
# print "this is a debug message"

def solution(A, B, K):
    M = len(A) + 1
    class Node(object):
        def __init__(self, idx):
            self.idx = idx
            self.neighbours = []
            self.children = []
            self.height = -1
            self.parent = None
    
    graph = [Node(idx) for idx in xrange(M)]
    for v1, v2 in zip(A, B):
        graph[v1].neighbours.append(graph[v2])
        graph[v2].neighbours.append(graph[v1])
    
    def root(node, parent=None):
        node.parent = parent
        for child in node.neighbours:
            if child is parent: continue
            node.children.append(child)
            root(child, node)
            
    root(graph[0])
    
    def needed_cameras(node, allowed_diameter):
        result = 0
        node.height = 0
        for child in node.children:
            result += needed_cameras(child, allowed_diameter)
        
        sigh = sorted((child.height, cid, child) 
                      for cid, child in enumerate(node.children)
                      if child.height < allowed_diameter)
        sigh = [child for _, _, child in sigh]
        result += len(node.children) - len(sigh)
        while len(sigh) > 1 and sigh[-1].height + sigh[-2].height + 2 > allowed_diameter:
            sigh.pop()
            result += 1
            
        for child in sigh:
            node.height = max(node.height, 1 + child.height)
            
        return result
        
    left = 0
    right = 900
    while left < right:
        mid  = (left + right) / 2
        need = needed_cameras(graph[0], mid)
        if need <= K:
            right = mid
        else:
            left = mid + 1
    return left