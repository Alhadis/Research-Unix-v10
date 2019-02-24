# include <stdio.h>
# include <ctype.h>
# include <signal.h>

# include "11cc.h"		/* god: has all the local pathnames */

/* 11cc command */

# define MAXINC 10
# define MAXFIL 100
# define MAXLIB 100
# define MAXOPT 100
char	*tmp0;
char	*tmp1;
char	*tmp2;
char	*tmp3;
char	*tmp4;
char	*tmp5;
char	*tmp6, *tmp7;	/* mjm */
char	*outfile;
char	*av[50];
char	*clist[MAXFIL];
char	*llist[MAXLIB];
int	pflag;
int	sflag;
int	cflag;
int	eflag;
int	exflag;
int	oflag;
int	Fflag;	/* mjm: RH fpp */
int	vflag;	/* mjm: verbose */
int	proflag;
int	noflflag;
int	zflag;		/* request zero-extension on all chars */
char	*chpass ;
char	*npassname ;
char	pass0[50] = LIBPATH(11c0);		/*god*/
char	pass1[50] = LIBPATH(11c1);		/*god*/
char	pass2[50] = LIBPATH(11c2);		/*god*/
char	passp[50] = "/lib/cpp";			/*god*/
char	passf[50] = LIBPATH(fpp);		/*mjm*/
char	*pref = LIBPATH(11crt0.o);		/*god*/
char	*copy();
char	*setsuf();
char	*strcat();
char	*strcpy();
char	*stralloc();
extern	int optind;
extern	int opterr;
extern	char *optarg;
extern	int optopt;

