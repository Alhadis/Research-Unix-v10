/*
 *
 *	image shrink for Brush  --  brush/shrink.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 *
 */

#include "brush.h"

#define DARK		1
#define LIGHT		0

char *stext[] = { "shades","black/white",NULL };
Menu smenu = { stext };
Image crunch();

Rectangle
makerect( x0,y0,x1,y1 )
	Word x0,y0,x1,y1;
{
	Rectangle r;
	r.origin.x = x0;
	r.origin.y = y0;
	r.corner.x = x1;
	r.corner.y = y1;
	return r;
}

int D4[4][4] =        { 0,0,2,2,
			0,0,2,2,
			3,3,1,1,
			3,3,1,1,};
int D2[2][2] = { 0,2,3,1 };

#define HIGHBIT		((unsigned)0x80000000)
#define LOW5		0x1f

unsigned bitmasks[] = {
			0x80000000,0x40000000,0x20000000,0x10000000,
			0x08000000,0x04000000,0x02000000,0x01000000,
			0x00800000,0x00400000,0x00200000,0x00100000,
			0x00080000,0x00040000,0x00020000,0x00010000,
			0x00008000,0x00004000,0x00002000,0x00001000,
			0x00000800,0x00000400,0x00000200,0x00000100,
			0x00000080,0x00000040,0x00000020,0x00000010,
			0x00000008,0x00000004,0x00000002,0x00000001,
		      };

Image
crunch( pic )		/* return smaller copy of Image pic	*/
	Image pic;
{
	register Bitmap *tmp;
	Point size;
	int dark;
	Rectangle dr;
	Help old_set;


	if ( ( dark = menupick3( &smenu ) ) < 0 ) return pic;

	size = div( extent( pic.r ),2 );
	if ( 0 >= size.x || 0 >= size.y ) return pic;

	dr = raddp( makerect( 0,0,size.x,size.y ),pic.r.origin );

	if ( nullrect( dr ) ) return pic;

	tmp = balloc( dr );

	if ( !tmp )
	   {
		errmsg( "no mem" );
		return pic;
	   }

	cursswitch( &hourglass );
	rectf( tmp,tmp->rect,F_CLR );

	/* copy src to dst */

	old_set = helpswitch( kill_set );	
	
	shrink( pic.bp,pic.r,tmp,dr.origin,dark );

	helpswitch( old_set );

	Ibfree( pic.bp );
	pic = make_image( tmp,tmp->rect,pic.pos,pic.alg );
	return pic;
}

shrink( sb,r,db,dorg,darkflag )		/* "Official" shrink */
	Bitmap *sb,*db;
	Rectangle r;
	Point dorg;
	int darkflag;
{
	Point s,d;			/* src & dst points		*/
	int dither,pixsum;
	register int total = 0;
	register unsigned Word *ptr;

	dither = 1;

	s = r.origin;
	d = dorg;

	for ( ; s.y<r.corner.y; d.y++,wait(CPU) )
	    {
		for ( d.x=dorg.x,s.x=RORG.x; s.x<r.corner.x; d.x++ )
		    {
			if ( bttn123() ) return;
			if ( darkflag==LIGHT ) dither = D4[s.y&3][s.x&3];

			ptr = (unsigned *)addr(sb,s);
			total  = (*ptr & bitmasks[s.x&LOW5] ) ? 1 : 0;
			if (*ptr & bitmasks[(s.x+1)&LOW5] ) total++;

			ptr += sb->width;
			if (*ptr & bitmasks[s.x&LOW5] ) total++;
			if (*ptr & bitmasks[(s.x+1)&LOW5] ) total++;

			if ( total > dither ) point( db,d,F_STORE );
			s.x += ( s.x&1 ) ? 1 : 2;
		    }
		s.y += ( s.y&1 ) ? 1 : 2;
	    }
}

#ifdef FOO
total( bp,p )
	Bitmap *bp;
	Point p;
{
	int result = 0;
	unsigned Word *ptr = (unsigned *)addr(bp,p);

	result  = (*ptr & HIGHBIT >> (p.x%32) ) ? 1 : 0;
	result += (*ptr & HIGHBIT >> ((p.x+1)%32) ) ? 1 : 0;

	ptr += bp->width;
	result += (*ptr & HIGHBIT >> (p.x%32) ) ? 1 : 0;
	result += (*ptr & HIGHBIT >> ((p.x+1)%32) ) ? 1 : 0;
	return result;
}

#endif

