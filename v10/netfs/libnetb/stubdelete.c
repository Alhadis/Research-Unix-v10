/*
 * stub for fsdelete
 */
#include "rf.h"

int
fsdelete(f, n)
Rfile *f;
char *n;
{
	fserrno = RFEINVAL;
	return (-1);
}
