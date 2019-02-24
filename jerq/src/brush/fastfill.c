/*
 *
 *	fast fill for Brush  --  brush/fastfill.c
 *
 *	Brush 
 *	Garry Hodgson	1987
 *
 * 	this fill algorithm from Paul Heckbert, via netnews.
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

#define STACKSIZE 1000		/* max depth of stack */

#define PUSH(Y, XL, XR, DY) \
    if (sp<stack+STACKSIZE && Y+(DY)>=roy && Y+(DY)<rcy) \
	{sp->y = Y; sp->xl = XL; sp->xr = XR; sp->dy = DY; sp++;}

#define POP(Y, XL, XR, DY) \
    {sp--; Y = sp->y+(DY = sp->dy); XL = sp->xl; XR = sp->xr;}

struct seg { short y, xl, xr, dy; } stack[STACKSIZE], *sp = stack;

fill( bp,r,p,stoppable )
	Bitmap *bp;
	Rectangle r;
	Point p;				/* seed point		*/
	Word stoppable;				/* enables btnn123() abort	*/
{
	register int l, x1, x2, dy;
	register Word rox,roy,rcx,rcy;
	Point tp;
	Word pix;				/* old pixel value	*/

	rox = r.origin.x;
	roy = r.origin.y;
	rcx = r.corner.x;
	rcy = r.corner.y;

	sp = stack;				/* reset stack		*/
	pix = getpoint( bp,p );			/* check seed point	*/

	if ( pix==1 || !ptinrect( p,r ) ) return;

	PUSH( p.y,p.x, p.x,1 );			/* good luck segment	*/
	PUSH( p.y+1,p.x, p.x,-1 );		/* seed segment		*/

	while ( sp>stack && !( stoppable && bttn123() ) )
	      {
		/* pop segment off stack and fill a neighboring scan line */
		POP( p.y,x1,x2,dy );
		for ( p.x=x1,tp=p; p.x>=rox && getpoint( bp,p )==pix; p.x-- );
		segment( bp,tp,p,F_OR );
		if ( p.x>=x1 ) goto skip;
		l = p.x+1;
		if ( l<x1 )
			PUSH( p.y,l,x1-1,-dy );		/* leak on left? */
		p.x = x1+1;
		do {
			for ( tp=p; p.x<rcx && getpoint( bp,p )==pix; p.x++ );
			segment( bp,tp,p,F_OR );
			PUSH( p.y,l,p.x-1,dy );
			if ( p.x>x2+1 )
				PUSH( p.y,x2+1,p.x-1,-dy );	/* leak on right? */
skip:		    	for (p.x++; p.x<=x2 && getpoint( bp,p )!=pix; p.x++);
		    	l = p.x;
		   } while ( p.x<=x2 );
	      }
}

#define MAXSEEDS	20

fastfill()
{
	Rectangle r;
	register Bitmap *safe;
	Point seeds[MAXSEEDS];
	Word i,nseeds;
	Help old_set;
	Texture16 negshade;

	old_set = helpswitch( sweep_set );	
	r = sweeprect( Pt(1,1) );

	helpswitch( old_set );

	if ( nullrect(r) || !rectclip( &r,screen.rect ) )
		return;	
	if ( !( safe = balloc( r ) ) )
		erreturn( "no memory" );

	/* make a safe copy */

	bitblt( &screen,r,safe,r.origin,F_STORE );

	helpswitch( pos_set );	
	cursswitch( null(Texture) );

	errmsg( "seed points?" );
	nseeds = 0;
	point( &screen,seeds[nseeds++] = mouse.xy,F_OR );
	waitbuttons( UP );

	for ( ; !bttn3() && nseeds<MAXSEEDS; )
		if ( bttn12() )
		   {
			point( &screen,seeds[nseeds++] = mouse.xy,F_OR );
			while( bttn12() );
		   }

	waitbuttons( UP );

	/* clear all them dots */

	bitblt( safe,r,&screen,r.origin,F_STORE );

	helpswitch( kill_set );	
	cursinhibit();

	for ( i=0; i<nseeds && !bttn123(); i++ )
		fill( &screen,r,seeds[i],TRUE );

	if ( bttn123() )
		bitblt( safe,r,&screen,r.origin,F_STORE );
	else {
		for ( i=0; i<16; i++ )
			negshade.bits[i] = ~shade->bits[i];
		texture16( &screen,r,&negshade,F_CLR );
		bitblt( &screen,r,safe,r.origin,F_OR );

		bitblt( safe,r,&screen,r.origin,F_STORE );
	     }
	cursallow();

	bfree( safe );
	helpswitch( old_set );	
}
