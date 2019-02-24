/*
 * Editor
 */

#include <signal.h>
#include <setjmp.h>

/* make BLKSIZE and LBSIZE 512 for smaller machines */
#define	BLKSIZE	4096
#define	NBLK	2047

#define	NULL	0
#define	FNSIZE	128
#define	LBSIZE	4096
#define	ESIZE	256
#define	GBSIZE	256
#define	NBRA	5
#define	EOF	-1
#define	KSIZE	9

#define	CBRA	1
#define	CCHR	2
#define	CDOT	4
#define	CCL	6
#define	NCCL	8
#define	CDOL	10
#define	CEOF	11
#define	CKET	12
#define	CBACK	14
#define	CCIRC	15

#define	STAR	01

char	Q[]	= "";
char	T[]	= "TMP";
#define	READ	0
#define	WRITE	1

int	peekc;
int	lastc;
char	savedfile[FNSIZE];
char	file[FNSIZE];
char	linebuf[LBSIZE];
char	rhsbuf[LBSIZE/2];
char	expbuf[ESIZE+4];
int	given;
unsigned int	*addr1, *addr2;
unsigned int	*dot, *dol, *zero;
char	genbuf[LBSIZE];
long	count;
char	*nextip;
char	*linebp;
int	ninbuf;
int	io;
int	pflag;
long	lseek();
SIG_TYP	oldhup;
SIG_TYP	oldquit;
int	vflag	= 1;
int	oflag;
int	listf;
int	listn;
int	col;
char	*globp;
int	tfile	= -1;
int	tline;
char	*tfname;
char	*loc1;
char	*loc2;
char	ibuff[BLKSIZE];
int	iblock	= -1;
char	obuff[BLKSIZE];
int	oblock	= -1;
int	ichanged;
int	nleft;
char	WRERR[]	= "WRITE ERROR";
int	names[26];
int	anymarks;
char	*braslist[NBRA];
char	*braelist[NBRA];
int	nbra;
int	subnewa;
int	subolda;
int	fchange;
int	wrapp;
int	bpagesize = 20;
unsigned nlall = 128;

unsigned int	*address();
char	*getline();
char	*getblock();
char	*place();
char	*mktemp();
char	*malloc();
char	*realloc();
jmp_buf	savej;

main(argc, argv)
char **argv;
{
	register char *p1, *p2;
	extern int onintr(), quit(), onhup();
	SIG_TYP oldintr;

	oldquit = signal(SIGQUIT, SIG_IGN);
	oldhup = signal(SIGHUP, SIG_IGN);
	oldintr = signal(SIGINT, SIG_IGN);
	if (signal(SIGTERM, SIG_IGN) == SIG_DFL)
		signal(SIGTERM, quit);
	argv++;
	while (argc > 1 && **argv=='-') {
		switch((*argv)[1]) {

		case '\0':
			vflag = 0;
			break;

		case 'q':
			signal(SIGQUIT, SIG_DFL);
			vflag = 1;
			break;

		case 'o':
			oflag = 1;
			break;
		}
		argv++;
		argc--;
	}
	if (oflag) {
		p1 = "/dev/stdout";
		p2 = savedfile;
		while (*p2++ = *p1++)
			;
	}
	if (argc>1) {
		p1 = *argv;
		p2 = savedfile;
		while (*p2++ = *p1++)
			if (p2 >= &savedfile[sizeof(savedfile)])
				p2--;
		globp = "r";
	}
	zero = (unsigned *)malloc(nlall*sizeof(unsigned));
	tfname = mktemp("/tmp/eXXXXX");
	init();
	if (oldintr!=SIG_IGN)
		signal(SIGINT, onintr);
	if (oldhup!=SIG_IGN)
		signal(SIGHUP, onhup);
	setjmp(savej);
	commands();
	quit();
}

