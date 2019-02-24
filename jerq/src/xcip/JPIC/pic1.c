#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"
#define	dprintf	if(dbg)printf

#define	PI	3.141592654
#define	PI2	PI/2
#define	SCALE	200	/* default scale: units/inch */
#define	WID	150	/* default width for boxes and ellipses */
#define	HT	100	/* default height and line length */

struct	obj	*objlist[MAXOBJ];	/* store the elements here */
int	nobj	= 0;

struct attr	attr[40];	/* attributes stored here as collected */
int	nattr	= 0;	/* number of entries in attr_list */

struct	text	text[MAXTEXT];	/* text strings stored here as collected */
int	ntext	= 0;
int	ntext1	= 0;	/* record ntext here on entry to each figure */

struct symtab	symtab[SYMTAB];
int	nsymtab	= 0;

coord	curx	= 0;
coord	cury	= 0;

int	hvmode	= R_DIR;	/* R => join left to right, D => top to bottom, etc. */

#define	MAXTYPE	12
int	first[MAXTYPE];	/* indicates whether first of particular type */
			/* has been seen within this picture */

int	codegen	= 0;	/* 1=>output for this picture; 0=>no output */

struct	obj	*makenode();
char	*malloc();

float	deltx	= 6;	/* max x value in output, for scaling */
float	delty	= 6;	/* max y value in output, for scaling */
float	scale	= 0;	/* implies simply scaling by this value; no crop or shift */
int	dbg	= 0;
extern	FILE	*yyin;	/* input file pointer */
int	lineno	= 0;
char	*filename	= "-";
int	synerr	= 0;
char	*cmdname;
int	crop	= 1;	/* trim off exterior white space if non-zero */

#define	DEV202	1
#define	DEVAPS	2
#define	DEVCAT	3
#define	DEV450	4

/* You may want to change this if you don't have a 202... */

int	devtype	= DEV202;
int	res	= 972;	/* default is 202 */
int	DX	= 4;	/* used only for old-style troff */
int	DY	= 4;

/* mandatory values for graphic systems CAT: */
/*
int	devtype = DEVCAT;
int	res	= 432;
int	DX = 3;
int	DY = 3;
*/

float	hshift	= 0;	/* move this far left for text (in em's) */
float	vshift	= 0.2;	/* this far down */

coord	sxmin;		/* lower limit from s command */
coord	symin;
coord	sxmax	= 4096;	/* upper */
coord	symax	= 4096;

coord	xmin	= 30000;	/* min values found in actual data */
coord	ymin	= 30000;
coord	xmax	= -30000;	/* max */
coord	ymax	= -30000;

extern	float	atof();

main(argc,argv)
char **argv;
{
	int casel, c;

	cmdname = argv[0];
	casel = 0;
	while (argc > 1 && *argv[1] == '-') {
		switch (c = argv[1][1]) {
		case 'T':
			if (strcmp(&argv[1][2], "aps") == 0) {
				res = 720;
				devtype = DEVAPS;
				DX = DY = 1;
			} else if (strcmp(&argv[1][2], "cat") == 0) {
				res = 432;
				devtype = DEVCAT;
				DX = DY = 3;
			} else if (strcmp(&argv[1][2], "450") == 0) {
				res = 240;
				devtype = DEV450;
			} else {
				res = atoi(&argv[1][2]);
			}
			break;
		case 'c':
			crop = 0;
			break;
		case 'l':
			delty = atof(&argv[1][2]);
			casel = 0;
			break;
		case 'w':
		case 's':
			if (argv[1][2] == 0) {
				argv++;
				argc--;
				deltx = atof(&argv[1][0]);
			} else
				deltx = atof(&argv[1][2]);
			if (c == 's')
				scale = deltx;
			break;
		case 'd':
			dbg = 1;
			break;
		}
		argc--;
		argv++;
	}
	if (!casel)
		delty = deltx;
	setdefaults();
	if (argc <= 1) {
		yyin = stdin;
		getdata(yyin);
	} else
		while (argc-- > 1) {
			if ((yyin = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "pic: can't open %s\n", *argv);
				exit(1);
			}
			filename = *argv;
			getdata(yyin);
			fclose(yyin);
		}
	exit(0);
}

setdefaults()	/* set default sizes for variables like boxht */
{
	static struct {
		char *name;
		int val;
	} defaults[] ={
		"lineht", HT,
		"linewid", HT,
		"moveht", HT,
		"movewid", HT,
		"dashwid", HT/10,
		"boxht", HT,
		"boxwid", WID,
		"circlerad", HT/2,
		"arcrad", HT/2,
		"ellipseht", HT,
		"ellipsewid", WID,
		"arrowht", HT/5,
		"arrowwid", HT/10,
		"textht", HT,
		"textwid", WID,
		"scale", SCALE,
		NULL, 0
	};
	int i;

	for (i = 0; defaults[i].name != NULL; i++)
		makevar(defaults[i].name, VARNAME, defaults[i].val);
}

