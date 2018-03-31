# -*- coding: utf-8 -*-

import string, codecs
from collections import defaultdict

############################################

# Import n-grams as distributed by http://ruscorpora.ru/corpora-freq.html

ng = { 1: '1grams-3.txt',
       2: '2grams-3.txt',
       3: '3grams-3.txt' }

NGramFile = "ngrams.ngrams-txt"

Counts = defaultdict(int)

for n, fname in ng.items():
    print("Loading", fname)
    with codecs.open(fname, encoding='utf-8') as f:
        for l in f:
            c = l.split('\t')
            count = int(c[0])
            g = []
            i = 1
            while i < len(c):
                g.append( c[i].strip() )
                i += 2

            if n == 1:
                Counts[ g[0] ] = count
            else:
                Counts[ tuple(g) ] = count

#####
print('\nSaving n-grams')
with codecs.open(NGramFile, "w", "utf-8-sig") as f:
    for key, val in Counts.items():
        if type(key)==tuple:
            s = str(len(key)) + ' '
            for k in key:
                s += k + ' '
        else:
            s = '1 ' + key + ' '

        s = s[:-1] + '\t'
        s += str(val) + "\n"
        f.write(s)

print("Ready. Results in", NGramFile)
        
        
    
