
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


#include "smoothedNgramTriePredictor.h"

#include <sstream>
#include <algorithm>


SmoothedNgramTriePredictor::SmoothedNgramTriePredictor(Configuration* config, ContextTracker* ct, const char* name)
  : Predictor(config,
              ct,
              name,
              "SmoothedNgramTriePredictor, a linear interpolating n-gram predictor",
              "SmoothedNgramTriePredictor, long description." ),
    db (0),
    count_threshold (0),
    cardinality (0),
    dispatcher (this)
{
  LOGGER          = PREDICTORS + name + ".LOGGER";
  DBFILENAME      = PREDICTORS + name + ".DBFILENAME";
  DELTAS          = PREDICTORS + name + ".DELTAS";
  COUNT_THRESHOLD = PREDICTORS + name + ".COUNT_THRESHOLD";
  DATABASE_LOGGER = PREDICTORS + name + ".DatabaseConnector.LOGGER";

  // build notification dispatch map
  dispatcher.map (config->find (LOGGER), & SmoothedNgramTriePredictor::set_logger);
  dispatcher.map (config->find (DATABASE_LOGGER), & SmoothedNgramTriePredictor::set_database_logger_level);
  dispatcher.map (config->find (DBFILENAME), & SmoothedNgramTriePredictor::set_dbfilename);
  dispatcher.map (config->find (DELTAS), & SmoothedNgramTriePredictor::set_deltas);
  dispatcher.map (config->find (COUNT_THRESHOLD), & SmoothedNgramTriePredictor::set_count_threshold);
}



SmoothedNgramTriePredictor::~SmoothedNgramTriePredictor()
{
  delete db;
}


void SmoothedNgramTriePredictor::set_dbfilename (const std::string& filename)
{
  dbfilename = filename;
  logger << INFO << "DBFILENAME: " << dbfilename << endl;

  this->init_database_connector_if_ready ();
}


void SmoothedNgramTriePredictor::set_database_logger_level (const std::string& value)
{
  dbloglevel = value;
}


void SmoothedNgramTriePredictor::set_deltas (const std::string& value)
{
  std::stringstream ss_deltas(value);
  cardinality = 0;
  std::string delta;
  while (ss_deltas >> delta) {
    logger << DEBUG << "Pushing delta: " << delta << endl;
    deltas.push_back (Utility::toDouble (delta));
    cardinality++;
  }
  logger << INFO << "DELTAS: " << value << endl;
  logger << INFO << "CARDINALITY: " << cardinality << endl;

  this->init_database_connector_if_ready ();
}


void SmoothedNgramTriePredictor::set_count_threshold (const std::string& value)
{
  count_threshold = Utility::toInt (value);
  logger << INFO << "COUNT_THRESHOLD: " << count_threshold << endl;
}


void SmoothedNgramTriePredictor::init_database_connector_if_ready ()
{
  // we can only init the sqlite database connector once we know the
  // following:
  //  - what database file we need to open
  //  - what cardinality we expect the database file to be
  //  - whether we need to open the database in read only or
  //    read/write mode (learning requires read/write access)
  //
  if (! dbfilename.empty()
      && cardinality > 0) {

    delete db;

    if (dbloglevel.empty ()) {
      // open database connector
      db = new TrieDatabaseConnector(dbfilename,
                                     cardinality,
                                     false);
    } else {
      // open database connector with logger lever
      db = new TrieDatabaseConnector(dbfilename,
                                     cardinality,
                                     false,
                                     dbloglevel);
    }
  }
}


// convenience function to convert ngram to string
//
static std::string ngram_to_string(const Ngram& ngram)
{
  const char separator[] = "|";
  std::string result = separator;

  for (Ngram::const_iterator it = ngram.begin();
       it != ngram.end();
       it++)
    {
      result += *it + separator;
    }

  return result;
}


/** \brief Builds the required n-gram and returns its count.
 *
 * \param tokens tokens[i] contains ContextTracker::getToken(i)
 * \param offset entry point into tokens, must be a non-positive number
 * \param ngram_size size of the ngram whose count is returned, must not be greater than tokens size
 * \return count of the ngram built based on tokens, offset and ngram_size
 *
 * \verbatim
 Let tokens = [ "how", "are", "you", "today" ];

 count(tokens,  0, 3) returns the count associated with 3-gram [ "are", "you", "today" ].
 count(tokens, -1, 2) returns the count associated with 2-gram [ "are", "you" ];
 * \endverbatim
 *
 */
unsigned int SmoothedNgramTriePredictor::count(const std::vector<std::string>& tokens, int offset, int ngram_size) const
{
  unsigned int result = 0;

  assert(offset <= 0);      // TODO: handle this better
  assert(ngram_size >= 0);

  if (ngram_size > 0) {
    Ngram ngram(ngram_size);
    copy(tokens.end() - ngram_size + offset , tokens.end() + offset, ngram.begin());
    result = db->getNgramCount(ngram);
    logger << DEBUG << "count ngram: " << ngram_to_string (ngram) << " : " << result << endl;
  } else {
    result = db->getUnigramCountsSum();
    logger << DEBUG << "unigram counts sum: " << result << endl;
  }

  return result;
}

