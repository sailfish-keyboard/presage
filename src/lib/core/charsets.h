
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


#ifndef PRESAGE_CHARSETS
#define PRESAGE_CHARSETS

// ISO 8859-1 standard ///////////////////////
#include "iso8859_1.h"


// FIXME: ensure values are correct
const char RIGHT_ARROW = 28;
const char LEFT_ARROW  = 29;
const char UP_ARROW    = 30;
const char DOWN_ARROW  = 31;
const char END         = 128;    // fix this!!!
const char HOME        = 128;    // <----------
const char BACKSPACE   = '\b';
const char DELETE      = 18;     // ?
const char PAGE_UP     = 128;
const char PAGE_DOWN   = 128;


const char DEFAULT_WORD_CHARS[]={
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
	
    static_cast< char >( Agrave  ),
    static_cast< char >( Aacute  ),
    static_cast< char >( Acirc   ),
    static_cast< char >( Atilde  ),
    static_cast< char >( Auml    ),
    static_cast< char >( Aring   ),
    static_cast< char >( AElig   ),
    static_cast< char >( Ccedil  ),
    static_cast< char >( Egrave  ),
    static_cast< char >( Eacute  ),
    static_cast< char >( Ecirc   ),
    static_cast< char >( Euml    ),
    static_cast< char >( Igrave  ),
    static_cast< char >( Iacute  ),
    static_cast< char >( Icirc   ),
    static_cast< char >( Iuml    ),
    static_cast< char >( ETH     ),
    static_cast< char >( Ntilde  ),
    static_cast< char >( Ograve  ),
    static_cast< char >( Oacute  ),
    static_cast< char >( Ocirc   ),
    static_cast< char >( Otilde  ),
    static_cast< char >( Ouml    ),
    static_cast< char >( times   ),
    static_cast< char >( Oslash  ),
    static_cast< char >( Ugrave  ),
    static_cast< char >( Uacute  ),
    static_cast< char >( Ucirc   ),
    static_cast< char >( Uuml    ),
    static_cast< char >( Yacute  ),
    static_cast< char >( THORN   ),
    static_cast< char >( szlig   ),
    static_cast< char >( agrave  ),
    static_cast< char >( aacute  ),
    static_cast< char >( acirc   ),
    static_cast< char >( atilde  ),
    static_cast< char >( auml    ),
    static_cast< char >( aring   ),
    static_cast< char >( aelig   ),
    static_cast< char >( ccedil  ),
    static_cast< char >( egrave  ),
    static_cast< char >( eacute  ),
    static_cast< char >( ecirc   ),
    static_cast< char >( euml    ),
    static_cast< char >( igrave  ),
    static_cast< char >( iacute  ),
    static_cast< char >( icirc   ),
    static_cast< char >( iuml    ),
    static_cast< char >( eth     ),
    static_cast< char >( ntilde  ),
    static_cast< char >( ograve  ),
    static_cast< char >( oacute  ),
    static_cast< char >( ocirc   ),
    static_cast< char >( otilde  ),
    static_cast< char >( ouml    ),
    static_cast< char >( divide  ),
    static_cast< char >( oslash  ),
    static_cast< char >( ugrave  ),
    static_cast< char >( uacute  ),
    static_cast< char >( ucirc   ),
    static_cast< char >( uuml    ),
    static_cast< char >( yacute  ),
    static_cast< char >( thorn   ),
    static_cast< char >( yuml    ),

    '\0' };

const char DEFAULT_SEPARATOR_CHARS[]={
    '!',
    '"',
    '#',
    '$',
    '%',
    '&',
    '\'',
    '(',
    ')',
    '*',
    '+',
    ',',
    '-',
    '.',
    '/',
    ':',
    ';',
    '<',
    '=',
    '>',
    '?',
    '@',
    '[',
    '\\',
    ']',
    '^',
    '_',
    '`',
    '{',
    '|',
    '}',
    '~',
    '\0' };

const char DEFAULT_BLANKSPACE_CHARS[]={
    ' ',  // space
    '\f', // form feed
    '\n', // newline
    '\r', // carriage return
    '\t', // horizontal tab
    '\v', // vertical tab
    '\0' };

const char DEFAULT_CONTROL_CHARS[]={
    RIGHT_ARROW,
    LEFT_ARROW,
    UP_ARROW,
    DOWN_ARROW,
    END,
    HOME,
    BACKSPACE,
    DELETE,
    PAGE_UP,
    PAGE_DOWN,
    '\0' };


#endif // PRESAGE_CHARSETS
