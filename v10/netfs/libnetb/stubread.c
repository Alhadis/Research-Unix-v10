/*
 * stub for fsread
 */
#include "rf.h"

int
fsread(f, off, buf, len)
Rfile *f;
long off;
char *buf;
int len;
{
	fserrno = RFEINVAL;
	return (-1);
}
