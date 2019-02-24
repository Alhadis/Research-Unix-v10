/*
 *	drive 8400 typesetter
 *	This will probably run it faster if you
 *	modify setfont and put1 to actually send out
 *	a font change command when needed, instead of
 *	after every special character.

 *	realistically, this code has never been tested,
 *	so take it with a grain of salt

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
w	paddable word space -- no action needed
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
#define	NFONT	10

int	output	= 0;	/* do we do output at all? */
int	nolist	= 0;	/* output page list if > 0 */
int	olist[20];	/* pairs of page numbers */
int	spage	= 9999;	/* stop every spage pages */
int	scount	= 0;

struct	dev	dev;
struct Font *fontbase[NFONT+1];
short	*pstab;
int	nsizes;
int	nfonts;
int	smnt;	/* index of first special font */
int	nchtab;
char	*chname;
short	*chtab;
char	*fitab[NFONT+1];
char	*widthtab[NFONT+1];	/* widtab would be a better name */
char	*codetab[NFONT+1];	/* device codes */

#define	FATAL	1
#define	BMASK	0377
int	dbg	= 0;
int windowht = 250;	/* resettable logical height of d202 faceplate */
int	res;		/* input assumed computed according to this resolution */

char	*typesetter	= "/dev/8400";
FILE	*tf	= NULL;	/* output file */
char	*fontdir	= "/usr/lib/font";
extern char devname[];

#define	hmot(n)		hpos += n
#define	hgoto(n)	hpos = n
#define	vmot(n)		vgoto(vpos + n)

int	size	= -1;	/* this is invalid */
int	font	= -1;	/* current font */
int	hpos;		/* horizontal position where we are supposed to be next (left = 0) */
int	oldh;		/* previous H position */
int	vpos;		/* current vertical position (down positive) */
int	oldv;		/* current pos in 1/4 point units */
int	horig;		/* h origin of current block; hpos rel to this */
int	vorig;
int	DX	= 2;	/* step size in x for drawing */
int	DY	= 2;	/* step size in y for drawing */
int	drawdot	= '.';	/* draw with this character */
int	drawsize = 1;	/* shrink by this factor when drawing */

main(argc, argv)
char *argv[];
{
	FILE *fp;
	int i;
	int busyflag = 0;
	int waitflag = 0;
	int done();

	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'f':
		case 'F':
			fontdir = argv[2];
			argv++;
			argc--;
			break;
		case 't':
			tf = stdout;
			break;
		case 'o':
			outlist(&argv[1][2]);
			break;
		case 'd':
			dbg = atoi(&argv[1][2]);
			if (dbg == 0) dbg = 1;
			tf = stdout;
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
		case 'i':
			windowht = atoi(&argv[1][2]);
			break;
		}
		argc--;
		argv++;
	}
  tryagain:
	if (tf != stdout)
		tf = fopen(typesetter, "w");
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
	sprintf(temp, "%s/dev%s/DESC.out", fontdir, devname);
	if ((fin = open(temp, 0)) < 0)
		error(FATAL, "can't open tables for %s\n", temp);
	read(fin, &dev, sizeof(struct dev));
	nfonts = dev.nfonts;
	nsizes = dev.nsizes;
	nchtab = dev.nchtab;
	filebase = malloc(dev.filesize);	/* enough room for whole file */
	read(fin, filebase, dev.filesize);	/* all at once */
	pstab = (short *) filebase;
	chtab = pstab + nsizes + 1;
	chname = (char *) (chtab + dev.nchtab);
	p = chname + dev.lchname;
	for (i = 1; i <= nfonts; i++) {
		fontbase[i] = (struct Font *) p;
		nw = *p & BMASK;	/* 1st thing is width count */
		if (smnt == 0 && fontbase[i]->specfont == 1)
			smnt = i;	/* first special font */
		p += sizeof(struct Font);	/* that's what's on the beginning */
		widthtab[i] = p;
		codetab[i] = p + 2 * nw;
		fitab[i] = p + 3 * nw;
		p += 3 * nw + dev.nchtab + 128 - 32;
		t_fp(i, fontbase[i]->namefont, fontbase[i]->intname);
		if(dbg > 1) fontprint(i);
	}
	fontbase[0] = (struct Font *) malloc(3*255 + dev.nchtab + (128-32) + sizeof (struct Font));
	widthtab[0] = (char *) fontbase[0] + sizeof (struct Font);
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
	read(fin, fontbase[n], 3*norig + nchtab+128-32 + sizeof(struct Font));
	if ((fontbase[n]->nwfont & BMASK) > norig)
		error(FATAL, "Font %s too big for position %d\n", s, n);
	close(fin);
	nw = fontbase[n]->nwfont & BMASK;
	widthtab[n] = (char *) fontbase[n] + sizeof(struct Font);
	codetab[n] = (char *) widthtab[n] + 2 * nw;
	fitab[n] = (char *) widthtab[n] + 3 * nw;
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
	fprintf(stderr, "d8400: ");
	fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (f)
		done();
}


