/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:misc.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

extern	int	pic_compat;
extern	int	batch;

double getcomp(p, t)	/* return component of a position; these must now be */
	obj *p;		/* transformed as there is no later chance to do so. */
	int t;
{
	float	bnd[4];

	switch (t) {
	case DOTX:	return Xformx(p, 1, p->o_x, p->o_y);
	case DOTY:	return Xformy(p, 1, p->o_x, p->o_y);
	case DOTRAD:
		switch (p->o_type) {
		case SECTOR:
		case ARC:
		case BOX:
		case ARROW:
		case LINE:	return p->o_val[N_VAL].f;
		case CIRCLE:
		case ELLIPSE:	break;	/* fall through to DOTWID case */
		}
	case DOTHT:
	case DOTWID:	if (p->o_type <= TEXT) {
				get_bounds(p, bnd, 1);
				if (t == DOTHT)
					return (bnd[3] - bnd[1]);
				else
					return (bnd[2] - bnd[0]);
			}
	}
	yyerror("can't happen getcomp");
}

exprsave(f)
	double f;
{
	if (nexpr >= nexprlist)
		exprlist = (float *) grow((char *)exprlist, "exprlist",
					nexprlist += 256, sizeof(float));
	exprlist[nexpr] = f;
	return ++nexpr;
}

char *sprintgen(fmt)
	char *fmt;
{
	char buf[1000];

	sprintf (buf, fmt,  exprlist[0], exprlist[1], exprlist[2],
					 exprlist[3], exprlist[4]);
	nexpr = 0;
	free(fmt);
	return tostring(buf);
}

printexpr(f)	/* print expression for debugging */
	double f;
{
	fprintf(stderr, "%g\n", f);
}

printpos(p)	/* print position for debugging */
	obj *p;
{
	double	x, y;

	x = Xformx(p, 1, p->o_x, p->o_y);
	y = Xformy(p, 0, p->o_x, p->o_y);
	fprintf(stderr, "%g, %g\n", x, y);
}

char *tostring(s)
	register char *s;
{
	register char *p;

	p = malloc(strlen(s)+1);
	if (p == NULL) {
		yyerror("out of space in tostring on %s", s);
		exit(1);
	}
	strcpy(p, s);
	return(p);
}

obj *makepos(x, y, corner, q)	/* make a position cell */
	double	x, y;
	int	corner;
	obj	*q;
{
	obj *p;

	p = makenode(PLACE, 1, 0);
	if (q)
		p->o_parent = q;
	p->o_x = x;
	p->o_y = y;
	p->o_val[0].f = corner;
	return(p);
}

obj *makebetween(f, p1, p2)	/* make position between p1 and p2   */
	double	f;		/* again, transforms must be applied */
	obj	*p1, *p2;
{
	obj	*p;
	double	x1, y1, x2, y2;

	p = makenode(PLACE, 3, 0);
	x1 = Xformx(p1, 1, p1->o_x, p1->o_y);
	y1 = Xformy(p1, 0, p1->o_x, p1->o_y);
	x2 = Xformx(p2, 1, p2->o_x, p2->o_y);
	y2 = Xformy(p2, 0, p2->o_x, p2->o_y);
	p->o_x = x1 + f * (x2 - x1);
	p->o_y = y1 + f * (y2 - y1);
	p->o_val[0].f = f;
	p->o_val[1].o = p1;
	p->o_val[2].o = p2;
	return(p);
}

int	xdelta[8] = {1,1,1,0,-1,-1,-1,0},
	ydelta[8] = {-1,0,1,1,1,0,-1,-1};

obj *getnth(p, nth)	/* find nth point of an object */
	obj	*p;
	int	nth;
{
	float	x, y;
	int	n = nth;

	switch (p->o_type) {
	default:	if (n != 1)
				yyerror("object has only 1 point defined");
			return p;
	case ARC:	if (n > 2) {
				yyerror("arcs have only 2 points defined");
				return p;
			}
			/* else fall through to SECTOR case */
	case SECTOR:	if ((n %= 3) == 0)
				return p;
			else if (n == 1) {
				x = p->o_val[N_VAL+2].f;
				y = p->o_val[N_VAL+3].f;
			}
			else {
				x = p->o_val[N_VAL+4].f;
				y = p->o_val[N_VAL+5].f;
			}
			break;
	case BLOCK:	while (n--) {
				p = p->o_next;
				if (p->o_type <= TEXT)
					continue;
				else if (p->o_type == BLOCKEND) {
					yyerror("[] has less than %d objects",
							nth);
					p = p->o_parent;
				}
			}
			return p;	
	case BOX:	n %= 8;
			x = p->o_x + xdelta[n] * p->o_wid / 2;
			y = p->o_y + ydelta[n] * p->o_ht  / 2;
			break;
	case ARROW:
	case LINE:
	case SPLINE:	if (--n > p->o_val[N_VAL+3].f) {
				yyerror("line has less than %d points", nth);
				return p;
			}
			x = p->o_val[N_VAL + 4 + 2 * n].f;
			y = p->o_val[N_VAL + 5 + 2 * n].f;
			break;
	case CIRCLE:
	case ELLIPSE:	n %= 8;
			x = xdelta[n] * p->o_wid / 2;
			y = ydelta[n] * p->o_ht  / 2;
			if (n % 2 == 0) {
				x *= M_SQRT1_2;
				y *= M_SQRT1_2;
			}
			x += p->o_x;
			y += p->o_y;
			break;
	}
	/*  DBK--Here also there is a question of whether x and y should
	    be transformed */
	return makepos(x, y, nth, p);
}

