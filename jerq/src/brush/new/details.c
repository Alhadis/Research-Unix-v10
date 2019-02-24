/*
 *
 *	detailed image edit for Brush  --  brush/details.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 *
 */

#include "brush.h"

#define M		8	/* mag factor	*/
#define WINSIZE		40
#define R(p,size)	raddp( Rect( -size/2-1,-size/2-1,size/2,size/2 ),p )

Rectangle snarfbox(),findroom();

Texture16 box8      = { 0x0000,0x0000,0x0000,0x0000,
			0x0FF0,0x0FF0,0x0FF0,0x0FF0,
			0x0FF0,0x0FF0,0x0FF0,0x0FF0,
			0x0000,0x0000,0x0000,0x0000,	};
Point sticky();

details()
{
	Rectangle r,sr,er;
	Point p,lastm;
	int hit,lasthit;
	Bitmap *bp;
	Help old_set;
	Texture16 *oldbrush;

	sr = snarfbox( WINSIZE );
	waitbuttons( UP );
	if ( nullrect( sr ) ) return;

	er = findroom( sr,Drect );
	if ( nullrect( er ) ) return;
	r = inset( er,-2 );

	if ( !( bp=balloc( r ) ) )
	   {
		errmsg( "no mem" );
		return;
	   }

	drawtrap( Rpt( add( sr.origin,Pt(-1,-1) ),sr.corner ),
		  Pt(0,0),F_XOR );

	bitblt( &display,r,bp,r.origin,F_STORE );	/* stash screen area	*/
	rectf( &display,r,F_STORE );
	rectf( &display,er,F_XOR );

/*	copy image into work area	*/

	cursinhibit();
	old_set = helpswitch( pos_set );

	for ( p.x=0; p.x<WINSIZE; p.x++ )
		for ( p.y=0; p.y<WINSIZE; p.y++ )
			if ( getpoint( &display,add( p,sr.origin ) ) )
				setpoint( &display,er,p,F_STORE );

	cursallow();
	oldbrush = cursswitch( &box8 );

/*	edit the sucker 'til button 3 quits	*/

	for ( ; !button3(); wait(MOUSE) )
		if ( ( hit=mouse.buttons&6 ) && ptinrect( mouse.xy,er ) )
			if ( !eqpt( lastm,p=sticky( er,mouse.xy ) )
			     || lasthit!=hit )
			   {
				lastm = p;
				lasthit = hit;
				setpoint( &display,er,p,(hit&4)?F_STORE:F_CLR );
				point( &display,
					add( sr.origin,p ),(hit&4)?F_STORE:F_CLR );
			   }

	bitblt( bp,r,&display,r.origin,F_STORE );	/* restore bkg	*/
	drawtrap( Rpt( add( sr.origin,Pt(-1,-1) ),sr.corner ),
		  Pt(0,0),F_XOR );


	cursswitch( oldbrush );
	helpswitch( old_set );
	bfree( bp );
}

#define HEIGHT	(M*WINSIZE)

Rectangle bigrect = { { 0,0 },{ HEIGHT,HEIGHT } };

Rectangle
findroom( sr,bound )
	Rectangle sr,bound;
{
	Point p;
	Rectangle r;

	int  vert,horz,midflag;

	midflag = FALSE;

	if ( bound.corner.x > sr.corner.x+2+M*WINSIZE )		/* RIGHT	*/
	   {
		r.origin.x = sr.corner.x+2;
		r.corner.x = r.origin.x + HEIGHT;
	   }
	else	if ( sr.origin.x > bound.origin.x+2+M*WINSIZE )	/* LEFT	*/
		   {
			r.corner.x = sr.origin.x-2;
			r.origin.x = r.corner.x - HEIGHT;
		   }
		else {						/* MIDDLE	*/
			midflag = TRUE;
			r.origin.x = bound.origin.x;
			r.corner.x = r.origin.x + HEIGHT;
		     }

	if ( bound.corner.y > sr.corner.y+2+M*WINSIZE )		/* BOTTOM	*/
	   {
		r.origin.y = sr.corner.y+2;
		r.corner.y = r.origin.y + HEIGHT;
	   }

	else	if ( sr.origin.y > bound.origin.y+2+M*WINSIZE )	/* TOP	*/
		   {
			r.corner.y = sr.origin.y-2;
			r.origin.y = r.corner.y - HEIGHT;
		   }
		else {						/* MIDDLE	*/
			if ( midflag == TRUE ) return makerect( 0,0,0,0 );
			r.origin.y = bound.origin.y;
			r.corner.y = r.origin.y + HEIGHT;
		     }
	return r;
}

Point
sticky( r,p )
	Rectangle r;
	Point p;
{
	return div( sub( p,r.origin ),M ); 
}

setpoint( bp,r,p,code )
	Bitmap *bp;
	Rectangle r;
	Point p;
	int code;
{
	static Rectangle dotrect = { { 0,0 },{ M,M } };

	p = add( r.origin,mul( p,M ) );
	rectf( bp,raddp( dotrect,p ),code );
}

#define HIGHBIT		((unsigned)0x80000000)

getpoint( bp,p )
	Bitmap *bp;
	Point p;
{
	Word *ptr = addr( bp,p );

	return (( *ptr ) & (HIGHBIT>>(p.x%32) ) ) ? 1 : 0;
}

#ifdef SAFE
Rectangle
findroom( sr,bound )
	Rectangle sr,bound;
{
	Point p;

	if ( bound.corner.x > sr.corner.x+2+M*WINSIZE )
		p.x = sr.corner.x+2;
	else	if ( sr.origin.x > bound.origin.x+2+M*WINSIZE )
			p.x = sr.origin.x-M*WINSIZE-2;
		else	if ( bound.corner.x > sr.origin.x+2+M*WINSIZE )
				p.x = sr.origin.x+2;
			else	if ( sr.corner.x > bound.origin.x+2+M*WINSIZE )
					p.x = sr.corner.x-M*WINSIZE-2;
				else	return makerect( 0,0,0,0 );


	if ( bound.corner.y > sr.corner.y+2+M*WINSIZE )
		p.y = sr.corner.y+2;
	else	if ( sr.origin.y > bound.origin.y+2+M*WINSIZE )
			p.y = sr.origin.y-M*WINSIZE-2;
		else	if ( bound.corner.y > sr.origin.y+2+M*WINSIZE )
				p.y = sr.origin.y+2;
			else	if ( sr.corner.y > bound.origin.y+2+M*WINSIZE )
					p.y = sr.corner.y-M*WINSIZE-2;
				else	return makerect( 0,0,0,0 );
	return raddp( bigrect,p );
}

#endif
