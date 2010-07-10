
###########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#
#  Copyright (C) 2010  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
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

if HAVE_DBUS_PYTHON

dist_bin_SCRIPTS = \
	presage_dbus_service

python_PYTHON = \
	presage_dbus_service.py

dbus_servicedir = $(datadir)/dbus-1/services
dbus_service_DATA = org.gnome.presage.service

EXTRA_DIST = org.gnome.presage.service.in

org.gnome.presage.service:	org.gnome.presage.service.in Makefile
	sed -e "s|{bindir}|$(bindir)|" $< > $@

CLEANFILES = org.gnome.presage.service

endif