obj *getpos(p, corner)	/* find position of point */
	obj	*p;
	int	corner;
{
	float	x, y;

	if (pic_compat && (p->o_type == CIRCLE || p->o_type == ELLIPSE) &&
		(corner == NE || corner == NW || corner == SW || corner == SE))
			return getnth(p, corner == NE ? 2 : corner == NW ? 4 :
							corner == SW ? 6 : 8);
	if (corner == -1 )
		if (pic_compat && (p->o_type == LINE || p->o_type == ARROW ||
							p->o_type == SPLINE))
			corner = START;
		else
			return p;
	whatpos(p, corner, &x, &y);
	return makepos(x,y,corner,(corner < EAST || corner > SW)? p: (obj *)0);
}

whatpos(p, corner, px, py)	/* what is the position (no side effect) */
	obj	*p;
	int	corner;
	float	*px, *py;
{
	float	x, y, bnd[4];

	if (corner >= EAST && corner <= SW) {
		get_bounds(p, bnd, 1);
		switch (corner) {
	case NE:	x = bnd[2];		y = bnd[3];	break;
	case SW:	x = bnd[0];		y = bnd[1];	break;
	case SE:	x = bnd[2];		y = bnd[1];	break;
	case NW:	x = bnd[0];		y = bnd[3];	break;
	case NORTH:	x = (bnd[0]+bnd[2])/2;	y = bnd[3];	break;
	case SOUTH:	x = (bnd[0]+bnd[2])/2;	y = bnd[1];	break;
	case EAST:	x = bnd[2];	y = (bnd[1]+bnd[3])/2;	break;
	case WEST:	x = bnd[0];	y = (bnd[1]+bnd[3])/2;	break;
		}
	}
	else {	/* DBK--note that get_bounds returns the transformed position,
		   but the following calculations do not!! */
		x = p->o_x;
		y = p->o_y;

/* futz around for special cases: */

		switch (p->o_type) {
	case BLOCK:	if (corner == START)
				whatpos(p->o_next, START, &x, &y);
			else if (corner == END)
				whatpos(p->o_val[N_VAL+1].o->o_prev,END,&x,&y);
			break;
	case ARC:	if (corner == START) {
				x = p->o_val[N_VAL+2].f;
				y = p->o_val[N_VAL+3].f;
			}
			else if (corner == END)	{
				x = p->o_val[N_VAL+4].f;
				y = p->o_val[N_VAL+5].f;
			}
			break;
	case LINE:
	case SPLINE:
	case ARROW:	switch (corner) {
				int	n;
		case START:	x = p->o_val[N_VAL+4].f;
				y = p->o_val[N_VAL+5].f;
				break;
		case END:	n = N_VAL + 4 + 2*p->o_val[N_VAL+3].f;
				x = p->o_val[n].f;
				y = p->o_val[n+1].f;
				break;
			}
			break;
		}
	}
	*px = x;
	*py = y;
	return 1;
}

obj *gethere()	/* make a place for curx,cury */
{
	return(makepos(curx, cury, 0, (obj *)0));
}

obj *getlast(n, t)	/* find n-th previous occurrence of type t */
	int n, t;
{
	int k;
	obj *p;

	k = n;
	if (t == TEXTOBJ)	/* for purely syntactical reasons */
		t = TEXT;
	for (p = objtail->o_prev; p != objhead; p = p->o_prev) {
		if (p->o_type == BLOCKEND) {
			p = p->o_parent;
			if (t != BLOCK && t != OBJECT)
				continue;
		}
		if (t == OBJECT && p->o_type > TEXT)
			continue;
		if (t != OBJECT && p->o_type != t)
			continue;
		if (--k > 0)
			continue;	/* not there yet */
		return(p);
	}
	yyerror("there is no %dth last", n);
	return(NULL);
}