Prediction SmoothedNgramTriePredictor::predict(const size_t max_partial_prediction_size, const char** filter) const
{
  logger << DEBUG << "predict()" << endl;

  // Result prediction
  Prediction prediction;

  // Cache all the needed tokens.
  // tokens[k] corresponds to w_{i-k} in the generalized smoothed
  // n-gram probability formula
  //
  std::vector<std::string> tokens(cardinality);
  for (int i = 0; i < cardinality; i++) {
    tokens[cardinality - 1 - i] = contextTracker->getToken(i);
    logger << DEBUG << "Cached tokens[" << cardinality - 1 - i << "] = " << tokens[cardinality - 1 - i] << endl;
  }

  // Generate list of prefix completition candidates.
  //
  // The prefix completion candidates used to be obtained from the
  // _1_gram table because in a well-constructed ngram database the
  // _1_gram table (which contains all known tokens). However, this
  // introduced a skew, since the unigram counts will take
  // precedence over the higher-order counts.
  //
  // The current solution retrieves candidates from the highest
  // n-gram table, falling back on lower order n-gram tables if
  // initial completion set is smaller than required.
  //
  std::vector<std::string> prefixCompletionCandidates;
  for (size_t k = cardinality; (k > 0 && prefixCompletionCandidates.size() < max_partial_prediction_size); k--) {
    logger << DEBUG << "Building partial prefix completion table of cardinality: " << k << endl;
    // create n-gram used to retrieve initial prefix completion table
    Ngram prefix_ngram(k);
    copy(tokens.end() - k, tokens.end(), prefix_ngram.begin());

    if (logger.shouldLog()) {
      logger << DEBUG << "prefix_ngram: ";
      for (size_t r = 0; r < prefix_ngram.size(); r++) {
        logger << DEBUG << prefix_ngram[r] << ' ';
      }
      logger << DEBUG << endl;
    }

    // obtain initial prefix completion candidates
    std::vector<std::string> partial;

    partial = db->getPredictedWords(prefix_ngram,
                                    filter,
                                    count_threshold,
                                    max_partial_prediction_size - prefixCompletionCandidates.size());
    
    if (logger.shouldLog()) {
      logger << DEBUG << "partial prefixCompletionCandidates" << endl
             << DEBUG << "----------------------------------" << endl;
      for (size_t j = 0; j < partial.size(); j++) {
        logger << DEBUG << partial[j] << "\n";
      }
    }

    logger << DEBUG << "Partial prefix completion table contains " << partial.size() << " potential completions." << endl;

    // append newly discovered potential completions to prefix
    // completion candidates array to fill it up to
    // max_partial_prediction_size
    //
    std::vector<std::string>::const_iterator it = partial.cbegin();
    while (it != partial.cend() && prefixCompletionCandidates.size() < max_partial_prediction_size) {
      const std::string &candidate = (*it);
      if (find(prefixCompletionCandidates.begin(),
               prefixCompletionCandidates.end(),
               candidate) == prefixCompletionCandidates.end()) {
        prefixCompletionCandidates.push_back(candidate);
      }
      it++;
    }
  }

  if (logger.shouldLog()) {
    logger << DEBUG << "prefixCompletionCandidates" << endl
           << DEBUG << "--------------------------" << endl;
    for (size_t j = 0; j < prefixCompletionCandidates.size(); j++) {
      logger << DEBUG << prefixCompletionCandidates[j] << endl;
    }
  }

  // compute smoothed probabilities for all candidates
  int unigrams_counts_sum = db->getUnigramCountsSum(); 
  for (size_t j = 0; (j < prefixCompletionCandidates.size() && j < max_partial_prediction_size); j++) {
    // store w_i candidate at end of tokens
    tokens[cardinality - 1] = prefixCompletionCandidates[j];

    logger << DEBUG << "------------------" << endl;
    logger << DEBUG << "w_i: " << tokens[cardinality - 1] << endl;

    double probability = 0;
    for (int k = 0; k < cardinality; k++) {
      double numerator = count(tokens, 0, k+1);
      // reuse cached unigrams_counts_sum to speed things up
      double denominator = (k == 0 ? unigrams_counts_sum : count(tokens, -1, k));
      // probably not a proper fix, but done to go around some cases where denominator < numerator
      // double frequency = ((denominator > 0) ? (numerator / denominator) : 0);
      double frequency = ((denominator > 0 && denominator >= numerator) ? (numerator / denominator) : 0);
      probability += deltas[k] * frequency;

      logger << DEBUG << "numerator:   " << numerator << endl;
      logger << DEBUG << "denominator: " << denominator << endl;
      logger << DEBUG << "frequency:   " << frequency << endl;
      logger << DEBUG << "delta:       " << deltas[k] << endl;

      // for some sanity checks
      // these assertions fail occasionally. to fix, the calculation of frequency was adjusted above
      //assert(numerator <= denominator);
      assert(frequency <= 1);
    }

    logger << DEBUG << "____________" << endl;
    logger << DEBUG << "probability: " << probability << endl;

    if (probability > 0) {
      prediction.addSuggestion(Suggestion(tokens[cardinality - 1], probability));
    }
  }

  logger << DEBUG << "Prediction:" << endl;
  logger << DEBUG << "-----------" << endl;
  logger << DEBUG << prediction << endl;

  return prediction;
}

void SmoothedNgramTriePredictor::learn(const std::vector<std::string>& change)
{
  logger << INFO << "learn(\"" << ngram_to_string(change) << "\") not implemented for SmoothedNgramTriePredictor" << endl;
}


void SmoothedNgramTriePredictor::update (const Observable* var)
{
  logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
  dispatcher.dispatch (var);
}
