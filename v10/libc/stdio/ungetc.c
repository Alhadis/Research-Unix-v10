#include <stdio.h>

ungetc(c, iop)
register FILE *iop;
{
	if (c == EOF)
		return(-1);
	if ((iop->_flag&_IOREAD)==0 || iop->_ptr <= iop->_base)
		if (iop->_ptr == iop->_base && iop->_cnt==0)
			iop->_ptr++;
		else
			return(-1);
	iop->_cnt++;
	if(*--iop->_ptr != c)	/* sscanf buffer may be read-only */
		*iop->_ptr = c;
	return(c);
}
