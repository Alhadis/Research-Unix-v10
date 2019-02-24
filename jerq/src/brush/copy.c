/*
 *
 *	bitmap copy for Brush  --  brush/copy.c
 *
 *	includes:	magnify, flip x, flip y, rotation, shearing.
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

new_image()		/* gets rect and sets up Image	*/
{
	Image result;
	Rectangle r;

	result.r = sweeprect( Pt(1,1) );
	if ( nullrect( result.r ) )
		return FAILURE;

	result.bp  = balloc( result.r );
	result.mask  = null( Bitmap );
	if ( !result.bp )
	   {
		errmsg( "cannot alloc in new_image()" );
		return FAILURE;
	   }
	cursinhibit();
	bitblt( &bigscreen,result.r,result.bp,result.r.origin,F_STORE );
	cursallow();

	result.pos = result.r.origin;
	result.alg = alignment( result.pos );

	SetCurrentImage( result );
	return SUCCESS;
}

blowup( pic )		/* return magnified copy of Image pic	*/
	Image pic;
{
	Rectangle dr;
	Point scale,size;
	register Bitmap *tmp;

	size = extent( pic.r );
	if ( 0>= size.x || 0>=size.y ) return FAILURE;

	dr = sweeprect( size );
	if ( nullrect( dr ) ) return FAILURE;

	scale.x = max( 1,( dr.corner.x-dr.origin.x )/size.x );
	scale.y = max( 1,( dr.corner.y-dr.origin.y )/size.y );

	tmp = balloc( dr );

	if ( !tmp )
	   {
		errmsg( "no mem" );
		return FAILURE;
	   }

	cursswitch( &hourglass );
	rectf( tmp,tmp->rect,F_CLR );

	/* copy src to dst */

	bitblt( pic.bp,pic.r,tmp,dr.origin,F_STORE );
	magnify( tmp,dr.origin,size,scale );

	pic = MakeImage( tmp,tmp->rect,pic.pos,pic.alg );
	SetCurrentImage( pic );
	return SUCCESS;
}

edgeit( pic )
	Image pic;
{
	Bitmap *bp;
	Point p,p_align;
	Rectangle r;

	p_align = add( pic.alg,Pt(15,15) );
	p_align.x %= 16;
	p_align.y %= 16;

	p = r.origin = p_align;
	r.corner = add( p,add( Pt(2,2),extent( pic.r ) ) );
	cursswitch( &hourglass );

	if ( !nullrect( r ) )
		if ( ( bp = balloc( r ) ) != null(Bitmap) )
		   {
			rectf( bp,r,F_CLR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 0,1 ) ),F_OR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 2,1 ) ),F_OR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 1,0 ) ),F_OR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 1,2 ) ),F_OR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 1,1 ) ),F_CLR );
			pic = MakeImage( bp,bp->rect,pic.pos,p_align );
			SetCurrentImage( pic );
			return SUCCESS;
		   }
	return FAILURE;
}

flippit( pic,direction )
	Image pic;
	Word direction;
{
	Bitmap *bp;
	Point p,p_align;
	Rectangle r;

	if ( direction == FLIPX )
	   {
		p_align.x = 16 - pic.r.corner.x%16;
		p_align.y = pic.r.origin.y%16;
	   }

	else {
		p_align.x = pic.r.origin.x%16;
		p_align.y = 16 - pic.r.corner.y%16;
	     }

	r.origin = p_align;
	r.corner = add( r.origin,extent( pic.r ) );
	cursswitch( &hourglass );

	if ( !nullrect( r ) )
		if ( bp = balloc( r ) )
		   {
			switch( direction )
			      {
				case FLIPX:	flip_x( pic.bp,pic.r,bp,r.origin );
						break;
				case FLIPY:	flip_y( pic.bp,pic.r,bp,r.origin );
						break;
			      }
			pic = MakeImage( bp,bp->rect,pic.pos,p_align );
			SetCurrentImage( pic );
			return SUCCESS;
		   }
	return FAILURE;
}

