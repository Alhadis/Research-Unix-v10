#include <stdio.h>
#include "trace.h"
#include "trace.d"

 extern int nrprocs, *processes, *state;

 char *Realloc(), *Emalloc(), *Smalloc();

 unsigned short **pstates;
 int pgrowth = 32;
 int nrpstates = 0;
 int pbound = 0;

growPtable()
{ int p = pbound + pgrowth;

	if (p == pgrowth)
		pstates = (unsigned short **)
			Emalloc(p * sizeof(unsigned short *));
	else
		pstates = (unsigned  short **)
			Realloc(pstates, p * sizeof(unsigned short *));
	pbound = p;
}

unsigned short *
Pinsert()
{ unsigned short *try;
  register int i;

	if (nrpstates >= pbound)
		growPtable();

	try = (unsigned  short *)
		Smalloc(nrprocs * sizeof(unsigned  short));

	for (i = 0; i < nrprocs; i++)
		try[i] = (unsigned short) (state[i] + (processes[i]<<10));

	pstates[nrpstates++] = try;

	return try;
}

unsigned  short *
inPtable()
{ register int i;

	for (i = 0; i < nrpstates; i++)
		if (sameP(pstates[i]))
			return pstates[i];

	return Pinsert();
}

sameP(at)
	unsigned  short *at;
{ register int i;

	for (i = 0; i < nrprocs; i++)
	{	if ((at[i]&1023) != (unsigned short) state[i]
		||   at[i]>>10   != (unsigned short) processes[i])
			return 0;
	}
	return 1;
}
