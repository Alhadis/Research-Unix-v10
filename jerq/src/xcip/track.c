#include "cip.h"

extern Rectangle brushes[];


/* Rounds the given integer based on currAlignment. */
	int 
alignInt(i) 
	int i;
{
	if( currAlignment > 0 ) {
		if( i >= 0 ) {
			return ( ( (i+(currAlignment>>1)) / currAlignment) 
				* currAlignment);
		} else {
			return ( ( (i-(currAlignment>>1)) / currAlignment) 
				* currAlignment);
		}

	} else {
		return( i );
	}
}


	int 
alignByInt(i, value ) 
	register int	i;
	register int	value;
{
	if( i >= 0 ) {
		return ( ( (i+value) / currAlignment) * currAlignment);
	} else {
		return ( ( (i-value) / currAlignment) * currAlignment);
	}
}


/* Rounds the point given based on currAlignment. */
	Point 
align(p) 
	Point p;
{
	if( currAlignment > 0 ) {
		p.x = alignInt(p.x);
		p.y = alignInt(p.y);
	}
	return p;
}


	Point 
alignUp(p) 
	Point p;
{
	if( currAlignment > 0 ) {
		p.x = alignByInt( p.x, currAlignment-1 );
		p.y = alignByInt( p.y, currAlignment-1 );
	}
	return( p );
}


	Point 
alignDown(p) 
	Point p;
{
	if( currAlignment > 0 ) {
		p.x = alignByInt( p.x, 0 );
		p.y = alignByInt( p.y, 0 );
	}
	return( p );
}


  Point 
track(p,offset,b,th) 
  Point p,offset; int b; 
  register struct thing *th;
{
  Point t, ot;

  cursinhibit();
  if ( b != ARC ) {
    p = add(p,offset);
    ot = p;
  }
  else {
    ot = add(p,offset);
  }
  if (th != TNULL || b==BOX || b==LINE || b==REVLINE || b==ARC) {
    do {
      t = add( align( sub(MOUSE_XY,offset) ), offset );
      if ((t.x!=ot.x)||(t.y!=ot.y)) {
	switch (b) {
	  case BOX: {
	    xbox (canon (p, ot));
	    xbox (canon (p, t));
	    break;
	  }
	  case ARC: {
	    th->type = ARC;
	    th->otherValues.arc.start = p;
	    th->otherValues.arc.end = sub( ot,offset );
	    th->origin=computeArcOrigin( p, sub( ot,offset ));
	    if (!eqpt(th->otherValues.arc.start,
		      th->otherValues.arc.end)) {
	      draw(th,offset);	
	    }
	    th->otherValues.arc.end = sub( t,offset );
	    th->origin=computeArcOrigin( p, sub( t,offset ));
	    draw(th,offset);	
	    break;
	  }
	  case LINE: {
	    xsegment(p,ot);
	    xsegment(p,t);
	    break;
	  }
	  case REVLINE: {	/* See note 1 */
	    xsegment(ot,p);
	    xsegment(t,p);
	    break;
	  }
	  case GROWCIRCLE: {
	    draw(th,offset);
	    th->otherValues.radius=alignInt(distance(add(th->origin,offset),t));
	    draw(th,offset);
	    break;
	  }
	  case MOVE: {
	    draw(th,offset);
	    th->origin = sub(t,offset);
	    draw(th,offset);
	    break;	
	  }
	  case GROWEHT: {
	    draw(th,offset);
	    th->otherValues.ellipse.ht =  
		abs(th->origin.y-t.y+offset.y)<<1;
	    draw(th,offset);
	    break;
	  }
	  case GROWEWID: {
	    draw(th,offset);
	    th->otherValues.ellipse.wid = 
		abs(th->origin.x-t.x+offset.x)<<1;
	    draw(th,offset);
	    break;
	  }
	  case ELLIPSE: {
	    draw(th,offset);
	    th->otherValues.ellipse.wid = 
		(abs(th->origin.x-t.x+offset.x))<<1;
	    th->otherValues.ellipse.ht =  
		abs(th->origin.y-t.y+offset.y)<<1;
	    draw(th,offset);
	    break;
	  }
	}
	ot = t;
      }
      nap(2);
    } while (button2());
  }
  if ( b == ARC ) {
    draw(th,offset);
  }
  switch (b) {
    case BOX: {
      xbox (canon (p, ot));
      break;
    }
/*  case REVLINE:	/* REVLINE not needed, used only with splines */
    case LINE: {
      /* Don't draw line if tracking a spline */
      if (th->type != SPLINE) {
	xsegment(p,ot);
      }
      break;
    }
  }
  cursallow();
  return(sub(t,offset));
}


