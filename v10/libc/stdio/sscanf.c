/*
 * pANS stdio -- sscanf
 */
#include "iolib.h"
int sscanf(const char *s, const char *fmt, ...){
	int n;
	va_list args;
#ifdef V10
	FILE _strbuf;

	_strbuf._flag = _IOREAD|_IOSTRG;
	_strbuf._ptr = _strbuf._base = (unsigned char *) s;
	_strbuf._cnt = 0;
	while (*s++)
		_strbuf._cnt++;
#define f &_strbuf
#define sclose(x)
#else
	FILE *f=sopenr(s);
#endif
	va_start(args, fmt);
	n=vfscanf(f, fmt, args);
	va_end(args);
	sclose(f);
	return n;
}
