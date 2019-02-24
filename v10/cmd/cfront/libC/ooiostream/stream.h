/*ident	"@(#)ctrans:incl/stream.h	1.1.5.1" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
      
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

*****************************************************************************/

#ifndef STREAMH
#define STREAMH

#include <iostream.h>
#include <iomanip.h>
#include <stdiostream.h>

#ifndef NULL
#define NULL	0
#endif

extern char*  oct(long, int =0);
extern char*  dec(long, int =0);
extern char*  hex(long, int =0);

extern char*  chr(int, int =0);	/* chr(0) is the empty string "" */
extern char*  str(const char*, int =0);
extern char*  form(const char* ...);
			/* printf format
                         * Things may go terribly wrong (maybe even core
                         * dumps, if form tries to create a string with
                         * more than "max_field_width" characters. */

/* WS used to be a special in streams. The WS manipulator
 * is implemented differently but may be extracted from an istream
 * with the same effect as the old form.
 */

extern istream& WS(istream&) ;
extern void eatwhite(istream&) ;

static const int input = (ios::in) ;
static const int output = (ios::out) ;
static const int append = (ios::app) ;
static const int atend = (ios::ate) ;
static const int _good = (ios::goodbit) ;
static const int _bad = (ios::badbit) ;
static const int _fail = (ios::failbit) ;
static const int _eof = (ios::eofbit) ;

typedef io_state state_value ;

#endif
