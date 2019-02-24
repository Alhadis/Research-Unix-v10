/*
 * stub for fswalk
 */
#include "rf.h"

Rfile *
fswalk(f, n)
Rfile *f;
char *n;
{
	fserrno = RFENOENT;
	return (0);
}
