/*	@(#)main.c	1.7	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	"sym.h"
#include	"timeout.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/filio.h>

static BOOL	beenhere = FALSE;
char		tmpout[20] = "/tmp/sh-";
struct fileblk	stdfile;
struct fileblk *standin = &stdfile;

static int	mailchk = 180;
static char	*mailp;
static long	mod_time = 0;

#ifdef pdp11
#include	<execargs.h>
#endif

extern int	exfile();
extern char 	*simple();



main(c, v, e)
int	c;
char	*v[];
char	*e[];
{
	struct namnod	*n;

	stdsigs();

	/*
	 * initialise storage allocation
	 */

	stakbot = 0;
	addblok((unsigned)0);

	/*
	 * set names from userenv
	 */

	setup_env();

	/*
	 * look for options
	 * dolc is $#
	 */
	dolc = options(c, v);

	if (dolc < 2)
	{
		flags |= stdflg;
		{
			register char *flagc = flagadr;

			while (*flagc)
				flagc++;
			*flagc++ = STDFLG;
			*flagc = 0;
		}
	}
	if ((flags & stdflg) == 0)
		dolc--;
	dolv = v + c - dolc;
	dolc--;

	/*
	 * return here for shell file execution
	 * but not for parenthesis subshells
	 */
	setjmp(subshell);

	/*
	 * number of positional parameters
	 */
	replace(&cmdadr, dolv[0]);	/* cmdadr is $0 */

	/*
	 * set pidname '$$'
	 */
	assnum(&pidadr, getpid());

	/*
	 * set up temp file names
	 */
	settmp();

	/*
	 * default internal field separators - $IFS
	 */
	dfault(&ifsnod, sptbnl);

	if ((beenhere++) == FALSE)	/* ? profile */
	{
		if (*(simple(cmdadr)) == '-')
		{
			if ((input = pathopen(nullstr, profile)) >= 0)
			{
				exfile(ttyflg);
				flags &= ~ttyflg;
			}
		}
		/*
		 * open input file if specified
		 */
		if (comdiv)
		{
			estabf(comdiv);
			input = -1;
		}
		else
		{
			input = ((flags & stdflg) ? 0 : chkopen(cmdadr));

#ifdef ACCT
			if (input != 0)
				preacct(cmdadr);
#endif
			comdiv--;
		}
	}
#ifdef pdp11
	else
		*execargs = (char *)dolv;	/* for `ps' cmd */
#endif
		
	exfile(0);
	done();
}

static int
exfile(prof)
BOOL	prof;
{
	long	mailtime = 0;	/* Must not be a register variable */
	long 	curtime = 0;
	register int	userid;

	/*
	 * move input
	 */
	if (input > 0)
	{
		Ldup(input, INIO);
		input = INIO;
	}

	userid = geteuid();

	/*
	 * decide whether interactive
	 */
	if ((flags & intflg) ||
	    ((flags&oneflg) == 0 &&
	    isatty(output) &&
	    isatty(input)) )
	    
	{
		dfault(&ps1nod, (userid ? stdprompt : supprompt));
		dfault(&ps2nod, readmsg);
		flags |= ttyflg | prompt;
		ignsig(SIGTERM);
		setmail(mailnod.namval.val);
	}
	else
	{
		flags |= prof;
		flags &= ~prompt;
	}

	if (setjmp(errshell) && prof)
	{
		close(input);
		return;
	}
	/*
	 * error return here
	 */

	loopcnt = peekc = peekn = 0;
	fndef = 0;
	iopend = 0;

	if (input >= 0)
		initf(input);
	/*
	 * command loop
	 */
	for (;;)
	{
		tdystak(0);
		stakchk();	/* may reduce sbrk */
		exitset();

		if ((flags & prompt) && standin->fstak == 0 && !eof)
		{

			if (mailp)
			{
				time(&curtime);

				if ((curtime - mailtime) >= mailchk)
				{
					chkmail();
				        mailtime = curtime;
				}
			}

			prs(ps1nod.namval.val);

#ifdef TIME_OUT
			alarm(TIMEOUT);
#endif

			flags |= waiting;
		}

		trapnote = 0;
		peekc = readc();
		if (eof)
			return;

#ifdef TIME_OUT
		alarm(0);
#endif

		flags &= ~waiting;

		execute(cmd(NL, MTFLG), 0, eflag);
		eof |= (flags & oneflg);
	}
}

chkpr()
{
	if ((flags & prompt) && standin->fstak == 0)
		prs(ps2nod.namval.val);
}

settmp()
{
	itos(getpid());
	serial = 0;
	tmpnam = movstr(numbuf, &tmpout[TMPNAM]);
}

Ldup(fa, fb)
register int	fa, fb;
{

#ifndef	SYSV
	dup2(fa, fb);
	close(fa);
	ioctl(fb, FIOCLEX, 0);
#else
        if (fa >= 0)
                { close(fb);
                  fcntl(fa,0,fb);               /* normal dup */
                  close(fa);
                  fcntl(fb, 2, 1);              /* autoclose for fb */
                }
#endif
}


chkmail()
{
	struct stat	statb;

	if (mailp && stat(mailp, &statb) >= 0)
	{
		if(statb.st_size && mod_time
			&& statb.st_mtime != mod_time)
		{
			prs(mailmsg);
		}
		mod_time = statb.st_mtime;
	}
	else if (mod_time == 0)
		mod_time = 1;
}


setmail(mailpath)
	char *mailpath;
{

	if (mailp = mailpath)
		mod_time=0;
}
