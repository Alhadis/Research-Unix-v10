/* Copyright 1990, AT&T Bell Labs */
#include <stdlib.h>
#include <ctype.h>
#include "fsort.h"



static char *modifiers(struct field*, char*, int);
static char *keyspec(struct pos*, char*);
static void globalmods(struct field*);
static void chkfieldno(struct field*);

struct field fields[NF] = {
	{ 0, 0, 0, 0, 0, 0, 0, { 0, 0 }, { NP, 0 } }
};
int nfields = 0;

int tab;
int signedrflag;
int simplekeyed;

#define blank(p) (*(p)==' ' || *(p)=='\t')

enum { OLD, NEW };

	/* interpret 1 or 2 arguments and return how many */
int
fieldarg(char *argv1, char *argv2)
{
	char *av1 = argv1;
	char *av2 = argv2;
	struct field *field;

	if(av1[0] == '+' && isdigit(av1[1])) {
		if(++nfields >= NF)
			fatal("too many fields", argv1, 0);
		field = &fields[nfields];
		field->end.fieldno = NP+1;
		field->style = OLD;

		av1 = keyspec(&field->begin, av1+1);
		if(*modifiers(field, av1, 0))
			goto bad;

		if(av2==0 || av2[0]!='-' || !isdigit(av2[1]))
			return 1;
		av2 = keyspec(&field->end, av2+1);
		argv1 = argv2;	/* in case of diagnostic */
		if(*modifiers(field, av2, 1))
			goto bad;
		return 2;
	} else if(*modifiers(fields, av1+1, -1))
		goto bad;	/* believed not to happen */
	return 1;
bad:
	fatal("bad field specification", argv1, 0);
	return 0;	/* dummy */
}

void
optionk(char *arg, struct field *fields, int *nfields)
{
	char *a = arg;
	struct field *field;
	if(++*nfields >= NF)
		fatal("too many fields", arg, 0);
	field = &fields[*nfields];
	field->begin.charno = 1;
	field->end.fieldno = NP+1;
	field->style = NEW;

	a = keyspec(&field->begin, a);
	a = modifiers(field, a, 0);
	if(*a == ',') {
		a = keyspec(&field->end, a+1);
		a = modifiers(field, a, 1);
	}
	if(*a == 0)
		return;
bad:
	fatal("bad -k specification", arg, 0);
}

static char *
keyspec(struct pos *p, char *arg)
{
	if(!isdigit(*arg))
		fatal("missing field number", "", 0);
	p->fieldno = strtoul(arg, &arg, 10);
	if(*arg == '.')
		if(!isdigit(*++arg))
			fatal("missing character number", "", 0);
		else
			p->charno = strtoul(arg, &arg, 10);
	return arg;
}

/* keyed = 1 if there are fields present (+ options) or if
   numeric (-ng), translation (-f) or deletion (-idb) options
   are present.  In these cases, a separate key is constructed
   for rsort.  The key, however is not carried on 
   intermediate files.  (It would be interesting to try.)
   It must be reconstructed for the merge phase, and that
   may be expensive, since relatively few comparisons
   happen in that phase.  simplekeyed = 1 if there are options,
   so that pure ascii comparison won't work, but no fields, no
   months, no numerics. */

void
fieldwrapup(void)
{
	int i;
	if(nfields==0 && aflag)
		fatal("-a without -k", "", 0);
	if(fields->coder == 0) fields->coder = tcode;
	if(fields->trans == 0) fields->trans = ident;
	if(fields->keep == 0) fields->keep = all;
	for(i=1; i<=nfields; i++) {
		globalmods(&fields[i]);
		chkfieldno(&fields[i]);
	}
	for(i=1; i<=naccum; i++) {
		chkaccum(&accum[i]);
		chkfieldno(&accum[i]);
	}
	signedrflag = fields->rflag? -1: 1; /* used only by merge.c*/
	simplekeyed = nfields==0 && fields->coder==tcode 
		      && (fields->trans!=ident || fields->keep!=all);
	if(nfields==0 && !keyed)	/* used only by rsort.c */
		rflag = fields->rflag;
	if(nfields > 0)
		keyed = 1;
}

static void
conflict(void)
{
	warn("conflicting key types", "", 0);
}

static void
dupla(uchar **oldp, uchar *new)
{
	if(*oldp != 0 && *oldp != new)
		conflict();
	*oldp = new;
}

static void
duplb(int (**oldp)(uchar*,uchar*,int,struct field*), int (*new)(uchar*,uchar*,int,struct field*))
{
	if(*oldp != 0 && *oldp != new)
		conflict();
	*oldp = new;
}

/* eflag=-1 global flags, =0 field start, =1 field end */

static char *
modifiers(struct field *field, char *argv1, int eflag)
{
	for( ; *argv1; argv1++) {
		switch(*argv1) {
		case 'b': if(eflag==1) field->eflag = 1;
			  else field->bflag = 1; goto ckglob;
		case 'r': field->rflag = 1; goto ckglob;
		case 'f': dupla(&field->trans, fold); break;
		case 'd': dupla(&field->keep, dict); break;
		case 'i': dupla(&field->keep, ascii); break;
		case 'g': duplb(&field->coder, gcode); break;
		case 'n': duplb(&field->coder, ncode); break;
		case 'M': duplb(&field->coder, Mcode); break;
		default:
			goto done;
		}
		keyed = 1;
	ckglob:
		if(field==fields && nfields>0)
			warn("field spec precedes global option",argv1,1);
	}
done:
	if(field->coder==ncode && field->keep)
		conflict();
	return argv1;
}

