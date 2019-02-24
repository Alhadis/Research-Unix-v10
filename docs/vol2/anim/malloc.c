int	mallocinit = 0;
FILE	*mallocfp;

char *emalloc(n)
int n;
{	char *p;
	if (mallocinit == 0) {
		mallocinit = 1;
		mallocfp = fopen("/tmp/malloc.hist", "w");
	}
	p = malloc((unsigned) n);
	if (p == NULL) fatal("out of memory");
	fprintf(mallocfp, "m\t%d\t%d\n", (int) p, n);
	return p;
}

efree(p)
char *p;
{	fprintf(mallocfp, "f\t%d\n", (int) p);
	free(p);
}
