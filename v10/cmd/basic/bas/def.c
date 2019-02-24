#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

Symptr	getvar();
Stkptr	nextframe();


/*
 * def --- define a function (just save input pointer for now)
 */

def()
{
	register Symptr v;
	int		type;

	if (*inptr != FN)
		badsyn();
	v = getvar(&type, NO);
	v->v_un.v_fn.fn_curline = curline;
	v->v_un.v_fn.fn_inptr = inptr;
	while (!endtest())
		++inptr;
}


/*
 * fn --- invoke a previously defined function
 */

fn()
{
	register Symptr f;
	register char	*s;
	Stkfr		var;
	int		type, vtype, i, n;
	char		*base, *v, *saveptr;
	Linep		saveline;

	base = stkptr;
	f = getvar(&type, NO);
	if (f->v_un.v_fn.fn_inptr == NULL)
		err("function undefined");
	expectc(LPAR);
	for (n = 0; !endtest(); ) {
		expr();
		++n;
		if (*inptr == RPAR)
			break;
		expectc(COMMA);
		}
	expectc(RPAR);
	s = stkptr;

	saveline = curline;
	saveptr = inptr;
	curline = f->v_un.v_fn.fn_curline;
	inptr = f->v_un.v_fn.fn_inptr;
	expectc(LPAR);
	for (i = 1; i <= n; ++i) {
		var.k_un.k_symp = getvar(&vtype, NO);
		var.k_type = vtype;
		var.k_len = VARFRLEN;
		push(&var);
		if (i < n)
			expectc(COMMA);
		}
	expectc(RPAR);
	expectc(EQ);
	v = stkptr;		/* variables on stack */
	exchange(s, v, n);
	expr();
	endchk();
	exchange(s, v, n);
	s = stkptr;		/* the current expression */
	stkptr = base;		/* restore the original stack */
	push((Stkptr)s);	/* push the result onto the stack */
	curline = saveline;
	inptr = saveptr;
}


/*
 * exchange --- swap n contiguous stack frames pointed to by sp and vp
 */

exchange(sp, vp, n)
char	*sp, *vp;
{
	register Stkptr s, v;
	register Symptr p;
	int		i, len;
	char		*ptr;
	double		f;

	s = (Stkptr)sp;
	v = (Stkptr)vp;
	for (i = 0; i < n; ++i) {
		p = v->k_un.k_symp;		/* pointer to variable */
		if (s->k_type != v->k_type + EXPR)
			err("types don't match");
		if (tflg)
			fprintf(stderr, "exchange value %.2s ", p->v_name);
		switch(v->k_type) {
		case FLOAT:
			if (tflg) {
				f = (SINGLE)? p->v_un.v_float : p->v_un.v_double;
				fprintf(stderr, " %.6f <==> %.6f\n",
						f, s->k_un.k_dbl);
				}
			f = s->k_un.k_dbl;
			if (SINGLE) {
				s->k_un.k_dbl = p->v_un.v_float;
				p->v_un.v_float = f;
				}
			else {
				s->k_un.k_dbl = p->v_un.v_double;
				p->v_un.v_double = f;
				}
			break;
		case INT:
			if (tflg)
				fprintf(stderr, " %d <==> %d\n",
					p->v_un.v_int, (int)s->k_un.k_dbl);
			f = s->k_un.k_dbl;
			s->k_un.k_dbl = p->v_un.v_int;
			p->v_un.v_int = f;
			break;
		case STRING:
			ptr = s->k_un.k_str.s_ptr;
			len = s->k_un.k_str.s_len;
			s->k_un.k_str.s_ptr = p->v_un.v_str.s_ptr;
			s->k_un.k_str.s_len = p->v_un.v_str.s_len;
			p->v_un.v_str.s_ptr = ptr;
			p->v_un.v_str.s_len = len;
			break;
			}
		s = nextframe(s);
		v = nextframe(v);
		}
}
