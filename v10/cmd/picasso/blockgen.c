/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:blockgen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

#define	NBRACK	20	/* depth of [...] */
#define	NBRACE	20	/* depth of {...} */

struct	pushstack stack[NBRACK];
int	nstack	= 0;
struct	pushstack bracestack[NBRACE];
int	nbstack	= 0;
obj	*cur_block = (obj *)0;

obj *leftthing(c)	/* called for {... or [... */
			/* really ought to be separate functions */
	int c;
{
	obj *p;

	if (c == '[') {
		if (nstack >= NBRACK)
			fatal("[...] nested too deep");
		stack[nstack].p_x = curx;
		stack[nstack].p_y = cury;
		stack[nstack].p_hvmode = hvmode;
		stack[nstack].p_xmin = Gbox[0];
		stack[nstack].p_ymin = Gbox[1];
		stack[nstack].p_xmax = Gbox[2];
		stack[nstack].p_ymax = Gbox[3];
		nstack++;
		Gbox[0] = Gbox[1] = 32767;
		Gbox[2] = Gbox[3] = -32767;
		p = makenode(BLOCK, N_VAL+2, (int)getfval("curlayer"));
		curx = cury = 0;
		cur_block = p;
	} else {
		if (nbstack >= NBRACK)
			fatal("{...} nested too deep");
		bracestack[nbstack].p_x = curx;
		bracestack[nbstack].p_y = cury;
		bracestack[nbstack].p_hvmode = hvmode;
		nbstack++;
		p = NULL;
	}
	return(p);
}

obj *rightthing(p, c)	/* called for ... ] or ... } */
	obj *p;
{
	obj *q;

	if (c == '}') {
		nbstack--;
		curx = bracestack[nbstack].p_x;
		cury = bracestack[nbstack].p_y;
		hvmode = bracestack[nbstack].p_hvmode;
		q = makenode(MOVE, 0, 0);
	} else {
		nstack--;
		hvmode = stack[nstack].p_hvmode;
		q = makenode(BLOCKEND, 0, p->o_layer);
		cur_block = p->o_parent;
		curx = stack[nstack].p_x;
		cury = stack[nstack].p_y;
		p->o_val[N_VAL+0].o = q;
		p->o_val[N_VAL+1].s = stack[nstack+1].p_symtab;
		p->o_x   = (Gbox[2]+Gbox[0])/2;
		p->o_wid = (Gbox[2]-Gbox[0]);
		if (p->o_wid < 0)		p->o_wid = 0;
		p->o_y   = (Gbox[3]+Gbox[1])/2;
		p->o_ht  = (Gbox[3]-Gbox[1]);
		if (p->o_ht < 0)		p->o_ht = 0;
		Gbox[0] = stack[nstack].p_xmin;
		Gbox[1] = stack[nstack].p_ymin;
		Gbox[2] = stack[nstack].p_xmax;
		Gbox[3] = stack[nstack].p_ymax;
	}
	return(q);
}

obj *blockgen(p)	/* handles [...] */
	obj *p;
{
	float	h, w, xwith, ywith;
	float	ctrx, ctry, x0, y0, x1, y1;
	int	i, at, with;
	obj	*ppos;
	Attr	*ap;

	at = with = xwith = ywith = 0;
	if (ntext > ntext1)
		set_text();
	ctrx = curx + p->o_x;
	ctry = cury + p->o_y;
	w  = p->o_wid;
	h  = p->o_ht;
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		case HEIGHT:
			h = ap->a_val.f;
			break;
		case WIDTH:
			w = ap->a_val.f;
			break;
		case WITH:
			with = ap->a_val.i;	/* corner */
			break;
		case PLACE:	/* actually with position ... */
			ppos = ap->a_val.o;
			xwith = p->o_x - Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			ywith = p->o_y - Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			with = PLACE;
			break;
		case AT:
		case FROM:
			ppos = ap->a_val.o;
			ctrx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			ctry = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			at++;
			break;
		case LAYER:
			if ((p->o_layer = (int)ap->a_val.f) > top_layer)
				top_layer = p->o_layer;
			break;
		case FONT:
			reset_font((double)ap->a_val.f);
			break;
		case SIZE:
			reset_size(ap->a_sub, (double)ap->a_val.f);
			break;
		case SPACE:
			reset_space(ap->a_sub, (double)ap->a_val.f);
			break;
		case TEXTATTR:
			savetext(ap->a_sub, ap->a_val.p);
			break;
		case TCOLOR:
			p->o_text = checkcolor((double)ap->a_val.f);
			break;
		}
	}
	if (!at && !with)
		with = isright(hvmode) ? WEST : isleft(hvmode) ? EAST :
				isup(hvmode) ? SOUTH : NORTH;
	if (with) {
		switch (with) {
		case NORTH:	ywith = -h/2;			break;
		case SOUTH:	ywith =  h/2;			break;
		case EAST:	xwith = -w/2;			break;
		case WEST:	xwith =  w/2;			break;
		case NE:	xwith = -w/2;	ywith = -h/2;	break;
		case SE:	xwith = -w/2;	ywith =  h/2;	break;
		case NW:	xwith =  w/2;	ywith = -h/2;	break;
		case SW:	xwith =  w/2;	ywith =  h/2;	break;
		}
		ctrx += xwith;
		ctry += ywith;
	}
	x0 = ctrx - w / 2;
	x1 = ctrx + w / 2;
	y0 = ctry - h / 2;
	y1 = ctry + h / 2;
	p->o_nt1 = ntext1;
	p->o_nt2 = ntext;
	if (ntext > ntext1) {
		ntext1 = ntext;
		checktextcolor(p);
	}
	translate(p, ctrx - p->o_x, ctry - p->o_y);
	text_bounds(p);
	if (isright(hvmode))
		curx = x1;
	else if (isleft(hvmode))
		curx = x0;
	else if (isup(hvmode))
		cury = y1;
	else
		cury = y0;
	stack[nstack+1].p_symtab = NULL;	/* so won't be found again */
	return(p);
}
