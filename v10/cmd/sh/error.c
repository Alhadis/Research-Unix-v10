/*	@(#)error.c	1.4	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"


/* ========	error handling	======== */

failed(s1, s2, per)
char	*s1, *s2;
{
	extern int sys_nerr;
	extern char *sys_errlist[];
	extern int errno;

	prp();
	prs_cntl(s1);
	if (s2)
	{
		prs(colon);
		prs(s2);
	}
	if (per)
	{
		if (errno >= 0 && errno < sys_nerr) {
			prs(colon);
			prs(sys_errlist[errno]);
		} else {
#if NOTDEF		/* prn gets negative numbers wrong */
			prs(": error ");
			prn(errno);
#else
			prs(": Unknown error");
#endif
		}
	}
	newline();
	exitsh(ERROR);
}

error(s)
char	*s;
{
	failed(s, NIL, 0);
}

exitsh(xno)
int	xno;
{
	/*
	 * Arrive here from `FATAL' errors
	 *  a) exit command,
	 *  b) default trap,
	 *  c) fault with no trap set.
	 *
	 * Action is to return to command level or exit.
	 */
	exitval = xno;
	flags |= eflag;
	if ((flags & (forked | errflg | ttyflg)) != ttyflg)
		done();
	else
	{
		clearup();
		restore(0);
		clear_buff();
		execbrk = breakcnt = funcnt = 0;
		longjmp(errshell, 1);
	}
}

done()
{
	register char	*t;

	if (t = trapcom[0])
	{
		trapcom[0] = 0;
		execexp(t, 0);
		shfree(t);
	}
	else
		chktrap();

	rmtemp(0);
	rmfunctmp();

#ifdef ACCT
	doacct();
#endif
	exit(exitval);
}

rmtemp(base)
struct ionod	*base;
{
	while (iotemp > base)
	{
		unlink(iotemp->ioname);
		shfree(iotemp->iolink);
		iotemp = iotemp->iolst;
	}
}

rmfunctmp()
{
	while (fiotemp)
	{
		unlink(fiotemp->ioname);
		fiotemp = fiotemp->iolst;
	}
}
