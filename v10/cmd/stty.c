#include <stdio.h>
#include <sys/ttyio.h>
#include <sys/nttyio.h>
#include <sys/filio.h>

#define	CTRL(x)		(x&037)

extern int	errno, ntty_ld, tty_ld;
#define	NOLD	(-1)
int ttyfd;

struct {
	char	*s_name;
	int	s_define;
	int	s_speed;
} speeds[] = {
	"0",	B0,	0,
	"50",	B50,	50,
	"75",	B75,	75,
	"110",	B110,	110,
	"134",	B134,	134,
	"150",	B150,	150,
	"200",	B200,	200,
	"300",	B300,	300,
	"600",	B600,	600,
	"1200",	B1200,	1200,
	"1800",	B1800,	1800,
	"2400",	B2400,	2400,
	"4800",	B4800,	4800,
	"9600",	B9600,	9600,
	"exta",	EXTA,	19200,
	"extb",	EXTB,	0,
	"19200",	EXTA,	19200,		/* must follow extb */
	0,
};

#define	SGTTY	0
#define	TTYDEV	1
#define	NTTY	2

struct {
	char	*string;
	int	set;
	int	reset;
	int	where;
} modes[] = {
	{ "raw",	RAW,	0,	SGTTY},
	{ "-raw",	0,	RAW,	SGTTY},	
	{ "cooked",	0,	RAW,	SGTTY},	
	{ "-nl",	CRMOD,	0,	SGTTY},
	{ "nl",		0,	CRMOD,	SGTTY},	
	{ "echo",	ECHO,	0,	SGTTY},
	{ "-echo",	0,	ECHO,	SGTTY},	
	{ "LCASE",	LCASE,	0,	SGTTY},
	{ "lcase",	LCASE,	0,	SGTTY},
	{ "-LCASE",	0,	LCASE,	SGTTY},	
	{ "-lcase",	0,	LCASE,	SGTTY},	
	{ "-tabs",	XTABS,	0,	SGTTY},
	{ "tabs",	0,	XTABS,	SGTTY},	
	{ "tandem",	TANDEM,	0,	SGTTY},
	{ "-tandem",	0,	TANDEM,	SGTTY},	
	{ "cbreak",	CBREAK,	0,	SGTTY},
	{ "-cbreak",	0,	CBREAK,	SGTTY},	
	{ "cr0",	CR0,	CR3,	SGTTY},	
	{ "cr1",	CR1,	CR3,	SGTTY},	
	{ "cr2",	CR2,	CR3,	SGTTY},	
	{ "cr3",	CR3,	CR3,	SGTTY},	
	{ "tab0",	TAB0,	XTABS,	SGTTY},	
	{ "tab1",	TAB1,	XTABS,	SGTTY},	
	{ "tab2",	TAB2,	XTABS,	SGTTY},	
	{ "nl0",	NL0,	NL3,	SGTTY},	
	{ "nl1",	NL1,	NL3,	SGTTY},	
	{ "nl2",	NL2,	NL3,	SGTTY},	
	{ "nl3",	NL3,	NL3,	SGTTY},	
	{ "ff0",	FF0,	FF1,	SGTTY},	
	{ "ff1",	FF1,	FF1,	SGTTY},	
	{ "bs0",	BS0,	BS1,	SGTTY},	
	{ "bs1",	BS1,	BS1,	SGTTY},
	{ "8bit",	F8BIT,	0,	TTYDEV},
	{ "-8bit",	0,	F8BIT,	TTYDEV},
	{ "even",	EVENP,	0,	TTYDEV},
	{ "-even",	0,	EVENP,	TTYDEV},
	{ "odd",	ODDP,	0,	TTYDEV},
	{ "-odd",	0,	ODDP,	TTYDEV},
	{ "crtbs",	LCRTBS,		LPRTERA,	NTTY},
	{ "-crtbs",	0,		LCRTBS, 	NTTY},
	{ "prterase",	LPRTERA,	LCRTBS+LCRTKIL+LCRTERA,	NTTY},
	{ "-prterase",	0,		LPRTERA,	NTTY},
	{ "crterase",	LCRTERA,	LPRTERA,	NTTY},
	{ "-crterase",	0,		LCRTERA,	NTTY},
	{ "crtkill",	LCRTKIL,	LPRTERA,	NTTY},
	{ "-crtkill",	0,		LCRTKIL,	NTTY},
	{ "ctlecho",	LCTLECH,	0,		NTTY},
	{ "-ctlecho",	0,		LCTLECH,	NTTY},
	{ "tilde",	LTILDE,		0,		NTTY},
	{ "-tilde",	0,		LTILDE,		NTTY},
	{ "litout",	LLITOUT,	0,		NTTY},
	{ "-litout",	0,		LLITOUT,	NTTY},
	{ "flusho",	LFLUSHO,	0,		NTTY},
	{ "-flusho",	0,		LFLUSHO,	NTTY},
	{ "nohang",	LNOHANG,	0,		NTTY},
	{ "-nohang",	0,		LNOHANG,	NTTY},
	{ "decctq",	LDECCTQ,	0,		NTTY},
	{ "-decctq",	0,		LDECCTQ,	NTTY},
	0,
};

