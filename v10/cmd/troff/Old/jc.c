/*
 *	drive jerq terminal.
 *	not many sizes and fonts right now
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
#include	<signal.h>
#include	<math.h>
#include	<ctype.h>
#include	<sgtty.h>
#include "dev.h"
#define	NFONT	10

struct sgttyb sttymodes, sttysave, sttychars;
int xx, yy;
int	xoffset	= 0;	/* shift output right by this amount */
int	yoffset = 0;	/* shift down (!) by this amount */
int	output	= 0;	/* do we do output at all? */
int	nolist	= 0;	/* output page list if > 0 */
int	olist[20];	/* pairs of page numbers */

int	erase	= 1;
float	aspect	= 1;	/* default aspect ratio */
int	revmode	= 0;	/* reverse video if set */
int	(*sigint)();
int	(*sigquit)();
int	jtcalled = 0;	/* 1 if managed to call jt */

struct dev dev;
struct font *fontbase[NFONT+1];
short	*pstab;
int	nsizes	= 1;
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
int	Xflag	= 0;
long	lineno	= 0;
int	res	= 972;		/* input assumed computed according to this resolution */
				/* initial value to avoid 0 divide */
#define	JERQRES	100.0		/* resolution of jerq */
FILE	*tf	= NULL;		/* output file pointer */
char	outfile[20];
char	*fontdir	= "/usr/lib/font";
extern char devname[];

FILE *fp	= stdin;	/* input file pointer */

main(argc, argv)
char *argv[];
{
	char buf[BUFSIZ];
	int done();
	char *getenv(), *jerq;

	jerq = getenv("JERQ");
	if (jerq == NULL) {
		error(!FATAL, "JERQ parameter not set\n");
		exit(1);
	}
	sprintf(outfile, "/dev/%s", jerq);
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 't':
			tf = stdout;
			break;
		case 'x':
			xoffset = atoi(&argv[1][2]);
			break;
		case 'y':
			yoffset = atoi(&argv[1][2]);
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
			tf = stdout;
			break;
		case 'j':
			outfile[8] = argv[1][2];	/* kludge */
			break;
		case 'X':	/* use experimental version */
			Xflag++;
			break;
		}
		argc--;
		argv++;
	}

	if (tf == NULL && (tf = fopen(outfile, "w")) == NULL)
		error(FATAL, "can't open terminal %s", outfile);
	if (tf != stdout) {
		pushmode();
		jtcalled++;
		if (Xflag)
			system("68ld /usr/bwk/troff/devjerq/jt");
		else
			system("68ld /usr/jerq/lib/jt");
	}
	sigint = signal(SIGINT, done);
	sigquit = signal(SIGQUIT, SIG_IGN);
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

pushmode()
{
	ioctl(1, TIOCGETP, &sttymodes);
	sttysave=sttymodes;
	sttymodes.sg_flags|=RAW;
	sttymodes.sg_flags&=~ECHO;
	ioctl(1, TIOCSETP, &sttymodes);
}

popmode()
{
	ioctl(1, TIOCSETP, &sttysave);
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
	strcpy(devname, "202");	/* this is the only char set we have */
				/* the resolution, etc., is already in */
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
	for (i = 0; i <= nfonts; i++) {
		fontbase[i] = NULL;
		widthtab[i] = codetab[i] = fitab[i] = NULL;
	}
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
	if (fontbase[n] != NULL && strcmp(s, fontbase[n]->namefont) == 0)
		return;
	if (s1 == NULL || s1[0] == '\0')
		sprintf(temp, "%s/dev%s/%s.out", fontdir, devname, s);
	else
		sprintf(temp, "%s/%s.out", s1, s);
	if ((fin = open(temp, 0)) < 0) {
		error(!FATAL, "can't open font table %s", temp);
		return;
	}
	if (fontbase[n] != NULL)
		free(fontbase[n]);
	fontbase[n] = (struct font *) malloc(3*255 + dev.nchtab +
				(128-32) + sizeof(struct font));
	if (fontbase[n] == NULL)
		error(FATAL, "Out of space in loadfont %s\n", s);
	read(fin, fontbase[n], 3*255 + nchtab+128-32 + sizeof(struct font));
	close(fin);
	if (smnt == 0 && fontbase[n]->specfont == 1)
		smnt = n;
	nw = fontbase[n]->nwfont & BMASK;
	widthtab[n] = (char *) fontbase[n] + sizeof(struct font);
	codetab[n] = (char *) widthtab[n] + 2 * nw;
	fitab[n] = (char *) widthtab[n] + 3 * nw;
	t_fp(n, fontbase[n]->namefont, fontbase[n]->intname);
	if (dbg > 1) fontprint(n);
}

#define	MINY	0
#define	MAXY	(1023-10)
#define	MAXX	800

error(f, s, a1, a2, a3, a4, a5, a6, a7) {
	if (dbg) {
		fprintf(stderr, "jc: ");
		fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
		fprintf(stderr, " near line %ld\n", lineno);
	}
	if (f)
		done(2);
}


/*
	Here beginneth all the stuff that really depends
	on the jerq (we hope).
*/


char	devname[20]	= "jerq";

#define	oput(c)	if (output) xychar(xx, yy, c); else;

int	stopped	= 0;
int	size	= 1;
int	font	= 1;		/* current font */
int	hpos;		/* horizontal position where we are supposed to be next (left = 0) */
int	vpos;		/* current vertical position (down positive) */

int	horig;		/* h origin of current block; hpos rel to this */
int	vorig;		/* v origin of current block; vpos rel to this */

