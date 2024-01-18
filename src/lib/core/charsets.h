
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


const unsigned char DEFAULT_WORD_CHARS[]={
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
	
    static_cast< unsigned char >( Agrave  ),
    static_cast< unsigned char >( Aacute  ),
    static_cast< unsigned char >( Acirc   ),
    static_cast< unsigned char >( Atilde  ),
    static_cast< unsigned char >( Auml    ),
    static_cast< unsigned char >( Aring   ),
    static_cast< unsigned char >( AElig   ),
    static_cast< unsigned char >( Ccedil  ),
    static_cast< unsigned char >( Egrave  ),
    static_cast< unsigned char >( Eacute  ),
    static_cast< unsigned char >( Ecirc   ),
    static_cast< unsigned char >( Euml    ),
    static_cast< unsigned char >( Igrave  ),
    static_cast< unsigned char >( Iacute  ),
    static_cast< unsigned char >( Icirc   ),
    static_cast< unsigned char >( Iuml    ),
    static_cast< unsigned char >( ETH     ),
    static_cast< unsigned char >( Ntilde  ),
    static_cast< unsigned char >( Ograve  ),
    static_cast< unsigned char >( Oacute  ),
    static_cast< unsigned char >( Ocirc   ),
    static_cast< unsigned char >( Otilde  ),
    static_cast< unsigned char >( Ouml    ),
    static_cast< unsigned char >( times   ),
    static_cast< unsigned char >( Oslash  ),
    static_cast< unsigned char >( Ugrave  ),
    static_cast< unsigned char >( Uacute  ),
    static_cast< unsigned char >( Ucirc   ),
    static_cast< unsigned char >( Uuml    ),
    static_cast< unsigned char >( Yacute  ),
    static_cast< unsigned char >( THORN   ),
    static_cast< unsigned char >( szlig   ),
    static_cast< unsigned char >( agrave  ),
    static_cast< unsigned char >( aacute  ),
    static_cast< unsigned char >( acirc   ),
    static_cast< unsigned char >( atilde  ),
    static_cast< unsigned char >( auml    ),
    static_cast< unsigned char >( aring   ),
    static_cast< unsigned char >( aelig   ),
    static_cast< unsigned char >( ccedil  ),
    static_cast< unsigned char >( egrave  ),
    static_cast< unsigned char >( eacute  ),
    static_cast< unsigned char >( ecirc   ),
    static_cast< unsigned char >( euml    ),
    static_cast< unsigned char >( igrave  ),
    static_cast< unsigned char >( iacute  ),
    static_cast< unsigned char >( icirc   ),
    static_cast< unsigned char >( iuml    ),
    static_cast< unsigned char >( eth     ),
    static_cast< unsigned char >( ntilde  ),
    static_cast< unsigned char >( ograve  ),
    static_cast< unsigned char >( oacute  ),
    static_cast< unsigned char >( ocirc   ),
    static_cast< unsigned char >( otilde  ),
    static_cast< unsigned char >( ouml    ),
    static_cast< unsigned char >( divide  ),
    static_cast< unsigned char >( oslash  ),
    static_cast< unsigned char >( ugrave  ),
    static_cast< unsigned char >( uacute  ),
    static_cast< unsigned char >( ucirc   ),
    static_cast< unsigned char >( uuml    ),
    static_cast< unsigned char >( yacute  ),
    static_cast< unsigned char >( thorn   ),
    static_cast< unsigned char >( yuml    ),

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
