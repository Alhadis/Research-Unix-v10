#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "dist.h"

char *prog;

void
eprintf(char *fmt, ...)
{
	time_t now;
	char mesg[MAXLINE];
	va_list ap;
	static int fd = -1;

	time(&now);
	sprintf(mesg, "%s[%d %.24s]: ", prog, getpid(), ctime(&now));
	va_start(ap, fmt);
	vsprintf(mesg + strlen(mesg), fmt, ap);
	strcat(mesg, "\n");
	write(2, mesg, strlen(mesg));
	if (fd < 0)
		fd = open(SDIR "/log", 1);
	lseek(fd, 0L, 2);	/* we really want O_APPEND mode, but... */
	write(fd, mesg, strlen(mesg));
}
