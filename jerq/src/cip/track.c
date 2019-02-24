/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"

extern Rectangle brushes[];

Point 
track(p,offset,b,th) 
Point p,offset; int b; 
register struct thing *th;
{
  Point t, ot, r;

  if ( b != ARC ) {
    p = add(p,offset);
    ot = p;
  }
  else {
    ot = add(p,offset);		
  }
  if (th != TNULL || b==BOX || b==LINE || b==REVLINE || b==ARC) {
    do {
      t = MOUSE_XY;
      if ((t.x!=ot.x)||(t.y!=ot.y)) {
	cursinhibit();
	switch (b) {
	  case BOX: {
	    r = near(t,th,offset);
	    if (r.x != 0) {
	      t = r;
	    }
	    else {
	      r=near(Pt(p.x,t.y),th,offset);
	      if (r.x!=0) {
		t.y = r.y;
	      }
	      else {
		r=near(Pt(t.x,p.y),th,offset);
		if (r.x != 0) {
		  t.x = r.x;
		}
	      }
	    }
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
	    th->origin=computeArcOrigin( p,sub( t,offset ));
	    draw(th,offset);	
	    break;
	  }
	  case REVLINE:
	  case LINE: {
	    r=near(t,th,offset);
	    if (r.x!=0) {
	      t=r;
	    }
	    else {
	      if (abs(p.x-t.x)<nearlyStraight) {
		t.x=p.x;
	      }
	      else {
		if (abs(p.y-t.y)<nearlyStraight) {
		  t.y=p.y;
		}
	      }
	    }
	    if (b== LINE) {	/* See note 1 */
	      xsegment(p,ot);
	      xsegment(p,t);
	    }
	    else {
	      xsegment(ot,p);
	      xsegment(t,p);
	    }
	    break;
	  }
	  case GROWCIRCLE: {
	    draw(th,offset);
	    th->otherValues.radius=distance(add(th->origin,offset),t);
	    draw(th,offset);
	    break;
	  }
	  case MOVE: {
	    r=near(t,th,offset);
	    if (r.x != 0) {
	      t=r;
	    }
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
	cursallow();
      }
      nap(2);
    } while (button2());
  }
  if ( b == ARC ) {
    draw(th,offset);
  }
  cursinhibit();
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
    p = MOUSE_XY;
    cursinhibit();
    if ((p.x!=op.x)||(p.y!=op.y)) {
      xsegment(o1,op);
      xsegment(op,o2);
      xsegment(o1,p);
      xsegment(p,o2);
    }
    op = p;
    cursallow();
    nap(2);
  } while (button2());
  cursinhibit ();
  cursallow ();
  return(sub(p,offset));
}

Rectangle
moveBox(p,r,h,offset)
Point p, offset; 
Rectangle r; 
register struct thing *h;
{
  Rectangle or; Point dor, dc; Point op, q;

  or = r;
  dor= sub(or.origin,p);
  dc = sub(or.corner,p);
  op = add(p,offset);
  do {
    p = MOUSE_XY;	
    r.origin = add(or.origin,sub(p,op));
    q = near(add(r.origin,offset),h,offset);
    if (q.x!=0) {
      p = sub(q,dor);
    }
    else {
      r.corner = add(or.corner,sub(p,op));
      q = near(add(r.corner,offset),h,offset);
      if (q.x!=0) {
	p = sub(q,dc);
      }
    }
    r.origin = add(or.origin,sub(p,op));
    r.corner = add(or.corner,sub(p,op));
    cursinhibit();
    xbox(raddp(or,offset));
    xbox(raddp (r, offset));
    op = p;
    or = r;
    cursallow();
    nap(2);
  } while (button2()); 
  return(r);
}
    
arcOrigin(t,offset) 
register struct thing *t; 
Point offset;
{
  Point oc, c, c1, c2, s, e, om, m, org, mid; 

  s = add(offset,t->otherValues.arc.start);
  e = add(offset,t->otherValues.arc.end);
  org = add(offset,t->origin);
  mid = sub(org,div(add(s,e),2));
  oc = org;
  om = oc;
  do {
    cursinhibit();
    m = MOUSE_XY;
    if (distance(m,om)>2) {
      c1.x = m.x;
      c1.y = muldiv(mid.y, c1.x-org.x, mid.x) + org.y;
      c2.y = m.y;
      c2.x = muldiv(c2.y-org.y, mid.x,mid.y) + org.x;
      c = (distance(oc,c1)<distance(c,c2))?c1:c2;
      eraseAndDrawArc(oc,e,s,c,e,s);
      oc = c;
      om = m;
      t->origin = sub(c,offset);
    }
    cursallow();
    nap(2);
  } while (button2());
}

arcStart(t,offset) 
register struct thing *t; 
Point offset;
{
  Point oc, s, e, os; 

  os = add(offset,t->otherValues.arc.start);
  e = add(offset,t->otherValues.arc.end);
  oc = add(offset,t->origin);
  do {
    cursinhibit();
    s = MOUSE_XY;
    if (distance(s,os)>2) {
      eraseAndDrawArc(oc,e,os,oc,e,s);
      os = s;
      t->otherValues.arc.start=sub(s,offset);
    }
    cursallow();
    nap(2);
  } while (button2());
}

arcEnd(t,offset) 
register struct thing *t; 
Point offset;
{
  Point oc, s, e, oe; 

  s = add(offset,t->otherValues.arc.start);
  oe = add(offset,t->otherValues.arc.end);
  oc = add(offset,t->origin);
  do {
    cursinhibit();
    e = MOUSE_XY;
    if (distance(e,oe)>2) {
      eraseAndDrawArc(oc,oe,s,oc,e,s);
      oe = e;
      t->otherValues.arc.end=sub(e,offset);
    }
    cursallow();
    nap(2);
  } while (button2());
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
