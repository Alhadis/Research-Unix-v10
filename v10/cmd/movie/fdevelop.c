/* fdevelop: convert script to intermediate language in two passes
	Input defined in script.def, output in int.def
	Pass 1 reads named file or stdin, writes intermediate file
	Pass 2 reads intermediate, writes output
	Intermediate file is like output, with these differences:
		Header lines (d v, d c, etc.) absent
		Erase commands: refer to line numbers in int file and
			do not have geometric command following
		Geometry commands: numbers not yet scaled, slots not
			assigned
   fdevelop is a filter, transforming stdin or one named file to stdout
	the shell script develop uses this program to develop foo.s
	  into foo.i, if needed
 */

#include "fdevelop.h"

#define SCALE	9999

extern	dox(), doy();
extern float	stof();

/* SLOTS */
int	slotmax = 10000;

/* STATUS OF EACH LINE IN INTERMEDIATE FILE*/
int	linemax = 50000;
char	*linevec;	/* malloc'ed to [linemax] before pass 1 */
#define MAXVIEWS	10
			  /* if >10 views, change one-byte getc in pass2 */
#define	ERASED		11
#define	NOTGEOM		12

/* VIEWS */
extern 		recordx(), recordy();
extern int	scalex(), scaley();
int	viewcnt = 0;
struct	Viewelmt {
	char	name[MAXSTR];	/* character string name */
	float	maxx;		/* range of values, both coords */
	float	minx;
	float	maxy;
	float	miny;
	float	factorx;	/* factors used by scalex and scaley */
	float	factory;
	int	initx;		/* max and min set yet? */
	int	inity;
}	viewarr[MAXVIEWS];

/* CLICKS */
#define	MAXCLICKS	30
int	clickcnt = 0;
char	clickname [MAXCLICKS] [MAXSTR];

/* OPTIONS FOR GEOMETRIC OBJECTS */
extern	getoptions();
#define OPTMAX 10
char	optstring[OPTMAX];
#define TEXT	0
#define LINE	1
#define BOX	2
#define CIRCLE	3
char	*defaulttab[] = { /* match with above defines */
	"cm7",	/* TEXT:   center, medium, white	*/
	"s-7",	/* LINE:   solid, -, white		*/
	"n7",	/* BOX:    nofill, white		*/
	"n7"	/* CIRCLE: nofill, white		*/
};
struct	Geoelmt {
	int 	cmd;
	int	pos;
	char	*name;
	char	val;
} geooptarr[] = {
	TEXT,	0,	"center",	'c',
	TEXT,	0,	"ljust",	'l',
	TEXT,	0,	"rjust",	'r',
	TEXT,	0,	"above",	'a',
	TEXT,	0,	"below",	'b',
	TEXT,	1,	"medium",	'm',
	TEXT,	1,	"small",	's',
	TEXT,	1,	"big",		'b',
	TEXT,	1,	"bigbig",	'B',
	TEXT,	2,	"c0",		'0',
	TEXT,	2,	"c1",		'1',
	TEXT,	2,	"c2",		'2',
	TEXT,	2,	"c3",		'3',
	TEXT,	2,	"c4",		'4',
	TEXT,	2,	"c5",		'5',
	TEXT,	2,	"c6",		'6',
	TEXT,	2,	"c7",		'7',
	TEXT,	2,	"black",	'0',
	TEXT,	2,	"red",		'1',
	TEXT,	2,	"green",	'2',
	TEXT,	2,	"yellow",	'3',
	TEXT,	2,	"blue",		'4',
	TEXT,	2,	"magenta",	'5',
	TEXT,	2,	"cyan",		'6',
	TEXT,	2,	"white",	'7',
	LINE,	0,	"solid",	's',
	LINE,	0,	"fat",		'f',
	LINE,	0,	"fatfat",	'F',
	LINE,	0,	"dotted",	'o',
	LINE,	0,	"dashed",	'a',
	LINE,	1,	"-",		'-',
	LINE,	1,	"->",		'>',
	LINE,	1,	"<-",		'<',
	LINE,	1,	"<->",		'x',
	LINE,	2,	"c0",		'0',
	LINE,	2,	"c1",		'1',
	LINE,	2,	"c2",		'2',
	LINE,	2,	"c3",		'3',
	LINE,	2,	"c4",		'4',
	LINE,	2,	"c5",		'5',
	LINE,	2,	"c6",		'6',
	LINE,	2,	"c7",		'7',
	LINE,	2,	"black",	'0',
	LINE,	2,	"red",		'1',
	LINE,	2,	"green",	'2',
	LINE,	2,	"yellow",	'3',
	LINE,	2,	"blue",		'4',
	LINE,	2,	"magenta",	'5',
	LINE,	2,	"cyan",		'6',
	LINE,	2,	"white",	'7',
	BOX,	0,	"nofill",	'n',
	BOX,	0,	"fill",		'f',
	BOX,	1,	"c0",		'0',
	BOX,	1,	"c1",		'1',
	BOX,	1,	"c2",		'2',
	BOX,	1,	"c3",		'3',
	BOX,	1,	"c4",		'4',
	BOX,	1,	"c5",		'5',
	BOX,	1,	"c6",		'6',
	BOX,	1,	"c7",		'7',
	BOX,	1,	"black",	'0',
	BOX,	1,	"red",		'1',
	BOX,	1,	"green",	'2',
	BOX,	1,	"yellow",	'3',
	BOX,	1,	"blue",		'4',
	BOX,	1,	"magenta",	'5',
	BOX,	1,	"cyan",		'6',
	BOX,	1,	"white",	'7',
	CIRCLE,	0,	"nofill",	'n',
	CIRCLE,	0,	"fill",		'f',
	CIRCLE,	1,	"c0",		'0',
	CIRCLE,	1,	"c1",		'1',
	CIRCLE,	1,	"c2",		'2',
	CIRCLE,	1,	"c3",		'3',
	CIRCLE,	1,	"c4",		'4',
	CIRCLE,	1,	"c5",		'5',
	CIRCLE,	1,	"c6",		'6',
	CIRCLE,	1,	"c7",		'7',
	CIRCLE,	1,	"black",	'0',
	CIRCLE,	1,	"red",		'1',
	CIRCLE,	1,	"green",	'2',
	CIRCLE,	1,	"yellow",	'3',
	CIRCLE,	1,	"blue",		'4',
	CIRCLE,	1,	"magenta",	'5',
	CIRCLE,	1,	"cyan",		'6',
	CIRCLE,	1,	"white",	'7'
};
int	geooptcnt = (sizeof(geooptarr) / sizeof(struct Geoelmt));

