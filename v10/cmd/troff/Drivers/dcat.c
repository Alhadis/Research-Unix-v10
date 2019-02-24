THIS VERSION IS NOT UP TO DATE;
INCLUDED FOR HISTORICAL REASONS.
/*
 *	drive C/A/T typesetter
 *	currently stdout only
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
Dt ...\n	draw operation 't':
	Dl x y		line from here by x,y
	Dc d		circle of diameter d with left side here
	De x y		ellipse of axes x,y with left side here
	Da x y r	arc counter-clockwise by x,y of radius r
	D~ x y x y ...	wiggly line by x,y then x,y ...
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
	x H n	set character height to n
	x S n	set slant to N

	Subcommands like "i" are often spelled out like "init".
*/

#include	<stdio.h>
#include	<ctype.h>
#include	<signal.h>

#include "dev.h"
#define	NFONT	8

int	output	= 0;	/* do we do output at all? */
int	nolist	= 0;	/* output page list if > 0 */
int	olist[20];	/* pairs of page numbers */
int	spage	= 9999;	/* stop every spage pages */
int	scount	= 0;

struct	dev	dev;
struct font *fontbase[NFONT+1];
int	*pstab;
int	nsizes;
int	nfonts;
int	smnt;	/* index of first special font */
int	nchtab;
char	*chname;
int	*chtab;
char	*fitab[NFONT+1];
char	*widthtab[NFONT+1];	/* widtab would be a better name */
char	*codetab[NFONT+1];	/* device codes */

#define	FATAL	1
#define	BMASK	0377
int	dbg	= 0;
int	res;		/* input assumed computed according to this resolution */

#define	HERE	0
#define	THERE	1
#define	HOST	"r70\n"
char	*typesetter[]	= { 
			"/dev/cat",	/* typesetter device */
			"r70!/dev/202"
};
FILE	*tf	= NULL;	/* output file */
char	*fontdir	= "/usr/bwk/troff/devcat";
extern char devname[];

#define	hmot(n)		hpos += n
#define	hgoto(n)	hpos = n

int	size	= -1;	/* this is invalid */
int	font	= -1;	/* current font */
int	hpos;		/* horizontal position where we are supposed to be next (left = 0) */
int	oldh;		/* previous H position */
int	vpos;		/* current vertical position (down positive) */
int	oldv;		/* current pos in 1/4 point units */
int	horig;		/* h origin of current block; hpos rel to this */
int	vorig;
int	DX	= 1;	/* step size in x for drawing */
int	DY	= 3;	/* step size in y for drawing */
int	drawdot	= '.';	/* draw with this character */
int	drawsize = 1;	/* shrink by this factor when drawing */

main(argc, argv)
char *argv[];
{
	FILE *fp;
	int i;
	int busyflag = 0;
	int waitflag = 0;
	int where = HERE;
	char wherebuf[100];
	int done();

	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 't':
			tf = stdout;
			break;
		case 'o':
			outlist(&argv[1][2]);
			break;
		case 'd':
			dbg = atoi(&argv[1][2]);
			if (dbg == 0) dbg = 1;
			break;
		case 'b':
			busyflag = 1;
			break;
		case 'w':
			waitflag = 1;
			break;
		case 's':
			spage = atoi(&argv[1][2]);
			if (spage <= 0)
				spage = 9999;
			break;
		}
		argc--;
		argv++;
	}
  tryagain:
	if ((fp = fopen("/etc/where", "r")) != NULL)
		if(fgets(wherebuf, sizeof(wherebuf), fp) != NULL)
			if(strcmp(wherebuf, HOST) != 0)
				where = THERE;
	fclose(fp);
	if (tf != stdout)
		tf = fopen(typesetter[where], "w");
	if (busyflag) {
		printf(tf==NULL? "Busy.\n": "Available.\n");
		exit(0);
	}
	if (tf == NULL) {
		if (waitflag == 0) {
			error(!FATAL, "can't open typesetter\n");
			exit(1);
		} else {
			sleep(60);
			goto tryagain;
		}
	}

	if (signal(SIGINT, done) == SIG_IGN) {
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
	} else {
		signal(SIGQUIT, done);
		signal(SIGHUP, done);
	}
	signal(SIGTERM, done);

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
	account();
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
        char str[20], str1[50], buf[50];
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
		fgets(buf, sizeof buf, fp);	/* in case there's a filename */
		ungetc('\n', fp);	/* fgets goes too far */
		str1[0] = 0;	/* in case there's nothing to come in */
		sscanf(buf, "%s", str1);
		loadfont(n, str, str1);
		break;
	/* these don't belong here... */
	case 'H':	/* char height */
		fscanf(fp, "%d", &n);
		t_charht(n);
		break;
	case 'S':	/* slant */
		fscanf(fp, "%d", &n);
		t_slant(n);
		break;
	}
	while ((c = getc(fp)) != '\n')	/* skip rest of input line */
		if (c == EOF)
			break;
}

