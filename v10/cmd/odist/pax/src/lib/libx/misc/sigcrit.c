/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * signal critical region support
 */

#include <sig.h>

#define elements(x)	(sizeof(x)/sizeof(x[0]))

#include "FEATURE/sigsetmask"

static int	signals[] =		/* held inside critical region	*/
{
	SIGINT,
	SIGQUIT,
	SIGHUP,
};

#ifndef SIG_SETMASK
#ifndef	_lib_sigsetmask

static long	hold;			/* held signal mask		*/

/*
 * hold last signal for later delivery
 */

static void
interrupt(sig)
int	sig;
{
	(void)signal(sig, interrupt);
	hold |= sigmask(sig);
}

#endif
#endif

/*
 * critical signal region handler
 *
 * op>0		push region, return region level
 * op==0	pop region, return region level
 * op<0		return non-zero if any signals held in current region
 *
 * signals[] held until region popped
 */

int
sigcritical(op)
int	op;
{
	register int	i;
	static int	level;
#ifdef SIG_SETMASK
	static sigset_t	mask;
	sigset_t	nmask;
#else
#ifdef _lib_sigsetmask
	static long	mask;
#else
	static signal_t	handler[elements(signals)];
#endif
#endif

	if (op > 0)
	{
		if (!level++)
		{
#ifdef SIG_SETMASK
			(void)sigemptyset(&nmask);
			for (i = 0; i < elements(signals); i++)
				(void)sigaddset(&nmask, signals[i]);
			(void)sigprocmask(SIG_BLOCK, &nmask, &mask);
#else
#ifdef _lib_sigsetmask
			mask = 0;
			for (i = 0; i < elements(signals); i++)
				mask |= sigmask(signals[i]);
			mask = sigblock(mask);
#else
			hold = 0;
			for (i = 0; i < elements(signals); i++)
				if ((handler[i] = signal(signals[i], interrupt)) == SIG_IGN)
				{
					(void)signal(signals[i], SIG_IGN);
					hold &= ~sigmask(signals[i]);
				}
#endif
#endif
		}
		return(level);
	}
	else if (!op)
	{
		if (!--level)
		{
#ifdef SIG_SETMASK
			(void)sigprocmask(SIG_SETMASK, &mask, (sigset_t*)0);
#else
#ifdef _lib_sigsetmask
			(void)sigsetmask(mask);
#else
			for (i = 0; i < elements(signals); i++)
				(void)signal(signals[i], handler[i]);
			if (hold)
			{
				for (i = 0; i < elements(signals); i++)
					if (hold & sigmask(signals[i]))
						(void)kill(getpid(), signals[i]);
				(void)pause();
			}
#endif
#endif
		}
		return(level);
	}
	else
	{
#ifdef SIG_SETMASK
		(void)sigpending(&nmask);
		for (i = 0; i < elements(signals); i++)
			if (sigismember(&nmask, signals[i]))
				return(1);
		return(0);
#else
#ifdef _lib_sigsetmask
		/* no way to get pending signals without installing handler */
		return(0);
#else
		return(hold != 0);
#endif
#endif
	}
}
