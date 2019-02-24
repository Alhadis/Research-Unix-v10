/*
 *
 *	drawing for Brush  --  brush/draw.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Word mapwords[16];
Bitmap map = { mapwords,1,{{0,0},{16,16}} };

void doit();

/*	dashed line stuff	*/

Word BrushSize = 0;		/* calculated by setbrush() via analyze()	*/
Word DashIndex = 0;		/* Dashes will draw when this == 0		*/

#define DrawPoint(p)	{ if ( Flex.value ) \
				reflect( p,realmode( drawmode ),Flex.value ); \
			  else	doit( &screen,p,realmode( drawmode ) );		}

static int delta;
static Point pos,target,slope,d;
static Point m,lastm;

aim( p )
	Point p;
{
	target = p;

	slope.x = 2*abs(target.x-pos.x);
	slope.y = 2*abs(target.y-pos.y);
	d.x = target.x > pos.x ? 1:-1;
	d.y = target.y > pos.y ? 1:-1;
	delta = ( slope.x > slope.y ) ? -slope.x/2 : -slope.y/2;
}

stepline()
{
	if ( slope.x > slope.y )
	   {
		if ( pos.x != target.x )
		   {
			if ( Dashing() )
			   {
				if ( DashOn() )
					DrawPoint( pos );
				if ( ++DashIndex==BrushSize )
					DashIndex = 0;
			   }
			else	DrawPoint( pos );
			pos.x += d.x;
			if ( ( delta += slope.y ) > 0 )
			   {
				delta -= slope.x;
				pos.y += d.y;
			   }
		   }
	   }
	else if ( pos.y != target.y )
		{
			if ( Dashing() )
			   {
				if ( DashOn() )
					DrawPoint( pos );
				if ( ++DashIndex==BrushSize )
					DashIndex = 0;
			   }
			else	DrawPoint( pos );
			pos.y += d.y;
			if ( ( delta+=slope.x) > 0 )
			   {
				delta -= slope.y;
				pos.x += d.x;
			   }
		}
}

freehand( mode )		/* this does freehand drawing using mouse	*/
	int mode;
{
	Point start;
	start = pos = target = lastm = mouse.xy;

	DashIndex = 0;		/* init dashed line index	*/

	DrawPoint( pos );

	if ( !Continuous() )
	   {
		for ( ; bttn12(); wait(MOUSE) )
		    {
			m = ( squaring() ) ? tweak( mouse.xy,start ) : mouse.xy;
			DrawPoint( m );
		    }
		pos = m;
	   }
	else
		for ( ; bttn12(); wait(MOUSE) )
		    { 
			m = ( squaring() ) ? tweak( mouse.xy,start ) : mouse.xy;
			if ( m.x!=lastm.x || m.y!=lastm.y )
				aim( lastm=m );
			stepline();
		    }
	cursset( pos );
}

void
doit( bp,p,mode )		/* draws one brush worth at a time	*/
	register Bitmap *bp;
	Point p;
	int mode;
{
	Rectangle br;

	br = map.rect = raddp( BRUSHRECT,p );

	if ( Spraying() )
	   {
		bitblt( bp,br,&map,br.origin,F_STORE );
		spraymask.rect = br;
		texture16( &spraymask,br,shade,F_STORE );
		if ( mode==F_AND )
		   {
			rectf( &spraymask,br,F_XOR );
			mode = F_CLR;
		   }
		splatter( &spraymask,spraymask.rect );
		switch( mode )
		      {
			case F_COPY:	
			case F_STORE:	
			case F_OR:	bitblt( &spraymask,spraymask.rect,
						&map,br.origin,F_OR );
					break;
			case F_XOR:	bitblt( &spraymask,spraymask.rect,
						&map,br.origin,F_XOR );
					break;
			case F_CLR:	bitblt( &spraymask,spraymask.rect,
						&map,br.origin,F_CLR );
					break;
		      }

		bitblt( &notmask,notmask.rect,&map,br.origin,F_CLR );

	/* poke a brush shaped hole in display, then OR 'em together	*/

		bitblt( &mask,mask.rect,bp,br.origin,F_CLR );
		bitblt( &map,br,bp,br.origin,F_OR );
	   }

	else {
		texture16( &map,br,shade,F_STORE );
		bitblt( &notmask,notmask.rect,&map,br.origin,F_CLR );
		switch( mode )
		      {
			case F_OR:	bitblt( &map,br,bp,br.origin,F_OR );
					break;
			case F_COPY:	
			case F_STORE:	bitblt( &mask,mask.rect,
						bp,br.origin,F_CLR );
					bitblt( &map,br,bp,br.origin,F_OR );
					break;
			case F_XOR:	bitblt( &map,br,bp,br.origin,F_XOR );
					break;
			case F_CLR:	bitblt( &map,br,bp,br.origin,F_CLR );
					break;
			case F_AND:	bitblt( &mask,mask.rect,
						&map,br.origin,F_XOR );
					bitblt( &map,br,bp,br.origin,F_CLR );
					break;
		      }
	     }
}

