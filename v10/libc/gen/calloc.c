char *
calloc(num, size)
	unsigned num, size;
{
	register char *mp;
	char *malloc();

	num *= size;
	if(mp = malloc(num))
		memset(mp, 0, num);
	return(mp);
}

cfree(p, num, size)
char *p;
unsigned num, size;
{
	free(p);
}