commands()
{
	int getfile(), gettty();
	register unsigned *a1;
	register c;
	register int temp;
	char lastsep;

	for (;;) {
	if (pflag) {
		pflag = 0;
		addr1 = addr2 = dot;
		print();
	}
	c = '\n';
	for (addr1 = 0;;) {
		lastsep = c;
		a1 = address();
		c = getchr();
		if (c!=',' && c!=';')
			break;
		if (lastsep==',')
			error(Q);
		if (a1==0) {
			a1 = zero+1;
			if (a1>dol)
				a1--;
		}
		addr1 = a1;
		if (c==';')
			dot = a1;
	}
	if (lastsep!='\n' && a1==0)
		a1 = dol;
	if ((addr2=a1)==0) {
		given = 0;
		addr2 = dot;	
	}
	else
		given = 1;
	if (addr1==0)
		addr1 = addr2;
	switch(c) {

	case 'a':
		add(0);
		continue;

	case 'b':
		nonzero();
		browse();
		continue;

	case 'c':
		nonzero();
		newline();
		rdelete(addr1, addr2);
		append(gettty, addr1-1);
		continue;

	case 'd':
		nonzero();
		newline();
		rdelete(addr1, addr2);
		continue;

	case 'E':
		fchange = 0;
		c = 'e';
	case 'e':
		setnoaddr();
		if (vflag && fchange) {
			fchange = 0;
			error(Q);
		}
		filename(c);
		init();
		addr2 = zero;
		goto caseread;

	case 'f':
		setnoaddr();
		filename(c);
		puts(savedfile);
		continue;

	case 'g':
		global(1);
		continue;

	case 'i':
		add(-1);
		continue;


	case 'j':
		if (!given)
			addr2++;
		newline();
		join();
		continue;

	case 'k':
		nonzero();
		if ((c = getchr()) < 'a' || c > 'z')
			error(Q);
		newline();
		names[c-'a'] = *addr2 & ~01;
		anymarks |= 01;
		continue;

	case 'm':
		move(0);
		continue;

	case 'n':
		listn++;
		newline();
		print();
		continue;

	case '\n':
		if (a1==0) {
			a1 = dot+1;
			addr2 = a1;
			addr1 = a1;
		}
		if (lastsep==';')
			addr1 = a1;
		print();
		continue;

	case 'l':
		listf++;
	case 'p':
	case 'P':
		newline();
		print();
		continue;

	case 'Q':
		fchange = 0;
	case 'q':
		setnoaddr();
		newline();
		quit();

	case 'r':
		filename(c);
	caseread:
		if ((io = open(file, 0)) < 0) {
			lastc = '\n';
			error(file);
		}
		setwide();
		squeeze(0);
		ninbuf = 0;
		c = zero != dol;
		append(getfile, addr2);
		exfile();
		fchange = c;
		continue;

	case 's':
		nonzero();
		substitute(globp!=0);
		continue;

	case 't':
		move(1);
		continue;

	case 'u':
		nonzero();
		newline();
		if ((*addr2&~01) != subnewa)
			error(Q);
		*addr2 = subolda;
		dot = addr2;
		continue;

	case 'v':
		global(0);
		continue;

	case 'W':
		wrapp++;
	case 'w':
		setwide();
		squeeze(dol>zero);
		if ((temp = getchr()) != 'q' && temp != 'Q') {
			peekc = temp;
			temp = 0;
		}
		filename(c);
		if(!wrapp ||
		  ((io = open(file,1)) == -1) ||
		  ((lseek(io, 0L, 2)) == -1))
			if ((io = creat(file, 0666)) < 0)
				error(file);
		wrapp = 0;
		if (dol > zero)
			putfile();
		exfile();
		if (addr1<=zero+1 && addr2==dol)
			fchange = 0;
		if (temp == 'Q')
			fchange = 0;
		if (temp)
			quit();
		continue;

	case '=':
		setwide();
		squeeze(0);
		newline();
		count = addr2 - zero;
		putd();
		putchr('\n');
		continue;

	case '!':
		callunix();
		continue;

	case EOF:
		return;

	}
	error(Q);
	}
}

print()
{
	register unsigned *a1;

	nonzero();
	a1 = addr1;
	do {
		if (listn) {
			count = a1-zero;
			putd();
			putchr('\t');
		}
		puts(getline(*a1++));
	} while (a1 <= addr2);
	dot = addr2;
	listf = 0;
	listn = 0;
	pflag = 0;
}

