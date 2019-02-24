/*	@(#)fault.c	1.5	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"

extern int	done();

char	*trapcom[MAXTRAP];
BOOL	trapflg[MAXTRAP] =
{
	0,
	0,	/* hangup */
	0,	/* interrupt */
	0,	/* quit */
	0,	/* illegal instr */
	0,	/* trace trap */
	0,	/* IOT */
	0,	/* EMT */
	0,	/* float pt. exp */
	0,	/* kill */
	0, 	/* bus error */
	0,	/* memory faults */
	0,	/* bad sys call */
	0,	/* bad pipe call */
	0,	/* alarm */
	0, 	/* software termination */
	0,	/* unassigned */
#ifndef	SYSV
	0,	/* non-tty stop signal */
	0,	/* tty stop signal */
	0,	/* continue */
	0,	/* death of child */
	0,	/* tty input */
	0,	/* tty output */
	0,	/* lintrup (berkeley weirdness) */
	0,	/* exceeded cpu limit */
	0,	/* exceeded file limit */
#endif
};

int 	(*(sigval[]))() = 
{
	0,
	done,
	fault,
	fault,
/* temporary hack */
	/*done,*/0,
	done,
	done,
	done,
	done,
	0,
	done,
	done,
	done,
	done,
	fault,
	fault,
	done,
	0,
#ifndef	SYSV
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
#endif
};

/* ========	fault handling routines	   ======== */


fault(sig)
register int	sig;
{
	register int	flag;

	signal(sig, fault);
	if (sig == SIGALRM)
	{
		if (flags & waiting)
			done();
	}
	else
	{
		flag = (trapcom[sig] ? TRAPSET : SIGSET);
		trapnote |= flag;
		trapflg[sig] |= flag;
		if (sig == SIGINT)
			wasintr++;
	}
}

stdsigs()
{
	setsig(SIGHUP);
	setsig(SIGINT);
	ignsig(SIGQUIT);
	setsig(SIGPIPE);
	setsig(SIGALRM);
	setsig(SIGTERM);
#ifndef	SYSV
	setsig(SIGSTOP);
	setsig(SIGTSTP);
	setsig(SIGCONT);
	setsig(SIGTTIN);
	setsig(SIGTTOU);
	setsig(SIGXCPU);
	setsig(SIGXFSZ);
#endif
}

ignsig(n)
{
	register int	s;

	if ((s = (signal(n, SIG_IGN) == SIG_IGN)) == 0)
	{
		trapflg[n] |= SIGMOD;
	}
	return(s);
}

getsig(n)
{
	register int	i;

	if (trapflg[i = n] & SIGMOD || ignsig(i) == 0)
		signal(i, fault);
}


setsig(n)
{
	register int	i;

	if (ignsig(i = n) == 0)
		signal(i, sigval[i]);
}

oldsigs()
{
	register int	i;
	register char	*t;

	i = MAXTRAP;
	while (i--)
	{
		t = trapcom[i];
		if (t == 0 || *t)
			clrsig(i);
		trapflg[i] = 0;
	}
	trapnote = 0;
}

clrsig(i)
int	i;
{
	shfree(trapcom[i]);
	trapcom[i] = 0;
	if (trapflg[i] & SIGMOD)
	{
		trapflg[i] &= ~SIGMOD;
		signal(i, sigval[i]);
	}
}

/*
 * check for traps
 */
chktrap()
{
	register int	i = MAXTRAP;
	register char	*t;

	trapnote &= ~TRAPSET;
	while (--i)
	{
		if (trapflg[i] & TRAPSET)
		{
			trapflg[i] &= ~TRAPSET;
			if (t = trapcom[i])
			{
				int	savxit = exitval;

				execexp(t, 0);
				exitval = savxit;
				exitset();
			}
		}
	}
}