getdata(fin)
register FILE *fin;
{
	char buf[1000], buf1[50];
	FILE *svyyin;
	int svlineno;
	char *svfilename, *p;

	lineno = 0;
	while (fgets(buf, sizeof buf, fin) != NULL) {
		lineno++;
		if (*buf == '.' && *(buf+1) == 'P' && *(buf+2) == 'S') {
			for (p = &buf[3]; *p == ' '; p++)
				;
			if (*p++ == '<') {
				svyyin = yyin;
				svlineno = lineno;
				svfilename = filename;
				sscanf(p, "%s", buf1);
				if ((yyin = fopen(buf1, "r")) == NULL) {
					fprintf(stderr, "pic: can't open %s\n", buf1);
					exit(1);
				}
				lineno = 0;
				filename = p;
				getdata(yyin);
				fclose(yyin);
				lineno = svlineno;
				yyin = svyyin;
				filename = svfilename;
				continue;
			}
			reset();
			yyparse();
			if (buf[3] == ' ')	/* assume next thing is width */
				deltx = delty = atof(&buf[4]);
                        else {    /* use scale to determine size */
				int t;
				t = xmax - xmin;
				if (t == 0)
					t = ymax - ymin;
				deltx = delty = (float) t / (float)getvar(lookup("scale"));
			}
			dprintf("deltx = %.3f\n", deltx);
			if (codegen && !synerr) {
				openpl(&buf[3]);	/* puts out .PS, with ht & wid stuck in */
				print();	/* assumes \n at end */
				closepl();	/* this does the .PE */
			}
		}
		else
			fputs(buf, stdout);
	}
}

print()
{
	struct obj *p;
	int i, j, m;
	coord x0, y0, x1, y1, ox, oy, dx, dy;

	for (i = 0; i < nobj; i++) {
		p = objlist[i];
		ox = p->o_x;
		oy = p->o_y;
		x1 = p->o_val[0];
		y1 = p->o_val[1];
		m = p->o_mode;
		switch (p->o_type) {
		case TROFF:
			troff(text[p->o_nt1].t_val);
			break;
		case BOX:
			move(ox, oy);
			dotext(p);	/* if there are any text strings */
			x0 = ox - (x1+1) / 2;
			y0 = oy - (y1+1) / 2;
			x1 = ox + x1 / 2;
			y1 = oy + y1 / 2;
			if (p->o_attr & INVIS)
				;	/* nothing at all */
			else if (p->o_dotdash == 0)
				box(x0, y0, x1, y1);
			else
				dotbox(x0, y0, x1, y1, p->o_dotdash, p->o_ddval);
			if (ishor(m))
				move(isright(m) ? x1 : x0, oy);	/* right side */
			else
				move(ox, isdown(m) ? y0 : y1);	/* bottom */
			break;
		case CIRCLE:
			move(ox, oy);
			dotext(p);
			if ((p->o_attr & INVIS) == 0)
				circle(ox, oy, x1);
			if (ishor(m))
				move(ox + isright(m) ? x1 : -x1, oy);
			else
				move(ox, oy + isup(m) ? x1 : -x1);
			break;
		case ELLIPSE:
			move(ox, oy);
			dotext(p);
			if ((p->o_attr & INVIS) == 0)
				ellipse(ox, oy, x1, y1);
			if (ishor(m))
				move(ox + isright(m) ? x1 : -x1, oy);
			else
				move(ox, oy - isdown(m) ? y1 : -y1);
			break;
		case ARC:
			move(ox, oy);
			dotext(p);
			if (p->o_attr & HEAD1)
				arrow(x1 - (y1 - oy), y1 + (x1 - ox),
				      x1, y1, p->o_val[4], p->o_val[5]);
                        if (p->o_attr & INVIS)
                                /* probably wrong when it's cw */
                                move(x1, y1);
                        else
				arc(ox, oy, x1, y1, p->o_val[2], p->o_val[3]);
			if (p->o_attr & HEAD2)
				arrow(p->o_val[2] + p->o_val[3] - oy, p->o_val[3] - (p->o_val[2] - ox),
				      p->o_val[2], p->o_val[3], p->o_val[4], p->o_val[5]);
			if (p->o_attr & CW_ARC)
				move(x1, y1);	/* because drawn backwards */
			break;
		case LINE:
		case ARROW:
		case SPLINE:
			move((ox + x1)/2, (oy + y1)/2);	/* center */
			dotext(p);
			if (p->o_attr & HEAD1)
				arrow(ox + p->o_val[5], oy + p->o_val[6], ox, oy, p->o_val[2], p->o_val[3]);
                        if (p->o_attr & INVIS)
                                move(x1, y1);
			else if (p->o_type == SPLINE)
				spline(ox, oy, p->o_val[4], &p->o_val[5]);
			else {
				int i, j, ndx, ndy;
				dx = ox;
				dy = oy;
				for (i=0, j=5; i < p->o_val[4]; i++, j += 2) {
					ndx = dx + p->o_val[j];
					ndy = dy + p->o_val[j+1];
					if (p->o_dotdash == 0)
						line(dx, dy, ndx, ndy);
					else
						dotline(dx, dy, ndx, ndy, p->o_dotdash, p->o_ddval);
					dx = ndx;
					dy = ndy;
				}
			}
			if (p->o_attr & HEAD2) {
				int i, j;
				dx = ox;
				dy = oy;
				for (i = 0, j = 5; i < p->o_val[4] - 1; i++, j += 2) {
					dx += p->o_val[j];
					dy += p->o_val[j+1];
				}
				arrow(dx, dy, x1, y1, p->o_val[2], p->o_val[3]);
			}
			break;
		case MOVE:
			move(ox, oy);
			dotext(p);
			break;
		case TEXT:
			move(ox, oy);
			label(x1, y1, 0);
			free(x1);
			break;
		}
	}
}

