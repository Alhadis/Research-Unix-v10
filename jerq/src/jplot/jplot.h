#ifdef MYDIR
#define JPLOT "/usr/llc/plot/5620/jplot"
#else
#define	JPLOT	"/usr/jerq/mbin/jplot"
#endif
#define MAXCOUNT 20
#define void int
#define cmd_sent()	{ if(counter++ > MAXCOUNT){counter = 0;graphic(TOK); flush(); t = term();}}

extern int	mpx;			/* 0 if standalone, 1 if mpx */
extern int	wantready;	/* 0 if blast ahead, 1 if wait till READY */
extern int	jerq;			/* fd to jerq tty */
extern int 	counter;
extern int	t;
void
	alpha(),		/* output a character in alphanumeric mode */
	arc(),			/* draw an arc */
	box(),			/* draw an box */
	circle(),		/* draw a circle */
	closepl(),		/* close the plot */
	vec(),			/* draw to a point */
	point(),			/* draw a dot */
	erase(),		/* erase the screen */
	finish(),		/* flush buffer, turn off READYs */
	graphic(),		/* output a character in graphics mode */
	text(),		/* output text */
	line(),			/* draw a line */
	move(),			/* move to a point */
	openpl(),		/* open a plot */
	start(),		/* turn on READYs */
	xysc()			/* scale, pack and output x and y coordinates */
;

extern struct penvir {
double
	bottom,			/* screen bottom y */
	left,			/* screen bottom x */
	ymin,			/* user's bottom y */
	xmin,			/* user's bottom x */
	scalex,			/* scale factor x */
	scaley,			/* scale factor y */
	sidex,			/* length of screen x */
	sidey,			/* length of screen y */
	grade,
	quantum,
	copyx,
	copyy
;	
int pen;
int *texture;
} *e1, *esave, *e0;
#define unorm(y)	(e1->ymin + (y-e1->bottom)/e1->scaley)
#define SCX(x)	 ((x-e1->xmin)*e1->scalex+e1->left)
#define SCY(y)	((y-e1->ymin)*e1->scaley+e1->bottom)
extern int t1[], t0[];
struct seg {
	int x, y, X, Y;
};
