
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

 *  Adjustments for HunspellPredictor: Copyright (C) 2018 rinigus https://github.com/rinigus

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


#include "hunspellPredictor.h"

#include <assert.h>


HunspellPredictor::HunspellPredictor (Configuration* config, ContextTracker* ht, const char* name)
    : Predictor(config,
	     ht,
	     name,
	     "HunspellPredictor, Hunspell predictor",
	     "HunspellPredictor, Hunspell based predictor"
	     ),
      dispatcher (this)
{
    LOGGER      = PREDICTORS + name + ".LOGGER";
    AFFIX       = PREDICTORS + name + ".AFFIX";
    DICTIONARY  = PREDICTORS + name + ".DICTIONARY";
    PROBABILITY = PREDICTORS + name + ".PROBABILITY";

    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & HunspellPredictor::set_logger);
    dispatcher.map (config->find (AFFIX), & HunspellPredictor::set_affix);
    dispatcher.map (config->find (DICTIONARY), & HunspellPredictor::set_dictionary);
    dispatcher.map (config->find (PROBABILITY), & HunspellPredictor::set_probability);
}

HunspellPredictor::~HunspellPredictor()
{
    // intentionally empty
}

void HunspellPredictor::set_affix (const std::string& value)
{
    affix_path = value;
    logger << INFO << "AFFIX: " << value << endl;
    load_speller();
}

void HunspellPredictor::set_dictionary (const std::string& value)
{
    dictionary_path = value;
    logger << INFO << "DICTIONARY: " << value << endl;
    load_speller();
}

void HunspellPredictor::set_probability (const std::string& value)
{
    probability = Utility::toDouble (value);
    logger << INFO << "PROBABILITY: " << value << endl;
}

void HunspellPredictor::load_speller()
{
    hunspell.reset();
    if (affix_path.empty() ||  dictionary_path.empty())
      return;
    hunspell.reset(new Hunspell(affix_path.c_str(), dictionary_path.c_str()));
}

Prediction HunspellPredictor::predict(const size_t max_partial_predictions_size, const char** filter) const
{
    Prediction result;

    std::string prefix = contextTracker->getPrefix();

    if (!hunspell)
      return result;

    unsigned int count = 0;
    if (hunspell->spell(prefix))
      {
        logger << DEBUG << prefix << " is correct, adding as suggestion" << endl;
        
        // correct spelling, let's add available suffixes
        result.addSuggestion(Suggestion(prefix,probability)); // add the word itself
        count += 1;
                
        std::vector<std::string> wlst = hunspell->suffix_suggest(prefix);
        double dprob = probability / (wlst.size() + 2);
        double cprob = probability - dprob;

        std::vector<std::string>::const_iterator it = wlst.cbegin();
        for ( ; count < max_partial_predictions_size && it != wlst.cend(); ++count, ++it, cprob -= dprob) {
          result.addSuggestion(Suggestion(*it, cprob));
          logger << DEBUG << "suffix suggestion: " << *it << endl;
        }
      }
    else
      {
        logger << DEBUG << prefix << " misspelled" << endl;

        // correct spelling, let's suggest correct words
        std::vector<std::string> wlst = hunspell->suggest(prefix);
        double dprob = probability / (wlst.size() + 1);
        double cprob = probability - dprob;
        
        std::vector<std::string>::const_iterator it = wlst.cbegin();
        for ( ; count < max_partial_predictions_size && it != wlst.cend(); ++count, ++it, cprob -= dprob) {
          result.addSuggestion(Suggestion(*it, cprob));
          logger << DEBUG << "speller suggestion: " << *it << endl;
        }
      }
    
    return result;
}

void HunspellPredictor::learn(const std::vector<std::string>& change)
{
}

void HunspellPredictor::update (const Observable* var)
{
    logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
    dispatcher.dispatch (var);
}
