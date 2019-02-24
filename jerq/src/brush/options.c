/*
 *
 *	set options for Brush  --  brush/options.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

extern Word BrushSize;

Word
analyze( brush )		/* figure out brush size for dashed lines	*/
	Texture16 *brush;
{
	Word i,miny = 15,maxy = 0,minx = 15,maxx = 0;
	Word lo,hi;
	Word xsize,ysize;

	for ( i=0; i<16; i++ )
		if ( brush->bits[i] )
		   {
			miny = i;
			lo = lowest( brush->bits[i] );
			minx = min( minx,lo );
			hi = highest( brush->bits[i] );
			maxx = max( maxx,hi );
			break;
		   }
	for ( i=15; i>=0; i-- )
		if ( brush->bits[i] )
		   {
			maxy = i;
			break;
		   }
	xsize = 1 + abs( maxx - minx );
	ysize = 1 + abs( maxy - miny );
	return 2*max( xsize,ysize );
}

#define HIGHBIT		0x8000

lowest( bits )
	unsigned short bits;
{
	unsigned short mask=HIGHBIT;
	int i;

	for ( i=0; i<16; i++,mask>>=1 )
		if ( bits & mask )
			break;
	return i;
}

#define LOWBIT		0x0001

highest( bits )
	unsigned short bits;
{
	unsigned short mask=LOWBIT;
	int i;

	for ( i=15; i>=0; i--,mask<<=1 )
		if ( bits & mask )
			break;
	return i;
}


setbrush( tp )		/* set brush to texture16 tp, init mask bitmaps	*/
	Texture16 *tp;
{
	Word i;

	brush = tp;
	for ( i=0; i<16; i++ )
	    {
		maskwords[i] = ((Word)brush->bits[i])<<16;
		notmaskwords[i] = maskwords[i] ^ 0xffffffff;
	    }
	BrushSize = analyze( brush );
	cursswitch( brush );
}

select( r,num )
	Rectangle r;	/* rect where the textures live */
	Word num;	/* how many to choose from */
{
	Word i,hgt,off;
	hgt = RHEIGHT(r)/num;
	off = mouse.xy.y-r.origin.y;

	if ( bttn3() && ptinrect( mouse.xy,r ) )
		return muldiv( mouse.xy.y-r.origin.y+4,num,RHEIGHT( r ) );
	else	return -1;
}

imagemenu( r,current,images,num )
	Rectangle r;		/* rect in D coords */
	Word current;		/* index of current value for hilighting */
	Texture16 *images[];	/* array of images to be drawn */
	Word num;		/* number of images */
{
	Point p;
	int i,hgt;

	hgt  = RHEIGHT( r )/num;
	p = add( r.origin,Pt(4,4) );

	rectf( &display,r,F_STORE );
	rectf( &display,inset( r,2 ),F_XOR );

	for ( i=0; i<num; i++ )
	    {
		tx16blt( images[i],Rect(0,0,16,16),
			 &display,add(p,Pt(0,i*hgt)),F_STORE );
		if ( i== current )
			highlight( add(p,Pt(0,i*hgt)) );
	    }
}

highlight( p )		/* highlight current brush/shade selection */
	Point p;
{
	rectf( &display,Rpt( sub( p,Pt(2,2) ),add( p,Pt(18,19) ) ),F_XOR );
	rectf( &display,Rpt( sub( p,Pt(2,4) ),add( p,Pt(18,21) ) ),F_XOR );
}

tx16blt( t,r,bp,p,code )	/* copies a texture16 to a bitmap pt	*/	
	Texture16 *t;		/*  WITHOUT alignment			*/
	Rectangle r;
	Bitmap *bp;
	Point p;
	Word code;
{
	static Word tmpwords[16];
	static Bitmap tmp = { tmpwords,1,{ {0,0},{16,16} } };

	r = rsubp( r,r.origin );
	texture16( &tmp,r,t,F_STORE );
	bitblt( &tmp,r,bp,p,code );
}

