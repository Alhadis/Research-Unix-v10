/*
 * getty -- adapt to terminal speed on dialup, and call login
 */

#include <sys/ttyio.h>
#include <signal.h>
#include <utsname.h>

#define ERASE	'\b'
#define KILL	'@'

#define	CEOT	004
#define	CKILL	'@'
#define	CQUIT	034		/* FS, cntl shift L */
#define	CINTR	0177		/* DEL */
#define	CSTOP	023		/* Stop output: ctl-s */
#define	CSTART	021		/* Start output: ctl-q */
#define	CBRK	0377

struct sgttyb tmode;
struct ttydevb tspeed;

struct	tab {
	char	tname;		/* this table name */
	char	nname;		/* successor table name */
	int	iflags;		/* initial flags */
	int	fflags;		/* final flags */
	int	ispeed;		/* input speed */
	int	ospeed;		/* output speed */
	char	*message;	/* login message */
} itab[] = {

/* table '0'-1-2-3 300,1200,150,110 */

	'0', 1,
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD+CR2,
	B300, B300,
	"login: ",

	1, 2,
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD+FF1,
	B1200, B1200,
	"login: ",

	2, 3,
	ANYP+RAW+NL1+CR1, EVENP+ECHO+FF1+CR2+TAB1+NL1,
	B150, B150,
	"login: ",

	3, '0',
	ANYP+RAW+NL1+CR1, ANYP+ECHO+CRMOD+XTABS+LCASE+CR1,
	B110, B110,
	"login: ",

/* table '2' -- 9600 */
	'2', '2',
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD,
	B9600, B9600,
	"login: ",

/* table '3'-'5' -- 1200,300 */
	'3', '5',
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD+FF1,
	B1200, B1200,
	"login: ",

/* table '5'-'3' -- 300,1200 */
	'5', '3',
	ANYP+RAW+NL1+CR1, ANYP+ECHO+CR1,
	B300, B300,
	"login: ",

/* table '4' -- Console Decwriter */
	'4', '4',
	ANYP+CRMOD+CBREAK, ANYP+ECHO+CRMOD+XTABS,
	B300, B300,
	"login: ",

/* table '6' -- 2400  */
	'6', '6' ,
	ANYP+RAW , ANYP+ECHO+CRMOD ,
	B2400 , B2400 ,
	"login: ",
 
/* table '7' - - 4800 */
	'7' , '7' ,
	ANYP+RAW , ANYP+ECHO+CRMOD ,
	B4800 , B4800 ,
	"login: " ,

/* table '8'-10-'9' - - 9600 - 1200 - 300 */
	'8', 10,
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD,
	B9600, B9600,
	"login: ",

	10, '9',
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD+FF1,
	B1200, B1200,
	"login: ",

	'9', '8',
	ANYP+RAW+NL1+CR1, ANYP+ECHO+CR1,
	B300, B300,
	"login: ",

/* table 'j' -- 19200 */
	'j', 'j',
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD,
	EXTA, EXTA,
	"login: ",

/* table 'a'-'b' - - 2400 - 1200 */
	'a', 'b',
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD,
	B2400, B2400,
	"login: ",

	'b', 'a',
	ANYP+RAW+NL1+CR1, ANYP+XTABS+ECHO+CRMOD,
	B1200, B1200,
	"login: ",
};


#define	NITAB	sizeof itab/sizeof itab[0]
#define	EOT	04		/* EOT char */

char	name[16];
int	crmod;
int	upper;
int	lower;

char partab[] = {
	0001,0201,0201,0001,0201,0001,0001,0201,
	0202,0004,0003,0205,0005,0206,0201,0001,
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

main(argc, argv)
char **argv;
{
	register struct tab *tabp;
	char tname;
	struct tchars tc;
	struct utsname myname;

	tname = '0';
	if (argc > 1)
		tname = argv[1][0];
	for (;;) {
		for(tabp = itab; tabp < &itab[NITAB]; tabp++)
			if(tabp->tname == tname)
				break;
		if(tabp >= &itab[NITAB])
			tabp = itab;
		tmode.sg_ispeed = tabp->ispeed;
		tmode.sg_ospeed = tabp->ospeed;
		tmode.sg_flags = tabp->iflags;
		tspeed.ispeed = tabp->ispeed;
		tspeed.ospeed = tabp->ospeed;
		tspeed.flags = tabp->iflags & ANYP;
		ioctl(0, TIOCSETP, &tmode);
		ioctl(0, TIOCSDEV, &tspeed);
		tc.t_intrc = CINTR;
		tc.t_quitc = CQUIT;
		tc.t_stopc = CSTOP;
		tc.t_startc = CSTART;
		tc.t_brkc = CBRK;
		tc.t_eofc = CEOT;
		ioctl(0, TIOCSETC, &tc);
		ioctl(0, TIOCNXCL, 0);
		if (tmode.sg_ospeed > B1200)
			puts("\n\r");
		else
			puts("\n\r\r\r\r\r");
		uname(&myname);
		puts(myname.nodename);
		puts("\n\r\r\r");
		puts(tabp->message);
		if(getname()) {
			if (upper == 0 && lower == 0)
				continue;
			tmode.sg_erase = ERASE;
			tmode.sg_kill = KILL;
			tmode.sg_flags = tabp->fflags;
			if(crmod)
				tmode.sg_flags |= CRMOD;
			if(upper)
				tmode.sg_flags |= LCASE;
			if(lower)
				tmode.sg_flags &= ~LCASE;
			ioctl(0, TIOCSETP, &tmode);
			putchr('\n');
			execl("/etc/login", "login", name, 0);
			exit(1);
		}
		tname = tabp->nname;
	}
}

getname()
{
	register char *np;
	register c;
	char cs;

	crmod = 0;
	upper = 0;
	lower = 0;
	np = name;
	for (;;) {
		if (read(0, &cs, 1) <= 0)
			exit(0);
		if ((c = cs&0177) == 0)
			return(0);
		if (c==EOT)
			exit(1);
		if (c=='\r' || c=='\n' || np >= &name[16])
			break;
		putchr(cs);
		if (c>='a' && c <='z')
			lower++;
		else if (c>='A' && c<='Z') {
			upper++;
			c += 'a'-'A';
		} else if (c==ERASE) {
			if (np > name)
				np--;
			continue;
		} else if (c==KILL) {
			putchr('\r');
			putchr('\n');
			np = name;
			continue;
		}
		*np++ = c;
	}
	*np = 0;
	if (c == '\r')
		crmod++;
	return(1);
}

puts(as)
char *as;
{
	register char *s;

	s = as;
	while (*s)
		putchr(*s++);
}

putchr(cc)
{
	char c;
	c = cc;
	c |= partab[c&0177] & 0200;
	write(1, &c, 1);
}