unsigned *
address()
{
	register int sign;
	register unsigned *a, *b;
	int opcnt, nextopand;
	register int c;

	nextopand = -1;
	sign = 1;
	opcnt = 0;
	a = dot;
	do {
		do c = getchr(); while (c==' ' || c=='\t');
		if ('0'<=c && c<='9') {
			peekc = c;
			if (!opcnt)
				a = zero;
			a += sign*getnum();
		} else switch (c) {
		case '$':
			a = dol;
			/* fall through */
		case '.':
			if (opcnt)
				error(Q);
			break;
		case '\'':
			c = getchr();
			if (opcnt || c<'a' || 'z'<c)
				error(Q);
			a = zero;
			do a++; while (a<=dol && names[c-'a']!=(*a&~01));
			break;
		case '?':
			sign = -sign;
			/* fall through */
		case '/':
			compile(c);
			b = a;
			for (;;) {
				a += sign;
				if (a<=zero)
					a = dol;
				if (a>dol)
					a = zero;
				if (execute(a))
					break;
				if (a==b)
					error(Q);
			}
			break;
		default:
			if (nextopand == opcnt) {
				a += sign;
				if (a<zero || dol<a)
					continue;       /* error(Q); */
			}
			if (c!='+' && c!='-' && c!='^') {
				peekc = c;
				if (opcnt==0)
					a = 0;
				return (a);
			}
			sign = 1;
			if (c!='+')
				sign = -sign;
			nextopand = ++opcnt;
			continue;
		}
		sign = 1;
		opcnt++;
	} while (zero<=a && a<=dol);
	error(Q);
	/*NOTREACHED*/
}

int getnum()
{
	register int r, c;

	r = 0;
	while ((c=getchr())>='0' && c<='9')
		r = r*10 + c - '0';
	peekc = c;
	return (r);
}

setwide()
{
	if (!given) {
		addr1 = zero + (dol>zero);
		addr2 = dol;
	}
}

setnoaddr()
{
	if (given)
		error(Q);
}

nonzero()
{
	squeeze(1);
}

squeeze(i)
int i;
{
	if (addr1<zero+i || addr2>dol || addr1>addr2)
		error(Q);
}

newline()
{
	register c;

	if ((c = getchr()) == '\n' || c == EOF)
		return;
	if (c=='p' || c=='l' || c=='n') {
		pflag++;
		if (c=='l')
			listf++;
		else if (c=='n')
			listn++;
		if ((c=getchr())=='\n')
			return;
	}
	error(Q);
}

filename(comm)
{
	register char *p1, *p2;
	register c;

	count = 0;
	c = getchr();
	if (c=='\n' || c==EOF) {
		p1 = savedfile;
		if (*p1==0 && comm!='f')
			error(Q);
		p2 = file;
		while (*p2++ = *p1++)
			;
		return;
	}
	if (c!=' ')
		error(Q);
	while ((c = getchr()) == ' ')
		;
	if (c=='\n')
		error(Q);
	p1 = file;
	do {
		if (p1 >= &file[sizeof(file)-1] || c==' ' || c==EOF)
			error(Q);
		*p1++ = c;
	} while ((c = getchr()) != '\n');
	*p1++ = 0;
	if (savedfile[0]==0 || comm=='e' || comm=='f') {
		p1 = savedfile;
		p2 = file;
		while (*p1++ = *p2++)
			;
	}
}

exfile()
{
	close(io);
	io = -1;
	if (vflag) {
		putd();
		putchr('\n');
	}
}

onintr()
{
	signal(SIGINT, onintr);
	putchr('\n');
	lastc = '\n';
	error(Q);
}

onhup()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	if (dol > zero) {
		addr1 = zero+1;
		addr2 = dol;
		io = creat("ed.hup", 0600);
		if (io > 0)
			putfile();
	}
	fchange = 0;
	quit();
}

error(s)
char *s;
{
	register c;

	wrapp = 0;
	listf = 0;
	listn = 0;
	putchr('?');
	puts(s);
	count = 0;
	lseek(0, (long)0, 2);
	pflag = 0;
	if (globp)
		lastc = '\n';
	globp = 0;
	peekc = lastc;
	if(lastc)
		while ((c = getchr()) != '\n' && c != EOF)
			;
	if (io > 0) {
		close(io);
		io = -1;
	}
	longjmp(savej, 1);
}

getchr()
{
	char c;
	if (lastc=peekc) {
		peekc = 0;
		return(lastc);
	}
	if (globp) {
		if ((lastc = *globp++) != 0)
			return(lastc);
		globp = 0;
		return(EOF);
	}
	if (read(0, &c, 1) <= 0)
		return(lastc = EOF);
	lastc = c&0177;
	return(lastc);
}

gettty()
{
	register int rc;

	if (rc = gety())
		return(rc);
	if (linebuf[0]=='.' && linebuf[1]==0)
		return(EOF);
	return(0);
}

