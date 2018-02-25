
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


#ifndef PRESAGE_HUNSPELLPREDICTOR
#define PRESAGE_HUNSPELLPREDICTOR

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef HAVE_HUNSPELL_HUNSPELL_HXX
#error "Requires Hunspell library"
#endif

#include "predictor.h"
#include "../core/dispatcher.h"

#include <fstream>
#include <hunspell/hunspell.hxx>
#include <memory>


/** Hunspell based predictive predictor.
 *
 * Generates a prediction by running them through speller.
 *
 */
class HunspellPredictor : public Predictor, public Observer {
public:
    HunspellPredictor (Configuration*, ContextTracker*, const char*);
    ~HunspellPredictor();

    virtual Prediction predict (const size_t size, const char** filter) const;

    virtual void learn (const std::vector<std::string>& change);

    virtual void update (const Observable* variable);

    void set_affix (const std::string& value);
    void set_dictionary (const std::string& value);
    void set_probability (const std::string& value);

private:
    void load_speller();
    
private:
    std::string LOGGER;
    std::string AFFIX;
    std::string DICTIONARY;
    std::string PROBABILITY;

    std::string affix_path;
    std::string dictionary_path;
    double probability;

    Dispatcher<HunspellPredictor> dispatcher;
    std::unique_ptr<Hunspell> hunspell;
};

#endif // PRESAGE_HUNSPELLPREDICTOR