/* VARIABLES FOR LEXICAL ANALYSIS */
char	buf[MAXBUF];
int	lexsaweof = 0;

/* UTILITY VARIABLES */
char	*NULLSTR = "";
char	*cmdname;
int	lineno, outlineno;
char	*tempfname;
FILE	*infp, *tempfp;

extern double atof();
#define STOF(s)	((float) atof(s))

main(argc, argv)
	char *argv[];
{
	register int	i;

	cmdname = argv[0];
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'l':
			linemax = atoi(&argv[1][2]);
			break;
		case 's':
			slotmax = atoi(&argv[1][2]);
			break;
		}
		argc--;
		argv++;
	}
	if (argc == 1)
		infp = stdin;
	else {
		if ((infp = fopen(argv[1], "r")) == NULL)
			error(FATAL, "can't open input file");
	}
	linevec = emalloc(linemax);
	for (i = 0; i < linemax; i++)
		linevec[i] = 0;
	tempfname = "/tmp/dev.XXXXXX";
	mktemp(tempfname);
	/* DEBUG tempfname = "djunk"; */
	if ((tempfp = fopen(tempfname, "w")) == NULL)
		error(FATAL, "can't open temp file");
	pass1();
	fclose(tempfp);
	if ((tempfp = fopen(tempfname, "r")) == NULL)
		error(FATAL, "can't open temp file");
	pass2();
	unlink(tempfname); /* DEBUG */
}

