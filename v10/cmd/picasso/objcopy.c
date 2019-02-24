/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:objcopy.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

obj *copyone (p, delx, dely)
	obj	*p;
	double	delx, dely;
{
	obj	*q, *r;
	unsigned n;

	if (p->o_type > TEXT)
		return p;
	n = sizeof(obj) + sizeof(YYSTYPE) * (p->o_size - 1);
	if ((q = (obj *)malloc(n)) == NULL) {
		yyerror("out of space in object copy");
		exit(1);
	}
	memcpy (q, p, n);
	q->o_next = objtail;
	q->o_prev = objtail->o_prev;
	q->o_prev->o_next = objtail->o_prev = q;	
	q->o_nt1 = ntext;		/* copy all the text strings */
	q->o_nt2 = copytext(p->o_nt1, p->o_nt2);
	translate(q, delx, dely);

	if (q->o_type == BLOCK) {	/* copy objects through BLOCKEND */

		for (r = p->o_next; r != p->o_val[N_VAL].o; r = r->o_next)
			copyone(r, 0.0, 0.0)->o_parent = q;

		q->o_val[N_VAL].o = r = makenode (BLOCKEND, 0, p->o_layer);
		r->o_parent = q;
	}
	return q;
}

obj *copypos (p, q)
	obj	*p, *q;
{
	obj	*r;
	float	bnd[4];
	double	ox,oy;

	ox = Xformx(p, 1, p->o_x, p->o_y);
	oy = Xformy(p, 0, p->o_x, p->o_y);
	r = copyone(p, q->o_x - ox, q->o_y - oy);
	get_bounds(r, bnd, 1);
	track_bounds(bnd[0],bnd[1],bnd[2],bnd[3]);
}

obj *copyobj (p, xx, yy)
	obj	*p;
	char	*xx, *yy;
{
	obj	*q;
	struct	symtab	*x, *y;
	int	i;
	float	cbox[4], obox[4];
	double	x0, y0, x1, y1;

	if ((x = lookup(xx)) == NULL || (y = lookup(yy)) == NULL) {
		yyerror("no position defined by (%s,%s)", xx, yy);
		return (p);
	}
	else {
		x0 = Xformx(p, 1, p->o_x, p->o_y);
		y0 = Xformy(p, 0, p->o_x, p->o_y);
		cbox[0] = cbox[1] = cbox[2] = cbox[3] = 0;
		first_xy (x, y, &x1, &y1);
		do {
			q = copyone (p, x1-x0, y1-y0);
			extreme (x1-x0, y1-y0, cbox);
		} while (next_xy (x, y, &x1, &y1));
		get_bounds (p, obox, 1);
		for (i = 0; i < 4; ++i)
			cbox[i] += obox[i];
		track_bounds(cbox[0],cbox[1],cbox[2],cbox[3]);
		return q;	/* i.e., last copy */
	}
}
