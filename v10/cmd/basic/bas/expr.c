#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

#define NOOP		0
#define TYPEMASK	07
#define min(a,b)	((a)<(b)?(a):(b))
#define prio(x) 	priority[-x]
#define between(a,b,c)	((a)<=(b)&&(b)<=(c))
#define FIRSTOP 	OR
#define LASTOP		PLUS
#define OPCOUNT 	20
#define MAXOP		10	
#define MAXDIGITS	64

static char	priority[OPCOUNT];
static char	priodefs[] = {
	20,	EXP,
	11,	MUL,	DIV,
	10,	PLUS,	MINUS,
	9,	EQ,	NE,	GE,	LE,	LT,	GT,
	7,	AND,
	6,	OR,
	0,	NOOP
	};

Stkptr	pushvar(), nextframe();
Symptr	getvar();
char	*allocstr(), *getsvar();
double	popfloat(), modf(), atof(), cvtnumber(), exp(), log(), fabs(), floor();


/*
 * initprio --- initialize the operator-priority map
 */

initprio()
{
	register int	pri;
	register char	*p;

	for (p = priodefs; p < priodefs + sizeof priodefs; p++)
		if (*p >= 0)
			pri = *p;
		else
			prio(*p) = pri;
	if (tflg)
		for (pri = FIRSTOP; pri <= LASTOP; pri++)
			fprintf(stderr, "priority[%d] == %d\n", pri, prio(pri));
}


/*
 * expr --- interpret an expression, push result onto stack
 */

expr()
{
	register int	op, evop;
	register char	*opptr;
	char		opstk[MAXOP];
	int		c;

#define getop() 	(between(FIRSTOP,*inptr,LASTOP)? *inptr++ : 0)
#define getitem()	{\
			c = *inptr;\
			if (isalpha(c))\
				pushvar();\
			else if (isdigit(c))\
				pushfloat(cvtnumber(&inptr, MAXINT));\
			else\
				item();\
			}

	getitem();
	if ((op = getop()) == 0)
		return;
	opptr = opstk;
	*opptr++ = NOOP;
	for (;;)
		if (prio(opptr[-1]) >= prio(op)) {
			evop = *--opptr;
			if (evop == NOOP)
				return;
			eval(evop);
			}
		else {
			getitem();
			*opptr++ = op;
			if (opptr > opstk + MAXOP)
				err("expression too complex");
			op = getop();
			}
}


/*
 * eval --- evaluate the specified operator using stacked operands
 */

eval(op)
{
	double		fp1, fp2;
	register Stkptr s;

	s = (Stkptr)stkptr;
	if (s->k_type == STRINGEXPR) {
		streval(op);
		return;
		}
	fp2 = popfloat();
	s = (Stkptr)stkptr;
	if (s->k_type != FLOATEXPR)
		badstk(FLOATEXPR);
	fp1 = s->k_un.k_dbl;

	switch(op) {
	case PLUS:
		fp1 += fp2;
		break;
	case MINUS:
		fp1 -= fp2;
		break;
	case MUL:
		fp1 *= fp2;
		break;
	case DIV:
		fp1 /= fp2;
		break;
	case EXP:
		if (fp1 > 0)
			fp1 = exp(log(fp1) * fp2);
		else if (fp1 != 0 && fp2 == 0)
			fp1 = 1;
		else if (fp1 == 0 && fp2 != 0)
			fp1 = 0;
		else if (fp1 < 0 && (fp2 - 2*floor(fp2 / 2.)) == 0)
			fp1 = exp(log(fabs(fp1)) * fp2);
		else if (fp1 < 0 && (fp2 - floor(fp2)) == 0)
			fp1 = -exp(log(fabs(fp1)) * fp2);
		else {fp1 = 0;
		fprintf(stderr, "0 to the 0 power and negative numbers");
		fprintf(stderr, " to noninteger powers \n");
		fprintf(stderr, " can not be calculated. 0 was returned.\n");
			};
		break;
	case GT:
		fp1 = (fp1 > fp2);
		break;
	case LT:
		fp1 = (fp1 < fp2);
		break;
	case LE:
		fp1 = (fp1 <= fp2);
		break;
	case GE:
		fp1 = (fp1 >= fp2);
		break;
	case EQ:
		fp1 = (fp1 == fp2);
		break;
	case NE:
		fp1 = (fp1 != fp2);
		break;
	case OR:
		fp1 = ((fp1 != 0) || (fp2 != 0));
		break;
	case AND:
		fp1 = ((fp1 != 0) && (fp2 != 0));
		break;
	default:
		err("bad operator");
		}
	s->k_un.k_dbl = fp1;
}


