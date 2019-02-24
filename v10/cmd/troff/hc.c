/*
 *	drive hp2621 terminal 
 *	there is some bug here that blows up on vaxes.
 */

/*
output language from troff:
all numbers are character strings

sn	size in points
fn	font as number from 1-n
cx	ascii character x
Cxyz	funny char xyz. terminated by white space
Hn	go to absolute horizontal position n
Vn	go to absolute vertical position n (down is positive)
hn	go n units horizontally (relative)
vn	ditto vertically
nnc	move right nn, then print c (exactly 2 digits!)
		(this wart is an optimization that shrinks output file size
		 about 35% and run-time about 15% while preserving ascii-ness)
dt ...\n	draw operation 't':
	d/ dx dy c	line from here to dx,dy using c
	dO r		circle of radius r centered here
	dE rx ry	ellipse of semi-axes rx, ry centered here
	dA dx1 dy1 dx2 dy2	arc counter-clockwise with center here
			from dx1,dy1 to dx2,dy2.
nb a	end of line (information only -- no action needed)
	b = space before line, a = after
p	new page begins -- set v to 0
#...\n	comment
x ...\n	device control functions:
	x i	init
	x T s	name of device is s
	x r n h v	resolution is n/inch
		h = min horizontal motion, v = min vert
	x p	pause (can restart)
	x s	stop -- done for ever
	x t	generate trailer
	x f n s	font position n contains font s

	Subcommands like "i" are often spelled out like "init".
*/

#include	<stdio.h>
#include	<signal.h>
#include	<ctype.h>

#include "dev.h"
#define	NFONT	10

int 	keepon	= 0;
int	output	= 0;	/* do we do output at all? */
int	nolist	= 0;	/* output page list if > 0 */
int	olist[20];	/* pairs of page numbers */

int	erase	= 1;
float	aspect	= 1.5;	/* default aspect ratio */
int	wflag	= 0;	/* wait, looping, for new input if on */
int	(*sigint)();
int	(*sigquit)();

struct dev dev;
struct font *fontbase[NFONT];
short	psizes[]	={ 11, 16, 22, 36, 0};	/* approx sizes available */
short	*pstab		= psizes;
int	nsizes	= 1;
int	pscode[]	={ ';', ':', '9', '8'};
int	nfonts;
int	smnt;	/* index of first special font */
int	nchtab;
char	*chname;
short	*chtab;
char	*fitab[NFONT];
char	*widthtab[NFONT];	/* widtab would be a better name */
char	*codetab[NFONT];	/* device codes */

#define	FATAL	1
#define	BMASK	0377
int	dbg	= 0;
int	res	= 972;		/* input assumed computed according to this resolution */
				/* initial value to avoid 0 divide */
char	*typesetter	= "/dev/202";	/* typesetter device */
FILE	*tf	= stdout;	/* output file */
char	*fontdir	= "/usr/lib/font";
extern char devname[];

int	hscale	= 75;
int	vscale	= 100;
FILE *fp = stdin;	/* input file pointer */

main(argc, argv)
char *argv[];
{
	char buf[BUFSIZ];
	int done();
	float atof();

	setbuf(stdout, buf);
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'h':
			hscale = atoi(&argv[1][2]);
			break;
		case 'v':
			vscale = atoi(&argv[1][2]);
			break;
		case 'a':
			aspect = atof(&argv[1][2]);
			break;
		case 'e':
			erase = 0;
			break;
		case 'o':
			outlist(&argv[1][2]);
			break;
		case 'd':
			dbg = atoi(&argv[1][2]);
			if (dbg == 0) dbg = 1;
			break;
		case 'c':
			keepon = 1;
			break;
		}
		argc--;
		argv++;
	}

	if (argc <= 1)
		conv(stdin);
	else
		while (--argc > 0) {
			if (strcmp(*++argv, "-") == 0)
				fp = stdin;
			else if ((fp = fopen(*argv, "r")) == NULL)
				error(FATAL, "can't open %s", *argv);
			conv(fp);
			fclose(fp);
		}
	done();
}