main(argc, argv)
char *argv[]; 
{
	char *rhopt;	/* mjm */
	char *t;
	char *assource;
	char **pv, *ptemp[MAXOPT], **pvt;
	int nc, nl, i, j, c, nxo, na;
	int idexit();

	opterr = 0;
	i = nc = nl = nxo = 0;
	setbuf(stdout, (char *)NULL);
	pv = ptemp;
	while (optind<argc)
	    switch (c = getopt(argc, argv, "So:OpEPcfL:l:D:I:U:C:t:B:zvF")) {
	case 'S':
		sflag++;
		cflag++;
		continue;

	case 'o':
		outfile = optarg;
		if ((c=getsuf(outfile))=='c'||c=='o') {
			error("Would overwrite %s", outfile);
			exit(8);
		}
		continue;

	case 'O':
		oflag++;
		continue;

	case 'p':
		error("profiling not supported", (char *)NULL); /*god*/
		exit(-1); /*god*/
/*god*/		/*proflag++;
		continue;*/	/*god*/

	case 'E':
		exflag++;
		cflag++;
		pflag++;
		*pv++ = "-E";
		continue;

	case 'P':
		pflag++;
		*pv++ = "-P";
	case 'c':
		cflag++;
		continue;

	case 'f':
/*god*/		error("floating point interpreter not supported",(char *)NULL);
		error("use -F to invoke floating point preprocessor",
			(char *)NULL); /*god*/
		exit(-1); /*god*/
/*god*/		/*noflflag++;
		if (npassname || chpass)
			error("-f overwrites earlier option", (char *)NULL);
		npassname = "/lib/f";
		chpass = "1";
		continue;*/		/*god*/

	case 'L':
		t = stralloc(strlen(optarg)+2);
		t[0] = '-';
		t[1] = 'L';
		t[2] = '\0';
		strcat(t, optarg);
		llist[nl++] = t;
		break;

	case 'l':
		t = stralloc(strlen(optarg)+2);
		t[0] = '-';
		t[1] = 'l';
		t[2] = '\0';
		strcat(t, optarg);
		llist[nl++] = t;
		break;

	case 'D':
	case 'I':
	case 'U':
	case 'C':
		*pv = stralloc(strlen(optarg)+2);
		if (pv >= ptemp+MAXOPT) {
			error("Too many DIUC options", (char *)NULL);
			dexit();
		}
		(*pv)[0] = '-';
		(*pv)[1] = c;
		(*pv)[2] = '\0';
		strcat(*pv, optarg);
		pv++;
		break;

	case 't':
		if (chpass)
			error("-t overwrites earlier option", (char *)NULL);
		chpass = optarg;
		if (chpass[0]==0)
			chpass = "012pf"; /*god: allow select fpp pass*/
		break;

	case 'B':
		if (npassname)
			error("-B overwrites earlier option", (char *)NULL);
		npassname = optarg;
		if (npassname[0]==0)
			npassname = LIBPATH(11);	/*god*/
		break;

	case '?':
		t = stralloc(3);
		t[0] = '-';
		t[1] = optopt;
		t[2] = '\0';
		llist[nl++] = t;
		break;

	case 'z':
		zflag++;
		break;

	case 'v':		/* mjm: callsys print */
		vflag++;
		break;

	case 'F':		/* mjm: RH fpp */
		Fflag++;
		if( (optind < argc)/*god*/ && (argv[optind][1] == 'F') )
			rhopt = argv[optind];
		else if(argv[optind-1][1] == 'F')
			rhopt = argv[optind-1];
		for(t = rhopt+2; *t != '\0'; t++)	/* skip rest after -F */
			getopt(argc, argv, "qunl");
		break;

	case EOF:
		t = argv[optind];
		optind++;
		if((c=getsuf(t))=='c' || c=='s'|| exflag) {
			clist[nc++] = t;
			if (nc>=MAXFIL) {
				error("Too many source files", (char *)NULL);
				exit(1);
			}
			t = setsuf(t, 'o');
		}
		if (nodup(llist, t)) {
			llist[nl++] = t;
			if (nl >= MAXLIB) {
				error("Too many object/library files", (char *)NULL);
				exit(1);
			}
			if (getsuf(t)=='o')
				nxo++;
		}
	}
	if (npassname && chpass ==0)
		chpass = "012pf";		/*god: allow select fpp pass*/
	if (chpass && npassname==0)
		npassname = LIBPATH(11);	/*god*/
	if (chpass)
		for (t=chpass; *t; t++) {
			switch (*t) {
			case '0':
				strcpy (pass0, npassname);
				strcat (pass0, "c0");
				continue;
			case '1':
				strcpy (pass1, npassname);
				strcat (pass1, "c1");
				continue;
			case '2':
				strcpy (pass2, npassname);
				strcat (pass2, "c2");
				continue;
			case 'p':
				strcpy (passp, npassname);
				strcat (passp, "cpp");
				continue;
			case 'f':		/*god: include fpp in here */
				strcpy (passf, npassname);
				strcat (passf, "fpp");
			}
		}
/*god*/	/*if (noflflag)
		pref = proflag ? "/lib/fmcrt0.o" : "/lib/fcrt0.o";
	else if (proflag)
		pref = "/lib/mcrt0.o";*/	/*god*/
	if(nc==0)
		goto nocom;
	if (pflag==0 || Fflag) {	/* mjm: added Fflag */
		tmp0 = copy("/tmp/ctm0a");
		while (access(tmp0, 0)==0)
			tmp0[9]++;
		while((creat(tmp0, 0400))<0) {
			if (tmp0[9]=='z') {
				error("11cc: cannot create temp", (char *)NULL);
				exit(1);
			}
			tmp0[9]++;
		}
	}
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, idexit);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, idexit);
	(tmp1 = copy(tmp0))[8] = '1';
	(tmp2 = copy(tmp0))[8] = '2';
	(tmp3 = copy(tmp0))[8] = '3';
	if (oflag)
		(tmp5 = copy(tmp0))[8] = '5';
	if (pflag==0)
		(tmp4 = copy(tmp0))[8] = '4';
	if (Fflag) {		/* mjm */
		(tmp6 = copy(tmp0))[8] = '6';	/* mjm */
		(tmp7 = copy(tmp0))[8] = '7';	/* mjm */
	}
	pvt = pv;
	for (i=0; i<nc; i++) {
		if (nc>1)
			printf("%s:\n", clist[i]);
		if (getsuf(clist[i])=='s') {
			assource = clist[i];
			goto assemble;
		} 
		else
			assource = tmp3;
		if (pflag)
			tmp4 = setsuf(clist[i], 'i');
		av[0] = "cpp";
		av[1] = clist[i];
		av[2] = exflag ? "-" : tmp4;
		if(Fflag)
			av[2] = tmp6;	/* mjm */
		na = 3;
		for(pv=ptemp; pv <pvt; pv++)
			if(!Fflag || *(*pv+1) != 'P')	/* mjm */
			av[na++] = *pv;		/* mjm: no -P when -F */
		av[na++] = "-Uvax";	/*god: (5/23) get rid of vax define */
		av[na++] = "-Dpdp11=1";	/*god: (5/23) and define pdp11 */
		av[na++]=0;
		if (callsys(passp, av)) {
			cflag++;
			eflag++;
			continue;
		}
		if(Fflag) {		/* mjm */
			av[0] = "fpp";
			av[1] = tmp6;
			av[2] = tmp4;
			av[3] = tmp7;
			av[4] = rhopt;
			av[5] = 0;
			if (callsys(passf, av)) {	/* mjm */
				eflag++;
				continue;
			}
		}
		av[0]= "11c0";		/*god*/
		if (pflag) {
			cflag++;
			continue;
		}
		j = 1;
		if (zflag)
			av[j++] = "-u";
		av[j++] = tmp4;
		av[j++] = tmp1;
		av[j++] = tmp2;
		if (proflag)
			av[j++] = "-P";
		av[j++] = 0;
		if (callsys(pass0, av)) {
			cflag++;
			eflag++;
			continue;
		}
		av[0] = "11c1";		/*god*/
		av[1] = tmp1;
		av[2] = tmp2;
		if (sflag)
			assource = tmp3 = setsuf(clist[i], 's');
		av[3] = tmp3;
		if (oflag)
			av[3] = tmp5;
		av[4] = 0;
		if(callsys(pass1, av)) {
			cflag++;
			eflag++;
			continue;
		}
		if (oflag) {
			av[0] = "11c2";	/*god*/
			av[1] = tmp5;
			av[2] = tmp3;
			av[3] = 0;
			if (callsys(pass2, av)) {
				unlink(tmp3);
				assource = tmp5;
			} 
			else
				unlink(tmp5);
		}
		if (sflag)
			continue;
assemble:
		av[0] = "11as";		/*god*/
		av[1] = "-u";
		av[2] = "-o";
		av[3] = setsuf(clist[i], 'o');
		av[4] = assource;
		av[5] = 0;
		cunlink(tmp1);
		cunlink(tmp2);
		cunlink(tmp4);
		if (callsys(BINPATH(11as), av) > 1) {	/*god*/
			cflag++;
			eflag++;
			continue;
		}
	}
