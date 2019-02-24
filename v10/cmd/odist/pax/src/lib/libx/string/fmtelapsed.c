/*
 * return pointer to formatted elapsed time for t 1/n secs
 * compatible with strelapsed()
 * 8 consecutive calls before overwrite
 * return value length is at most 6
 */

#include <stdio.h>

#define elements(x)	(sizeof(x)/sizeof(x[0]))

char*
fmtelapsed(t, n)
register unsigned long	t;
register int		n;
{
	register unsigned long	s;

	static int		amt[] = { 1, 60, 60, 24, 7, 4, 12, 20 };
	static char		chr[] = "smhdwMYS";

	static char		tms[8][7];
	static int		tm;

	if (t == 0L) return("0");
	if (t == ~0L) return("%");
	if (++tm >= elements(tms)) tm = 0;
	s = t / n;
	if (s < 60) (void)sprintf(tms[tm], "%d.%02ds", s % 100, (t * 100 / n) % 100);
	else
	{
		for (n = 1; n < elements(amt) - 1; n++)
		{
			if ((t = s / amt[n]) < amt[n + 1]) break;
			s = t;
		}
		(void)sprintf(tms[tm], "%d%c%02d%c", (s / amt[n]) % 100, chr[n], s % amt[n], chr[n - 1]);
	}
	return(tms[tm]);
}
