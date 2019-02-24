char *
nalloc(pp, size, asize)
register char **pp;
{
	char *malloc(), *realloc();
	register char *p; int np;
	for (;;) {
		if ((p = pp[1]) + size <= pp[2])
			return (pp[1] += size, p);
		if (p = pp[0])
			np = pp[1] - p, p = realloc(p, asize += pp[2] - p);
		else
			np = 0, p = malloc(asize);
		if (p == 0)
			return 0;
		pp[0] = p;
		pp[1] = p + np;
		pp[2] = p + asize;
	}
}
