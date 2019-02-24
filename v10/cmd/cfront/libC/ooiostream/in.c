/*ident	"@(#)ctrans:lib/stream/in.c	1.1.1.1" */
/**************************************************************************
			Copyright (c) 1984 AT&T
	  		  All Rights Reserved  	

	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.

in.c:

*****************************************************************************/

#include <ctype.h>
#include <iostream.h>

#define ISTREAM istream

void ISTREAM::eatwhite ()
{
	register streambuf *nbp = bp;
	register int c = nbp->sgetc();
	while (isspace(c)) c = nbp->snextc();
	if (c == EOF) setstate(eofbit);
}

void ISTREAM::xget(char* c) 
{
	register streambuf* sbp = bp ;
	if ( !ipfx(1) ) return ;
	x_gcount = 0 ; 
	register int newc = sbp->sbumpc() ;
	if ( newc == EOF ) {
		setstate(failbit|eofbit) ;
		return ;
		}
	x_gcount = 1 ;
	*c = newc ;
	}

istream& ISTREAM::operator>>(register char* s)
{
	/* get string */

	register int w = width(0) ;
	if ( flags()&skipws ) {
		// We don't know a maximum number of required
		// characters 
		if ( !ipfx(0) ) return *this ;
		}
	else if ( !ipfx(w) ) {
		return *this ;
		}

	register streambuf *nbp = bp;
	register int c = nbp->sgetc();

	if (c == EOF) setstate(failbit|eofbit) ;

	if ( w > 0 ) {
		while (!isspace(c) && c != EOF && --w > 0 ) {
			*s++ = c;
			c = nbp->snextc();
		}
	} else {
		while (!isspace(c) && c != EOF ) {
			*s++ = c;
			c = nbp->snextc();
		}
	}
	
	*s = '\0';

	if (c == EOF) setstate(eofbit) ;

	return *this;
}

istream& ISTREAM::operator>>(unsigned char* s)
{
	return *this >> (char*)s ;
}

istream& ISTREAM::putback(register char c)
{
	if ( !good() ) return *this ;
	if ( bp->sputbackc(c) == EOF ) setstate(badbit) ;
	return *this;
}
