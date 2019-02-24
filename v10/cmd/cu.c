#include <stdio.h>
#include <signal.h>
#include <sys/ttyio.h>
#include <sys/filio.h>
#include <ctype.h>
/*
 *	cu telno [ class ]
 *	Escape with `~' at beginning of line.
 *	Ordinary diversions are ~<, ~> and ~>>.
 *	Silent output diversions are ~>: and ~>>:.
 *	Terminate output diversion with ~> alone.
 *	Output command requests are ~! and ~:! (silent).
 *	Quit is ~. and ~! gives local command or shell.
 *	Also ~$ for canned procedure pumping remote.
 *	~%put from [to]  and  ~%take from [to] invoke builtins
 */

char	CRLF[2] = {'\r', '\n'};
#define	equal(s1,s2)	(strcmp(s1, s2)==0)
char	*cunfile;
int	ln;	/* fd for comm line */
char	tkill, terase;	/* current input kill & erase */
char	c;
int	intr;
int	nhup;
int	nflag;
int	tandm;
int	hduplx;
int	errflg;
int	speed = B9600;	/* used only for direct */
int	parity = 0;
int	chan[2];	/* interprocess channel */
extern	int optind, opterr;
extern	char *optarg;
struct	sgttyb	realtty;
struct	tchars	realtch;
int	sig2();

char	*connmsg[] = {
	"",
	"ACU busy",
	"call dropped",
	"no carrier",
	"can't fork",
	"acu access",
	"tty access",
	"tty hung",
	"usage: cu [-hnt] telno [ class ]",
	"unknown service class",
	"stuff dk error message here",	/* hack */
};
extern char *errstr;

struct dial {
	char	*telno;
	char	*dialtype;
	char	*comment;
};

char partab[] = {
	0001,0201,0201,0001,0201,0001,0001,0201,
	0202,0004,0003,0201,0005,0206,0201,0001,
	0201,0001,0001,0201,0001,0201,0201,0001,
	0001,0201,0201,0001,0201,0001,0001,0201,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0201
};

/*
 *	spawn child to invoke rd to read from line, output to fd 1
 *	main line invokes wr to read tty, write to line
 */

main(ac, av)
char *av[];
{
	int fk;
	struct sgttyb stbuf;
	struct ttydevb tdbuf;
	struct dial d;

	signal(SIGPIPE, SIG_IGN);
	d.telno = NULL;
	d.dialtype = NULL;
	d.comment = "";
	options(ac, av);
	if (optind >= ac || errflg) {
		prf(connmsg[8]);
		exit(8);
	}
	ioctl(0, TIOCGETP, &realtty);
	ioctl(0, TIOCGETC, &realtch);
	gettelno(av[optind], &d);
	if (optind+1 < ac)
		d.dialtype = av[optind+1];
	if (d.dialtype==NULL || *d.dialtype=='\0')
		d.dialtype = "1200";
	if (nflag) {
		printf("%s %s %s\n", d.telno, d.dialtype, d.comment);
		exit(0);
	}
	if (equal(d.dialtype, "direct")) {
		ln = open(d.telno, 2);
		if (ln>=0) {
			struct tchars tcr;

			ioctl(ln, TIOCGETP, &stbuf);
			stbuf.sg_flags &= ~ECHO;
			stbuf.sg_flags |= RAW|EVENP|ODDP;
			stbuf.sg_ispeed = speed;	/* obsolete */
			stbuf.sg_ospeed = speed;	/* obsolete */
			ioctl(ln, TIOCGETC, &tcr);
			tcr.t_stopc = '\027';
			tcr.t_startc = '\031';
			ioctl(ln, TIOCGDEV, &tdbuf);
			tdbuf.ispeed = tdbuf.ospeed = speed;
			tdbuf.flags |= F8BIT|EVENP|ODDP;
			ioctl(ln, TIOCSETP, &stbuf);
			ioctl(ln, TIOCSETC, &tcr);
			ioctl(ln, TIOCSDEV, &tdbuf);
			ioctl(ln, TIOCHPCL, 0);
			ioctl(ln, TIOCEXCL, 0);
		}
	} else{
		ln = dialout(d.telno, d.dialtype);
		if(ln==-1 && errstr){
			ln=-10;
			connmsg[10]=errstr;
		}
	}
	if (ln < 0) {
		prf("Connect failed: %s",connmsg[-ln]);
		exit(-ln);
	}
	ioctl(ln, TIOCGETP, &stbuf);
	prf("Connected");
	if (d.comment && *d.comment && *d.comment!='\n')
		prf(d.comment);
	if (tandm) {
		ioctl(ln, TIOCGETP, &stbuf);
		stbuf.sg_flags = ODDP+EVENP+TANDEM+CBREAK;
		ioctl(ln, TIOCSETN, &stbuf);
	}
	if(pipe(chan)<0) {
		prf("no pipe");
		exit(1);
	}
	fk = fork();
	nhup = (int)signal(SIGINT, SIG_IGN);
	if (fk == 0) {
		close(chan[1]);
		rd();
		prf("\007Lost carrier");
		exit(3);
	}
	close(chan[0]);
	mode(1);
	wr();
	mode(0);
	kill(fk, SIGKILL);
	stbuf.sg_ispeed = 0;
	stbuf.sg_ospeed = 0;
	ioctl(ln, TIOCSETN, &stbuf);
	tdbuf.ispeed = tdbuf.ospeed = 0;
	ioctl(ln, TIOCSDEV, &tdbuf);
	prf("Disconnected");
	exit(0);
}

