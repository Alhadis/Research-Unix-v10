#include "ps.h"
#include <sys/vmmac.h>

#define NSTACK	8192
#define NCMD	63
#define CMDSIZ	(4*((DIRSIZ + 3 + 3)/4))

char stack[NSTACK], cmdline[CMDSIZ];

char *
getargs(f, pp, up)
int f; register struct proc *pp; register struct user *up;
{
	register char *sp; char *argp, *argend, *envend;
	int nstack, staddr, stblk;

	nstack = ctob(up->u_ssize);
	if (nstack > NSTACK)
		nstack = NSTACK;
	staddr = UBASE - nstack;

	if (pp->p_flag & SLOAD) {
		if (!Seek(f, staddr) || !Read(f, stack, nstack))
			goto ucommand;
	} else {
		stblk = up->u_smap.dm_map[dndmap()] + DMMIN;
		if (!Seek(drum, stblk*NBPG - nstack) ||
		    !Read(drum, stack, nstack))
			goto ucommand;
	}

	sp = stack + nstack;
	while (*--sp == 0)
		if (sp <= stack) goto ucommand;
	envend = sp + 2;

	sp = (char *)((long)sp & (~3));
	while (*(long *)(sp -= 4) != 0)
		if (sp <= stack) goto ucommand;
	argp = sp + 4;

	while (*(long *)(sp -= 4) != 0)
		if (sp <= stack) goto ucommand;
	argend = *(long *)(sp + 4) - staddr + stack;	/* envp[0] */

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
	sprintf(cmdline, "(%-.*s)", DIRSIZ, up->u_comm);
	return cmdline;
}

dndmap()
{
	register f, n; char cbuf[16];
	static called, delta;
	if (called++)
		return delta;
	if ((f = open("/etc/ndmap", 0)) < 0)
		return 0;
	n = read(f, cbuf, sizeof cbuf-1);
	close(f);
	if (n <= 0)
		return 0;
	cbuf[n] = 0;
	return (delta = atoi(cbuf) - NDMAP);
}
