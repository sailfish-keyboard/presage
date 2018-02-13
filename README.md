Presage
=======

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