fileinit()	/* read in font and code files, etc. */
{
	int i, fin, nw;
	char *malloc(), *filebase, *p;
	char temp[60];

	/* open table for device,
	/* read in resolution, size info, font info, etc.
	/* and set params
	*/
	sprintf(temp, "%s/dev%s/desc.out", fontdir, devname);
	if ((fin = open(temp, 0)) < 0) {
		error(FATAL, "can't open tables for %s\n", temp);
	}
	read(fin, &dev, sizeof(struct dev));
	nfonts = dev.nfonts;
	nsizes = dev.nsizes;
	nchtab = dev.nchtab;
	filebase = malloc(dev.filesize);	/* enough room for whole file */
	read(fin, filebase, dev.filesize);	/* all at once */
	pstab = (int *) filebase;
	chtab = pstab + nsizes + 1;
	chname = (char *) (chtab + dev.nchtab);
	p = chname + dev.lchname;
	for (i = 1; i <= nfonts; i++) {
		fontbase[i] = (struct font *) p;
		nw = *p & BMASK;	/* 1st thing is width count */
		if (smnt == 0 && fontbase[i]->specfont == 1)
			smnt = i;	/* first special font */
		p += sizeof(struct font);	/* that's what's on the beginning */
		widthtab[i] = p;
		fitab[i] = p + nw;
		p += nw + dev.nchtab + 128 - 32;
		loadcode(i, nw, NULL);
		t_fp(i, fontbase[i]->namefont, fontbase[i]->intname);
		if(dbg > 1) fontprint(i);
	}
	fontbase[0] = (struct font *) malloc(255 + dev.nchtab + (128-32) + sizeof (struct font));
	widthtab[0] = (char *) fontbase[0] + sizeof (struct font);
	fontbase[0]->nwfont = 255;
	close(fin);
}

fontprint(i)	/* debugging print of font i (0,...) */
{
	int j, k, n;
	char *p;

	printf("font %d:\n", i);
	p = (char *) fontbase[i];
	n = fontbase[i]->nwfont & BMASK;
	printf("base=0%o, nchars=%d, spec=%d, name=%s, widtab=0%o, fitab=0%o\n",
		p, n, fontbase[i]->specfont, fontbase[i]->namefont, widthtab[i], fitab[i]);
	printf("widths:\n");
	for (j=0; j <= n; j++) {
		printf(" %2d", widthtab[i][j] & BMASK);
		if (j % 20 == 19) printf("\n");
	}
	printf("\ncodetab:\n");
	for (j=0; j <= n; j++) {
		printf(" %2d", codetab[i][j] & BMASK);
		if (j % 20 == 19) printf("\n");
	}
	printf("\nfitab:\n");
	for (j=0; j <= dev.nchtab + 128-32; j++) {
		printf(" %2d", fitab[i][j] & BMASK);
		if (j % 20 == 19) printf("\n");
	}
	printf("\n");
}

loadcode(n, nw, s1)	/* load codetab on position n (0...); #chars is nw */
int n, nw;
char *s1;
{
	int cin;
	char temp[60];
	char *malloc();

	if (codetab[n] != NULL)
		free(codetab[n]);
	codetab[n] = malloc(nw);	/* read in device codes */
	if (s1 == NULL || *s1 == 0)
		sprintf(temp, "%s/dev%s/%s.code", fontdir, devname, fontbase[n]->namefont);
	else
		sprintf(temp, "%s/%s.code", s1, fontbase[n]->namefont);
	if ((cin = open(temp, 0)) < 0)
		error(FATAL, "can't open code tables %s", temp);
	read(cin, codetab[n], nw);
	close(cin);
}

