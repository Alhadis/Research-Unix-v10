/*
 * miscellaneous error handling
 */

#include <stdio.h>

extern char *malloc(), *realloc();

char *
emalloc(size)
int size;
{
	char *p;

	if ((p = malloc(size)) == NULL)
		panic("out of memory\n");
	return (p);
}

char *
erealloc(p, size)
char *p;
int size;
{
	if ((p = realloc(p, size)) == NULL)
		panic("out of memory\n");
	return (p);
}

/*
 * print to the log (== standard error)
 */

log(s, p0, p1, p2, p3, p4, p5)
char *s;
{
	char *ctime();
	long time();
	long now;

	time(&now);
	fprintf(stderr, "%.15s ", ctime(&now)+4);
	fprintf(stderr, s, p0, p1, p2, p3, p4, p5);
	fflush(stderr);
}

/*
 * fatal error: log and exit
 */
panic(s, p0, p1, p2, p3, p4, p5)
char *s;
{
	char *ctime();
	long time();
	long now;

	time(&now);
	fprintf(stderr, "%.15s panic:", ctime(&now)+4);
	fprintf(stderr, s, p0, p1, p2, p3, p4, p5);
	fflush(stderr);
	exit(1);
}