int	DX	= 8;	/* step size in x for drawing */
int	DY	= 8;	/* step size in y for drawing */
int	drawdot	= '.';	/* draw with this character */
int	drawsize = 1;	/* shrink by this factor when drawing */

t_init(reinit)	/* initialize device */
int reinit;
{
	int i;

	if (! reinit) {
		fprintf(tf, "i\n");
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
	fflush(tf);
	stopped = 0;
	if (erase) {
		fprintf(tf, "e\n");
		fflush(tf);
	}
	hpos = vpos = 0;
	setsize(t_size(10));	/* start somewhere */
	sendpt();
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
int	pgnum[100];	/* their actual numbers */
long	pgadr[100];	/* their seek addresses */

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
	vgoto(11 * res - 100);
	sendpt();
	fflush(tf);

  next:
	for (bp = buf; (*bp = readch()); )
		if (*bp++ == '\n')
			break;
	*bp = 0;
	switch (buf[0]) {
	case '\004':
	case '\177':
	case 0:
		done();
		break;
	case '\n':
		if (stopped)
			done();
		output = in_olist(n);
		t_init(1);
		return;
	case '!':
		callunix(&buf[1]);
		fputs("!\n", stderr);
		break;
	case 'e':
		erase = 1 - erase;
		if (erase) {
			putc('e', tf);
			fflush(tf);
		}
		break;
	case 'r':
		putc('r', tf);
		fflush(tf);
		break;
	case 'a':
		aspect = atof(&buf[1]);
		break;
	case 'x':
		xoffset = atoi(&buf[1]);
		break;
	case 'y':
		yoffset = atoi(&buf[1]);
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
	case 'q':
		done();
		break;
	case '?':
		fputs("!cmd	unix cmd\n", stderr);
		fputs("p	print this page again\n", stderr);
		fputs("-n	go back n pages\n", stderr);
		fputs("n	print page n (previously printed)\n", stderr);
		fputs("o...	set the -o output list to ...\n", stderr);
		fputs("en	n=0 -> don't erase; n=1 -> erase\n", stderr);
		fputs("an	set aspect ratio to n\n", stderr);
		fputs("xn	set x offset to n (+ => right)\n", stderr);
		fputs("yn	set y offset to n (+ => down)\n", stderr);
		break;
	default:
		fputs("?\n", stderr);
		break;
	}
	goto next;
}

t_newline()	/* do whatever for the end of a line */
{
	hpos = 0;
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
	/* punt for now */
}

t_slant(n)	/* set slant to n */
int n;
{
	/* punt for now */
}

t_font(s)	/* convert string to internal font number */
char *s;
{
	int n;

	n = atoi(s);
	if (n < 1 || n > nfonts)
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
	w = res / 2 * pstab[size-1] / 72;
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
	fflush(tf);
	if (c == 's') {
		stopped = 1;
		t_page(9999);
	}
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
	if (output)
		sendpt();
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
		oput(code);
	if (font != ofont)
		setfont(ofont);
}

setsize(n)	/* set point size to n (internal) */
int n;
{

	if (!output)
		return;
	if (n == size)
		return;	/* already there */
	putc('s', tf);
	putint(pstab[n-1]);
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
	putc('f', tf);
	/* putint(fontname[n].number); */
	putint(n);	/* for now */
	font = n;
}

done()
{
	output = 1;
	hgoto(0);
	vgoto(11 * res - 100);	/* bottom of page */
	sendpt();
	fflush(tf);
	if (jtcalled)
		system("run - /usr/jerq/lib/term");
	popmode();
	exit(0);
}

callunix(line)
char line[];
{
	int rc, status, unixpid;
	if( (unixpid=fork())==0 ) {
		signal(SIGINT,sigint); signal(SIGQUIT,sigquit);
		close(0); close(1); dup(2); dup(2);
		execl("/bin/sh", "-sh", "-c", line, 0);
		exit(255);
	}
	else if(unixpid == -1)
		return;
	else{	signal(SIGINT, SIG_IGN); signal(SIGQUIT, SIG_IGN);
		while( (rc = wait(&status)) != unixpid && rc != -1 ) ;
		signal(SIGINT, done); signal(SIGQUIT,sigquit);
	}
}
readch(){
	char c;
	if (read(2,&c,1)<1 || (c == '\004') || (c == '\177')) c=0;
	return(c);
}

scalept(n)
{
	((float)(n) * JERQRES / res) * aspect + 0.5;
}

sendpt()
{
	float fx, fy;

	fx = hpos + horig;
	fy = vpos + vorig;
	xx = fx * JERQRES / res * aspect + 0.5;
	xx += xoffset;
	yy = fy * (MAXY-MINY) / (11 * res) + MINY + 0.5;
	yy += yoffset;
}

xychar(x,y,c)
{
	static int lasty = 0;
	static int lastx = 0;

	if (y != lasty) {
		putc('y', tf);
		putint(y);
		lasty = y;
	}
	/* optimization? */
	if (x-lastx >= 0 && x-lastx <= 255 && c != '0') {
		putc('X', tf);
		putc(x-lastx, tf);
		putc(c, tf);
	} else {
		putc('x', tf);
		putint(x);
		if (c) {
			putc('c', tf);
			putc(c, tf);
		}
	}
	lastx = x;
}

putint(n)
{
	if (dbg) {
		printf("%02d\n", n);
		return;
	}
	putc(n>>8, tf);
	putc(n, tf);
}