dotline(x0, y0, x1, y1, ddtype, ddval) /* dotted line */
coord x0, y0, x1, y1;
int ddtype;
int ddval;
{
	static int prevval = SCALE/20;	/* 20 per inch by default */
	int i, numdots;
	double a, b, sqrt(), dx, dy;

	if (ddval == 0)
		ddval = prevval;
	prevval = ddval;
	/* don't save dot/dash value */
	dx = x1 - x0;
	dy = y1 - y0;
	if (ddtype == DOT) {
		numdots = sqrt(dx*dx + dy*dy) / prevval + 0.5;
		for (i = 0; i <= numdots; i++) {
			a = (float) i / (float) numdots;
			move(x0 + (int)(a * dx), y0 + (int)(a * dy));
			dot();
		}
	} else if (ddtype == DASH) {
		double d, dashsize, spacesize;
		d = sqrt(dx*dx + dy*dy) + 0.5;
		if (d <= 2 * prevval) {
			line(x0, y0, x1, y1);
			return;
		}
		numdots = d / (2 * prevval - 1) + 1;	/* ceiling */
		dashsize = prevval;
		spacesize = (d - numdots * dashsize) / (numdots - 1);
		for (i = 0; i < numdots-1; i++) {
			a = i * (dashsize + spacesize) / d;
			b = a + dashsize / d;
			line(x0 + (int)(a*dx), y0 + (int)(a*dy), x0 + (int)(b*dx), y0 + (int)(b*dy));
			a = b;
			b = a + spacesize / d;
			move(x0 + (int)(a*dx), y0 + (int)(a*dy));
		}
		line(x0 + (int)(b * dx), y0 + (int)(b * dy), x1, y1);
	}
	prevval = SCALE/20;
}

dotbox(x0, y0, x1, y1, ddtype, ddval)	/* dotted or dashed box */
coord x0, y0, x1, y1;
int ddtype;
int ddval;
{
	dotline(x0, y0, x1, y0, ddtype, ddval);
	dotline(x1, y0, x1, y1, ddtype, ddval);
	dotline(x1, y1, x0, y1, ddtype, ddval);
	dotline(x0, y1, x0, y0, ddtype, ddval);
}

dotext(p)	/* print text strings of p in proper vertical spacing */
struct obj *p;
{
	int i, nhalf;

	nhalf = p->o_nt2 - p->o_nt1 - 1;
	for (i = p->o_nt1; i < p->o_nt2; i++) {
		label(text[i].t_val, text[i].t_type, nhalf);
		nhalf -= 2;
	}
}

reset()
{
	int i;

	for (i = 0; i < nobj; i++)
		free(objlist[i]);
	nobj = 0;
	nattr = 0;
	for (i = 0; i < ntext; i++)
		free(text[i].t_val);
	ntext = ntext1 = 0;
	for (i = 0; i < MAXTYPE; i++)
		first[i] = 0;
	codegen = synerr = 0;
	curx = cury = 0;
	hvmode = R_DIR;
	sxmin = symin = 0;
	sxmax = symax = 4096;
	xmin = ymin = 30000;
	xmax = ymax = -30000;
}

coord	xstack[20];	/* store curx here for pushdown {...} */
coord	ystack[20];
int	hvstack[20];
int	nstack	= 0;

push()
{
	xstack[nstack] = curx;
	ystack[nstack] = cury;
	hvstack[nstack] = hvmode;
	nstack++;
}

pop()
{
	struct obj *p;

	nstack--;
	curx = xstack[nstack];
	cury = ystack[nstack];
	hvmode = hvstack[nstack];
	p = makenode(MOVE, 0);
	dprintf("M %d %d\n", curx, cury);
}

setvar(n, v, t)	/* store v in variable in table slot n */
{
	symtab[n].s_val = v;
	symtab[n].s_type = t;
}

getvar(n)	/* return value of variable in slot n */
{
	return(symtab[n].s_val);
}

setdir(n)	/* set direction from n */
int n;
{
	switch (n) {
	case UP:	hvmode = U_DIR; break;
	case DOWN:	hvmode = D_DIR; break;
	case LEFT:	hvmode = L_DIR; break;
	case RIGHT:	hvmode = R_DIR; break;
	}
	return(hvmode);
}

coord getcomp(p, t)	/* return component of a position */
struct obj *p;
int t;
{
	switch (t) {
	case DOTX:
		return(p->o_x);
	case DOTY:
		return(p->o_y);
	case DOTWID:
		switch (p->o_type) {
		case BOX:
			return(p->o_val[0]);
		case CIRCLE:
		case ELLIPSE:
			return(2 * p->o_val[0]);
		case LINE:
		case ARROW:
			return(p->o_val[0] - p->o_x);
		}
	case DOTHT:
		switch (p->o_type) {
		case BOX:
			return(p->o_val[1]);
		case CIRCLE:
		case ELLIPSE:
			return(2 * p->o_val[1]);
		case LINE:
		case ARROW:
			return(p->o_val[1] - p->o_y);
		}
	case DOTRAD:
		switch (p->o_type) {
		case CIRCLE:
		case ELLIPSE:
			return(p->o_val[0]);
		}
	}
}


makeattr(type, val)	/* add attribute type and val */
int type;
int val;	/* typing probably wrong */
{
	if (type == 0 && val == 0) {	/* clear table for next stat */
		nattr = 0;
		return;
	}
	dprintf("attr %d:  %d %d\n", nattr, type, val);
	attr[nattr].a_type = type;
	attr[nattr].a_val = val;
	nattr++;
}