struct sgttyb	mode;
struct ttydevb	speed;
struct tchars	tc;
struct ltchars	ltc;
struct luchars	luc;

struct	special {
	char	*name;
	char	*cp;
	char	def;
} special[] = {
	"erase",	&mode.sg_erase,		CTRL('h'),
	"kill",		&mode.sg_kill,		'@',
	"intr",		&tc.t_intrc,		0177,
	"quit",		&tc.t_quitc,		CTRL('\\'),
	"start",	&tc.t_startc,		CTRL('q'),
	"stop",		&tc.t_stopc,		CTRL('s'),
	"eof",		&tc.t_eofc,		CTRL('d'),
	"brk",		&tc.t_brkc,		0377,

	"susp",		&ltc.t_suspc,		CTRL('z'),
	"dsusp",	&ltc.t_dsuspc,		CTRL('y'),
	"rprnt",	&ltc.t_rprntc,		CTRL('r'),
	"flush",	&ltc.t_flushc,		CTRL('o'),
	"werase",	&ltc.t_werasc,		CTRL('w'),
	"lnext",	&ltc.t_lnextc,		CTRL('v'),

	"undo",		&luc.t_undoc,		0377,
	"urot",		&luc.t_urotc,		0377,
	0
};

char	*ego;
int	ldisc;		/* current line disc */
int	lmode;		/* new tty local mode bits */