outlist(s)	/* process list of page numbers to be printed */
char *s;
{
	int n1, n2, i;

	nolist = 0;
	while (*s) {
		n1 = 0;
		if (isdigit(*s))
			do
				n1 = 10 * n1 + *s++ - '0';
			while (isdigit(*s));
		else
			n1 = -9999;
		n2 = n1;
		if (*s == '-') {
			s++;
			n2 = 0;
			if (isdigit(*s))
				do
					n2 = 10 * n2 + *s++ - '0';
				while (isdigit(*s));
			else
				n2 = 9999;
		}
		olist[nolist++] = n1;
		olist[nolist++] = n2;
		if (*s != '\0')
			s++;
	}
	olist[nolist] = 0;
	if (dbg)
		for (i=0; i<nolist; i += 2)
			printf("%3d %3d\n", olist[i], olist[i+1]);
}

in_olist(n)	/* is n in olist? */
int n;
{
	int i;

	if (nolist == 0)
		return(1);	/* everything is included */
	for (i = 0; i < nolist; i += 2)
		if (n >= olist[i] && n <= olist[i+1])
			return(1);
	return(0);
}

conv(fp)
register FILE *fp;
{
	register int c, k;
	int m, n, i, n1, m1;
	char str[100], buf[300];

	while ((c = getc(fp)) != EOF) {
		switch (c) {
		case '\n':	/* when input is text */
		case ' ':
		case 0:		/* occasional noise creeps in */
			break;
		case '{':	/* push down current environment */
			t_push();
			break;
		case '}':
			t_pop();
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			/* two motion digits plus a character */
			hmot((c-'0')*10 + getc(fp)-'0');
			put1(getc(fp));
			break;
		case 'c':	/* single ascii character */
			put1(getc(fp));
			break;
		case 'C':
			fscanf(fp, "%s", str);
			put1s(str);
			break;
		case 't':	/* straight text */
			fgets(buf, sizeof(buf), fp);
			t_text(buf);
			break;
		case 'D':	/* draw function */
			fgets(buf, sizeof(buf), fp);
			switch (buf[0]) {
			case 'l':	/* draw a line */
				sscanf(buf+1, "%d %d", &n, &m);
				drawline(n, m, ".");
				break;
			case 'c':	/* circle */
				sscanf(buf+1, "%d", &n);
				drawcirc(n);
				break;
			case 'e':	/* ellipse */
				sscanf(buf+1, "%d %d", &m, &n);
				drawellip(m, n);
				break;
			case 'a':	/* arc */
				sscanf(buf+1, "%d %d %d %d", &n, &m, &n1, &m1);
				drawarc(n, m, n1, m1);
				break;
			case '~':	/* wiggly line */
				drawwig(buf+1);
				break;
			default:
				error(FATAL, "unknown drawing function %s\n", buf);
				break;
			}
			break;
		case 's':
			fscanf(fp, "%d", &n);	/* ignore fractional sizes */
			setsize(t_size(n));
			break;
		case 'f':
			fscanf(fp, "%s", str);
			setfont(t_font(str));
			break;
		case 'H':	/* absolute horizontal motion */
			/* fscanf(fp, "%d", &n); */
			while ((c = getc(fp)) == ' ')
				;
			k = 0;
			do {
				k = 10 * k + c - '0';
			} while (isdigit(c = getc(fp)));
			ungetc(c, fp);
			hgoto(k);
			break;
		case 'h':	/* relative horizontal motion */
			/* fscanf(fp, "%d", &n); */
			while ((c = getc(fp)) == ' ')
				;
			k = 0;
			do {
				k = 10 * k + c - '0';
			} while (isdigit(c = getc(fp)));
			ungetc(c, fp);
			hmot(k);
			break;
		case 'w':	/* word space */
			break;
		case 'V':
			fscanf(fp, "%d", &n);
			vgoto(n);
			break;
		case 'v':
			fscanf(fp, "%d", &n);
			vmot(n);
			break;
		case 'p':	/* new page */
			fscanf(fp, "%d", &n);
			t_page(n);
			break;
		case 'n':	/* end of line */
			while (getc(fp) != '\n')
				;
			t_newline();
			break;
		case '#':	/* comment */
			while (getc(fp) != '\n')
				;
			break;
		case 'x':	/* device control */
			devcntrl(fp);
			break;
		default:
			error(!FATAL, "unknown input character %o %c\n", c, c);
			done();
		}
	}
}