/*
 * streval --- evaluate an operator with string operands
 */

streval(op)
{
	register int	i;
	char		*ptr1, *ptr2;
	int		len1, len2;

	if (op == PLUS) {
		concat();
		return;
		}
	popstring(&ptr2, &len2);
	popstring(&ptr1, &len1);
	i = strcmpn(ptr1, len1, ptr2, len2);

	switch(op) {
	case GT:
		i = (i > 0);
		break;
	case LT:
		i = (i < 0);
		break;
	case LE:
		i = (i <= 0);
		break;
	case GE:
		i = (i >= 0);
		break;
	case EQ:
		i = (i == 0);
		break;
	case NE:
		i = (i != 0);
		break;
	default:
		err("bad operator");
		}
	pushint(i);
}


/*
 * strcmpn --- compare fixed length strings
 */

strcmpn(ptr1, len1, ptr2, len2)
char	*ptr1, *ptr2;
{
	register int	l;
	register char	*p1, *p2;

	l = min(len1, len2);
	len1 -= l;
	len2 -= l;
	p1 = ptr1;
	p2 = ptr2;
	while (l > 0 && *p1++ == *p2++)
		--l;
	if (l != 0)
		return(*--p1 - *--p2);
	while (len1 > 0) {	/* string 1 longer */
		if (*p1++ != ' ')
			return(*--p1 - ' ');
		--len1;
		}
	while (len2 > 0) {	/* string 2 longer */
		if (*p2++ != ' ')
			return(' ' - *--p2);
		--len2;
		}
	return(0);		/* strings are equal */
}


/*
 * item --- interpret a basic expression element
 */

item()
{
	register Stkptr s;
	register int	c;
	
	switch((c = *inptr++)) {

	case FN:	/* function call */
		--inptr;	/* back up to FN token */
		fn();
		break;
	case PLUS:	/* unary + */
		item();
		break;
	case MINUS:	/* unary - */
		item();
		s = (Stkptr)stkptr;
		if (s->k_type != FLOATEXPR)
			err("float required");
		s->k_un.k_dbl = -s->k_un.k_dbl;
		break;
	case LPAR:	/* parenthesized expr */
		expr();
		expectc(RPAR);
		break;
	case QUOTE:	/* string constant */
	case PRIME:
		strconst(c);
		break;
	default:	/* float constant, variable, or builtin func */
		--inptr;
		if (isdigit(c) || c == '.')
			pushfloat(cvtnumber(&inptr, MAXINT));
		else if (isalpha(c))
			pushvar();
		else if (function())
			;
		else
			err("bad operand");
		}
}


/*
 * cvtnumber --- convert a string to floating point
 */

double cvtnumber(ptr, len)
char		**ptr;
register int	len;
{
	register char	*n, *p;
	char		numbuff[MAXDIGITS];
	double		f;
	
	p = *ptr;
	n = numbuff;
	if (*p == '+') {
		++p;
		--len;
		}
	else if (*p == '-') {
		*n++ = *p++;
		--len;
		}
	for (; isdigit(*p) || *p == '.' || *p == 'e'; ) {
		if (n >= &numbuff[MAXDIGITS-1]) {
			*ptr = p;
			err("too many digits");
			}
		*n++ = *p++;
		if (--len <= 0)
			break;
		}
	*n = 0;
	f = atof(numbuff);
	*ptr = p;
	return(f);
}


/*
 * strconst --- interpret a string constant in an expression
 */

strconst(c)
{
	Stkfr		s;
	
	s.k_un.k_str.s_ptr = inptr;
	while (*inptr && *inptr != c)
		++inptr;
	s.k_un.k_str.s_len = inptr - s.k_un.k_str.s_ptr;
	s.k_len = STRFRLEN;
	s.k_type = STRINGEXPR;
	push(&s);
	if (*inptr == c)
		++inptr;
}


/*
 * badtype --- report a data type error
 */

badtype()
{

	err("bad type");
}


/*
 * pushvar --- push the value of a variable onto the stack
 */

