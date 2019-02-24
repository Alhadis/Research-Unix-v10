/*ident	"@(#)ctrans:lib/stream/flt.c	1.1.4.1" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
        
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

flt.c:

*****************************************************************************/

#include <iostream.h>
#include <ctype.h>
#include <stdio.h>
#include <libc.h>
#include <string.h>

#define OSTREAM ostream
#define ISTREAM istream

// This file contains all the functions having to do with i/o of
// floats and doubles.  It drags in lots of stuff from stdio, which
// is why I made it separate.

istream& ISTREAM::operator>>(double& d)
/*
	{+|-} d* {.} d* { e|E {+|-} d+ } 
	except that
		- a dot must be pre- or succeeded by at least one digit
		- an exponent must be preceded by at least one digit
*/
{
	register c = 0;
	register anydigits = 0 ;
	char buf[256];
	register char* p = buf;
	register streambuf* nbp = bp;
	
	if (!ipfx() ) return *this ;

	/* get the sign */
	switch (c = nbp->sgetc()) {
	case EOF:
		setstate(eofbit|failbit) ;
		return *this;
	case '-':
	case '+':
		*p++ = c;
		c = bp->snextc();
	}

	/* get integral part */
	while (isdigit(c)) {
		*p++ = c;
		c = bp->snextc();
		anydigits = 1 ;
	}

	/* get fraction */
	if (c == '.') {
		do {
			*p++ = c;
			c = bp->snextc();
			anydigits = 1 ;
		} while (isdigit(c));
	}

	/* get exponent */
	if ( anydigits && (c == 'e' || c == 'E')) {
		*p++ = c;
		switch (c = nbp->snextc()) {
		case EOF:
			setstate(eofbit|failbit) ;
			return *this;
		case '-':
		case '+':
			*p++ = c;
			c = bp->snextc();
		}
		while (isdigit(c)) {
			*p++ = c;
			c = bp->snextc();
		}
	}

	*p = 0;
	d = atof(buf);

	if (c == EOF) setstate(eofbit) ;
	if (!anydigits) setstate(badbit) ;
	return *this;
}


istream& ISTREAM::operator>>(float& f)
{
	double d;


	if (!ipfx() ) return *this ;

	*this >> d ;
	if ( good() ) f = d;

	return *this;
}

ostream& OSTREAM::operator<<(float f)  { return *this << (double)f ; }

static const int fltbits =
		ios::scientific|ios::fixed|ios::uppercase ;

ostream& OSTREAM::operator<<(double d)
{
	char buf[32];
	int w = width(0) ;
	int p = precision() ;
	int explicitp = ( p > 0 ) || (flags()&ios::fixed) ;
	if (!opfx()) return *this;
	long bits = flags() & fltbits ;
	char fmt[20] ;
	strcpy(fmt,"%") ;
	if ( flags() & ios::showpoint ) {
		strcat(fmt,"#") ;
		}
	if ( flags() & ios::showpos ) {
		strcat(fmt,"+") ;
		}
	if ( explicitp ) {
		strcat(fmt,".*") ;
		}

	long mode = flags()
		    & (ios::scientific|ios::fixed|ios::uppercase);
	
	switch(mode) {
	case ios::scientific :		strcat(fmt,"e") ; break ;
	case ios::scientific|ios::uppercase :
						strcat(fmt,"E") ; break;
	case ios::fixed :			strcat(fmt,"f") ; break ;
	case ios::fixed|ios::uppercase:	strcat(fmt,"F") ; break ;
	case ios::uppercase :		strcat(fmt,"G") ; break ;
	default :				strcat(fmt,"g") ; break ;
	}

	if ( explicitp ) {
		sprintf(buf,fmt,p,d) ;
		}
	else {
		sprintf(buf,fmt,d) ;
		}

	int padf = w-strlen(buf) ;
	if ( padf < 0 ) padf = 0 ;
	if ( padf && !(flags()&ios::left) ) {
		while ( padf-- > 0 ) put(fill()) ;
		}
	*this << buf ;
	while ( padf-- > 0 ) put(fill()) ;
	return *this ;
	}
