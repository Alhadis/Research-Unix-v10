/* Copyright AT&T Bell Laboratories, 1993 */
#include	<stdio.h>

fputs(const char *s, FILE *iop)
{
	unsigned char *e, *t;
	for(;;) {
		t = iop->_ptr;
		e = t + iop->_cnt;
		while(*s && t < e)
			*t++ = *s++;
		iop->_cnt -= t - iop->_ptr;
		iop->_ptr = t;
		if(*s == 0)
			break;
		if(putc(*s++, iop) == EOF)
			return EOF;
	}
	return 0;
}
