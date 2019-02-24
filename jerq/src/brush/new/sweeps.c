/*
 *
 *	Sweepers for Brush  --  brush/sweeps.c
 *
 *	includes:	sweeprect() & stretchrect()
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Rectangle
rcanon( p1,p2,size )	/* return canonical representation of rect(p1,p2)	*/
	Point p1,p2,size;
{
	Rectangle r;

	p2.x = p1.x + ( ( p2.x-p1.x )/size.x )*size.x;
	p2.y = p1.y + ( ( p2.y-p1.y )/size.y )*size.y;
	r.origin.x = min( p1.x,p2.x );
	r.origin.y = min( p1.y,p2.y );
	r.corner.x = max( p1.x,p2.x );
	r.corner.y = max( p1.y,p2.y );
	return r;
}

Rectangle
sweeprect( size )	/* like getrect(), but only integral magnifications */
	Point size;	/*  of size to be magnified			    */
{
	Rectangle r;
	Point org,last;
	char scratch[20];
	Texture16 *tp;
	Help old_set;

	wait( MOUSE );
	old_set = helpswitch( sweep_set );
	tp = (Texture16 *)cursswitch( &sweepcursor );
	while( !bttn123() );

	if ( bttn12() )
	   {
		r.origin = r.corner = mouse.xy;
		waitbuttons( UP );
		cursswitch( tp );
		helpswitch( old_set );
		return r;
	   };

	last = org = mouse.xy;
	r = rcanon( org,last );

	drawtrap( r,Pt(0,0),F_XOR );
	stringf( &defont,scratch,&bigscreen,add( RCOR,Pt(10,10) ),F_XOR,
		  "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );

	for ( ; bttn3(); nap(2) )
		if ( !eqpt( last,mouse.xy ) )
		   {
			drawtrap( r,Pt(0,0),F_XOR );
			stringf( &defont,scratch,
				 &bigscreen,add( RCOR,Pt(10,10) ),F_XOR,
				 "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );
			last = mouse.xy;
			r = rcanon( org,last,size );
			stringf( &defont,scratch,
				 &bigscreen,add( RCOR,Pt(10,10) ),F_XOR,
				 "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );
			drawtrap( r,Pt(0,0),F_XOR );
		   }

	stringf( &defont,scratch,&bigscreen,add( RCOR,Pt(10,10) ),F_XOR,
		  "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );
	drawtrap( r,Pt(0,0),F_XOR );

	wait( MOUSE );
	cursswitch( tp );
	helpswitch( old_set );
	return r;
}

Point
stretchrect( r )
	Rectangle r;
{
	Point p,off;
	Texture16 *tp;
	char scratch[15];

	Help old_set;

	old_set = helpswitch( sweep_set );
	tp = (Texture16 *)cursswitch( &sweepcursor );

	cursset( r.origin );
	off.x = off.y = 0;

	drawtrap( r,off,F_XOR );
	stringf( &defont,scratch,&display,sub( add(RORG,off),Pt(10,10) ),F_XOR,
		  "%d %d",off.x,off.y );

	while( !bttn123() );

	while( bttn3() )
		if ( !eqpt( off,p=sub( tweak( mouse.xy,r.origin ),r.origin ) ) )
		   {
			drawtrap( r,off,F_XOR );
			stringf( &defont,scratch,
				 &display,sub( add(RORG,off),Pt(10,10) ),F_XOR,
				 "%d %d",off.x,off.y );
			off = p;
			stringf( &defont,scratch,
				 &display,sub( add(RORG,off),Pt(10,10) ),F_XOR,
				 "%d %d",off.x,off.y );
			drawtrap( r,off,F_XOR );
		   }
	drawtrap( r,off,F_XOR );
	stringf( &defont,scratch,&display,sub( add(RORG,off),Pt(10,10) ),F_XOR,
		  "%d %d",off.x,off.y );

	cursswitch( tp );
	helpswitch( old_set );
	return off;
}

#define R(p,size)	raddp( Rect( -size/2-1,-size/2-1,size/2,size/2 ),p )

Rectangle
snarfbox( size )
	int size;
{
	Point p;
	Word i,j;
	Help old_set;
	Rectangle r;

	old_set = helpswitch( snarf_set );
	cursinhibit();

	p = mouse.xy;
	drawtrap( R( p,size ),Pt(0,0),F_XOR );

	for ( ; !bttn123(); nap(2) )
		if ( !eqpt( p,mouse.xy ) )
		   {
			drawtrap( R( p,size ),Pt(0,0),F_XOR );
			p = mouse.xy;
			drawtrap( R( p,size ),Pt(0,0),F_XOR );
		   }

	if ( bttn3() )
		r = raddp( Rect( -size/2,-size/2,size/2,size/2 ),p );
	else	r = makerect( 0,0,0,0 );

	drawtrap( R( p,size ),Pt(0,0),F_XOR );
	cursallow();

	helpswitch( old_set );
	return r;
}

#ifdef OLDRECT
Rectangle
newrect( bp,size )	/* like getrect(), but only integral magnifications */
	Bitmap *bp;
	Point size;	/*  of size to be magnified			    */
{
	Rectangle r;
	Point org,last;
	char scratch[20];
	Texture16 *tp;
	Help old_set;

	wait( MOUSE );
	old_set = helpswitch( sweep_set );
	tp = (Texture16 *)cursswitch( &sweepcursor );
	while( !bttn123() );

	if ( bttn12() )
	   {
		r.origin = r.corner = mouse.xy;
		waitbuttons( UP );
		cursswitch( tp );
		return r;
	   };

	last = org = mouse.xy;
	r = rcanon( org,last );
	drawtrap( r,Pt(0,0),F_XOR );
	stringf( &defont,scratch,bp,add( RCOR,Pt(10,10) ),F_XOR,
		  "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );

	for ( ; bttn3(); nap(2) )
		if ( !eqpt( last,mouse.xy ) )
		   {
			drawtrap( r,Pt(0,0),F_XOR );
			stringf( &defont,scratch,bp,add( RCOR,Pt(10,10) ),F_XOR,
				 "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );
			last = mouse.xy;
			r = rcanon( org,last,size );
			stringf( &defont,scratch,bp,add( RCOR,Pt(10,10) ),F_XOR,
				 "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );
			drawtrap( r,Pt(0,0),F_XOR );
		   }

	stringf( &defont,scratch,bp,add( RCOR,Pt(10,10) ),F_XOR,
		  "%d %d",RWIDTH(r)/size.x,RHEIGHT(r)/size.y );
	drawtrap( r,Pt(0,0),F_XOR );
	wait( MOUSE );
	cursswitch( tp );
	helpswitch( old_set );
	return r;
}

#endif