/*
 *	wr: write to remote: 0 -> line.
 *	~.	terminate
 *	~<file	send file
 *	~!	local login-style shell
 *	~!cmd	execute cmd locally
 *	~$proc	execute proc locally, send output to line
 *	~%cmd	execute builtin cmd (put and take)
 */

wr()
{
	int ds, fk, lcl, x, nc;
	char *p, b[600];

	for (;;) {
		p = b;
		while (rdc(0) >= 1) {
			if (p == b)
				lcl=(c == '~');
			if (p == b+1 && b[0] == '~')
				lcl=(c!='~');
			if (c == 0)
				c = 0177;
			if (!lcl) {
				if (c==0177)
					ioctl(ln, TIOCFLUSH, 0);
				if (wrc(ln, c, 1) <= 0) {
					prf("line gone");
					return;
				}
				if (c==0177)
					ioctl(0, TIOCFLUSH, 0);
			}
			if (lcl) {
				if (c == 0177)
					c = tkill;
				if (c == '\r' || c == '\n')
					goto A;
				if (!hduplx)
					wrc(1, c, 1);
			}
			*p++ = c;
			if (c == terase) {
				p = p-2; 
				if (p<b)
					p = b;
			}
			if (c == tkill || c == 0177 || c == '\r' || c == '\n')
				p = b;
		}
		return;
A: 
		if (!hduplx || realtty.sg_flags&CRMOD)
			echo("");
		*p = 0;
		switch (b[1]) {
		case '.':
		case '\004':
			return;
		case 'b':
			sendbreak();
			break;

		case '!':
		case '$':
			fk = fork();
			if (fk == 0) {
				close(1);
				dup(b[1] == '$'? ln:2);
				close(ln);
				mode(0);
				if (!nhup)
					signal(SIGINT, SIG_DFL);
				if (b[2] == 0)
					execl("/bin/sh","sh",0);
				else
					execl("/bin/sh","sh","-c",b+2,(char *)0);
				prf("Can't execute shell");
				exit(1);
			}
			if (fk!=(-1)) {
				while (wait((int *)0)!=fk)
					;
			}
			mode(1);
			if (b[1] == '!')
				echo("!");
			break;
		case '<':
			if (b[2] == 0) break;
			if ((ds = open(b+2,0))<0) {
				prf("Can't divert %s",b+1); 
				break;
			}
			intr = x = 0;
			mode(2);
			if (!nhup)
				signal(SIGINT, sig2);
			while (!intr && (nc = rdc(ds)) >= 1) {
				if (wrc(ln, c, nc==1) <= 0) {
					x = 1; 
					break;
				}
			}
			signal(SIGINT, SIG_IGN);
			close(ds);
			mode(1);
			if (x)
				return;
			break;
		case '%':
			dopercen(&b[2]);
			break;
		default:
			prf("Use `~~' to start line with `~'");
		}
		continue;
	}
}

