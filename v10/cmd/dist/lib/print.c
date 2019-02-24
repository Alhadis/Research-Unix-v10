#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"

#ifdef DOPRNT
#define vfprintf(fp, fmt, ap) _doprnt(fmt, ap, fp)
#endif

char *prog = "(uninitialized)";

static FILE *logfp;

void
openlog(char *logfile)
{
	if (logfp)
		fclose(logfp);
	logfp = fopen(logfile, "a");
}

static void
vlprintf(char *fmt, va_list ap)
{
	time_t now;

	if (logfp) {
		time(&now);
		fprintf(logfp, "%s [%d %.24s]: ", prog, getpid(), ctime(&now));
		vfprintf(logfp, fmt, ap);
		putc('\n', logfp);
		fflush(logfp);
	}
}

void
eprintf(char *fmt, ...)
{
	va_list ap;

	fprintf(stderr, "%s: ", prog);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	putc('\n', stderr);

	va_start(ap, fmt);
	vlprintf(fmt, ap);
	va_end(ap);
}

void
lprintf(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vlprintf(fmt, ap);
	va_end(ap);
}
