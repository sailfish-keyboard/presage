# get charmap

import sys, re

f = sys.stdin.buffer

charmap = set()

buf = f.readline()
while buf:
    line = buf.decode('utf-8', 'ignore')  # ignore UTF-8 encoding errors
    for k in line:
        charmap.add(k)
    buf = f.readline()

chars = list(charmap)
chars.sort()
s = ''
for k in chars:
    s += k + " "
print(s)
