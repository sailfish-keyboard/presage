
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

 *  Adjustments for TrieDatabaseConnector: Copyright (C) 2018 rinigus https://github.com/rinigus

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


#ifndef PRESAGE_SMOOTHEDNGRAMTRIEPREDICTOR
#define PRESAGE_SMOOTHEDNGRAMTRIEPREDICTOR

#include "predictor.h"
#include "../core/logger.h"
#include "../core/dispatcher.h"
#include "dbconnector/trieDatabaseConnector.h"

#include <assert.h>

/** Smoothed n-gram statistical predictor.
 *
 */
class SmoothedNgramTriePredictor : public Predictor, public Observer {
public:
  SmoothedNgramTriePredictor(Configuration*, ContextTracker*, const char*);
  virtual ~SmoothedNgramTriePredictor();

  virtual Prediction predict(const size_t size, const char** filter) const;

  // no-op
  virtual void learn(const std::vector<std::string>& change);

  // no-op
  virtual void forget(const std::string& word);

  virtual void update (const Observable* variable);

protected:
  virtual void init_database_connector_if_ready ();
    
private:
  unsigned int count(const std::vector<std::string>& tokens, int offset, int ngram_size) const;

  void set_dbfilename (const std::string& filename);
  void set_deltas (const std::string& deltas);
  void set_count_threshold (const std::string& value);
  void set_database_logger_level (const std::string& level);

protected:
  TrieDatabaseConnector*  db;
  std::string         dbfilename;
  std::string         dbloglevel;

  std::string LOGGER;
  std::string DBFILENAME;
  std::string DELTAS;
  std::string COUNT_THRESHOLD;
  std::string DATABASE_LOGGER;

  size_t              cardinality; // cardinality == what is the n in n-gram?

private:
  std::vector<double> deltas;
  int                 count_threshold;

  Dispatcher<SmoothedNgramTriePredictor> dispatcher;
};

#endif // PRESAGE_SMOOTHEDNGRAMPREDICTOR
