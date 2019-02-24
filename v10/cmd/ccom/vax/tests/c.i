/* ac.c */
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
struct tm { 
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};
struct utmp {
	char	ut_line[8];		
	char	ut_name[8];		
	long	ut_time;		
};
struct	utmp ibuf, ubuf[400	], tbuf[101	];
int	nnames;		
char	*wtmp = "/usr/adm/wtmp";
char	REBOOT[] = "~";
char	OLDTIME[] = "|";
char	NEWTIME[] = "{";
char	DATERR[] = "ac: botched date change\n";
int	pflag, byday, tflag;
time_t	lasttime, dtime, starttime, stoptime;
int	ncount;
char	*SYSBUF[1024];
char	**nptr;
char	*ctime(), *strncpy();
time_t	time(), nexttime(), prevtime();
struct	tm *localtime();
main(argc, argv)
	register int argc;
	register char **argv;
{
	register struct _iobuf *wf;
	setbuf((&_iob[1]), SYSBUF);
	while (--argc > 0 && **++argv == '-')
	switch(*++*argv) {
	case 'd':
		byday++;
		continue;
	case 'w':
		if (--argc > 0)
			wtmp = *++argv;
		continue;
	case 'p':
		pflag++;
		continue;
	case 't':
		tflag++;
		pflag++;
		continue;
	}
	ncount = argc;
	nptr = argv;
	if ((wf = fopen(wtmp, "r")) == 0) {
		fprintf((&_iob[2]), "ac: cannot open %s\n", wtmp);
		exit(1);
	}
	while (fread((char *)&ibuf, sizeof(ibuf), 1, wf) == 1) {
		loop();
		lasttime = ibuf.ut_time;
	}
	*ibuf.ut_name = '\0';
		(void)strncpy(ibuf.ut_line,  REBOOT, sizeof(ibuf.ut_line));
	time(&ibuf.ut_time);
	loop();
	print();
	exit(0);
}
loop()
{
	register struct utmp *tp;
	register char *p;
	register int n, i;
	if (tflag && *ibuf.ut_name)
			(void)strncpy(ibuf.ut_name,  ibuf.ut_line, sizeof(ibuf.ut_name));
	if (	(*ibuf.ut_line == *  NEWTIME && strncmp(ibuf.ut_line,   NEWTIME, sizeof(ibuf.ut_line)) == 0)) {
		if (dtime == 0) {
			fprintf((&_iob[2]), DATERR);
			return;
		}
			for(tp = tbuf; tp < &tbuf[101	]; tp++) if (*tp->ut_line)
			tp->ut_time += ibuf.ut_time - dtime;
		dtime = 0;
		return;
	}
	if (dtime != 0) {
		fprintf((&_iob[2]), DATERR);
		dtime = 0;
	}
	if (	(*ibuf.ut_line == *  OLDTIME && strncmp(ibuf.ut_line,   OLDTIME, sizeof(ibuf.ut_line)) == 0)) {
		dtime = ibuf.ut_time;
		return;
	}
	if (lasttime > ibuf.ut_time + 10	)
		fprintf((&_iob[2]), "ac: entry not in time sequence\n");
	if (byday) {
		if (starttime == 0 || starttime > ibuf.ut_time + 10	) {
			starttime = prevtime(ibuf.ut_time);
			stoptime = nexttime(ibuf.ut_time);
		}
		while (ibuf.ut_time >= stoptime) {
				for(tp = tbuf; tp < &tbuf[101	]; tp++) if (*tp->ut_line) {
				update(tp, stoptime);
				tp->ut_time = stoptime;
			}
			print();
			starttime = stoptime;
			stoptime = nexttime(stoptime);
		}
	}
	if (	(*ibuf.ut_line == *  REBOOT && strncmp(ibuf.ut_line,   REBOOT, sizeof(ibuf.ut_line)) == 0)) {
			for(tp = tbuf; tp < &tbuf[101	]; tp++) if (*tp->ut_line) {
			update(tp, ibuf.ut_time);
			*tp->ut_name = '\0';
			tp->ut_time = 0;
		}
		return;
	}
	i = 0;
	p = ibuf.ut_line;
	while (n = *p++)
		i += i+n;
	tp = &tbuf[((unsigned)(i*2)) % 101	];
	n = 101	; do {
		if (	(*tp->ut_line == * ibuf.ut_line && strncmp(tp->ut_line,  ibuf.ut_line, sizeof(tp->ut_line)) == 0)) {
			if (*tp->ut_name)
				update(tp, ibuf.ut_time);
			*tp = ibuf;
			return;
		}
		if (*tp->ut_line == '\0') {
			*tp = ibuf;
			return;
		}
		if (--tp < tbuf)
			tp += 101	;
	} while (--n);
	fprintf((&_iob[2]), "ac: tbuf table overflow\n");
	exit(1);
}
update(tp, t)
	register struct utmp *tp;
	time_t t;
{
	register struct utmp *up;
	register int n;
	if (*tp->ut_name && among(tp)) {
		t -= tp->ut_time;
		if (t < 0)
			fprintf((&_iob[2]), "ac: strange login: %-8.8s%6.2f\n",
				tp->ut_name, (double)t/3600);
		up = ubuf;
		if ((n = nnames) > 0) do {
			if (	(*up->ut_name == * tp->ut_name && strncmp(up->ut_name,  tp->ut_name, sizeof(up->ut_name)) == 0)) {
				up->ut_time += t;
				return;
			}
			up++;
		} while (--n);
		if (nnames < 400	) {
				(void)strncpy(up->ut_name,  tp->ut_name, sizeof(up->ut_name));
			up->ut_time = t;
			nnames++;
			return;
		}
		fprintf((&_iob[2]), "ac: ubuf table overflow\n");
		exit(1);
	}
}
among(up)
	register struct utmp *up;
{
	register int j;
	if (ncount <= 0)
		return(1);
	for (j = 0; j < ncount; j++)
		if (	(*up->ut_name == * nptr[j] && strncmp(up->ut_name,  nptr[j], sizeof(up->ut_name)) == 0))
			return(1);
	return(0);
}
print()
{
	register struct utmp *up;
	register int n;
	time_t ttime, t;
	int namecomp();
	qsort(ubuf, nnames, sizeof(ubuf[0]), namecomp);
	ttime = 0;
	up = ubuf;
	if ((n = nnames) > 0) do {
		ttime += up->ut_time;
		if (pflag && up->ut_time != 0)
			printf("\t%-8.8s%6.2f\n",
				up->ut_name, (double)up->ut_time/3600);
		*up->ut_name = '\0';
		up++;
	} while (--n);
	nnames = 0;
	if (ttime != 0) {
		if (byday) {
			t = stoptime-1;
			printf("%.6s", ctime(&t)+4);
		}
		printf("\ttotal%9.2f\n", (double)ttime/3600);
	}
	fflush((&_iob[1]));
}
namecomp(up1, up2)
	register struct utmp *up1, *up2;
{
	return(	strncmp(up1->ut_name,  up2->ut_name, sizeof(up1->ut_name)));
}
time_t
nexttime(t)
	time_t t;
{
	register int curday;
	time_t n;
	curday = localtime(&t)->tm_yday;
	for (n = 86400L; n; n >>= 1) {
		do {
			t += n;
		} while (curday == localtime(&t)->tm_yday);
		t -= n;
	}
	return(t+1);
}
time_t
prevtime(t)
	time_t t;
{
	register int curday;
	time_t n;
	curday = localtime(&t)->tm_yday;
	for (n = 86400L; n; n >>= 1) {
		do {
			t -= n;
		} while (curday == localtime(&t)->tm_yday);
		t += n;
	}
	return(t);
}