devcntrl(fp)	/* interpret device control functions */
FILE *fp;
{
	char str[20];
	int c, n;

	fscanf(fp, "%s", str);
	switch (str[0]) {	/* crude for now */
	case 'i':	/* initialize */
		fileinit();
		t_init(0);
		break;
	case 'T':	/* device name */
		fscanf(fp, "%s", devname);
		break;
	case 't':	/* trailer */
		t_trailer();
		break;
	case 'p':	/* pause -- can restart */
		t_reset('p');
		break;
	case 's':	/* stop */
		t_reset('s');
		break;
	case 'r':	/* resolution assumed when prepared */
		fscanf(fp, "%d", &res);
		break;
	case 'f':	/* font used */
		fscanf(fp, "%d %s", &n, str);
		loadfont(n, str);
		break;
	}
	while (getc(fp) != '\n')	/* skip rest of input line */
		;
}

fileinit()	/* read in font and code files, etc. */
{
}

fontprint(i)	/* debugging print of font i (0,...) */
{
}

loadcode(n, nw)	/* load codetab on position n (0...); #chars is nw */
int n, nw;
{
}

loadfont(n, s)	/* load font info for font s on position n (1...) */
int n;
char *s;
{
}

error(f, s, a1, a2, a3, a4, a5, a6, a7) {
	fprintf(stderr, "hc: ");
	fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (f)
		exit(1);
}


/*
	Here beginneth all the stuff that really depends
	on the 202 (we hope).
*/


char	devname[20]	= "hp2621";

#define	ESC	033
#define	HOME	'H'
#define	CLEAR	'J'
#define	FF	014

#define	PGHEIGHT	100	/* could be larger */
#define	PGWIDTH		100	/* ditto */
/* should map input into a larger thing, then print window */
char	pagebuf[PGHEIGHT][PGWIDTH];
int	minh	= PGWIDTH;
int	maxh	= 0;
int	minv	= PGHEIGHT;
int	maxv	= 0;


int	size	= 1;
int	font	= 1;		/* current font */
int	hpos;		/* horizontal position where we are supposed to be next (left = 0) */
int	vpos;		/* current vertical position (down positive) */

int	horig;		/* h origin of current block; hpos rel to this */
int	vorig;		/* v origin of current block; vpos rel to this */

int	DX	= 10;	/* step size in x for drawing */
int	DY	= 10;	/* step size in y for drawing */
int	drawdot	= '.';	/* draw with this character */
int	drawsize = 1;	/* shrink by this factor when drawing */

t_init(reinit)	/* initialize device */
int reinit;
{
	int i, j;

	fflush(stdout);
	hpos = vpos = 0;
	for (i = 0; i < PGHEIGHT; i++)
		for (j = 0; j < PGWIDTH; j++)
			pagebuf[i][j] = ' ';
	minh = PGWIDTH;
	maxh = 0;
	minv = PGHEIGHT;
	maxv = 0;
}

#define	MAXSTATE	5

struct state {
	int	ssize;
	int	sfont;
	int	shpos;
	int	svpos;
	int	shorig;
	int	svorig;
};
struct	state	state[MAXSTATE];
struct	state	*statep = state;