loadfont(n, s, s1)	/* load font info for font s on position n (0...) */
int n;
char *s, *s1;
{
	char temp[60];
	int fin, nw, norig;

	if (n < 0 || n > NFONT)
		error(FATAL, "illegal fp command %d %s", n, s);
	if (strcmp(s, fontbase[n]->namefont) == 0)
		return;
	if (s1 == NULL || s1[0] == '\0')
		sprintf(temp, "%s/dev%s/%s.out", fontdir, devname, s);
	else
		sprintf(temp, "%s/%s.out", s1, s);
	if ((fin = open(temp, 0)) < 0)
		error(FATAL, "can't open font table %s", temp);
	norig = fontbase[n]->nwfont & BMASK;
	read(fin, fontbase[n], norig + nchtab+128-32 + sizeof(struct font));
	if ((fontbase[n]->nwfont & BMASK) > norig)
		error(FATAL, "Font %s too big for position %d\n", s, n);
	close(fin);
	nw = fontbase[n]->nwfont & BMASK;
	widthtab[n] = (char *) fontbase[n] + sizeof(struct font);
	fitab[n] = (char *) widthtab[n] + nw;
	loadcode(n, nw, s1);
	t_fp(n, fontbase[n]->namefont, fontbase[n]->intname);
	fontbase[n]->nwfont = norig;	/* so can later use full original size */
	if (dbg > 1) fontprint(n);
}

done()
{
	if (tf == NULL)
		exit(1);
	t_reset('s');
	exit(0);
}

error(f, s, a1, a2, a3, a4, a5, a6, a7) {
	fprintf(stderr, "dcat: ");
	fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (f)
		done();
}


/*
	Here beginneth all the stuff that really depends
	on the Graphic Systems CAT.
*/


char	devname[20]	= "cat";

#define	RES	432	/* CAT is 432 goobies per inch */
			/* this is the way the cat actually works.
			/* the input resolution might be different,
			/* but this version of the program doesn't use it yet.
			*/
#define	TRAILER	(14 * res)	/* trailer for last page */
#define	ROUND3(n)	(((n+1)/3)*3)
#define	IMOT	16	/* mandatory initial motion for typesetter */
#define	LMARGIN	0	/* left margin offset */
#define	HMAX	(46 * (res/6))	/* maximum horizontal size = 46 picas */
#define	VMAX	(15 * res)	/* 15 inch page */

#define	DOUBLE	0200	/* double lens indicated by this */

#define	INIT	0100	/* initialize typesetter */
			/* supposedly the same as */
			/* DOWN, FWD, LRAIL, LMAG, LCASE */
#define	STOP	0111	/* all done */
#define	PAD	0101	/* lower rail padding */
#define	FWD	0107	/* horizontal forward (right) */
#define	BACK	0110	/* horizontal backward */
#define	UP	0114	/* vertical up (back up page) */
#define	DOWN	0112	/* down page */
#define	HMOT	0200	/* horizontal motion bit */
#define	VMOT	0140	/* vertical motion bits */
#define	SIZE	0120	/* size change */

	/* font 1 = lower rail, lower mag */
	/* font 2 = upper rail, lower mag */
	/* font 3 = lower rail, upper mag */
	/* font 4 = upper rail, upper mag */
	/* god only knows how tilt factors into this */
#define	URAIL	0102
#define	LRAIL	0101
#define	UMAG	0103
#define	LMAG	0104
#define	LTILT	0117	/* for CAT 8 */
#define	UTILT	0116	/* copied from Scribe data base, not tested */
#define	UCASE	0106	/* upper case mode */
#define	LCASE	0105

#define	MAXFWD	127	/* max number of horizontal units */
#define	MAXDOWN	31	/* max vertical units */

long	paper;		/* paper used */

static int	hdir	= FWD;	/* current horizontal dir */
static int	vdir	= DOWN;	/* current vertical dir */

static int	lastw	= 0;	/* last width, used by t_text */
static int	rail	= -1;	/* LRAIL or URAIL */
static int	mag	= -1;	/* LMAG or UMAG */
static int	tilt	= -1;
static int	curcase	= -1;	/* either LCASE or UCASE */
static int	hpos;		/* current horizontal position (left = 0) */
static int	vpos;		/* current vertical position (down positive) */