dopercen(line)
register char *line;
{
	char *args[10];
	register narg, f;
	int rcount, nc;

	for (narg = 0; narg < 10;) {
		while(*line == ' ' || *line == '\t')
			line++;
		if (*line == '\0')
			break;
		args[narg++] = line;
		while(*line != '\0' && *line != ' ' && *line != '\t')
			line++;
		if (*line == '\0')
			break;
		*line++ = '\0';
	}
	if (equal(args[0], "break")) {
		sendbreak();
		return;
	} else if (equal(args[0], "take")) {
		if (narg < 2) {
			prf("usage: ~%%take from [to]");
			return;
		}
		if (narg < 3)
			args[2] = args[1];
		write(chan[1],args[2],strlen(args[2]));
		wrln("echo '~>:'");
		wrln(args[2]);
		wrln(";tee /dev/null <");
		wrln(args[1]);
		wrln(";echo '~>'\n");
		return;
	} else if (equal(args[0], "put")) {
		if (narg < 2) {
			prf("usage: ~%%put from [to]");
			return;
		}
		if (narg < 3)
			args[2] = args[1];
		if ((f = open(args[1], 0)) < 0) {
			prf("cannot open: %s", args[1]);
			return;
		}
		wrln("stty -echo;cat >");
		wrln(args[2]);
		wrln(";stty echo\n");
		sleep(5);
		intr = 0;
		if (!nhup)
			signal(SIGINT, sig2);
		mode(2);
		rcount = 0;
		while(!intr && (nc = rdc(f)) >= 1) {
			rcount++;
			if (c == tkill || c == terase)
				wrc(ln, '\\', 0);
			if (wrc(ln, c, nc == 1) <= 0)
				intr = 1;
		}
		signal(SIGINT, SIG_IGN);
		close(f);
		if (intr) {
			wrc(ln, '\n', 1);
			prf("stopped after %d bytes", rcount);
		}
		wrc(ln, '\004', 1);
		sleep(5);
		mode(1);
		return;
	}
	prf("~%%%s unknown\n", args[0]);
}

wrln(s)
register char *s;
{
	register n = strlen(s);

	write(ln, s, n);
}

/*
 *	rd: read from remote: line -> 1
 *	catch:
 *	~>[>][:][file]
 *	stuff from file...
 *	~>	(ends diversion)
 *	  ways for remote to run local command:
 *	~!command (run command locally)
 *	~:!command (run silently locally)
 */

