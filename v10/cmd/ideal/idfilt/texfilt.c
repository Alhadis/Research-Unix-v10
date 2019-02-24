#include "idfilt.h"

float xscale, yscale;
extern double atan2();

#define xadj(x) (int)((abs(x)<0.00001)?0:(xscale*(x)))
#define yadj(y) (int)((abs(y)<0.00001)?0:(yscale*(y)))
#define PI	3.1415926535897932384626433832795028841971693993751
#define	PI2	(2*PI)

int inpicture=0;	/* because of nesting, we can see more than one
			   .IS and more than one .IE for a given picture */

void idjusttext (str)
char *str;
{
	if (
		strncmp (str, "...", 3) &&
		strncmp (str, ".IS", 3) &&
		strncmp (str, ".IE", 3) &&
		strncmp (str, ".IF", 3) &&
		strncmp (str, ".lf", 3)
	)
		fputs (str, stdout);
}

void idstart ()
{
	/*
	 * Don't print prologue here, because might not draw anything.
	 * Wait until bounds set.
	 */
}

float realwidth, realheight;

void idendbound ()
{
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
	/*
	 * The tpic special coordinate system is in milli-inches,
	 * with positive y downward.
	 */
	if (!samescale) {
		realwidth = width;
		xscale = 1000*width/(maxx - minx);
		if (!heightset) {
			yscale = - xscale;
		} else
			yscale = 1000*height/(miny - maxy);
	} else
		realwidth = (maxx - minx)*xscale;
	realheight = heightset?height:yscale*(miny-maxy)/1000;
	boundset = TRUE;
	inpicture = 1;

	/*
	 * First, allocate a \graph box if there isn't one.
	 * Expand plain's \newbox here because plain makes it "outer".
	 */
	printf ("\\catcode`@=11\n");
	printf ("\\expandafter\\ifx\\csname graph\\endcsname\\relax\n");
	printf (" \\alloc@4\\box\\chardef\\insc@unt\\graph\\fi\n");
	printf ("\\catcode`@=12\n");
	/*
	 * Now set \graph to a vtop containing a vbox containing an hbox.
	 */
	printf ("\\setbox\\graph=\\vtop{%%\n");
	printf ("  \\baselineskip=0pt \\lineskip=0pt \\lineskiplimit=0pt\n");
	printf ("  \\vbox to0pt{\\hbox{%%\n");
}

void idendE ()
{
	if (inpicture) {
		printf ("    \\kern %gin\n", realwidth);
		printf ("  }\\vss}%%\n");
		printf ("  \\kern %gin\n}\n", realheight);
		inpicture = 0;
	}
}

void idendF ()
{
	if (inpicture) {
		printf ("    \\kern %gin\n", realwidth);
		printf ("  }\\vss}%%\n");
		printf ("  \\kern %gin\n}\n", realwidth);
		inpicture = 0;
	}
}

void idline (x1, y1, x2, y2)
float x1;
float y1;
float x2;
float y2;
{
	printf ("    \\special{pa %d %d}\\special{pa %d %d}\\special{fp}%%\n",
		xadj(x1-minx),
		yadj(y1-maxy),
		xadj(x2-minx),
		yadj(y2-maxy)
	);
}

void idcircle (x0, y0, r)
float x0;
float y0;
float r;
{
	printf ("    \\special{ar %d %d %d %d %f %f}%%\n",
		xadj(x0-minx),
		yadj(y0-maxy),
		xadj(r),
		-yadj(r),
		0.0,
		PI2
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
	else
		printf ("    \\special{ar %d %d %d %d %f %f}%%\n",
			xadj(x0-minx),
			yadj(y0-maxy),
			xadj(r),
			- yadj(r),
			PI2 - t2,
			PI2 - t1
		);
}

void idleft (x, y, str)
float x;
float y;
char *str;
{
	str == ++str;
	printf ("    \\smash{\\rlap{\\kern%5.2fin\\raise%5.2fin\\hbox{%s}}}%%\n",
		xadj(x-minx)/1000.,
		yadj(maxy-y)/1000.,
		str
	);
}

void idcenter (x, y, str)
float x;
float y;
char *str;
{
	str = ++str;
	printf ("    \\smash{\\rlap{\\kern%5.2fin\\raise%5.2fin\\hbox to 0pt{\\hss %s\\hss}}}%%\n",
		xadj(x-minx)/1000.,
		yadj(maxy-y)/1000.,
		str
	);
}

void idright (x, y, str)
float x;
float y;
char *str;
{
	str = ++str;
	printf ("    \\smash{\\rlap{\\kern%5.2fin\\raise%5.2fin\\llap{%s}}}%%\n",
		xadj(x-minx)/1000.,
		yadj(maxy-y)/1000.,
		str
	);
}

void idspline (sx, sy)
float sx, sy;
{
	printf ("     \\special{pa %d %d}%%\n",
		xadj(sx-minx),
		yadj(sy-maxy)
	);
}

void idknot (sx, sy)
float sx, sy;
{
	printf ("     \\special{pa %d %d}%%\n",
		xadj(sx-minx),
		yadj(sy-maxy)
	);
}

void idendspline ()
{
	printf ("    \\special{sp}%%\n");
}

void idnoerase ()
{
}


void idyeserase ()
{
}
