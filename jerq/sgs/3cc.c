/*
 * cc - front end for C compiler
 */
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <dir.h>

char	*cpp = "/lib/cpp";
char	*ccom = "/usr/jerq/lib/m32/comp";
char	*as = "/usr/jerq/bin/3as";
char	*c2 = "/usr/jerq/lib/m32/optim";
char	*ld = "/usr/jerq/bin/3ld";
char	*ifile = "/usr/jerq/lib/muxmap";

char	tmp0[30];		/* big enough for /tmp/ctm%05.5d */
char	*tmp1, *tmp2, *tmp3, *tmp4, *tmp5;
char	*outfile;
char	*savestr(), *strspl(), *setsuf();
int	idexit();
char	**av, **clist, **llist, **plist;
int	cflag, eflag, gflag, pflag, sflag, Rflag, exflag, proflag, oflag;
int	dont, jflag, mflag=1;	/* default is to compile for mux(9) */
int	exfail;
char	*chpass;
char	*npassname;
extern	int	optind;
extern	int	opterr;
extern	char	*optarg;
extern	int	optopt;

int	nc, nl, np, nxo, na;

#define	cunlink(x)	if (x) unlink(x)

main(argc, argv)
	char **argv;
{
	char *t;
	char *assource;
	int i, j, c;

	/* ld currently adds upto 5 args; 10 is room to spare */
	/* I upped it anyway -- rob */
	av = (char **)calloc(argc+20, sizeof (char **));
	clist = (char **)calloc(argc+20, sizeof (char **));
	llist = (char **)calloc(argc+20, sizeof (char **));
	plist = (char **)calloc(argc+20, sizeof (char **));
	opterr = 0;
	while (optind<argc) switch (c = getopt(argc, argv, "dJmSo:ROPgEPcD:I:U:C:t:B:l:")) {
	case 'd':
		dont++;
		continue;
	case 'S':
		sflag++;
		cflag++;
		continue;
	case 'J':
		jflag++;
	case 'm':
		mflag=0;
		ifile = "/usr/jerq/lib/jxmap";
		continue;
	case 'l':
		llist[nl++] = strspl("-l", optarg);
		continue;
	case 'o':
		outfile = optarg;
		switch (getsuf(outfile)) {

		case 'c':
		case 'o':
			error("-o would overwrite %s", outfile);
			exit(8);
		}
		continue;
	case 'R':
		Rflag++;
		continue;
	case 'O':
		oflag++;
		continue;
	case 'p':
		proflag++;
		continue;
	case 'g':
		gflag++;
		continue;
	case 'E':
		exflag++;
		cflag++;
		continue;
	case 'P':
		pflag++;
	case 'c':
		cflag++;
		continue;
	case 'D':
	case 'I':
	case 'U':
	case 'C':
		plist[np] = strspl("-X", optarg);
		plist[np++][1] = c;
		continue;
	case 't':
		if (chpass)
			error("-t overwrites earlier option", 0);
		chpass = optarg;
		if (chpass[0]==0)
			chpass = "012p";
		continue;
	case 'B':
		if (npassname)
			error("-B overwrites earlier option", 0);
		npassname = optarg;
		if (npassname[0]==0)
			npassname = "/usr/scj/mcc";
		continue;
	case '?':
		t = strspl("-", "x");
		t[1] = optopt;
		llist[nl++] = t;
		continue;

	case EOF:
		t = argv[optind];
		optind++;
		c = getsuf(t);
		if (c=='c' || c=='s' || c=='i' || exflag) {
			clist[nc++] = t;
			t = setsuf(t, 'o');
		}
		if (nodup(llist, t)) {
			llist[nl++] = t;
			if (getsuf(t)=='o')
				nxo++;
		}
	}
	if (npassname && chpass ==0)
		chpass = "012p";
	if (chpass && npassname==0)
		npassname = "/usr/new";
	if (chpass)
	for (t=chpass; *t; t++) {
		switch (*t) {

		case '0':
			ccom = strspl(npassname, "comp");
			continue;
		case '2':
			c2 = strspl(npassname, "optim");
			continue;
		case 'p':
			cpp = strspl(npassname, "cpp");
			continue;
		}
	}
	if (proflag)
		printf("3cc: no profiling option\n");
	if (nc==0)
		goto nocom;
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, idexit);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, idexit);
	if (pflag==0)
		sprintf(tmp0, "/tmp/ctm%05.5d", getpid());
	tmp1 = strspl(tmp0, "1");
	tmp2 = strspl(tmp0, "2");
	tmp3 = strspl(tmp0, "3");
	if (pflag==0)
		tmp4 = strspl(tmp0, "4");
	if (oflag)
		tmp5 = strspl(tmp0, "5");
	for (i=0; i<nc; i++) {
		int suffix = getsuf(clist[i]);
		if (nc > 1) {
			printf("%s:\n", clist[i]);
			fflush(stdout);
		}
		if (suffix == 's') {
			assource = clist[i];
			goto assemble;
		} else
			assource = tmp3;
		if (suffix == 'i')
			goto compile;
		if (pflag)
			tmp4 = setsuf(clist[i], 'i');
		na=0;
		av[na++] = "cpp";
		av[na++] = "-Uvax";
		av[na++] = "-Dm32";
		if(mflag)
			av[na++] = "-DMUX";
		for (j = 0; j < np; j++)
			av[na++] = plist[j];
		av[na++] = "-I/usr/jerq/include";
		av[na++] = clist[i];
		av[na++] = exflag ? "-" : tmp4;
		av[na++] = 0;
		switch(callsys(cpp, av)){
		case 0:
			break;
#define	CLASS 27
		case CLASS:
			if(callsys("/lib/cpre", av)==0)
				break;
			/* fall through */
		default:
			exfail++;
			eflag++;
		}
		if (pflag || exfail) {
			cflag++;
			continue;
		}
compile:
		if (sflag)
			assource = tmp3 = setsuf(clist[i], 's');
		av[0] = "3comp";
		av[1] = "-i";
		av[2] = suffix=='i'? clist[i]: tmp4;
		av[3] = "-o";
		av[4] = oflag? tmp5 : tmp3;
		av[5] = "-f";
		av[6] = clist[i];
		if(gflag)
			na = 7;
		else{
			av[7] = "-ds";
			av[8] = "-dl";
			na = 9;
		}
		av[na] = 0;
		if (callsys(ccom, av)) {
			cflag++;
			eflag++;
			continue;
		}
		if (oflag) {
			av[0] = "3optim";
			av[1] = "-I";
			av[2] = tmp5;
			av[3] = "-O";
			av[4] = tmp3;
			av[5] = 0;
			if (callsys(c2, av)) {
				unlink(tmp3);
				tmp3 = assource = tmp5;
			} else
				unlink(tmp5);
		}
		if (sflag)
			continue;
	assemble:
		cunlink(tmp1); cunlink(tmp2); cunlink(tmp4);
		av[0] = "3as";
		na = 1;
		if(!gflag)
			av[na++] = "-dl";
		av[na++] = "-o"; av[na++] = setsuf(clist[i], 'o');
		if (Rflag)
			av[na++] = "-R";
		av[na++] = assource;
		av[na] = 0;
		if (callsys(as, av) > 1) {
			cflag++;
			eflag++;
			continue;
		}
	}
