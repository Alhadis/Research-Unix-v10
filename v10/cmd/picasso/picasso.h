/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:picasso.h	1.0	*/

#include <stdio.h>
#include <math.h>
/* Research hack */
#define M_PI PI
#define M_PI_2 PI/2.0
#define M_1_PI 1.0/PI
#define M_SQRT2 1.41421
#define M_SQRT1_2 .7071085
/* End of Research hack */

#define	DEFAULT	0
#define	MINRAD	3/pgscale	/* slop for box corners, ellipse major/minor */
				/* (should probably be a predefined variable)*/
#define	HEAD1	  1
#define	HEAD2	  2
#define	HEAD12	(HEAD1+HEAD2)
#define	HEADFILL  4
#define	CW_ARC	  8			/* clockwise arc */
#define	DOTBIT	 16
#define	DASHBIT	 32
#define	DOTDASH	(DOTBIT+DASHBIT)	/* i.e., PostScript dash pattern */
					/* (implementation not yet done) */ 
#define	EDGED	 64
#define	FILLED	128
#define	LINECAP	256
#define	JOIN	1024
#define	MITER	4096

#define	CENTER	 1			/* text types */
#define	LJUST	 2
#define	RJUST	 4
#define	ABOVE	 8
#define	BELOW	16
#define	EQNTXT	32

#define	GSCALE	1.0	/* default global scale: units/inch */
#define	WID	0.75	/* default width for boxes and ellipses */
#define	WID2	0.375
#define	HT	0.5	/* default height and line length */
#define	HT2	0.25	/* because no floating init exprs! */
#define	HT5	0.1
#define	HT10	0.05

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

typedef union {
	long		i;
	float		f;
	float		*a;	/* array of values; dimension kept in symtab */
	char		*p;
	struct	obj	*o;
	struct	symtab	*s;
} valtype;

typedef struct obj {	/* stores various things in variable length */
	struct	 obj	*o_next;	/* forward link in object list	    */
	struct	 obj	*o_prev;	/* back link			    */
	struct	 obj	*o_parent;	/* parent block or place reference  */
	short		o_layer;	/* "draft" layers are < 0	    */ 
	unsigned short	o_size;
	unsigned short	o_type;
	unsigned short	o_nt1;		/* 1st index in text[] for object   */
	unsigned short	o_nt2;		/* 2nd; difference is #text strings */
	float		o_x;		/* coordinates of object's "center" */
	float		o_y;
	float		o_text;		/* color attribute for any labels   */
	valtype		o_val[1];	/* actually, > 1 in general	    */
} obj;

#define    N_VAL	14		/* base o_val size [xform, attrs]   */
#define	   o_xform	o_val
#define    o_mxx	o_val[ 0].f
#define    o_myx	o_val[ 1].f
#define    o_mxy	o_val[ 2].f
#define    o_myy	o_val[ 3].f
#define    o_mxt	o_val[ 4].f
#define    o_myt	o_val[ 5].f
#define    o_wid	o_val[ 7].f
#define    o_ht		o_val[ 8].f
#define    o_attr	o_val[ 9].i	/* attribute flags and counts here. */
#define    o_ddpat	o_val[10]
#define    o_weight	o_val[11].f
#define    o_color	o_val[12].f
#define    o_fill	o_val[13].f

struct	objattr	{			/* common attributes of objects */
		short	a_flags;
		short	a_layer;
		float	a_ht;
		double	a_wid;
		double	a_rad;
		double	a_weight;
		double	a_lcolor;
		double	a_pcolor;
		double	a_tcolor;
		valtype	a_dashpat;
};

typedef	valtype	YYSTYPE;			/* the yacc stack type	*/

extern	YYSTYPE	yylval, yyval;

struct symtab {
	char		*s_name;
	unsigned short	s_type;
	unsigned short	s_dim;		/* actually, dim-1; i.e. max index */
	valtype		s_val;
	struct	symtab	*s_next;
};

typedef struct {	/* attribute of an object */
	int	a_type;
	int	a_sub;
	valtype	a_val;
} Attr;

