#include "ideal.h"
#include "y.tab.h"

boolean dbg = FALSE;
int when_bug = 0;
char *filename;
int lineno = 0;
char libstr[100];
boolean radflag = FALSE;
#ifdef DUMPVARS
boolean dumpvars = FALSE;
#endif

BOXPTR boxlist = NULL;

main(argc, argv)
int argc;
char *argv[];
{
	bug_off;
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'd':
			when_bug = argv[1][2]?atoi(&argv[1][2]):~0;
			break;
		case 'l':
			idinclude (&argv[1][2], LIBFIL);
			while (yyparse());
			break;
		case 'r':
			radflag = TRUE;
			break;
		case 'p':
		case '4':
		case 'n':
			break;
#ifdef DUMPVARS
		case 'v':
			dumpvars = TRUE;
			break;
#endif
		default:
			fprintf(stderr, "ideal: unknown flag %c\n", argv[1][1]);
			break;
		}
		argc--;
		argv++;
	}
	if (argc < 2) {
		filename = "-";
		lineno = 0;
		filepush (stdin);
		while (yyparse ());
	} else
		while (argc-- > 1) {
			filename = *argv;
			lineno = 0;
			idinclude (*++argv, CHATTY);
			while (yyparse ());
		}
	exit(0);
}

interpret()
{
	PUTNODE dummyroot;
	if (when_bug & 01) bug_on;
	else bug_off;
	dummyroot.name = lookup("main");
	dummyroot.parm = boxgen (lookup("main"), (STMTPTR) NULL);
	/* if boxlist includes main, execute it */
	if (boxlist && findbox(lookup("main"),TRUE)->stmtlist) {
		NOADPTR noadtree;
		bug_off;
		/* make room for all variables */
		noadtree = buildnoadtree (&dummyroot);
		/* solve all equations */
		eqneval (noadtree);
		nl_eval ();
		depvarkill ();
#ifdef DUMPVARS
		if (dumpvars)
			dumpnoad (noadtree);
#endif
		/* make a list of segments in the picture */
		noadtree->linelist = build (noadtree, noadtree->linelist);
		/* draw the thing */
		act (noadtree->linelist);
		/* free the thing, but save definitions */
		noadfree (noadtree);
		fflush (stdout);
		forget (lookup ("main"));
	}
}

idinclude (filnam, mode)
char *filnam;
int mode;
{
	FILE *nufile;
	char *lib, *getenv();
	dprintf "opening file %s\n", filnam);
	printf (".lf 1 %s\n", filnam);
	if (mode == CHATTY)
		nufile = fopen (filnam, "r");
	else if (mode == LIBFIL) {
		if (!(lib = getenv("LIBDIR")))
			lib = "/usr/lib/ideal/lib/";
		strcpy (libstr, lib);
		strcat (&libstr[0],filnam);
		filnam = libstr;
		nufile = fopen (filnam, "r");
	}
	filename = filnam;
	if (!nufile) {
		fprintf (stderr, "ideal: can't open file %s\n", filnam);
		exit (1);
	} else {
		filepush (nufile);
	}
}

act (the_picture)
LINEPTR the_picture;
{
	LINEPTR lineseg;
	double bounds[4];
#define	maxx	bounds[MAXX]
#define	maxy	bounds[MAXY]
#define	minx	bounds[MINX]
#define	miny	bounds[MINY]
	maxx = -10000.0;
	maxy = -10000.0;
	minx = 10000.0;
	miny = 10000.0;
	for (lineseg = the_picture; lineseg; lineseg = lineseg->next) {
		switch (lineseg->kind) {
		case LINE:
			bbline (lineseg, bounds);
			break;
		case CIRCLE:
			bbcirc ((CIRCPTR) lineseg, bounds);
			break;
		case ARC:
			bbarc ((ARCPTR) lineseg, bounds);
			break;
		case STRING:
			bbstring ((TEXTPTR) lineseg, bounds);
			break;
		case SPLINE:
			break;
		default:
			fprintf (stderr, "ideal: act: can't happen\n");
			break;
		}
	}
	if (the_picture) {
		boundscall (bounds);
	}
	for (lineseg = the_picture; lineseg; lineseg = lineseg->next) {
		switch (lineseg->kind) {
		case LINE:
			linecall (lineseg);
			break;
		case CIRCLE:
			circcall ((CIRCPTR) lineseg);
			break;
		case ARC:
			arccall ((ARCPTR) lineseg);
			break;
		case STRING:
			textcall ((TEXTPTR) lineseg);
			break;
		case SPLINE:
			splcall (((SPLPTR) lineseg)->knotlist);
			break;
		default:
			fprintf (stderr, "ideal: act: can't happen\n");
			break;
		}
	}
}

#ifdef DUMPVARS

/* print the variable values in the first level blocks of n */
dumpnoad(n)
NOADPTR n;
{
	VARPTR vp, vpi;
	NOADPTR np;
	char *bname;
	fprintf(stderr,"DUMP\n");
	for (np=n->son; np; np=np->brother) {
		bname=idprint(np->defnode->name);
		for (vp=np->boxvarlist ; vp ; ) {
			if(vp->re_name) {
				fprintf(stderr,"%s.%s ", bname,
					idprint(THENAME(vp)));
				depprint(vp->deplist);
				fprintf(stderr," ");
				/* next should be imag part */
				vpi=vp->next;
				if(!vpi || THENAME(vpi)!=THENAME(vp))
					exit(1);
				depprint(vpi->deplist);
				fprintf(stderr,"\n");
				vp=vpi;
			}
			vp = vp->next;
		}
	}
}

#endif
