/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"

Rectangle BBpic;

findBBpic(h) 
register struct thing *h;
{
  register struct thing *t;

  BBpic.origin.x = Xmax;  BBpic.corner.x=Xmin;
  BBpic.origin.y = YBOT-YPIC;  BBpic.corner.y=0;
  if ((t=h) != (struct thing *) NULL) {
    do {
      BBpic.origin.x = min(BBpic.origin.x,t->bb.origin.x);
      BBpic.origin.y = min(BBpic.origin.y,t->bb.origin.y);
      BBpic.corner.x = max(BBpic.corner.x,t->bb.corner.x);
      BBpic.corner.y = max(BBpic.corner.y,t->bb.corner.y);
      t = t->next;
    } while (t != h);
  }
}

Point 
translate(p,b) 
Point p; 
Rectangle b;
{
  return(sub(Pt(p.x,b.corner.y),Pt(b.origin.x,p.y)));
}

writePIC(t,f,b) 
register struct thing *t; 
FILE *f; 
Rectangle b;
{
  Point p, q, r;   
  register int i;

  switch (t->type) {
    case CIRCLE: {
      p = translate(t->origin,b);
      fprintf(f,"circle rad %d at %d,%d\n",
	  t->otherValues.radius,p.x,p.y);
      break;
    }
    case BOX: {
      p = translate(t->bb.origin,b);
      fprintf(f,"box ht %d wid %d with .nw at %d,%d %s\n",
	  t->bb.corner.y - t->bb.origin.y,
	  t->bb.corner.x - t->bb.origin.x,p.x,p.y,
	  (t->boorder==DOTTED) ? "dotted" :
	  ((t->boorder==DASHED)?"dashed":""));
      break;
    }
    case ELLIPSE: {
      p= translate(t->origin,b);
      fprintf(f,"ellipse ht %d wid %d at %d,%d\n",
	  t->otherValues.ellipse.ht,
	  t->otherValues.ellipse.wid,p.x,p.y);
      break;
    }
    case LINE: {
      p = translate(t->origin,b);
      q = translate(t->otherValues.end,b);
      fprintf(f,"line "); 
      switch (t->arrow) {
	case startARROW: {
	  fprintf(f,"<-");
	  break; 
	}
	case endARROW:  {
	  fprintf(f,"->");
	  break;
	}
	case doubleARROW: {
	  fprintf(f,"<->");
	  break;
	}
      }
      fprintf(f," from %d,%d to %d,%d ",p.x,p.y,q.x,q.y);
      if (t->boorder==DOTTED) {
	fprintf(f,"dotted"); 
      }
      else {
	if (t->boorder==DASHED) {
	  fprintf(f,"dashed");
	}
      }
      fprintf(f,"\n");
      break;
    }
    case ARC: {
      p = translate(t->otherValues.arc.start,b);
      q = translate(t->otherValues.arc.end,b);
      r = translate(t->origin,b);
      fprintf(f,"arc from %d,%d to %d,%d at %d,%d\n",
	  p.x,p.y,q.x,q.y,r.x,r.y);
      break;
    }
    case TEXT: {
      i = (t->otherValues.text.f->f->height)>>1;
      p = translate(Pt(t->origin.x,t->origin.y+i),b);
      fprintf(f,"\"\\f%d\\s%d\\&%s\\f1\\s0\" at %d,%d%s\n",
	  t->otherValues.text.f->num,t->otherValues.text.f->ps,
	  t->otherValues.text.s, p.x, p.y,
	  (t->otherValues.text.just==LEFTJUST)?" ljust" :
	  ((t->otherValues.text.just==RIGHTJUST)?" rjust":""));
      break;
    }
    case SPLINE: {
      fprintf(f,"spline ");
      switch (t->arrow) {
	case startARROW: {
	  fprintf(f,"<-");
	  break; 
	}
	case endARROW:  {
	  fprintf(f,"->");
	  break;
	}
	case doubleARROW: {
	  fprintf(f,"<->");
	  break;
	}
      }
      for (i=1; i<t->otherValues.spline.used; i++) {
	p = translate(t->otherValues.spline.plist[i],b);
	if (i==1) {
	  fprintf(f," from %d,%d",p.x,p.y);
	}
	else {
	  fprintf(f,"\\\nto %d,%d",p.x,p.y);
	}
      }
      fprintf(f,"\n");
      break;
    }
    case MACRO: {
      p = translate(t->origin,b);
      fprintf(f,"m%d with .nw at %d,%d\n",
	         t->otherValues.list->outName,p.x,p.y);
      break;
    }
  }
}
