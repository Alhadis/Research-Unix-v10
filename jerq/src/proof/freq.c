#define	DEV	0
#define	COMMENT	1
#define	ENDLINE	2
#define	PAGE	3
#define	RELV	4
#define	ABSV	5
#define	WORD	6
#define	RELH	7
#define	ABSH	8
#define	FONT	9
#define	PSIZE	10
#define	DRAW	11
#define	TEXT	12
#define	TWOCHAR	13
#define	ONECHAR	14
#define	TWONCHAR	15
#define	WSPACE	16

int cnt[200], tot[200];
char *mesg[] =
{
	"dev control",
	"comments",
	"endline",
	"page",
	"rel ver",
	"abs ver",
	"new word",
	"rel horiz",
	"abs horiz",
	"change font",
	"set size",
	"draw cmd",
	"text cmd",
	"multiple char",
	"single char",
	"nnCHAR",
	"white space",
	0
};
/*
 *	drive 202 typesetter
 *	This will probably run the 202 faster if you
 *	modify setfont and put1 to actually send out
 *	a font change command when needed, instead of
 *	after every special character.
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
#define	N_OLIST	100
int	olist[N_OLIST];	/* pairs of page numbers */
int	spage	= 9999;	/* stop every spage pages */
int	scount	= 0;
int	xoffset	= 0;	/* units are typesetter goobies! */

struct	dev	dev;
struct font *fontbase[NFONT+1];
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

char	*typesetter	= "/dev/202";
FILE	*tf	= NULL;	/* output file */
char	*fontdir	= "/usr/lib/font";

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

	conv(stdin);
	dumpstat();
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
			tot[WSPACE]++;
			cnt[WSPACE]++;
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			/* two motion digits plus a character */
			getc(fp);
			getc(fp);
			cnt[TWONCHAR]++;
			tot[TWONCHAR] += 3;
			break;
		case 'c':	/* single ascii character */
			getc(fp);
			cnt[ONECHAR]++;
			tot[ONECHAR] += 2;
			break;
		case 'C':
			tot[TWOCHAR] += sucks(fp)+1;
			cnt[TWOCHAR]++;
			break;
		case 't':	/* straight text */
			tot[TEXT] += sucknl(fp)+1;
			cnt[TEXT]++;
			break;
		case 'D':	/* draw function */
			tot[DRAW] += sucknl(fp)+1;
			cnt[DRAW]++;
			break;
		case 's':
			tot[PSIZE] += suckd(fp)+1;
			cnt[PSIZE]++;
			break;
		case 'f':
			tot[FONT] += sucks(fp)+1;
			cnt[FONT]++;
			break;
		case 'H':	/* absolute horizontal motion */
			tot[ABSH] += suckd(fp)+1;
			cnt[ABSH]++;
			break;
		case 'h':	/* relative horizontal motion */
			tot[RELH] += suckd(fp)+1;
			cnt[RELH]++;
			break;
		case 'w':	/* word space */
			cnt[WORD]++;
			tot[WORD]++;
			break;
		case 'V':
			tot[ABSV] += suckd(fp)+1;
			cnt[ABSV]++;
			break;
		case 'v':
			tot[RELV] += suckd(fp)+1;
			cnt[RELV]++;
			break;
		case 'p':	/* new page */
			tot[PAGE] += suckd(fp)+1;
			cnt[PAGE]++;
			break;
		case 'n':	/* end of line */
			tot[ENDLINE] += sucknl(fp)+1;
			cnt[ENDLINE]++;
			break;
		case '#':	/* comment */
			tot[COMMENT] += sucknl(fp)+1;
			cnt[COMMENT]++;
			break;
		case 'x':	/* device control */
			cnt[DEV]++;
			tot[DEV] += sucknl(fp)+1;
			break;
		default:
			break;
		}
	}
}

dumpstat()
{
	register i;
	int total = 0;

	for(i = 0; mesg[i]; i++)
		total += tot[i];
	printf("total chars = %d\n", total);
	for(i = 0; mesg[i]; i++)
	{
		printf("%20s%8d@%4.1f%8d(%.1f%%)\n", mesg[i], cnt[i],
			cnt[i]?(float)tot[i]/cnt[i]:0.0, tot[i],
			tot[i]*100.0/total);
	}
}

suckd(fp)
	FILE *fp;
{
	register i, c;

	i = 0;
	while((c = getc(fp)) == ' ')
		i++;
	while(isdigit(c))
	{
		i++;
		c = getc(fp);
	}
	ungetc(c, fp);
	return(i);
}

sucknl(fp)
	FILE *fp;
{
	register i, c;

	for(i = 1; (c = getc(fp)) != '\n'; i++)
		if(c == EOF) break;
	return(i);
}

sucks(fp)
	FILE *fp;
{
	register i, c;

	for(i = 0; (c = getc(fp)) != ' '; i++)
		if(c == '\n') break;
	ungetc(c, fp);
	return(i);
}
