# Presage

This is a fork of [Presage](http://presage.sourceforge.net/) developed further 
to incorporate into Sailfish OS keyboard as a prediction module.

This fork should support the same platforms as the upstream. However, it is developed on 
Linux and may lack testing on other platforms.

All changes introduced in the fork are listed at https://github.com/rinigus/presage/compare/upstream...master

Main changes:

* Addition of
  (MARISA)(http://s-yata.github.io/marisa-trie/docs/readme.en.html)-based
  Predictor. Compared to SQLite-backed predictor, MARISA-based
  predictor is much faster and requires several times smaller
  databases
  
* Fixes required to make it work on Sailfish OS (loading empty
  configurations and others)
  
* Conversion of floating point numbers in XML configuration is
  performed in "C" locale
  
  
See upstream (README)[https://github.com/rinigus/presage/blob/master/README] for general introduction
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
