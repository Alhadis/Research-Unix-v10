/* @(#)fprintf.c	4.1 (Berkeley) 12/21/80 */
#include	<stdio.h>

fprintf(iop, fmt, args)
FILE *iop;
char *fmt;
{
	register int n;
	unsigned char buff[BUFSIZ];

	if (iop->_flag & _IONBF)
		setbuf(iop, buff);

	n = _doprnt(fmt, &args, iop);

	if (iop->_base == buff)
	{
		fflush(iop);
		setbuf(iop, NULL);
	}

	return(ferror(iop)? EOF: n);
}