nocom:
	if (cflag==0 && nl!=0) {
		i = 0;
		av[0] = "3ld"; na=1;
		av[na++] = "-o";
		av[na++] = outfile? outfile : "a.out";
		av[na++] = "-L/usr/jerq/lib";
		if (jflag)
			av[na++] = "/usr/jerq/lib/rom.o";
		av[na++] = ifile;
		while (i < nl)
			av[na++] = llist[i++];
		if(mflag){
			av[na++] = "-r";
			av[na++] = "-a";
		}
		if(mflag)
			av[na++] = "-lmj";
		if(jflag){
			av[na++] = "-lj";
			av[na++] = "-lsys";
		}
		av[na++] = "-lj";
		av[na++] = "-lc";
		av[na++] = 0;
		eflag |= callsys(ld, av);
		if (nc==1 && nxo==1 && eflag==0)
			unlink(setsuf(clist[0], 'o'));
		if(!eflag && mflag){
			int f=open(outfile? outfile : "a.out", 1);
			short m=0x5620;
			if(f==-1 || lseek(f, 22, 0)==-1 || write(f, &m, 2)!=2)
				error("can't write magic number in %s",
					outfile? outfile : "a.out");
			else
				close(f);
		}
	}
	dexit();
}

idexit()
{

	eflag = 100;
	dexit();
}