flip_x( sb,r,db,p )
	Bitmap *sb,*db;
	Rectangle r;
	Point p;
{
	Rectangle col;
	Word i,wid;

	wid = RWIDTH( r );
	col = r;
	col.corner.x = col.origin.x + 1;

	for ( i=0; i<=wid; i++ )
		bitblt( sb,raddp( col,Pt(i,0) ),
			db,add( p,Pt(wid-i-1,0) ),F_STORE );
}
	
flip_y( sb,r,db,p )
	Bitmap *sb,*db;
	Rectangle r;
	Point p;
{
	Rectangle row;
	Word i,hgt;

	hgt = RHEIGHT( r );
	row = r;
	row.corner.y = row.origin.y + 1;

	for ( i=0; i<=hgt; i++ )
		bitblt( sb,raddp( row,Pt(0,i) ),
			db,add( p,Pt(0,hgt-i-1) ),F_STORE );
}


#define RBO	rbuf.origin
#define RBC	rbuf.corner

protate( pic )
	Image pic;
{
	Bitmap *bp;
	Rectangle r;
	Rectangle rbuf;		/* holds quadrant of bp->r orig image in */
	Point alg;
	int hgt,wid,size;

	wid = RWIDTH( pic.r );
	hgt = RHEIGHT( pic.r ); 
	size = wid + hgt;

	alg.x = ( pic.r.origin.y + 15 )%16;
	alg.y = 16 - pic.r.corner.x%16;

	r.origin = add( sub( pic.r.origin,pic.alg ),alg );
	r.corner = add( r.origin,Pt( size,size ) );

	rbuf.origin = r.origin;
	rbuf.corner = add( r.origin,Pt(wid,hgt) );

	if ( !nullrect( pic.r ) )
		if ( bp = balloc( r ) )
		   {
			cursswitch( &hourglass );
			rectf( bp, bp->rect, F_CLR );
			rbuf.corner = r.corner;
			rbuf.origin = sub( r.corner,Pt(wid,hgt) );
			bitblt( pic.bp,pic.r,bp, RBO,F_XOR );

			horshear( bp,rbuf,-hgt,TRUE );

			vershear( bp,
				  Rect( RBO.x-hgt,RBO.y,RBC.x,RBC.y ),
	    			  -size,FALSE );

			horshear( bp,
				  Rect( r.origin.x,r.origin.y,RBC.x,RBO.y ),
				  -wid,TRUE );
			pic = MakeImage( bp,
					  raddp( Rect( 0,0,hgt+1,wid+1 ),r.origin ),
					  pic.pos,alg );
			SetCurrentImage( pic );
			return SUCCESS;
		   }
	return FAILURE;
}

mrotate( pic )
	Image pic;
{
	Bitmap *bp;
	Rectangle r;
	Rectangle rbuf;		/* holds quadrant of bp->r orig image in */
	Point alg;
	int hgt,wid,size;

	wid = RWIDTH( pic.r ); 
	hgt = RHEIGHT( pic.r ); 
	size = wid + hgt;

	alg.x = 16 - pic.r.corner.y%16;
	alg.y = pic.r.origin.x%16;

	r.origin = add( sub( pic.r.origin,pic.alg ),alg );
	r.corner = add( r.origin,Pt( size,size ) );

	rbuf.origin = r.origin;
	rbuf.corner = add( r.origin,Pt(wid,hgt) );

	if ( !nullrect( pic.r ) )
		if ( bp = balloc( r ) )
		   {
			cursswitch( &hourglass );
			rectf( bp, bp->rect, F_CLR );
			bitblt( pic.bp,pic.r,bp, r.origin,F_XOR );
			horshear( bp,rbuf,hgt,TRUE );
			vershear( bp,
				  Rect( RBO.x,RBO.y,RBC.x+hgt,RBC.y ),
	    			  size,FALSE );
			horshear( bp,
				  Rect( RBO.x,RBC.y-1,RBC.x+hgt,RBC.y+wid-1 ),
				  -wid,FALSE );
			bitblt( bp,raddp( Rect( 0,hgt-1,hgt,size ),r.origin ),
				bp,r.origin,F_STORE );
			pic = MakeImage( bp,raddp( Rect( 0,0,hgt,wid ),r.origin ),
					  pic.pos,alg );
			SetCurrentImage( pic );
			return SUCCESS;
		   }
	return FAILURE;
}

