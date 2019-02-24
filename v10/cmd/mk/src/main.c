#include	"mk.h"

#define		MKFILE		"mkfile"

static char *version = "@(#)mk general release 3";
int debug;
Rule *rules, *metarules;
int nproclimit;
int nflag = 0;
int tflag = 0;
int iflag = 0;
int kflag = 0;
int mflag = 0;
int aflag = 0;
int uflag = 0;
char *explain = 0;
Word *target1;
int nreps = 1;
Job *jobs;
char *whatif = 0;
#ifdef	PROF
short buf[10000];
#endif

main(argc, argv)
	char **argv;
{
	register Word *w;
	register char *s;
	char *files[256], **f = files, **ff;
	char mkargs[256], *ma = mkargs;
	int i;
	int tfd = -1;
	char *temp = "/tmp/mkargXXXXXX";

	*ma = 0;
	for(argv++; *argv && (**argv == '-'); argv++)
	{
		for(s = argv[0]; *s; *ma++ = *s++);
		*ma++ = ' ';
		switch(argv[0][1])
		{
		case 'a':
			aflag = 1;
			break;
		case 'd':
			if(*(s = &argv[0][2]))
				while(*s) switch(*s++)
				{
				case 'p':	debug |= D_PARSE; break;
				case 'g':	debug |= D_GRAPH; break;
				case 'e':	debug |= D_EXEC; break;
				}
			else
				debug = 0xFFFF;
			break;
		case 'e':
			explain = &argv[0][2];
			break;
		case 'f':
			if(*++argv == 0)
				usage();
			*f++ = *argv;
			for(s = argv[0]; *s; *ma++ = *s++);
			*ma++ = ' ';
			break;
		case 'i':
			iflag = 1;
			break;
		case 'k':
			kflag = 1;
			break;
		case 'm':
			mflag = 1;
			aflag = nflag = 1;
			break;
		case 'n':
			nflag = 1;
			break;
		case 't':
			tflag = 1;
			break;
		case 'u':
			uflag = 1;
			break;
		case 'w':
			if(argv[0][2])
				whatif = &argv[0][2];
			else {
				if(*++argv == 0)
					usage();
				whatif = &argv[0][0];
			}
			break;
		default:
			usage();
		}
	}
#ifdef	PROF
	{
		extern etext();
		monitor(main, etext, buf, sizeof buf, 300);
	}
#endif

	if(aflag)
		iflag = 1;

	account();
	syminit();
	builtins();
	initenv();

	/*
		assignment args become null strings
	*/
	for(i = 0; argv[i]; i++) if(strchr(argv[i], '=')){
		for(s = argv[i]; *s; *ma++ = *s++);
		*ma++ = ' ';
		if(tfd < 0){
			mktemp(temp);
			close(creat(temp, 0600));
			if((tfd = open(temp, 2)) < 0){
				perror(temp);
				Exit();
			}
		}
		Fprint(tfd, "%s\n", argv[i]);
		*argv[i] = 0;
	}
	if(tfd >= 0){
		Fflush(tfd);
		lseek(tfd, 0L, 0);
		parse("command line args", tfd, 1, 1);
		unlink(temp);
	}

	if(ma != mkargs) ma--;
	*ma = 0;
	symlook("MKFLAGS", S_VAR, strdup(mkargs));
	for(ma = mkargs, i = 0; argv[i]; i++){
		if(*argv[i] == 0) continue;
		if(i) *ma++ = ' ';
		for(s = argv[i]; *s; *ma++ = *s++);
	}
	*ma = 0;
	symlook("MKARGS", S_VAR, strdup(mkargs));

	if(f == files){
		if(access(MKFILE, 4) == 0)
			parse(MKFILE, open(MKFILE, 0), 0, 1);
	} else
		for(ff = files; ff < f; ff++)
			parse(*ff, open(*ff, 0), 0, 1);
	parsebuiltins();
	if(DEBUG(D_PARSE)){
		dumpw("default targets", target1);
		dumpr("rules", rules);
		dumpr("metarules", metarules);
		dumpv("variables");
	}
	if(whatif)
		timeinit(whatif);
	execinit();
	/* skip assignment args */
	while(*argv && (**argv == 0))
		argv++;
	sigcatch();
	if(*argv == 0){
		if(target1)
			for(w = target1; w; w = w->next)
				mk(w->s);
		else {
			Fprint(2, "mk: nothing to mk\n");
			Exit();
		}
	} else
		for(; *argv; argv++)
			if(**argv)
				mk(*argv);
	if(uflag)
		praccount();
	if(mflag)
		makefile();
	exit(0);
}

usage()
{

	Fprint(2, "Usage: mk [-f file] [-n] [-a] [-e] [-t] [-k] [-i] [-d[egp]] [targets ...]\n");
	Exit();
}

char *
Malloc(n)
{
	register char *s;

	if(s = malloc(n))
		return(s);
	Fprint(2, "mk: cannot alloc %d bytes\n", n);
	Exit();
	return((char *)0);	/* shut cyntax up */
}

Exit()
{
	while(wait((int *)0) >= 0)
		;
	exit(1);
}

char *
strndup(s, n)
	char *s;
{
	register char *goo;

	goo = Malloc(n);
	memcpy(goo, s, n);
	return(goo);
}

initenv()
{
	extern char **environ;
	register char **p, *s, *ss;

	for(p = environ; *p; p++){
		s = shname(*p);
		switch(*s)
		{
		case '=':
			*s = 0;
			ss = strdup(s+1);
			symlook(strdup(*p), S_VAR, ss)->value = ss;
			*s = '=';
			break;
		case 0:
			ss = strdup("");
			symlook(strdup(*p), S_VAR, ss)->value = ss;
			break;
		}
	}
}

assert(s, n)
	char *s;
{
	if(!n){
		Fprint(2, "mk: Assertion ``%s'' failed.\n", s);
		Exit();
	}
}
