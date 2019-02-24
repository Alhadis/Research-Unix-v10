#include "cip.h"

struct thing *
Transform(t, p) 
register struct thing *t; 
Point p;
{
  Rectangle r;
  register Point *plist;
  register int i;
  Point temp;

  if (t->type == MACRO) {
    if (p.y != 0) {
      t->bb.origin.y = p.y - t->bb.origin.y;
      t->bb.corner.y = p.y - t->bb.corner.y;
    }
    else {
      t->bb.origin.x = p.x - t->bb.origin.x;
      t->bb.corner.x = p.x - t->bb.corner.x;
    }
    t->bb = canon (t->bb.origin, t->bb.corner);
    t->origin = t->bb.origin;
    return (t);
  }

  if (p.y != 0) {		/* x-axis - change y coordinate */
    t->origin.y = p.y - t->origin.y;
  }
  else {		/* y-axis - change x coordinate */
    t->origin.x = p.x - t->origin.x;
  }
  switch(t->type) {
    case CIRCLE:
    case ELLIPSE: {
      break;
    }
    case TEXT: {
      if (p.y != 0) {
	t->origin.y -= t->otherValues.text.f->f->height;
      }
      break;
    }
    case BOX: {
      if (p.y != 0) {
	t->otherValues.corner.y = p.y - t->otherValues.corner.y;
      }
      else {
	t->otherValues.corner.x = p.x - t->otherValues.corner.x;
      }
      r = canon (t->origin, t->otherValues.corner);
      t->origin = r.origin;
      t->otherValues.corner = r.corner;
      break;
    }
    case LINE: {
      if (p.y != 0) {
	t->otherValues.end.y = p.y - t->otherValues.end.y;
      }
      else {
	t->otherValues.end.x = p.x - t->otherValues.end.x;
      }
      break;
    }
    case ARC: {
      if (p.y != 0) {
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
      plist = t->otherValues.spline.plist;
      for (i=0; i<t->otherValues.spline.used; i++) {
	if (p.y == 0) {
	  plist[i].x = p.x - plist[i].x;
	}
	else {
	  plist[i].y = p.y - plist[i].y;
	}
      }
      break;
    }
  }
  BoundingBox(t);
  return(t);
}
