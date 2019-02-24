/*	@(#)io.c	1.5	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	<fcntl.h>

short topfd;

/* ========	input output and file copying ======== */

initf(fd)
int	fd;
{
	register struct fileblk *f = standin;

	f->fdes = fd;
	f->fsiz = ((flags & oneflg) == 0 ? BUFSIZ : 1);
	f->fnxt = f->fend = f->fbuf;
	f->feval = 0;
	f->flin = 1;
	f->feof = FALSE;
}

estabf(s)
register char *s;
{
	register struct fileblk *f;

	(f = standin)->fdes = -1;
	f->fend = length(s) + (f->fnxt = s);
	f->flin = 1;
	return(f->feof = (s == 0));
}

push(af)
struct fileblk *af;
{
	register struct fileblk *f;

	(f = af)->fstak = standin;
	f->feof = 0;
	f->feval = 0;
	standin = f;
}

pop()
{
	register struct fileblk *f;

	if ((f = standin)->fstak)
	{
		if (f->fdes >= 0)
			close(f->fdes);
		standin = f->fstak;
		return(TRUE);
	}
	else
		return(FALSE);
}

struct tempblk *tmpfptr;

pushtemp(fd,tb)
	int fd;
	struct tempblk *tb;
{
	tb->fdes = fd;
	tb->fstak = tmpfptr;
	tmpfptr = tb;
}

poptemp()
{
	if (tmpfptr)
	{
		close(tmpfptr->fdes);
		tmpfptr = tmpfptr->fstak;
		return(TRUE);
	}
	else
		return(FALSE);
}
	
chkpipe(pv)
int	*pv;
{
	if (pipe(pv) < 0 || pv[INPIPE] < 0 || pv[OTPIPE] < 0)
		error(piperr);
}

chkopen(idf)
char *idf;
{
	register int	rc;

	if ((rc = open(idf, 0)) < 0)
		failed(idf, NIL, 1);
	else
		return(rc);
}

rename(f1, f2)
register int	f1, f2;
{
#ifndef SYSV
	if (f1 != f2)
	{
		dup2(f1, f2);
		close(f1);
		if (f2 == 0)
			ioset |= 1;
	}
#else
	int	fs;

	if (f1 != f2)
	{
		fs = fcntl(f2, 1, 0);
		close(f2);
		fcntl(f1, 0, f2);
		close(f1);
		if (fs == 1)
			fcntl(f2, 2, 1);
		if (f2 == 0)
			ioset |= 1;
	}
#endif
}

create(s)
char *s;
{
	register int	rc;

	if ((rc = creat(s, 0666)) < 0)
		failed(s, NIL, 1);
	else
		return(rc);
}

tmpfil(tb)
	struct tempblk *tb;
{
	int fd;

	itos(serial++);
	movstr(numbuf, tmpnam);
	fd = create(tmpout);
	pushtemp(fd,tb);
	return(fd);
}

/*
 * set by trim
 */
extern BOOL		nosubst;
#define			CPYSIZ		512

copy(ioparg)
struct ionod	*ioparg;
{
	register char	*cline;
	register char	*clinep;
	register struct ionod	*iop;
	char	c;
	char	*ends;
	char	*start;
	int		fd;
	int		i;
	

	if (iop = ioparg)
	{
		struct tempblk tb;

		copy(iop->iolst);
		ends = mactrim(iop->ioname);
		if (nosubst)
			iop->iofile &= ~IODOC;
		fd = tmpfil(&tb);

		if (fndef)
			iop->ioname = make(tmpout);
		else
			iop->ioname = cpystak(tmpout);

		iop->iolst = iotemp;
		iotemp = iop;

		cline = clinep = start = locstak();
		for (;;)
		{
			chkpr();
			if (nosubst)
			{
				c = readc();

				while (!eolchar(c))
				{
					*clinep++ = c;
					c = readc();
				}
			}
			else
			{
				c = nextc(*ends);
				
				while (!eolchar(c))
				{
					*clinep++ = c;
					c = nextc(*ends);
				}
			}

			*clinep = 0;
			if (eof || eq(cline, ends))
			{
				if ((i = cline - start) > 0)
					write(fd, start, i);
				break;
			}
			else
				*clinep++ = NL;

			if ((i = clinep - start) < CPYSIZ)
				cline = clinep;
			else
			{
				write(fd, start, i);
				cline = clinep = start;
			}
		}

		poptemp();		/* pushed in tmpfil -- bug fix for problem
					   deleting in-line scripts */
	}
}


link_iodocs(i)
	struct ionod	*i;
{
	while(i)
	{
		shfree(i->iolink);

		itos(serial++);
		movstr(numbuf, tmpnam);
		i->iolink = make(tmpout);
		link(i->ioname, i->iolink);

		i = i->iolst;
	}
}


swap_iodoc_nm(i)
	struct ionod	*i;
{
	while(i)
	{
		shfree(i->ioname);
		i->ioname = i->iolink;
		i->iolink = 0;

		i = i->iolst;
	}
}


savefd(fd)
	int fd;
{
	register int	f;

#ifdef	SYSV
	f = fcntl(fd, F_DUPFD, 10);
#else
	f = dup2(fd, 10);
#endif
	return(f);
}


restore(last)
	register int	last;
{
	register int 	i;
	register int	dupfd;

	for (i = topfd - 1; i >= last; i--)
	{
		if ((dupfd = fdmap[i].dup_fd) > 0)
			rename(dupfd, fdmap[i].org_fd);
		else
			close(fdmap[i].org_fd);
	}
	topfd = last;
}
