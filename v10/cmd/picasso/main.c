/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:main.c	1.0			*/

#include	<ctype.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"picasso.h"
#include	"y.tab.h"

extern	int	yydebug;

YYSTYPE	origin;
obj	*objhead  = NULL;
obj	*objtail  = NULL;

Attr	*attr	  = NULL;	/* attributes stored here as collected */
int	nattrlist = 0;
int	nattr	  = 0;		/* number of entries in attr_list */

Text	*text	  = NULL;	/* text strings stored here as collected */
int	ntextlist = 0;		/* size of text[] array */
int	ntext	  = 0;
int	ntext1	  = 0;		/* record ntext here on entry to each figure */

float	*exprlist = NULL;
int	nexprlist = 0;
int	nexpr	  = 0;
float	curx	  = 0;
float	cury	  = 0;

rgb	*rgbtable = NULL;
int	nrgbtable = 0;
int	nrgb	  = 0;

int	hvmode	  = R_DIR;	/* R => left->right, D => top->bottom, etc. */
int	codegen	  = 0;		/* 1=>output for this picture; 0=>no output */
int	PEseen	  = 0;		/* 1=> PE seen during parsing */
int	pass_thru = 0;		/* 1=> prepare output to go through troff   */
int	nosqueeze = 0;		/* 0=> translate/scale to fit page    */
int	objbuf	  = 0;		/* >0 => print out objects as buffer fills */
int	pic_compat = 0;		/* !0 => interpret input PIC-style */
int	parsing   = 0;		/* controls entrance to eqn pipeline */
int	draftlayer= 0;
int	margin	  = 1;		/* margin (in points) around a picture	*/
float	magnification = 1.0;
float	eqn_move  = .03;	/* vertical adjustment for eqn stuff */
int	verbose	  = 0;
int	lineno	  = 0;
int	batch	  = 0;
char	*filename = "-";
int	synerr	  = 0;
int	anyerr	  = 0;		/* becomes 1 if synerr ever 1 */
char	*cmdname;
float	Gbox[4]	= { 32767, 32767, -32767, -32767 };
double	pght	= 11,
	pgwid	= 8.5;
valtype	cur_xform[6];
FILE	*textfp = stdout;
char	*gwblib = GWBFILES;
char	*fontdir = FONTDIR;