Stkptr pushvar()
{
	register char	*s;
	register int	i;
	int		type;
	
	s = getsvar(&type);
	
	switch(type) {
	case STRING:
		pushstring(((String *)s)->s_ptr, ((String *)s)->s_len);
		break;
	case INT:
		pushint(*(int *)s);
		break;
	case FLOAT:
		if (SINGLE)
			pushfloat(*(float *)s);
		else
			pushfloat(*(double *)s);
		break;
	default:
		err("value expected");
		}
	return((Stkptr)stkptr);
}


/*
 * getsc --- convert multi-dimensional subscript to single-dimensional
 */

getsc(v)
register Symptr v;
{
	register int	i, j, n;
	
	if (nsubs != v->v_nsubs)
		err("wrong number of subscripts");
	for (j = 0, n = 0;; ) {
		i = subsc[j];
		if (i < 1 || i > v->v_un.v_vec.v_subsc[j])
			err("subscript %d out of range (%d)", j + 1, i);
		n += i - 1;
		if (++j >= nsubs)
			break;
		n *= v->v_un.v_vec.v_subsc[j];
		}
	return(n);
}


/*
 * intvalued --- determine if a floating point number is integral
 */

intvalued(f)
double	f;
{
	double	ipart;

	return(modf(f, &ipart) == 0.0);
}


/*
 * let --- interpret a LET statement
 */

let()
{
	register Stkptr s;
	register char	*v;
	register int	i;
	int		type, stype;
	

	v = getsvar(&type);	/* v points to value in variable */

	expectc(EQ);
	expr();
	s = (Stkptr)stkptr;	/* get the expression */

	stype = s->k_type & TYPEMASK;

	if (stype != type && (stype == STRING || type == STRING))
		mixed();

	switch(type) {
		case FLOAT:
			if (SINGLE)
				*(float *)v = popfloat();
			else
				*(double *)v = popfloat();
			break;
		case INT:
			*(int *)v = popint();
			/* pushfloat(*(double *)v); */
			break;
		case STRING:
			storestring(v);
			break;
		default:
			err("invalid variable");
		}

}


/*
 * cvt --- convert value at top of stack to type "type"
 */

cvt(type)
{
	register Stkptr s;
	register int	stype;
	
	s = (Stkptr)stkptr;
	type &= TYPEMASK;
	stype = s->k_type & TYPEMASK;
	if (stype == type)
		return;
	switch(type) {
	case FLOAT:
		if (stype == INT) {
			pushfloat((double)popint());
			return;
			}
		break;
	case INT:
		if (stype == FLOAT) {
			pushint((int)popfloat());
			return;
			}
		}
	err("invalid type conversion");
}


/*
 * concat --- concatenate the strings at the top of the stack
 *	      don't pop the strings until they have been copied
 *	      to their new location in case garbage collection
 *	      takes place during allocation
 */

concat()
{
	register Stkptr s, p;
	register char	*q;
	int		slen, plen;
	
	s = (Stkptr)stkptr;
	slen = s->k_un.k_str.s_len;
	p = nextframe(s);
	plen = p->k_un.k_str.s_len;
	if (p->k_type != STRINGEXPR)
		mixed();
	if (slen + plen > MAXSTRING)
		err("string too long");
	q = allocstr(NULL, slen + plen, 0);
	move(plen, p->k_un.k_str.s_ptr, q);
	move(slen, s->k_un.k_str.s_ptr, q + plen);
	pop(STRINGEXPR);		/* get rid of topmost string */
	p->k_un.k_str.s_ptr = q;	/* replace the other */
	p->k_un.k_str.s_len += slen;
}


/*
 * getsvar --- return pointer to value of a variable
 *	       return its type in "type"
 */

char *getsvar(type)
int	*type;
{
	register char	*p;
	register Symptr v;
	register int	i;
	
	v = getvar(type, NO);
	if (nsubs == 0)
		return((char *)&v->v_un);	/* not subscripted */
	i = getsc(v);		/* get and check subscript */
	switch(*type) {
	case FLOAT:
		if (SINGLE)
			p = (char *)&v->v_un.v_vec.v_vecun.v_fltvec[i];
		else
			p = (char *)&v->v_un.v_vec.v_vecun.v_dblvec[i];
		break;
	case INT:
		p = (char *)&v->v_un.v_vec.v_vecun.v_intvec[i];
		break;
	case STRING:
		p = (char *)&v->v_un.v_vec.v_vecun.v_strvec[i];
		break;
	default:
		badtype();
		}
	return(p);
}


/*
 * mixed --- report mixed data mode error
 */

mixed()
{

	err("mixed modes");
}