t_init(reinit)	/* initialize device */
int reinit;
{
	int i;

	hpos = vpos = oldv = oldh = 0;
	drawdot = '.';
	drawsize = 2;	/* half size */
	setsize(t_size(10));	/* start somewhere */
	setfont(1);
	if (!reinit) {
		putc(INIT, tf);
		hmot(IMOT);
		vmot(3 * MAXDOWN);
		hpos = vpos = 0;
		movep(LMARGIN, 0);
	} else {
		putc(DOWN, tf);
		putc(FWD, tf);
		putc(LRAIL, tf);
		putc(LMAG, tf);
		putc(LCASE, tf);
	}
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

t_page(n)	/* do whatever new page functions */
{
	int i;

	if (output) {
		if (tf != stdout)
			paper += vpos;
		if (++scount >= spage) {
			t_reset('p');
			scount = 0;
		}
	}
	vpos = 0;
	oldv = 0;
	output = 1;
	/* somehow get to left margin? */
	if (nolist == 0)
		return;	/* no -o specified */
	output = 0;
	for (i = 0; i < nolist; i += 2)
		if (n >= olist[i] && n <= olist[i+1]) {
			output = 1;
			break;
		}
}

t_newline()	/* do whatever for the end of a line */
{
	hpos = 0;	/* because we want to be back at the left margin */
}

t_size(n)	/* convert integer to internal size number*/
int n;
{
	int i;

	if (n <= pstab[0])
		return(1);
	else if (n >= pstab[nsizes-1])
		return(nsizes);
	for (i = 0; n > pstab[i]; i++)
		;
	return(i+1);
}

t_charht(n)	/* set character height to n */
int n;
{
}

t_slant(n)	/* set slant to n */
int n;
{
}

t_font(s)	/* convert string to internal font number */
char *s;
{
	int n;

	n = atoi(s);
	if (n < 0 || n > nfonts)
		n = 1;
	return(n);
}

t_text(s)	/* print string s as text */
char *s;
{
	int c, w;
	char str[100];

	if (!output)
		return;
	while (c = *s++) {
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
		hmot(lastw);
		if (dbg) printf("width = %d\n", lastw);
	}
}

t_reset(c)
{
	int n;

	if (output)
		paper += vpos;
	output = 1;	/* by God */
	putc(INIT, tf);
	putc(STOP, tf);
	for (n = 0; n < 8; n++)	/* flush out CAT internal buffer */
		putc(PAD, tf);
	fflush(tf);
}

char *tracct[] = {	/* accounting file */
	"/usr/adm/tracct",
	"r70!/usr/adm/tracct"
};

account()	/* record paper use */
{
	FILE *f = NULL;
	char *name, *getlogin();
	int i;

	return;	/* for now */
	if (tf == stdout)
		return;
	for(i=0; i<sizeof(tracct)/sizeof(*tracct) && f== NULL; i++)
		f = fopen(tracct[i], "a");
	if(f != NULL) {
		name = getlogin();
		if (name == NULL)
			name = "???";
		fprintf(f, "%4d %s\n", (int)(paper/res + 20+6)/12, name);
	}
}

t_trailer()
{
	return;
	vpos = oldv = 0;
	vgoto(TRAILER);
	vpos = oldv = 0;
}

movep(x, y)	/* go to internal position x, y */
int x, y;
{
	hmot(x - hpos);
	vmot(ROUND3(y) - vpos);
}

hflush()	/* actual horizontal output occurs here */
{
	int htrue;
	int k, dir, n;

	if (!output)
		return;
	htrue = hpos + horig;
	n = htrue - oldh;
	if (htrue+n > HMAX)
		n = HMAX - htrue;
	if (htrue+n < 0)
		n = -htrue;
	htrue += n;
	dir = (n >= 0) ? FWD : BACK;
	if (dir != hdir)
		putc(hdir = dir, tf);
	if (n < 0)
		n = -n;
	while (n > 0) {
		k = (n > MAXFWD) ? MAXFWD : n;
		putc(HMOT | ~k, tf);
		n -= k;
		}
        oldh = htrue + horig;
}

vgoto(n)
{
	vmot(n - vpos);
}

vmot(n)	/* generate n units of vertical motion */
int n;
{
	int dir, k;
	if (vpos+n > VMAX)
		n = VMAX - vpos;
	if (vpos+n < 0)
		n = -vpos;
	vpos += n;
	vpos = ROUND3(vpos);
	dir = (n >= 0) ? DOWN : UP;
	if (dir != vdir)
		putc(vdir = dir, tf);
	if (n < 0)
		n = -n;
	n = ((n+1) / 3);	/* round to multiple of 3 */
	while (n > 0) {
		k = (n > MAXDOWN) ? MAXDOWN : n;
		putc(VMOT | (~k & MAXDOWN), tf);
		n -= k;
	}
	if (vpos % 3 != 0)
		fprintf(stderr, "vpos = %d\n", vpos);
}


put1s(s)	/* s is a funny char name */
char *s;
{
	int i;

	if (!output)
		return;
	if (dbg) printf("%s ", s);
	for (i = 0; i < nchtab; i++)
		if (strcmp(&chname[chtab[i]], s) == 0)
			break;
	if (i < nchtab)
		put1(i + 128);
}

put1(c)	/* output char c */
int c;
{
	char *pw;
	register char *p;
	register int i, k;
	int j, ofont, code, w;

	if (!output)
		return;
	c -= 32;
	if (c <= 0) {
		if (dbg) printf("non-exist 0%o\n", c+32);
		lastw = widthtab[font][0] * pstab[size-1] / dev.unitwidth;
		return;
	}
	k = ofont = font;
	i = fitab[font][c] & BMASK;
	if (i != 0) {	/* it's on this font */
		p = codetab[font];
		pw = widthtab[font];
	} else if (smnt > 0) {		/* on special (we hope) */
		for (k=smnt, j=0; j <= nfonts; j++, k = (k+1) % (nfonts+1))
			if ((i = fitab[k][c] & BMASK) != 0) {
				p = codetab[k];
				pw = widthtab[k];
				setfont(k);
				break;
			}
	}
	if (i == 0 || (code = p[i] & BMASK) == 0 || k > nfonts) {
		if (dbg) printf("not found 0%o\n", c+32);
		return;
	}
	if (code < 64) {	/* it's lower case */
		if (curcase != LCASE)
			putc(curcase = LCASE, tf);
	} else {
		if (curcase != UCASE)
			putc(curcase = UCASE, tf);
		code -= 64;
	}
	hflush();
	if (dbg) {
		if (isprint(c+32))
			printf("%c %d\n", c+32, code);
		else
			printf("%03o %d\n", c+32, code);
	} else
		putc(code, tf);	/* character is < 254 */
	if (font != ofont)
		setfont(ofont);
	lastw = pw[i] & 077;
	lastw = (lastw * pstab[size-1] + dev.unitwidth/2) / dev.unitwidth;
}

setsize(n)	/* set point size to n (internal) */
int n;
{
	static int stab[] ={ 0,010,0,01,07,02,03,04,05,0211,06,0212,0213,0214,0215,0216,0217 };
	int spos;

	if (!output)
		return;
	if (n == size)
		return;	/* already there */
	putc(stab[n] & ~DOUBLE | SIZE, tf);
	if (!(stab[size] & DOUBLE) && stab[n] & DOUBLE) {
		spos = hpos;
		hmot(55);	/* single-double transition */
		hpos = spos;	/* lens-compensation motion doesn't affect absolute position */
	} else if (stab[size] & DOUBLE && !(stab[n] & DOUBLE)) {
		spos = hpos;
		hmot(-55);	/* double-single transition */
		hpos = spos;
	}
	size = n;
}

/* font position info: */

struct {
	char *name;
	int number;
} fontname[NFONT+1];

t_fp(n, s, si)	/* font position n now contains font s, intname si */
int n;
char *s, *si;
{
	fontname[n].name = s;
	fontname[n].number = atoi(si);
}

setfont(n)	/* set font to n */
int n;
{
	if (!output)
		return;
	/* needs stuff added for 8 font version */
	/* easiest to loop over 3 font bits */
	/* masking them off */
	switch (n) {
	case 1:
		if (rail != LRAIL)
			putc(rail = LRAIL, tf);
		if (mag != LMAG)
			putc(mag = LMAG, tf);
		break;
	case 2:
		if (rail != URAIL)
			putc(rail = URAIL, tf);
		if (mag != LMAG)
			putc(mag = LMAG, tf);
		break;
	case 3:
		if (rail != LRAIL)
			putc(rail = LRAIL, tf);
		if (mag != UMAG)
			putc(mag = UMAG, tf);
		break;
	case 4:
		if (rail != URAIL)
			putc(rail = URAIL, tf);
		if (mag != UMAG)
			putc(mag = UMAG, tf);
		break;
	default:
		error(FATAL, "illegal font %d\n", n);
	}
	font = n;
}
