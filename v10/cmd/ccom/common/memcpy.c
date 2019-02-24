char *
memcpy(s1, s2, n)
register char *s1, *s2;
register int n;
{
	char *r = s1;

	while (n--)
		*s1++ = *s2++;
	return (r);
}
