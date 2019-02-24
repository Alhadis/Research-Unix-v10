#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib.h"

#define MIN(A, B) ((A) < (B) ? (A) : (B))

void *
xmalloc(size_t size)
{
	void *result;

	result = malloc(size);
	if (size && !result) {
		eprintf("malloc failure");
		exit(1);
	}
	return result;
}

void *
xrealloc(void *ptr, size_t size)
{
	void *result;

	result = realloc(ptr, size);
	if (size && !result) {
		eprintf("realloc failure");
		exit(1);
	}
	return result;
}

char *
xstrdup(char *s)
{
	char *t;

	t = xmalloc(strlen(s) + 1);
	strcpy(t, s);
	return t;
}

long
fpcopy(FILE *dst, FILE *src, long size)
{
	char buf[4096];
	long cc, total;

	total = 0;
	while (total < size) {
		cc = fread(buf, 1, MIN(sizeof buf, size - total), src);
		if (cc <= 0)
			break;
		if (fwrite(buf, 1, cc, dst) != cc)
			break;
		total += cc;
	}
	if (ferror(dst) || ferror(src))
		return -1;
	return total;
}

void
discard(FILE *fp, long n)
{
	char buf[4096];
	long cc;

	while (n >= sizeof buf) {
		cc = fread(buf, 1, sizeof buf, fp);
		if (cc <= 0)
			return;
		n -= cc;
	}
	fread(buf, 1, n, fp);
}
