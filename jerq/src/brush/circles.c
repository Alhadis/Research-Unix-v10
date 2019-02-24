/*
 *
 *	circles & discs for Brush  --  brush/circles.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 *
 */

#include "brush.h"

void disct16(),myellipse();
Point xydist(),alignment();

/*	dashed line stuff	*/

discs( oldpic,filled )		/* draw textured discs */
	Image oldpic;
	Word filled;
{
	Point rad;
	Point org,p;
	Rectangle r;
	Bitmap *tmp;
	char scratch[20];
	Help old_set;
	int foo;

	old_set = helpswitch( circ_set );	
	cursswitch( &sweepcircle );

	while( !bttn123() ) wait( MOUSE );

	if ( bttn12() )
	   {
		helpswitch( old_set );	
		cursswitch( brush );
		return FAILURE;
	   }

	org = p = mouse.xy;
	rad.x = rad.y = 0;

	ellipse( &display,org,rad.x,rad.y,F_XOR );
	stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				"%d %d",rad.x,rad.y );

	while ( bttn3() )
		if ( !eqpt( p,mouse.xy ) )
		   {
			ellipse( &display,org,rad.x,rad.y,F_XOR );
			stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				 "%d %d",rad.x,rad.y );
			rad = xydist( p=mouse.xy,org );
			stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				 "%d %d",rad.x,rad.y );
			ellipse( &display,org,rad.x,rad.y,F_XOR );
		   }

	ellipse( &display,org,rad.x,rad.y,F_XOR );
	stringf( &defont,scratch,&display,add( p,Pt(10,10) ),F_XOR,
				"%d %d",rad.x,rad.y );
	helpswitch( old_set );	
	cursswitch( &hourglass );

	/* punt if ellipse too small	*/

	if ( rad.x < 1 || rad.y < 1 )
		return FAILURE;

	/* add brush width if circle */


	if ( !filled )
	   {
		r.origin = sub( org,add( rad,Pt(8,8) ) );
		r.corner = add( org,add( rad,Pt(9,9) ) );

		if ( r.origin.x<0 )
		   {
			foo = 16*( (r.origin.x-16)/16 );
			r.origin.x -= foo;
			r.corner.x -= foo;
			org.x -= foo;
		   }

		if ( r.origin.y<0 )
		   {
			foo = 16*( (r.origin.y-16)/16 );
			r.origin.y -= foo;
			r.corner.y -= foo;
			org.y -= foo;
		   }

		if ( !( tmp = balloc( r ) ) ) return FAILURE;
		rectf( tmp,tmp->rect,F_CLR );
		myellipse( tmp,org,rad,F_STORE );
	   }
	
	else {
		r.origin = sub( org,rad );
		r.corner = add( org,add( rad,Pt(1,1) ) );

		if ( r.origin.x<0 )
		   {
			foo = 16*( (r.origin.x-16)/16 );
			r.origin.x -= foo;
			r.corner.x -= foo;
			org.x -= foo;
		   }

		if ( r.origin.y<0 )
		   {
			foo = 16*( (r.origin.y-16)/16 );
			r.origin.y -= foo;
			r.corner.y -= foo;
			org.y -= foo;
		   }
		if ( !(tmp = balloc( r ) ) )
		   {
			errmsg( "no mem" );
			return FAILURE;
		   }
		rectf( tmp,tmp->rect,F_CLR );
		eldisc( tmp,org,rad.x,rad.y,F_STORE );
		texture16( tmp,tmp->rect,shade,F_OR );
		eldisc( tmp,org,rad.x,rad.y,F_XOR );
		texture16( tmp,tmp->rect,shade,F_XOR );
	      }

	if ( drawmode != F_XOR ) point( tmp,org,F_XOR );
	SetCurrentImage( MakeImage( tmp,tmp->rect,r.origin,alignment(r.origin) ) );
	return SUCCESS;
}

