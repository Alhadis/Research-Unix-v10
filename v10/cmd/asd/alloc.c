/*
 *	storage allocator
 *
 *	calls malloc or realloc and aborts if unsuccessful
 */

#include "asd.h"
#include <stdio.h>

char *malloc(), *realloc();

char *
alloc (n)
	unsigned n;
{
	register char *p;

	p = malloc (n);
	schk (p);
	return p;
}

char *
ralloc (s, n)
	char *s;
	unsigned n;
{
	register char *p;

	if (s == NULL)
		return alloc (n);
	
	p = realloc (s, n);
	schk (p);
	return p;
}

/* return a copy of a string */
char *
copy (s)
	register char *s;
{
	register char *r;
	r = alloc ((unsigned) (strlen (s) + 1));
	strcpy (r, s);
	return r;
}
