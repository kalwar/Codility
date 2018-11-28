# you can write to stdout for debugging purposes, e.g.
# print("this is a debug message")

_end = '_end_'

def make_trie(words):
    root = dict()
    for word in words:
        current_dict = root
        for letter in word:
            current_dict = current_dict.setdefault(letter, {})
        current_dict[_end] = _end
    return root

def get_endings(di, prefix=''):
    res = []
    for c in di:
        if c == _end:
            res.append(prefix)
        else:
            res.extend(get_endings(di[c], prefix + c))
    return res

def put_spaces(s, A):
    a = A
    cc = ''
    while s:
        c = s[0]
        cc += c
        s = s[1:]
        if c not in a:
            return None
        a = a[c]
        if _end in a:
            sp = put_spaces(s, A)
            if sp:
                return cc + ' ' + sp
    return cc

def return_result(sentence, A):
    s = ''.join(sentence.split(' '))
    if s == s[::-1]:
        return sentence
    return sentence + ' ' + put_spaces(s[::-1], A)

def solution(S):
    A = make_trie(S.split(' '))
    B = make_trie(S[::-1].split(' '))
    
    stack = []
    for a in S.split(' '):
        stack.append((a, '', ''))

    seen = set()

    while stack:
        x, y, sentence = stack.pop()
        if (x, y) in seen:
            continue
        seen.add((x,y))
        bb = B
        cc = ''
        while x:
            if _end in bb:
                stack.append((x, '', sentence + cc))
            c, x = x[0], x[1:]
            cc += c
            if c not in bb:
                cc = ''
                break
            bb = bb[c]
        if cc:
            for ending in get_endings(bb):
                if ending == '':
                    return return_result(sentence + cc, A)
                stack.append(('', ending, sentence + cc + ' '))

        cc = ''
        aa = A
        while y:
            if _end in aa:
                stack.append(('', y, sentence + cc + ' '))
            c, y = y[0], y[1:]
            cc += c
            if c not in aa:
                cc = ''
                break
            aa = aa[c]
        if cc:
            for ending in get_endings(aa):
                if ending == '':
                    return return_result(sentence + cc, A)
                stack.append((ending, '', sentence + cc))
    return "NO"
