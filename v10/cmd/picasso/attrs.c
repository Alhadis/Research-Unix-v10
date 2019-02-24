/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:attrs.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

setdir(n)	/* set direction (hvmode) from LEFT, RIGHT, etc. */
	int n;
{
	switch (n) {
	case UP:	hvmode = U_DIR;		break;
	case DOWN:	hvmode = D_DIR;		break;
	case LEFT:	hvmode = L_DIR;		break;
	case RIGHT:	hvmode = R_DIR;		break;
/*	case REVERSE:	hvmode = (hvmode+2)%4;	break;	*/
	}
	return(hvmode);
}

curdir()	/* convert current dir (hvmode) to RIGHT, LEFT, etc. */
{
	switch (hvmode) {
	case R_DIR:	return RIGHT;
	case L_DIR:	return LEFT;
	case U_DIR:	return UP;
	case D_DIR:	return DOWN;
	}
	yyerror("can't happen curdir");
}

double	setrgbindex ()		/* returns integer-valued index to (r,g,b) */
{
	int	n;

	if (nexpr > 3)
		yyerror("color components are r, g, b only; excess ignored");
	for (n = 0; n < 3; ++n) {
		if (n >= nexpr)
			 exprlist[n] = 0;
		else if (exprlist[n] < 0 || exprlist[n] > 1) {
			yyerror("color component %g not between 0 and 1",
						exprlist[n]);
			nexpr = 0;
			return 0;
		}
	}
	nexpr = 0;
	for (n = nrgb; n--;)
		if (exprlist[0] == rgbtable[n].r &&	/* backwards in case  */
		    exprlist[1] == rgbtable[n].g &&	/* there's locality.  */
		    exprlist[2] == rgbtable[n].b) {
			return ((double)n);
	}
	if (nrgb >= nrgbtable)
		rgbtable = (rgb *) grow((char *)rgbtable, "rgbtable",
				nrgbtable += 100, sizeof(rgb));
	rgbtable[nrgb].r = exprlist[0];
	rgbtable[nrgb].g = exprlist[1];
	rgbtable[nrgb].b = exprlist[2];
	return ((double)nrgb++);
}

double
checkcolor (f)		/* gray level OR negative integer rgb index */
	double	f;
{
/*	if (f < 0.0 || (f > 1.0 && f != (double)((int)f)))
		{ yyerror("gray level %g not between 0 and 1", f); f = -1.0; }
*/	if (f > 1.0 || f < 0.0)
		f = (double)((int)f);
	if (f < -1.0)
		f = -1.0;	/* end of change */
	else if (f >= nrgb)
		{ yyerror("rgb color index %g not defined", f); f = -1.0; }
	return f;
}

makefattr(type, sub, f)	/* float attr */
	int type, sub;
	double f;
{
	YYSTYPE val;
	val.f = f;
	makeattr(type, sub, val);
}

makeoattr(type, o)	/* obj* attr */
	obj *o;
{
	YYSTYPE val;
	val.o = o;
	makeattr(type, 0, val);
}

makeiattr(type, i)	/* int attr */
	int i;
{
	YYSTYPE val;
	val.i = i;
	makeattr(type, 0, val);
}

int	def_textattr = CENTER;

maketattr(sub, p)	/* text attribute: takes two */
	char *p;
{
	YYSTYPE val;
	val.p = p;
	if (sub == 0)
		sub = def_textattr;
	else if (sub & (CENTER|LJUST|RJUST))
		def_textattr = (def_textattr & ~(CENTER|LJUST|RJUST)) | sub;
	else if (sub & (ABOVE|BELOW))
		def_textattr = (def_textattr & ~(ABOVE|BELOW)) | sub;
	makeattr(TEXTATTR, sub, val);
}

addtattr(sub)		/* add text attrib to existing item */
{
	attr[nattr-1].a_sub |= sub;
/*	def_textattr != sub;  looks like a bug and should be */
	def_textattr |= sub;
}

makevattr(p)	/* varname attribute */
	char *p;
{
	YYSTYPE val;

	val.p = p;
	makeattr(VARNAME, 0, val);
}