t_push()	/* begin a new block */
{
	hflush();
	statep->ssize = size;
	statep->sfont = font;
	statep->shorig = horig;
	statep->svorig = vorig;
	statep->shpos = hpos;
	statep->svpos = vpos;
	horig = hpos;
	vorig = vpos;
	hpos = vpos = 0;
	if (statep++ >= state+MAXSTATE)
		error(FATAL, "{ nested too deep");
	hpos = vpos = 0;
}

t_pop()	/* pop to previous state */
{
	if (--statep < state)
		error(FATAL, "extra }");
	size = statep->ssize;
	font = statep->sfont;
	hpos = statep->shpos;
	vpos = statep->svpos;
	horig = statep->shorig;
	vorig = statep->svorig;
}

int	np;	/* number of pages seen */
int	npmax;	/* high-water mark of np */
int	pgnum[40];	/* their actual numbers */
long	pgadr[40];	/* their seek addresses */

t_page(n)	/* do whatever new page functions */
{
	long ftell();
	int c, m, i;
	char buf[100], *bp;

	pgnum[np++] = n;
	pgadr[np] = ftell(fp);
	if (np > npmax)
		npmax = np;
	if (output == 0) {
		output = in_olist(n);
		t_init(1);
		return;
	}
	/* have just printed something, and seen p<n> for next one */
	putpage();
	fflush(stdout);

	if (keepon) {
		t_init(1);
		return;
	}
  next:
	for (bp = buf; (*bp = readch()); )
		if (*bp++ == '\n')
			break;
	*bp = 0;
	switch (buf[0]) {
	case 0:
		done();
		break;
	case '\n':
		output = in_olist(n);
		t_init(1);
		return;
	case '!':
		callunix(&buf[1]);
		fputs("!\n", stderr);
		break;
	case 'e':
		erase = 1 - erase;
		break;
	case 'w':
		wflag = 1 - wflag;
		break;
	case 'a':
		aspect = atof(&buf[1]);
		break;
	case '-':
	case 'p':
		m = atoi(&buf[1]) + 1;
		if (fp == stdin) {
			fputs("you can't; it's not a file\n", stderr);
			break;
		}
		if (np - m <= 0) {
			fputs("too far back\n", stderr);
			break;
		}
		np -= m;
		fseek(fp, pgadr[np], 0);
		output = 1;
		t_init(1);
		return;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		m = atoi(&buf[0]);
		for (i = 0; i < npmax; i++)
			if (m == pgnum[i])
				break;
		if (i >= npmax || fp == stdin) {
			fputs("you can't\n", stderr);
			break;
		}
		np = i + 1;
		fseek(fp, pgadr[np], 0);
		output = 1;
		t_init(1);
		return;
	case 'o':
		outlist(&buf[1]);
		output = 0;
		t_init(1);
		return;
	case '?':
		fputs("!cmd	unix cmd\n", stderr);
		fputs("p	print this page again\n", stderr);
		fputs("-n	go back n pages\n", stderr);
		fputs("n	print page n (previously printed)\n", stderr);
		fputs("o...	set the -o output list to ...\n", stderr);
		fputs("en	n=0 -> don't erase; n=1 -> erase\n", stderr);
		fputs("an	sets aspect ratio to n\n", stderr);
		break;
	default:
		fputs("?\n", stderr);
		break;
	}
	goto next;
}

putpage()
{
	int i, j, k;

	fflush(stdout);
	for (i = minv; i <= maxv; i++) {
		for (k = maxh; pagebuf[i][k] == ' '; k--)
			;
		if (k > minh + 78)
			k = minh + 78;
		for (j = minh; j <= k; j++)
			putchar(pagebuf[i][j]);
		putchar('\n');
	}
	fflush(stdout);
}

t_newline()	/* do whatever for the end of a line */
{
	hpos = 0;
}

t_size(n)	/* convert integer to internal size number*/
int n;
{
}

t_font(s)	/* convert string to internal font number */
char *s;
{
}

