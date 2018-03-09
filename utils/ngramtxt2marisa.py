#!/usr/bin/env python3

# Create MARISA-based database for n-grams
#
# Requires: python3, numpy, sqlite and marisa python bindings

import argparse

import numpy as np
import marisa, codecs, os, sys

parser = argparse.ArgumentParser(description='''
This script generates N-gram database suitable for MARISA-based
Presage predictor. The database is stored in dedicated folder
and consists of MARISA Trie file with n-gram words and separate
file with n-gram counts.

The database is generated from n-gram text file consisting of lines
in the following format:
NGRAM word word ... word\tCOUNT

where NGRAM is 1 (for word frequencies), 2 or more, words of the n-gram are separated by
space from NGRAM and between each other, and COUNT is the number of times it occured in
the corpus. Note that COUNT is separated from the last word by TAB
''')

parser.add_argument('inputfile', type=str,
                    help='n-gram text file')

parser.add_argument('output', type=str,
                    help='Name of the new directory where n-gram database in MARISA format will be written. This directory will be created by the script')

parser.add_argument('--threshold', type=int, default=0,
                    help='Minimal n-gram counts propagated into the database. Default 0 (all recorded n-grams are propagated into MARISA-based database)')

parser.add_argument('--overwrite', action="store_true",
                    help='Overwrite existing MARISA database')



args = parser.parse_args()

if os.path.exists(args.output):
    if args.overwrite:
        print('\nGoing to overwrite existing MARISA trie database\n')
    else:
        print('Cannot write MARISA database into existing directory %s' % args.output)
        print('Please provide path for directory that will be created by this script')
        sys.exit(-1)

else:
    os.makedirs(args.output)

factor = max(args.threshold,1)

# open the data file
print('Loading n-grams')
data = {}
ranges = {}
keyset = marisa.Keyset()
with codecs.open(args.inputfile, encoding='utf-8') as f:
    for line in f:
        key, count = line.split('\t')
        count = int(count)
        if count >= args.threshold:
            data[key] = count
            keyset.push_back(key)
            n = key.split()[0]
            if n in ranges:
                mn, mx = ranges[n]
                ranges[n] = ( min(mn,count), max(mx, count) )
            else:
                ranges[n] = ( count, count )
                

kk = list(ranges.keys())
kk.sort()
for k in kk:
    print('Range of counts for ' + k + '-gram: ', ranges[k][0], ranges[k][1]) 
    
# get sum
print('Calculating sum of word counts')
scount = 0
for k, v in data.items():
    if k[:2] == '1 ':
        scount += v
if scount==0:
    scount = 1 # setting 1 as minimum
    
print("\nSum of 1-gram:", scount)
if factor > 1:
    scount = int(scount/factor)
    print("Normalized sum of 1-gram:", scount)
    
if scount > 2**31:
    print("Trouble: sum of 1-grams doesn't fit INT32. Please normalize the data manually or automatically by increasing threshold for counts")
    sys.exit(-1)

print()

# save ngrams
print('Saving in Marisa format')
trie = marisa.Trie()
trie.build(keyset)
trie.save(os.path.join(args.output, "ngrams.trie"))

print("Keys: ", trie.num_keys(), "\n")

arr = np.zeros(trie.num_keys()+1, dtype=np.int32)
arr[0] = scount
agent = marisa.Agent()
for k in data:
    agent.set_query(k)
    trie.lookup(agent)
    arr[ agent.key_id() + 1 ] = int(data[k] / factor)

binwrite=open(os.path.join(args.output, 'ngrams.counts'),'wb')
arr.tofile(binwrite)
binwrite.close()