makelattr(p, q)	/* "locus" attribute; x and y coordinate lists via varnames */
	char	*p, *q;
{
	YYSTYPE val;
	val.p = p;
	makeattr(XLIST, 0, val);
	val.p = q;
	makeattr(YLIST, 0, val);
}

makedattr(name)		/* dash pattern (array of dash/space widths) */
	char	*name;
{
	int	n;
struct	symtab	*p;
	YYSTYPE	val;

	if (name) {
		if ((p = lookup(name)) == NULL) {
			yyerror("no variable named %s", name);
			return;
		}
		n = p->s_dim+1;
	}
	else {
		n = nexpr;
		nexpr = 0;
	}
	if ((val.a = (float *)malloc(sizeof(float)*(n+1))) == NULL)
		yyerror("out of room in makedattr");
	else {
		val.a[0] = (float)n;
		while (n--)
			val.a[n+1] = (name? p->s_val.a[n] : exprlist[n]);
	}
	makeattr(DASHPAT, 0, val);
}

makeattr(type, sub, val)	/* add attribute type and val */
	int type, sub;
	YYSTYPE val;
{
	if (type == 0 && val.i == 0) {	/* clear table for next stat */
		nattr = 0;
		def_textattr = CENTER;
		return;
	}
	if (nattr >= nattrlist)
		attr = (Attr *) grow((char *)attr, "attr", nattrlist += 100,
								sizeof(Attr));
	attr[nattr].a_type = type;
	attr[nattr].a_sub = sub;
	attr[nattr].a_val = val;
	nattr++;
}

double	setattr(p, type, val)
	obj	*p;
	int	type;
	double	val;
{
	int	cw_switch;
	double	x, y;
	obj	*q;

	if (p->o_type == BLOCK) {
		for (q = p->o_next; q != p->o_val[N_VAL].o; q = q->o_next)
			setattr (q, type, val);
	}
	else if (p->o_type <= TEXT) switch (type) {

case TCOLOR:	if (val >= 0.0 || checkcolor(val) >= 0.0)
			p->o_text = val;
		break;
case LCOLOR:	if (val >= 0.0 || checkcolor(val) >= 0.0)
			p->o_color = val;
		break;
case PCOLOR:	if (val < 0.0 || checkcolor(val) < 0.0)
			p->o_attr &= ~FILLED;
		else {
			p->o_fill = val;	/* ignored for TEXT? */
			p->o_attr |= FILLED;
		}
		break;
case LAYER:	if (val < -128) val = -128; else if (val > 127) val = 127;
		p->o_layer = (short)val;
		if (val > getfval("maxlayer"))
			setfval ("maxlayer", val);
		break;
case LWEIGHT:	p->o_weight = val;
		break;
case NOEDGE:	if (val != 0.0)
			p->o_attr &= ~EDGED;
		else
			p->o_attr |= EDGED;
		break;

case CCW:
case CW:	if (p->o_type == ARC) {
			cw_switch = (p->o_attr & CW_ARC);
			if (type == CW)
				cw_switch = !cw_switch;
			if (cw_switch) {
				x = p->o_val[N_VAL+2].f;
				y = p->o_val[N_VAL+3].f;
				p->o_val[N_VAL+2] = p->o_val[N_VAL+4];
				p->o_val[N_VAL+3] = p->o_val[N_VAL+5];
    /* exchange from & to */	p->o_val[N_VAL+4].f = x;
				p->o_val[N_VAL+5].f = y;
				p->o_attr ^= (p->o_attr & CW_ARC);
			}
		}
		break;
case DIAMETER:	val /= 2;
case RADIUS:	switch (p->o_type) {
			case ARROW:
			case LINE:	
			case BOX:
			case ARC:
			case SECTOR:	p->o_val[N_VAL].f = val;
					break;
			case CIRCLE:
			case ELLIPSE:	p->o_wid = val * 2;
					break;
		}
		break;
case WIDTH:	p->o_wid = val;
		break;
case HEIGHT:	p->o_ht = val;
		break;
default:	yyerror ("can't happen setattr");
	}
	return val;
}

miscattrs(ap, obat)
	Attr	*ap;
struct	objattr	*obat;
{
	float	*fp;
	int	n;

