# this is <Makefile>
# ----------------------------------------------------------------------------
# $Id$
# Copyright (c) 2012 by Daniel Armbruster 
# ----
# This file is part of liboptimizexx.
# 
# liboptimizexx is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# liboptimizexx is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with liboptimizexx.  If not, see <http://www.gnu.org/licenses/>.
# ----
# Daniel Armbruster 01/03/2012
# 
# REVISIONS and CHANGES
# 01/03/2012	V0.1	Daniel Armbruster
# ----------------------------------------------------------------------------
CPPFLAGS=-I$(LOCINCLUDEDIR) 
LDFLAGS=-L$(LOCLIBDIR) 

CXXFLAGS=-fhonor-std -Wall $(FLAGS)
CXXFLAGS=-Wall -pedantic $(FLAGS)
CXXFLAGS=-Wall $(FLAGS)

all:

STANDARDTEST=parameterspacetest iteratortest gridsearchtest montecarlotest

clean:
	-find . -name \*.o | xargs --no-run-if-empty /bin/rm -v
	-/bin/rm -v $(STANDARDTEST)

# ----------------------------------------------------------------------------

$(addsuffix .o,$(STANDARDTEST)): %.o: %.cc
	@$(CXX) -c -o $@ $< $(CXXFLAGS) $(CPPFLAGS) $(FLAGS)

$(STANDARDTEST): %: %.o 	
	@echo -e "\n[ Compiling test program: $@ ]\n"	
	@$(CXX) -o $@ $< $(LDFLAGS) -loptimizexx

# ----- END OF Makefile -----