gety()
{
	register int c;
	register char *gf;
	register char *p;

	p = linebuf;
	gf = globp;
	while ((c = getchr()) != '\n') {
		if (c==EOF) {
			if (gf)
				peekc = c;
			return(c);
		}
		if ((c &= 0177) == 0)
			continue;
		*p++ = c;
		if (p >= &linebuf[LBSIZE-2])
			error(Q);
	}

	*p++ = 0;
	return(0);
}

getfile()
{
	register c;
	register char *lp, *fp;

	lp = linebuf;
	fp = nextip;
	do {
		if (--ninbuf < 0) {
			if ((ninbuf = read(io, genbuf, LBSIZE)-1) < 0)
				if (lp>linebuf) {
					puts("'\\n' appended");
					*genbuf = '\n';
				}
				else return(EOF);
			fp = genbuf;
			while(fp < &genbuf[ninbuf]) {
				if (*fp++ & 0200)
					break;
			}
			fp = genbuf;
		}
		c = *fp++;
		if (c=='\0')
			continue;
		if (c&0200 || lp >= &linebuf[LBSIZE]) {
			lastc = '\n';
			error(Q);
		}
		*lp++ = c;
		count++;
	} while (c != '\n');
	*--lp = 0;
	nextip = fp;
	return(0);
}

putfile()
{
	register unsigned *a1;
	register int n;
	register char *fp, *lp;
	register nib;

	nib = BLKSIZE;
	fp = genbuf;
	a1 = addr1;
	do {
		lp = getline(*a1++);
		for (;;) {
			if (--nib < 0) {
				n = fp-genbuf;
				if(write(io, genbuf, n) != n) {
					puts(WRERR);
					error(Q);
				}
				nib = BLKSIZE-1;
				fp = genbuf;
			}
			count++;
			if ((*fp++ = *lp++) == 0) {
				fp[-1] = '\n';
				break;
			}
		}
	} while (a1 <= addr2);
	n = fp-genbuf;
	if(write(io, genbuf, n) != n) {
		puts(WRERR);
		error(Q);
	}
}

append(f, a)
unsigned *a;
int (*f)();
{
	register unsigned *a1, *a2, *rdot;
	int nline, tl;

	nline = 0;
	dot = a;
	while ((*f)() == 0) {
		if ((dol-zero)+1 >= nlall) {
			unsigned *ozero = zero;

			nlall += 1024;
			if ((zero = (unsigned *)realloc((char *)zero, nlall*sizeof(unsigned)))==NULL) {
				error("MEM?");
				onhup();
			}
			dot += zero - ozero;
			dol += zero - ozero;
		}
		tl = putline();
		nline++;
		a1 = ++dol;
		a2 = a1+1;
		rdot = ++dot;
		while (a1 > rdot)
			*--a2 = *--a1;
		*rdot = tl;
	}
	return(nline);
}

add(i)
int i;
{
	if (i && (given || dol>zero)) {
		addr1--;
		addr2--;
	}
	squeeze(0);
	newline();
	append(gettty, addr2);
}

browse()
{
	register forward, n;
	static bformat;	/*0*/
	static bnum; /*0*/

	forward = 1;
	if((peekc=getchr()) != '\n'){
		if (peekc=='-' || peekc=='+') {
			if(peekc=='-')
				forward=0;
			getchr();
		}
		if((n=getnum()) > 0)
			bpagesize = n;
	}
	newline();
	if (pflag) {
		bformat = listf;
		bnum = listn;
	} else {
		listf = bformat;
		listn = bnum;
	}
	if (forward) {
		addr1 = addr2;
		if((addr2 += bpagesize)>dol)
			addr2 = dol;
	} else {
		if((addr1=addr2-bpagesize) <= zero)
			addr1 = zero+1;
	}
	print();
}

callunix()
{
	SIG_TYP savint;
	register pid, rpid;
	int retcode;

	setnoaddr();
	if ((pid = fork()) == 0) {
		signal(SIGHUP, oldhup);
		signal(SIGQUIT, oldquit);
		execl("/bin/sh", "sh", "-t", 0);
		exit(0100);
	}
	savint = signal(SIGINT, SIG_IGN);
	while ((rpid = wait(&retcode)) != pid && rpid != -1)
		;
	signal(SIGINT, savint);
	if (vflag) {
		puts("!");
	}
}

quit()
{
	if (vflag && fchange && dol!=zero) {
		fchange = 0;
		error(Q);
	}
	unlink(tfname);
	exit(0);
}