obj *getfirst(n, t)	/* find n-th occurrence of type t */
	int n, t;
{
	int k;
	obj *p;

	k = n;
	if (t == TEXTOBJ)
		t = TEXT;
	for (p = objhead->o_next; p != objtail; p = p->o_next) {
		if (p->o_type == BLOCK
		&& t != BLOCK && t != OBJECT) {		/* skip block,  */
			if (p->o_val[N_VAL].o != NULL)	/* unless it's  */
				p = p->o_val[N_VAL].o;	/* still active */
			continue;
		}
		if (t == OBJECT && p->o_type > TEXT)
			continue;
		if (t != OBJECT && p->o_type != t)
			continue;
		if (--k > 0)
			continue;	/* not there yet */
		return(p);
	}
	yyerror("there is no %dth", n);
	return(NULL);
}

double getblkvar(p, s)	/* find variable s2 in block p */
	obj *p;
	char *s;
{
	YYSTYPE y, getblk();

	y = getblk(p, s);
	free(s);
	return y.f;
}

obj *getblock(p, s)	/* find variable s in block p */
	obj *p;
	char *s;
{
	YYSTYPE y, getblk();

	y = getblk(p, s);
	free(s);
	return y.o;
}

YYSTYPE getblk(p, s)	/* find union type for s in p */
	obj *p;
	char *s;
{
	static YYSTYPE bug;
	struct symtab *stp;

	if (p->o_type != BLOCK) {
		yyerror(".%s is not in that block", s);
		return(bug);
	}
	for (stp = p->o_val[N_VAL+1].s; stp != NULL; stp = stp->s_next)
		if (strcmp(s, stp->s_name) == 0)
			return(stp->s_val);
	yyerror("there is no .%s in that []", s);
	return(bug);
}

obj *fixpos(p, x, y)	/* this, addpos & subpos SHOULD be altered to give   */
	obj *p;		/* o_x,o_y as offset from position of p, with p as   */
	double x, y;	/* o_parent, but I haven't yet worked out the xform. */
{
#if 1
/* DBK: 3/21/90 -- comment out the following if clause.  It has the effect
   of changing A in the expression A + x,y !!!
	if (p->o_type == PLACE) {
		p->o_x += x;
		p->o_y += y;
		return p;
	}
   end of commented out code *************/
	x += Xformx(p, 1, p->o_x, p->o_y);
	y += Xformy(p, 0, p->o_x, p->o_y);
#else
	if (p->o_type == PLACE) {
		x += p->o_x;
		y += p->o_y;
	}
	else {
		x += Xformx(p, 1, p->o_x, p->o_y);
		y += Xformy(p, 0, p->o_x, p->o_y);
	}
#endif
	return makepos(x, y, 0, (obj *)0);
}

obj *addpos(p, q)
	obj *p, *q;
{
	double	x, y;

	x = Xformx(q, 1, q->o_x, q->o_y);
	y = Xformy(q, 0, q->o_x, q->o_y);
/* DBK:  see above
	if (p->o_type == PLACE) {
		p->o_x += x;
		p->o_y += y;
		return p;
	}
 */
	x += Xformx(p, 1, p->o_x, p->o_y);
	y += Xformy(p, 0, p->o_x, p->o_y);
	return makepos(x, y, 0, (obj *)0);
}

obj *subpos(p, q)
	obj *p, *q;
{
	double	x, y;

	x = Xformx(q, 1, q->o_x, q->o_y);
	y = Xformy(q, 0, q->o_x, q->o_y);
/* DBK:  see above
	if (p->o_type == PLACE) {
		p->o_x -= x;
		p->o_y -= y;
		return p;
	}
 */
	x -= Xformx(p, 1, p->o_x, p->o_y);
	y -= Xformy(p, 0, p->o_x, p->o_y);
	return makepos(x, y, 0, (obj *)0);
}

obj *makenode(type, n, layer)
	int	type, n, layer;
{
	obj	*p;

	if (objbuf && batch)
		print_buf();
	p = (obj *) calloc(1, sizeof(obj) + (n-1)*sizeof(YYSTYPE));
	if (p == NULL) {
		yyerror("out of space in makenode");
		exit(1);
	}
	if (n > 65535)
		yyerror("making excessively large object");
	p->o_next = objtail;
	p->o_prev = objtail->o_prev;
	p->o_prev->o_next = objtail->o_prev = p;	
	p->o_parent = cur_block;
	p->o_x = curx;
	p->o_y = cury;
	p->o_nt1 = ntext1;
	p->o_nt2 = ntext;
	p->o_text = -1;
#if 1
	/* force all strings in object to have same justification */
	{
	int	jflags, lastf;
	jflags = RJUST|LJUST|CENTER|ABOVE|BELOW;
	lastf = text[ntext-1].t_type & jflags;
	for ( ; ntext1 < ntext; ntext1++)
		text[ntext1].t_type = text[ntext1].t_type & ~ jflags | lastf;
	}
#endif
	ntext1 = ntext;	/* ready for next caller */
	if (layer < -128)
		layer = -128;
	else if (layer > 127)
		layer = 127;
	if ((p->o_layer = layer) > 0 && layer > top_layer)
		top_layer = layer;
	if ((p->o_type = type) <= TEXT) {
		p->o_mxx = p->o_myy = 1.0;	/* initialize xform matrix */
		p->o_size = n;			/* keep count, for copies  */
	}
	return(p);
}

