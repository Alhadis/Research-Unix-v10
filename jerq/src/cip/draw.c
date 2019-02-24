/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"
#include "line.h"
#include "layer.h"
#define DOT 2
#define DASH 10
#define ARROWwid 10
#define ARROWht 5

Rectangle saveScreenmap;
Word *saveBase;
extern Point jString ();
extern void jMoveTo ();
extern Rectangle brushes[];
struct thing addOffset();

/* This routine clips an arc within rectangle brushes[PIC] */

xarc(p0, p1, p2) 
Point p0, p1, p2;
{
  P->layer->base = addr(P->layer,brushes[PIC].origin);
  P->layer->rect = brushes[PIC];
  arc(P->layer , p0, p1, p2, F_XOR);
  P->layer->base = saveBase;
  P->layer->rect = saveScreenmap;
}


/* This routine clips a spline within rectangle brushes[PIC] */

xspline(offset,p, n) 
Point offset;
register Point *p;
int n;
{
  P->layer->base = addr(P->layer,brushes[PIC].origin);
  P->layer->rect = brushes[PIC];
  spline(offset,p, n);
  P->layer->base = saveBase;
  P->layer->rect = saveScreenmap;
}

/* This routine clips a line within rectangle brushes[PIC] */

xsegment (p, q)
Point p, q;
{
  P->layer->base = addr(P->layer,brushes[PIC].origin);
  P->layer->rect = brushes[PIC];
  segment(P->layer , p, q, F_XOR);
  PtCurrent = q;
  P->layer->base = saveBase;
  P->layer->rect = saveScreenmap;
}

draw(t,offset) 
register struct thing *t; 
Point offset; 
{
  register struct thing *s; 
  Rectangle rc; 
  Point p1,p2; 
  register int u;
  register Font *ft;

  if (t != (struct thing *) NULL) {
    cursinhibit();
    P->layer->base = addr(P->layer,brushes[PIC].origin);
    P->layer->rect = brushes[PIC];
    switch(t->type) {
      case CIRCLE: {
	circle(P->layer,add(offset,t->origin),
	       t->otherValues.radius,F_XOR);
	break;
      }
      case BOX: {
	rc = raddp(t->bb,offset);
	if (t->boorder == DOTTED) {
	  dashedBox(rc,DOT);
	}
	else {
	  if (t->boorder == DASHED) {
	    dashedBox(rc,DASH);
	  }
	  else {
	    box(rc);
	  }
	}
	break;
      }
      case ELLIPSE: {
	Ellipse(add(offset,t->origin),t->otherValues.ellipse.ht,
	    t->otherValues.ellipse.wid);
	break;
      }
      case LINE:  {
	p1 = add(t->origin,offset);
	p2 = add(t->otherValues.end,offset);
	if (t->boorder == DOTTED) {
	  dashedLine(p1,p2,DOT);
	}
	else {
	  if (t->boorder == DASHED) {
	    dashedLine(p1,p2,DASH);
	  }
	  else {
	    segment(P->layer,p1,p2,F_XOR);
	    PtCurrent = p2;
	  }
        }
	if ((t->arrow==startARROW)||(t->arrow==doubleARROW)) {
	  arrow(p2,p1);
	}
	if ((t->arrow==endARROW)||(t->arrow==doubleARROW)) {
	  arrow(p1,p2);
	}
	break;
      }
      case ARC: {
	arc (P->layer , add(offset,t->origin),
			 add(offset,t->otherValues.arc.start),
	                 add(offset,t->otherValues.arc.end),F_XOR);
	break;
      }
      case TEXT: {
	ft = t->otherValues.text.f->f;
	u = strwidth(ft,t->otherValues.text.s);
	switch (t->otherValues.text.just) {
	  case CENTER: {
	    p1 = add(offset,sub(t->origin,Pt(u>>1,0)));
	    break;
	  }
	  case LEFTJUST: {
	    p1 = add(offset,t->origin);
	    break;
	  }
	  case RIGHTJUST: {
	    p1 = add(offset,sub(t->origin,Pt(u,0)));
	    break;
	  }
	}
	string(ft,t->otherValues.text.s,P->layer,p1,F_XOR);
	break;
      }
      case SPLINE: {
	u = t->otherValues.spline.used;
	spline(offset,t->otherValues.spline.plist,u);
	if ((t->arrow==startARROW)||(t->arrow==doubleARROW)) {
	  arrow(add(offset,t->otherValues.spline.plist[2]),
	        add(offset,t->otherValues.spline.plist[1]));
	}
	if ((t->arrow==endARROW)||(t->arrow==doubleARROW)) {
	  arrow(add(offset,t->otherValues.spline.plist[u-2]),
	        add(offset,t->otherValues.spline.plist[u-1]));
	}
	break;
      }
      case MACRO: {
	if ((s=t->otherValues.list->parts) != (struct thing *)NULL) {
	  do {
	    draw(s,add(offset,t->origin));
	    s = s->next;
	  } while (s != t->otherValues.list->parts);
	}
	break;
      }
    }
    P->layer->base = saveBase;
    P->layer->rect = saveScreenmap;
    cursallow();
  }
}

