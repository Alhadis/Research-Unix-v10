/*
 *	dsort
 *
 *	sort troff output into troff output that only goes one
 *	direction down the page.
 *	caveat user:
 *		there are bugs herein
 *		it doesn't do anything with graphics like \D'...'
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

#define	FATAL	1
int	dbg	= 0;
int	res;		/* input assumed computed according to this resolution */
int	size	= 0;	/* current size */
int	font	= 0;	/* current font */
int	hpos;		/* horizontal position where we are supposed to be next (left = 0) */
int	vpos;		/* current vertical position (down positive) */
int	horig;		/* h origin of current block; hpos rel to this */
int	vorig;


main(argc, argv)
char *argv[];
{
	FILE *fp;
	int i;
	int done();
	extern int obufsize;

	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'd':
			dbg = atoi(&argv[1][2]);
			if (dbg == 0) dbg = 1;
			obufsize = 50;
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

/* new data declarations go here */
struct vlist {
	int	v;
	int	h;
	int	s;
	int	f;
	char	*p;
};

#define	NVLIST	1500
struct	vlist	vlist[NVLIST + 1];
struct	vlist	*vlp	= vlist;
int	nvlist	= 0;
#define	OBUFSIZE	32000
int	obufsize	= OBUFSIZE;
#define	SLOP		500
char	obuf[OBUFSIZE + SLOP];
char	*op	= obuf;

conv(fp)
register FILE *fp;
{
	register int c, k;
	int m, n, i, n1, m1;
	char str[100], buf[300];

	while ((c = getc(fp)) != EOF) {
		if(dbg)fprintf(stderr, "%c i=%d V=%d\n", c, op-obuf, vpos);
		pause();
		if (op >= obuf + obufsize)
			oflush();
		switch (c) {
		case '\n':	/* when input is text */
		case ' ':
			*op++ = c;
			break;
		case '{':	/* push down current environment */
			*op++ = c;
			t_push();
			break;
		case '}':
			*op++ = c;
			t_pop();
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			/* two motion digits plus a character */
			*op++ = c;
			*op++ = getc(fp);
			hmot((op[-2]-'0') * 10 + op[-1] - '0');
			*op++ = getc(fp);
			break;
		case 'c':	/* single ascii character */
			*op++ = c;
			*op++ = getc(fp);
			break;
		case 'C':
			*op++ = c;
			while ((c = getc(fp)) != ' ' && c != '\n')
				*op++ = c;
			ungetc(c, fp);
			break;
		case 't':	/* straight text */
			*op++ = c;
			fgets(op, SLOP, fp);
			op += strlen(op);
			break;
		case 'D':	/* draw function */
			*op++ = c;
			fgets(op, SLOP, fp);
			switch (*op) {
			case 'l':	/* draw a line */
				sscanf(op+1, "%d %d", &n, &m);
				hmot(n);
				break;
			case 'c':	/* circle */
				sscanf(op+1, "%d", &n);
				hmot(n);
				break;
			case 'e':	/* ellipse */
				sscanf(op+1, "%d %d", &m, &n);
				hmot(m);
				break;
			case 'a':	/* arc */
				sscanf(op+1, "%d %d %d", &n, &m, &n1);
				hmot(n);
				break;
			case '~':	/* wiggly line */
				hmot(0);	/* bug */
				break;
			default:
				error(FATAL, "unknown drawing function %s\n", buf);
				break;
			}
			op += strlen(op);	/* skip over new stuff */
			break;
		case 's':
			*op++ = c;
			fscanf(fp, "%s", op);	/* ignore fractional sizes */
			size = atoi(op);
			op += strlen(op);
			break;
		case 'f':
			*op++ = c;
			fscanf(fp, "%s", op);
			font = atoi(op);
			op += strlen(op);
			break;
		case 'H':	/* absolute horizontal motion */
			/* fscanf(fp, "%d", &n); */
			*op++ = c;
			while ((c = getc(fp)) == ' ')
				;
			k = 0;
			do {
				*op++ = c;
				k = 10 * k + c - '0';
			} while (isdigit(c = getc(fp)));
			ungetc(c, fp);
			hgoto(k);
			break;
		case 'h':	/* relative horizontal motion */
			/* fscanf(fp, "%d", &n); */
			*op++ = c;
			while ((c = getc(fp)) == ' ')
				;
			k = 0;
			do {
				*op++ = c;
				k = 10 * k + c - '0';
			} while (isdigit(c = getc(fp)));
			ungetc(c, fp);
			hmot(k);
			break;
		case 'w':
			break;
		case 'V':
			*op++ = 0;
			if (vlp >= vlist + NVLIST) {
				oflush();
			}
			vlp->p = op;
			*op++ = c;
			fscanf(fp, "%s", op);
			n = atoi(op);
			vgoto(n);
			op += strlen(op);
			vlp->v = vpos;
			vlp->h = hpos;
			vlp->s = size;
			vlp->f = font;
			vlp++;
			nvlist++;
			break;
		case 'v':
			*op++ = c;
			fscanf(fp, "%d", &n);
			vmot(n);
/* punt for now, since never occurs */
			break;
		case 'p':	/* new page */
			vpos = 0;
			oflush();
			*op++ = c;
			fscanf(fp, "%s", op);
			op += strlen(op);
			break;
		case 'n':	/* end of line */
			*op++ = c;
			while ((*op++ = getc(fp)) != '\n')
				;
			hpos = 0;
			break;
		case '#':	/* comment */
			*op++ = c;
			while ((*op++ = getc(fp)) != '\n')
				;
			break;
		case 'x':	/* device control */
			oflush();
			putchar(c);
			fgets(buf, sizeof buf, fp);
			fputs(buf, stdout);
			fflush(stdout);
			break;
		default:
			error(!FATAL, "unknown input character %o %c\n", c, c);
			done();
		}
	}
}

pause(i, j, k)
{
}

oflush()	/* sort, then dump out contents of obuf */
{
	char *p;
	struct vlist *vp;
	int i;
	int compar();

	if(dbg)fprintf(stderr, "into oflush, V=%d\n", vpos);
	if (op == obuf)
		return;
 	qsort((char *) vlist, nvlist, sizeof (struct vlist), compar);
	*op++ = 0;
	vp = vlist;
	printf("V%d\n",  vp->v);
	for (i = 0; i < nvlist; i++, vp++) {
		printf("H%ds%df%d\n", vp->h, vp->s, vp->f);
		for (p = vp->p; *p != 0; p++)
			putchar(*p);
	}
	fflush(stdout);
	vlp = vlist;
	vlp->p = op = obuf;
	sprintf(op, "V%dH%d\n", vpos, hpos);
	op += strlen(op);
	vlp->h = hpos;
	vlp->v = vpos;
	vlp->s = size;
	vlp->f = font;
	*op = 0;
	vlp++;
	nvlist = 1;
}


compar(p1, p2)
struct vlist *p1, *p2;
{
	return(p1->v - p2->v);
}

done()
{
	oflush();
	exit(0);
}

error(f, s, a1, a2, a3, a4, a5, a6, a7) {
	fprintf(stderr, "dsort: ");
	fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (f)
		done();
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

	vpos = 0;
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

vgoto(n)
{
	vpos = n;
}

vmot(n)	/* generate n units of vertical motion */
int n;
{
	vgoto(vpos + n);	/* ignores rounding */
}
