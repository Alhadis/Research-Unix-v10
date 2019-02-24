/*	@(#)stak.c	1.4	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"

char *setbrk();


/* ========	storage allocation	======== */

char *
getstak(asize)			/* allocate requested stack */
int	asize;
{
	register char	*oldstak;
	register int	size;

	size = round(asize, BYTESPERWORD);
	oldstak = stakbot;
	staktop = stakbot += size;
	while (staktop >= brkend)
		setbrk(round(staktop - brkend + 1, BRKINCR));
	return(oldstak);
}

/*
 * set up stack for local use
 * should be followed by `endstak'
 */
char *
locstak()
{
	if (brkend - stakbot < BRKINCR)
	{
		if (setbrk(brkincr) == (char *) -1)
			error(nostack);
		if (brkincr < BRKMAX)
			brkincr += 256;
	}
	return(stakbot);
}

char *
savstak()
{
	assert(staktop == stakbot);
	return(stakbot);
}

char *
endstak(argp)		/* tidy up after `locstak' */
register char	*argp;
{
	register char	*oldstak;

	*argp++ = 0;
	oldstak = stakbot;
	stakbot = staktop = (char *)sround(argp, BYTESPERWORD);
	return(oldstak);
}

tdystak(x)		/* try to bring stack back to x */
register char	*x;
{
	while ((char *)(stakbsy) > (char *)(x))
	{
		shfree(stakbsy);
		stakbsy = stakbsy->word;
	}
	staktop = stakbot = max((char *)(x), (char *)(stakbas));
	rmtemp(x);
}

stakchk()
{
	if ((brkend - stakbas) > BRKINCR + BRKINCR)
		setbrk(-BRKINCR);
}

char *
cpystak(x)
char	*x;
{
	return(endstak(movstr(x, locstak())));
}

setstak(x)
char *x;
{
	staktop = absstak(x);
	while (staktop >= brkend)
		setbrk(BRKINCR);
}

pushstak(c)
char c;
{
	*staktop++ = c;
	while (staktop >= brkend)
		setbrk(BRKINCR);
}