troffgen(s)	/* save away a string of troff commands */
char *s;
{
	savetext(CENTER, s);	/* use the existing text mechanism */
	makenode(TROFF, 0);
}

printexpr(n)	/* print expression for debugging */
int n;
{
	dprintf("%d\n", n);
}

struct obj *boxgen(type)
{
	static int prevh = HT;
	static int prevw = WID;	/* golden mean, sort of */
	int i, invis, at, ddtype, ddval;
	int with, xwith, ywith;
	int h, w;
	coord x0, y0, x1, y1;
	struct obj *p, *ppos;

	/* EXPENSIVE! */
	h = getvar(lookup("boxht"));
	w = getvar(lookup("boxwid"));
	invis = at = 0;
	with = xwith = ywith = 0;
	ddtype = ddval = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case HEIGHT:
			h = attr[i].a_val;
			break;
		case WIDTH:
			w = attr[i].a_val;
			break;
		case SAME:
			h = prevh;
			w = prevw;
			break;
		case WITH:
			with = attr[i].a_val;	/* corner */
			break;
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			at++;
			break;
		case INVIS:
			invis = INVIS;
			break;
		case DOT:
		case DASH:
			ddtype = attr[i].a_type;
			ddval = attr[i].a_val;
			if (ddval == 0)
				ddval = getvar(lookup("dashwid"));
			break;
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		}
	}
	if (with) {
		switch (with) {
		case NORTH:	ywith = -((h+1) / 2); break;
		case SOUTH:	ywith = h / 2; break;
		case EAST:	xwith = -((w+1) / 2); break;
		case WEST:	xwith = w / 2; break;
		case NE:	xwith = -((w+1) / 2); ywith = -((h+1) / 2); break;
		case SE:	xwith = -((w+1) / 2); ywith = h / 2; break;
		case NW:	xwith = w / 2; ywith = -((h+1) / 2); break;
		case SW:	xwith = w / 2; ywith = h / 2; break;
		}
		curx += xwith;
		cury += ywith;
	}
	if (!at) {
		if (isright(hvmode))
			curx += (w+1) / 2;
		else if (isleft(hvmode))
			curx -= w / 2;
		else if (isup(hvmode))
			cury += (h+1) / 2;
		else
			cury -= h / 2;
	}
	x0 = curx - w / 2;
	y0 = cury - h / 2;
	x1 = curx + (w+1) / 2;
	y1 = cury + (h+1) / 2;
	extreme(x0, y0);
	extreme(x1, y1);
	p = makenode(BOX, 2);
	p->o_val[0] = w;
	p->o_val[1] = h;
	p->o_dotdash = ddtype;
	p->o_ddval = ddval;
	p->o_attr = invis;
	dprintf("B %d %d %d %d at %d %d, h=%d, w=%d\n", x0, y0, x1, y1, curx, cury, h, w);
	if (isright(hvmode))
		curx = x1;
	else if (isleft(hvmode))
		curx = x0;
	else if (isup(hvmode))
		cury = y1;
	else
		cury = y0;
	prevh = h;
	prevw = w;
	return(p);
}

savetext(t, s)	/* record text elements for current object */
int t;
char *s;
{
	switch (t) {
	case CENTER:	t = 'C'; break;
	case LJUST:	t = 'L'; break;
	case RJUST:	t = 'R'; break;
	case SPREAD:	t = 'S'; break;
	case FILL:	t = 'F'; break;
	case ABOVE:	t = 'A'; break;
	case BELOW:	t = 'B'; break;
	}
	if (ntext >= MAXTEXT) {
		fprintf(stderr, "too many text strings (%d)\n", ntext);
		exit(1);
	}
	text[ntext].t_type = t;
	text[ntext].t_val = s;
	dprintf("saving %c text %s at %d\n", t, s, ntext);
	ntext++;
}

struct obj *circgen(type)
{
	static int rad[2] = { HT/2, WID/2 };
	static int rad2[2] = { HT/2, HT/2 };
	static coord x0, y0, x1, y1, x2, y2;
	int i, at, t, invis;
	int with, xwith, ywith;
	int r, r2;
	struct obj *p, *ppos;

