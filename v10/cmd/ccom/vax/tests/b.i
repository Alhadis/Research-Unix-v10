typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	struct	_physadr { int r[1]; } *physadr;
typedef	int	daddr_t;
typedef	char *	caddr_t;
typedef	u_short ino_t;
typedef	int	swblk_t;
typedef	int	size_t;
typedef	int	time_t;
typedef	int	label_t[14];
typedef	short	dev_t;
typedef	int	off_t;
typedef long	portid_t;
typedef struct	fd_set { int fds_bits[1]; } fd_set;
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
extern char	_ctype[];
typedef int	(*SIG_TYP)();
SIG_TYP signal();
struct	dir
{
	ino_t	d_ino;
	char	d_name[14];
};
struct vtimes {
	int	vm_utime;		
	int	vm_stime;		
	
	unsigned vm_idsrss;		
	unsigned vm_ixrss;		
	int	vm_maxrss;		
	int	vm_majflt;		
	int	vm_minflt;		
	int	vm_nswap;		
	int	vm_inblk;		
	int	vm_oublk;		
};
struct timeb
{
	time_t	time;
	unsigned short millitm;
	short	timezone;
	short	dstflag;
};
struct vtimes newv, oldv;
struct timeb newt, oldt;
char	*cpp = "/lib/cpp";
char	*ccom = "/usr/pjw/comp/vcc/comp.sv";
char	*c2 = "/lib/c2";
char	*as = "/bin/as";
char	*ld = "/bin/ld";
char	*crt0 = "/lib/crt0.o";
char	tmp0[30];		
char	*tmp1, *tmp2, *tmp3, *tmp4, *tmp5;
char	*outfile;
char	*savestr(), *strspl(), *setsuf();
int	idexit();
char	**av, **clist, **llist, **plist;
int	cflag, eflag, gflag, oflag, pflag, sflag, wflag, Rflag, exflag, proflag;
int	vflag; 
char	*dflag;
int	exfail;
char	*chpass;
char	*npassname;
extern	int	optind;
extern	int	opterr;
extern	char	*optarg;
extern	int	optopt;
int	nc, nl, np, nxo, na;
main(argc, argv)
	char **argv;
{
	char *t;
	char *assource;
	int i, j, c;
	
	av = (char **)calloc(argc+10, sizeof (char **));
	clist = (char **)calloc(argc, sizeof (char **));
	llist = (char **)calloc(argc, sizeof (char **));
	plist = (char **)calloc(argc, sizeof (char **));
	opterr = 0;
	while (optind<argc) switch (c = getopt(argc, argv, "vsSo:ROPgwEpPcD:I:U:C:t:B:l:d:")) {
	case 'v':
		vflag++;
		continue;
	case 'S':
		sflag++;
		cflag++;
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
	case 'w':
		wflag++;
		continue;
	case 'E':
		exflag++;
	case 'P':
		pflag++;
		t = strspl("-", "x");
		t[1] = optopt;
		plist[np++] = t;
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
			npassname = "/usr/c/o";
		continue;
	case 'd':
		dflag = strspl("-d", optarg);
		continue;
	case '?':
	case 's':
		t = strspl("-", "x");
		t[1] = optopt;
		llist[nl++] = t;
		continue;
	case (-1):
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
	if (gflag) {
		if (oflag)
			fprintf((&_iob[2]), "cc: warning: -g disables -O\n");
		oflag = 0;
	}
	if (npassname && chpass ==0)
		chpass = "012p";
	if (chpass && npassname==0)
		npassname = "/usr/new";
	if (chpass)
	for (t=chpass; *t; t++) {
		switch (*t) {
		case '0':
			ccom = strspl(npassname, "ccom");
			continue;
		case '2':
			c2 = strspl(npassname, "c2");
			continue;
		case 'p':
			cpp = strspl(npassname, "cpp");
			continue;
		}
	}
	if (proflag)
		crt0 = "/lib/mcrt0.o";
	if (nc==0)
		goto nocom;
	if (signal(2	, 	(int (*)())1) != 	(int (*)())1)
		(void)signal(2	, idexit);
	if (signal(15	, 	(int (*)())1) != 	(int (*)())1)
		(void)signal(15	, idexit);
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
			fflush((&_iob[1]));
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
		av[0] = "cpp";
		av[1] = clist[i];
		av[2] = exflag ? "-" : tmp4;
		na = 3;
		for (j = 0; j < np; j++)
			av[na++] = plist[j];
		av[na++] = 0;
		switch (callsys(cpp, av)) {
			case 0:
				break;
			default:
				if (callsys("/lib/cpre",av)) {
					exfail++;
					exflag++;
				}
		}
		if (pflag || exfail) {
			cflag++;
			continue;
		}
		if (sflag)
			assource = tmp3 = setsuf(clist[i], 's');
compile:
		av[0] = "ccom";
		av[1] = suffix=='i'? clist[i]: tmp4;
		av[2] = oflag? tmp5:tmp3;
		na = 3;
		if (proflag)
			av[na++] = "-XP";
		if (gflag)
			av[na++] = "-Xg";
		if (wflag)
			av[na++] = "-w";
		av[na] = 0;
		if (callsys(ccom, av)) {
			cflag++;
			eflag++;
			continue;
		}
		if (oflag) {
			av[0] = "c2"; av[1] = tmp5; av[2] = tmp3; av[3] = 0;
			if (callsys(c2, av)) {
				unlink(tmp3);
				tmp3 = assource = tmp5;
			} else
				unlink(tmp5);
		}
		if (sflag)
			continue;
	assemble:
			if (tmp1) unlink(tmp1); 	if (tmp2) unlink(tmp2); 	if (tmp4) unlink(tmp4);
		av[0] = "as"; av[1] = "-o"; av[2] = setsuf(clist[i], 'o');
		na = 3;
		if (Rflag)
			av[na++] = "-R";
		if (dflag)
			av[na++] = dflag;
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
		av[0] = "ld"; av[1] = "-X"; av[2] = crt0; na = 3;
		if (outfile) {
			av[na++] = "-o";
			av[na++] = outfile;
		}
		while (i < nl)
			av[na++] = llist[i++];
		if (gflag)
			av[na++] = "-lg";
		av[na++] = "-lc";
		av[na++] = 0;
		eflag |= callsys(ld, av);
		if (nc==1 && nxo==1 && eflag==0)
			unlink(setsuf(clist[0], 'o'));
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
			if (tmp1) unlink(tmp1);
			if (tmp2) unlink(tmp2);
		if (sflag==0)
				if (tmp3) unlink(tmp3);
			if (tmp4) unlink(tmp4);
			if (tmp5) unlink(tmp5);
	}
	exit(eflag);
}
error(s, x)
	char *s, *x;
{
	struct _iobuf *diag = exflag ? (&_iob[2]) : (&_iob[1]);
	fprintf(diag, "cc: ");
	fprintf(diag, s, x);
	 (--( diag)->_cnt>=0? ((int)(*( diag)->_ptr++=(unsigned)('\n'))):_flsbuf((unsigned)('\n'), diag));
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
	if (c <= 14 && c > 2 && *s++ == '.')
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
	register char **vp;	
	if(vflag) {	
		vp = v;
		fprintf((&_iob[2]),"+ ");
		while (*vp)
			fprintf((&_iob[2]),"%s ",*vp++);
		fprintf((&_iob[2]), "\n");
	}
	ftime(&oldt);
	vtimes(0, &oldv);
	t = vfork();
	if (t == -1) {
		printf("No more processes\n");
		return (100);
	}
	if (t == 0) {
		execv(f, v);
		printf("Can't find %s\n", f);
		fflush((&_iob[1]));
		_exit(100);
	}
	while (t != wait(&status))
		;
	ftime(&newt);
	vtimes(0, &newv);
	printf("%s\n", f);
	printf("real %g user %g sys %g ", newt.time - oldt.time
		+ newt.millitm / 1000. - oldt.millitm / 1000.,
		(newv.vm_utime - oldv.vm_utime) / 60.,
		(newv.vm_stime - oldv.vm_stime) / 60.);
	printf("reads %d writes %d\n", newv.vm_inblk - oldv.vm_inblk,
		newv.vm_oublk - oldv.vm_oublk);
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
char	*savetab;
int	saveleft;
char *
savestr(cp)
	register char *cp;
{
	register int len;
	len = strlen(cp) + 1;
	if (len > saveleft) {
		saveleft = 1024;
		if (len > saveleft)
			saveleft = len;
		savetab = (char *)malloc(saveleft);
		if (savetab == 0) {
			fprintf((&_iob[2]), "ran out of memory (savestr)\n");
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
	char buf[1024];
	strcpy(buf, left);
	strcat(buf, right);
	return (savestr(buf));
}
