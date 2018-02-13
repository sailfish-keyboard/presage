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

#include "trieDatabaseConnector.h"
#include "../../presageException.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sstream>

#include <set>
#include <deque>


TrieDatabaseConnector::TrieDatabaseConnector(const std::string database_name,
                                                           const size_t cardinality,
                                                           const bool read_write)
  : DatabaseConnector(database_name, cardinality, read_write)
{
  if (read_write)
    {
      logger << ERROR << "TrieDatabaseConnector accessed in read-write mode" << endl;
      throw PresageException(PRESAGE_ERROR, "TrieDatabaseConnector accessed in read-write mode");
    }
  
  openDatabase();
}

TrieDatabaseConnector::TrieDatabaseConnector(const std::string database_name,
                                                           const size_t cardinality,
                                                           const bool read_write,
                                                           const std::string logger_level)
  : DatabaseConnector(database_name, cardinality, read_write, logger_level)
{
  if (read_write)
    {
      logger << ERROR << "TrieDatabaseConnector accessed in read-write mode" << endl;
      throw PresageException(PRESAGE_ERROR, "TrieDatabaseConnector accessed in read-write mode");
    }
  
  openDatabase();
}

TrieDatabaseConnector::~TrieDatabaseConnector()
{
  closeDatabase();
}


NgramTable TrieDatabaseConnector::executeSql(const std::string query) const
{
  throw PresageException(PRESAGE_ERROR, "TrieDatabaseConnector does not support SQL queries. The class is used incorrectly");
  return NgramTable(); // to keep compiler happy
}


void TrieDatabaseConnector::openDatabase()
{
  // close database if it has been open earlier
  closeDatabase();
  
  std::string basename = get_database_filename();
  std::string triename = basename + "/ngrams.trie";
  std::string countsname = basename + "/ngrams.counts";

  logger << DEBUG << "Loading database: " << triename << " / " << countsname << endl;

  // load marisa trie
  try {
    db_trie.load(triename.c_str());
  }
  catch (marisa::Exception e) {
    throw PresageException(PRESAGE_ERROR,
                           "TrieDatabaseConnector: Error opening MARISA database");
  }

  // load counts and mmap to int vector
  {
    int fd = open(countsname.c_str(), O_RDONLY);
    if (fd < 0)
      throw PresageException(PRESAGE_ERROR,
                             "TrieDatabaseConnector: Error opening NGrams counts database, failed to get file descriptor");

    struct stat st;
    if (fstat(fd, &st)<0)
      {
        close(fd);
        throw PresageException(PRESAGE_ERROR,
                               "TrieDatabaseConnector: Error opening NGrams counts database, failed to determine file size");
      }
    count_size = st.st_size;

    if ((count_size / sizeof(count_type))*sizeof(count_type) != count_size)
      {
        close(fd);
        throw PresageException(PRESAGE_ERROR,
                               "TrieDatabaseConnector: Error opening NGrams counts database, file size is not matching sizeof(int)");
      }

    count_data = (count_type*)mmap(NULL, count_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (count_data == MAP_FAILED)
      {
        close(fd);
        count_data = nullptr;
        count_size = 0;
        throw PresageException(PRESAGE_ERROR,
                               "TrieDatabaseConnector: Error opening NGrams counts database, mmap failed");
      }

    close(fd); // we can close fd after mmap is established
  }
}

void TrieDatabaseConnector::closeDatabase()
{
  if (count_data)
    {
      munmap(count_data, count_size);
      count_data = nullptr;
      count_size = 0;
    }

  db_trie.clear();
}
  
int TrieDatabaseConnector::getNgramCount(const Ngram ngram) const
{
  std::string search = buildSearchString(ngram);

  marisa::Agent agent;
  agent.set_query(search.c_str());
  int count = 0;
  if (db_trie.lookup(agent))
    count = getCount( agent.key().id() );

  logger << DEBUG << "TrieDatabaseConnector:getNgramCount: " << search << " : " << count << endl;
  
  return count; // ngram not found
}

std::vector<std::string> TrieDatabaseConnector::getPredictedWords(const Ngram ngram,
                                                                  const char** filter,
                                                                  const int count_threshold,
                                                                  int limit) const
{
  // helper struct used to keep intermediate result
  struct Result {
    std::string txt;
    int count{0};
    bool operator < (const Result &other) const {
      return count<other.count || (count==other.count && txt<other.txt);
    }
  };

  std::set<Result> results;
  Result curr_min;

  // form search strings
  std::string search_base = buildSearchString(ngram);
  std::deque<std::string> searches;
  if (filter == NULL)
    searches.push_back(search_base);
  else
    for (int j = 0; filter[j] != 0; j++)
      searches.push_back(search_base + filter[j]);

  for (std::deque<std::string>::const_iterator i = searches.cbegin(); i!=searches.cend(); ++i)
    {
      marisa::Agent agent;
      agent.set_query((*i).c_str());

      logger << DEBUG << "TrieDatabaseConnector:getNgramLikeTable: search string: " << (*i) << endl;
      
      while (db_trie.predictive_search(agent))
        {
          int c = getCount( agent.key().id() );
          if (c > curr_min.count)
            {
              Result r;
              r.count = c;
              r.txt = std::string(agent.key().ptr(), agent.key().length());
              results.insert(r);

              logger << DEBUG << "insert into tmp results: " << r.txt << " -> " << r.count << endl;

              if (results.size() > limit)
                {
                  logger << DEBUG << "drop from tmp results: " << curr_min.txt << " -> " << curr_min.count << endl;
                  
                  results.erase(curr_min);
                  curr_min = *(results.begin());
                }
            }
        }
    }

  logger << DEBUG << "Found words:\n";

  std::string last_word = ngram[ ngram.size() - 1 ];
  std::vector<std::string> table;
  for (std::set<Result>::const_reverse_iterator i = results.crbegin(); i != results.crend(); ++i)
    {
      std::string last_ngram = last_word + i->txt.substr( search_base.length() );
      table.push_back(last_ngram);

      logger << DEBUG << last_ngram << " ";
    }

  logger << DEBUG << endl;
  
  return table;
}


std::string TrieDatabaseConnector::buildSearchString(const Ngram &ngram) const
{
  std::stringstream ss;
  ss << ngram.size() << " ";
  for (size_t i = 0; i < ngram.size(); i++)
    {
      ss << ngram[i]; // TODO: lowercase
      if (i < ngram.size()-1)
        ss << " ";
    }

  return ss.str();
}


///////////////////////////////////////////////////////////////
// Functions interacting with counts data. These have to be in
// sync with the counts data file format
int TrieDatabaseConnector::getUnigramCountsSum()
{
  if (count_data == nullptr) return 0;
  return count_data[0];
}

int TrieDatabaseConnector::getCount(const size_t id) const
{
  size_t idx = 1 /* unigram sum is stored here */ + id;
  if (idx*sizeof(count_type) >= count_size)
    throw PresageException(PRESAGE_ERROR,
                           "TrieDatabaseConnector: requesting count with index that is out of bounds");

  return count_data[idx];
}
