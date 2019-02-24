a()
{	register char *m;
	register int i;
	*(int *)(m + i) = 12;
	m[i] = 12;
}
