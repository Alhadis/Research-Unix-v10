static	char *sccsid = "@(#)main.c	8th Edition (Bell Labs) 85/10/28";
# include "defs"
/*
command make to update programs.
Flags:	'd'  print out debugging comments
	'p'  print out a version of the input graph
	's'  silent mode--don't print out commands
	'f'  the next argument is the name of the description file;
	     "makefile" is the default
	'i'  ignore error codes from the shell
	'S'  stop after any command fails (normally do parallel work)
	'n'  don't issue, just print, commands
	't'  touch (update time of) files but don't issue command
	'q'  don't do anything, but check if object is up to date;
	     returns exit code 0 if up to date, -1 if not
	'e'  use environment macros after rather than before makefiles
	'o'  use old suffix rules instead of % patterns
	'Pn' set process limit to n
	'z'  always use shell, never issue commands directly
*/

nameblkp mainname	= NULL;
nameblkp firstname	= NULL;
lineblkp sufflist	= NULL;
struct varblock *firstvar	= NULL;
struct pattern *firstpat	= NULL;
struct dirhd *firstod		= NULL;
wildp firstwild			= NULL;
wildp lastwild			= NULL;
nameblkp *hashtab;
int nhashed;
int hashsize;
int hashthresh;

int proclimit	= PROCLIMIT;
int nproc	= 0;
int proclive	= 0;
struct process procstack[MAXPROC];

#include <signal.h>
int sigivalue	= 0;
int sigqvalue	= 0;

int dbgflag	= NO;
int prtrflag	= NO;
int silflag	= NO;
int noexflag	= NO;
int keepgoing	= NO;
int noruleflag	= NO;
int touchflag	= NO;
int questflag	= NO;
int oldflag	= NO;
int ndocoms	= NO;
int ignerr	= NO;    /* default is to stop on error */
int forceshell	= NO;
int okdel	= YES;
int envlast	= NO;
int inarglist	= NO;
char **envpp	= NULL;

extern char *dfltmacro[];
extern char *dfltpat[];
extern char *dfltsuff[];
extern char **environ;
char **linesptr;

#ifdef pwb
char *prompt	= ">";	/* other systems -- pick what you want */
#else
char *prompt	= "";	/* other systems -- pick what you want */
#endif
int nopdir	= 0;
char funny[128];

main(argc,argv,envp)
int argc;
char *argv[];
char *envp[];
{
register nameblkp p;
int i, j;
int descset, nfargs;
int nowait = NO;
TIMETYPE tjunk;
char c, *s, *mkflagp;
static char makeflags[30] = "-";
static char onechar[2] = "X";

#ifdef unix
	int intrupt();
#endif

#ifdef METERFILE
	meter(METERFILE);
#endif

descset = 0;
mkflagp = makeflags+1;

funny['\0'] = (META | TERMINAL);
for(s = "=|^();&<>*?[]:$`'\"\\\n" ; *s ; ++s)
	funny[*s] |= META;
for(s = "\n\t :;&>|" ; *s ; ++s)
	funny[*s] |= TERMINAL;


newhash(HASHSIZE);

inarglist = YES;
for(i=1; i<argc; ++i)
	if(argv[i]!=0 && argv[i][0]!='-' && eqsign(argv[i]))
		argv[i] = 0;

setvar("$", "$", NO);
inarglist = NO;

for(i=1; i<argc; ++i)
    if(argv[i]!=0 && argv[i][0]=='-')
	{
	for(j=1 ; (c=argv[i][j])!='\0' ; ++j)  switch(c)
		{
		case 'd':
			++dbgflag;
			*mkflagp++ = 'd';
			break;

		case 'p':
			prtrflag = YES;
			break;

		case 's':
			silflag = YES;
			*mkflagp++ = 's';
			break;

		case 'i':
			ignerr = YES;
			*mkflagp++ = 'i';
			break;

		case 'S':
			keepgoing = NO;
			*mkflagp++ = 'S';
			break;

		case 'k':
			keepgoing = YES;
			*mkflagp++ = 'k';
			break;

		case 'n':
			noexflag = YES;
			*mkflagp++ = 'n';
			break;

		case 'r':
			noruleflag = YES;
			break;

		case 't':
			touchflag = YES;
			*mkflagp++ = 't';
			break;

		case 'q':
			questflag = YES;
			*mkflagp++ = 'q';
			break;

		case 'e':
			envlast = YES;
			*mkflagp++ = 'e';
			break;

		case 'o':
			oldflag = YES;
			*mkflagp++ = 'o';
			break;
	
		case 'z':
			forceshell = YES;
			*mkflagp++ = 'z';
			break;

		case 'P':
			if(isdigit(argv[i][j+1]))
				{
				proclimit = argv[i][++j] - '0';
				if(proclimit < 1)
					proclimit = 1;
				}
			else
				fatal("illegal proclimit parameter");
			*mkflagp++ = 'P';
			*mkflagp++ = argv[i][j];
			break;

		case 'f':
			if(i >= argc-1)
			  fatal("No description argument after -f flag");
			if( ! rddescf(argv[i+1]) )
				fatal1("Cannot open %s", argv[i+1]);
			argv[i+1] = 0;
			++descset;
			break;

		default:
			onechar[0] = c;	/* to make lint happy */
			fatal1("Unknown flag argument %s", onechar);
		}

	argv[i] = NULL;
	}

if(mkflagp > makeflags+1)
	setvar("MAKEFLAGS", makeflags, NO);

if( !descset )
if(	!rddescf("makefile") &&
	!rddescf("Makefile") &&
	(exists(s = "s.makefile") || exists(s = "s.Makefile")) )
		{
		char junk[20];
		concat("get ", s, junk);
		(void) dosys(junk, NO, NO, junk);
		rddescf(s+2);
		unlink(s+2);
		}


if(envlast)
	loadenv();
if(!noruleflag && !oldflag)
	rdarray(dfltpat);

if(prtrflag) printdesc(NO);

if( srchname(".IGNORE") )
	ignerr = YES;
if( srchname(".SILENT") )
	silflag = YES;
if( srchname(".OLDFLAG") )
	oldflag = YES;
if( p=srchname(".SUFFIXES") )
	sufflist = p->linep;
if( !sufflist && !firstwild)
	fprintf(stderr,"No suffix or %% pattern list.\n");
/*
if(sufflist && !oldflag)
	fprintf(stderr, "Suffix lists are old-fashioned. Use %% patterns\n);
*/

#ifdef unix
	sigivalue = (int) signal(SIGINT, SIG_IGN) & 01;
	sigqvalue = (int) signal(SIGQUIT, SIG_IGN) & 01;
	enbint(intrupt);
#endif

nfargs = 0;

for(i=1; i<argc; ++i)
	if(s = argv[i])
		{
		if((p=srchname(s)) == NULL)
			p = makename(s);
		++nfargs;
		if(i+1<argc && equal(argv[i+1], "&") )
			{
			++i;
			nowait = YES;
			}
		else
			nowait = NO;
		doname(p, 0, &tjunk, nowait);
		if(dbgflag) printdesc(YES);
		}

/*
If no file arguments have been encountered, make the first
name encountered that doesn't start with a dot
*/

if(nfargs == 0)
	if(mainname == 0)
		fatal("No arguments or description file");
	else	{
		doname(mainname, 0, &tjunk, NO);
		if(dbgflag) printdesc(YES);
		}

if(!nowait)
	waitstack(0);
exit(0);
}



