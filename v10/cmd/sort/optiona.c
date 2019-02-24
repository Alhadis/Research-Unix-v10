#include <float.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "fsort.h"

	/* portability hack for old libc's */
#define realloc(p,q) (p?realloc(p,q):malloc(q))

struct field accum[NF];
int naccum;

typedef struct {
	uchar *e;	/* last+1st char */
	signed char s;	/* -1 for neg, 1 for pos */
	char c;		/* 1 if + sign, else 0 */
	char z;		/* 1 for leading 0, else 0 */
	char p;		/* nonzero if a decimal point */
	int f;		/* number of digits after dec. pt. */
} desc;

#define max(a, b) ((a)>(b)?(a):(b))
#define mod10(a) (((a)+20)%10)
#define div10(a) (((a)+20)/10-2)

/* find least significant digit and other facts about a number
   in field beginning at a and ending just before e */

desc
lsd(uchar *a, uchar *e)
{
	desc d = { 0, 1, 0, 0, 0, 0 };
	while(a<e && (*a==' '||*a=='\t')) a++;
	if(a >= e)
		;
	else if(*a == '-') {
		d.s = -1;
		a++;
	} else if(*a == '+') {
		d.c = 1;
		a++;
	}
	if(a+1<e && *a=='0' && isdigit(a[1])) d.z = 1;
	while(a<e && isdigit(*a))
		a++;
	if(a<e && *a=='.') {
		d.p = 1;
		while(++a<e && isdigit(*a))
			d.f++;
	}
	d.e = a;
	return d;
}

/* add the fields at a and b as desc-ribed,
   putting result, with decimal point omitted,
   right justified before re.
   calculates in 10's complement and recomplements
   magnitude if negative.
   set *sgn -1 for neg, 1 for signed +, 0 for unsigned. */

uchar *
add(uchar *re, uchar *a, desc ad, uchar *b, desc bd, int *sgn)
{
	int d = 0;
	uchar *t;
	uchar *r = re;
	while(ad.f > bd.f) {
		d += (*--ad.e - '0')*ad.s;
		*--re = mod10(d);
		d = div10(d);
		ad.f--;
	}
	while(bd.f > ad.f) {
		d += (*--bd.e - '0')*bd.s;
		*--re = mod10(d);
		d = div10(d);
		bd.f--;
	}
	while(ad.e>a || bd.e>b) {
		if(ad.f-- == 0) {
			if(ad.p)
				ad.e--;
			if(bd.p)
				bd.e--;
		}
		if(ad.e > a)
			if(isdigit(*--ad.e))
				d += (*ad.e - '0')*ad.s;
			else 
				ad.e = a;
		if(bd.e > b)
			if(isdigit(*--bd.e))
				d += (*bd.e - '0')*bd.s;
			else
				bd.e = b;
		*--re = mod10(d);
		d = div10(d);
	}
	*sgn = ad.c | bd.c;
	if(d > 0)
		*--re = 1;	/* happens only on overflow */
	else if(d < 0) {
		int x = 10;
		*sgn = -1;
		for(t = r; --t>=re; ) {
			*t = (x - *t)%10;
			if(*t != 0)
				x = 9;
		}
		if(d < -1)
			*--re = 1; /* hygiene, can't happen */
	}
	while(re<r && *re==0)
		re++;
	return re;
}

int
fieldaccum(uchar *a, uchar *ae, uchar *b, uchar *be)
{
	static uchar *r, *re;	/* not pure ansi (re-r=nil-nil=?) */
	desc da = lsd(a, ae);
	desc db = lsd(b, be);
	int sgn, Sgn, f;
	uchar *v;
	while(re-r < max(da.e-a+db.f, db.e-b+da.f) + 1) {
		int l = re - r + 100;
		r = (uchar*)realloc(r, l);
		if(r == 0)
			fatal("out of space","",0);
		re = r + l;
	}
	f = max(da.f, db.f);
	v = add(re, a, da, b, db, &sgn);
	Sgn = sgn != 0;
	if(Sgn+max(re-v,f+1)+(da.p|db.p) > ae-a)
		return 0;
	while(re>v && --f>=0)
		*--ae = *--re + '0';
	while(--f >= 0)
		*--ae = '0';
	if(da.p | db.p)
		*--ae = '.';
	if(re <= v)
		*--ae = '0';
	while(re > v)
		*--ae = *--re + '0';
	if(da.z | db.z)
		while(ae > a+Sgn)
			*--ae = '0';
	if(sgn > 0)
		*--ae = '+';
	else if(sgn < 0)
		*--ae = '-';
	if(ae<a || v<r)
		fatal("bug in accumulation","",0);
	while(ae > a)
		*--ae = ' ';
	return 1;
}

void
chkaccum(struct field *field)
{
	if(field->coder == 0)
		field->coder = acode;
	if(field->coder != acode)
		goto bad;
	if(field->trans == 0)
		field->trans = ident;
	if(field->trans != ident)
		goto bad;
	if(field->keep == 0)
		field->keep = all;
	if(field->keep != all)
		goto bad;
	if(field->rflag)
		goto bad;
	return;
bad:
	fatal("improper modifier with -a","",0);
	return;
}

/* acode is unlike other coding functions.  instead of
   making a key in the place pointed to by op,
   it makes a list of field locations.  it might
   be more honest if op were declared void* */

struct loc { uchar *from, *to; };

int
acode(uchar *ip, uchar *op, int len, struct field *f)
{
	struct loc *lp = (struct loc*)op;
	lp->from = ip;
	lp->to = ip + len;
	if(!tab && !f->bflag &&
	   f->begin.fieldno>0 && f->begin.charno==0)
		lp->from++;
	return sizeof(*lp);
}

int
doaccum(struct rec *arec, struct rec *brec)
{
	int i;
	struct loc aloc[NF], bloc[NF];
	fieldcode(data(arec), (uchar*)aloc, arec->dlen,
		(uchar*)-1, accum, naccum);
	fieldcode(data(brec), (uchar*)bloc, brec->dlen,
		(uchar*)-1, accum, naccum);
	for(i=0; i<naccum; i++)
		if(!fieldaccum(aloc[i].from, aloc[i].to,
			       bloc[i].from, bloc[i].to)) {
			if(i==0)
				warn("sum too long; record kept: ",
			      	     (char*)data(brec), brec->dlen);
			else
				fatal("sum too long on adding: ",
			      	     (char*)data(brec), brec->dlen);
			return 0;
		}
	return 1;
}

struct rec *
listaccum(struct rec *head, struct rec *tail)
{
	struct rec *q = head;
	struct rec *p = head;
	for(;;) {
		if(q == tail)
			break;
		q = q->next;
		if(doaccum(p, q))
			p->next = q->next;
		else
			p = q;
	}
	return p;
}