rd()
{
	int ds, slnt, pid, hold=0, nc;
	char *p, *q, b[600];
	char name[100], len;

	p = b;
	ds = -1;
	while ((nc = rdc(ln)) >= 1) {
		if (ds < 0 && hold==0)
			slnt = 0;
		if (p==b && c=='~')
			hold= ++slnt;
		if (hold && slnt && p==b+1 && c!=':') {
			wrc(1, '~', 1);
			slnt--;
			hold = 0;
		}
		if (!slnt)
			wrc(1, c, nc==1);
		*p++ = c;
		if (c!='\n' && p < &b[599])
			continue;
		q = p; 
		p = b;
		hold = 0;
/*			impossibly insecure
		if (strncmp(b, "~:!",3)==0||strncmp(b, "~!", 2)==0) {
			*--q= '\0';
			if (*--q == '\r')
				*q= '\0';
			mode(0);
			if ((pid=fork())==0) {
				p = b+2;
				if (*p=='!')
					p++;
				execl("/bin/sh", "sh", "-c", p, (char *)0);
				exit(0);
			}
			while (wait((int *)0)!=pid)
				;
			mode(1);
			continue;
		}
*/
		if (b[0]!='~' || b[1]!='>') {
			if (*(q-2) == '\r') {
				q--; 
				*(q-1)=(*q);
			}
			if (ds>=0)
				write(ds, b, q-b);
			continue;
		}
		if (ds>=0)
			close(ds);
		if (slnt) {
			write(1, b, q - b);
			write(1, CRLF, sizeof(CRLF));
		}
		if (*(q-2) == '\r')
			q--;
		*(q-1) = 0;
		slnt = 0;
		q = b+2;
		if (*q == '>')
			q++;
		if (*q == ':') {
			slnt = 1; 
			q++;
		}
		if (*q == 0) {
			ds  = -1; 
			continue;
		}
		len = read(chan[0],name,sizeof(name));
		name[len>=0?len:0] = 0;
		if(strcmp(name,q)) {
			prf("masquerading ~%take");
			return;
		}
		if (b[2]!='>' || (ds = open(q,1))<0)
			ds = creat(q, 0644);
		lseek(ds, (long)0, 2);
		if (ds<0)
			prf("Can't divert %s",b+1);
	}
}

mode(f)
{
	struct sgttyb stbuf;
	static struct tchars nochars = { -1, -1, -1, -1, -1, -1};

	ioctl(0, TIOCGETP, &stbuf);
	tkill = stbuf.sg_kill;
	terase = stbuf.sg_erase;
	if (f == 0) {
		ioctl(0, TIOCSETP, &realtty);
		ioctl(0, TIOCSETC, &realtch);
		return;
	}
	if (f == 1) {
		stbuf.sg_flags |= CBREAK;
		stbuf.sg_flags &= ~CRMOD;
		if (!hduplx)
			stbuf.sg_flags &= ~ECHO;
		ioctl(0, TIOCSETP, &stbuf);
		ioctl(0, TIOCSETC, &nochars);
		return;
	}
	if (f == 2) {
		stbuf.sg_flags &= ~(ECHO|CRMOD);
		ioctl(0, TIOCSETP, &stbuf);
		ioctl(0, TIOCSETC, &realtch);
		return;
	}
}

echo(s)
char *s;
{
	register n = strlen(s);

	if (n>0)
		write(1, s, n);
	write(1, CRLF, sizeof(CRLF));
}

/* VARARGS1 */
prf(f, s)
char *f;
char *s;
{
	printf(f, s);
	printf(CRLF);
	fflush(stdout);
}

sendbreak()
{
	struct sgttyb b;
	int olds;

#ifdef TIOCSBRK
	ioctl(ln, TIOCSBRK, 0);
#else
	ioctl(ln, TIOCGETP, &b);
	olds = b.sg_ispeed;
	b.sg_ispeed = B50;
	b.sg_ospeed = B50;
	ioctl(ln, TIOCSETP, &b);
	write(ln, "\0\0\0", 3);
	b.sg_ispeed = olds;
	b.sg_ospeed = olds;
	ioctl(ln, TIOCSETP, &b);
#endif
}

/*
 * Symbolic phone numbers
 */
gettelno(np, dp)
char *np;
register struct dial *dp;
{
	char cunumber[128];
	char *hp;
	register char *xnp;
	char *getenv();

	if (cunfile) {
		if (look(np, dp, cunfile))
			return;
	} else {
		hp = getenv("HOME");
		if (hp) {
			strcpy(cunumber, hp);
			strcat(cunumber, "/lib/cunumber");
			if (look(np, dp, cunumber))
				return;
		}
		if (look(np, dp, "/usr/lib/cunumber"))
			return;
	}
	xnp = np;
	if (*np != '/')
		while (*xnp) {
			if (*xnp!=';' && *xnp!=':' && *xnp!='-' && *xnp!='*'
			 && *xnp!='#' && !isdigit(*xnp)) {
				prf("Symbolic number not found");
				exit(1);
			}
			xnp++;
		}
	dp->telno = np;
}

