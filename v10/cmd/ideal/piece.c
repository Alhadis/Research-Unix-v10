#include "ideal.h"
#include "y.tab.h"

void linecall (linefax)
LINEPTR linefax;
{
	printf ("...line %g %g %g %g\n",
		linefax->x0, linefax->y0,
		linefax->x1, linefax->y1
	);
}

void circcall (circfax)
CIRCPTR circfax;
{
	printf ("...circle %g %g %g\n",
	circfax->x0, circfax->y0, circfax->r
	);
}

void arccall (arcfax)
ARCPTR arcfax;
{
	printf ("...arc %g %g %g %g %g %g %g %g %g\n",
	arcfax->x0, arcfax->y0,
	arcfax->x1, arcfax->y1,
	arcfax->x2, arcfax->y2,
	arcfax->theta1, arcfax->theta2,

	fabs(arcfax->radius)
	);
}

void textcall (textfax)
TEXTPTR textfax;
{
	switch (textfax->command) {
	case LEFT:
		printf ("...left %g %g '%s\n",
			textfax->x0,
			textfax->y0,
			textfax->string
		);
		break;
	case CENTER:
		printf ("...center %g %g '%s\n",
			textfax->x0,
			textfax->y0,
			textfax->string
		);
		break;
	case RIGHT:
		printf ("...right %g %g '%s\n",
			textfax->x0,
			textfax->y0,
			textfax->string
		);
		break;
	default:
		fprintf (stderr, "ideal: textcall: can't happen\n");
		break;
	}
}

void splcall (knotlist)
EXPRPTR knotlist;
{
	printf ("...spline %g %g\n",
		Re(((INTLPTR) knotlist->expr)),
		Im(((INTLPTR) knotlist->expr))
	);
	knotlist = knotlist->next;
	while (knotlist) {
		printf ("...knot %g %g\n",
			Re(((INTLPTR) knotlist->expr)),
			Im(((INTLPTR) knotlist->expr))
		);
		knotlist = knotlist->next;
	}
	printf ("...endspline\n");
}

#define	maxx	bounds[MAXX]
#define	maxy	bounds[MAXY]
#define	minx	bounds[MINX]
#define	miny	bounds[MINY]

void boundscall (bounds)
double bounds[4];
{
	printf ("...maxx %g\n", maxx);
	printf ("...maxy %g\n", maxy);
	printf ("...minx %g\n", minx);
	printf ("...miny %g\n", miny);
}

void bbline (lineseg, bounds)
LINEPTR lineseg;
double bounds[4];
{
	maxx = max(maxx, max(lineseg->x0, lineseg->x1));
	maxy = max(maxy, max(lineseg->y0, lineseg->y1));
	minx = min(minx, min(lineseg->x0, lineseg->x1));
	miny = min(miny, min(lineseg->y0, lineseg->y1));
}

void bbcirc (circle, bounds)
CIRCPTR circle;
double bounds[4];
{
	maxx = max(maxx, circle->x0 + fabs(circle->r));
	minx = min(minx, circle->x0 - fabs(circle->r));
	maxy = max(maxy, circle->y0 + fabs(circle->r));
	miny = min(miny, circle->y0 - fabs(circle->r));
}

void bbstring (text, bounds)
TEXTPTR text;
double bounds[4];
{
	maxx = max(maxx, text->x0);
	minx = min(minx, text->x0);
	maxy = max(maxy, text->y0);
	miny = min(miny, text->y0);
}

/***************************************************************************
   bounding box of a circular arc             Eric Grosse  24 May 84

Conceptually, this routine generates a list consisting of the start,
end, and whichever north, east, south, and west points lie on the arc.
The bounding box is then the range of this list.
    list = {start,end}
    j = quadrant(start)
    k = quadrant(end)
    if( j==k && long way 'round )  append north,west,south,east
    else
      while( j != k )
         append center+radius*[j-th of north,west,south,east unit vectors]
         j += 1  (mod 4)
    return( bounding box of list )
The following code implements this, with simple optimizations.
***********************************************************************/

bbarc(arc, bounds)
ARCPTR arc;
double bounds[4];
{
  double x0, y0, x1, y1, xc, yc;  /* start, end, center */
  double xmin, xmax, ymin, ymax;
  /* assumes center isn't too far out */
  double r, x, y;
  int j, k;
  int quadrant();
  x0 = arc->x1;
  y0 = arc->y1;
  x1 = arc->x2;
  y1 = arc->y2;
  xc = arc->x0;
  yc = arc->y0;
  x0 -= xc; y0 -= yc;
  x1 -= xc; y1 -= yc;
  xmin = (x0<x1)?x0:x1; ymin = (y0<y1)?y0:y1;
  xmax = (x0>x1)?x0:x1; ymax = (y0>y1)?y0:y1;
  r = sqrt(x0*x0+y0*y0);
  if(r>0.){
    j = quadrant(x0,y0);
    k = quadrant(x1,y1);
    if((j==k)&&(y1*x0<x1*y0)){
      /* viewed as complex numbers, if Im(z1/z0)<0, arc is big */
      if( xmin > -r) xmin = -r; if( ymin > -r) ymin = -r;
      if( xmax <  r) xmax =  r; if( ymax <  r) ymax =  r;
    }else{
      while(j!=k){
        switch(j){
         case 1: if( ymax <  r) ymax =  r; break; /* north */
         case 2: if( xmin > -r) xmin = -r; break; /* west */
         case 3: if( ymin > -r) ymin = -r; break; /* south */
         case 4: if( xmax <  r) xmax =  r; break; /* east */
        }
        j = j%4 + 1;
      }
    }
  }
  xmin += xc; ymin += yc;
  xmax += xc; ymax += yc;

  maxx = max(maxx,xmax);
  minx = min(minx,xmin);
  maxy = max(maxy,ymax);
  miny = min(miny,ymin);
}

int
quadrant(x,y)
  double x,y;
{
  double z=0.;
  if(      (x>=z)&&(y> z) ){ return(1); }
  else if( (x< z)&&(y>=z) ){ return(2); }
  else if( (x<=z)&&(y< z) ){ return(3); }
  else if( (x> z)&&(y<=z) ){ return(4); }
  else{ fprintf(stderr,"can't happen: x,y=%g,%g",x,y); exit(1);}
}

void bbspline (knotlist, bounds)
EXPRPTR knotlist;
double bounds[4];
{
	double xmin, xmax, ymin, ymax, x, y;
	xmin = xmax = Re(((INTLPTR) knotlist->expr));
	ymin = ymax = Im(((INTLPTR) knotlist->expr));
	knotlist = knotlist->next;
	while (knotlist) {
		x = Re(((INTLPTR) knotlist->expr));
		xmin = min(xmin, x);
		xmax = max(xmax, x);
		y = Im(((INTLPTR) knotlist->expr));
		ymin = min(ymin, y);
		ymax = max(ymax, y);
		knotlist = knotlist->next;
	}
	minx = min(minx, xmin);
	maxx = max(maxx, xmax);
	miny = min(miny, ymin);
	maxy = max(maxy, ymax);
}