main(argc, argv)
	int	argc;
	char	*argv[];
{
extern	int	fpecatch();
extern	double	atof();
extern	char	*strchr();
extern	FILE	*tmpfile();
extern	char	*optarg;
extern	int	optind;
	int	n;
	char	*str, buf[20];

	signal(SIGFPE, fpecatch);
	cmdname = argv[0];
	while ((n = getopt(argc, argv, "b:F:I:l:m:M:p:e:tvx")) != EOF) {
		switch (n) {
		case 'I':
			gwblib = optarg;
			break;
		case 'F':
			fontdir = optarg;
			break;
		case 'b':
			objbuf = atoi(optarg);
			break;
		case 'l':
			draftlayer = -atoi(optarg);
			break;
		case 'm':
			margin = atoi(optarg);
			break;
		case 'M':
			magnification = atof(optarg);
			break;
		case 'e':
			eqn_move = atof(optarg);
		case 'p':
			if ((str=strchr(optarg,'x')) == NULL) {
				if ((pght=atof(optarg)) <= 0)
					pght = 11;
			}
			else {
				if ((pgwid=atof(optarg)) <= 0)
					pgwid = 8.5;
				if ((pght=atof(str+1)) <= 0)
					pght = 11;
			}
			break;
		case 't':
			pass_thru = 1;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'x':
			nosqueeze = 1;
			break;
		}
	}
	batch = (! Xokay()) || pass_thru || nosqueeze;
	if (!batch) { /* if otherwise interactive but stdin is a pipe,
			 assume we should batch process the input */
		struct	stat	statbuf;

/*
		if (fstat(fileno(stdin), &statbuf) == 0)
			if ((statbuf.st_mode & S_IFMT) == S_IFIFO ||
					(statbuf.st_mode & S_IFMT) == S_IFREG)
*/
				batch = 1;
	}
	if (batch && optind >= argc && isatty(fileno(stdin)) == 1) {
		fprintf(stderr, "usage: %s  -t file | troff-pipeline\n\
   or: %s file > postscript-file\n\
   or: other-command file | %s options ...\n", argv[0], argv[0], argv[0]);
		exit(1);
	}
	argc -= --optind;
	argv += optind;
	if (pass_thru && (textfp = tmpfile()) == NULL) {
		fprintf(stderr, "%s: can't open internal temporary file\n",
				cmdname);
		exit(1);
	}
	text = (Text *) grow((char *)text, "text", ntextlist += 1000,
								sizeof(Text));
	attr = (Attr *) grow((char *)attr, "attr", nattrlist += 100,
								sizeof(Attr));
	exprlist = (float *) grow((char *)exprlist, "exprlist",
					nexprlist += 256, sizeof(float));
	devinit("post");
	sprintf(buf, "/%d/", getpid());
	pushsrc(pString, buf);
	definition("pid");
	origin.o = objhead = (obj *) calloc(1, sizeof(obj));
		   objtail = (obj *) calloc(1, sizeof(obj));
	objhead->o_next = objtail;
	objtail->o_prev = objhead;
	objhead->o_type = PLACE;
	makevar(tostring("O"),PLACENAME,origin);
	primaries();
	setdefaults();

	pushsrc(File, curfile = infile);
	if (argc <= 1) {
		curfile->fin = batch ? stdin : NULL;
		curfile->fname = tostring("-");
		getdata(curfile);
	} else {
		batch = 1;	/* because file names specified */
		while (argc-- > 1) {
			if ((curfile->fin = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "%s: can't open %s\n",
								cmdname, *argv);
				exit(1);
			}
			curfile->fname = tostring(*argv);
			getdata(curfile);
			fclose(curfile->fin);
			free(curfile->fname);
		}
	}
	if (!batch)
		Xloop();
	if (pass_thru) {
		fflush(textfp);
		rewind(textfp);
		while(fgets(buf, BUFSIZ, textfp) != NULL)
			fputs(buf, stdout);
		fclose(textfp);
	}
	else
		if (batch)
			endpl();
		else
			Xendpl();
	exit(anyerr);
}

struct clor {
	char	*c_name;
	float	c_red;
	float	c_green;
	float	c_blue;
	unsigned long	c_pixel;
    } dfcols[] = {
	{ "black",	0., 0., 0., 0 },	/* Indexes 0 and 1 correspond */
	{ "white",	1., 1., 1., 0 },	/* to grayscale values 0 and 1*/
	{ "red",	1., 0., 0., 0 },	/* This "pun" is deliberate;  */
	{ "green",	0., 1., 0., 0 },	/* gray output depends on it. */
	{ "blue",	0., 0., 1., 0 },
	{ "yellow",	1., 1., 0., 0 },
	{ "magenta",	1., 0., 1., 0 },
	{ "cyan",	0., 1., 1., 0 },
	{ "lightgrey",	0.6588, 0.6588, 0.6588, 0 },
	{ "navy",	0.1372, 0.1372, 0.5568, 0 },
	{ "skyblue",	0.196, 0.6, 0.8, 0 },
	{ "orange",	0.8, 0.2, 0.2, 0 },
	{ "sienna",	0.5568, 0.4196, 0.1372, 0 },
	{ "palegreen",	0.5607, 0.7372, 0.5607, 0 },
	{ "darkgreen",	0.1843, 0.3098, 0.1843, 0 },
	{ "blueviolet",0.6235, 0.3725, 0.6235, 0 },
	{ NULL,		0., 0., 0., 0 }
};

primaries ()			/* set up initial 8-color color table	*/
{
	int	i;
	char	*s;

	for (i = 0; (s = dfcols[i].c_name) != NULL; i++)
		makeindex(s, dfcols[i].c_red, dfcols[i].c_green,
						dfcols[i].c_blue);
}

makeindex (s, r, g, b)		/* simulate user color definition for s */
	char	*s;
	double	r, g, b;
{
	YYSTYPE	v;

	exprlist[0] = r; exprlist[1] = g; exprlist[2] = b; nexpr = 3;
	v.f = setrgbindex();
	makevar(tostring(s), VARNAME, v);
}

