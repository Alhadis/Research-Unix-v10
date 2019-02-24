/* Map-plotting functions for PostScript */
#include "plot.h"

static double _a,_b,_c,_d, _ox, _oy; 
static int anystroke = 0;
static char *fontname = "/Times-Roman";
static int fontsize = 10;

#define xs(_x) (_ox = _a*(_x)+_b)
#define ys(_y) (_oy = _c*(_y)+_d)

void
space(int xmin, int ymin, int xmax, int ymax)
{
	_a = 6.5*72/(xmax-xmin);
	_b = -_a*xmin + 1.0*72;
	_c = 6.5*72/(ymax-ymin);
	_d = -_c*ymin + 3.5*72;
}


/* open the plotting output */
void
erase(void)
{
	printf("%%!PS\n"
		"%%Creator: map\n"
		"%%BoundingBox: 72 252 540 720\n"
		"%%EndComments\n"
		"/m /moveto load def\n"
		"/s /stroke load def\n"
		"/t {moveto show} bind def\n"
		"/v /lineto load def\n"
		"%%EndProlog\n"
		"gsave\n"
		"/Helvetica findfont 10 scalefont setfont\n"
		"1 setlinecap\n"
		"1 setlinejoin\n"
		".5 setlinewidth\n");
	printf("%s findfont\n"
		"/scale %d def\n"
		"scale scalefont\n"
		"setfont\n", fontname, fontsize);
	printf("newpath\n"
		"72 72 m\n");
	anystroke = 1;
}

/* close the plotting output */
void
closepl(void)
{
	printf("showpage\ngrestore\n");
}

/* make sure the page or screen is clear */
void
openpl(void)
{
	anystroke = 0;
}

/* plot a point at _x,_y, which becomes current */
void
point(int _x, int _y)
{
	move(_x, _y);
	cont(_x, _y);
}

/* place text, first letter at current point, which does not change */
void
label(char *_s)
{
	if(anystroke)
		printf("s\n");
	printf("(%s) %.1f %.1f scale .3 mul sub t\n",_s,_ox,_oy);
	anystroke = 0;
}

/* draw line from current point to _x,_y, which becomes current */
void
cont(int _x, int _y)
{
	printf("%.1f %.1f v\n", xs(_x),ys(_y));
	anystroke = 1;
}

/* _x,_y becomes current point */
void
move(int _x, int _y)
{
	if(anystroke)
		printf("s\n");
	printf("%.1f %.1f m\n", xs(_x), ys(_y));
	anystroke = 1;
}

struct dashpat {
	char *name;
	char *pat;
	double width;
} pattern[] = {
	"solid", "[] 0", 0.5,
	"dotted", "[3 6] 0", 0,
	"dotdash", "[18 6 3 6] 0", .5,
};

/* specify style for drawing lines: "dotted", "solid", "dotdash" */
void
linemod(char *s)
{
	int i;
	for(i=0; i<sizeof(pattern)/sizeof(*pattern); i++)
		if(strcmp(s,pattern[i].name) == 0)
			break;
	if(i >= sizeof(pattern)/sizeof(*pattern))
		i = 0;
	if(anystroke)
		printf("s\n");
	printf("%f setlinewidth\n%s setdash\n",
		pattern[i].width, pattern[i].pat);
	anystroke = 0;
}

