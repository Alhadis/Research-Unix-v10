/*
 * stub for fsrmdir
 */
#include "rf.h"

int
fsrmdir(f, n)
Rfile *f;
char *n;
{
	fserrno = RFEINVAL;
	return (-1);
}
