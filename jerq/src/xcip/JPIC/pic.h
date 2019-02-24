#define	HEAD1	1
#define	HEAD2	2
#define	HEAD12	(HEAD1+HEAD2)
#define	INVIS	4
#define	CW_ARC	8	/* clockwise arc */
#define	dprintf	if(dbg)printf
#define	PI	3.141592654
#define	PI2	PI/2
#define	SCALE	200	/* default scale: units/inch */
#define	WID	150	/* default width for boxes and ellipses */
#define	HT	100	/* default height and line length */
#define	MAXOBJ	10000
#define	MAXTEXT	2000
#define	SYMTAB	2000
#define	DEV202	1
#define	DEVAPS	2
#define	DEVCAT	3
#define	DEV450	4

/* these have to be like so, so that we can write */
/* things like R & V, etc. */
#define	H	0
#define	V	1
#define	R_DIR	0
#define	U_DIR	1
#define	L_DIR	2
#define	D_DIR	3
#define	ishor(n)	(((n) & V) == 0)
#define	isvert(n)	(((n) & V) != 0)
#define	isright(n)	((n) == R_DIR)
#define	isleft(n)	((n) == L_DIR)
#define	isdown(n)	((n) == D_DIR)
#define	isup(n)		((n) == U_DIR)

typedef	int	coord;

struct attr {	/* attribute of an object */
	int	a_type;
	int	a_val;
};

struct obj {	/* stores various things in variable length */
	int	o_type;
	int	o_count;	/* number of things */
	int	o_nobj;		/* index in objlist */
	int	o_mode;		/* hor or vert */
	coord	o_x;	/* coord of "center" */
	coord	o_y;
	int	o_nt1;	/* 1st index in text[] for this object */
	int	o_nt2;	/* 2nd; difference is #text strings */
	int	o_attr;	/* various attributes of interest */
	int	o_dotdash;	/* kludge in a dot/dash mode */
	int	o_ddval;	/* value of dot/dash expression */
	int	o_val[1];	/* actually this will be > 1 in general */
};

struct symtab {
	char	*s_name;
	int	s_type;
	int	s_val;
	struct symtab *s_next;
};

struct text {
	int	t_type;
	char	*t_val;
};

#define	dprintf	if(dbg)printf
extern	int	dbg;
extern	struct	obj	*objlist[];
extern	int	nobj;
extern	struct	attr	attr[];
extern	int	nattr;
extern	struct	text	text[];
extern	int	ntext;
extern	int	ntext1;
extern	coord	curx, cury;
extern	int	hvmode;
extern	int	codegen;
extern	struct	obj	*makenode();
extern	char	*malloc();
extern	struct symtab *lookup(), *makevar();

extern	float	deltx, delty, scale;
extern	int	lineno;
extern	int	synerr;
extern	int	crop;

extern	int	devtype, res, DX, DY;

extern	coord	sxmin, sxmax, symin, symax;
extern	coord	xmin, ymin, xmax, ymax;
extern	double	atof();

struct pushstack {
	coord	p_x;
	coord	p_y;
	int	p_hvmode;
	coord	p_xmin;
	coord	p_ymin;
	coord	p_xmax;
	coord	p_ymax;
	struct symtab *p_symtab;
};
extern	struct pushstack stack[];
extern	int	nstack;
