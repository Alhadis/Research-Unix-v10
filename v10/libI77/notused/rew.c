/*	@(#)rew.c	1.2	*/
/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#include <stdio.h>

extern int fflush();
extern long lseek();

void
rewind(iop)
register FILE *iop;
{
	(void) fflush(iop);
	(void) lseek(fileno(iop), 0L, 0);
	iop->_cnt = 0;
	iop->_ptr = iop->_base;
	iop->_flag &= ~(_IOERR | _IOEOF);
	if(iop->_flag & _IORW)
		iop->_flag &= ~(_IOREAD | _IOWRT);
}
