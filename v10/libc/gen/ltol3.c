/* @(#)ltol3.c	4.1 (Berkeley) 12/21/80 */
ltol3(cp, lp, n)
char	*cp;
long	*lp;
int	n;
{
	register i;
	register char *a, *b;

	a = cp;
	b = (char *)lp;
	for(i=0;i<n;i++) {
#ifdef pdp11
		*a++ = *b++;
		b++;
		*a++ = *b++;
		*a++ = *b++;
#else
#ifdef vax
		*a++ = *b++;
		*a++ = *b++;
		*a++ = *b++;
		b++;
#else
#ifdef interdata
		b++;
		*a++ = *b++;
		*a++ = *b++;
		*a++ = *b++;
#else
	Unknown machine!
#endif
#endif
#endif
	}
}