/* NOTE 1: */
/* Lines drawn and undrawn using XOR mode must always be drawn in the */
/* same direction.  This is because the last point on the line is not */
/* drawn.  The same line drawn twice, but in opposite directions */
/* will leave two points on the screen.  So, because of this when */
/* the tracking routine is tracking a spline from p[2] to p[1] the */
/* line must be drawn in the opposite direction (from p[1] to p[2]). */
/* Therefore, the need for the REVLINE value. */


  Point 
track2(offset,o1, o2, p) 
  Point offset, o1, o2, p;
{
  Point op;

  o1 = add(offset,o1);
  o2 = add(offset,o2);
  op = add(offset,p);
  do {
    cursinhibit ();
    p = add( align( sub(MOUSE_XY,offset) ), offset );
    if ((p.x!=op.x)||(p.y!=op.y)) {
      xsegment(o1,op);
      xsegment(op,o2);
      xsegment(o1,p);
      xsegment(p,o2);
    }
    op = p;
    cursallow ();
    nap(2);
  } while (button2());
  return(sub(p,offset));
}


	Rectangle
moveBox(p,r,offset)
	Point		p;
	Point		offset; 
	Rectangle	r; 
{
	Point		op;
	Rectangle	or; 

	cursinhibit();
	or = r;
	op = p;
	do {
		p = sub(MOUSE_XY,offset);	
		r.origin = align( add(or.origin,sub(p,op)) );

		if( !eqpt( r.origin, or.origin ) ) {

			r.corner = add( or.corner, sub(r.origin,or.origin) );

			xbox( raddp(or,offset) );
			xbox( raddp(r, offset) );

			op = p;
			or = r;
		}

		nap(2);

	} while (button2()); 

	cursallow();
	return(r);
}

    
arcOrigin(t,offset) 
  register struct thing *t; 
  Point offset;
{
  Point oc, c, c1, c2, s, e, om, m, org, mid; 

  cursinhibit();
  s = add(offset,t->otherValues.arc.start);
  e = add(offset,t->otherValues.arc.end);
  org = add(offset,t->origin);
  mid = sub(org,div(add(s,e),2));
  oc = org;
  om = oc;
  do {
    m = add( align( sub(MOUSE_XY,offset) ), offset );
    if (distance(m,om)>2) {
      c1.x = m.x;
      /* max function prevents division by 0 */
      c1.y = muldiv(mid.y, c1.x-org.x, max(mid.x,1)) + org.y;
      c2.y = m.y;
      c2.x = muldiv(c2.y-org.y, mid.x, max(mid.y,1)) + org.x;
      c = (distance(oc,c1)<distance(c,c2))?c1:c2;
      eraseAndDrawArc(oc,e,s,c,e,s);
      oc = c;
      om = m;
      t->origin = sub(c,offset);
    }
    nap(2);
  } while (button2());
  cursallow();
}


arcStart(t,offset) 
  register struct thing *t; 
  Point offset;
{
  Point oc, s, e, os; 

  cursinhibit();
  os = add(offset,t->otherValues.arc.start);
  e = add(offset,t->otherValues.arc.end);
  oc = add(offset,t->origin);
  do {
    s = add( align( sub(MOUSE_XY,offset) ), offset );
    if (distance(s,os)>2) {
      eraseAndDrawArc(oc,e,os,oc,e,s);
      os = s;
      t->otherValues.arc.start=sub(s,offset);
    }
    nap(2);
  } while (button2());
  cursallow();
}


arcEnd(t,offset) 
  register struct thing *t; 
  Point offset;
{
  Point oc, s, e, oe; 

  cursinhibit();
  s = add(offset,t->otherValues.arc.start);
  oe = add(offset,t->otherValues.arc.end);
  oc = add(offset,t->origin);
  do {
    e = add( align( sub(MOUSE_XY,offset) ), offset );
    if (distance(e,oe)>2) {
      eraseAndDrawArc(oc,oe,s,oc,e,s);
      oe = e;
      t->otherValues.arc.end=sub(e,offset);
    }
    nap(2);
  } while (button2());
  cursallow();
}


eraseAndDrawArc(oc,oe,os,c,e,s) 
Point oc, oe, os, c, e, s;
{
  xarc(oc,os,oe);
  xsegment(oc,os);
  xsegment(oc,oe);
  xarc(c,s,e);
  xsegment(c,s);
  xsegment(c,e);
}