pass1()
{
	int	currentview;		/* view number */
	int	erased[MAXVIEWS];	/* last line erased */
	int	badlabel;		/* 1 if label, 0 if geom */
	char	savelabel[MAXSTR];	/* label */
	int	geomalready = 0;	/* seen any geometry yet? */
	struct Symbol	*sp;
	int	i;

#define	DOGEOM		badlabel = 0;\
			geomalready = 1;\
			linevec[outlineno] = currentview;

#define DOERASE(L)	linevec[(L)] = ERASED;\
			linevec[outlineno] = NOTGEOM;\
			fprintf(tempfp, "e\t%d\n", (L));\
			outlineno++;

	/* init */
	currentview = 0;
	for (i = 0; i < MAXVIEWS; i++)
		erased[i] = 0;
	opensymtab();

	/* read and process file */
	lineno = 0;
	outlineno = 1;
	while (moreinput(infp)) {
		if (++lineno > linemax)
			error(FATAL, "too many input lines");
		/* fprintf(stderr, "STARTING TO PROCESS INPUT LINE %d\n",
			lineno); */
		linevec[outlineno] = NOTGEOM;
		lex(infp);
		if (buf[0] == '#') {
			gobble2(infp);
			continue;
		}
		badlabel = 0;
		if (buf[strlen(buf)-1] == ':') {
			buf[strlen(buf)-1] = '\0';
			badlabel = 1;
			strcpy(savelabel, buf);
			sp = lookup(buf, currentview);
			if (sp != NULL) {
				if (linevec[sp->outnum] == currentview) {
					DOERASE(sp->outnum)
				}
				sp->outnum = outlineno;
			} else {
				insert(buf, currentview, outlineno);
			}
			lex(infp);
		}
		if (eq(buf, "text")) {
			DOGEOM
			getoptions(TEXT);
			fprintf(tempfp, "g\t0\tt\t%d\t%s",
				currentview, optstring);
			dox(currentview);
			lex(infp);
			doy(currentview);
			lexstr(infp);
			fprintf(tempfp, "\t%s\n", buf);
			outlineno++;
		} else if (eq(buf, "circle")) {
			float savex, rad;
			DOGEOM
			getoptions(CIRCLE);
			fprintf(tempfp, "g\t0\tc\t%d\t%s",
				currentview, optstring);
			dox(currentview);
			savex = STOF(buf);
			lex(infp);
			doy(currentview);
			lex(infp);
			if (!strisnum(buf)) {
				error(WARN, "radius not a number");
				strcpy(buf, "0");
			}
			rad = STOF(buf);
			if (rad < 0.0)
				error(WARN, "radius is negative");
			recordx(currentview, savex-rad);
			recordx(currentview, savex+rad);
			fprintf(tempfp, "\t%s\n", buf);	
			outlineno++;
			gobble(infp);
		} else if (eq(buf, "line") || eq(buf, "box")) {
			char cmdchar;
			DOGEOM
			if (eq(buf, "line")) {
				getoptions(LINE);
				cmdchar = 'l';
			} else {
				getoptions(BOX);
				cmdchar = 'b';
			}
			fprintf(tempfp, "g\t0\t%c\t%d\t%s",
				cmdchar, currentview, optstring);
			dox(currentview);
			lex(infp);
			doy(currentview);
			lex(infp);
			dox(currentview);
			lex(infp);
			doy(currentview);
			fprintf(tempfp, "\n");
			outlineno++;
			gobble(infp);
		} else if (eq(buf, "view")) {
			lex(infp);
			if (eq(buf,NULLSTR)) {
				error(WARN, "no name in view statement");
				strcpy(buf, "def.view");
			}
			if (viewcnt == 0 && geomalready)
				error(WARN, "first view after geom");
			for (i = 0; i < viewcnt; i++)
				if (eq(buf, viewarr[i].name))
					break;
			if (i >= viewcnt) {
				viewcnt++;
				if (i >= MAXVIEWS)
					error(FATAL, "too many views");
				strcpy(viewarr[i].name, buf);
				viewarr[i].initx = viewarr[i].inity = 0;
			}
			currentview = i;
			gobble(infp);
		} else if (eq(buf, "click")) {
			lex(infp);
			if (eq(buf,NULLSTR))
				strcpy(buf, "def.click");
			for (i = 0; i < clickcnt; i++)
				if (eq(buf, clickname[i]))
					break;
			if (i >= clickcnt) {
				clickcnt++;
				if (i >= MAXCLICKS)
					error(FATAL, "too many click names");
				strcpy(clickname[i], buf);
			}
			fprintf(tempfp, "c\t%d\n", i);
			outlineno++;
			gobble(infp);
		} else if (eq(buf, "erase")) {
			lex(infp);
			if (eq(buf,NULLSTR))
				error(WARN, "no label in erase statement");
			sp = lookup(buf, currentview);
			if (sp != NULL) {
				if (linevec[sp->outnum] == currentview) {
					DOERASE(sp->outnum)
				}
				delete(buf, currentview);
			} else {
				error(WARN, "undefined label");
			}
			gobble(infp);
		} else if (eq(buf, "clear")) {
			int i, endline;
			fprintf(tempfp, "b\ts\t%d\n", currentview);
			endline = outlineno++;
			for (i = erased[currentview]+1; i <= endline; i++) {
				if (linevec[i] == currentview) {
					DOERASE(i)
				}
			}
			erased[currentview] = outlineno;
			fprintf(tempfp, "b\te\t%d\n", currentview);
			linevec[outlineno] = NOTGEOM;
			outlineno++;
			gobble(infp);
		} else {
			if (!eq(buf, NULLSTR))
				error(WARN, "unrecognized command");
			gobble(infp);
		}
		if (badlabel) {
			error(WARN, "label on nongeometric object");
			delete(savelabel, currentview);
		}
	}
	/* tidy up */
	lineno = 0;
	closesymtab();
	if (viewcnt == 0) {
		viewcnt = 1;
		strcpy(viewarr[0].name, "def.view");
	}
}


