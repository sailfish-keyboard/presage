# Presage

This is a fork of [Presage](http://presage.sourceforge.net/) developed further 
to incorporate into Sailfish OS keyboard as a prediction module.

This fork should support the same platforms as the upstream. However, it is developed on 
Linux and may lack testing on other platforms.

All changes introduced in the fork are listed at https://github.com/rinigus/presage/compare/upstream...master

Main changes:

* Addition of
  [MARISA](http://s-yata.github.io/marisa-trie/docs/readme.en.html)-based
  Predictor. Compared to SQLite-backed predictor, MARISA-based
  predictor is much faster and requires several times smaller
  databases
  
* Fixes required to make it work on Sailfish OS (loading empty
  configurations and others)
  
* Conversion of floating point numbers in XML configuration is
  performed in "C" locale
  
  
See upstream [README](https://github.com/rinigus/presage/blob/master/README) for general introduction
and build instructions.


## Generation of n-gram database for MARISA-based predictor

There are different ways to generate n-gram databases. One is to use
included `text2ngram` tool and make the database by processing text
corpus with it. Alternative, is to use NLTK or some other
package. Finally, you could use an existing database and convert to a
suitable format. Here, `text2ngram` and NLTK approaches are covered.

In general, its advisable to clean corpus before n-gram database
generation. Also, it is suggested not to store numbers in that
database since they probably have small use in text prediction
context. To check which characters are in your corpus, you could use
`utils/charmap.py` script.


### n-gram database by text2ngram

To generate n-gram database for MARISA-based predictor, make SQLite
based n-gram database first. For that, use the provided `text2ngram`
utility and build sequential n-gram tables in SQLite format. For example

```
for i in 1 2 3; do text2ngram  -n $i -l -f sqlite -o database_aa.db mytext.filtered; done
```

will generate database covering 1, 2, and 3-gram cases. 

With SQLite database ready, run Python script `utils/sqlite2marisa.py`
to convert n-gram database. For example

```
utils/sqlite2marisa.py database_aa.db database_aa
```

If needed, MARISA database can be reduced by cutting off n-grams using
threshold command line option of the converter.

Note that endianness of the system generating the database and
the device on which you plan to use it should be the same.


### n-gram database by NLTK

Natural Language Toolkit [NLTK](http://www.nltk.org/) is a Python
library packaging many related tools. With the respect of n-gram
database generation, its important to split the text into words,
generate all existing n-grams and count their occurrence. Taking into
account that each language has its own character set and tokenization
rules, the scripts for n-gram generation would have to be adjusted to
used corpus and language.

As an example script, `utils/process_en.py` is the script used to
parse English corpus based on [OANC](http://www.anc.org/) and
[OpenSubtitles](http://www.opensubtitles.org/) dump from
[OPUS](http://opus.lingfil.uu.se) (described in Jörg Tiedemann, 2012,
Parallel Data, Tools and Interfaces in OPUS. In Proceedings of the 8th
International Conference on Language Resources and Evaluation, LREC
2012). Corpus was generated from OANC text files, OpenSubtitles parsed
by https://github.com/inikdom/opensubtitles-parser and all `cat`
together.

For those not familiar with NLTK, I suggest to use
`utils/process_en.py` as a base and adjust to your language. Please
submit the scripts to `utils` for language processing, that way it
would be possible to learn new ways and adjust processing for everyone
if needed. In case of English, an existent word tokenization was
adjusted to keep contracted words together.

The processing of the corpus, should generate n-gram database in UTF-8
text file that has the following line format:

```
NGRAM word word ... word\tCOUNT
```

where NGRAM is 1 (for word frequencies), 2 or more, words of the
n-gram are separated by space from NGRAM and between each other, and
COUNT is the number of times it occured in the corpus. Note that COUNT
is separated from the last word by TAB. The order of n-grams in this
file doesn't matter.

Generated n-gram text file can be converted into MARISA database using
`utils/ngramtxt2marisa`. See its help for details.



### Packaging for Sailfish OS

As soon as the database is ready, it is easy to package it for
Sailfish by using a provided script
`packaging/sailfish/package-language.sh` . For that, you need Linux PC
with `rpmbuild` and `sed` installed in the path. Note that `rpmbuild` is available
for Linux distributions that don't use RPMs for native packaging. 

To create RPM with Presage language support, run

```
packaging/sailfish/package-language.sh Language langcode database-directory version
```

where 

* Language: Specify language in English starting with the capital letter, ex 'Estonian'
* langcode: Specify language two-letter code, ex 'et'
* database-directory: Directory path with the MARISA-formatted database
* version: Version of the language package, ex '1.0.0'

When finished, language support will be packaged into RPM in the current directory. For example, 
Estonian database is packaged using

```
packaging/sailfish/package-language.sh Estonian et database_et 1.0.0
```