rdelete(ad1, ad2)
unsigned *ad1, *ad2;
{
	register unsigned *a1, *a2, *a3;

	a1 = ad1;
	a2 = ad2+1;
	a3 = dol;
	dol -= a2 - a1;
	do {
		*a1++ = *a2++;
	} while (a2 <= a3);
	a1 = ad1;
	if (a1 > dol)
		a1 = dol;
	dot = a1;
	fchange = 1;
}

gdelete()
{
	register unsigned *a1, *a2, *a3;

	a3 = dol;
	for (a1=zero; (*a1&01)==0; a1++)
		if (a1>=a3)
			return;
	for (a2=a1+1; a2<=a3;) {
		if (*a2&01) {
			a2++;
			dot = a1;
		} else
			*a1++ = *a2++;
	}
	dol = a1-1;
	if (dot>dol)
		dot = dol;
	fchange = 1;
}

char *
getline(tl)
unsigned tl;
{
	register char *bp, *lp;
	register nl;

	lp = linebuf;
	bp = getblock(tl, READ);
	nl = nleft;
	tl &= ~((BLKSIZE/2)-1);
	while (*lp++ = *bp++)
		if (--nl == 0) {
			bp = getblock(tl+=(BLKSIZE/2), READ);
			nl = nleft;
		}
	return(linebuf);
}

putline()
{
	register char *bp, *lp;
	register nl;
	register unsigned tl;

	fchange = 1;
	lp = linebuf;
	tl = tline;
	bp = getblock(tl, WRITE);
	nl = nleft;
	tl &= ~((BLKSIZE/2)-1);
	while (*bp = *lp++) {
		if (*bp++ == '\n') {
			*--bp = 0;
			linebp = lp;
			break;
		}
		if (--nl == 0) {
			bp = getblock(tl+=(BLKSIZE/2), WRITE);
			nl = nleft;
		}
	}
	nl = tline;
	tline += (((lp-linebuf)+03)>>1)&077776;
	return(nl);
}

char *
getblock(atl, iof)
unsigned atl;
{
	extern read(), write();
	register bno, off;
	
	bno = (atl/(BLKSIZE/2));
	off = (atl<<1) & (BLKSIZE-1) & ~03;
	if (bno >= NBLK) {
		lastc = '\n';
		error(T);
	}
	nleft = BLKSIZE - off;
	if (bno==iblock) {
		ichanged |= iof;
		return(ibuff+off);
	}
	if (bno==oblock)
		return(obuff+off);
	if (iof==READ) {
		if (ichanged)
			blkio(iblock, ibuff, write);
		ichanged = 0;
		iblock = bno;
		blkio(bno, ibuff, read);
		return(ibuff+off);
	}
	if (oblock>=0)
		blkio(oblock, obuff, write);
	oblock = bno;
	return(obuff+off);
}

blkio(b, buf, iofcn)
char *buf;
int (*iofcn)();
{
	lseek(tfile, (long)b*BLKSIZE, 0);
	if ((*iofcn)(tfile, buf, BLKSIZE) != BLKSIZE) {
		error(T);
	}
}

init()
{
	register *markp;

	close(tfile);
	tline = 2;
	for (markp = names; markp < &names[26]; )
		*markp++ = 0;
	subnewa = 0;
	anymarks = 0;
	iblock = -1;
	oblock = -1;
	ichanged = 0;
	close(creat(tfname, 0600));
	tfile = open(tfname, 2);
	dot = dol = zero;
}

global(k)
{
	register char *gp;
	register c;
	register unsigned *a1;
	char globuf[GBSIZE];

	if (globp)
		error(Q);
	setwide();
	squeeze(dol>zero);
	if ((c=getchr())=='\n')
		error(Q);
	compile(c);
	gp = globuf;
	while ((c = getchr()) != '\n') {
		if (c==EOF)
			error(Q);
		if (c=='\\') {
			c = getchr();
			if (c!='\n')
				*gp++ = '\\';
		}
		*gp++ = c;
		if (gp >= &globuf[GBSIZE-2])
			error(Q);
	}
	if (gp == globuf)
		*gp++ = 'p';
	*gp++ = '\n';
	*gp++ = 0;
	for (a1=zero; a1<=dol; a1++) {
		*a1 &= ~01;
		if (a1>=addr1 && a1<=addr2 && execute(a1)==k)
			*a1 |= 01;
	}
	/*
	 * Special case: g/.../d (avoid n^2 algorithm)
	 */
	if (globuf[0]=='d' && globuf[1]=='\n' && globuf[2]=='\0') {
		gdelete();
		return;
	}
	for (a1=zero; a1<=dol; a1++) {
		if (*a1 & 01) {
			*a1 &= ~01;
			dot = a1;
			globp = globuf;
			commands();
			a1 = zero;
		}
	}
}