pass2()
{
	typedef struct Slot {
		union {
			int	i;
			char	*p;
		} v;
	} Slot;
	char	cmd;		/* first char on a line */
	int	tlineno;	/* line number in temp file */
	int	slothead;	/* ptr to free list within slots */
	Slot	*slotarr;	/* init by malloc to slotmax elmts */
	int	i;
	int	c;
	int	v;
	struct Viewelmt *vp;

	/* Init */
	opensymtab();
	slotarr = (Slot *) emalloc(slotmax * sizeof(Slot));
	for (i = 1; i <= slotmax-2; i++)
		slotarr[i].v.i = i+1;
	slothead = 1;
	/* Write header for output file */
	for (i = 0; i < viewcnt; i++) {
		vp = &viewarr[i];
		printf("d\tv\t%d\t%s\t%g\t%g\t%g\t%g\n", i,
			vp->name, vp->minx, vp->miny, vp->maxx, vp->maxy);
	}
	for (i = 0; i < clickcnt; i++)
		printf("d\tc\t%d\t%s\n", i, clickname[i]);
	printf("d\tp\te\n");
	/* Calculate view factors used to scale */
	for (v = 0; v < viewcnt; v++) {
		vp = &viewarr[v];
		if (vp->minx == vp->maxx) {
			vp->minx = 0.0;
			vp->maxx = 2*vp->maxx;
			if (vp->maxx == 0.0) {
				vp->minx = -1.0;
				vp->maxx =  1.0;
			}
		}
		vp->factorx = SCALE / (vp->maxx-vp->minx);
		if (vp->miny == vp->maxy) {
			vp->miny = 0.0;
			vp->maxy = 2*vp->maxy;
			if (vp->maxy == 0.0) {
				vp->miny = -1.0;
				vp->maxy =  1.0;
			}
		}
		vp->factory = SCALE / (vp->maxy-vp->miny);
	}
	/* Read and process intermediate file */
	tlineno = 0;
	while ((c = getc(tempfp)) != EOF) {
		tlineno++;
		cmd = c;
		getc(tempfp);	/* gobble tab */
		putchar(cmd);
		putchar('\t');
		if (cmd == 'g') {
			int snum, vnum;
			char gcmd;
			char opts[OPTMAX];
			float x1, y1, x2, y2;
			int   i1, j1, i2, j2;
			char bufa[MAXBUF+100];
			/* fscanf(tempfp, "%d %c %d", &snum, &gcmd, &vnum); */
			  getc(tempfp); /* don't need snum -- always zero */
			  getc(tempfp);	/* gobble tab */
			  gcmd = getc(tempfp);
			  getc(tempfp);	/* gobble tab */
			  vnum = getc(tempfp) - '0';
			fscanf(tempfp, "%s %f %f", opts, &x1, &y1);
			if (linevec[tlineno] != ERASED)
				snum = 0;
			else {
				snum = slothead;
				slothead = slotarr[snum].v.i;
				if (slothead == 0)
					error(FATAL, "ran out of slots");
			}
			insert(NULLSTR, tlineno, snum);
			i1 = scalex(vnum, x1);
			j1 = scaley(vnum, y1);
			if (gcmd == 'b' || gcmd == 'l') {
				fscanf(tempfp, "%f %f", &x2, &y2);
				if (getc(tempfp) != '\n')
					error(FATAL, "develop bug: missing newline");
				i2 = scalex(vnum, x2);
				j2 = scaley(vnum, y2);
				if (gcmd == 'b') {
					int t; /* normalize: min, max */
					if (i1 > i2) { t=i1; i1=i2; i2=t; }
					if (j1 > j2) { t=j1; j1=j2; j2=t; }
				}
				sprintf(bufa, "%d\t%c\t%d\t%s\t%d\t%d\t%d\t%d",
					snum, gcmd, vnum, opts, i1, j1, i2, j2);
			} else if (gcmd == 'c') {
				fscanf(tempfp, "%f", &x2);
				if (getc(tempfp) != '\n')
					error(FATAL, "develop bug: missing newline");
				i2 = scalex(vnum, x1+x2) - i1;
				if (i2 < 0)
					i2 = -i2;
				if (i2 == 0)
					i2 = 1;
				sprintf(bufa, "%d\t%c\t%d\t%s\t%d\t%d\t%d",
					snum, gcmd, vnum, opts, i1, j1, i2);
			} else if (gcmd == 't') {
				getc(tempfp); /* gobble tab */
				lexrest(tempfp);
				sprintf(bufa, "%d\t%c\t%d\t%s\t%d\t%d\t%s",
					snum, gcmd, vnum, opts, i1, j1, buf);
			} else error(FATAL, "develop bug: invalid g cmd");
			slotarr[snum].v.p = emalloc(strlen(bufa)+1);
			strcpy(slotarr[snum].v.p, bufa);
			puts(bufa);
		} else if (cmd == 'e') {
			int linenum, slotnum;
			Symbol *sp;
			fscanf(tempfp, "%d", &linenum);
			if (getc(tempfp) != '\n')
				error(FATAL, "develop bug: missing newline");
			sp = lookup(NULLSTR, linenum);
			if (sp == NULL)
				error(FATAL, "develop bug: bad erase lookup");
			slotnum = sp->outnum;
			puts(slotarr[slotnum].v.p);
			efree(slotarr[slotnum].v.p);
			slotarr[slotnum].v.i = slothead;
			slothead = slotnum;
			delete(NULLSTR, linenum);
		} else {
			lexrest(tempfp);
			puts(buf);
		}
	}
	/* tidy up */
		/* closesymtab(); delete stuff in slots? */
}