static unsigned long randxx=1;

void
srandx(x)
unsigned x;
{
	randxx = x;
}

int
sparse32( n )
	Word n;
{
	unsigned long temp;
	Word i;
	temp = 0xFFFFFFFF;

	for ( i=0; i<n; i++ )
	    {
		randxx = randxx*1103515245L + 123457L;
		temp &= ( randxx&(4<<(rand()%12)) ) ? ~randxx : randxx;
	    }

	return temp;
}

#define randrect(x,y)	raddp( Rect(0,0,x,y),Pt(rand()%16,rand()%16))

splatter( bp,r )	/* splatters Bitmap bp with random junk */
	Bitmap *bp;	/* used for spray paint masking		*/
	Rectangle r;
{
	Word row,col,rrem,crem;
	Word wid,hgt;

	wid = RWIDTH(r);
	hgt = RHEIGHT(r);

	for ( row=0; row<hgt; row+=16 )
	    {
		rrem = min( (hgt-row),16 );
		for ( col=0; col<wid; col+=16 )
		    {
			crem = min( (wid-col),16 );
			bitblt( &bigmask,randrect(crem,rrem),bp,
				add( r.origin,Pt(col,row) ),F_CLR );
		    }
	    }
}


pline( bp,p0,p1,mode,flex )	/* paint a line using brush,shade,mode	*/
	Bitmap *bp;
	Point p0,p1;
	int mode;
	Word flex;
{
	short dy,dx;
	register short *minptr,*maxptr,smax,smin,incr1,incr2,err;
	Point p;
	int i;

	DashIndex = 0;			/* reset dashed line flag */
	dx = abs( p1.x-p0.x );
	dy = abs( p1.y-p0.y );

#define hack_draw( bp,p,mode,flex )	{ if (bp==&screen) \
					       reflect( p,mode,flex ); \
	     				  else doit( bp,p,mode );	 }

	if ( dx+dy<2 )
	   {
		hack_draw( bp,p0,mode,flex );
		return;
	   }
	
	if ( dx > dy )
	   {
		maxptr = &p.x;
		minptr = &p.y;
		smax = sign( p1.x-p0.x );
		smin = sign( p1.y-p0.y );
		err = 2*dy - dx;
		incr1 = 2*dy;
		incr2 = 2*( dy - dx );
	   }
	else {
		maxptr = &p.y;
		minptr = &p.x;
		smax = sign( p1.y-p0.y );
		smin = sign( p1.x-p0.x );
		err = 2*dx - dy;
		incr1 = 2*dx;
		incr2 = 2*( dx - dy );
	     }

	hack_draw( bp,p0,mode,flex );

	for ( p = p0; !eqpt( p,p1 ); )	/* p.x!=p1.x || p.y!=p1.y */
	    {
		*maxptr += smax;
		if   ( err < 0 ) err += incr1;
		else {
			*minptr += smin;
			err += incr2;
		     }
		if ( Dashing() )
		   {
			if ( DashOn() )
				hack_draw( bp,p,mode,flex );
			if ( ++DashIndex==BrushSize )
				DashIndex = 0;
		   }
		else	hack_draw( bp,p,mode,flex );
	    }
}