join()
{
	register char *gp, *lp;
	register unsigned *a1;

	nonzero();
	gp = genbuf;
	for (a1=addr1; a1<=addr2; a1++) {
		lp = getline(*a1);
		while (*gp = *lp++)
			if (gp++ >= &genbuf[LBSIZE-2])
				error(Q);
	}
	lp = linebuf;
	gp = genbuf;
	while (*lp++ = *gp++)
		;
	*addr1 = putline();
	if (addr1<addr2)
		rdelete(addr1+1, addr2);
	dot = addr1;
}

substitute(inglob)
{
	register *mp, nl;
	register unsigned *a1;
	int gsubf;
	int getsub();
	int n;

	n = getnum();	/* OK even if n==0 */
	gsubf = compsub();
	for (a1 = addr1; a1 <= addr2; a1++) {
		if (execute(a1)){
			unsigned *ozero;
			int m = n;
			do {
				int span = loc2-loc1;
				if (--m <= 0) {
					dosub();
					if (!gsubf)
						break;
					if (span==0) {	/* null RE match */
						if (*loc2=='\0')
							break;
						loc2++;
					}
				}
			} while (execute((unsigned *)0));
			if (m <= 0) {
				inglob |= 01;
				subnewa = putline();
				*a1 &= ~01;
				if (anymarks) {
					for (mp = names; mp < &names[26]; mp++)
						if (*mp == *a1)
							*mp = subnewa;
				}
				subolda = *a1;
				*a1 = subnewa;
				ozero = zero;
				nl = append(getsub, a1);
				nl += zero-ozero;
				a1 += nl;
				addr2 += nl;
			}
		}
	}
	if (inglob==0)
		error(Q);
}

compsub()
{
	register seof, c;
	register char *p;

	if ((seof = getchr()) == '\n' || seof == ' ')
		error(Q);
	compile(seof);
	p = rhsbuf;
	for (;;) {
		c = getchr();
		if (c=='\\')
			c = getchr() | 0200;
		if (c=='\n') {
			if (globp && globp[0])	/* last '\n' does not count */
				c |= 0200;
			else {
				peekc = c;
				pflag++;
				break;
			}
		}
		if (c==seof)
			break;
		*p++ = c;
		if (p >= &rhsbuf[LBSIZE/2])
			error(Q);
	}
	*p++ = 0;
	if ((peekc = getchr()) == 'g') {
		peekc = 0;
		newline();
		return(1);
	}
	newline();
	return(0);
}

getsub()
{
	register char *p1, *p2;

	p1 = linebuf;
	if ((p2 = linebp) == 0)
		return(EOF);
	while (*p1++ = *p2++)
		;
	linebp = 0;
	return(0);
}

dosub()
{
	register char *lp, *sp, *rp;
	int c;

	lp = linebuf;
	sp = genbuf;
	rp = rhsbuf;
	while (lp < loc1)
		*sp++ = *lp++;
	while (c = *rp++&0377) {
		if (c=='&') {
			sp = place(sp, loc1, loc2);
			continue;
		} else if (c&0200 && (c &= 0177) >='1' && c < nbra+'1') {
			sp = place(sp, braslist[c-'1'], braelist[c-'1']);
			continue;
		}
		*sp++ = c&0177;
		if (sp >= &genbuf[LBSIZE])
			error(Q);
	}
	lp = loc2;
	loc2 = sp - genbuf + linebuf;
	while (*sp++ = *lp++)
		if (sp >= &genbuf[LBSIZE])
			error(Q);
	lp = linebuf;
	sp = genbuf;
	while (*lp++ = *sp++)
		;
}

char *
place(sp, l1, l2)
register char *sp, *l1, *l2;
{

	while (l1 < l2) {
		*sp++ = *l1++;
		if (sp >= &genbuf[LBSIZE])
			error(Q);
	}
	return(sp);
}

