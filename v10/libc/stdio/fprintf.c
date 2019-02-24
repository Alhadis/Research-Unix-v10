/*
 * pANS stdio -- fprintf
 */
#include "iolib.h"

#ifdef V10
#define flags _flag
#define buf _base
#else
#endif

int fprintf(FILE *f, const char *fmt, ...){
	int n;
	va_list args;
	char buff[BUFSIZ];

	if (f->flags & _IONBF)
		setbuf(f, buff);
	va_start(args, fmt);
	n=vfprintf(f, fmt, args);
	va_end(args);
	if ((char *)f->buf == buff) {
		fflush(f);
		setbuf(f, NULL);
		}
	return n;
}
