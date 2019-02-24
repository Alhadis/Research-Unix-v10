/*
 * Copy s2 to s1, always copy n bytes
 * return s1
 *
 * JES 7Aug81 Allow overlapping moves in either direction.
 */

/* eventually do this in machine language */
extern char *
Memcpy(register char *s1, register const char *s2, int n)
{
	char *	os1 = s1;
	register char *	ns;

	if (s1 <= s2) {
		ns = s1 + n;  /* stopper for s1 */
		while ( s1 < ns )
			*s1++ = *s2++;
	} else {
		ns = s1;
		s1 += n;
		s2 += n;
		while ( ns < s1 )
			*--s1 = *--s2;
	}
	return(os1);
}

