/* @(#)printf.c	4.1 (Berkeley) 12/21/80 */
#include	<stdio.h>

printf(fmt, args)
char *fmt;
{
	register int n;
	unsigned char buff[BUFSIZ];

	if (stdout->_flag & _IONBF)
		setbuf(stdout, buff);

	n = _doprnt(fmt, &args, stdout);

	if (stdout->_base == buff)
	{
		fflush(stdout);
		setbuf(stdout, NULL);
	}

	return(ferror(stdout)? EOF: n);
}