xbox(r) 
Rectangle r; 
{
  xsegment(r.origin,Pt(r.corner.x,r.origin.y));
  xsegment(Pt(r.corner.x,r.origin.y),r.corner);
  xsegment(r.corner,Pt(r.origin.x,r.corner.y));
  xsegment(Pt(r.origin.x,r.corner.y),r.origin);
}

box(r) 
Rectangle r; 
{
  segment(P->layer , r.origin,Pt(r.corner.x,r.origin.y),F_XOR);
  segment(P->layer , Pt(r.corner.x,r.origin.y),r.corner,F_XOR);
  segment(P->layer , r.corner,Pt(r.origin.x,r.corner.y),F_XOR);
  segment(P->layer , Pt(r.origin.x,r.corner.y),r.origin,F_XOR);
  PtCurrent = r.origin;
}

dashedBox(r,dashsize) 
Rectangle r; 
register int dashsize;
{
  dashedLine(r.origin,Pt(r.corner.x,r.origin.y),dashsize);
  dashedLine(Pt(r.corner.x,r.origin.y),r.corner,dashsize);
  dashedLine(r.corner,Pt(r.origin.x,r.corner.y),dashsize);
  dashedLine(Pt(r.origin.x,r.corner.y),r.origin,dashsize);
}

dashedLine(s,end,dashsize) 
Point s, end; 
int dashsize;
{
  register int e, dx, dy, i, toDraw, yinc, xinc, swit;

  dx = abs(end.x - s.x);
  dy = abs(end.y - s.y);
  xinc = ((end.x-s.x)>0)? 1 : -1;
  yinc = ((end.y-s.y)>0)? 1 : -1;
  swit = (dy>dx);
  toDraw = 1;
  e = (swit)? (2*dx - dy) : (2*dy - dx);
  for (i=0; i < ((swit) ? dy : dx); i++) {
    if (i>0 && i%dashsize==0) {
      toDraw = (toDraw==1)?0:1;
    }
    if (toDraw) {
      point(P->layer,s,F_XOR);
    }
    if (e>0) {
      if (swit) {
	s.x += xinc;
      }
      else {
	s.y += yinc;
      }
      e += (swit)? (2*dx - 2*dy) : (2*dy - 2*dx);
    }
    else {
      e += (swit)? 2*dx : 2*dy;
    }
    if (swit) {
      s.y += yinc;
    }
    else {
      s.x += xinc;
    }
  }
}

int 
degrees(d)  
register int d;
{
  while (d>360) {
    d -= 360;
  }
  while (d<0) {
    d += 360;
  }
  return(d);
}

