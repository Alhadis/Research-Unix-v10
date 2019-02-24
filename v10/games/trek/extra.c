#ifdef TRACE
tTf()
{
	return(0);
}
#endif

bmove(f, t, n)
register char *f, *t;
register int n;
{
	do
		*t++ = *f++;
	while(--n);
	return(t);
}

syserr(s, a, b, c, d, e, f)
{
	printf(s, a, b, c, d, e, f);
	exit(1);
}
