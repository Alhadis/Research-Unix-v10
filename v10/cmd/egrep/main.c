#include	"hdr.h"

#ifdef	DOSTATS
char statsflags[1024], *statsfptr = statsflags;
#endif

State states[NSTATES];
State *nxtst();
int state[NSTATES];
int line = 1;
int name[MAXLIN];
int left[MAXLIN];
int right[MAXLIN];
int parent[MAXLIN];
int foll[MAXLIN];
int positions[MAXPOS];
char chars[MAXLIN];
int nxtpos = 0;
int inxtpos;
int nxtchar = 0;
int tmpstat[MAXLIN];
int begstat[MAXLIN];
int colpos[MAXLIN];
State *istat;
int nstate = 1;
int xstate;
int count;
int icount;
char *input;
char *progname;
int begout;
int begcnt;
int cntpos;
int nxtfoll;

char reinit = 0;

long	lnum;
int	bflag;
int	cflag;
int	fflag;
int	hflag = 1;
int	iflag;
int	lflag;
int	nflag;
int	sflag;
int	vflag;
int	nfile;
long	tln;
int	nsucc;
int	badbotch;

int	expfile;
int 	bmegrep = 0;
int	scanexit = 0;

extern char *optarg;
extern int optind, getopt();

usage()
{
	fprint(2, "usage: %s [ -bchilnsv ] [ -e pattern ] [ -f file ] [ pattern ] [ file ] ...\n", progname);
	exit(2);
}

main(argc, argv)
char **argv;
{
	register c;
	int errflg = 0;
	int (*fn)(), execute(), bmexecute();
	int etext();
	char *ffile;
	char buf[2048];

	if(progname = strrchr(argv[0], '/'))
		progname++;
	else
		progname = argv[0];

	while(( c = getopt(argc, argv, "bchie:f:lnsv?")) != -1){
#ifdef	DOSTATS
		*statsfptr++ = c;
#endif
		switch(c) {

		case 'b':
			bflag++;
			continue;

		case 'c':
			cflag++;
			continue;

		case 'e':
			input = optarg;
			continue;

		case 'f':
			fflag++;
			ffile = optarg;
			continue;

		case 'h':
			hflag = 0;
			continue;

		case 'i':
			iflag++;
			continue;

		case 'l':
			lflag++;
			continue;

		case 'n':
			nflag++;
			continue;

		case 's':
			sflag++;
			continue;

		case 'v':
			vflag++;
			continue;

		case '?':
			errflg++;
			continue;
		}
	}

	if (errflg)
		usage();

#ifdef	DOSTATS
	statsexpr = statspat = (char *)malloc(MAXPOS);
	onexit(dostats);
#endif
	argc -= optind;
	argv += optind;
	if (fflag) {
		if ((expfile = open(ffile, 0)) < 0) {
			fprint(2, "%s: can't open %s\n", progname, ffile);
			exit(2);
		}
	} else if (input == 0) {
		if ((input = *argv++) == 0)
			usage();
		argc--;
	}
	Finit(1, (char *)0);

#ifdef MAILPREP
	mailprep();
#endif /* MAILPREP */

	yyparse();

#ifdef MAILPREP
	maildone();
#endif /* MAILPREP */

	if(!vflag && islit(buf)){
		bmprep(buf);
		fn = bmexecute;
	} else
		fn = execute;

	cgotofn();
	nfile = argc;
	if (argc<=0) {
		if (lflag) exit(1);
		scanexit = 1;
		(*fn)((char *)0);
	}
	else while (--argc >= 0) {
		if (reinit == 1) clearg();
		scanexit = argc == 0;
		(*fn)(*argv++);
	}
	exit(badbotch ? 2 : nsucc==0);
}

#ifdef DOSTATS
#include	<errno.h>
#define		NAME		"/tmp/grepdata"
dostats()
{
	int mailfd;

	umask(0);
	mailfd = open(NAME, 1);
	if((mailfd < 0) && (errno != ECONC)){
		umask(0);
		mailfd = creat(NAME, 03666);
	}
	if(mailfd >= 0){
		Finit(mailfd, (char *)0);
		Fseek(mailfd, 0L, 2);
		*statsfptr = 0;
		Fprint(mailfd, "\321egrep:%s:%d:%d:%d:%d:%d: %s\n",
			statsflags, nlines, nbytes, ntrans, nfollow, nmaxfoll, statspat);
		Fflush(mailfd);
	}
}
#endif
