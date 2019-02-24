#include <signal.h>

static char status [NSIG];

/* status codes */
#define	NONE		0
#define	UNCAUGHT	1
#define	CAUGHT		2

static void
trap (sig)
{
	if (sig >= 0 && sig < NSIG)
		status[sig] = CAUGHT;
	signal (sig, trap);
}

int
sigtrap (sig)
	register int sig;
{
	register int cancel = 0, r = NONE;

	if (sig < 0) {
		cancel++;
		sig = -sig;
	}

	if (sig > 0 && sig < NSIG) {

		r = status[sig];

		if (cancel) {
			status[sig] = NONE;
			if (r != NONE && signal (sig, SIG_IGN) != SIG_IGN)
				signal (sig, SIG_DFL);
		} else {
			status[sig] = UNCAUGHT;
			if (r == NONE && signal (sig, SIG_IGN) != SIG_IGN)
				signal (sig, trap);
		}
	}

	return r;
}