/*
	Here beginneth all the stuff that really depends
	on the 8400 (we hope).
*/


char	devname[20]	= "8400";

#define	START_TAKE	0x80
#define	END_TAKE	0x81
#define	INIT_REV	0x85
#define	POINT_SIZE	0x87
#define	SET_SIZE	0x88	/* means horizontal width indep of ht */
#define	VERT_FWD	0x89
#define	VERT_REV	0x8a
#define	HOR_POS		0x8b
#define	SLANT_MODE	0x8f
#define	SEL_FACE	0x96	/* select font, probably */
#define	FONT_NUMBER	0x99
#define	HOR_RIGHT	0x9b
#define	HOR_LEFT	0x9c

#define	RES	1728	/* 8400 is ??? goobies per inch vertically */
			/* but 972 horizontally! */
			/* can get 486 vertically by using electronic baseline jump */
#define	TRAILER	(14 * res)
#define	LMARGIN	0	/* left margin offset */
#define	HMAX	(48 * (res/6))	/* maximum horizontal size = 48 picas */
#define	VMAX	(15 * res)	/* 15 inch page */

int	lastc, lastw;	/* last character and width (maybe not used) */

long	paper;		/* paper used */

t_init(reinit)	/* initialize device */
int reinit;
{
	int i;

	hpos = vpos = oldv = oldh = 0;
	/* the above are not true until the code below happens*/
	setsize(t_size(10));	/* start somewhere */
	setfont(1);

	for (i = 0; i < 10; i++)	/* reset sequence */
		putc(0, tf);
	putcmd(START_TAKE, 0);	/* resets device */
	putcmd(INIT_REV, 0);	/* sets up for reverse vertical */

	for (i = 0; i < nchtab; i++)
		if (strcmp(&chname[chtab[i]], "l.") == 0)
			break;
	if (i < nchtab) {
		drawdot = i + 128;
		drawsize = 1;
	} else {
		drawdot = '.';
		drawsize = 2;	/* half size */
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
	hpos = 0;	/* because we're now back at the left margin */
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
	setsize(n);		/* first set the size */
	putcmd(SET_SIZE, pstab[size-1]);	/* then restore horizontal */
}

t_slant(n)	/* set slant to n */
int n;
{
	putcmd(SLANT_MODE, n);
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
	fflush(tf);
}

char *tracct = 	"/usr/adm/tracct";

account()	/* record paper use */
{
	FILE *f = NULL;
	char *name, *getlogin();

	if (tf == stdout)
		return;
	f = fopen(tracct, "a");
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

hflush()	/* actual horizontal output occurs here */
{
	register int dh;
	int htrue;

	if (!output)
		return;
	htrue = hpos + horig;
	dh = htrue - oldh;
	if (dh > 0) {
		if (dbg)
			printf(" h> %d\n", dh & BMASK);
		else
			putcmd(HOR_RIGHT, dh);	
	} else if (dh < 0) {
		if (dbg)
			printf(" h<- %d\n", dh);
		else
			putcmd(HOR_LEFT, dh);
	}
	oldh = htrue + horig;
}

vgoto(n)
{
	int dv, vtrue;

	if (!output)
		return;
	if (dbg) printf(" vgoto %d to %d\n", vpos, n);
	vtrue = vpos + vorig;
	dv = n - vtrue;
	if (dv > 0) {
		if (dbg)
			printf(" v down %d\n", dv);
		else
			putcmd(VERT_FWD, dv);
	} else if (dv < 0) {
		if (dbg)
			printf("v up %d\n", dv);
		else
			putcmd(VERT_REV, dv);
	}
	vpos = n;
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
	hflush();
	if (dbg) {
		if (isprint(c+32))
			printf("%c %d\n", c+32, code);
		else
			printf("%03o %d\n", c+32, code);
	} else
		putc(code, tf);
		/* SHOULD INTEGRATE THIS WITH HORIZONTAL MOTION */

	if (font != ofont)
		setfont(ofont);
	lastw = pw[i] & 077;
	lastw = (lastw * pstab[size-1] + dev.unitwidth/2) / dev.unitwidth;
}

setsize(n)	/* set point size to n (internal) */
int n;
{

	if (!output)
		return;
	if (n == size)
		return;	/* already there */
	/* for now, only cope with integer sizes */
	putcmd(POINT_SIZE, pstab[n-1]);
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
	if (n < 0 || n > NFONT)
		error(FATAL, "illegal font %d\n", n);
	putcmd(FONT_NUMBER, fontname[n].number);
	font = n;
}

putcmd(n1, n2)
{
	putc(n1, tf);
	putc(((n2>>8) & 0177) | 0200, tf);	/* always turn on */
	putc(n2 & 0377, tf);
}
