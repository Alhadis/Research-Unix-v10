/*	@(#)word.c	1.4	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	"sym.h"
#include	<errno.h>

static int	readb();
static int	histc();

/* ========	character handling for command lines	========*/


word()
{
	register char	c, d;
	int		alpha = 1;
	int		lbrok, rbrok;

	wdnum = 0;
	wdset = 0;

	while (1)
	{
		while (c = nextc(0), space(c))		/* skipc() */
			;

		if (c == COMCHAR)
		{
			while ((c = readc()) != NL && c != EOF);
			peekc = c;
		}
		else
		{
			break;	/* out of comment - white space loop */
		}
	}
	if (!eofmeta(c))
	{
		struct argnod	*arg = (struct argnod *)locstak();
		/* pushstak() starting at arg->argval[0] */

		staktop += BYTESPERWORD;

		lbrok = rbrok = 0;
		do
		{
			if (c == LITERAL)
			{
				pushstak(DQUOTE);
				while ((c = readc()) && c != LITERAL)
				{
					pushstak(c | QUOTE);
					if (c == NL)
						chkpr();
				}
				pushstak(DQUOTE);
			}
			else
			{
				pushstak(c);
				if (c == '=')
					wdset |= alpha;
				/* hack for ${} */
				else if (c == '$')
					lbrok++;
				else if (lbrok)
				{
					if (c == '{')
						rbrok++;
					lbrok = 0;
				}
				else if (rbrok && c == '}')
					rbrok--;
				if (!alphanum(c))
					alpha = 0;
				if (qotchar(c))
				{
					d = c;
					while ((pushstak(c = nextc(d)), c) && c != d)
					{
						if (c == NL)
							chkpr();
					}
				}
			}
		} while ((c = nextc(0), !eofmeta(c) || (lbrok && c=='{') || (rbrok && c=='}')));
		fixstak();
		if (!letter(arg->argval[0]))
			wdset = 0;

		peekn = c | MARK;
		if (arg->argval[1] == 0 && 
		    (d = arg->argval[0], digit(d)) && 
		    (c == '>' || c == '<'))
		{
			word();
			wdnum = d - '0';
		}
		else		/*check for reserved words*/
		{
			if (reserv == FALSE || (wdval = syslook(arg->argval,reserved, no_reserved)) == 0)
			{
				wdarg = arg;
				wdval = 0;
			}
		}
	}
	else if (dipchar(c))
	{
		if ((d = nextc(0)) == c)
		{
			wdval = c | SYMREP;
			if (c == '<')
				peekn = nextc(0) | MARK;
		}
		else
		{
			peekn = d | MARK;
			wdval = c;
		}
	}
	else
	{
		if ((wdval = c) == EOF)
			wdval = EOFSYM;
		if (iopend && eolchar(c))
		{
			copy(iopend);
			iopend = 0;
		}
	}
	reserv = FALSE;
	return(wdval);
}

skipc()
{
	register char c;

	while (c = nextc(0), space(c))
		;
	return(c);
}

nextc(quote)
char	quote;
{
	register char	c, d;

retry:
	if ((d = readc()) == ESCAPE)
	{
		if ((c = readc()) == NL)
		{
			chkpr();
			goto retry;
		}
		else if (quote && c != quote && !escchar(c))
			peekc = c | MARK;
		else
			d = c | QUOTE;
	}
	return(d);
}
readc()
{
	register char	c;
	register int	len;
	register struct fileblk *f;

	if (peekn)
	{
		peekc = peekn;
		peekn = 0;
	}
	if (peekc)
	{
		c = peekc;
		peekc = 0;
		return(c);
	}
	f = standin;
retry:
	if (f->fnxt != f->fend)
	{
		if ((c = *f->fnxt++) == 0)
		{
			if (f->feval)
			{
				if (estabf(*f->feval++))
					c = EOF;
				else
					c = SP;
			}
			else
				goto retry;	/* = c = readc(); */
		}
		if (standin->fstak == 0) {
			if (flags & readpr)
				prc(c);
			if (flags & prompt && histnod.namval.val)
				histc (c);
		}
		if (c == NL)
			f->flin++;
	}
	else if (f->feof || f->fdes < 0)
	{
		c = EOF;
		f->feof++;
	}
	else if ((len = readb()) <= 0)
	{
		close(f->fdes);
		f->fdes = -1;
		c = EOF;
		f->feof++;
	}
	else
	{
		f->fend = (f->fnxt = f->fbuf) + len;
		goto retry;
	}
	return(c);
}

static
readb()
{
	register struct fileblk *f = standin;
	register int	len;

	do
	{
		if (trapnote & SIGSET)
		{
			newline();
			sigchk();
		}
		else if ((trapnote & TRAPSET) && (rwait > 0))
		{
			newline();
			chktrap();
			clearup();
		}
	} while ((len = read(f->fdes, f->fbuf, f->fsiz)) < 0 && errno==EINTR && trapnote);
	return(len);
}

int histfd = 0;
static histc (c)
{
	static char histbuf[256];
	static char *histp=histbuf;

	if (histfd == 0) {
		if ((histfd = open(histnod.namval.val, 1)) < 0 &&
		    (histfd = creat(histnod.namval.val, 0666)) < 0)
			return;
	}
	if (histfd > 0) {
		*histp++ = c;
		if (c == '\n' || c == ';' || histp >= &histbuf[sizeof histbuf]) {
			lseek(histfd, 0L, 2);
			write(histfd, histbuf, histp-histbuf);
			histp = histbuf;
		}
	}
}
