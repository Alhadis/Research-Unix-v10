/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"

editThing(m,p,t)  
Point m;			/* Mouse location */
Point p; 			/* Offset */
register struct thing *t;	/* Thing to be edited */
{
  register int d,dx,dy,u;  
  Point o, np; 
  if (t != TNULL) {
	  o = t->origin;
    switch(t->type) {
      case CIRCLE: {
	d = norm(o.x-m.x,o.y-m.y,0);
	if (d< (t->otherValues.radius>>1)) {
	  track(m,p,MOVE,t); 
	}
	else {
	  track(m,p,GROWCIRCLE,t);
	}
	d = t->otherValues.radius;
	break;
      }
      case BOX: {
	np = t->otherValues.corner;
	dx = (np.x - o.x)>>2;
	dy = (np.y - o.y)>>2;
	if (ptinrect(m,Rect(o.x+dx,o.y+dy,np.x-dx,np.y-dy))) {
	  if (t->boorder != SOLID) {
	    draw(t,p);
	    xbox(raddp(t->bb,p));
	  }
	  t->bb = moveBox(m,t->bb,t,p);
	  t->otherValues.corner = t->bb.corner;
	  t->origin = t->bb.origin;
	  xbox(raddp(t->bb,p));
	}
	else {
	  draw(t,p);
	  t->origin.x = 0;
	  t->origin.y = 0;
	  t->bb.origin = t->origin;
	  t->bb.corner = t->origin;
	  t->otherValues.corner = t->origin;
	  if (!ptinrect(m,Rect(o.x-dx,o.y-dy,o.x+dx,o.y-dy))) {
	    if (ptinrect(m,Rect(o.x-dx,np.y-dy,o.x+dx,np.y+dy))) {
	      np.y = o.y;
	    }
	    else {
	      if (ptinrect(m,Rect(np.x-dx,np.y-dy,np.x+dx,np.y+dy))) {
	        np = o;
	      }
	      else {
		if (ptinrect(m,Rect(np.x-dx,o.y-dy,np.x+dx,o.y+dy))) {
		  np.x = o.x;
		}
	      }
	    }
	  }
	  o = track(np,p,BOX,t);
	  t->origin.x = min(o.x,np.x);
	  t->origin.y = min(o.y,np.y);
	  t->otherValues.corner.x = max(o.x,np.x);
	  t->otherValues.corner.y = max(o.y,np.y);
	}		
	break;
      }
      case ELLIPSE: {
	dx = abs(m.x - o.x);  
	dy = abs(m.y - o.y);
	d = norm(dx,dy,0);
	if ((dx > dy) && (d > (t->otherValues.ellipse.wid>>2))) {
	  track(m,p,GROWEWID,t); 
	}
	else {
	  if ((dx < dy) && (d > (t->otherValues.ellipse.ht>>2))) {
	    track(m,p,GROWEHT,t); 
	  }
	  else {
	    track(m,p,MOVE,t); 
	  }
	}
	break;
      }
      case LINE: {
	np=t->otherValues.end;
	draw(t,p);
	if (distance(m,o)<distance(m,np)) {
	  t->origin = track(np,p,LINE,t);
	}
	else {
	  t->otherValues.end = track(o,p,LINE,t);
	}
	break;
      }
      case ARC: {
	d = (distance(o,t->otherValues.arc.start))>>1;
	if (distance(o,m)<d)  {
	  arcOrigin(t,p);
	}
	else {
	  if (distance(m,t->otherValues.arc.start)<
	      distance(m,t->otherValues.arc.end)) {
		arcStart(t,p);
	  }
	  else {
	    arcEnd(t,p);
	  }
	}
	break;
      }
      case MACRO: {
	draw(t,p);
	xbox(raddp(t->bb,p));
	t->bb = moveBox(m,t->bb,t,p);
	t->origin = t->bb.origin;
	xbox(raddp(t->bb,p));
	break;
      }
      case TEXT: {
	track(m,p,MOVE,t);
	break;
      }
      case SPLINE: {
	u = t->otherValues.spline.used;
	d=findNearestPoint(m,t->otherValues.spline.plist,u);
	if ((t->arrow==startARROW)||(t->arrow==doubleARROW)) {
	  arrow(add(p,t->otherValues.spline.plist[2]),
	        add(p,t->otherValues.spline.plist[1]));
	}
	if ((t->arrow==endARROW)||(t->arrow==doubleARROW)) {
	  arrow(add(p,t->otherValues.spline.plist[u-2]),
	        add(p,t->otherValues.spline.plist[u-1]));
	}
	if ((d == u-1) || (d==1)) {
	  xsegment
	      (add(p,t->otherValues.spline.plist[(d==1)?1:(d-1)]),
	       add(p,t->otherValues.spline.plist[(d==1)?2:d]));
	  np=track(t->otherValues.spline.plist[(d==1)?2:(d-1)],
	      p,(d==1)?REVLINE:LINE,t);	/* See note 1 in track.c */
	}
	else {
	  np=track2(p,t->otherValues.spline.plist[d-1],
	              t->otherValues.spline.plist[d+1],
	              t->otherValues.spline.plist[d]);
	}
	xspline(p,t->otherValues.spline.plist,u);
	t->otherValues.spline.plist[d]=np;
	t->origin=t->otherValues.spline.plist[1];
	break;
      }
    } /* end switch */
    BoundingBox(t);
    switch (t->type) {
      case TEXT:
      case CIRCLE:
      case ARC:
      case ELLIPSE: {
	break;
      }
      default: {
	draw(t,p);	
	break;
      }
    }
  }
} 
    
