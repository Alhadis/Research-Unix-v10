/*
 * ascii<>radix50
 */

#define	BADC	0100

static char r50c[128] = {
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	000,	BADC,	BADC,	BADC,	033,	BADC,	BADC,	BADC,
	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,	034,	BADC,
	036,	037,	040,	041,	042,	043,	044,	045,
	046,	047,	BADC,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	001,	002,	003,	004,	005,	006,	007,
	010,	011,	012,	013,	014,	015,	016,	017,
	020,	021,	022,	023,	024,	025,	026,	027,
	030,	031,	032,	BADC,	BADC,	BADC,	BADC,	BADC,
	BADC,	001,	002,	003,	004,	005,	006,	007,
	010,	011,	012,	013,	014,	015,	016,	017,
	020,	021,	022,	023,	024,	025,	026,	027,
	030,	031,	032,	BADC,	BADC,	BADC,	BADC,	BADC
};

static char r50a[] = " abcdefghijklmnopqrstuvwxyz$.?0123456789";

/*
 * convert up to three characters
 * from ascii to r50
 * advances s; returns the r50
 * -1 means an error
 */
int
ator50(s)
register char **s;
{
	register char c;
	register int i;
	register unsigned short r;

	r = 0;
	for (i = 0; i < 3 && (c = **s) != 0 ; i++) {
		if (c & 0200)
			return (-1);
		if ((c = r50c[c]) == BADC)
			return (-1);
		r *= 050;
		r += c;
		(*s)++;
	}
	for (; i < 3; i++)
		r *= 050;		/* justify */
	return (r);
}

/*
 * convert r50 to ascii,
 * suppressing leading blanks
 * s is where to put the string;
 * the new s is returned.
 * s had best have at least three characters remaining
 */

char *
r50toa(s, x)
register unsigned short x;
register char *s;
{
	register unsigned char	c;

	if (x) {
		c = x % 050;
		x /= 050;
		if (x)
			s = r50toa(s, x);
		*s++ = r50a[c];
	}
	return (s);
}
