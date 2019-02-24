/* Copyright AT&T Bell Laboratories, 1993 */
#include	<stdio.h>

char *
fgets(char *ptr, int n, FILE *iop)
{
	int l = 0;
	char *s = ptr;
	unsigned char *t;
	while(--n > 0) {
		l = iop->_cnt;
		if(l > 0) {
			if(l > n) l = n;
			t = iop->_ptr;
			while(--l >= 0 && (*s++ = *t++) != '\n')
				continue;
			l = t - iop->_ptr;
			iop->_ptr = t;
			iop->_cnt -= l;
			if(t[-1] == '\n' || (n -= l) <= 0)
				break;
		}
		l = getc(iop);
		if(l == EOF)
			break;
		*s++ = l;
		if(l == '\n')
			break;
	}
	*s = 0;
	return l<0 && s==ptr? 0: ptr;
}