move(cflag)
{
	register unsigned *adt, *ad1, *ad2;
	int getcopy();

	nonzero();
	if ((adt = address())==0)	/* address() guarantees addr is in range */
		error(Q);
	newline();
	if (cflag) {
		register unsigned *ozero;
		int delta;

		ad1 = dol;
		ozero = zero;
		append(getcopy, ad1++);
		ad2 = dol;
		delta = zero - ozero;
		ad1 += delta;
		adt += delta;
	} else {
		ad2 = addr2;
		for (ad1 = addr1; ad1 <= ad2;)
			*ad1++ &= ~01;
		ad1 = addr1;
	}
	ad2++;
	if (adt<ad1) {
		dot = adt + (ad2-ad1);
		if ((++adt)==ad1)
			return;
		reverse(adt, ad1);
		reverse(ad1, ad2);
		reverse(adt, ad2);
	} else if (adt >= ad2) {
		dot = adt++;
		reverse(ad1, ad2);
		reverse(ad2, adt);
		reverse(ad1, adt);
	} else
		error(Q);
	fchange = 1;
}

reverse(a1, a2)
register unsigned *a1, *a2;
{
	register int t;

	for (;;) {
		t = *--a2;
		if (a2 <= a1)
			return;
		*a2 = *a1;
		*a1++ = t;
	}
}

getcopy()
{
	if (addr1 > addr2)
		return(EOF);
	getline(*addr1++);
	return(0);
}

compile(eof)
register eof;
{
	register c;
	register char *ep;
	char *lastep;
	char bracket[NBRA], *bracketp;
	int cclcnt;

	ep = expbuf;
	bracketp = bracket;
	if ((c = getchr()) == '\n') {
		peekc = c;
		c = eof;
	}
	if (c == eof) {
		if (*ep==0)
			error(Q);
		return;
	}
	nbra = 0;
	if (c=='^') {
		c = getchr();
		*ep++ = CCIRC;
	}
	peekc = c;
	lastep = 0;
	for (;;) {
		if (ep >= &expbuf[ESIZE])
			goto cerror;
		c = getchr();
		if (c == '\n') {
			peekc = c;
			c = eof;
		}
		if (c==eof) {
			if (bracketp != bracket)
				goto cerror;
			*ep++ = CEOF;
			return;
		}
		if (c!='*')
			lastep = ep;
		switch (c) {

		case '\\':
			if ((c = getchr())=='(') {
				if (nbra >= NBRA)
					goto cerror;
				*bracketp++ = nbra;
				*ep++ = CBRA;
				*ep++ = nbra++;
				continue;
			}
			if (c == ')') {
				if (bracketp <= bracket)
					goto cerror;
				*ep++ = CKET;
				*ep++ = *--bracketp;
				continue;
			}
			if (c>='1' && c<'1'+NBRA) {
				*ep++ = CBACK;
				*ep++ = c-'1';
				continue;
			}
			*ep++ = CCHR;
			if (c=='\n')
				goto cerror;
			*ep++ = c;
			continue;

		case '.':
			*ep++ = CDOT;
			continue;

		case '\n':
			goto cerror;

		case '*':
			if (lastep==0 || *lastep==CBRA || *lastep==CKET)
				goto defchar;
			*lastep |= STAR;
			continue;

		case '$':
			if ((peekc=getchr()) != eof && peekc!='\n')
				goto defchar;
			*ep++ = CDOL;
			continue;

		case '[':
			*ep++ = CCL;
			*ep++ = 0;
			cclcnt = 1;
			if ((c=getchr()) == '^') {
				c = getchr();
				ep[-2] = NCCL;
			}
			do {
				if (c=='\n')
					goto cerror;
				if (c=='-' && ep[-1]!=0) {
					if ((c=getchr())==']') {
						*ep++ = '-';
						cclcnt++;
						break;
					}
					while (ep[-1]<c) {
						*ep = ep[-1]+1;
						ep++;
						cclcnt++;
						if (ep>=&expbuf[ESIZE])
							goto cerror;
					}
				}
				*ep++ = c;
				cclcnt++;
				if (ep >= &expbuf[ESIZE])
					goto cerror;
			} while ((c = getchr()) != ']');
			lastep[1] = cclcnt;
			continue;

		defchar:
		default:
			*ep++ = CCHR;
			*ep++ = c;
		}
	}
   cerror:
	expbuf[0] = 0;
	nbra = 0;
	error(Q);
}

