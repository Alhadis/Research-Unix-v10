/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"

struct thing *
reflect(master,p) 
register struct thing *master; 
Point p;
{
  register struct thing *t; 
  register Point *plist;  
  register int i;
  Point temp;

  if ((t = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    *t = *master;
    if (p.y!=0) {		/* x-axis - change y coordinate */
      t->origin.y = p.y - t->origin.y;
    }
    else {		/* y-axis - change x coordinate */
      t->origin.x = p.x - t->origin.x;
    }
    switch(t->type) {
      case CIRCLE:
      case ELLIPSE:
      case TEXT: {
	break;
      }
      case BOX: {
	if (p.y!=0) {
	  t->otherValues.corner.y = p.y - t->otherValues.corner.y;
	}
	else {
	  t->otherValues.corner.x = p.x - t->otherValues.corner.x;
	}
	break;
      }
      case LINE: {
	if (p.y!=0) {
	  t->otherValues.end.y = p.y - t->otherValues.end.y;
	}
	else {
	  t->otherValues.end.x = p.x - t->otherValues.end.x;
	}
	break;
      }
      case ARC: {
	if (p.y!=0) {
	  t->otherValues.arc.start.y = p.y - t->otherValues.arc.start.y;
	  t->otherValues.arc.end.y = p.y - t->otherValues.arc.end.y;
	}
	else {
	  t->otherValues.arc.start.x = p.x - t->otherValues.arc.start.x;
	  t->otherValues.arc.end.x = p.x - t->otherValues.arc.end.x;
	}
	temp = t->otherValues.arc.start;
	t->otherValues.arc.start = t->otherValues.arc.end;
	t->otherValues.arc.end = temp;
	break;
      }
      case SPLINE: {
	if ((plist = (Point *) getSpace((t->otherValues.spline.used+3)
				*sizeof(Point))) != (Point *) NULL) {
	  for (i=0; i<t->otherValues.spline.used+3; i++) {
	    plist[i] = t->otherValues.spline.plist[i];
	    if (p.y==0) {
	      plist[i].x = p.x - plist[i].x;
	    }
	    else {
	      plist[i].y = p.y - plist[i].y;
	    }
	  }
	  t->otherValues.spline.plist = plist;
	}
	break;
      }
      case MACRO: {
	break;
      }
    }
    BoundingBox(t);
    return(t);
  }
  return (master);
}
