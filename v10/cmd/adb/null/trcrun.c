/*
 * OS-specific functions for running the debugged process
 * dummy version
 */

#include "defs.h"
#include "regs.h"

extern char lastc, peekc;

static int child;

killpcs()
{
}

grab()
{
	error("no");
}

ungrab()
{
	error("no");
}

startpcs()
{
	error("no");
}

runstep(keepsig)
int keepsig;
{
}

runrun(keepsig)
int keepsig;
{
}


bpwait()
{
}

bkput()
{
}
