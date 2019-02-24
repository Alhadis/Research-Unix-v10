#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

static Exprtype toktype;
static int toklit;
static char *beg, *end;
static int maxid;
static Expr *e0(void);
static Expr *d0(void);
static Expr *r18(void);
static void err(char*);
static int parno;
static jmp_buf gohome;
static unsigned char *mymap;

void
egpost(re_re *r)
{
	r->maxid = maxid;
	r->backref = r->root->backref;
	r->parens = r->root->parens;
}

Expr *
eg_newexpr(Exprtype t, int l, Expr *left, Expr *right)
{
	register Expr *e = (Expr *)egmalloc(sizeof(Expr), "eg_newexpr");

	if (!e)
		return 0;
	e->type = t;
	e->parent = 0;
	e->lit = l;
	if(e->lit)
		e->id = maxid++;
	else
		e->id = 0;
	e->backref = 0;
	e->parens = 0;
	if(e->l = left){
		left->parent = e;
		if(left->backref) e->backref = 1;
		if(left->parens) e->parens = 1;
	}
	if(e->r = right){
		right->parent = e;
		if(right->backref) e->backref = 1;
		if(right->parens) e->parens = 1;
	}
	e->follow = 0;
	e->flen = 0;
	e->reallit = 0;
	return(e);
}

void
eg_lexinit(char *b, char *e)
{
	beg = b;
	end = e;
	maxid = 1;
	parno = 1;
}

void
eg_lex(void)
{
	if(beg == end){
		toktype = EOP;
		toklit = -1;
	} else switch(toklit = *beg++)
	{
	case '.':	toktype = Dot; break;
	case '*':	toktype = Star; break;
	case '+':	toktype = Plus; break;
	case '?':	toktype = Quest; break;
	case '^':	toktype = Carat; break;
	case '$':	toktype = Dollar; break;
	case '[':	toktype = Charclass; break;
	case '\n':
	case '|':	toktype = Alternate; break;
	case '(':	toktype = Lpar; break;
	case ')':	toktype = Rpar; break;
	case '\\':	toktype = Backslash;
			if(beg == end)
				err("bad \\");
			else
				toklit = *beg++;
			break;
	default:	toktype = Literal; break;
	}
}

void
eg_epr(register Expr *e, char *res, int doset)
{
	char r1[EPRINTSIZE], r2[EPRINTSIZE], rid[EPRINTSIZE];
	int ids = 0;		/* sort of a debugging flag */

	if(e == 0){
		SPR res, "!0!");
		return;
	}
	r1[0] = 0;
	if(ids)
		SPR rid, "%d:", e->id);
	else
		rid[0] = 0;
	switch(e->type)
	{
	case Literal:
		if(doset)
			eg_spr(e->flen, e->follow, r1);
		SPR res, "%s'%c'%s", rid, e->lit, r1);
		break;
	case Dot:
	case Carat:
	case Dollar:
		if(doset)
			eg_spr(e->flen, e->follow, r1);
		SPR res, "%s%c%s", rid, e->lit, r1);
		break;
	case Compcharclass:
	case Charclass:
		*res++ = '[';
		if(e->type == Compcharclass)
			*res++ = '^';
		memmove(res, (char *)e->r, (int)e->l);
		res += (int)e->l;
		*res++ = ']';
		*res = 0;
		break;
	case Cat:
		eg_epr(e->l, r1, doset);
		eg_epr(e->r, r2, doset);
		SPR res, "%s%s%s", rid, r1, r2);
		break;
	case Alternate:
		eg_epr(e->l, r1, doset);
		eg_epr(e->r, r2, doset);
		SPR res, "%s(%s|%s)", rid, r1, r2);
		break;
	case Star:
		eg_epr(e->l, r1, doset);
		SPR res, "%s(%s)*", rid, r1);
		break;
	case Plus:
		eg_epr(e->l, r1, doset);
		SPR res, "%s(%s)+", rid, r1);
		break;
	case Quest:
		eg_epr(e->l, r1, doset);
		SPR res, "%s(%s)?", rid, r1);
		break;
	case Group:
		eg_epr(e->l, r1, doset);
		SPR res, "%sG<%s>", rid, r1);
		break;
	case EOP:
		eg_epr(e->l, r1, doset);
		SPR res, "%s%s<EOP>", rid, r1);
		break;
	case Backref:
		SPR res, "%s\\%d", rid, e->lit);
		break;
	default:
		SPR res, "<undef type %d>", e->type);
		err(res);
		break;
	}
}