fpecatch()
{
	fatal("floating point exception");
}

char *grow(ptr, name, num, size)	/* make array bigger */
	char *ptr, *name;
	int num, size;
{
	char *p;

	if (ptr == NULL)
		p = malloc(num * size);
	else
		p = realloc(ptr, num * size);
	if (p == NULL)
		fatal("can't grow %s to %d", name, num * size);
	return p;
}

struct {
	char *name;
	float val;
	short scalable;		/* 1 => adjust when scale changes */
} defaults[] ={
	"flatness",     0,	0,
	"miterlimit",	0,	0,
	"linecap",	-1,	0,
	"linejoin",	-1,	0,
	"textsize",	10,	0,
	"textspace",	12,	0,
	"textcolor",	0,	0,
	"textfont",	0,	0,
	"moveht",	HT,	1,
	"movewid",	HT,	1,
	"arrowht",	HT5,	1,
	"arrowwid",	HT10,	1,
	"arrowend",	0,	0,
	"arrowfill",	1,	0,	/* arrowhead style */
	"lineht",	HT,	1,
	"linewid",	HT,	1,
	"linerad",	0,	1,
	"dashwid",	HT10,	1,
	"boxht",	HT,	1,
	"boxwid",	WID,	1,
	"boxrad",	0,	1,
	"ellipseht",	HT,	1,
	"ellipsewid",	WID,	1,
	"arcrad",	HT2,	1,
	"circlerad",	HT2,	1,
	"fillcolor",	0,	0,
	"linecolor",	0,	0,
	"lineweight",	0,	1,
	"curlayer",	0,	0,	/* current drawing layer  */
	NULL, 0
	};

int	top_layer = 0;			/* high-water mark */

setdefaults()	/* set default sizes for variables like boxht */
{
	int i;
	YYSTYPE v;

	for (i = 0; defaults[i].name != NULL; i++) {
		v.f = defaults[i].val;
		makevar(tostring(defaults[i].name), VARNAME, v);
	}
}

resetvar()	/* reset variables listed */
{
	int i, j;

	if (nattr == 0) {	/* none listed, so do all */
		setdefaults();
		pic_compat = 0;
		return;
	}
	for (i = 0; i < nattr; i++) {
		if (strcmp(attr[i].a_val.p, "p_i_c") == 0)
			pic_compat = 1;
		else for (j = 0; defaults[j].name != NULL; j++)
			if (strcmp(defaults[j].name, attr[i].a_val.p) == 0) {
				setfval(defaults[j].name, defaults[j].val);
				break;
			}
	}
}

static float savevars[sizeof defaults / sizeof defaults[0]];

savepicvars()	/* save current values, restore original values */
{
	int 	i;
	char	*s;

	for (i = 0; (s = defaults[i].name) != NULL; i++) {
		savevars[i] = getfval(s);
		setfval(s, defaults[i].val);
	}
}

restorepicvars()	/* restore values saved above */
{
	int	i;
	char	*s;

	for (i = 0; (s = defaults[i].name) != NULL; i++)
		setfval(s, savevars[i]);
}

checkscale(v)	/* adjust default variables dependent on scale */
	double	v;
{
	int	i;

	for (i = 1; defaults[i].name != NULL; i++) {
		if (defaults[i].scalable)
			setfval(defaults[i].name,
				defaults[i].val * v);
	}
}

