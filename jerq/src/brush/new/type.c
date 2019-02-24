/*
 *
 *	typed string input for Brush   --   brush/type.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */
#include "myjerq.h"
#include <font.h>
#define LEN		60

char *
type( prompt,seed )		/* pop up typed input from keyboard	*/
	char *prompt,*seed;
{
	static char buff[LEN];
	register char *s,*begin,*first;
	Bitmap *bp;
	Point spot;
	static Rectangle r = {{0,0},{0,0}};

/*	stuff the prompt & seed into buff	*/

	strcpy( buff,prompt );
	begin = buff + strlen( prompt );
	strcpy( begin,seed );
	first = s = begin + strlen( seed );

/*	figure out size rect needed		*/

	r.corner.y = 8 + defont.height;
	r.corner.x = 16 + LEN * strwidth( &defont," " );

/*	alloc & initialize scratch bitmap	*/

	bp = balloc( r );
	rectf( bp,r,F_STORE );
	rectf( bp,inset( r,2 ),F_XOR );

/*	adjust mouse pos to avoid running off screen edge	*/

	spot = mouse.xy;
	spot.x = min( spot.x,Drect.corner.x-RWIDTH(r) );

/*	save screen area, draw prompt & seed	*/

	screenswap( bp,r,raddp( r,spot ) );
	type_msg( buff,raddp( inset( r,2 ),spot ) );

	for ( ; s<buff+LEN-2; )
	    {
		wait( KBD );
		switch( *s++ = kbdchar() )
		      {
			case '\r':	--s;
					goto out;
			case '\b':	s-=2;
					if ( s<begin ) s = begin;
					break;
			case '@':	s=begin;
		      }
		*s = 0;
		type_msg( buff,raddp( inset( r,2 ),spot ) );
	    }
out:	*s = 0;

/*	restore screen & free bitmap	*/

	screenswap( bp,r,raddp( r,spot ) );
	bfree( bp );

	return s!=first ? begin : (char *)0;
}

type_msg( s,r )
	char *s;
	Rectangle r;
{
	rectf( &display, r, F_CLR );
	string( &defont, s, &display, add( r.origin,Pt(8,2) ), F_XOR );
}

