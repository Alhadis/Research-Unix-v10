/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:for.c	1.0	*/
#include "picasso.h"
#include "y.tab.h"

#define	SLOP	1.001

typedef struct {
	struct	symtab	*sym;	/* index variable */
		float	to;	/* limit */
		float	by;
		int	op;	/* operator */
		char	*str;	/* string to push back */
} For;

For	forstk[10];	/* stack of for loops */
For	*forp = forstk;	/* pointer to current top */

forloop(var, from, to, op, by, str)	/* set up a for loop */
	char *var;
	double from, to, by;
	int op;
	char *str;
{
	if (++forp >= forstk+10)
		fatal("for loop nested too deep");

/* note: actually we here want to take a vector variable and construct its   */
/*	 values directly, then access them one at a time below; the current  */
/*	 version is a temporary concession to old pic's version of the 'for' */

	forp->sym = findvar(var, VARNAME);
	if (forp->sym->s_dim && forp->sym->s_val.a)
		free(forp->sym->s_val.a);
	forp->sym->s_dim = 0;
	forp->sym->s_val.f = from;
	forp->to = to;
	if (by == 0.)
		fatal("step size of 0 not allowed");

	/*  For additive or subtractive step, make sure step is positive.
	    Otherwise, make sure step is greater than 1 in absolute value. */
	if ((op == ' ' || op == '+') && by < 0.) {
		op = '-';
		by = -by;
	}
	else if (op == '-' && by < 0.) {
		op = '+';
		by = -by;
	}
	else if (op == '*' && fabs(by) < 1.) {
		op = '/';
		by = 1 / by;
	}
	else if (op == '/' && fabs(by) < 1.) {
		op = '*';
		by = 1 / by;
	}
	forp->op = op;
	forp->by = by;
	forp->str = str;
	nextfor();
	unput('\n');
}

nextfor()	/* do one iteration of a for loop */
{
	int	done;
	float	v = forp->sym->s_val.f;

	switch (forp->op) {
	    case '+':
	    case ' ':
		done = v > forp->to;
		break;
	    case '-':
		done = v < forp->to;
		break;
	    case '*':
		done = v * forp->to < 0. ? (fabs(forp->by) > 0)
					 : (fabs(v) > fabs(forp->to)) ;
		break;
	    case '/':
		done = v * forp->to < 0. ? (fabs(forp->by) > 0)
					 : (fabs(v) < fabs(forp->to)) ;
		break;
	}
	if (done) {
		free(forp->str);
		if (--forp < forstk)
			fatal("forstk popped too far");
	} else {		/* another iteration */
		pushsrc(pString, "\n_Endfor\n");
		pushsrc(pString, forp->str);
	}
}

endfor()	/* end one iteration of for loop */
{
	switch (forp->op) {
	case '+':
	case ' ':
		forp->sym->s_val.f += forp->by;
		break;
	case '-':
		forp->sym->s_val.f -= forp->by;
		break;
	case '*':
		forp->sym->s_val.f *= forp->by;
		break;
	case '/':
		forp->sym->s_val.f /= forp->by;
		break;
	}
	nextfor();
}

char *ifstat(expr, thenpart, elsepart)
	double expr;
	char *thenpart, *elsepart;
{
	if (expr) {
		unput('\n');
		pushsrc(Free, thenpart);
		pushsrc(pString, thenpart);
		unput('\n');
  		if (elsepart)
			free(elsepart);
		return thenpart;	/* to be freed later */
	} else {
		free(thenpart);
		if (elsepart) {
			unput('\n');
			pushsrc(Free, elsepart);
			pushsrc(pString, elsepart);
			unput('\n');
		}
		return elsepart;
	}
}
