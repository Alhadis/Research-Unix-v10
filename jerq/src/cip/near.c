/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"

extern int gridState;

Point 
near(p,h,offset) 
Point p, offset; 
struct thing *h;
{
  register struct thing *t;  
  register int g; 
  register Point *s;
  Point stickyPoint[9], found, q; 

  p = sub(p,offset);
  found.x=0;  found.y=0; 
  t = (h==(struct thing *)NULL)?(struct thing *)NULL:h->next;
  if (t != h) {
    do {
      if (ptinrect(p,inset(t->bb,-nearPT))) {
	s = stickyPoint;
	if (t->type!=ARC) {
	  *s++ = t->origin;
	}
	switch(t->type) {
	  case LINE: {
	    *s++ = t->otherValues.end;
	    *s++ = div(add(t->origin,t->otherValues.end),2);
	    break;
	  }
	  case MACRO:
	  case BOX: {
	    *s++ = t->bb.corner;
	    s->x = t->origin.x; 
	    s->y = t->bb.corner.y;
	    s++;
	    s->x = t->bb.corner.x;
	    s->y = t->origin.y;
	    s++;
	    if (t->type==BOX) {
	      s->x = t->origin.x;
	      s->y = (t->origin.y + t->bb.corner.y)/2;
	      s++;
	      s->x = (t->origin.x + t->bb.corner.x)/2;
	      s->y = t->origin.y;
	      s++;
	      s->x = t->bb.corner.x;
	      s->y = (t->origin.y + t->bb.corner.y)/2;
	      s++;
	      s->x = (t->origin.x + t->bb.corner.x)/2;
	      s->y = t->bb.corner.y;
	      s++;
	      *s++ = div(add(t->origin,t->bb.corner),2);
	    }
	    break;
	  }
	  case CIRCLE: {
	    q.x = 0;
	    q.y = t->otherValues.radius;
	    *s++ = add(t->origin,q);
	    *s++ = sub(t->origin,q);
	    *s++ = add(t->origin,Pt(q.y,q.x));
	    *s++ = sub(t->origin,Pt(q.y,q.x));
	    break;
	  }
	  case ELLIPSE: {
	    q.y = t->otherValues.ellipse.ht>>1;
	    q.x = t->otherValues.ellipse.wid>>1;
	    *s++ = add(t->origin,Pt(0,q.y));
	    *s++ = sub(t->origin,Pt(0,q.y));
	    *s++ = add(t->origin,Pt(q.x,0));
	    *s++ = sub(t->origin,Pt(q.x,0));
	    break;
	  }
	  case SPLINE: {
	    *s++ = 
	    t->otherValues.spline.plist[t->otherValues.spline.used];
	    break;
	  }
	  case ARC: {
	    *s++ = t->otherValues.arc.start;
	    *s++ = t->otherValues.arc.end;
	    break;
	  }
	}
	for (g=0; g<(s-stickyPoint); g++) {
	  if (distance(p,stickyPoint[g])<nearPT) {
	    found = stickyPoint[g];
	  }
	}
      }
      if (found.x==0) {
	t = t->next;
      }
    } while ((t != h)&&(found.x==0));
  }
  if ((found.x==0) && (gridState==GRIDon)) {
    found = sub(mul(div(add(p,offset),8),8),offset);
  }
  return( (found.x==0 && found.y==0) ? found : add(found,offset) );
}
