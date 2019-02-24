/*
 *
 *	circles & discs for Brush  --  brush/circles.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 *
 */

#include "brush.h"

void disct16(),myellipse();
Point xydist(),alignment();


Image
discs( oldpic,filled )		/* draw textured discs */
	Image oldpic;
	Word filled;
{
	Point rad;
	Point org,p;
	Rectangle r;
	Bitmap *tmp;
	char scratch[20];
	Help old_set;
	int foo;

	old_set = helpswitch( circ_set );	
	cursswitch( &sweepcircle );

	while( !bttn123() ) wait( MOUSE );

	if ( bttn12() )
	   {
		helpswitch( old_set );	
		cursswitch( brush );
		return oldpic;
	   }

	org = p = mouse.xy;
	rad.x = rad.y = 0;

	ellipse( &display,org,rad.x,rad.y,F_XOR );
	stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				"%d %d",rad.x,rad.y );

	while ( bttn3() )
		if ( !eqpt( p,mouse.xy ) )
		   {
			ellipse( &display,org,rad.x,rad.y,F_XOR );
			stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				 "%d %d",rad.x,rad.y );
			rad = xydist( p=mouse.xy,org );
			stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				 "%d %d",rad.x,rad.y );
			ellipse( &display,org,rad.x,rad.y,F_XOR );
		   }

	ellipse( &display,org,rad.x,rad.y,F_XOR );
	stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				"%d %d",rad.x,rad.y );
	helpswitch( old_set );	
	cursswitch( &hourglass );

	/* punt if ellipse too small	*/

	if ( rad.x < 4 || rad.y < 4 )
	   {
		errmsg( "too small" );
		return oldpic;
	   }

	/* add brush width if circle */


	if ( !filled )
	   {
		r.origin = sub( org,add( rad,Pt(8,8) ) );
		r.corner = add( org,add( rad,Pt(9,9) ) );

		if ( r.origin.x<0 )
		   {
			foo = 16*( (r.origin.x-16)/16 );
			r.origin.x -= foo;
			r.corner.x -= foo;
			org.x -= foo;
		   }

		if ( r.origin.y<0 )
		   {
			foo = 16*( (r.origin.y-16)/16 );
			r.origin.y -= foo;
			r.corner.y -= foo;
			org.y -= foo;
		   }

		if ( !( tmp = balloc( r ) ) ) return oldpic;
		rectf( tmp,tmp->rect,F_CLR );
		myellipse( tmp,org,rad,F_STORE );
	   }
	
	else {
		r.origin = sub( org,rad );
		r.corner = add( org,add( rad,Pt(1,1) ) );

		if ( r.origin.x<0 )
		   {
			foo = 16*( (r.origin.x-16)/16 );
			r.origin.x -= foo;
			r.corner.x -= foo;
			org.x -= foo;
		   }

		if ( r.origin.y<0 )
		   {
			foo = 16*( (r.origin.y-16)/16 );
			r.origin.y -= foo;
			r.corner.y -= foo;
			org.y -= foo;
		   }
		if ( !(tmp = balloc( r ) ) )
		   {
			errmsg( "no mem" );
			return oldpic;
		   }
		rectf( tmp,tmp->rect,F_CLR );
		eldisc( tmp,org,rad.x,rad.y,F_STORE );
		texture16( tmp,tmp->rect,shade,F_OR );
		eldisc( tmp,org,rad.x,rad.y,F_XOR );
		texture16( tmp,tmp->rect,shade,F_XOR );
	      }

	if ( drawmode != F_XOR ) point( tmp,org,F_XOR );
	Ibfree( oldpic.bp );
	return make_image( tmp,tmp->rect,r.origin,alignment(r.origin) );
}

Point
xydist( p,q )
	Point p,q;
{
	p.x = abs( p.x-q.x );
	p.y = abs( p.y-q.y );
	return p;
}
void
disct16( bp,p,rad,tp,code )
	Bitmap *bp;
	Point p;
	Word rad;
	Texture16 *tp;
	Word code;
{
	Texture local;
	register short *dest = (short *)&local;
	register short *ssrc  = (short *)tp;
	register int    i;

	i = 16;
	do {
		*dest = *ssrc;
		*(dest + 1) = *ssrc;
		*((int *)(dest +32)) = *((int *)dest);
		ssrc++;
		dest += 2;
	} while ( --i > 0 );
	discture( bp,p,rad,&local,code );
}

extern void doit();

void
myellipse( bp,p,rad,f )
	Bitmap *bp;
	Point p,rad;
	Code f;
{
	ellip1( bp,p,rad.x,rad.y,doit,Pt(0,rad.y),Pt(0,rad.y),f );
}