Point stretchrect();

shear( pic )
	Image pic;
{
	Bitmap *bp;
	Rectangle rbuf;		/* holds quadrant of bp->r orig image in */
	Rectangle r;
	Point p,dir;
	int hgt,wid;

	if ( !nullrect( pic.r ) )
	   {
		dir = stretchrect( RectAt( pic.r,pic.pos ) );
		wid = RWIDTH( pic.r ) + abs( dir.x );
		hgt = RHEIGHT( pic.r ) + abs( dir.y );
	  
		r.origin = pic.r.origin;
		r.corner = add( pic.r.origin,Pt( wid,hgt ) );

		rbuf = pic.r;
	  
		if ( bp = balloc( r ) )
		   {
			cursswitch( &hourglass );
			rectf( bp,bp->rect,F_CLR );
			if ( dir.x < 0 )
			   {
				rbuf.origin.x -= dir.x;
				rbuf.corner.x -= dir.x;
			   }
			if ( dir.y < 0 )
			   {
				rbuf.origin.y -= dir.y;
				rbuf.corner.y -= dir.y;
			   }
			bitblt( pic.bp,pic.r,bp,rbuf.origin,F_XOR );
			if ( dir.x != 0 )
				horshear( bp,rbuf,dir.x,TRUE );
			else vershear( bp,rbuf,dir.y,TRUE );
			pic =  MakeImage( bp,r,
					   pic.pos,alignment(r.origin) );
			SetCurrentImage( pic );
			return SUCCESS;
		   }
	   }
	return FAILURE;
}

Point
tweak( p,ank )
	Point p,ank;
{
	Point del;

	del = sub( ank,p );
	p.x = ( abs(del.x)>abs(del.y) ) ? p.x : ank.x;
	p.y = ( abs(del.x)>abs(del.y) ) ? ank.y : p.y;
	return p;
}

#define llft	Pt(r.origin.x,r.corner.y)
#define urt	Pt(r.corner.x,r.origin.y)

drawtrap( r,off,code )
	Rectangle r;
	Point off;
	Word code;
{
	moveto( add( r.origin,off ) );
	if ( off.x )
	     drawto( llft,code );
	else drawto( add( llft,off ),code );

	drawto( r.corner,code );

	if ( off.y )
	     drawto( urt,code );
	else drawto( add( urt,off ),code );

	drawto( add( r.origin,off ),code );
}

Point lastpt;

moveto( p )
	Point p;
{
	lastpt = p;
}

drawto( p,code )
	Point p;
	Word code;
{
	segment( &bigscreen,lastpt,p,code );
	lastpt = p;
}

horshear( bp,r,dx,top )
	Bitmap *bp;
	Rectangle r;
	int dx;
	int top;
{
	int j,hgt,shift,off;

	hgt = RHEIGHT( r );

	for ( j=0; j<hgt; j++ )
	    {
		shift = top ? hgt-j-1 : j;
		off = muldiv( shift,dx,hgt );
		bitblt( bp,Rect(r.origin.x,r.origin.y+j,r.corner.x,r.origin.y+j+1),
			bp,Pt(r.origin.x+off,r.origin.y+j),
			F_STORE );
		if ( dx > 0 )
			rectf( bp,Rect( r.origin.x,r.origin.y+j,
				r.origin.x+off,r.origin.y+j+1 ),F_CLR );
		else rectf( bp,Rect( r.corner.x+off,r.origin.y+j,
			    r.corner.x,r.origin.y+j+1 ),F_CLR );
	    }
}