nocom:
	if (cflag==0 && nl!=0) {
		i = 0;
		av[0] = "11ld";			/*god*/
		av[1] = "-X";
		av[2] = pref;
		j = 3;
		if (outfile) {
			av[j++] = "-o";
			av[j++] = outfile;
		}
		while(i<nl)
			av[j++] = llist[i++];
		av[j++] = 0;
		eflag |= callsys(BINPATH(11ld), av);	/*god*/
		if (nc==1 && nxo==1 && eflag==0)
			cunlink(setsuf(clist[0], 'o'));
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
		cunlink(tmp0);
	}
	if(Fflag) {		/* mjm */
		cunlink(tmp6);
		cunlink(tmp7);
		cunlink(tmp0);
	}
	exit(eflag);
}

error(s, x)
char *s, *x;
{
	fprintf(exflag?stderr:stdout, s, x);
	putc('\n', exflag? stderr : stdout);
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
	while(t = *s++)
		if (t=='/')
			c = 0;
		else
			c++;
	s -= 3;
	if (c<=14 && c>2 && *s++=='.')
		return(*s);
	return(0);
}

char *
setsuf(as, ch)
char *as;
{
	register char *s, *s1;

	s = s1 = copy(as);
	while(*s)
		if (*s++ == '/')
			s1 = s;
	s[-1] = ch;
	return(s1);
}

callsys(f, v)
char f[], *v[]; 
{
	int t, status;
	register char **vp;	/* god */

	if(vflag) {	/* god & mjm */
		vp = v;
		fprintf(stderr,"+ ");
		while (*vp)
			fprintf(stderr,"%s ",*vp++);
		fprintf(stderr, "\n");
	}

	if ((t=fork())==0) {
		execv(f, v);
		printf("Can't find %s\n", f);
		exit(100);
	} else
		if (t == -1) {
			printf("Try again\n");
			return(100);
		}
	while(t!=wait(&status))
		;
	if (t = status&0377) {
		if (t!=SIGINT) {
			printf("Fatal error (%d) in %s\n", t,f); /*mjm*/
			eflag = 8;
		}
		dexit();
	}
	return((status>>8) & 0377);
}

char *
copy(s)
register char *s;
{
	register char *ns;

	ns = stralloc(strlen(s));
	return(strcpy(ns, s));
}

char *
stralloc(n)
{
	char *malloc();
	register char *s;

	s = malloc(n+1);
	if (s==NULL) {
		error("out of space", (char *)NULL);
		dexit();
	}
}

nodup(l, os)
char **l, *os;
{
	register char *t, *s;
	register int c;

	s = os;
	if (getsuf(s) != 'o')
		return(1);
	while(t = *l++) {
		while(c = *s++)
			if (c != *t++)
				break;
		if (*t=='\0' && c=='\0')
			return(0);
		s = os;
	}
	return(1);
}

cunlink(f)
char *f;
{
	if (f==NULL)
		return;
	unlink(f);
}