Point
xydist( p,q )
	Point p,q;
{
	p.x = abs( p.x-q.x );
	p.y = abs( p.y-q.y );
	return p;
}
void
disct16( bp,p,rad,tp,code )
	Bitmap *bp;
	Point p;
	Word rad;
	Texture16 *tp;
	Word code;
{
	Texture local;
	register short *dest = (short *)&local;
	register short *ssrc  = (short *)tp;
	register int    i;

	i = 16;
	do {
		*dest = *ssrc;
		*(dest + 1) = *ssrc;
		*((int *)(dest +32)) = *((int *)dest);
		ssrc++;
		dest += 2;
	} while ( --i > 0 );
	discture( bp,p,rad,&local,code );
}

extern void doit();

void
myellipse( bp,p,rad,f )
	Bitmap *bp;
	Point p,rad;
	Code f;
{
	myellip1( bp,p,rad.x,rad.y,doit,Pt(0,rad.y),Pt(0,rad.y),f );
}

/*
 *
 *	the following was shamelessly swiped from $DMD/src/lib/libj/ellipse.c
 *	so it could be modified for dashed lines.
 *
 *	what IS this world coming to?
 *
 */

#define labs(x,y) if((x=y)<0) x= -x
#define samesign(x,y) (((int)(x)^(int)(y)) > 0)

#define		BIG	077777
#define		HUGE	07777777777L


/* calculate b*b*x*x + a*a*y*y - a*a*b*b avoiding ovfl */

static long
resid(a,b,x,y)
register long a,b;
long x,y;
{
	long e = 0;
	long u = b*(a*a - x*x);
	long v = a*y*y;
	register q = u>BIG? HUGE/u: BIG;
	register r = v>BIG? HUGE/v: BIG;
	while(a || b) {
		if(e>=0 && b) {
			if(q>b) q = b;
			e -= q*u;
			b -= q;
		} else {
			if(r>a) r = a;
			e += r*v;
			a -= r;
		}
	}
	return(e);
}

myellip1(bp, p0, a, b, action, p1, p2, f)
	Point p0, p1, p2;
	long a, b;
	register void (*action)();
	register Bitmap *bp;
	Code f;
{
	int dx = p1.y>0? 1: p1.y<0? -1: p1.x>0? -1: 1;
	int dy = p1.x>0? -1: p1.x<0? 1: p1.y>0? -1: 1;
	long a2 = a*a;
	long b2 = b*b;
	register long dex = b2*(2*dx*p1.x+1);
	register long e;

	register long dey = a2*(2*dy*p1.y+1);
	register long ex, ey, exy;

	if(!eqpt(p1, p2) &&
	   (p1.x==0 && p2.x==0 && samesign(p1.y, p2.y) ||
	    p1.y==0 && p2.y==0 && samesign(p1.x, p2.x))) {
		segment(bp, add(p0, p1), add(p0,p2), f);
		return;
	}
	e = resid(a, b, p1.x, p1.y);
	a2 *= 2;
	b2 *= 2;
	do {
		labs(ex, e+dex);
		labs(ey, e+dey);
		labs(exy, e+dex+dey);
		if(exy<=ex || ey<ex) {
			p1.y += dy;
			e += dey;
			dey += a2;
		}
		if(exy<=ey || ex<ey) {
			p1.x += dx;
			e += dex;
			dex += b2;
		}
		if(p1.x == 0) {
			if(samesign(p1.y, dy))
				if ( Dashing() )
				   {
					if ( DashOn() )
						(*action)(bp, add(p0, p1), f);
					if ( ++DashIndex==BrushSize )
						DashIndex = 0;
				   }
				else	(*action)(bp, add(p0, p1), f);

			if(abs(p1.y) == b) {
				dy = -dy;
				dey = -dey + a2;
			}
		} else if(p1.y == 0) {
			if(samesign(p1.x, dx))
				if ( Dashing() )
				   {
					if ( DashOn() )
						(*action)(bp, add(p0, p1), f);
					if ( ++DashIndex==BrushSize )
						DashIndex = 0;
				   }
				else	(*action)(bp, add(p0, p1), f);
			if(abs(p1.x) == a) {
				dx = -dx;
				dex = -dex + b2;
			}
		} else
			if ( Dashing() )
			   {
				if ( DashOn() )
					(*action)(bp, add(p0, p1), f);
				if ( ++DashIndex==BrushSize )
					DashIndex = 0;
			   }
			else	(*action)(bp, add(p0, p1), f);
	} while(! eqpt(p1, p2));
}
