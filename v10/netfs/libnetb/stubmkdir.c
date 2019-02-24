/*
 * stub for fsmkdir
 */
#include "rf.h"

int
fsmkdir(f, n, m, u, g)
Rfile *f;
char *n;
int m;
int u, g;
{
	fserrno = RFEINVAL;
	return (-1);
}
