/* Copyright AT&T Bell Laboratories, 1993 */
#include	<stdio.h>

unsigned
fread(void *ptr, unsigned size, unsigned count, FILE *iop)
{
	int l, c;
	unsigned char *s = ptr;
	unsigned char *t;
	unsigned long n = (unsigned long)count*size;
	for(;;) {
		l = iop->_cnt;
		if(l > n) l = n;
		t = iop->_ptr;
		iop->_cnt -= l;
		iop->_ptr += l;
		n -= l;
		while(--l >= 0)
			*s++ = *t++;
		if(n == 0)
			return count;
		c = getc(iop);
		if(c == EOF)
			return count - (n+size-1)/size;
		*s++ = c;
		n--;
	}
}

unsigned
fwrite(const void *ptr, unsigned size, unsigned count, FILE *iop)
{
	int l;
	unsigned char *s = ptr;
	unsigned char *t;
	unsigned long n = (unsigned long)count*size;
	for(;;) {
		l = iop->_cnt;
		if(l > n) l = n;
		t = iop->_ptr;
		iop->_cnt -= l;
		iop->_ptr += l;
		n -= l;
		while(--l >= 0)
			*t++ = *s++;
		if(n == 0)
			return count;
		if(putc(*s++, iop) == EOF)
			return count - (n+size-1)/size;
		n--;
	}
}
