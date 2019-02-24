/*
 * gnope to unix mapping
 */

int
Read(f, b, s)
int f;
char *b;
int s;
{
	return (read(f, b, (long)s));
}

int
Write(f, b, s)
int f;
char *b;
int s;
{
	return (write(f, b, (long)s));
}

typedef (*pf)();

pf
signal(s, f)
int s;
pf f;
{
	return ((pf)0);
}

char *
Malloc(sz)
int sz;
{
	char *malloc();

	return (malloc((long)sz));
}
