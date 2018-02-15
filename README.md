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
