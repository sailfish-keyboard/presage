
#############
#  Soothsayer, an extensible predictive text entry system
#  ------------------------------------------------------
#
#  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

## Process this file with automake to produce Makefile.in

SUBDIRS =	tokenizer dbconnector

EXTRA_DIST =	profileTest.xml

if HAVE_CPPUNIT

TESTS =		coreTestRunner

check_PROGRAMS = $(TESTS)

coreTestRunner_SOURCES =	coreTestRunner.cpp                          \
				suggestionTest.h suggestionTest.cpp         \
				predictionTest.h predictionTest.cpp         \
				tokenizer/testStringSuite.h                 \
				tokenizer/testStringSuite.cpp               \
				contextTrackerTest.h contextTrackerTest.cpp \
				selectorTest.h selectorTest.cpp             \
				profileTest.h profileTest.cpp               \
				profileManagerTest.h profileManagerTest.cpp \
				meritocracyCombinerTest.h meritocracyCombinerTest.cpp \
				loggerTest.h loggerTest.cpp                 \
				variableTest.h variableTest.cpp             \
				configurationTest.h configurationTest.cpp   

coreTestRunner_CPPFLAGS =	-I$(top_srcdir)/src/lib
coreTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
coreTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
coreTestRunner_LIBS =		-ldl
coreTestRunner_LDADD =		$(top_builddir)/src/lib/libsoothsayer.la


# Temporarily disabled
#pluginManagerDriver_SOURCES =	pluginManagerDriver.cpp
##pluginManagerDriver_LDFLAGS =	-ldl



DEPENDENCIES =	libsoothsayer.la

endif