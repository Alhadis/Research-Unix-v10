/*
 * stub for fscreate
 */
#include "rf.h"

Rfile *
fscreate(f, n, m, u, g)
Rfile *f;
char *n;
int m;
int u, g;
{
	fserrno = RFEINVAL;
	return (0);
}