dexit()
{

	if (!pflag) {
		cunlink(tmp1);
		cunlink(tmp2);
		if (sflag==0)
			cunlink(tmp3);
		cunlink(tmp4);
		cunlink(tmp5);
	}
	exit(eflag);
}

error(s, x)
	char *s, *x;
{
	FILE *diag = exflag ? stderr : stdout;

	fprintf(diag, "3cc: ");
	fprintf(diag, s, x);
	putc('\n', diag);
	exfail++;
	cflag++;
	eflag++;
}

getsuf(as)
char as[];
{
	register int c;
	register char *s;
	register int t;

	s = as;
	c = 0;
	while (t = *s++)
		if (t=='/')
			c = 0;
		else
			c++;
	s -= 3;
	if (c <= DIRSIZ && c > 2 && *s++ == '.')
		return (*s);
	return (0);
}

char *
setsuf(as, ch)
	char *as;
{
	register char *s, *s1;

	s = s1 = savestr(as);
	while (*s)
		if (*s++ == '/')
			s1 = s;
	s[-1] = ch;
	return (s1);
}

callsys(f, v)
	char *f, **v;
{
	int t, status;

	if(dont){
		printf("3cc: (%s)", f);
		while(*v)
			printf(" %s", *v++);
		printf("\n");
		return 0;
	}
	t = fork();
	if (t == -1) {
		printf("No more processes\n");
		return (100);
	}
	if (t == 0) {
		execv(f, v);
		printf("Can't find %s\n", f);
		fflush(stdout);
		_exit(100);
	}
	while (t != wait(&status))
		;
	if ((t=(status&0377)) != 0 && t!=14) {
		if (t!=2) {
			printf("Fatal error in %s\n", f);
			eflag = 8;
		}
		dexit();
	}
	return ((status>>8) & 0377);
}

nodup(l, os)
	char **l, *os;
{
	register char *t, *s;
	register int c;

	s = os;
	if (getsuf(s) != 'o')
		return (1);
	while (t = *l++) {
		while (c = *s++)
			if (c != *t++)
				break;
		if (*t==0 && c==0)
			return (0);
		s = os;
	}
	return (1);
}

#define	NSAVETAB	1024
char	*savetab;
int	saveleft;

char *
savestr(cp)
	register char *cp;
{
	register int len;

	len = strlen(cp) + 1;
	if (len > saveleft) {
		saveleft = NSAVETAB;
		if (len > saveleft)
			saveleft = len;
		savetab = (char *)malloc(saveleft);
		if (savetab == 0) {
			fprintf(stderr, "ran out of memory (savestr)\n");
			exit(1);
		}
	}
	strncpy(savetab, cp, len);
	cp = savetab;
	savetab += len;
	saveleft -= len;
	return (cp);
}

char *
strspl(left, right)
	char *left, *right;
{
	char buf[BUFSIZ];

	strcpy(buf, left);
	strcat(buf, right);
	return (savestr(buf));
}