main (argc, argv)
	int	argc;
	char	*argv[];
{
	char	*arg, obuf[BUFSIZ];
	int 	i;
	struct special *sp;

	ego = argv[0];
	setbuf(stdout, obuf);
	if ((ttyfd = open("/dev/tty", 1)) < 0) {
		fprintf(stderr, "%s: can't open /dev/tty\n", ego);
		exit(1);
	}
	(void) getmodes();
	if (argc < 2) {
		prmodes();
		exit (0);
	}
	for (arg = *++argv; --argc > 0; arg = *++argv) {

#define eq(x)	(strcmp(arg, x)==0)

		if (eq ("all")) {
			prmodes();
			continue;
		}
		if (eq ("old") || eq ("-new")) {
			if (swdisc(tty_ld, 0))
				goto done1;
			continue;
		}
		if (eq ("new")) {
			if (swdisc(ntty_ld, 0))
				goto done1;
			continue;
		}
		if (eq("old!") || eq("tty!")) {
			if (swdisc(tty_ld, 1))
				goto done1;
			continue;
		}
		if (eq("notty")) {
			if (swdisc(NOLD, 0))
				goto done1;
			continue;
		}
		if (eq ("crt")) {
			lmode |= LCRTBS | LCTLECH;
			if (speeds[mode.sg_ispeed].s_speed >= 1200)
				lmode |= LCRTERA | LCRTKIL;
			continue;
		}
		if (eq ("ek")) {
			mode.sg_erase = '#';
			mode.sg_kill = '@';
			continue;
		}
		if (eq ("hup")) {
			(void) ioctl (ttyfd, TIOCHPCL, NULL);
			continue;
		}
		for (sp = special; sp->name; sp++)
			if (eq (sp->name)) {
				if (--argc > 0) {
					arg = *++argv;
					if (*arg == 'u')
						*sp->cp = 0377;
					else if (*arg == '^')
						*sp->cp = (arg[1] == '?') ?
						    0177 : arg[1] & 037;
					else	*sp->cp = *arg;
					goto cont;
				}
				fprintf(stderr, "%s: missing %s character\n",
				    ego, arg);
				goto done1;
			}
		for (i = 0; speeds[i].s_name; i++)
			if (eq (speeds[i].s_name)) {
				mode.sg_ispeed = mode.sg_ospeed = 
				    speeds[i].s_define;	/* temp compat */
				speed.ispeed = speed.ospeed = speeds[i].s_define;
				goto cont;
			}
		for (i = 0; modes[i].string; i++)
			if (eq (modes[i].string))
			switch (modes[i].where) {
			case SGTTY:
				mode.sg_flags &=~ modes[i].reset;
				mode.sg_flags |= modes[i].set;
				goto cont;

			case TTYDEV:
				speed.flags &=~ modes[i].reset;
				speed.flags |= modes[i].set;
				goto cont;

			case NTTY:
				lmode &=~ modes[i].reset;
				lmode |= modes[i].set;
				goto cont;
			}
		fprintf(stderr, "%s: %s: unknown mode\n", ego, arg);
done1:
		setmodes();
		exit(1);
cont:
		;
	}
	setmodes();
	exit(0);
}

getmodes()
{
	int ret;
	ldisc = ioctl (ttyfd, FIOLOOKLD, 0);
	ret = ioctl (ttyfd, TIOCGETP, &mode) == -1;
	if (ioctl(ttyfd, TIOCGDEV, &speed) < 0) {
		speed.ispeed = mode.sg_ispeed;
		speed.ospeed = mode.sg_ospeed;
		speed.flags = mode.sg_flags & (EVENP|ODDP);
	}
	(void) ioctl (ttyfd, TIOCGETC, &tc);
	if (ldisc == ntty_ld) {
		if (ioctl(ttyfd, TIOCLGET, &lmode) < 0)
			perror("TIOCLGET");
		if (ioctl(ttyfd, TIOCGLTC, &ltc) < 0)
			perror("TIOCGLTC");
	}
	return ret;
}

pit (what, itsname, sep)
	unsigned char	what;
	char 	*itsname, *sep;
{
	printf("%s", itsname);
	if (what == 0377) {
		printf(" <undef>%s", sep);
		return;
	}
	printf(" = ");
	if (what & 0200) {
		printf("M-");
		what &= ~ 0200;
	}
	if (what == 0177) {
		printf("^");
		what = '?';
	}
	else if (what < ' ') {
		printf("^");
		what += '@';
	}
	printf("%c%s", what, sep);
	return;
}