static void
ccl(int *count, char **str, int oldrange)
{
	register n;
	int cnt;
	char tab[256], *s;
	int range, lastc, i;

#define	TSET(b)	{if(tab[n = mymap[(b)]] == 0){ tab[n] = 1; cnt++; } }

	cnt = 0;
	memset(tab, 0, sizeof tab);
	lastc = -1;
	range = 0;
	/* scan for chars */
	for(; (beg < end); beg++){
		toklit = *beg;
		if(*beg == ']'){
			if(!oldrange)
				break;
			if(lastc >= 0)
				break;
		}
		if(*beg == '-'){
			if(lastc < 0){
				TSET('-')
				lastc = *(unsigned char *)beg;
			} else
				range = 1;
			continue;
		}
		if(*beg == '\\'){
			if(++beg >= end){
				err("unexpected eop after \\ in char class");
				beg--;
			}
		}
		if(range){
			for(i = *(unsigned char *)beg; i >= lastc; i--)
				TSET(i)
			range = 0;
		} else
			TSET(*(unsigned char *)beg)
		lastc = *(unsigned char *)beg;
	}
	if(range){
		if(oldrange)
			TSET('-')
		else
			err("unterminated range in []");
	}
	if(beg < end)
		beg++;
	else
		err("eop during ccl");
	if(cnt == 0)
		err("empty charclass");
	eg_lex();
	*count = cnt;
	*str = s = (char *)egmalloc(cnt, "charclass defn");
	if (!s)
		return;
	for(n = 0; n < 256; n++)
		if(tab[n])
			*s++ = n;
}

/*
	gre patterns:

	e0:	e1 { '|' e1 }*
	e1:	e2 { e2 }*
	e2:	e3 { '*' | '?' | '+' | \{ m,n \} }*
	e3:	lit | '.' | '^' | '$' | '(' e0 ')'
*/