getdata()
{
	char *p, buf[1000], buf1[100];
	int ln;
	int ineqn = 0;

	if (curfile->fin == NULL) {	/* only happens in interactive mode */
		batch = 1;
		reset();
		batch = 0;
		ln = nosqueeze;
		nosqueeze = 1;
		Xopenpl("");
		Xprint('E');
		Xallexpose();
		nosqueeze = ln;
		return;
	}
	curfile->lineno = 0;
	while (fgets(buf, sizeof buf, curfile->fin) != NULL) {
		curfile->lineno++;
		if (*buf == '.' && buf[1] == 'P' && buf[2] == 'S') {
			for (p = &buf[3]; *p == ' '; p++)
				;
			if (*p++ == '<') {
				Infile svfile;
				svfile = *curfile;
				sscanf(p, "%s", buf1);
				if ((curfile->fin=fopen(buf1, "r")) == NULL)
					fatal("can't open %s", buf1);
				curfile->fname = tostring(buf1);
				getdata();
				fclose(curfile->fin);
				free(curfile->fname);
				*curfile = svfile;
				if (batch)
					continue;
				else
					break;
			}
			reset();
			parsing = 1;
			yyparse();
			parsing = 0;
			anyerr += synerr;
	/* added for PIC compatibility -- 4/18/90 -- DBK */
			scale_pic();
	/* end of PIC compatibility addition */
			/* yylval.i now contains 'E', 'F' or 'N' from .PE etc.*/

#if 0
			if (codegen && !synerr) {
				if (batch) {
					openpl(buf+3);
					print(yylval.i);
					closepl(buf+3);	/* assumes \n at end */
				}
				else {
					ln = nosqueeze;
					nosqueeze = 1;
					Xopenpl(buf+3);
					Xprint(yylval.i);
					Xallexpose();
					nosqueeze = ln;
				}
				break;
			}
#else
	/* this is a change made 2/20/90 by DBK */
		/* and a further change 4/13/90 */
		/* if batch mode
			if some objects and no errors, print page
			read rest of file
		   if interactive mode
			if any objects
			     display objects
			     abandon file
			else
			     read more of file     */
	/* behaves same way in batch mode */
	/* tries to display data up to this point in interactive, but
	   throws away rest of file */
			if (codegen && !synerr && batch) {
				openpl(buf+3);
				print(yylval.i);
				closepl(buf+3);	/* assumes \n at end */
			}
/*			else if (codegen && !batch) {		*/
			else if (!batch) {	/* no codegen for config */
				ln = nosqueeze;
				nosqueeze = 1;
				Xopenpl(buf+3);
				Xprint(yylval.i);
				Xallexpose();
				nosqueeze = ln;
				break;
			}
#endif
		}
		else if (buf[0] == '.' && buf[1] == 'l' && buf[2] == 'f') {
			if (sscanf(buf+3, "%d %s", &ln, buf1) == 2) {
				free(curfile->fname);
				curfile->fname = tostring(buf1);
				curfile->lineno = ln;	/* off by one?? */
			}
		}
		else if (pass_thru) {
			fputs(buf, textfp);
			if (*buf == '.')	/* track .ps, .ft, .vs, .ec  */
				troff(buf);
			else {
				int	dumft = 0, dumps = 0;
				while (parse_text(buf, &dumft, &dumps))
					;
			}
		}
		if (buf[0] == '.' && buf[1] == 'E' && buf[2] == 'Q')
			ineqn = 1;
		else if (buf[0] == '.' && buf[1] == 'E' && buf[2] == 'N')
			ineqn = 0;
		else if (ineqn == 1)
			scan_delim(buf);
		/* should also check no_eqn--DBK--3/29/90 */
	}
}

reset()
{
extern	int	nstack, errno;
	obj	*op, *op1;
	int	i;

	
	if (batch) {
	    for (op = objhead->o_next; op != objtail; ) {
		if (op->o_type == BLOCK)
			freesymtab(op->o_val[N_VAL+1].s);
		op1 = op;
		op = op->o_next;
		free(op1);
	    }
	    for (i = 0; i < ntext; i++) {
		if (text[i].t_val)
			free(text[i].t_val);
		text[i].t_line = 0;
	    }
	    ntextlines = ntext = ntext1 = 0;
	    nstack = 0;
	    Gbox[0] = Gbox[1] = 32767;
	    Gbox[2] = Gbox[3] = -32767;
	}
        objhead->o_next = objtail;
	objtail->o_prev = objhead;
	nattr = 0;
	objcount = top_layer = 0;
	errno = codegen = synerr = 0;
	curx = cury = 0;
	PEseen = 0;
	hvmode = R_DIR;
	cur_xform[0].f = cur_xform[3].f = 1;
	cur_xform[1].f = cur_xform[2].f =
	cur_xform[4].f = cur_xform[5].f = 0;
	if (!batch)
		Xclosepl("");
}
