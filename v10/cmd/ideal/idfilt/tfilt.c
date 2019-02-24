#include "idfilt.h"
#undef round
#define round(x) ((abs(x)<0.00001)?0:(x))

float xscale, yscale;

void idjusttext (str)
char *str;
{
	if (
		strncmp (str, ".IE", 3) &&
		strncmp (str, "...knot", 7) &&
		strncmp (str, "...endspline", 12) &&
		strncmp (str, "...left", 7) &&
		strncmp (str, "...center", 9) &&
		strncmp (str, "...right", 8)
	)
		fputs (str, stdout);
}

void idstart ()
{
}

void idendbound ()
{
	float realwidth;
	if (boundset)
		return;
	idminx (-6.0);
	idmaxy (6.0);
	idmaxx (6.0);
	idminy (-6.0);
	if (!banzai && maxx - minx < 0.2) {
		maxx += 1;
		minx -= 1;
	}
	if (!banzai && maxy - miny < 0.2) {
		maxy += 1;
		miny -= 1;
	}
	if (!samescale) {
		realwidth = width;
		xscale = width/(maxx - minx);
		if (!heightset)
			yscale = - xscale;
		else
			yscale = height/(miny - maxy);
	} else
		realwidth = (maxx - minx)*xscale;
printf("...%g %g %g\n", width, realwidth, xscale);
	minx -= 0.5*(colwid - realwidth)/xscale;
	maxx += 0.5*(colwid - realwidth)/xscale;
	boundset = TRUE;
	printf (".ne %4.2fi\n", heightset?height:yscale*(miny - maxy));
}

void idline (x1, y1, x2, y2)
float x1;
float y1;
float x2;
float y2;
{
	float X1, Y1, X2, Y2;
	boolean shortvert, shorthoriz, nonrectilinear;
	X1 = round(xscale*x1);
	Y1 = round(yscale*y1);
	X2 = round(xscale*x2);
	Y2 = round(yscale*y2);
	shortvert = X1 == X2 && abs(Y1-Y2) < 0.25;
	shorthoriz = Y1 == Y2 && abs(X1-X2) < 0.25;
	nonrectilinear = X1 != X2 && Y1 != Y2;
	if (wantquality || shortvert || shorthoriz || nonrectilinear)
		printf ("\\h'%gi'\\v'%gi'\\D'l %gi %gi'\\h'%gi'\\v'%gi'\n.sp -1\n",
			round(xscale*(x1-minx)),
			round(yscale*(y1-maxy)),
			round(xscale*(x2-x1)),
			round(yscale*(y2-y1)),
			round(-xscale*(x2-minx)),
			round(-yscale*(y2-maxy))
		);
	else {
		if (Y1 == Y2)
			printf ("\\h'%gi'\\v'%gi'\\l'%gi'\\h'%gi'\\v'%gi'\n.sp -1\n",
				round(xscale*(x1-minx)),
				round(yscale*(y1-maxy)),
				round(xscale*(x2-x1)),
				round(-xscale*(x2-minx)),
				round(-yscale*(y2-maxy))
			);
		if (X1 == X2)
			printf ("\\h'%gi'\\v'%gi'\\L'%gi'\\h'%gi'\\v'%gi'\n.sp -1\n",
				round(xscale*(x1-minx)),
				round(yscale*(y1-maxy)),
				round(yscale*(y2-y1)),
				round(-xscale*(x2-minx)),
				round(-yscale*(y2-maxy))
			);
	}
}

void idcircle (x0, y0, r)
float x0;
float y0;
float r;
{
	printf ("\\h'%gi'\\v'%gi'\\D'c %gi'\\h'%gi'\\v'%gi'\n.sp -1\n",
		round(xscale*(x0-r-minx)),
		round(yscale*(y0-maxy)),
		round(2*xscale*r),
		round(-xscale*(x0+r-minx)),
		round(-yscale*(y0-maxy))
	);
}

void idarc (x0, y0, x1, y1, x2, y2, t1, t2, r)
float x0;
float y0;
float x1;
float y1;
float x2;
float y2;
float t1;
float t2;
float r;
{
	if (xscale*r > 30000.0)
		idline (x1, y1, x2, y2);
	else {
		printf ("\\h'%gi'\\v'%gi'\\D'a %gi %gi %gi %gi'\\h'%gi'\\v'%gi'\n.sp -1\n",
			round(xscale*(x1-minx)),
			round(yscale*(y1-maxy)),
			round(xscale*(x0-x1)),
			round(yscale*(y0-y1)),
			round(xscale*(x2-x0)),
			round(yscale*(y2-y0)),
			round(-xscale*(x2-minx)),
			round(-yscale*(y2-maxy))
		);
	}
}

void idleft (x, y, str)
float x;
float y;
char *str;
{
	str == ++str;
	printf ("\\h'%gi'\\v'%gi'%s\\h'-\\w\\(ts%s\\(tsu'\n.sp -1\n",
		round(xscale*(x-minx)),
		round(yscale*(y-maxy)),
		str,
		str
	);
}

void idcenter (x, y, str)
float x;
float y;
char *str;
{
	str = ++str;
	printf ("\\h'%gi'\\v'%gi'\\h'-\\w\\(ts%s\\(tsu/2u'%s\\h'-\\w\\(ts%s\\(tsu/2u'\n.sp -1\n",
		round(xscale*(x-minx)),
		round(yscale*(y-maxy)),
		str,
		str,
		str
	);
}

void idright (x, y, str)
float x;
float y;
char *str;
{
	str = ++str;
	printf ("\\h'%gi'\\v'%gi'\\h'-\\w\\(ts%s\\(tsu'%s\\h'-\\w\\(ts%s\\(tsu'\n.sp -1\n",
		round(xscale*(x-minx)),
		round(yscale*(y-maxy)),
		str,
		str,
		str
	);
}

void idendE ()
{
	if (boundset)
		printf (".sp %gi\n.sp 1\n.sp 1\n",
			round(yscale*(miny-maxy))
		);
	printf (".IE\n");
}

void idendF ()
{
}

float osplx, osply;

void idspline (sx, sy)
float sx, sy;
{
	osplx = sx;
	osply = sy;
	printf ("\\h'%gi'\\v'%gi'\\D'~",
		round(xscale*(osplx-minx)),
		round(yscale*(osply-maxy))
	);
}

void idknot (sx, sy)
float sx, sy;
{
	printf (" %gi %gi",
		round(xscale*(sx-osplx)),
		round(yscale*(sy-osply))
	);
	osplx = sx;
	osply = sy;
}

void idendspline ()
{
	printf ("'\\h'%gi'\\v'%gi'\n.sp -1\n",
		round(xscale*(minx-osplx)),
		round(yscale*(maxy-osply))
	);
}

void idnoerase ()
{
}


void idyeserase ()
{
}