/* STOPPED FORMATTING HERE	*/

vershear( bp,r,dy,lft )
	Bitmap *bp;
	Rectangle r;
	int dy;
	int lft;
{
	int i,wid,shift,off;

	wid = RWIDTH( r );

	for ( i=0; i<wid; i++ )
	    {
		shift = lft ? wid-i-1 : i;
		off = muldiv( shift,dy,wid );
		bitblt( bp,Rect(r.origin.x+i,r.origin.y,r.origin.x+i+1,r.corner.y),
			bp,Pt(r.origin.x+i,r.origin.y+off),
			F_STORE);
		if ( dy > 0 )
			rectf( bp,Rect( r.origin.x+i,r.origin.y,
				r.origin.x+i+1,r.origin.y+off ),F_CLR );
		else	rectf( bp,Rect( r.origin.x+i,r.corner.y+off,
				r.origin.x+i+1,r.corner.y ),F_CLR );
	    }
}

magnify( bp,p,size,scale )
	Bitmap *bp;
	Point p;
	Point size,scale;
{
	register int i,shift;

	if ( scale.x>1 )
	   {
		for ( i=size.x-1; i>0; i-- )	/* spread horizontally */
		    {
			bitblt( bp,raddp( Rect( i,0,i+1,size.y ),p ),
				bp,add( Pt( i*scale.x,0 ),p ),F_OR );
			rectf( bp,raddp( Rect( i,0,i+1,size.y ),p ),F_CLR );
		    }

		for ( i=1; i<scale.x; i<<=1 )	/* smear horizontally */
		    {
			shift = min( i,scale.x-i );
			bitblt( bp,raddp( Rect( 0,0,size.x*scale.x-shift,size.y ),
					  p ),
				bp,add( Pt( shift,0 ),p ),F_OR );
		    }
	   }

	if ( scale.y>1 )
	   {
		for ( i=size.y-1; i>0; i-- )	/* spread vertically */
		    {
			bitblt( bp,raddp( Rect( 0,i,size.x*scale.x,i+1 ),p ),
				bp,add( Pt( 0,i*scale.y ),p ),F_OR );
			rectf( bp,raddp( Rect( 0,i,size.x*scale.x,i+1 ),p ),F_CLR );
		    }

		for ( i=1; i<scale.y; i<<=1 )	/* smear vertically */
		    {
			shift = min( i,scale.y-i );
			bitblt( bp,
				raddp( Rect( 0,0,
					     size.x*scale.x,size.y*scale.y-shift ),
					p ),
				bp,add( Pt( 0,shift ),p ),F_OR );
		    }
	
		if ( smoothing() )
			smooth( bp,bp->rect,scale );
	   }
}

Point
alignment( p )
	Point p;
{
	p.x %= 16;
	p.y %= 16;
	return p;
}

#ifdef GIMMEABREAK
shadowit( pic )
	Image pic;
{
	Bitmap *bp;
	Point p,p_align;
	Rectangle r;

	p_align = add( pic.alg,Pt(14,14) );
	p_align.x %= 16;
	p_align.y %= 16;

	p = r.origin = p_align;
	r.corner = add( p,add( Pt(2,2),extent( pic.r ) ) );
	cursswitch( &hourglass );

	if ( !nullrect( r ) )
		if ( ( bp = balloc( r ) ) != null(Bitmap) )
		   {
			rectf( bp,r,F_CLR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 0,0 ) ),F_OR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 1,1 ) ),F_CLR );
			bitblt( pic.bp,pic.r,bp,add( p,Pt( 2,2 ) ),F_OR );
			pic = MakeImage( bp,bp->rect,pic.pos,p_align );
			SetCurrentImage( pic );
			return SUCCESS;
		   }
	return FAILURE;
}
#endif

