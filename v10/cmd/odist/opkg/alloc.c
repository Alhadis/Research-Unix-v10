/*
 *	storage allocator
 *
 *	calls malloc or realloc and aborts if unsuccessful
 */

#include "asd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *
alloc (size_t n)
{
	register void *p;

	p = malloc (n);
	schk (p);
	return p;
}

void *
ralloc (void *s, size_t n)
{
	register void *p;

	if (s == NULL)
		return alloc (n);
	
	p = realloc (s, n);
	schk (p);
	return p;
}

/* return a copy of a string */
char *
copy (char *s)
{
	register char *r;
	r = alloc (strlen (s) + 1);
	strcpy (r, s);
	return r;
}