dolines()		/* draw linked straight lines */
{
	Point p0,last;
	Help old_set;

	DashIndex = 0;			/* set dashed line index	*/

	cursswitch( brush );
	old_set = helpswitch( point_set );	

	while( !bttn13() );

	if ( bttn1() )
	   {
		last = p0 = mouse.xy;
		cursswitch( brush );
		waitbuttons( UP );

		helpswitch( pos_set );

		while ( TRUE )
		      {
			p0 = last;
			last = rubberpt( &display,last );
			if ( bttn3() ) break;
			pline( &screen,p0,last,realmode( drawmode ),Flex.value );
			waitbuttons( UP );
		      }
	   }

	helpswitch( old_set );
}

Point
rubberpt( bp,org )		/* draw rubber band line from org 'til button	*/
	Bitmap *bp;		/* return point drawn to in D coords		*/
	Point org;
{
	Point last;

	waitbuttons( UP );
	last = mouse.xy;
	segment( bp,org,last,F_XOR );

	for ( ; !bttn123(); wait(CPU) )
		if ( !eqpt( last,mouse.xy ) )
		   {
			segment( bp,org,last,F_XOR );
			last = mouse.xy;
			segment( bp,org,last,F_XOR );
		   }
	segment( bp,org,last,F_XOR );
	return last;
}

reflect( p,mode,axes )
	Point p;
	int mode;
	int axes;
{
	Point mid;

	if ( !axes )
	   {
		doit( &screen,p,mode );
		return;	
	   }

	mid = midpt( Draw.r );

	if ( xreflect( axes ) )
	   {
		reflect( p,mode,axes&~XAXIS );
		reflect( Pt( p.x,mid.y+(mid.y-p.y) ),mode,axes&~XAXIS );
	   }
	if ( yreflect( axes ) )
	   {
		reflect( p,mode,axes&~YAXIS );
		reflect( Pt( mid.x+(mid.x-p.x),p.y ),mode,axes&~YAXIS );
	   }
}

#define NPOINTS		100

do_splines()
{
	Point parray[NPOINTS];
	Point p,newp;
	int i,n,mode;
	Help old_set;

	cursswitch( brush );
	old_set = helpswitch( point_set );	
	while( !bttn13() );

	if ( !bttn3() )
	   {
		parray[0] = parray[i=1] = p = newp = mouse.xy;

		for ( ;; wait(CPU) )
		    {
			newp = rubberpt( &display,p );
			if ( bttn3() || i == NPOINTS-2 ) break;
			mode = realmode( drawmode );
			parray[++i] = newp;
			segment( &screen,p,newp,F_XOR );
			p = newp;
		    }

		parray[i] = p;
		n = i+1;

		for ( ; --i>=0; )
		    {
			segment( &screen,p,parray[i],F_XOR );
			p = parray[i];
		    }
		helpswitch( kill_set );
		waitbuttons( UP );
		myspline( Pt(0,0),parray,n,mode );
	   }

	helpswitch( old_set );
}

myspline( offset,p,n,f )
	Point offset;
	register Point *p;
	int n, f;
{
	register long w, t1, t2, t3, scale=1000; 
	register int i, j, steps=10; 
	Point q,current;

    	if ( p != null(Point ) )
	   {
		p[0] = p[1];
		p[n] = p[n-1];
		cursinhibit();
		current = (add(offset,p[0]));
		for ( i = 0; i < n-1; i++ )
		    {
			for ( j = 0; j < steps; j++ )
			    {
				if ( bttn123() ) goto abort;
				w = scale * j / steps;
				t1 = w * w / (2 * scale);
				w = w - scale/2;
				t2 = 3*scale/4 - w * w / scale;
				w = w - scale/2;
				t3 = w * w / (2*scale);
				q.x = (t1*p[i+2].x + t2*p[i+1].x + 
					t3*p[i].x + scale/2) / scale;
				q.y = (t1*p[i+2].y + t2*p[i+1].y + 
					t3*p[i].y + scale/2) / scale;
				pline( &screen,current,add(offset,q), f,0 );
				current = add(offset,q);
			    }
		    }
abort:		cursallow();
	   }
}

