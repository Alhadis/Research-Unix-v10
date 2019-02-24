#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"

#define GCHDRSIZE	(sizeof(struct strarea))
#define TYPEMASK	017
#define gchdr(p)	((struct strarea *)(p))

struct strarea {
	unsigned	x_len;
	String		*x_next;
	};

static char	*typenames[] = {
	"", "integer", "real", "string", "", "function", "for", "gosub",
	"any expression", "integer expression", "real expression",
	"string expression", "", "", "", ""
	};

double	popfloat();
Stkptr	push(), pop(), nextframe();
char	*allocstr();


/*
 * push --- push frame pointed to by sp onto stack
 */

Stkptr push(sp)
Stkptr	sp;
{
	register Stkptr p;
	register char	*s;

	p = sp;
	s = stkptr - p->k_len;
	if (s < stkbase)
		err("stack overflow");
	if (tflg) {
		register int	i;

		fprintf(stderr, "push: type %d, length %d:",
				p->k_type, p->k_len);
		for (i = 0; i < p->k_len; ++i)
			fprintf(stderr, " %o", ((char *)sp)[i]&0377);
		putc('\n', stderr);
		}
	move(p->k_len, p, s);
	stkptr = (char *)s;
	return ((Stkptr)s);
}


/*
 * clrstk --- reinitialize the stack pointer
 */

clrstk()
{
	stkptr = stktop;
	*stkptr-- = 0;		/* k_len = 0 */
	*stkptr = 0;		/* k_type = 0 */
	restore();
}


/*
 * restore --- reset the DATA pointer to the beginning
 */

restore()
{

		data.k_un.k_gosub.g_inptr = NULL;
		data.k_un.k_gosub.g_curline = (Linep)lines;
}


/*
 * pop --- pop the top frame off the stack and return a pointer to it;
 *	   check that the frame is of the specified type
 */

Stkptr pop(type)
{
	register Stkptr s;
	register int	i;

	s = (Stkptr)stkptr;
	if (s->k_type == 0)
		badstk(0);
	if (tflg) {
		fprintf(stderr, "pop: type %d, length %d",
				s->k_type, s->k_len);
		for (i = 0; i < s->k_len; ++i)
			fprintf(stderr, " %o", ((char *)s)[i]&0377);
		putc('\n', stderr);
		}
	if (s->k_type != type && type != ANYTYPE)
		badstk(type);
	stkptr += s->k_len;
	return (s);
}


/*
 * pushstring --- push a string onto the stack
 */

Stkptr pushstring(ptr, len)
char	*ptr;
{
	Stkfr	s;

	s.k_type = STRINGEXPR;
	s.k_len = STRFRLEN;
	s.k_un.k_str.s_ptr = ptr;
	s.k_un.k_str.s_len = len;
	return (push(&s));
}


/*
 * pushfloat --- push a floating point value onto the stack
 */

Stkptr pushfloat(f)
double	f;
{
	register Stkptr s;
	Stkfr		fe;

	s = (Stkptr)(stkptr - DBLFRLEN);
	if ((char *)s < stkbase)
		badstk(0);
	if (tflg)
		fprintf(stderr, "pushfloat: %f\n", f);
	stkptr = (char *)s;
	s->k_type = FLOATEXPR;
	s->k_len = DBLFRLEN;
	s->k_un.k_dbl = f;
	return (s);
}


/*
 * popfloat --- pop a floating point value off the stack
 */

double popfloat()
{
	register Stkptr s;

	s = (Stkptr)stkptr;
	if (s->k_type != FLOATEXPR)
		badstk(FLOATEXPR);
	if (tflg)
		fprintf(stderr, "popfloat: %f\n", s->k_un.k_dbl);
	stkptr += s->k_len;
	return (s->k_un.k_dbl);
}


/*
 * badstk --- report a bad stack frame
 */

badstk(type)
{
	register int	i;

	if ((i = ((Stkptr)stkptr)->k_type) == 0)
		err("stack underflow");
	if (type)
		err("%s value or variable expected; got %s",
				typenames[type & 07], typenames[i & 07]);
	err("bad stack frame");
}


#ifndef popint
/*
 * popint --- pop an integer value off the stack
 */

popint()
{

	return ((int)popfloat());
}
#endif


/*
 * popstring --- pop a string off the stack
 */

popstring(sptr, lptr)
char	**sptr;
int	*lptr;
{
	register Stkptr s;

	s = pop(STRINGEXPR);
	*sptr = s->k_un.k_str.s_ptr;
	*lptr = s->k_un.k_str.s_len;
}


/*
 * allocstr --- allocate and copy string into string space;
 */

