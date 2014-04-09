#########################################################################
#    This file is part of ascii2unihy Armenian ASCII to Unicode converter
#    Copyright (C) 2008-2014 Eduard Babayan <edo248[at]gmail.com>
#
#    ascii2unihy is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    ascii2unihy is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with ascii2unihy.  If not, see <http://www.gnu.org/licenses/>.
#
#########################################################################

rm=/bin/rm -f
CC= g++
DEFS=  
PROGNAME= ascii2unihy
INCLUDES=  -I.
LIBS=


DEFINES= $(INCLUDES) $(DEFS) -DSYS_UNIX=1
CFLAGS= -g $(DEFINES)

SRCS = ascii2unihy.cpp ascii2unihy.h

OBJS = ascii2unihy.o

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(SRCS) $(LIBS)
	chmod +x $(PROGNAME)

clean:
	$(rm) $(OBJS) $(PROGNAME) core *~
