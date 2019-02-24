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

