/*
 * miscellany
 */

#include "defs.h"

/*
 * swap bytes
 */

unsigned short
swab(s)
short s;
{
	return (((s >> 8) & 0xff) | ((s & 0xff) << 8));
}

/*
 * swap bytes in a long
 */

long
swal(l)
long l;
{
	return (swab((short)(l >> 16)) | (swab((short)l) << 16));
}
