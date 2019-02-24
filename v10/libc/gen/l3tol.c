/* @(#)l3tol.c	4.1 (Berkeley) 12/21/80 */
l3tol(lp, cp, n)
long	*lp;
char	*cp;
int	n;
{
	register i;
	register char *a, *b;

	a = (char *)lp;
	b = cp;
	for(i=0;i<n;i++) {
#ifdef pdp11
		*a++ = *b++;
		*a++ = 0;
		*a++ = *b++;
		*a++ = *b++;
#else
#ifdef vax
		*a++ = *b++;
		*a++ = *b++;
		*a++ = *b++;
		*a++ = 0;
#else
#ifdef interdata
		*a++ = 0;
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
