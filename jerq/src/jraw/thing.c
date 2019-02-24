#include "jraw.h"
#include "thing.h"

void Thing.init(){}

void Dict.init()		/* really should be apply(init) */
{
	register i;
	for (i = 0; i < n; i++)
		x[i]->init();
}

void Dict.remove()
{
	register i;
	for (i = 0; i < n; i++) {
		delete a[i];
		delete x[i];
		a[i] = 0;
		x[i] = 0;
	}
	n = 0;
}

void Dict.put(FILE *f)
{
	register i;
	for (i = 0; i < n; i++)
		x[i]->put(f);
}

Dict *Dict.operator+=(char *s)
{
	register i,j;
	char **na;
	Thing **nx;
	if (n >= N) { /* grow it */
		na = new char*[2*N];
		nx = new Thing*[2*N];
		for (i = 0; i < n; i++) {
			na[i] = a[i];
			nx[i] = x[i];
		}
		delete a;
		delete x;
		a = na;
		x = nx;
		N *= 2;
	}
	for (i = 0; i < n && strcmp(a[i],s) < 0; i++)
		;
	for (j = n; j > i; j--) {
		a[j] = a[j-1];
		x[j] = x[j-1];
	}
	a[i] = s;
	n++;
	return this;
}

Thing *&Dict.operator[](char *s)
{
	register i;
	for (i = 0; i < n; i++)
		if (strcmp(a[i],s) == 0)
			return /* & */x[i];
	return 0;
}

strcmp(s1, s2)
register char *s1, *s2;
{

	while (*s1 == *s2++)
		if (*s1++=='\0')
			return(0);
	return(*s1 - *--s2);
}

char *
strcpy(s1, s2)
register char *s1, *s2;
{
	register char *os1;

	os1 = s1;
	while (*s1++ = *s2++)
		;
	return(os1);
}
