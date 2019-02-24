/*	@(#)ltostr.c	1.1	*/
/*
 *	ltostr -- convert long to decimal string
 *
 *
 *	char *
 *	ltostr(value, ptr)
 *	long value;
 *	char *ptr;
 *
 *	Ptr is assumed to point to the byte following a storage area
 *	into which the decimal representation of "value" is to be
 *	placed as a string.  Ltostr converts "value" to decimal and
 *	produces the string, and returns a pointer to the beginning
 *	of the string.  No leading zeroes are produced, and no
 *	terminating null is produced.  The low-order digit of the
 *	result always occupies memory position ptr-1.
 *	Ltostr's behavior is undefined if "value" is negative.  A single
 *	zero digit is produced if "value" is zero.
 *
 */

char *
ltostr(value, ptr)
register long value;
register char *ptr;
{
	register long t;

	do {
		*--ptr = '0' + value - 10 * (t = value / 10);
	} while ((value = t) != 0);

	return(ptr);
}