typedef struct {
	short	t_type;		/* CENTER, LJUST, etc. */
	short	t_font;
	short	t_size;
	short	t_space;
	short	t_line;
	float	t_width;
	char	*t_val;
} Text;

typedef	struct {
	float	r;
	float	g;
	float	b;
} rgb;

#define	pString	01
#define	Macro	02
#define	File	04
#define	Char	010
#define	Thru	020
#define	Free	040

typedef struct {	/* input source */
	int	type;	/* Macro, pString, File */
	char	*sp;	/* if pString or Macro */
} Src;

extern	Src	src[], *srcp;	/* input source stack */

typedef struct {
	FILE	*fin;
	char	*fname;
	int	lineno;
} Infile;

extern	Infile	infile[], *curfile;

#define	MAXARGS	20
typedef struct {	/* argument stack */
	char	*argstk[MAXARGS];	/* pointers to args */
	char	*argval;	/* points to space containing args */
} pArg;

extern	obj	*objhead, *objtail, *cur_block;;
extern	Attr	*attr;
extern	int	nattr, nattrlist;
extern	Text	*text;
extern	int	ntextlist;
extern	int	ntext, ntext1, ntextlines;
extern	float	*exprlist;
extern	int	nexpr;
extern	int	nexprlist;
extern	rgb	*rgbtable;
extern	int	nrgbtable;
extern	int	nrgb;
extern	float	curx, cury;
extern	valtype	cur_xform[6];
extern	int	xdelta[8], ydelta[8];
extern	int	hvmode;
extern	int	codegen;
extern	int	PEseen;
extern	int	pass_thru;
extern	int	verbose;
extern	int	batch;
extern	char	*cmdname;
extern	char	*troffname[];
extern	float	Gbox[4];
extern	double	pgscale;

extern	char	*malloc(), *calloc(), *realloc(), *tostring(), *grow();
extern	double	getsub(), getfval(), getcomp(), getblkvar();
extern	struct	symtab *lookup(), *findvar(), *makevar(), *newvar();
extern	char	*ifstat(), *delimstr(), *sprintgen();
extern	YYSTYPE	getvar();

extern	double	Xformx(), Xformy();
extern	double	Linx(),   Liny();

extern	int	nosqueeze;
extern	int	objbuf, objcount;
extern	int	lineno;
extern	int	synerr;
extern	int	redo_gbox;

extern	obj	*leftthing(), *boxgen(), *circgen(), *arcgen();
extern	obj	*linegen(), *splinegen(), *movegen(), *textgen(), *plotgen(), *picgen();
extern	obj	*troffgen(), *rightthing(), *blockgen();
extern	obj	*makenode(), *makepos(), *fixpos(), *addpos(), *subpos();
extern	obj	*makebetween();
extern	obj	*getpos(), *gethere(), *getnth();
extern	obj	*getfirst(), *getlast(), *getblock();
extern	obj	*copypos(), *copyobj();

struct pushstack {
	float	p_x;
	float	p_y;
	int	p_hvmode;
	float	p_xmin;
	float	p_ymin;
	float	p_xmax;
	float	p_ymax;
	struct	symtab	*p_symtab;
};
extern	struct	pushstack stack[];
extern	int	nstack;
extern	int	cw;
extern	int	draftlayer;
extern	int	top_layer;

extern	double	errcheck();

extern	double	Log();
extern	double	Log10();
extern	double	Exp();
extern	double	Pow();
extern	double	Sqrt();
extern	double	Cos();
extern	double	Sin();
extern	double	Atan2();
extern	double	Maxvar ();
extern	double	Maxlist();
extern	double	Minvar ();
extern	double	Minlist();

extern	double	setvar();
extern	double	setsize();
extern	double	setattr();
extern	double	setfont();
extern	double	setarray();
extern	double	setrgbindex();
extern	double	checkcolor();
extern	double	checkfont();
extern	double	getstringwidth();
extern	double	find_axes();
extern	double	*text_bounds();
extern	char	*parse_text();
extern	obj	*print_obj();
extern	obj	*print_xform();