static Expr *
e3(void)
{
	Expr *e;
	Exprtype t;
	int cnt;
	char *s;

	switch(toktype)
	{
	case Backslash:
		if((toklit >= '1') && (toklit <= '9')){
			e = eg_newexpr(Backref, toklit-'0', (Expr *)0, (Expr *)0);
			e->backref = 1;
		} else
			e = eg_newexpr(Literal, toklit, (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Literal:
		e = eg_newexpr(Literal, toklit, (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Dot:
		e = eg_newexpr(Dot, '.', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Carat:
		e = eg_newexpr(Carat, '^', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Dollar:
		e = eg_newexpr(Dollar, '$', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Charclass:
		t = toktype;
		if(*beg == '^'){
			t = Compcharclass;
			beg++;
		}
		ccl(&cnt, &s, 0);
		e = eg_newexpr(t, '[', (Expr *)0, (Expr *)0);
		e->l = (Expr *)cnt;	/* num of chars */
		e->r = (Expr *)s;	/* chars */
		break;
	case Lpar:
		eg_lex();
		cnt = parno++;
		e = e0();
		if(toktype == Rpar)
			eg_lex();
		else
			err("expected a ')'");
		e = eg_newexpr(Group, cnt, e, (Expr *)0);
		e->parens = 1;
		return(e);
	case EOP:
	default:
		err("expected a lit or '('");
		e = 0;
	}
	return(e);
}

static int
integer(void)
{
	int n;

	n = 0;
	while((toktype == Literal) && (toklit >= '0') && (toklit <= '9')){
		n = 10*n + toklit-'0';
		eg_lex();
	}
	return(n);
}

static Expr *
ecopy(Expr *e)
{
	Expr *ee;
	char res[256];

	if(e == 0)
		return(e);
	switch(e->type)
	{
	case Literal:
	case Dot:
	case Carat:
	case Dollar:
	case Backref:
		return(eg_newexpr(e->type, e->lit, (Expr *)0, (Expr *)0));
	case Compcharclass:
	case Charclass:
		ee = eg_newexpr(e->type, e->lit, (Expr *)0, (Expr *)0);
		ee->r = (Expr *)egmalloc((int)e->l, "expr copy");
		if (!ee->r)
			return 0;
		ee->l = e->l;
		memmove((char *)ee->r, (char *)e->r, (int)e->l);
		return(ee);
	case Cat:
	case Alternate:
		return(eg_newexpr(e->type, e->lit, ecopy(e->l), ecopy(e->r)));
	case Star:
	case Plus:
	case Quest:
	case Group:
	case EOP:
		return(eg_newexpr(e->type, e->lit, ecopy(e->l), (Expr *)0));
	default:
		SPR res, "<undef type %d>", e->type);
		err(res);
		return((Expr *)0);
	}
}

static Expr *
edup(Expr *expr, int n, int opt)
{
	if(n == 1){
		expr = ecopy(expr);
		if(opt)
			expr = eg_newexpr(Quest, 0, expr, (Expr *)0);
		return(expr);
	}
	return(eg_newexpr(Cat, 0, edup(expr, n-n/2, opt), edup(expr, n/2, opt)));
}

static Expr *
range(Expr *expr)
{
	int beg, end;
	Expr *e, *e1;

	if((toktype == Literal) && (toklit >= '0') && (toklit <= '9'))
		beg = integer();
	else
		err("expected a number in range");
	if((toktype == Literal) && (toklit == ',')){
		end = -1;
		eg_lex();
	} else
		end = -2;
	if((toktype == Literal) && (toklit >= '0') && (toklit <= '9'))
		end = integer();
	if((toktype == Backslash) && (toklit == '}'))
		eg_lex();
	else
		err("expected \\} in range");
	e1 = edup(expr, beg, 0);
	if(end == -2)
		e = e1;
	else if(end == -1)
		e = eg_newexpr(Cat, 0, e1, eg_newexpr(Star, 0, expr, (Expr *)0));
	else {
		if(end < beg)
			err("bad range specification");
		e = (end > beg)? eg_newexpr(Cat, 0, e1, edup(expr, end-beg, 1)) : e1;
	}
	return(e);
}

static Expr *
e2(void)
{
	Expr *e;
	Exprtype t;

	e = e3();
	while((toktype == Star) || (toktype == Plus) || (toktype == Quest)
			|| ((toktype == Backslash) && (toklit == '{'))){
		if((toktype == Backslash) && (toklit == '{')){
			eg_lex();
			e = range(e);
		} else {
			t = toktype;
			eg_lex();
			e = eg_newexpr(t, 0, e, (Expr *)0);
		}
	}
	return(e);
}

static Expr *
e1(void)
{
	Expr *e, *f;

	e = e2();
	while((toktype == Literal) || (toktype == Dot) || (toktype == Lpar)
			|| (toktype == Backslash) || (toktype == Dollar)
			|| (toktype == Carat) || (toktype == Charclass)){
		f = e2();
		e = eg_newexpr(Cat, 0, e, f);
	}
	return(e);
}

static Expr *
e0(void)
{
	Expr *e, *f;

	e = e1();
	while(toktype == Alternate){
		eg_lex();
		if(toktype == EOP)
			continue;
		f = e1();
		e = eg_newexpr(Alternate, 0, e, f);
	}
	return(e);
}

/*
	egrep patterns:

	d0:	d1 { '|' d1 }*
	d1:	d2 { d2 }*
	d2:	d3 { '*' | '?' | '+' }
	d3:	lit | '.' | '^' | '$' | '(' d0 ')'
*/

static Expr *
d3(void)
{
	Expr *e;
	Exprtype t;
	int cnt;
	char *s;

	switch(toktype)
	{
	case Backslash:
	case Literal:
		e = eg_newexpr(Literal, toklit, (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Dot:
		e = eg_newexpr(Dot, '.', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Carat:
		e = eg_newexpr(Carat, '^', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Dollar:
		e = eg_newexpr(Dollar, '$', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Charclass:
		t = toktype;
		if(*beg == '^'){
			t = Compcharclass;
			beg++;
		}
		ccl(&cnt, &s, 1);
		e = eg_newexpr(t, '[', (Expr *)0, (Expr *)0);
		e->l = (Expr *)cnt;	/* num of chars */
		e->r = (Expr *)s;	/* chars */
		break;
	case Lpar:
		eg_lex();
		e = d0();
		if(toktype == Rpar)
			eg_lex();
		else
			err("expected a ')'");
		return(e);
	default:
		err("expected a lit or '('");
		e = 0;
	}
	return(e);
}

static Expr *
d2(void)
{
	Expr *e;
	Exprtype t;

	e = d3();
	while((toktype == Star) || (toktype == Plus) || (toktype == Quest)){
		t = toktype;
		eg_lex();
		e = eg_newexpr(t, 0, e, (Expr *)0);
	}
	return(e);
}

static Expr *
d1(void)
{
	Expr *e, *f;

	e = d2();
	while((toktype == Literal) || (toktype == Dot) || (toktype == Lpar)
			|| (toktype == Dollar) || (toktype == Backslash)
			|| (toktype == Carat) || (toktype == Charclass)){
		f = d2();
		e = eg_newexpr(Cat, 0, e, f);
	}
	return(e);
}

static Expr *
d0(void)
{
	Expr *e, *f;

	e = d1();
	while(toktype == Alternate){
		eg_lex();
		if(toktype == EOP)
			continue;
		f = d1();
		e = eg_newexpr(Alternate, 0, e, f);
	}
	return(e);
}

/*
	grep patterns:

	r0:	r18 | '^' r18 | '^' r18 '$' | r18 '$'
	r18:	r17 { r17 }*
	r17:	r14 | r14 '*' | '\(' r18 '\)'
	r14:	lit | '.' | '*' | '\' d
*/

static Expr *
r14(void)
{
	Expr *e;
	Exprtype t;
	int cnt;
	char *s;

	switch(toktype)
	{
	case Alternate:
	case Plus:
	case Quest:
	case Star:
	case Lpar:
	case Rpar:
	case Dollar:
	case Carat:
	case Literal:
		e = eg_newexpr(Literal, toklit, (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Backslash:
		if((toklit >= '1') && (toklit <= '9')){
			e = eg_newexpr(Backref, toklit-'0', (Expr *)0, (Expr *)0);
			e->backref = 1;
		} else {
			e = eg_newexpr(Literal, toklit, (Expr *)0, (Expr *)0);
			e->reallit = 1;
		}
		eg_lex();
		break;
	case Dot:
		e = eg_newexpr(Dot, '.', (Expr *)0, (Expr *)0);
		eg_lex();
		break;
	case Charclass:
		t = toktype;
		if(*beg == '^'){
			t = Compcharclass;
			beg++;
		}
		ccl(&cnt, &s, 1);
		e = eg_newexpr(t, '[', (Expr *)0, (Expr *)0);
		e->l = (Expr *)cnt;	/* num of chars */
		e->r = (Expr *)s;	/* chars */
		break;
	default:
		err("expected a one-char RE");
		eg_lex();		/* make sure we don't loop */
		e = 0;
	}
	return(e);
}

static Expr *
r17(void)
{
	Expr *e;
	int cnt;

	if((toktype == Backslash) && (toklit == '(')){
		eg_lex();
		cnt = parno++;
		e = r18();
		if((toktype == Backslash) && (toklit == ')'))
			eg_lex();
		else
			err("expected a closing \\)");
		e = eg_newexpr(Group, cnt, e, (Expr *)0);
		e->parens = 1;
	} else {
		e = r14();
		if(toktype == Star){
			e = eg_newexpr(Star, 0, e, (Expr *)0);
			eg_lex();
		}
	}
	return(e);
}

static Expr *
r18(void)
{
	Expr *e, *f;

	e = r17();
	while(toktype != EOP){
		if((toktype == Backslash) && (toklit == ')'))
			break;
		f = r17();
		e = eg_newexpr(Cat, 0, e, f);
	}
	return(e);
}

static Expr *
r0(void)
{
	Expr *e, *e1;

	if(toktype == Carat){
		e1 = eg_newexpr(Carat, '^', (Expr *)0, (Expr *)0);
		eg_lex();
	} else
		e1 = 0;
	if(toktype == EOP)
		e = e1;
	else {
		e = r18();
		/* did we see a dollar that is not a literal? */
		if(e && (toktype == EOP)){
			/* singleton dollar */
			if((e->type == Literal) && (e->lit == '$'))
				e->type = Dollar;
			/* any other dollar */
			if((e->type == Cat) && !e->r->reallit && (e->r->type == Literal)
					&& (e->r->lit == '$'))
				e->r->type = Dollar;
		}
		if(e1){
			if(e)
				e = eg_newexpr(Cat, 0, e1, e);
			else
				e = e1;
		}
	}
	if(toktype == Dollar){
		if(e)
			e = eg_newexpr(Cat, 0, e, eg_newexpr(Dollar, '$', (Expr *)0, (Expr *)0));
		else
			e = eg_newexpr(Dollar, '$', (Expr *)0, (Expr *)0);
		eg_lex();
	}
	return(e);
}

Expr *
eg_eall(enum Parsetype type, unsigned char *map)
{
	Expr *e;

	mymap = map;
	if(setjmp(gohome) == 0){
		if(type == egrepparse)
			while(toktype == Alternate)	/* bogus but user-friendly */
				eg_lex();
		switch(type)
		{
		case greparse:		e = e0(); break;
		case grepparse:		e = r0(); break;
		case egrepparse:	e = d0(); break;
		}
		if(type == egrepparse)
			while(toktype == Alternate)	/* bogus but user-friendly */
				eg_lex();
		if(toktype != EOP)
			err("expected end of pattern");
	} else
		e = 0;
/*{char buf1[4096]; e, buf1, 0); print("e='%s'\n", buf1);}/**/
	return(e);
}

void
eg_spr(long c, int *p, register char *buf)
{
	if(c > 0){
		*buf++ = '{';
		*buf = 0;
		while(--c > 0){
			SPR buf, "%d,", *p++);
			buf = strchr(buf, 0);
		}
		SPR buf, "%d}", *p);
	} else
		SPR buf, "{}");
}

static void
err(char *s)
{
	char buf[4096];

	if(toklit < 0)
		SPR buf, "expression error: %s but got end of expression", s);
	else
		SPR buf, "expression error: %s near '%c'", s, toklit);
	re_error(buf);
	longjmp(gohome, 1);
}
