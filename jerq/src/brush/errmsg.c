/*
 *
 *	error messages for Brush  --  brush/errmsg.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "myjerq.h"
#include <jerqio.h>
#include <font.h>

extern char *_doprnt();
Point xformpt();
char _msg_buf[100];

msg( s,spot )
	char *s;
	Point spot;
{
	Bitmap *bp;
	static Rectangle r = { { 0,0 }, { 0,0 } };
	Word which;

	r.corner.y = 8 + defont.height;
	r.corner.x = 16 + strwidth( &defont,s );
	spot.x = min( spot.x,Drect.corner.x-strwidth( &defont,s ) );

	if ( bp = balloc( r ) )
	   {
		rectf( bp,r,F_STORE );
		rectf( bp,inset( r,2 ),F_XOR );
		string( &defont,s,bp,Pt(8,4),F_STORE );
		screenswap( bp,r,raddp( r,spot ) );
		which = click();
		screenswap( bp,r,raddp( r,spot ) );
		bfree( bp );
	   }
	else {
		string( &defont,s,&display,spot,F_STORE );
		which = click();
	     }
	return which;
}

errmsg( s )
	char *s;
{
	return msg( s,mouse.xy );
}

errprintf( fmt, x1)
	char *fmt;
	unsigned x1;
{
	FILE fake;
	register FILE *f;

	f = &fake;
	f->cp = f->base = _msg_buf;
	f->flag = f->count = 0;
	_doprnt(f, fmt, &x1);
	putc('\0',f);
	errmsg( _msg_buf );
}

stringf( font,buf,bp,p,code,fmt,x1 )
	Font *font;
	char *buf,*fmt;
	Bitmap *bp;
	Point p;
	Word code;
	unsigned x1;
{
	FILE fake;
	register FILE *f;

	f = &fake;
	f->cp = f->base = buf;
	f->flag = f->count = 0;
	_doprnt(f, fmt, &x1);
	putc('\0',f);
	string( font,buf,bp,p,code );
}


Point
xformpt( p,sr,dr )		/* xform p from source to dest rect coords   */
	Point p;
	Rectangle sr,dr;
{
	p.x = dr.origin.x + muldiv( p.x-sr.origin.x,RWIDTH ( dr ),RWIDTH ( sr ) );
	p.y = dr.origin.y + muldiv( p.y-sr.origin.y,RHEIGHT( dr ),RHEIGHT( sr ) );
	return( p );
}

