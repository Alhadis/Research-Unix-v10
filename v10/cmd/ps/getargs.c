#include "ps.h"
#include <sys/vmmac.h>

#define NCMD	63

static char stack[NSTACK];

char *
getargs(f, pp, up)
int f; register struct proc *pp; register struct user *up;
{
	register char *sp; char *argp, *argend, *envend;
	int nstack;

	if (pp->p_stat == SZOMB)
		goto ucommand;
	if((nstack = getstack(f, pp, up, stack)) == 0)
		goto ucommand;
	sp = stack+nstack;
	while (*--sp == 0)
		if (sp <= stack)
			goto ucommand;
	envend = sp + 2;

	sp = (char *)((long)sp & (~3));
	while (*(long *)(sp -= 4) != 0)
		if (sp <= stack)
			goto ucommand;
	argp = sp + 4;

	while (*(long *)(sp -= 4) != 0)
		if (sp <= stack)
			goto ucommand;
	argend = *(long *)(sp + 4) - (UBASE - nstack) + stack;	/* envp[0] */

	if (argend <= argp || argend > envend )
		argend = envend;
	if (argend > (sp = argp + NCMD))
		argend = sp;
	for (sp = argp; sp < argend; sp++) {
		if (*sp == 0 || *sp == '\t')
			*sp = ' ';
		else if (*sp < ' ' || *sp >= 0177)
			*sp = '?';
	}
	while (sp[-1] == ' ')
		sp--;
	*sp = 0;
	return argp;

ucommand:
	sprintf(stack, "(%-.*s)", DIRSIZ, up->u_comm);
	return stack;
}
