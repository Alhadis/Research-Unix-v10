/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * if sig>=0 then return signal text for signal sig
 * otherwise return signal name for signal -sig
 *
 * data defined in sigdata.c
 */

#include <stdio.h>

extern int	sig_max;

extern char*	sig_name[];
extern char*	sig_text[];

char*
strsignal(sig)
register int	sig;
{
	static char			buf[20];

	if (sig >= 0)
	{
		if (sig <= sig_max) return(sig_text[sig]);
		(void)sprintf(buf, "Signal %d", sig);
	}
	else
	{
		sig = -sig;
		if (sig <= sig_max) return(sig_name[sig]);
		(void)sprintf(buf, "%d", sig);
	}
	return(buf);
}
