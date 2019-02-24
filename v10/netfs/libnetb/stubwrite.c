/*
 * stub for fswrite
 */
#include "rf.h"

int
fswrite(f, off, buf, len)
Rfile *f;
long off;
char *buf;
int len;
{
	fserrno = RFEINVAL;
	return (-1);
}
