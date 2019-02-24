#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

static int	rencnt;
static Lnr	renbase, renincr, startln, endln;
static int	i;

char	*outlnr();
Lnr	cvtlnr(), newlnr();


/*
 * renumber --- renumber all stored lines
 */

renumber(line1, incr, brange, erange)
Lnr	line1;
Lnr	incr;
Lnr	brange;
Lnr	erange;
{
	register Linep	s;
	register Lnr	*p;
	Lnr		lnr;

	renbase = line1;
	renincr = incr;
	startln = brange;
	endln = erange;
	i = 0;
	p = (Lnr *)stkbase;
	rencnt = 0;
	lnr = line1;

	for ALL_LINES(s) {
		*p++ = s->l_lnr;	/* remember the line number */
		if ((char *)p >= stktop)
			err("too many lines");
		++rencnt;
		lnr += incr;
		if (lnr <= line1 || lnr > MAXLNR)
			err("invalid renumber parameters");
		}

	lnr = line1;
	for ALL_LINES(s) {
		if (s->l_lnr >= startln && s->l_lnr <= endln) {
		s->l_lnr = lnr;
		lnr += incr;
		}
		}
	for ALL_LINES(s) {
		immed.l_lnr = s->l_lnr;
		if (renumline(s))
			storeline(&immed);
		}
}


/*
 * renumline --- renumber the line pointed to by s
 *		 line numbers are always preceeded by:	
 *			 THEN, GOTO, GOSUB
 *		 for an ON statement, there is a list of line numbers
 *		 following the GOTO or GOSUB
 */

renumline(s)
Linep	s;
{
	register char	*p, *q;
	int		changed, onflag;
	Lnr		l;

	changed = 0;
	onflag = 0;
	p = s->l_line;
	for (q = immed.l_line, p = s->l_line; *p; ) {
		switch ((*q++ = *p++)) {
		case ON:
			++onflag;
			break;
		case COMMA:
			if (!onflag)
				break;
			/* fall thru if inside a ON ... GO ... */
		case USING:
		case THEN:
		case ELSE:
		case GOTO:
		case GOSUB:
			if (isdigit(*p)) {
				inptr = p;
				l = cvtlnr();
				p = inptr;
				l = newlnr(l);
				q = outlnr(q, l);
				++changed;
				}
			break;
		case COLON:
		case REM:
			onflag = 0;	/* reset onflag at end of statement */
			break;
			}
		}
	*q++ = 0;
	*q = 0;
	immed.l_len = q - immed.l_line + LINESIZE;
	if (immed.l_len & 1)
		++immed.l_len;
	return(changed);
}


/*
 * outlnr --- convert specified line number to character string
 */

char *outlnr(bp, lnr)
char	*bp;
Lnr	lnr;
{
	register char	*p;

	p = bp;
	sprintf(p, "%u", lnr);
	while (*p++)
		;
	return (--p);
}


/*
 * newlnr --- map old line number into a new one
 */

Lnr newlnr(lnr)
register Lnr	lnr;
{
	register Lnr	*p;
	register Linep	s;

	p = (Lnr *)stkbase;
	for ALL_LINES(s) {
		if (*p++ == lnr)
			return(s->l_lnr);
			}
	fprintf(stderr, "line %u not found\n", lnr);
	return (lnr);
}
