#include <stdio.h>
#include <math.h>
#include "trace.h"
#include "trace.d"

#define DEBUG	0

#define setv	 1
#define addeq	 2
#define subeq	 3
#define muleq	 4
#define diveq	 5
#define modeq	 6
#define plus	 7
#define minus	 8
#define times	 9
#define div	10
#define mod	11
#define power	12
#define uminus	13
#define gt	14
#define lt	15
#define ge	16
#define le	17
#define eq	18
#define ne	19
#define land	20
#define lor	21
#define lnot	22
#define princ	23
#define prdec	24
#define poinc	25
#define podec	26

#define OP	0
#define NM	1

#define unary(c)	(c == uminus || c == lnot || c >= princ)
#define binary(c)	!unary(c)

 extern struct REVPOL **expr;
 extern int nexpr, nrvars, *globvars;
 extern struct VARPARS *procpars;

 struct REVPOL *rev;
 int curproc, revp;

 struct {
	char how;
	short whichvar;
	short oldvalue;
 } storewhere;

evalexpr(n, pr, b)
	struct FREEZE *b;
{ int i;
	if (n == NONE)
		return 0;
	if (n < 0 || n >= nexpr)
		whoops("unknown expression");

	rev = expr[n];
	revp = 0;
	curproc = pr;
	storewhere.how = 0;

	if ((i = getval()) < 0)
		i += 3*MANY;
	else
	 	i -= 3*MANY;	/* should return stripped constant */
#if DEBUG
	fprintf(stderr, "evaluated expr %d, process %d, result %d\n", n, pr, i);
#endif

	b->whichvar = storewhere.whichvar;
	b->oldvalue = storewhere.oldvalue;

	return storewhere.how;
}

evalcond(n, pr)
{ int i;
	if (n < 0 || n >= nexpr)
	{	fprintf(stderr, "%d\n", n);
		whoops("unknown expression - evalcond");
	}

	rev = expr[n];
	revp = 0;
	curproc = pr;
	storewhere.how = 0;

	if ((i = getval()) < 0)
		i += 3*MANY;
	else
	 	i -= 3*MANY;	/* should return stripped constant */
#if DEBUG
	fprintf(stderr, "evaluated cond %d, process %d, result %d\n", n, pr, i);
#endif
	if (storewhere.how != 0)
		whoops("illegal assignment in expression");

	return i;
}

unoper(c, n)
{ int i = convert(n, curproc);
	switch (c)
	{	case uminus:	return (-i);
		case lnot  :	return (!i);
		case princ :	remem(n); setvar(n, i+1); return i;
		case prdec :	remem(n); setvar(n, i-1); return i;
		case poinc :	remem(n); return setvar(n, i+1);
		case podec :	remem(n); return setvar(n, i-1);
		default    :	whoops("unknown unary operator");
	}
}

bioper(c, n, m)	
{ int i, j;
  double a, b;

	if (c != setv)
		i = convert(n, curproc);
	j = convert(m, curproc);
	switch (c)
	{	case setv:	remem(n); return setvar(n, j);
		case addeq:	remem(n); return setvar(n, i+j);
		case subeq:	remem(n); return setvar(n, i-j);
		case muleq:	remem(n); return setvar(n, i*j);
		case diveq:	remem(n); return setvar(n, i/j);
		case modeq:	remem(n); return setvar(n, i%j);
		case plus:	return (i+j);
		case minus:	return (i-j);
		case times:	return (i*j);
		case div:	return (i/j);
		case mod:	return (i%j);
		case gt	:	return (i>j);
		case lt	:	return (i<j);
		case ge	:	return (i>=j);
		case le	:	return (i<=j);
		case eq	:	return (i==j);
		case ne	:	return (i!=j);
		case land:	return (i&&j);
		case lor:	return (i||j);
		default:	whoops("unkown binary operator");
	}
}

getval()
{ int tok = revp++;
  int res;

	if (rev[tok].toktyp == NM)
		res = rev[tok].tokval;
	else
	{	if (unary(rev[tok].tokval))
			res = unoper(rev[tok].tokval, getval());
		else
			res = bioper(rev[tok].tokval, getval(), getval());

		if (res < 0)
			res -= 3*MANY;
		else
			res += 3*MANY;
	}

	return res;
}

setvar(which, v)
{ int u = wapper(which, curproc);
  int towhat = v;

	if (u >= 2*MANY || u < 0)
		whoops("lhs of assignment not a variable");

	if (u < MANY)
	{	if (u >= nrvars)
		{	fprintf(stderr, "setvar %d %d %d\n", which, v, u);
			whoops("illegal assignment");
		}

		globvars[u] = (short) towhat;
	}
	else
	{	u -= MANY;
		if (u >= (int) procpars[curproc].nrlvars)
			whoops("unknown local variable");
		procpars[curproc].lvarvals[u]  = (short) towhat;
	}

	return towhat;
}

remem(u)
{	if (storewhere.how != 0)
		whoops("multiple assignment in expression");

	if (u < MANY)
	{
#if DEBUG
	fprintf(stderr, "setting global %d\n", u);
#endif
		storewhere.whichvar = (short) u;
		storewhere.oldvalue = globvars[u];
		storewhere.how = GV;
	}
	else
	{	storewhere.whichvar = (short) u;

		u -= MANY;
#if DEBUG
	fprintf(stderr, "setting local %d\n", u);
#endif
		storewhere.oldvalue = procpars[curproc].lvarvals[u];
		storewhere.how = LV;
	}
}