look(np, dp, fnp)
register char *np;
register struct dial *dp;
char *fnp;
{
	FILE *fp;
	static char line[128];
	register char *lp;
	register i;
	char *opts[8];
	register char **optp;
	char *w[4];

	if ((fp = fopen(fnp, "r")) == NULL)
		return(0);
	while (fgets(line, sizeof(line), fp)) {
		lp = line;
		optp = opts;
		for (i = 0; i<4; i++) {
			while (isspace(*lp))
				lp++;
			if (i==1 && *lp=='-') {
				*optp++ = lp;
				i--;
			} else
				w[i] = lp;
			while ((!isspace(*lp) || i==3) && *lp)
				lp++;
			if (*lp)
				*lp++ = '\0';
		}
		if (strcmp(w[0], np))
			continue;
		i = optind;
		optind = 0;
		options(optp-opts, opts);
		optind = i;
		dp->telno = w[1];
		dp->dialtype = w[2];
		dp->comment = w[3];
		fclose(fp);
		return(1);
	}
	fclose(fp);
	return(0);
}

options(ac, av)
char **av;
{
	register o;

	opterr = 0;
	while ((o = getopt(ac, av, "hntf:s:p:")) != EOF) {
		switch(o) {

		case '?':
			errflg++;
			continue;

		case 'h':
			hduplx++;
			continue;

		case 'f':
			cunfile = optarg;
			continue;

		case 't':
			tandm++;
			continue;

		case 'n':
			nflag++;
			continue;

		case 's':
			if ((speed = getspeed(optarg)) < 0) {
				fprintf(stderr, "-s %s: illegal speed\n");
				errflg++;
			}
			continue;

		case 'p':
			if ((parity = getpar(optarg)) < 0) {
				fprintf(stderr, "-p %s: illegal parity\n");
				errflg++;
			}
			continue;
		}
	}
}

struct speeds{
	char	*s_name;
	int	s_define;
} speeds[] = {
	"0",	B0,
	"50",	B50,
	"75",	B75,
	"110",	B110,
	"134",	B134,
	"150",	B150,
	"200",	B200,
	"300",	B300,
	"600",	B600,
	"1200",	B1200,
	"1800",	B1800,
	"2400",	B2400,
	"4800",	B4800,
	"9600",	B9600,
	"exta",	EXTA,
	"extb",	EXTB,
	"19200",	EXTA,
	0
};

getspeed(s)
char *s;
{
	register struct speeds *sp;

	for (sp = speeds; sp->s_name; sp++)
		if (strcmp(sp->s_name, s) == 0)
			return (sp->s_define);
	return (-1);
}

getpar(s)
char *s;
{
	switch (s[0]) {
	case '0':
		return (0);
	case '1':
		return (EVENP|ODDP);
	case 'e':
		return (EVENP);
	case 'o':
		return (ODDP);
	}
	return (-1);
}

wrc(f, c, flush)
register c;
{
	static char buf[64];
	static char *bp = buf;
	register r;

	c &= 0177;
	if (f==ln) {
		switch (parity) {
		case EVENP:
			c |= (partab[c] & 0200);
			break;
		case ODDP:
			c |= (partab[c] & 0200) ^ 0200;
			break;
		case EVENP|ODDP:
			c |= 0200;
			break;
		}
	}
	*bp++ = c;
	r = 1;
	if (flush || bp >= &buf[64]) {
		r = write(f, buf, bp-buf);
		bp = buf;
	}
	return(r);
}

rdc(ds)
{
	static char buf[64];
	static nc = 0;
	static char *bp;

	if (nc <= 0) {
		nc = read(ds, buf, 64); 
		bp = buf;
	}
	if (nc <= 0)
		return(nc);
	nc--;
	c = *bp++ & 0177;
	return(nc+1);
}

sig2()
{
	signal(SIGINT, SIG_IGN); 
	intr = 1;
}