	switch (ap->a_type) {
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
			if (ap->a_val.p != NULL)
				savetext(ap->a_sub, ap->a_val.p);
			else
				text[ntext-1].t_type = ap->a_sub;
						/* ??? can this ever happen */
						/* except after a previous  */
						/* text in the same object? */
			break;
		case NOEDGE:
			obat->a_flags &= ~EDGED;
			break;
		case LAYER:
			obat->a_layer = ap->a_val.f;
			break;
		case LWEIGHT:
			obat->a_weight = ap->a_val.f;
			break;
		case LCOLOR:
			obat->a_lcolor = ap->a_val.f;
			break;
		case PCOLOR:
			obat->a_flags |= FILLED;
			if (ap->a_sub != DEFAULT)
				obat->a_pcolor = ap->a_val.f;
			break;
		case TCOLOR:
			obat->a_tcolor = ap->a_val.f;
			break;
		case DOT:
		case DASH:
			n = ap->a_type == DOT ? 3 : 2;
			if ((fp = (float *)malloc(n * sizeof(float))) == NULL) {
				yyerror("out of space in miscattrs");
				break;
			}
			*fp = --n;
			fp[n] = (ap->a_sub == DEFAULT ? getfval("dashwid")
						      : ap->a_val.f);
			if (n == 2)
				fp[1] = -1;	/* fill in later, from weight */
			ap->a_val.a = fp;
			/* and fall through to the general case */
		case DASHPAT:
			obat->a_flags |= DOTDASH;
			obat->a_dashpat.a = ap->a_val.a;
			break;
		case HEIGHT:
			obat->a_ht = ap->a_val.f;
			break;
		case WIDTH:
			obat->a_wid = ap->a_val.f;
			break;
		case RADIUS:
			obat->a_rad = ap->a_val.f;
			break;
		case DIAMETER:
			obat->a_rad = ap->a_val.f / 2;
			break;
	}
}

float	miters[8] = {0, 0, 0, 0, 0, 0, 0, 0};

primattrs(p, obat)	/* note: ht, wid, rad and layer are set elsewhere   */
			/*	 because of nonuniformities in their design */
	obj	*p;	/*	 (this could be further rationalized!)	    */
struct	objattr	*obat;
{
	int	n;
	float	x, bnd[4];

	p->o_attr |= obat->a_flags;
	p->o_text  = obat->a_tcolor;
	checktextcolor(p);
	if (obat->a_flags & FILLED) {
		if (obat->a_pcolor == -1.)
			obat->a_pcolor = getfval ("fillcolor");
		p->o_fill = checkcolor((double)obat->a_pcolor);
	}
	if (obat->a_flags & EDGED) {
		if (obat->a_weight == -1.)
			obat->a_weight = getfval ("lineweight");
		if (obat->a_weight >= 0.)
			p->o_weight = obat->a_weight;
		if (obat->a_lcolor == -1.)
			obat->a_lcolor = getfval ("linecolor");
		p->o_color = checkcolor((double)obat->a_lcolor);
		if ((n = (int)getfval("linecap")) >= 0)
			p->o_attr |= (1 + n%3)*LINECAP;
		if ((n = (int)getfval("linejoin")) >= 0)
			p->o_attr |= (1 + n%3)*JOIN;
		if (n == 0) {				/* mitre join */
			x = getfval("miterlimit");
			if (x >= 1) {
				for (n = 1; n < 8; n++)
					if (miters[n] == 0 || miters[n] == x)
						break;
				if (n == 8)
					for (n = 1; n < 7; ++n)
						miters[n] = miters[n+1];
				miters[n] = x;
				p->o_attr |= n * MITER;
			}
		}
	}
	else
		p->o_weight = 0;
	if (obat->a_dashpat.a != NULL && obat->a_dashpat.a[1] == -1) /* dots */
		if ((obat->a_dashpat.a[1] = p->o_weight) == 0)
			obat->a_dashpat.a[1] = 1/(pgscale*2);
	p->o_ddpat = obat->a_dashpat;
}

checktextcolor (p)
	obj	*p;
{
	if (p->o_nt2 > p->o_nt1) {
		if (p->o_text == -1)
			p->o_text = getfval("textcolor");
		p->o_text = checkcolor((double)p->o_text);
	}
}
