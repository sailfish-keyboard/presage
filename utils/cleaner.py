#! /usr/bin/python3
# -*- coding: utf-8 -*-

# corpus file cleaner
# check output by hand to be sure :-)

# from https://git.tuxfamily.org/okboard/okb-engine.git/

import sys, re

f = sys.stdin.buffer

# language specific
accepted_chars = set(".'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")

buf = f.readline()
while buf:
    line = buf.decode('utf-8', 'ignore')  # ignore UTF-8 encoding errors
    line = re.sub('\r', '', line)

    # convert some UTF-8 character to ASCII equivalent (very incomplete)
    line = re.sub(r'[\u2010\u2011\u2012\u2013\u2014\u2015\u207B\u2212\uff5E]', '-', line)  # hyphen
    line = re.sub(r'[\u030D\u2018\u2019\u201A\u2032\u2039\u203A]', "'", line)  # quote
    line = re.sub(r'[\u030F\u02EE\u201C\u201D\u201E\u2033\u226A\u226B]', '"', line)  # double-quote
    line = re.sub(r'[\u2022\u2026\u2092\u22C5]', '.', line)  # ellipsis & dots

    # maybe to be added when non latin charsets are supported:
    # \u0572-\u0585, \u10dd-\u10eb, \u1d49-\u1ecd -> alpha
    # \u0400-\u04FF (cyrillic)
    # Smilies \u1f440-\u1f4aa, \u1f600-\1f633

    line = re.sub(r'[^\ \!-\~\t\r\n\'\u0080-\u023F]', ' ', line)

    # impose restrictions on words
    words = line.split()
    filtered = ""
    for w in words:
        if set(w) <= accepted_chars and w[-1] != "'":
            filtered += w + " "

    print(filtered)

    buf = f.readline()
