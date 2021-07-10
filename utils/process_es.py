# -*- coding: utf-8 -*-

import nltk, string, codecs
from nltk.util import ngrams
from nltk.corpus import words
from collections import defaultdict
from progress_bar import progress_bar

############################################
CorpusFName = 'corpus-es.txt'

# NGram output
NGramFile = CorpusFName + ".ngram-txt"

lang = 'spanish'

NGrams = 3
verbose = False

punctiation = set(string.punctuation)

# language specific: allowed chars
accepted_chars = set("AaBbCcDdEeFfGgHhIiJjKkLlMmNnÑñOoPpQqRrSsTtUuVvWwXxYyZzÁáÉéÍíÓóÚúÜü")

def allowed(word):
    return len(word) > 1 and set(word) <= accepted_chars

# load corpus into RAM
print('Loading corpus')
#txt = open(CorpusFName, 'rU').read()
txt = codecs.open(CorpusFName, encoding='utf-8').read()

# select word tokenizer
#tokenizer = TreebankWordTokenizerNoContract()

# split text into sentences
print ('Splitting to sentences')
sents = nltk.sent_tokenize(txt, language=lang)

# init count
Counts = defaultdict(int)
lines = 0
nlines = len(sents)
for s in sents:
    tokens = nltk.word_tokenize(s)

    # drop tokens that are punctuation symbols only
    keep = []
    for t in tokens:
        if not set(t) <= punctiation:
            keep.append(t.lower())

    if verbose:
        print('Original:', s)
        print('Parsed:', end=' ')
        for w in keep:
            print(w, end=' ')
        print()

    # word frequencies
    for w in keep:
        if allowed(w):
            Counts[w] += 1

    for N in range(2,NGrams+1):
        grams = ngrams(keep, N)
        for g in grams:
            ok = True
            for w in g:
                ok = (ok and allowed(w))
            if ok:
                Counts[g] += 1

    lines += 1
    if not verbose and lines % 1000 == 0:
        progress_bar(lines, nlines, prefix='Processed sentences:', decimals=3)

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