t_text(s)	/* print string s as text */
char *s;
{
	int c, w;
	char str[100];

	if (!output)
		return;
	while ((c = *s++) != '\n') {
		if (c == '\\') {
			switch (c = *s++) {
			case '\\':
			case 'e':
				put1('\\');
				break;
			case '(':
				str[0] = *s++;
				str[1] = *s++;
				str[2] = '\0';
				put1s(str);
				break;
			}
		} else {
			put1(c);
		}
		hmot(w);
	}
}

t_reset(c)
{
	int n;

	output = 1;
	fflush(stdout);
}

t_trailer()
{
}

hgoto(n)
{
	hpos = n;	/* this is where we want to be */
			/* before printing a character, */
			/* have to make sure it's true */
}

hmot(n)	/* generate n units of horizontal motion */
int n;
{
	hgoto(hpos + n);
}

hflush()	/* actual horizontal output occurs here */
{
}

vgoto(n)
{
	vpos = n;
}

vmot(n)	/* generate n units of vertical motion */
int n;
{
	vgoto(vpos + n);	/* ignores rounding */
}

put1s(s)	/* s is a funny char name */
char *s;
{
	int i;
	char *p;
	extern char *spectab[];
	static char prev[10] = "";
	static int previ;

	if (!output)
		return;
	if (strcmp(s, prev) != 0) {
		previ = -1;
		for (i = 0; spectab[i] != 0; i += 2)
			if (strcmp(spectab[i], s) == 0) {
				strcpy(prev, s);
				previ = i;
				break;
			}
	}
	if (previ >= 0) {
		for (p = spectab[previ+1]; *p; p++)
			store(*p);
	} else
		prev[0] = 0;
}

put1(c)	/* output char c */
int c;
{
	if (!output)
		return;
	store(c);
}

setsize(n)	/* set point size to n (internal) */
int n;
{
}

t_fp(n, s)	/* font position n now contains font s */
int n;
char *s;
{
}

setfont(n)	/* set font to n */
int n;
{
}

done()
{
	output = 1;
	putpage();
	fflush(stdout);
	exit(0);
}

callunix(line)
char line[];
{
	int rc, status, unixpid;
	if( (unixpid=fork())==0 ) {
		signal(SIGINT,sigint); signal(SIGQUIT,sigquit);
		close(0); dup(2);
		execl("/bin/sh", "-sh", "-c", line, 0);
		exit(255);
	}
	else if(unixpid == -1)
		return;
	else{	signal(SIGINT, SIG_IGN); signal(SIGQUIT, SIG_IGN);
		while( (rc = wait(&status)) != unixpid && rc != -1 ) ;
		signal(SIGINT,done); signal(SIGQUIT,sigquit);
	}
}

readch ()
{
	int c;
	static FILE *rcf;
	static nbol;	/* 0 if at beginning of a line */

	if (rcf == NULL) {
		rcf = fopen ("/dev/tty", "r");
		setbuf (rcf, NULL);
	}

	if (!nbol)
		fprintf (stderr, "hc: ");	/* issue prompt */
	if ((c = getc (rcf)) == EOF)
		return 0;
	nbol = (c != '\n');
	return c;
}

store(c)
{
	int i, j;

	i = hpos / hscale;
	j = vpos / vscale;
	if (i >= PGWIDTH)
		i = PGWIDTH - 1;
	if (j >= PGHEIGHT)
		j = PGHEIGHT - 1;
	/* note the reversal! */
	pagebuf[j][i] = c;
	if (i > maxh)
		maxh = i;
	if (i < minh)
		minh = i;
	if (j > maxv)
		maxv = j;
	if (j < minv)
		minv = j;
}

char *spectab[] ={
	"em", "-",
	"hy", "-",
	"bu", "o",
	"mu", "x",
	"sq", "[]",
	"en", "-",
	"ru", "_",
	"l.", ".",
	"br", "|",
	"vr", "|",
	"fm", "'",
	"or", "|",
	"pl", "+",
	"ci", "O",
	"**", "*",
	0, 0,
};

