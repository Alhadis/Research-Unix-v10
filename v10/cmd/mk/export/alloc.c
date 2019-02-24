#include	"libc.h"

static int goo;		/* so no empty object on cray */

#ifndef	CRAY

#define	A(x)	((((long)x)+sizeof(long)-1)&~(sizeof(long)-1))

char *next;
int nleft;

char *
malloc(n)
{
	extern char *sbrk();
	char *s;

	n = A(n);
	if(n > nleft){
		next = sbrk(nleft = A(32000));
		if(next == (char *)-1){
			perror("mk: sbrk");
			Exit();
		}
	}
	s = next;
	next += n;
	nleft -= n;
	return(s);
}

free(s)
	char *s;
{
}

char *
realloc(s, n)
	char *s;
	unsigned n;
{
	char *r;

	r = malloc(n);
	memcpy(r, s, n);
	return(r);
}

#endif