	at = invis = 0;
	with = xwith = ywith = 0;
	t = (type == CIRCLE) ? 0 : 1;
	if (type == CIRCLE)
		r = r2 = getvar(lookup("circlerad"));
	else if (type == ELLIPSE) {
		r = getvar(lookup("ellipsewid")) / 2;
		r2 = getvar(lookup("ellipseht")) / 2;
	}
	for (i = 0; i < nattr; i++)
		switch (attr[i].a_type) {
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		case RADIUS:
			r = attr[i].a_val;
			break;
		case DIAMETER:
		case WIDTH:
			r = (attr[i].a_val + 1) / 2;
			break;
		case HEIGHT:
			r2 = (attr[i].a_val + 1) / 2;
			break;
		case SAME:
			r = rad[t];
			r2 = rad2[t];
			break;
		case WITH:
			with = attr[i].a_val;
			break;
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			at++;
			break;
		case INVIS:
			invis = INVIS;
			break;
		}
	if (type == CIRCLE)
		r2 = r;	/* probably superfluous */
	if (with) {
		switch (with) {
		case NORTH:	ywith = -r2; break;
		case SOUTH:	ywith = r2; break;
		case EAST:	xwith = -r; break;
		case WEST:	xwith = r; break;
		case NE:	xwith = -r * 0.707; ywith = -r2 * 0.707; break;
		case SE:	xwith = -r * 0.707; ywith = r2 * 0.707; break;
		case NW:	xwith = r * 0.707; ywith = -r2 * 0.707; break;
		case SW:	xwith = r * 0.707; ywith = r2 * 0.707; break;
		}
		curx += xwith;
		cury += ywith;
	}
	if (!at) {
		if (isright(hvmode))
			curx += r;
		else if (isleft(hvmode))
			curx -= r;
		else if (isup(hvmode))
			cury += r2;
		else
			cury -= r2;
	}
	p = makenode(type, 2);
	p->o_val[0] = rad[t] = r;
	p->o_val[1] = rad2[t] = r2;
	if (r <= 0 || r2 <= 0) {
		yyerror("%s has invalid radius %d\n", (type==CIRCLE) ? "circle" : "ellipse", r<r2 ? r : r2);
	}
	p->o_attr = invis;
	extreme(curx+r, cury+r2);
	extreme(curx-r, cury-r2);
	if (type == CIRCLE)
		dprintf("C %d %d %d\n", curx, cury, r);
	if (type == ELLIPSE)
		dprintf("E %d %d %d %d\n", curx, cury, r, r2);
	if (isright(hvmode))
		curx += r;
	else if (isleft(hvmode))
		curx -= r;
	else if (isup(hvmode))
		cury += r2;
	else
		cury -= r2;
	return(p);
}

struct obj *arcgen(type)	/* handles circular and (eventually) elliptical arcs */
{
	static int prevw = HT/10;
	static int prevh = HT/5;
	static int prevrad = HT/2;
	static int dtox[2][4] ={ 1, -1, -1, 1, 1, 1, -1, -1 };
	static int dtoy[2][4] ={ 1, 1, -1, -1, -1, 1, 1, -1 };
	static int dctrx[2][4] ={ 0, -1, 0, 1, 0, 1, 0, -1 };
	static int dctry[2][4] ={ 1, 0, -1, 0, -1, 0, 1, 0 };
	static int nexthv[2][4] ={ U_DIR, L_DIR, D_DIR, R_DIR, D_DIR, R_DIR, U_DIR, L_DIR };
	double sqrt(), atan2(), sin(), cos();
	float dx2, dy2, ht, phi, r, d;
	int i, head, to, at, cw, invis;
	struct obj *p, *ppos;
	coord fromx, fromy, tox, toy;

	prevrad = getvar(lookup("arcrad"));
	prevh = getvar(lookup("arrowht"));
	prevw = getvar(lookup("arrowwid"));
	fromx = curx;
	fromy = cury;
	head = to = at = cw = invis = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		case HEAD:
			head += attr[i].a_val;
			break;
		case INVIS:
			invis = INVIS;
			break;
		case HEIGHT:	/* length of arrowhead */
			prevh = attr[i].a_val;
			break;
		case WIDTH:	/* width of arrowhead */
			prevw = attr[i].a_val;
			break;
		case RADIUS:
			prevrad = attr[i].a_val;
			break;
		case DIAMETER:
			prevrad = attr[i].a_val / 2;
			break;
		case CW:
			cw = 1;
			break;
		case FROM:	/* start point of arc */
			ppos = (struct obj *) attr[i].a_val;
			fromx = ppos->o_x;
			fromy = ppos->o_y;
			break;
		case TO:	/* end point of arc */
			ppos = (struct obj *) attr[i].a_val;
			tox = ppos->o_x;
			toy = ppos->o_y;
			to++;
			break;
		case AT:	/* center of arc */
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			at = 1;
			break;
		case UP:
			hvmode = U_DIR;
			break;
		case DOWN:
			hvmode = D_DIR;
			break;
		case RIGHT:
			hvmode = R_DIR;
			break;
		case LEFT:
			hvmode = L_DIR;
			break;
		}
	}
	if (!at && !to) {	/* the defaults are mostly OK */
		curx = fromx + prevrad * dctrx[cw][hvmode];
		cury = fromy + prevrad * dctry[cw][hvmode];
		tox = fromx + prevrad * dtox[cw][hvmode];
		toy = fromy + prevrad * dtoy[cw][hvmode];
		hvmode = nexthv[cw][hvmode];
	}
	else if (!at) {
		dx2 = (float)(tox - fromx) / 2;
		dy2 = (float)(toy - fromy) / 2;
		phi = atan2(dy2, dx2) + (cw ? -PI2 : PI2);
		for (r=prevrad; (d = r*r - (dx2*dx2+dy2*dy2)) <= 0.0; r *= 2)
			;	/* this kludge gets around too-small radii */
		ht = sqrt(d);
		curx = fromx + dx2 + ht * cos(phi) + 0.5;
		cury = fromy + dy2 + ht * sin(phi) + 0.5;
		dprintf("dx2,dy2=%g,%g, phi=%g, r,ht=%g,%g\n",
			dx2, dy2, phi, r, ht);
	}
	else if (at && !to) {	/* do we have all the cases??? */
		tox = fromx + prevrad * dtox[cw][hvmode];
		toy = fromy + prevrad * dtoy[cw][hvmode];
		hvmode = nexthv[cw][hvmode];
	}
	if (cw) {	/* interchange roles of from-to and heads */
		coord temp;
		temp = fromx; fromx = tox; tox = temp;
		temp = fromy; fromy = toy; toy = temp;
		if (head == HEAD1)
			head = HEAD2;
		else if (head == HEAD2)
			head = HEAD1;
	}
	p = makenode(type, 6);
	/* these are wrong in general */
	extreme(fromx, fromy);
	extreme(tox, toy);
	p->o_val[0] = fromx;
	p->o_val[1] = fromy;
	p->o_val[2] = tox;
	p->o_val[3] = toy;
	if (cw) {
		curx = fromx;
		cury = fromy;
	} else {
		curx = tox;
		cury = toy;
	}
	p->o_val[4] = prevw;
	p->o_val[5] = prevh;
	p->o_attr = head | (cw ? CW_ARC : 0) | invis;
	dprintf("arc at %d %d from %d %d to %d %d head %d %d\n",
		p->o_x, p->o_y,
		p->o_val[0], p->o_val[1], p->o_val[2], p->o_val[3], p->o_val[4], p->o_val[5]);
	return(p);
}