static void
globalmods(struct field *field)
{
	int flagged = field->bflag | field->eflag | field->rflag;
	if(!field->coder) field->coder = tcode;
	else flagged++;
	if(!field->trans) field->trans = ident;
	else flagged++;
	if(!field->keep) field->keep = all;
	else flagged++;
	if(!flagged) {
		field->coder = fields->coder;
		field->trans = fields->trans;
		field->keep = fields->keep;
		field->rflag = fields->rflag;
		field->bflag = fields->bflag;
		if(field->style == NEW)
			field->eflag = fields->bflag;
	}
}

/* convert field representation from numbers given in arguments
   to a 0-origin first,last+1 representation, with a negative
   quantity for a character offset to the end of this field */

static void
chkfieldno(struct field *field)
{
	if(field->style == NEW) {
		if(--field->begin.fieldno < 0 ||
		   --field->begin.charno < 0 ||
		   --field->end.fieldno < 0)
			fatal("improper 0 in field specifier", "", 0);
		if(field->end.charno == 0)
			field->end.charno--;
	} else if(field->end.charno==0 && field->end.fieldno>0) {
		if(tab && field->eflag)
			fatal("skipping blanks right after tab char"
			      " is ill-defined", "", 0);
		field->end.fieldno--;
		field->end.charno--;
	}
	if(field->begin.fieldno > NP)
		field->begin.fieldno = NP;
	if(field->end.fieldno > NP)
		field->end.fieldno = NP;
/*	fprintf(stderr,"%d %d.%d,%d.%d\n",field-fields,field->begin.fieldno, field->begin.charno,field->end.fieldno, field->end.charno);*/
}

int
fieldcode(uchar *dp, uchar *kp, int len, uchar *b, struct field *fields, int nfields)
{
	uchar *posns[NP+1];	/* field start positions */
	uchar *cp;
	struct field *field;
	uchar *op = kp;
	uchar *ep;
	uchar *bound = kp + MAXREC;
	int i;
	int np;
	if(bound > b)
		bound = b;
	posns[0] = dp;
	if(tab)
		for(np=1, i=len, cp=dp; i>0 && np<NP; i--) {
			if(*cp++ != tab)
				continue;
			posns[np++] = cp;
		}
	else
		for(np=1, i=len, cp=dp; i>0 && np<NP; ) {
			while(blank(cp) && i>0)
				cp++, i--;
			while(!blank(cp) && i>0)
				cp++, i--;
			posns[np++] = cp;
		}

	if(nfields > 0)
		field = &fields[1];
	else
		field = &fields[0];
	i = nfields;
	do {
		int t = field->begin.fieldno;
		uchar *xp = dp + len;
		if(t < np) {
			cp = posns[t];
			if(field->bflag && nfields)
				while(cp<xp && blank(cp))
					cp++;
			cp += field->begin.charno;
			if(cp > xp)
				cp = xp;
		} else
			cp = xp;
		t = field->end.fieldno;
		if(t < np) {
			if(field->end.charno < 0) {
				if(t >= np-1)
					ep = xp;
				else {
					ep = posns[t+1];
					if(tab) ep--;
				}
			} else {
				ep = posns[t];
				if(field->eflag)
					while(ep<xp && blank(ep))
						ep++;
				ep += field->end.charno;
			}
			if(ep > xp)
				ep = xp;
			else if(ep < cp)
				ep = cp;
		} else
			ep = xp;
		t = ep - cp;
		if(field->coder != acode && op+room(t) > bound)
			return -1;
		op += (*field->coder)(cp, op, ep-cp, field);
		field++;
	} while(--i > 0);
	return op - kp;
}

	/* Encode text field subject to options -r -fdi -b.
	   Fields are separated by 0 (or 255 if rflag is set)
           the anti-ambiguity stuff prevents such codes from
	   happening otherwise by coding real zeros and ones
	   as 0x0101 and 0x0102, and similarly for complements */

int
tcode(uchar *dp, uchar *kp, int len, struct field *f)
{
	uchar *cp = kp;
	int c;
	uchar *keep = f->keep;
	uchar *trans = f->trans;
	int reverse = f->rflag? ~0: 0;
	while(--len >= 0) {
		c = *dp++;
		if(keep[c]) {
			c = trans[c];
			if(c <= 1) {	/* anti-ambiguity */
				*cp++ = 1^reverse;
				c++;
			} else if(c >= 254) {
				*cp++ = 255^reverse;
				c--;
			}
			*cp++ = c^reverse;
		}
	}
	*cp++ = reverse;
	return cp - kp;
}

static char *month[] = { "jan", "feb", "mar", "apr", "may", 
	         "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

int
Mcode(uchar *dp, uchar *kp, int len, struct field *f)
{
	int j = -1;
	int i;
	uchar *cp;
	for( ; len>0; dp++, len--) {
		if(*dp!=' ' && *dp!='\t')
			break;
	}
	if(len >= 3)
		while(++j < 12) {
			cp = (uchar*)month[j];
			for(i=0; i<3; i++)
				if((dp[i]|('a'-'A')) != *cp++)
					break;
			if(i >= 3)
				break;
		}
	*kp = j>=12? 0: j+1;
	if(f->rflag)
		*kp ^= ~0;
	return 1;
}