recordx(vnum, t)
	int vnum;
	float t;
{
	struct Viewelmt	*vp;

	vp = &viewarr[vnum];
	if (vp->initx != 1) {
		vp->initx = 1;
		vp->minx = t;	
		vp->maxx = t;
	} else {
		if (t < vp->minx)
			vp->minx = t;	
		if (t > vp->maxx)
			vp->maxx = t;
	}
}

recordy(vnum, t)
	int vnum;
	float t;
{
	struct Viewelmt	*vp;

	vp = &viewarr[vnum];
	if (vp->inity != 1) {
		vp->inity = 1;
		vp->miny = t;	
		vp->maxy = t;
	} else {
		if (t < vp->miny)
			vp->miny = t;	
		if (t > vp->maxy)
			vp->maxy = t;
	}
}

int scalex(vnum, t)
	int vnum;
	float t;
{
	struct Viewelmt	*vp;

	vp = &viewarr[vnum];
	return (int) ((t - vp->minx) * vp->factorx);
}

int scaley(vnum, t)
	int vnum;
	float t;
{
	struct Viewelmt	*vp;

	vp = &viewarr[vnum];
	return (int) ((t - vp->miny) * vp->factory);
}

getoptions(cmdtype)	/* put options into optstring */
	int cmdtype;
{
	int i;
	struct Geoelmt *gp;

	strcpy(optstring, defaulttab[cmdtype]);
	for (lex(infp); !eq(buf,NULLSTR) && !strisnum(buf); lex(infp)) {
		for (i = 0; i < geooptcnt; i++) {
			gp = &geooptarr[i];
			if (cmdtype == gp->cmd && eq(buf, gp->name))
				break;
		}
		if (i < geooptcnt)
			optstring[gp->pos] = gp -> val;
		else
			error(WARN, "unrecognized option");
	}
}

dox(view)	/* handle x in input file */
	int view;
{
	if (!strisnum(buf)) {
		error(WARN, "x value not a number");
		strcpy(buf, "0");
	}
	recordx(view, STOF(buf));
	fprintf(tempfp, "\t%s", buf);	
}

doy(view)	/* handle y in input file */
	int view;
{
	if (!strisnum(buf)) {
		error(WARN, "y value not a number");
		strcpy(buf, "0");
	}
	recordy(view, STOF(buf));
	fprintf(tempfp, "\t%s", buf);	
}
