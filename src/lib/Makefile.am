
##########
#  Presage, an extensible predictive text entry system
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

SUBDIRS = 			core predictors

include_HEADERS =		presage.h \
				presageException.h \
				presageCallback.h 

lib_LTLIBRARIES = 		libpresage.la
libpresage_la_SOURCES = 	presage.cpp \
				presage.h \
				presageException.h \
				presageCallback.h \
				libpresage.map
libpresage_la_LIBADD =		core/libcore.la \
				predictors/libpredictors.la \
				-ltinyxml2
libpresage_la_LDFLAGS =		-no-undefined \
				-version-info 2:1:1
if HAVE_LD_WITH_VERSION_SCRIPT
libpresage_la_LDFLAGS +=	-Wl,--version-script,$(srcdir)/libpresage.map
endif

EXTRA_DIST =	libpresage.map
