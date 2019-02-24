/*
 * print machine-dependent numbers: signals, pc
 */

#include "defs.h"
#include "regs.h"
#include "space.h"
#include <signal.h>

static char *signals[] = {
	"",
	"hangup",
	"interrupt",
	"quit",
	"illegal instruction",
	"trace/BPT",
	"IOT",
	"EMT",
	"floating exception",
	"killed",
	"bus error",
	"memory fault",
	"bad system call",
	"broken pipe",
	"alarm call",
	"terminated",
	"signal 16",
	"signal 17",
	"child termination",
	"power fail",
	"memory error",
	"deadlock",
};

sigprint()
{
	if ((signo>=0) && (signo<sizeof signals/sizeof signals[0]))
		prints(signals[signo]);
}

printpc()
{

	dot = (ADDR)rtow(rget(PC));
	psymoff((WORD)dot, INSTSP, "/%16t");
	printins(CORF|INSTSP);
	printc(EOR);
}
