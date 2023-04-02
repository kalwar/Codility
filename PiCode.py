# you can write to stdout for debugging purposes, e.g.
# print("this is a debug message")

def solution(p, q):
  distinct_strings = [{}]
  seen = {}

  for left, right in zip(p, q):
    if seen.get((left, right)):
      continue
    seen[(left, right)] = True
    for key in distinct_strings:
      left_match, right_match = key.get(left), key.get(right)
      if not (left_match or right_match):
        if right != left:
          distinct_strings.append(key.copy())
          distinct_strings[-1][right] = True
        key[left] = True
    distinct_strings = list(set([frozenset(d.items()) for d in distinct_strings]))
    distinct_strings = [dict(s) for s in distinct_strings]
  return min([len(d) for d in distinct_strings])
