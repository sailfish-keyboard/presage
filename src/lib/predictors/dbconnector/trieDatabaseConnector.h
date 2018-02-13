/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

 *  This file: Copyright (C) 2018 rinigus https://github.com/rinigus

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/

#ifndef TRIEDATABASECONNECTOR_H
#define TRIEDATABASECONNECTOR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef HAVE_MARISA_H
#error "Requires MARISA library"
#endif

#include "databaseConnector.h"

#include <marisa.h>


// Read only database based on Marisa Trie (for keeping n-gram words)
// and counts file (integers stored as a binary array)
//
// Interface is based on DatabaseConnector with some important differences in
// data retrieval to optimize the performance. 
//
// Note that this class should not be used via pointer to DatabaseConnector

class TrieDatabaseConnector: public DatabaseConnector {
public:
  TrieDatabaseConnector(const std::string db,
                        const size_t cardinality,
                        const bool read_write);
  TrieDatabaseConnector(const std::string db,
                        const size_t cardinality,
                        const bool read_write,
                        const std::string logger_level);
  virtual ~TrieDatabaseConnector();

  /** Returns an integer equal to the sum of the counts of all unigrams.
   */
  int getUnigramCountsSum();

  /** Returns an integer equal to the specified ngram count.
   */
  int getNgramCount(const Ngram ngram) const;

  /** Returns predicted words matching the specified ngram-like
   ** query, satisfying the given filter and count threshold.
   */
  std::vector<std::string> getPredictedWords(const Ngram ngram,
                                             const char** filter,
                                             const int count_threshold,
                                             int limit = -1) const;

protected:
  virtual void openDatabase();
  virtual void closeDatabase();

  std::string buildSearchString(const Ngram &ngram) const;

  int getCount(const size_t id) const;

  virtual NgramTable executeSql(const std::string query) const;

private:
  marisa::Trie db_trie;

  typedef int32_t count_type;
  count_type *count_data{nullptr};
  size_t count_size{0};
};


#endif
