/*
 *
 *	texture editing for Brush  --  brush/editext.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Texture16 grid = {
			0xFFF0, 0x8010, 0x8010, 0x8010,
	 		0x8010, 0x8010, 0x8010, 0x8010,
	 		0x8010, 0x8010, 0x8010, 0xFFF0,
	 		0x0000, 0x0000, 0x0000, 0x0000,
		 };

Texture16 nogrid = {
			 0x000F, 0x7FEF, 0x7FEF, 0x7FEF,
			 0x7FEF, 0x7FEF, 0x7FEF, 0x7FEF,
			 0x7FEF, 0x7FEF, 0x7FEF, 0x000F,
			 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		   };

#define BSIZE			12
#define ith_bit( i )		(1<<(15-i))
#define getdot( tp,i,j )	(tp->bits[i] & ith_bit( j ))
#define on_off()		(( bttn1() ) ? F_STORE : F_CLR )

#define ij_to_p(i,j)	add( r.origin,mul( Pt(j,i),BSIZE ) )

Rectangle box = { 0,0,BSIZE,BSIZE };
Rectangle bigbox = { 0,0,16*BSIZE,16*BSIZE };

Texture16
edit_text( tp )
	Texture16 *tp;
{
	Rectangle r;
	Point p;
	Texture16 result;
	Word i,j;
	Bitmap *bp;
	Help old_set;

	result = *tp;
	r = raddp( bigbox,Draw.r.origin );
	if ( !(bp = balloc( r )) )
	   {
		errmsg( "no mem" );
		return *tp;
	   }

	screenswap( bp,r,r );

	for ( i=0; i<16*BSIZE; i+=BSIZE )
		for ( j=0; j<16*BSIZE; j+=BSIZE )
			tx16blt( &grid,box,
				 &display,add( r.origin,Pt(i,j) ),F_STORE );

	old_set = helpswitch( pos_set );

	for ( i=0; i<16; i++ )
		for ( j=0; j<16; j++ )
			if ( getdot( tp,i,j ) )
				tx16blt( &nogrid,box,
					 &display,ij_to_p(i,j),F_STORE );

	for ( ; !bttn3(); wait(CPU) )
		for ( ; bttn12(); wait( CPU ) )
			if ( ptinrect( p=mouse.xy,r ) )
				splat( &result,r,
					div(sub(p,r.origin),BSIZE),on_off() );
	screenswap( bp,r,r );
	bfree( bp );
	helpswitch( old_set );
	return result;
}

splat( tp,r,p,code )
	Texture16 *tp;
	Rectangle r;
	Point p;
	Word code;
{
	switch( code )
	      {
		case F_STORE:	tp->bits[p.y] |= ith_bit( p.x );
				tx16blt( &nogrid,raddp( box,r.origin ),
					 &display,ij_to_p(p.y,p.x),F_STORE );
				break;
		case F_CLR:	tp->bits[p.y] &= ~ith_bit( p.x );
				tx16blt( &grid,raddp( box,r.origin ),
					 &display,ij_to_p(p.y,p.x),F_STORE );
				break;
	      }
}

Rectangle snarfbox();

Texture16
snarf_text( tp )
	Texture16 *tp;
{
	static Word buff[16];
	static Bitmap dummy = { buff,1,{{ 0,0 },{ 16,16 }} };

	Point p;
	Rectangle r;
	Texture16 result;
	Word i,j;
	Help old_set;

	old_set = helpswitch( snarf_set );

	tx16blt( tp,Rect(0,0,16,16),&dummy,Pt(0,0),F_STORE );

	r = snarfbox( 16 );
	if ( bttn3() )
	   {
		waitbuttons( UP );
		bitblt( &bigscreen,r,&dummy,Pt( 0,0 ),F_STORE );
	   }
	else	waitbuttons( UP );

	for ( i=0; i<16; i++ )
	      result.bits[i] = buff[i]>>16;

	helpswitch( old_set );
	return result;
}