#ifdef unix
intrupt()
{
struct varblock *varptr();
char *p;
TIMETYPE exists();

if(okdel && !noexflag && !touchflag &&
	(p = varptr("@")->varval) && exists(p)>0 && !isprecious(p) )
		{
		fprintf(stderr, "\n***  %s removed.", p);
		unlink(p);
		}

fprintf(stderr, "\n");
exit(2);
}




isprecious(p)
char *p;
{
register lineblkp lp;
register depblkp dp;
register nameblkp np;

if(np = srchname(".PRECIOUS"))
	for(lp = np->linep ; lp ; lp = lp->nxtlineblock)
		for(dp = lp->depp ; dp ; dp = dp->nxtdepblock)
			if(equal(p, dp->depname->namep))
				return YES;

return NO;
}


enbint(k)
int (*k)();
{
if(sigivalue == 0)
	signal(SIGINT,k);
if(sigqvalue == 0)
	signal(SIGQUIT,k);
}
#endif

rddescf(descfile)
char *descfile;
{
static int firstrd = YES;

/* read and parse description */

if(firstrd)
	{
	firstrd = NO;
	if( !noruleflag )
		{
		rdarray(dfltmacro);
		if(oldflag)
			rdarray(dfltsuff);
		}
	if(!envlast)
		loadenv();

#ifdef pwb
		{
		char *nlog, s[100];
		nlog = logdir();
		if( ! parse( concat(nlog,"/makecomm",s) ) )
			parse( concat(nlog,"/Makecomm",s) );
	
		if( !parse("makecomm") )
			parse("Makecomm");
		}
#endif
	}

return  parse(descfile);
}


rdarray(s)
char **s;
{
linesptr = s;
parse(CHNULL);
}


loadenv()
{
for(envpp = environ ; *envpp ; ++envpp)
	eqsign(*envpp);
envpp = NULL;
}

printdesc(prntflag)
int prntflag;
{
nameblkp p;
depblkp dp;
struct varblock *vp;
struct dirhd *od;
struct shblock *sp;
lineblkp lp;

#ifdef unix
if(prntflag)
	{
	printf("Open directories:\n");
	for (od = firstod; od; od = od->nxtdirhd)
		printf("\t%s\n", od->dirn);
	}
#endif

if(firstvar != 0) printf("Macros:\n");
for(vp = firstvar; vp ; vp = vp->nxtvarblock)
	printf("\t%s = %s\n" , vp->varname , vp->varval);

for(p = firstname; p; p = p->nxtnameblock)
	{
	printf("\n\n%s",p->namep);
	if(p->linep != 0) printf(":");
	if(prntflag) printf("  done=%d",p->done);
	if(p==mainname) printf("  (MAIN NAME)");
	for(lp = p->linep ; lp ; lp = lp->nxtlineblock)
		{
		if( dp = lp->depp )
			{
			printf("\n depends on:");
			for(; dp ; dp = dp->nxtdepblock)
				if(dp->depname != 0)
					printf(" %s ", dp->depname->namep);
			}
	
		if(sp = lp->shp)
			{
			printf("\n commands:\n");
			for( ; sp ; sp = sp->nxtshblock)
				printf("\t%s\n", sp->shbp);
			}
		}
	}
printf("\n");
fflush(stdout);
}