execute(addr)
unsigned register *addr;
{
	register char *p1, *p2;
	register c;

	for (c=0; c<NBRA; c++) {
		braslist[c] = 0;
		braelist[c] = 0;
	}
	p2 = expbuf;
	if (addr == (unsigned *)0) {
		if (*p2==CCIRC)
			return(0);
		p1 = loc2;
	} else if (addr==zero)
		return(0);
	else
		p1 = getline(*addr);
	if (*p2==CCIRC) {
		loc1 = p1;
		return(advance(p1, p2+1));
	}
	/* fast check for first character */
	if (*p2==CCHR) {
		c = p2[1];
		do {
			if (*p1!=c)
				continue;
			if (advance(p1, p2)) {
				loc1 = p1;
				return(1);
			}
		} while (*p1++);
		return(0);
	}
	/* regular algorithm */
	do {
		if (advance(p1, p2)) {
			loc1 = p1;
			return(1);
		}
	} while (*p1++);
	return(0);
}

advance(lp, ep)
register char *ep, *lp;
{
	register char *curlp;
	int i;

	for (;;) switch (*ep++) {

	case CCHR:
		if (*ep++ == *lp++)
			continue;
		return(0);

	case CDOT:
		if (*lp++)
			continue;
		return(0);

	case CDOL:
		if (*lp==0)
			continue;
		return(0);

	case CEOF:
		loc2 = lp;
		return(1);

	case CCL:
		if (cclass(ep, *lp++, 1)) {
			ep += *ep;
			continue;
		}
		return(0);

	case NCCL:
		if (cclass(ep, *lp++, 0)) {
			ep += *ep;
			continue;
		}
		return(0);

	case CBRA:
		braslist[*ep++] = lp;
		continue;

	case CKET:
		braelist[*ep++] = lp;
		continue;

	case CBACK:
		if (braelist[i = *ep++]==0)
			error(Q);
		if (backref(i, lp)) {
			lp += braelist[i] - braslist[i];
			continue;
		}
		return(0);

	case CBACK|STAR:
		if (braelist[i = *ep++] == 0)
			error(Q);
		curlp = lp;
		if (braelist[i] - braslist[i] == 0)
			continue;
		while (backref(i, lp))
			lp += braelist[i] - braslist[i];
		while (lp >= curlp) {
			if (advance(lp, ep))
				return(1);
			lp -= braelist[i] - braslist[i];
		}
		continue;

	case CDOT|STAR:
		curlp = lp;
		while (*lp++)
			;
		goto star;

	case CCHR|STAR:
		curlp = lp;
		while (*lp++ == *ep)
			;
		ep++;
		goto star;

	case CCL|STAR:
	case NCCL|STAR:
		curlp = lp;
		while (cclass(ep, *lp++, ep[-1]==(CCL|STAR)))
			;
		ep += *ep;
		goto star;

	star:
		do {
			lp--;
			if (advance(lp, ep))
				return(1);
		} while (lp > curlp);
		return(0);

	default:
		error(Q);
	}
}

backref(i, lp)
register i;
register char *lp;
{
	register char *bp;

	bp = braslist[i];
	while (*bp++ == *lp++)
		if (bp >= braelist[i])
			return(1);
	return(0);
}

cclass(set, c, af)
register char *set;
register c;
{
	register n;

	if (c==0)
		return(0);
	n = *set++;
	while (--n)
		if (*set++ == c)
			return(af);
	return(!af);
}

putd()
{
	register r;

	r = count%10;
	count /= 10;
	if (count)
		putd();
	putchr(r + '0');
}

puts(sp)
register char *sp;
{
	col = 0;
	while (*sp)
		putchr(*sp++);
	putchr('\n');
}

char	line[70];
char	*linp	= line;

putchr(ac)
{
	register char *lp;
	register c;

	lp = linp;
	c = ac;
	if (listf) {
		if (c=='\n') {
			if (linp!=line && linp[-1]==' ') {
				*lp++ = '\\';
				*lp++ = 'n';
			}
		} else {
			if (col > (72-4-2)) {
				col = 8;
				*lp++ = '\\';
				*lp++ = '\n';
				*lp++ = '\t';
			}
			col++;
			if (c=='\b' || c=='\t' || c=='\\') {
				*lp++ = '\\';
				if (c=='\b')
					c = 'b';
				else if (c=='\t')
					c = 't';
				col++;
			} else if (c<' ' || c=='\177') {
				*lp++ = '\\';
				*lp++ =  (c>>6)    +'0';
				*lp++ = ((c>>3)&07)+'0';
				c     = ( c    &07)+'0';
				col += 3;
			}
		}
	}
	*lp++ = c;
	if(c == '\n' || lp >= &line[64]) {
		linp = line;
		write(oflag?2:1, line, lp-line);
		return;
	}
	linp = lp;
}
