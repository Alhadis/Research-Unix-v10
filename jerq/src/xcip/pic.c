#include "cip.h"

extern int numLines();
extern char fontString[];

Rectangle BBpic;

findBBpic(h) 
register struct thing *h;
{
  register struct thing *t;
  Point	p1;
  Point	p2;

  BBpic.origin.x = Xmax;  BBpic.corner.x=Xmin;
  BBpic.origin.y = YBOT-YPIC;  BBpic.corner.y=0;
  if ((t=h) != (struct thing *) NULL) {
    do {
      if( t->type == TEXT ) {

	/* Expand out the bounding box to nearest alignment points to
	   insure alignment when reading in picture. */
	p1 = alignDown( t->bb.origin );
	p2 = alignUp(   t->bb.origin );
      } else {
	p1 = t->bb.origin;
	p2 = t->bb.corner;
      }

      BBpic.origin.x = min(BBpic.origin.x,p1.x);
      BBpic.origin.y = min(BBpic.origin.y,p1.y);
      BBpic.corner.x = max(BBpic.corner.x,p2.x);
      BBpic.corner.y = max(BBpic.corner.y,p2.y);
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
  char * s;
  int	adj;

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
      /* Adjust text down a little - needed when printed - why? */
      adj = t->otherValues.text.f->f->height >> 3;
      if ( numLines(t->otherValues.text.s) > 1 ) {
	/* multi-line: use macro definition. */
        p = translate(t->bb.origin,b);
        fprintf(f, "t%d with .nw at %d,%d\n", t->otherValues.text.outName,
		p.x, p.y-adj );
      } else {
	/* single-line: use old method of simple text string. */
        i = (t->otherValues.text.f->f->height)>>1;
        p = translate(Pt(t->origin.x,t->origin.y+i+adj),b);
	s = addBackslashes(t->otherValues.text.s);

	font2string(t->otherValues.text.f->num);
	if( strlen(fontString) == 2 )
		fprintf( f, "\"\\f(%s", fontString );
	else
		fprintf( f, "\"\\f%s", fontString );

        fprintf(f,"\\s%d\\&%s\\f1\\s0\" at %d,%d%s\n",
	    t->otherValues.text.f->ps, s, p.x, p.y,
	    (t->otherValues.text.just==LEFTJUST)?" ljust" :
	    ((t->otherValues.text.just==RIGHTJUST)?" rjust":""));
      }
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