arrow(a, b)		/* draw arrow (without line) */
Point a,b; 
{
  register int alpha, rot, hyp;
  register int dx, dy;

  rot = atan2( ARROWwid / 2, ARROWht);
  hyp = norm(ARROWwid,ARROWht,0);
  alpha = atan2(b.y-a.y, b.x-a.x);
  dx = muldiv(hyp,cos(degrees(alpha + 180 + rot)),1024);
  dy = muldiv(hyp,sin(degrees(alpha + 180 + rot)),1024);
  /* line(x1+dx, y1+dy, x1, y1); */
  if ((b.x==a.x) && (b.y < a.y)) {
    dy = -dy;
  }
  cursinhibit(); 
  segment(&display,add(b,Pt(-dx,dy)),b,F_XOR); 
  cursallow();
  dx = muldiv(hyp,cos(degrees(alpha + 180 - rot)),1024);
  dy = muldiv(hyp,sin(degrees(alpha + 180 - rot)),1024);
  /* line(x1+dx, y1+dy, x1, y1); */
  if ((b.x==a.x) && (b.y < a.y)) {
    dy = -dy;
  }
  cursinhibit(); 
  segment(&display,add(b,Pt(dx,-dy)),b,F_XOR); 
  cursallow();
}

centeredText(p,s) 
Point p; 
register char *s; 
{
  jMoveTo(Pt(p.x-(jstrwidth(s)>>1),p.y)); 
  jString(s);
}

flash(b,offset) 
register Rectangle *b; 
Point offset;
{
  if (b != (Rectangle *) NULL) {
    cursinhibit();
    rectf(&display,inset(raddp(*b,offset),1),F_XOR);
    cursallow();
  }
}

flashThing(t,offset) 
register struct thing *t; 
Point offset;
{	
  if (t != (struct thing *) NULL) {
    cursinhibit();
    switch (t->type) {
      case CIRCLE:
      case BOX:
      case ELLIPSE:
      case LINE:
      case ARC:
      case SPLINE: {
	draw(t,offset);
	break;
      }
      case MACRO:
      case TEXT: {
	flash(&t->bb,offset);
	break;
      }
    }
    cursallow();
  }
}

Ellipse(p,h,w) 
Point p; 
register int h, w;
{

  ellipse(P->layer , p, w>>1, h>>1, F_XOR);
}

drawZigZag(offset,p,n) 
Point offset;  
register Point *p;  
int n; 
{
  register int i;
  register Point j, k;

  if (p != (Point *) NULL) {
    cursinhibit();
    if (n>0) {
      j = add(offset, p[1]);
      for (i=2; i<=n; i++) {
        k = add (offset, p[i]);
        xsegment(j, k);
        j = k;
      }
    }
    cursallow();
  }
}

#define SCALE (long) 1000
#define STEPS 10

spline(offset,p, n) 
Point offset; 
register Point *p;
int n;
{
  register long w, t1, t2, t3; 
  register int i, j; 
  Point q;
  Point pcurrent;		/* Current point */

  if (p != (Point *) NULL) {
    p[0] = p[1];
    p[n] = p[n-1];
    cursinhibit();
    pcurrent = add(offset,p[0]);
    for (i = 0; i < n-1; i++) {
      for (j = 0; j < STEPS; j++) {
        w = SCALE * j / STEPS;
        t1 = w * w / (2 * SCALE);
        w = w - SCALE/2;
        t2 = 3*SCALE/4 - w * w / SCALE;
        w = w - SCALE/2;
        t3 = w * w / (2*SCALE);
        q.x = (t1*p[i+2].x + t2*p[i+1].x + t3*p[i].x + SCALE/2) / SCALE;
        q.y = (t1*p[i+2].y + t2*p[i+1].y + t3*p[i].y + SCALE/2) / SCALE;
        segment(&display, pcurrent, add(offset,q), F_XOR);
        pcurrent = add(offset, q );
      }
    }
    cursallow();
  }
}