prmodes()
{
	register int	m;

	if (speed.ispeed != speed.ospeed)
		printf("input speed: %d baud, output speed: %d baud\n",
		    speeds[speed.ispeed].s_speed,
		    speeds[speed.ospeed].s_speed);
	else
		printf("speed: %d baud\n",
		       speeds[speed.ispeed].s_speed);
	pit (mode.sg_erase, "erase", "; ");
	pit (mode.sg_kill, "kill", "; ");
	pit (tc.t_intrc, "intr", "; ");
	pit (tc.t_quitc, "quit", "\n");
	pit (tc.t_startc, "start", "; ");
	pit (tc.t_stopc, "stop", "; ");
	pit (tc.t_eofc, "eof", "; ");
	pit (tc.t_brkc, "brk", "\n");

	if (ldisc == ntty_ld) {
		pit (ltc.t_werasc, "werase", "; ");
		pit (ltc.t_rprntc, "rprnt", "; ");
		pit (ltc.t_flushc, "flush", "; ");
		pit (ltc.t_lnextc, "lnext", "\n");
		pit (ltc.t_suspc, "susp", "; ");
		pit (ltc.t_dsuspc, "dsusp", "; ");
		pit (luc.t_undoc, "undo", "; ");
		pit (luc.t_urotc, "urot", "\n");
	}
	if (ldisc == tty_ld)
		printf("old ");
	else if (ldisc == ntty_ld)
		printf("new ");
	if (speed.flags & EVENP)
		printf("even ");
	if (speed.flags & ODDP)
		printf("odd ");
	if (speed.flags & F8BIT)
		printf("8bit ");
	m = mode.sg_flags;
#define	mpit(what,str)	printf(str + ((m & what) != 0))
	mpit (RAW, "-raw ");
	printf("-nl " + ((m & CRMOD) == 0));
	mpit (ECHO, "-echo ");
	mpit (LCASE, "-lcase ");
	printf("-tabs " + ((m & XTABS) != XTABS));
	mpit (CBREAK, "-cbreak ");
	mpit (TANDEM, "-tandem ");
#define delay(x,y)	if (x) printf("%s%d ", y, x)
	delay ((m & NLDELAY) / NL1, "nl");
	if ((m & TBDELAY) != XTABS)
		delay ((m & TBDELAY) / TAB1, "tab");
	delay ((m & CRDELAY) / CR1, "cr");
	delay ((m & VTDELAY) / FF1, "ff");
	delay ((m & BSDELAY) / BS1, "bs");
	printf("\n");

	if (ldisc == ntty_ld) {
		m = lmode;
		mpit (LCRTBS, "-crtbs ");
		mpit (LCRTERA, "-crterase ");
		mpit (LCRTKIL, "-crtkill ");
		mpit (LCTLECH, "-ctlecho ");
		mpit (LPRTERA, "-prterase ");
		printf("\n");

		mpit (LTILDE, "-tilde ");
		mpit (LFLUSHO, "-flusho ");
		mpit (LLITOUT, "-litout ");
		mpit (LNOHANG, "-nohang ");
		mpit (LDECCTQ, "-decctq ");
		printf("\n");
	}
}

/*
 * it's wrong to ignore returns, but as long as we ignore
 * them when we get modes, it is less surprising and leads
 * to fewer bugs, e.g. when no tty driver is present
 */
setmodes()
{
	(void)ioctl(ttyfd, TIOCSETN, &mode);
	(void)ioctl(ttyfd, TIOCSDEV, &speed);
	(void)ioctl(ttyfd, TIOCSETC, &tc);
	if (ldisc == ntty_ld) {
		if (ioctl(ttyfd, TIOCSLTC, &ltc) < 0)
			perror("TIOCSLTC");
		if (ioctl(ttyfd, TIOCLSET, &lmode) < 0)
			perror("TIOCLSET");
	}
}

int
swdisc(newld, force)
	int	newld, force;
{
	int curld = ioctl(ttyfd, FIOLOOKLD, 0);

	if (force == 0 && curld != tty_ld && curld != ntty_ld)
		return (0);	/* silly but less confusing */
	if (curld == tty_ld || curld == ntty_ld || curld == newld) {
		if (ioctl(ttyfd, FIOPOPLD, 0) < 0) {
			fprintf(stderr, "%s: can't pop line discipline\n", ego);
			return (-1);
		}
	}
	if (newld == NOLD)
		return (0);
	if (ioctl(ttyfd, FIOPUSHLD, &newld) < 0) {
		if (ioctl(ttyfd, FIOPUSHLD, &ldisc) < 0)
			perror("FIOPUSHLD");
		fprintf(stderr, "%s: can't push line discipline\n", ego);
		return (-1);
	}
	/* what follows is a bit silly */
	if (ioctl(ttyfd, TIOCSETN, &mode) < 0)
		perror("TIOCSETN");
	if (ioctl(ttyfd, TIOCSETC, &tc) < 0)
		perror("TIOCSETC");
	getmodes();
	return (0);
}
