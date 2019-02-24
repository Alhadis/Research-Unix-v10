/*	Copyright (c) 1990 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:picgen.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

obj *picgen()
{
	static	double	prevht	= HT;
	static	double	prevwid	= WID;	/* golden mean, sort of */
	static	char	*prevname = "";

	char	*checkeps();
	struct	objattr	obat;
	double	x0, y0, x1, y1, xwith, ywith;
	char	*epsname;
	int	i, at, with;
	obj	*p, *ppos;
	Attr	*ap;

	epsname = NULL;
	obat.a_ht  = 0;
	obat.a_wid = 0;
	obat.a_layer = (int)getfval("curlayer");
	obat.a_flags = EDGED;
	obat.a_weight = obat.a_lcolor = obat.a_pcolor = obat.a_tcolor = -1;
	obat.a_dashpat.a = (float *)0;
	set_text();
	at = with = xwith = ywith = 0;
	for (i = 0; i < nattr; i++) {
		ap = &attr[i];
		switch (ap->a_type) {
		default:
			miscattrs(ap, &obat);
			break;
		case SAME:
			obat.a_ht  = prevht;
			obat.a_wid = prevwid;
			epsname = prevname;
			break;
		case WITH:
			with = ap->a_val.i;	/* corner */
			break;
		case AT:
			ppos = ap->a_val.o;
			curx = Xformx(ppos, 1, ppos->o_x, ppos->o_y);
			cury = Xformy(ppos, 0, ppos->o_x, ppos->o_y);
			at++;
			break;
		}
	}
	/*
	 *	The first text string should be used as the name of an
	 *	Encapsulated PostScript file.  If missing, or if not an
	 *	EPSF, it's an error.
	 */
	if ((epsname = checkeps(&obat, epsname)) == NULL)
		return NULL;
	if (with) {
		switch (with) {
		case NORTH:	ywith = -obat.a_ht/2;			break;
		case SOUTH:	ywith =  obat.a_ht/2;			break;
		case EAST:	xwith = -obat.a_wid/2;			break;
		case WEST:	xwith =  obat.a_wid/2;			break;
		case NE:	xwith = -obat.a_wid/2;
				ywith = -obat.a_ht/2;			break;
		case SE:	xwith = -obat.a_wid/2;
				ywith =  obat.a_ht/2;			break;
		case NW:	xwith =  obat.a_wid/2;
				ywith = -obat.a_ht/2;			break;
		case SW:	xwith =  obat.a_wid/2;
				ywith =  obat.a_ht/2;			break;
		case CENTER:
		case START:
		case END:						break;
		default:	xwith =  -obat.a_wid/2 * xdelta[with%8];
				ywith =  -obat.a_ht/2  * ydelta[with%8];	 break;
		}
		curx += xwith;
		cury += ywith;
	}
	if (!at) {
		if (isright(hvmode))
			curx += obat.a_wid/2;
		else if (isleft(hvmode))
			curx -= obat.a_wid/2;
		else if (isup(hvmode))
			cury += obat.a_ht/2;
		else
			cury -= obat.a_ht/2;
	}
	p = makenode(PSFILE, N_VAL+1, obat.a_layer);
	p->o_val[N_VAL-1].p = epsname;
	p->o_val[N_VAL].f = 0.;		/* rounded corners, yet */
	p->o_wid = obat.a_wid;
	p->o_ht  = obat.a_ht;
	x0 = curx - obat.a_wid/2;
	y0 = cury - obat.a_ht/2;
	x1 = curx + obat.a_wid/2;
	y1 = cury + obat.a_ht/2;
	primattrs(p,&obat);
	text_bounds(p);
	track_bounds(x0-p->o_weight/2, y0-p->o_weight/2,
		     x1+p->o_weight/2, y1+p->o_weight/2);
	if (isright(hvmode))
		curx = x1;
	else if (isleft(hvmode))
		curx = x0;
	else if (isup(hvmode))
		cury = y1;
	else
		cury = y0;
	prevht  = obat.a_ht;
	prevwid = obat.a_wid;
	prevname = epsname;
	return(p);
}

char *
checkeps(obat, tname)
	struct	objattr	*obat;
	char	*tname;
{
	int	urx, ury, llx, lly;
	char	buf[128];
	FILE	*fp;

	if (tname != NULL)
		return tname;
	if (ntext1 == ntext) {		/* nostrings */
		yyerror("no EPS file named");
		return NULL;
	}
	tname = text[ntext1].t_val;
	if ((fp = fopen(tname, "r")) == NULL) {
		yyerror("can't read EPS file %s", tname);
		return NULL;
	}
	ntext1++;		/* filename is NOT one of the label strings */
	llx = lly == - (urx = ury = -65535);
	while (fgets(buf, 128, fp) != NULL) {
		if (strncmp(buf, "%%BoundingBox:", 14) != 0)
			continue;
		llx = lly == - (urx = ury = -65535);
		if (sscanf(buf+14, "%d %d %d %d", &llx, &lly, &urx, &ury) > 2)
			break;
	}
	fclose(fp);
	/* NOTE: if no BoundingBox comment was found, size will be 0x0 */
	/*	 unless height and width were set explicitly	*/
	if (urx < llx)
		urx = llx;
	if (ury < lly)
		ury = lly;
	if (obat->a_wid == 0 && obat->a_ht == 0) {
		obat->a_wid = (double) (urx - llx) / pgscale;
		obat->a_ht  = (double) (ury - lly) / pgscale;
	}
	else if (obat->a_wid == 0 && ury - lly != 0)
		obat->a_wid = obat->a_ht * (urx - llx) / (ury - lly);
	else if (obat->a_ht == 0 && urx - llx != 0)
		obat->a_ht = obat->a_wid * (ury - lly) / (urx - llx);
	if (obat->a_wid == 0)
		obat->a_wid = getfval("boxwid");
	if (obat->a_ht == 0)
		obat->a_ht = getfval("boxht");
	return tname;
}
