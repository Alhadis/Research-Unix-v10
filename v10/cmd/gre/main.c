#define	MAIN	1
#include	<ctype.h>
#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

/* handle void* which didn't exist prior to ANSI C and C++ */
#if defined(__STDC__) || defined(c_plusplus) || defined(__cplusplus)
# define VOID void
#else
# define VOID char
#endif

static enum { gre, grep, egrep, fgrep } whoami = gre;
static char fullopts[] = "e:f:1bcEFGhilLnsvx";
static char *opts = fullopts+4;			/* start in after last : */

static void
usage(void)
{
	EPR "usage: %s [ -%s ] [ -e pattern ] [ -f file ] [ pattern ] [ file ] ...\n", progname, opts);
	exit(2);
}

#ifdef	PROFILING
short profb[50000];
#endif

main(int argc, char **argv)
{
	register c;
	int errflg = 0;
	char *input = 0, *finput = 0;
	int k, sval;
	unsigned char map[256];
	int foundsome = 0;
	PROCFN procfn;
	RDFN rdfn;
	MATCHFN matchfn;
	VOID *pat;

#ifdef	PROFILING
	{ extern etext(); monitor((int (*)())2, etext, profb, ((int)etext) - 2+12+2400, 300); }
#endif

/*re_debug=20;/**/
	/*
		determine if we are to be restricted to compatability mode
	*/
	if(progname = strrchr(argv[0], '/'))
		progname++;
	else
		progname = argv[0];
#ifdef	PLAN9
	if(strcmp(progname, "ogrep") == 0)
#else
	if(strcmp(progname, "grep") == 0)
#endif
		whoami = grep;
	else if(strcmp(progname, "egrep") == 0)
		whoami = egrep;
	else if(strcmp(progname, "fgrep") == 0)
		whoami = fgrep;
	offsetunit = (whoami == gre)? 1 : 1024;		/* test before -[FGE] */
	/*
		read the options; decide legality after we know what we are doing.
		the options are split so we can maintain the usage line
		in one place. note the only option we have to be wary of
		is -f (not grep)
	*/
	while((c = getopt(argc, argv, fullopts)) != -1)
		switch(c)
		{
		case '1':	oneflag = 1; break;
		case 'b':	bflag = 1; break;
		case 'c':	cflag = 1; break;
		case 'e':	if(input){
					EPR "%s: only one -e arg allowed\n", progname);
					errflg = 1;
				}
				input = optarg; break;
		case 'E':	whoami = egrep; break;
		case 'f':	if(input){
					EPR "%s: only one -f arg allowed\n", progname);
					errflg = 1;
				}
				finput = optarg; break;
		case 'F':	whoami = fgrep; break;
		case 'G':	whoami = grep; break;
		case 'h':	hflag = 1; break;
		case 'i':	iflag = 1; break;
		case 'l':	lflag = 1; break;
		case 'L':	Lflag = 1; break;
		case 'n':	nflag = 1; break;
		case 's':	sflag = 1; break;
		case 'v':	vflag = 1; break;
		case 'x':	xflag = 1; break;
		case '?':	errflg = 1; break;
		}
	if(errflg)
		usage();
	argv += optind;
	/*
		check for bad flag combinations
	*/
	if(finput && (whoami == grep)){
		EPR "%s: cannot use -f with grep\n", progname);
		exit(2);
	}
	if(finput && input){
		EPR "%s: cannot use -f with -e\n", progname);
		exit(2);
	}
	if(!input && !finput){
		input = *argv++;
		if(input == 0)
			usage();
	}
	/*
		character mapping ?
	*/
	for(k = 0; k < 256; k++)
		map[k] = k;
	if(iflag)
		for(k = 'A'; k <= 'Z'; k++)
			map[k] = tolower(k);
	/*
		in the interests of readability, fob off grep-type specific
		handling to separate functions. setting bmfn means using
		bmfind; similiarly cwfn means use cwfind

	rules:
		lnum needs be maintained only if nflag.
		nbytes needs be maintained only if bflag.
		for -[s1lL], do a longjmp(env).
		for -c, increment nmatch.
	*/
	switch(whoami)
	{
	case gre:	dogre(greparse, input, finput, map, &procfn, &pat, &rdfn, &matchfn); break;
	case grep:	dogre(grepparse, input, finput, map, &procfn, &pat, &rdfn, &matchfn); break;
	case fgrep:	dofgrep(input, finput, map, &procfn, &pat, &rdfn, &matchfn); break;
	case egrep:	dogre(egrepparse, input, finput, map, &procfn, &pat, &rdfn, &matchfn); break;
	}
	/*
		do generic flag handling
	*/
	prname = !hflag && *argv && argv[1];
	/*
		do file arguments now! for uniformity, no args = '-'
	*/
	if(!*argv)
		*--argv = "-";
	for(; curfile = *argv++; close(ifd)){
		if(strcmp(curfile, "-") == 0)
			ifd = 0;
		else if((ifd = open(curfile, 0)) < 0){
			EPR "%s: ", progname);
			perror(curfile);
			errflg = 2;
			continue;
		}
		if(sflag && foundsome)
			continue;	/* don't need to scan */
		lnum = nmatch = nbytes = 0;
		longlinewarned = 0;
		if((sval = setjmp(env)) == 0)
			if((*procfn)(pat, rdfn, matchfn) < 0){
				EPR "%s: ", progname);
				perror(curfile);
				errflg = 2;
				continue;
			}
		if((lflag && sval) || (Lflag && !sval))
			PR "%s\n", curfile);
		if(cflag){
			if(prname)
				PR "%s:", curfile);
			PR "%ld\n", nmatch);
		}
		if(nmatch)
			foundsome = 1;
	}
	exit(errflg? errflg : (foundsome == 0));
	/*NOTREACHED*/
}

void
re_error(char *s)
{
	EPR "%s: %s\n", progname, s);
	exit(2);
}
