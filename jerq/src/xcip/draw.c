#include "cip.h"

#ifdef DMD630
/*
 * Definitions for globals involved with lines
 */
#ifdef LINE_C

short Jdmajor;		/* Delta for major direction */
short Jdminor;		/* Delta for minor direction */
short Jxmajor;	/* flag: is x the major direction? */
short Jslopeneg;	/* flag: is slope of line negative? */
Point Jsetline();
#undef PtCurrent
Point PtCurrent;
#define PtCurrent SPtCurrent

#else

extern short Jdmajor;		/* Delta for major direction */
extern short Jdminor;		/* Delta for minor direction */
extern short Jxmajor;	/* flag: is x the major direction? */
extern short Jslopeneg;	/* flag: is slope of line negative? */
extern Point Jsetline();

#endif
#else
#include "line.h"
#endif

#include "layer.h"

#define DOT 2
#define DASH 10
#define ARROWwid 10
#define ARROWht 10

#ifndef ONEDOTOH
Point PointCurr;
#endif
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
  PointCurr = q;
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
	    PointCurr = p2;
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
	drawText(add(offset,t->origin),
		 t->otherValues.text.s,
		 t->otherValues.text.just,
		 t->otherValues.text.spacing,
		 t->otherValues.text.f->f );
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
  PointCurr = r.origin;
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
    rectf( &display, raddp(*b,offset), F_XOR );
    cursallow();
  }
}


flashThing(t,offset) 
  register struct thing *t; 
  Point offset;
{	
  Rectangle r;
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
	/* Must add Pt(1,1) to corner to fill box fully - why ??? */
	r.origin = t->bb.origin;
	r.corner = add( t->bb.corner , Pt(1,1) );
	flash( &r , offset );
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
	Point		offset;  
	register Point *p;  
	int		n; 
{
	register int	i;
	Point		j;
	Point		k;

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


Bitmap	tempBitmap = { 0, 1, 0, 0, 16, 16, 0 };

#ifndef DMD630
Word	tempIcon[16];
#endif


drawIcon(axy,icon)
	Point			axy;
	register Texture16 *	icon;
{
#ifdef DMD630
	tempBitmap.base = icon->bits;
#else
	register int	i;
	register Word	word;

	for( i=0; i<15; i++) {
		word = icon->bits[i];
		tempIcon[i] = word << 16;
	}
	tempBitmap.base = tempIcon;
#endif
	bitblt(&tempBitmap, tempBitmap.rect, P->layer, 
		sub(axy, Pt(8,8)), F_XOR);
}


/* Copyright icon */
Texture16 copyright = {
	 0x0000, 0x0380, 0x0C60, 0x1010,
	 0x2388, 0x2448, 0x4404, 0x4404,
	 0x4404, 0x2448, 0x2388, 0x1010,
	 0x0C60, 0x0380, 0x0000, 0x0000,
};


drawCopyright( pt )
	Point pt;
{
	pt = string(&defont,"Copyright",&display,pt,F_XOR);
	drawIcon( add(pt,Pt(11,7)), &copyright );
	string(&defont,"1985 AT&T",&display,add(pt,Pt(24,0)),F_XOR);
	string(&defont,"All Rights Reserved",&display,add(pt,Pt(-74,18)),F_XOR);
}


/* CIP logo */

typedef struct {
	Point	start;
	Point	end;
} Logo_Line;

/* Insert line data convert from logo.pic file. */
Logo_Line	logo[] = {
#include "logo.data"
-1,-1,-1,-1
};

drawToolIcon()
{

	register Logo_Line *	line;

	jmoveto( Pt(40,25) );
	jstring("CIP");

	for( line = &logo[0]; line->start.x != -1; line++ ) {

#ifdef DMD630
		jsegment( Pt( 100 + line->start.x , 900 - line->start.y ), 
		          Pt( 100 + line->end.x   , 900 - line->end.y   ), 
				  F_STORE );
#else
		jsegment( Pt( 25 + line->start.x , 900 - line->start.y ), 
		          Pt( 25 + line->end.x   , 900 - line->end.y   ), 
				  F_STORE );
#endif

	}
}
