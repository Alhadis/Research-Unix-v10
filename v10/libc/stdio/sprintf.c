/*
 * pANS stdio -- sprintf
 */
#include "iolib.h"
int sprintf(char *buf, const char *fmt, ...){
	int n;
	va_list args;
	char *v;
#ifdef V10
	FILE _strbuf;
	_strbuf._flag = _IOWRT+_IOSTRG;
	_strbuf._ptr = (unsigned char *) buf;
	_strbuf._cnt = 100000;
#define f &_strbuf
#define sclose(x) buf[n] = 0
#else
	FILE *f=sopenw();
	if(f==NULL)
		return 0;
	setvbuf(f, buf, _IOFBF, 100000);
#endif
	va_start(args, fmt);
	n=vfprintf(f, fmt, args);
	va_end(args);
	sclose(f);
	return n;
}
