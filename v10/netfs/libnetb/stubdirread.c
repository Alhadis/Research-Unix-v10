/*
 * stub for fsdirread
 */
#include "rf.h"

int
fsdirread(f, off, buf, len, offp)
Rfile *f;
long off;
char *buf;
int len;
long *offp;
{
	fserrno = RFEINVAL;
	return (-1);
}
