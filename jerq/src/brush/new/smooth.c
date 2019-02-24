/*
 *
 *	image smoothing for Brush  --  Brush/smooth.c
 *
 *  NOTE:  this code courtesy of the famous T. G. Dennehy
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

smooth( b,r,bfac )
	Bitmap		*b;
	Rectangle	r;
	Point		bfac;
{
	Bitmap	*one,*two,*three;
	Point	fac;
	short	i;	

	for ( fac.x=1; ( i= ( fac.x << 1 ) ) <= bfac.x; fac.x=i );
	for ( fac.y=1; ( i= ( fac.y << 1 ) ) <= bfac.y; fac.y=i );
	if ( ( ( r.corner.x-r.origin.x ) < ( fac.x*2 ) ) ||
	     ( ( r.corner.y-r.origin.y ) < ( fac.y*2 ) ) )
		return -4;
 
/*	allocate some bitmap space	*/

	if ( ( one=balloc( r ) ) == null(Bitmap) )
		return -1;

	if ( ( two=balloc( r ) ) == null(Bitmap) )
	   {
		bfree(one);
		return -2;
	   }

	if ( ( three=balloc( r ) ) == null(Bitmap) )
	   {
		bfree(one);
		bfree(two);
		return -3;
	   }


	for ( ;; )
	    {

		if ( ( ( fac.x >>= 1 ) == 0 ) ||
		     ( ( fac.y >>= 1 ) == 0 )   )
			break;

/*	up and to the right	*/

		rectf( one,one->rect,F_CLR );
		bitblt( b,Rect( r.origin.x+fac.x,r.origin.y,r.corner.x,
				r.corner.y-fac.y),
 			one,add( one->rect.origin,Pt( 0,fac.y ) ),F_XOR );

/*	down and to the left	*/

		rectf( two,two->rect,F_STORE );
		bitblt( b,Rect( r.origin.x,r.origin.y+fac.y,
				r.corner.x-fac.x,r.corner.y),
			two,add( two->rect.origin,Pt( fac.x,0 ) ),F_XOR );

/*	smooth along y=x	*/

		bitblt( two,two->rect,one,one->rect.origin,F_CLR );

/*	down and to the right	*/

		rectf( two,two->rect,F_CLR );
		bitblt( b,Rpt( add( r.origin,Pt( fac.x,fac.y ) ),r.corner ),
			two,two->rect.origin,F_XOR );

/*	up and to the left	*/

		rectf( three,three->rect,F_STORE );
		bitblt( b,Rpt( r.origin,sub (r.corner,Pt( fac.x,fac.y ) ) ),
			three,add( three->rect.origin,Pt( fac.x,fac.y ) ),F_XOR );

/*	smooth along y=-x	*/

		bitblt( three,three->rect,two,two->rect.origin,F_CLR );

/*	OR the smoothing masks into the original picture	*/

		bitblt( two,two->rect,b,r.origin,F_OR );
		bitblt( one,one->rect,b,r.origin,F_OR );
           }

/*	free the space	*/


	bfree( one );
	bfree( two );
	bfree( three );

	return 0;
}
