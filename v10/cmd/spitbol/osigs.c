#include <signal.h>

void setsigs();

static void
ovfltrap (signo, param, xx, pc, psl)
{
	setsigs();		/* trap overflow again */
	psl |= 02;		/* set the overflow bit */
}

void
setsigs()
{
	signal (SIGFPE, ovfltrap);
}
