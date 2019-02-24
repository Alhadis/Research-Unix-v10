/*
 * stub for fslink
 */
#include "rf.h"

int
fslink(f, n, t)
Rfile *f;
char *n;
Rfile *t;
{
	fserrno = RFEINVAL;
	return (-1);
}
