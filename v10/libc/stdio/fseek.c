/*
 * Seek for standard library.  Coordinates with buffering.
 */

#include	<stdio.h>

long lseek();

fseek(iop, offset, ptrname)
	register FILE *iop;
	long offset;
{
	register d;
	register flag = iop->_flag;
	register long p;

	iop->_flag &= ~_IOEOF;
	if (flag & _IOWRT)
		fflush(iop);
	if (flag & _IORW)
		iop->_flag &= ~(_IOWRT|_IOREAD);
	if ((flag&_IOREAD) && iop->_base && ptrname!=2) do {
		if (ptrname==1 && offset<=iop->_cnt &&
		    offset>=iop->_base-iop->_ptr)
			d = offset;	
		else {
			p = lseek(fileno(iop), 0L, 1);
			if (p < 0)
				break;
			d = iop->_cnt;
			if (ptrname == 1) {
				offset += p - d;
				ptrname = 0;
			}
			if (offset<0 || flag&_IORW)
				break;
			p -= offset;
			if (p >= 0 && p <= iop->_ptr - iop->_base + d)
				d -= (int)p;
			else {
				d = (unsigned int)offset % BUFSIZ;
				if (d == 0)	/* room for ungetc */
					break;
				lseek(fileno(iop), offset - d, 0);
				if (_filbuf(iop) == EOF)
					break;
				if (--d > iop->_cnt)	/* part buf */
					break;
			}
		}
		iop->_cnt -= d;
		iop->_ptr += d;
		return (0);
	} while (0);
	if (flag & (_IORW|_IOREAD)) {
		iop->_cnt = 0;
		iop->_ptr = iop->_base;
	}
	return (lseek(fileno(iop), offset, ptrname) < 0? -1: 0);
}
