
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

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


#ifndef PRESAGE_SMOOTHEDNGRAMPLUGIN
#define PRESAGE_SMOOTHEDNGRAMPLUGIN

#include "plugins/plugin.h"
#include "core/utility.h"    // strtolower
#include "core/logger.h"

#include <assert.h>

#if defined(HAVE_SQLITE3_H) 
# include <sqlite3.h>
#elif defined(HAVE_SQLITE_H)
# include <sqlite.h>
#else
# error "SQLite is required. Please install SQLite."
#endif

#include "core/dbconnector/sqliteDatabaseConnector.h"


/** Smoothed n-gram statistical plugin.
 *
 */
class SmoothedNgramPlugin : public Plugin, public Observer {
public:
    SmoothedNgramPlugin(Configuration*, ContextTracker*);
    ~SmoothedNgramPlugin();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);
    virtual void extract();
    virtual void train();

    virtual void update (const Observable* variable) { /* temporary */ };

private:
    static const char* LOGGER;
    static const char* DBFILENAME;
    static const char* DELTAS;
    static const char* LEARN;
    static const char* DATABASE_LOGGER;

    unsigned int count(const std::vector<std::string>& tokens, int offset, int ngram_size) const;
    void check_learn_consistency(const Ngram& name) const;

    void setDbfilename (const std::string& filename);
    void setDeltas (const std::string& deltas);
    void setDatabaseLoggerLevel (const std::string& value);
    void setWannaLearn (const std::string& deltas);

    DatabaseConnector*  db;
    std::string         dbfilename;
    std::string         dbloglevel;
    std::vector<double> deltas;
    bool                wanna_learn;

};

#endif // PRESAGE_SMOOTHEDNGRAMPLUGIN
