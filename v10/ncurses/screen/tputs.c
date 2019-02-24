/* Copyright (c) 1979 Regents of the University of California */
static  char sccsid[] = "@(#)tputs.c	1.1	(1.9	3/6/83)";
#include <ctype.h>
#include "curses.h"
#include "term.h"
#ifdef NONSTANDARD
# include "ns_curses.h"
#endif

/*
 * Put the character string cp out, with padding.
 * The number of affected lines is affcnt, and the routine
 * used to output one character is outc.
 */
tputs(cp, affcnt, outc)
	register char *cp;
	int affcnt;
	int (*outc)();
{
	/* static (11 cc gripes) */ char *_tpad();

	if (cp == 0)
		return;

	/*
	 * The guts of the string.
	 */
	while (*cp)
		if (*cp == '$' && cp[1] == '<')
			cp = _tpad(cp, affcnt, outc);
		else
			(*outc)(*cp++);
}

static char *
_tpad(cp, affcnt, outc)
	register char *cp;
	int affcnt;
	int (*outc)();
{
	register int delay = 0;
	register int mspc10;
	register char *icp = cp;

	/* Eat initial $< */
	cp += 2;

	/*
	 * Convert the number representing the delay.
	 */
	if (isdigit(*cp)) {
		do
			delay = delay * 10 + *cp++ - '0';
		while (isdigit(*cp));
	}
	delay *= 10;
	if (*cp == '.') {
		cp++;
		if (isdigit(*cp))
			delay += *cp - '0';
		/*
		 * Only one digit to the right of the decimal point.
		 */
		while (isdigit(*cp))
			cp++;
	}

	/*
	 * If the delay is followed by a `*', then
	 * multiply by the affected lines count.
	 */
	if (*cp == '*')
		cp++, delay *= affcnt;
	if (*cp == '>')
		cp++;	/* Eat trailing '>' */
	else {
		/*
		 * We got a "$<" with no ">".  This is usually caused by
		 * a cursor addressing sequence that happened to generate
		 * $<.  To avoid an infinite loop, we output the $ here
		 * and pass back the rest.
		 */
		(*outc)(*icp++);
		return icp;
	}

	/*
	 * If no delay needed, or output speed is
	 * not comprehensible, then don't try to delay.
	 */
	if (delay == 0)
		return cp;
	/*
	 * Let handshaking take care of it - no extra cpu load from pads.
	 * Also, this will be more optimal since the pad info is usually
	 * worst case.  We only use padding info for such terminals to
	 * estimate the cost of a capability in choosing the cheapest one.
	 */
	if (xon_xoff)
		return cp;
	(void) _delay(delay, outc);
	return cp;
}