struct obj *linegen(type)
{
	static int prevdx = HT;
	static int prevdy = 0;
	static int prevw = HT/10;
	static int prevh = HT/5;
	int i, j, some, head, ddtype, ddval, invis;
	int chop, chop1, chop2, x0, y0, x1, y1;
	double sin(), cos(), atan2(), theta;
	int defx, defy;
	struct obj *p, *ppos;
	static int xtab[] = { 1, 0, -1, 0 };	/* R=0, U=1, L=2, D=3 */
	static int ytab[] = { 0, 1, 0, -1 };
	int ndxy, dx[50], dy[50];
	coord nx, ny;

	nx = curx;
	ny = cury;
	defx = getvar(lookup("linewid"));
	defy = getvar(lookup("lineht"));
	prevh = getvar(lookup("arrowht"));
	prevw = getvar(lookup("arrowwid"));
	dx[0] = dy[0] = ndxy = some = head = invis = 0;
	chop = chop1 = chop2 = 0;
	ddtype = ddval = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		case HEAD:
			head += attr[i].a_val;
			break;
		case INVIS:
			invis = INVIS;
			break;
		case CHOP:
			if (chop++ == 0)
				chop1 = chop2 = attr[i].a_val;
			else
				chop2 = attr[i].a_val;
			break;
		case DOT:
		case DASH:
			ddtype = attr[i].a_type;
			ddval = attr[i].a_val;
			if (ddval == 0)
				ddval = getvar(lookup("dashwid"));
			break;
		case SAME:
			dx[ndxy] = prevdx;
			dy[ndxy] = prevdy;
			some++;
			break;
		case LEFT:
			dx[ndxy] -= (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = L_DIR;
			break;
		case RIGHT:
			dx[ndxy] += (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = R_DIR;
			break;
		case UP:
			dy[ndxy] += (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = U_DIR;
			break;
		case DOWN:
			dy[ndxy] -= (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = D_DIR;
			break;
		case HEIGHT:	/* length of arrowhead */
			prevh = attr[i].a_val;
			break;
		case WIDTH:	/* width of arrowhead */
			prevw = attr[i].a_val;
			break;
		case TO:
			if (some) {
				nx += dx[ndxy];
				ny += dy[ndxy];
				ndxy++;
				dx[ndxy] = dy[ndxy] = some = 0;
			}
			ppos = (struct obj *) attr[i].a_val;
			dx[ndxy] = ppos->o_x - nx;
			dy[ndxy] = ppos->o_y - ny;
			some++;
			break;
		case BY:
			ppos = (struct obj *) attr[i].a_val;
			dx[ndxy] = ppos->o_x;
			dy[ndxy] = ppos->o_y;
			some++;
			break;
		case THEN:	/* turn off any previous accumulation */
			if (some) {
				nx += dx[ndxy];
				ny += dy[ndxy];
				ndxy++;
				dx[ndxy] = dy[ndxy] = some = 0;
			}
			break;
		case FROM:
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			nx = curx = ppos->o_x;
			ny = cury = ppos->o_y;
			break;
		}
	}
	if (some) {
		nx += dx[ndxy];
		ny += dy[ndxy];
		ndxy++;
		defx = dx[ndxy-1];
		defy = dy[ndxy-1];
	} else {
		defx *= xtab[hvmode];
		defy *= ytab[hvmode];
		dx[ndxy] = defx;
		dy[ndxy] = defy;
		ndxy++;
		nx += defx;
		ny += defy;
	}
	prevdx = defx;
	prevdy = defy;
	if (chop) {
		if (chop == 1 && chop1 == 0)	/* just said "chop", so use default */
			chop1 = chop2 = getvar(lookup("circlerad"));
		theta = atan2((float) defy, (float) defx);
		x0 = chop1 * cos(theta);
		y0 = chop1 * sin(theta);
		curx += x0;
		cury += y0;
		x1 = chop2 * cos(theta);
		y1 = chop2 * sin(theta);
		nx -= x1;
		ny -= y1;
		dx[0] -= x0;
		dy[0] -= y0;
		dx[ndxy-1] -= x1;
		dy[ndxy-1] -= y1;
		if(dbg)printf("chopping %d %d %d %d; cur=%d,%d end=%d,%d\n",
			x0, y0, x1, y1, curx, cury, nx, ny);
	}
	p = makenode(type, 5 + 2 * ndxy);
	curx = p->o_val[0] = nx;
	cury = p->o_val[1] = ny;
	if (head || type == ARROW) {
		p->o_val[2] = prevw;
		p->o_val[3] = prevh;
		if (head == 0)
			head = HEAD2;	/* default arrow head */
	}
	p->o_attr = head | invis;
	p->o_val[4] = ndxy;
	nx = p->o_x;
	ny = p->o_y;
	for (i = 0, j = 5; i < ndxy; i++, j += 2) {
		p->o_val[j] = dx[i];
		p->o_val[j+1] = dy[i];
		extreme(nx += dx[i], ny += dy[i]);
	}
	p->o_dotdash = ddtype;
	p->o_ddval = ddval;
	if (dbg) {
		printf("S or L from %d %d to %d %d with %d elements:\n", p->o_x, p->o_y, curx, cury, ndxy);
		for (i = 0, j = 5; i < ndxy; i++, j += 2)
			printf("%d %d\n", p->o_val[j], p->o_val[j+1]);
	}
	extreme(p->o_x, p->o_y);
	extreme(curx, cury);
	return(p);
}

struct obj *splinegen(type)
{
	linegen(type);
}

struct obj *movegen(type)
{
	static int prevdx, prevdy;
	int i, dx, dy, some;
	int defx, defy;
	struct obj *p;
	struct obj *ppos;
	static int xtab[] = { 1, 0, -1, 0 };	/* R=0, U=1, L=2, D=3 */
	static int ytab[] = { 0, 1, 0, -1 };

	defx = getvar(lookup("movewid"));
	defy = getvar(lookup("moveht"));
	dx = dy = some = 0;
	for (i = 0; i < nattr; i++) {
		switch (attr[i].a_type) {
		case LJUST: case RJUST: case CENTER: case SPREAD: case FILL: case ABOVE: case BELOW:
			savetext(attr[i].a_type, attr[i].a_val);
			break;
		case SAME:
			dx = prevdx;
			dy = prevdy;
			some++;
			break;
		case LEFT:
			dx -= (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = L_DIR;
			break;
		case RIGHT:
			dx += (attr[i].a_val==0) ? defx : attr[i].a_val;
			some++;
			hvmode = R_DIR;
			break;
		case UP:
			dy += (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = U_DIR;
			break;
		case DOWN:
			dy -= (attr[i].a_val==0) ? defy : attr[i].a_val;
			some++;
			hvmode = D_DIR;
			break;
		case TO:
			ppos = (struct obj *) attr[i].a_val;
			dx = ppos->o_x - curx;
			dy = ppos->o_y - cury;
			some++;
			break;
		case BY:
			ppos = (struct obj *) attr[i].a_val;
			dx = ppos->o_x;
			dy = ppos->o_y;
			some++;
			break;
		case FROM:
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			break;
		}
	}
	if (some) {
		defx = dx;
		defy = dy;
	} else {
		defx *= xtab[hvmode];
		defy *= ytab[hvmode];
	}
	prevdx = defx;
	prevdy = defy;
	extreme(curx, cury);
	curx += defx;
	cury += defy;
	extreme(curx, cury);
	p = makenode(MOVE, 0);
	dprintf("M %d %d\n", curx, cury);
	return(p);
}

struct obj *textgen(s, garb)
{
	static int prevdx, prevdy, prevrad = 10;
	int i, dx, dy, some, type;
	struct obj *p, *ppos;

	type = 'C';
	some = dx = dy = 0;
	for (i = 0; i < nattr; i++)
		switch (attr[i].a_type) {
		case LEFT:
			dx -= attr[i].a_val;
			some++;
			break;
		case RIGHT:
			dx += attr[i].a_val;
			some++;
			break;
		case UP:
			dy += attr[i].a_val;
			some++;
			break;
		case DOWN:
			dy -= attr[i].a_val;
			some++;
			break;
		case AT:
			ppos = (struct obj *) attr[i].a_val;
			curx = ppos->o_x;
			cury = ppos->o_y;
			break;
		case LJUST:
			type = 'L';
			break;
		case RJUST:
			type = 'R';
			break;
		case SPREAD:
			type = 'S';
			break;
		case FILL:
			type = 'F';
			break;
		case ABOVE:
			type = 'A';
			break;
		case BELOW:
			type = 'B';
			break;
		}
	dprintf("T %c %s\n", type, s);
	if (some) {
		prevdx = dx;
		prevdy = dy;
	} else {
		dx = prevdx;
		dy = prevdy;
	}
	extreme(curx, cury);
	curx += dx;
	cury += dy;
	extreme(curx, cury);	/* wrong */
	p = makenode(TEXT, 2);
	p->o_val[0] = s;
	p->o_val[1] = type;
	return(p);
}

char *tostring(s)
register char *s;
{
	register char *p;

	p = malloc(strlen(s)+1);
	if (p == NULL) {
		fprintf(stderr, "pic: out of space in tostring on %s", s);
		exit(1);
	}
	strcpy(p, s);
	return(p);
}

makevar(s, t, v)	/* make variable named s in table */
char *s;
int t;
int v;	/* maybe wrong later */
{
	int i;

	for (i = 0; i < nsymtab; i++)
		if (strcmp(s, symtab[i].s_name) == 0)
			return(i);
	if (nsymtab >= SYMTAB) 
		yyerror("symbol table overflow (%d)", SYMTAB);
	symtab[nsymtab].s_name = tostring(s);
	symtab[nsymtab].s_type = t;
	symtab[nsymtab].s_val = v;
	return(nsymtab++);
}

lookup(s)	/* find s in symtab */
char *s;
{
	int i;

	for (i = 0; i < nsymtab; i++)
		if (strcmp(s, symtab[i].s_name) == 0)
			return(i);
	return(-1);
}

struct obj *makepos(x, y)	/* make a position cell */
coord x;
coord y;
{
	struct obj *p;

	p = makenode(PLACE, 0);
	p->o_x = x;
	p->o_y = y;
	return(p);
}

struct obj *getpos(p, corner)	/* find position of point */
struct obj *p;
int corner;
{
	coord x, y, x1, y1;

	dprintf("getpos %o %d\n", p, corner);
	x = p->o_x;
	y = p->o_y;
	x1 = p->o_val[0];
	y1 = p->o_val[1];
	switch (p->o_type) {
	case PLACE:
		break;
	case BOX:
		switch (corner) {
		case NORTH:	y += y1 / 2; break;
		case SOUTH:	y -= y1 / 2; break;
		case EAST:	x += x1 / 2; break;
		case WEST:	x -= x1 / 2; break;
		case NE:	x += x1 / 2; y += y1 / 2; break;
		case SW:	x -= x1 / 2; y -= y1 / 2; break;
		case SE:	x += x1 / 2; y -= y1 / 2; break;
		case NW:	x -= x1 / 2; y += y1 / 2; break;
		}
		break;
	case CIRCLE:
	case ELLIPSE:
		switch (corner) {
		case NORTH:	y += y1; break;
		case SOUTH:	y -= y1; break;
		case EAST:	x += x1; break;
		case WEST:	x -= x1; break;
		case NE:	x += 0.707 * x1; y += 0.707 * y1; break;
		case SE:	x += 0.707 * x1; y -= 0.707 * y1; break;
		case NW:	x -= 0.707 * x1; y += 0.707 * y1; break;
		case SW:	x -= 0.707 * x1; y -= 0.707 * y1; break;
		}
		break;
	case LINE:
	case ARROW:
	case MOVE:
		switch (corner) {
		case START:	break;	/* already in place */
		case END:	x = x1; y = y1; break;
		case CENTER:	x = (x+x1)/2; y = (y+y1)/2; break;
		case NORTH:	if (y1 > y) { x = x1; y = y1; } break;
		case SOUTH:	if (y1 < y) { x = x1; y = y1; } break;
		case EAST:	if (x1 > x) { x = x1; y = y1; } break;
		case WEST:	if (x1 < x) { x = x1; y = y1; } break;
		}
		break;
	case ARC:
		switch (corner) {
		case START:
			if (p->o_attr & CW_ARC) {
				x = p->o_val[2]; y = p->o_val[3];
			} else {
				x = x1; y = y1;
			}
			break;
		case END:
			if (p->o_attr & CW_ARC) {
				x = x1; y = y1;
			} else {
				x = p->o_val[2]; y = p->o_val[3];
			}
			break;
		}
		break;
	case SPLINE:
		switch (corner) {
		case END:
			x = p->o_val[0];
			y = p->o_val[1];
			break;
		}
		break;
	}
	dprintf("getpos returns %d %d\n", x, y);
	return(makepos(x, y));
}

struct obj *gethere(n)	/* make a place for curx,cury */
{
	dprintf("gethere %d %d\n", curx, cury);
	return(makepos(curx, cury));
}

struct obj *getlast(n, t)	/* find n-th previous occurrence of type t */
int n, t;
{
	int i, k;
	struct obj *p;

	k = n;
	for (i = nobj-1; i >= 0; i--) {
		p = objlist[i];
		if (p->o_type != t)
			continue;
		if (--k > 0)
			continue;	/* not there yes */
		dprintf("got a last of x,y= %d,%d\n", p->o_x, p->o_y);
		return(p);
	}
	yyerror("there is no %dth last", n);
}

struct obj *getfirst(n, t)	/* find n-th occurrence of type t */
int n, t;
{
	int i, k;
	struct obj *p;

	k = n;
	for (i = 0; i < nobj; i++) {
		p = objlist[i];
		if (p->o_type != t)
			continue;
		if (--k > 0)
			continue;	/* not there yes */
		dprintf("got a first of x,y= %d,%d\n", p->o_x, p->o_y);
		return(p);
	yyerror("there is no %dth ", n);
	}
}

struct obj *fixpos(p, x, y)
struct obj *p;
coord x, y;
{
	dprintf("fixpos returns %d %d\n", p->o_x + x, p->o_y + y);
	return(makepos(p->o_x + x, p->o_y + y));
}

struct obj *makenode(type, n)
int type, n;
{
	struct obj *p;

	p = (struct obj *) malloc(sizeof(struct obj) + (n-1)*sizeof(coord));
	if (p == NULL) {
		fprintf(stderr, "pic: out of space in makenode\n");
		exit(1);
	}
	p->o_type = type;
	p->o_count = n;
	p->o_mode = hvmode;
	p->o_x = curx;
	p->o_y = cury;
	p->o_nt1 = ntext1;
	p->o_nt2 = ntext;
	ntext1 = ntext;	/* ready for next caller */
	p->o_attr = p->o_dotdash = p->o_ddval = 0;
	if (nobj >= MAXOBJ) {
		fprintf(stderr, "pic: objlist overflow\n");
		exit(1);
	}
	objlist[nobj++] = p;
	return(p);
}

extreme(x, y)	/* record max and min x and y values */
{
	if (x > xmax)
		xmax = x;
	if (y > ymax)
		ymax = y;
	if (x < xmin)
		xmin = x;
	if (y < ymin)
		ymin = y;
}
