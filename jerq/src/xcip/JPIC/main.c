#include	<stdio.h>
#include	"pic.h"
#include	"y.tab.h"

struct	obj	*objlist[MAXOBJ];	/* store the elements here */
int	nobj	= 0;

struct attr	attr[40];	/* attributes stored here as collected */
int	nattr	= 0;	/* number of entries in attr_list */

struct	text	text[MAXTEXT];	/* text strings stored here as collected */
int	ntext	= 0;
int	ntext1	= 0;	/* record ntext here on entry to each figure */

coord	curx	= 0;
coord	cury	= 0;

int	hvmode	= R_DIR;	/* R => join left to right, D => top to bottom, etc. */

int	codegen	= 0;	/* 1=>output for this picture; 0=>no output */

float	deltx	= 6;	/* max x value in output, for scaling */
float	delty	= 6;	/* max y value in output, for scaling */
float	scale	= 0;	/* implies simply scaling by this value; no crop or shift */
int	dbg	= 0;
extern	FILE	*yyin;	/* input file pointer */
extern	int	yylval;
int	lineno	= 0;
char	*filename	= "-";
int	synerr	= 0;
char	*cmdname;
int	crop	= 1;	/* trim off exterior white space if non-zero */

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

main(argc, argv)
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
		makevar(tostring(defaults[i].name), VARNAME, defaults[i].val);
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
			/* yylval now contains 'E' or 'F' from .PE or .PF */
			if (buf[3] == ' ')	/* assume next thing is width */
				deltx = delty = atof(&buf[4]);
                        else {    /* use scale to determine size */
				int t;
				t = xmax - xmin;
				if (t == 0)
					t = ymax - ymin;
				deltx = delty = (float) t / (float)getvar("scale");
			}
			dprintf("deltx = %.3f\n", deltx);
			if (codegen && !synerr) {
				openpl(&buf[3]);	/* puts out .PS, with ht & wid stuck in */
				print();	/* assumes \n at end */
				closepl(yylval);	/* does the .PE/F */
			}
			fflush(stdout);
		}
		else
			fputs(buf, stdout);
	}
}

reset()
{
	struct obj *op;
	int i;
	struct symtab *p;
	extern int nstack;

	for (i = 0; i < nobj; i++) {
		op = objlist[i];
		if (op->o_type == BLOCK)
			freesymtab(op->o_val[6]);
		free(objlist[i]);
	}
	nobj = 0;
	nattr = 0;
	for (i = 0; i < ntext; i++)
		free(text[i].t_val);
	ntext = ntext1 = 0;
	codegen = synerr = 0;
	nstack = 0;
	curx = cury = 0;
	hvmode = R_DIR;
	sxmin = symin = 0;
	sxmax = symax = 4096;
	xmin = ymin = 30000;
	xmax = ymax = -30000;
}