struct thing *
copyThing(t,p,offset) 
register struct thing *t; 
Point p, offset;
{
  register struct thing *c; 
  Point *pl; 
  int i, n; 

  if (t != TNULL) {
    switch(t->type) {
      case CIRCLE: {
	if ((c = newCircle(p)) != TNULL) {
	  c->otherValues.radius = t->otherValues.radius;
	}
	break;
      }
      case BOX: {
	if ((c=newBox(Rpt(p,add(p,sub(t->bb.corner,t->origin))))) 
	      != TNULL) {
	  c->boorder = t->boorder;
	}
	break;
      }
      case ELLIPSE: {
	if ((c = newEllipse(p)) != TNULL) {
	  c->otherValues = t->otherValues;
	}
	break;
      }
      case LINE: {
	if ((c = newLine(p,add(p,sub(t->otherValues.end,t->origin))))
	       != TNULL) {
	  c->boorder = t->boorder;
	  c->arrow = t->arrow;
	}
	break;
      }
      case ARC: {
	c = newArc(p,add(p, sub(t->otherValues.arc.end,
				t->otherValues.arc.start)));
	break;
      }
      case SPLINE: {
	n = t->otherValues.spline.used;
	c = TNULL;
	if ((pl = (Point *) getSpace ((n+2)*sizeof(Point)))
	     != (Point *)NULL){
	  p = sub(p, t->origin);
	  for (i=1; i<=n; i++) {
	    pl[i]=add(p,t->otherValues.spline.plist[i]);
	  }
	  c = newSpline(n,n,pl);		
	}
	break;
      }
      case TEXT: {
	if ((c = newText(p,t->otherValues.text.s)) != TNULL) {
	  c->otherValues.text.f = t->otherValues.text.f;
	  c->otherValues.text.just = t->otherValues.text.just;
	}
	break;
      }
      case MACRO: {
	c = newMacro(p,t->otherValues.list);
	break;
      }
    }
    if (c == TNULL) {
      return (c);
    }
    BoundingBox(c);
    draw(c,offset);
    return(c);
  }
  else {
    return(t);
  }
}

/* Returns the index of the point, in plist p, closest to point */
/* given by o. */

int 
findNearestPoint(o,p,n)
register Point *p;	/* Plist of spline points */
int n;			/* Number of points in Plist */
Point o;		/* Mouse location */
{
  register int i; 
  int f;		/* Plist index to point closest to o */
  int d;		/* Distance between point and o */
  int mind = -1;	/* Minimum distance */

  if (p != (Point *) NULL) {
    for (i=1; i<=n; i++) {
      d = norm(o.x-p[i].x, o.y-p[i].y, 0);
      if ((mind<0) || (mind>d)) {
	mind = d;
	f = i;
      }
    }
    return(f);
  }
  else {
    return(-1);
  }
}
