/**********************************************************************

    This file is part of ascii2unihy Armenian ASCII to Unicode converter
    Copyright (C) 2008-2014 Eduard Babayan <edo248[at]gmail.com>

    ascii2unihy is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ascii2unihy is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ascii2unihy.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/

//unicode.h
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <stdlib.h>

using namespace std;

#define UNICODE				0xFFFE
#define UNICODE_BIG_ENDIAN		0xFEFF
#define UTF8				0xEFBB

//define OUTPUT_ENCODING with one of the above numbers
#define OUTPUT_ENCODING			0xEFBB

//STRINGS
#define UTF_ERROR  "ERROR:error apperared during utf-8 decoding"
#define FILE_NOT_FOUND "ERROR:File \"%s\" not found or cannot be opened for reading."
#define FILE_CONVERTED "File successfully converted"
#define USAGE "Filename not specified\nUsage: unicode.exe input_file\n\n"
#define OUT_FILES "Output Files:"
#define AUTHOR "Thanks!\n\n"

#define TXT_OUT "output_unicode.txt"
#define HTML_OUT "output_unicode.html"
#define HTML_LINE_BREAK "<br />"

//MACROS
#define PRINT_ENCODING(code) ((code)==UTF8?"UTF-8":((code)==UNICODE?"Unicode":"Unicode Big Endian"))
#define PRINT_BYTES(b) "" << (b) << " Bytes"