freenode (p)		/* free space occupied by object p */
	obj	*p;
{
	obj	*q, *q1;

	if (p->o_type == BLOCK) {			/* I think there's */
		freesymtab(p->o_val[N_VAL+1].s);	/* a bug here, for */
		q = p->o_val[N_VAL].o;			/* nested blocks.  */
		p->o_prev->o_next = q->o_next;
		q->o_next->o_prev = p->o_prev;
		q->o_next = (obj *)NULL;
		for (q = p->o_next; q != NULL; ) {
			if (q->o_type == BLOCK)
				freesymtab(q->o_val[N_VAL+1].s);
			q1 = q;
			q = q->o_next;
			free(q1); 
		}
	}
	else {
		p->o_prev->o_next = p->o_next;
		p->o_next->o_prev = p->o_prev;
	}
	free (p);
	if (!objbuf) redo_gbox = 1;	/* page boundaries are now suspect */
}

extreme(x, y, bbox)	/* record max and min x and y values */
	double	x, y;
	float	*bbox;
{
	if ((float)x < bbox[0])	bbox[0] = x;
	if ((float)y < bbox[1])	bbox[1] = y;
	if ((float)x > bbox[2])	bbox[2] = x;
	if ((float)y > bbox[3])	bbox[3] = y;
}

track_bounds(x0, y0, x1, y1)	/* insert a bounding box into the global box */
	double	x0, y0, x1, y1;
{
	if (x0 < Gbox[0])	Gbox[0] = x0;
	if (y0 < Gbox[1])	Gbox[1] = y0;
	if (x1 > Gbox[2])	Gbox[2] = x1;
	if (y1 > Gbox[3])	Gbox[3] = y1;
}

get_bounds(p, bbox, flag) /* reconstruct bounding box from center/basis/xform */
	obj	*p;
	float	*bbox;
	int	flag;
{
static	obj	*lastp = (obj *)0;
static	double	x0, y0, x1, y1;
	double	w, h, wm, hm, x, y;
	double	owid, oht;

	if (flag && p == lastp)
		goto done;
	x = Xformx(p, 1, p->o_x, p->o_y);
	y = Xformy(p, 0, p->o_x, p->o_y);
	if (p->o_type > TEXT) {
		x0 = x1 = x;
		y0 = y1 = y;
	}
	else {
		if (p->o_type == TEXT) {
			x += Linx(p,0,p->o_val[N_VAL-2].f,p->o_val[N_VAL-1].f);
			y += Liny(p,0,p->o_val[N_VAL-2].f,p->o_val[N_VAL-1].f);
		}
		else if (p->o_type == ARC || p->o_type == SECTOR) {
			x -= Linx(p,0,p->o_val[N_VAL+6].f,p->o_val[N_VAL+7].f);
			y -= Liny(p,0,p->o_val[N_VAL+6].f,p->o_val[N_VAL+7].f);
		}
		owid = p->o_wid + p->o_weight;
		oht =  p->o_ht + p->o_weight;
		if (	 ( p->o_type == ARROW || p->o_type == LINE
			|| p->o_type == SPLINE || p->o_type == ARC)
						&& (p->o_attr & HEAD12)) {
		    w = p->o_val[N_VAL+(p->o_type == ARC ? 8 : 1)].f;
		    owid += w;		/* account for width of arrowheads */
		    oht  += w;
		}
		w = Linx(p, 0, owid, oht) / 2;	/* transform main diagonal */
		h = Liny(p, 0, owid, oht) / 2;
		wm = Linx(p, 0, -owid, oht) / 2; /* transform other diagonal */
		hm = Liny(p, 0, -owid, oht) / 2;
		if (w < 0)	w = -w;
		if (h < 0)	h = -h;
		if (wm < 0)	wm = -wm;
		if (hm < 0)	hm = -hm;
		if (w < wm)	w = wm;
		if (h < hm)	h = hm;
		x0 = x - w;
		x1 = x + w;
		y0 = y - h;
		y1 = y + h;
	}
	lastp = p;
done:	bbox[0] = x0;	bbox[1] = y0;	bbox[2] = x1; bbox[3] = y1;
}