char *allocstr(ptr, len, mlen)
char	*ptr;
{
	register char	*s;
	register int	l;

	l = len + GCHDRSIZE;
	if (l & 1)
		++l;		/* make it even */
	if (strptr + l >= endstring)
		collect();
	s = strptr;
	strptr += l;
	gchdr(s)->x_len = len;
	gchdr(s)->x_next = (String *)NULL;
	s += GCHDRSIZE;
	if (mlen)
		move(mlen, ptr, s);
	return (s);
}


/*
 * collect --- collect garbage in string space
 */

collect()
{
	register Symptr s;
	register Stkptr k;
	register char	*p, *q;
	register int	l, n;
	String		*sp, *tsp;

	for (s = chains[0][STRING]; s; s = s->v_next) {
		if (tflg)
			fprintf(stderr, "mark var %.2s ", s->v_name);
		mark(&s->v_un.v_str, 1);
		}
	for (s = chains[1][STRING]; s; s = s->v_next) {
		if (tflg)
			fprintf(stderr, "mark vec %.2s ", s->v_name);
		for (n = 1, l = s->v_nsubs; --l >= 0; )
			n *= s->v_un.v_vec.v_subsc[l];
		mark(s->v_un.v_vec.v_vecun.v_strvec, n);
		}
	for (k = (Stkptr)stkptr; k->k_type; k = nextframe(k))
		if (k->k_type == STRINGEXPR) {
			if (tflg)
				fprintf(stderr, "mark stk %X ", k);
			mark(&k->k_un.k_str, 1);	/* mark stack frame */
			}
	for (q = p = strspace; p < strptr; p += l) {
		if ((sp = gchdr(p)->x_next) == (String *)NULL) {
			l = gchdr(p)->x_len + GCHDRSIZE;
			if (l & 1)
				++l;
			}
		else {
			l = sp->s_len + GCHDRSIZE;
			if (l & 1)
				++l;
			if (p != q) {
				move(l, p, q);
				if (tflg)
					fprintf(stderr, "move %l %X %X\n",
							l, p, q);
				}
			gchdr(q)->x_len = sp->s_len;
			gchdr(q)->x_next = (String *)NULL;
			for (; sp != (String *)NULL; sp = tsp) {
				tsp = (String *)sp->s_ptr;
				sp->s_ptr = q + GCHDRSIZE;
				if (tflg)
					fprintf(stderr, "reset %X = %X %d\n",
							sp, sp->s_ptr, sp->s_len);
				}
			q += l;
			}
		}
	l = endstring - q;	/* amount now free */
	if (tflg)
		fprintf(stderr, "%d bytes recovered\n", l);
	strptr = q;
	if (l < MAXSTRSPACE / 10)
		err("not enough free space");
}


/*
 * mark --- mark a string as being used
 *	    this is accomplished by storing a pointer to the
 *	    String structure referring to this string in the
 *	    x_next field of the string header.
 *	    multiple references to the same string are linked
 *	    together through the s_ptr field of the String structures.
 */

mark(ptr, n)
String	*ptr;
{
	String		*s;
	register char	*p;

	for (s = ptr; --n >= 0; ++s) {
		p = s->s_ptr;
		if (isstring(p)) {
			p -= GCHDRSIZE; 	/* point to GC header */
			if (tflg)
				fprintf(stderr, "mark %X %d\n", p, s->s_len);
			if (s->s_len != gchdr(p)->x_len)
				err("bad string list");
			s->s_ptr = (char *)gchdr(p)->x_next;
			gchdr(p)->x_next = s;
			}
		}
}


/*
 * storestring --- store string at top of stack
 */

storestring(v)
String	*v;
{
/*
 * the old value of the variable is changed last, so that in the
 * case of an error, it is still available, allowing re-execution
 * of the line with the error.
 * if storing a pointer will suffice (a$ = b$)
 * then only store the pointer, otherwise allocate more space
 * and copy the string.
 */
	register char	*ptr;
	register int	len;
	register Stkptr s;

	s = (Stkptr)stkptr;
	len = s->k_un.k_str.s_len;
	ptr = s->k_un.k_str.s_ptr;
	if (!isstring(ptr)
			|| ((unsigned)ptr&1) != 0
			|| gchdr(ptr - GCHDRSIZE)->x_len != len) {
		ptr = allocstr(NULL, len, 0);	/* alloc but don't copy */
		move(len, s->k_un.k_str.s_ptr, ptr);
		}
	v->s_ptr = ptr;
	v->s_len = len;
	pop(STRINGEXPR);
}


/*
 * nextframe --- return ptr to frame after one pointed to by sp
 */

Stkptr nextframe(sp)
register Stkptr	sp;
{

	return ((Stkptr)((char *)sp + sp->k_len));
}
