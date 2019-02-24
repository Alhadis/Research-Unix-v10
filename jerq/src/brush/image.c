/*
 *
 *	Image handling for Brush  --  brush/image.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Image CurrentImage  = {
			null( Bitmap ),null( Bitmap ),
			{ { 0,0 },{ 0,0 } },
			{ 0,0 },{ 0,0 }
		      };

Image
MakeImage( bp,r,p,alg )		/* builds Image from pieces 	*/
	Bitmap *bp;
	Rectangle r;
	Point p,alg;
{
	Image result;

	result.bp   = bp;	
	result.mask = null( Bitmap );	
	result.r    = r;
	result.pos  = p;
	result.alg  = alg;

	return result;
}

FreeImage( ip )			/* bfree an Image's bitmaps	*/
	Image *ip;
{
	if ( ip->mask != null( Bitmap ) )
	   {
		bfree( ip->mask );
		ip->mask = null( Bitmap );
	   }
	if ( ip->bp != null( Bitmap ) )
	   {
		bfree( ip->bp );
		ip->bp = null( Bitmap );
	   }
}

SetCurrentImage( image )	/* Guess what this does!!	*/
	Image image;
{
	FreeImage( &CurrentImage );
	CurrentImage = image;
}

MoveImage( pic,drawmode )
	Image *pic;
	Word drawmode;
{
	Rectangle cr;		/* canonical ( 0-aligned ) rect form of r */
	Point pos;
	Word FirstTime = TRUE;		/* ugh! gak! dag! kludge city!	*/
	Help old_set;
	Bitmap *safe=null(Bitmap);

	if ( pic->bp==null(Bitmap) || pic->bp==&display ) return;

	if ( drawmode==F_COPY && pic->mask==null(Bitmap) )
		if ( BuildMask( pic ) == FAILURE )
			return;

	if ( ( safe=balloc( pic->r ) )==null(Bitmap) )
		return;

	old_set = helpswitch( pos_set );

	wait( MOUSE );		/* IMPORTANT if not current layer	*/
	cursinhibit();
	cursset( pic->pos );

	cr = rsubp( pic->r,pic->r.origin );
	bitblt( &bigscreen,raddp( cr,pic->pos ),
		safe,safe->rect.origin,F_STORE );	/* stash */

	while ( !bttn3() )
	      {
		if ( FirstTime || !eqpt( pic->pos,align( mouse.xy,pic->alg ) ) )
		   {
			FirstTime = FALSE;
			bitblt( safe,safe->rect,
				&screen,pic->pos,F_STORE );	/* restore */
			pic->pos = pos = align( mouse.xy,pic->alg );
			bitblt( &bigscreen,raddp( cr,pos ),
				safe,safe->rect.origin,F_STORE );	/* stash */

			switch( drawmode )	/* now draw the new result	*/
			      {
				case F_OR:
				case F_STORE:
				case F_XOR:	bitblt( pic->bp,pic->r,
							&screen,pos,drawmode );
						break;
				case F_AND:	rectf( pic->bp,pic->r,F_XOR );
						bitblt( pic->bp,pic->r,
							&screen,pos,F_CLR );
						rectf( pic->bp,pic->r,F_XOR );
						break;
				case F_COPY:	bitblt( pic->mask,
							inset( pic->mask->rect,1 ),
							&screen,pos,F_CLR );
						bitblt( pic->bp,pic->r,
							&screen,pos,F_OR );
						break;
			      }
		   }
		if ( bttn12() )
			if ( bttn1() )		/* make the change permanent */
				bitblt( &bigscreen,raddp( cr,pic->pos ),
					safe,safe->rect.origin,F_STORE );
			else	if ( bttn2() )
					bitblt( pic->bp,pic->r,
						safe,safe->rect.origin,F_CLR );
	      }

	bitblt( safe,safe->rect,
		&screen,pic->pos,F_STORE );	/* restore */

	wait( MOUSE );
	cursallow();
	helpswitch( old_set );
	bfree( safe );	
}

BuildMask( pic )		/* make bkg mask for pic	*/
	Image *pic;
{
	Rectangle cr;		/* canonical ( 0-aligned ) form of pic->r */
	Bitmap *bp;
	Texture16 *tp;

	cr = rsubp( pic->r,pic->r.origin );
	cr.corner = add( cr.corner,Pt( 2,2 ) );	/* allow for fill area	*/

	if ( ( bp=balloc( cr ) ) != null( Bitmap ) )
	   {
		tp = cursswitch( &hourglass );
		rectf( bp,cr,F_CLR );
		bitblt( pic->bp,pic->r,bp,Pt(1,1),F_STORE );
		fill( bp,cr,cr.origin,FALSE );
		bitblt( pic->bp,pic->r,bp,Pt(1,1),F_CLR );
		rectf( bp,cr,F_XOR );
		cursswitch( tp );
		pic->mask = bp;
		return SUCCESS;
	   }
	return FAILURE;
}


#ifdef COULD_BE_USEFUL
rectprint( r )
	Rectangle r;
{
	errprintf( "r is { { %d,%d },{ %d,%d } }",
		    r.origin.x,r.origin.y,r.corner.x,r.corner.y );
}

ShowMask( bp,r,p )
	Bitmap *bp;
	Rectangle r;
	Point p;
{
	waitbuttons( UP );
	screenswap( bp,r,raddp( r,p ) );
	waitbuttons( DOWN );
	waitbuttons( UP );
	screenswap( bp,r,raddp( r,p ) );
}
#endif

