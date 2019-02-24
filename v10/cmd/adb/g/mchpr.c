/*
 * code to print the name of a signal
 * machine-dependent for less than obvious reasons
 * this for gnot and gnope
 */

#include "defs.h"
#include "regs.h"
#include "space.h"

sigprint()
{
}

printpc()
{

	dot = (ADDR)rtow(rget(PC));
	psymoff((WORD)dot, INSTSP, "?%16t");
	printins(SYMF|INSTSP);
	printc(EOR);
}
