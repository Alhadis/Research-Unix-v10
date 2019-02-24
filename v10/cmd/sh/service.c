/*	@(#)service.c	1.11	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	<errno.h>

#ifdef	CRAY
#define	ARGMK	040000000
#else
#define ARGMK	01
#endif

static int	gsort();
static int	split();
extern char	*sysmsg[];
extern int 	maxsysmsg;
extern short topfd;



/*
 * service routines for `execute'
 */
initio(iop, save)
	struct ionod	*iop;
	int		save;
{
	register char	*ion;
	register int	iof, fd;
	int		ioufd;
	short	lastfd;

	lastfd = topfd;
	while (iop)
	{
		iof = iop->iofile;
		ion = mactrim(iop->ioname);
		ioufd = iof & IOUFD;

		if (*ion && (flags&noexec) == 0)
		{
			if (save)
			{
				fdmap[topfd].org_fd = ioufd;
				fdmap[topfd++].dup_fd = savefd(ioufd);
			}

			if (iof & IODOC)
			{
				struct tempblk tb;

				subst(chkopen(ion), (fd = tmpfil(&tb)));

				poptemp();	/* pushed in tmpfil() --
						   bug fix for problem with
						   in-line scripts
						*/

				fd = chkopen(tmpout);
				unlink(tmpout);
			}
			else if (iof & IOMOV)
			{
				if (eq(minus, ion))
				{
					fd = -1;
					close(ioufd);
				}
				else if ((fd = stoi(ion)) >= USERIO)
					failed(ion, badfile, 0);
				else
					fd = dup(fd);
			}
			else if ((iof & IOPUT) == 0)
				fd = chkopen(ion);
			else if (iof & IOAPP && (fd = open(ion, 1)) >= 0)
				lseek(fd, 0L, 2);
			else
				fd = create(ion);
			if (fd >= 0)
				rename(fd, ioufd);
		}

		iop = iop->ionxt;
	}
	if (histfd > 0) {
		close (histfd);
		histfd = 0;
	}
	return(lastfd);
}

char *
simple(s)
char	*s;
{
	char	*sname;

	sname = s;
	while (1)
	{
		if (any('/', sname))
			while (*sname++ != '/')
				;
		else
			return(sname);
	}
}

char *
getpath(s)
	char	*s;
{
	register char	*path;

	if (any('/', s) || any(('/' | QUOTE), s))
	{
		return(nullstr);
	}
	else if ((path = pathnod.namval.val) == 0)
		return(defpath);
	else
		return(cpystak(path));
}

pathopen(path, name)
register char *path, *name;
{
	register int	f;

	do
	{
		path = catpath(path, name);
	} while ((f = open(curstak(), 0)) < 0 && path);
	return(f);
}

char *
catpath(path, name)
register char	*path;
char	*name;
{
	/*
	 * leaves result on top of stack
	 */
	register char	*scanp = path;
	register char	*argp = locstak();

	while (*scanp && *scanp != COLON)
		pushstak(*scanp++);
	if (scanp != path)
		pushstak('/');
	if (*scanp == COLON)
		scanp++;
	path = (*scanp ? scanp : 0);
	scanp = name;
	do
		pushstak(*scanp);
	while(*scanp++);
	staktop=argp;
	return path;
}

char *
nextpath(path)
	register char	*path;
{
	register char	*scanp = path;

	while (*scanp && *scanp != COLON)
		scanp++;

	if (*scanp == COLON)
		scanp++;

	return(*scanp ? scanp : 0);
}

static char	*xecmsg;
static char	**xecenv;

int	execa(at)
	char	*at[];
{
	register char	*path;
	register char	**t = at;
	int		cnt;

	if ((flags & noexec) == 0)
	{
		xecmsg = notfound;
		path = getpath(*t);
		xecenv = setenv();

		while (path = execs(path,t))
			;
		failed(*t, xecmsg, 0);
	}
}

char *
execs(ap, t)
char	*ap;
register char	*t[];
{
	register char *p, *prefix;

	prefix = catpath(ap, t[0]);
	trim(p = curstak());
	sigchk();
	
	execve(p, &t[0], xecenv);
	switch (errno)
	{
	case ENOEXEC:		/* could be a shell script */
		funcnt = 0;
		flags = 0;
		*flagadr = 0;
		comdiv = 0;
		ioset = 0;
		clearup();	/* remove open files and for loop junk */
		if (input)
			close(input);
		input = chkopen(p);
	
#ifdef ACCT
		preacct(p);	/* reset accounting */
#endif

		/*
		 * set up new args
		 */
		
		setargs(t);
		longjmp(subshell, 1);

	case ENOMEM:
	case E2BIG:
	case ETXTBSY:
		failed(p, NIL, 1);

	default:
		xecmsg = badexec;
	case ENOENT:
		return(prefix);
	}
}


/*
 * for processes to be waited for
 */
#define MAXP 20
static int	pwlist[MAXP];
static int	pwc;

postclr()
{
	register int	*pw = pwlist;

	while (pw <= &pwlist[pwc])
		*pw++ = 0;
	pwc = 0;
}

post(pcsid)
int	pcsid;
{
	register int	*pw = pwlist;

	if (pcsid)
	{
		while (*pw)
			pw++;
		if (pwc >= MAXP - 1)
			pw--;
		else
			pwc++;
		*pw = pcsid;
	}
}

await(i, bckg)
int	i, bckg;
{
	int	rc = 0, wx = 0;
	int	w;
	int	ipwc = pwc;

	post(i);
	while (pwc)
	{
		register int	p;
		register int	sig;
		int		w_hi;
		int	found = 0;

		{
			register int	*pw = pwlist;

			p = wait(&w);
			if (wasintr)
			{
				wasintr = 0;
				if (bckg)
				{
					break;
				}
			}
			while (pw <= &pwlist[ipwc])
			{
				if (*pw == p)
				{
					*pw = 0;
					pwc--;
					found++;
				}
				else
					pw++;
			}
		}
		if (p == -1)
		{
			if (bckg)
			{
				register int *pw = pwlist;

				while (pw <= &pwlist[ipwc] && i != *pw)
					pw++;
				if (i == *pw)
				{
					*pw = 0;
					pwc--;
				}
			}
			continue;
		}
		w_hi = (w >> 8) & LOBYTE;
		if (sig = w & 0177)
		{
			if (sig == 0177)	/* ptrace! return */
			{
				prs("ptrace: ");
				sig = w_hi;
			}
			if (sig < 0 || sig >= maxsysmsg || sysmsg[sig])
			{
				if (i != p || (flags & prompt) == 0)
				{
					prp();
					prn(p);
					blank();
				}
				if (sig < 0 || sig >= maxsysmsg) {
					prs("Signal ");
					prn(sig);
				} else
					prs(sysmsg[sig]);
				if (w & 0200)
					prs(coredump);
			}
			newline();
		}
		if (rc == 0 && found != 0)
			rc = (sig ? sig | SIGFLG : w_hi);
		wx |= w;
		if (p == i)
		{
			break;
		}
	}
	if (wx && flags & errflg)
		exitsh(rc);
	flags |= eflag;
	exitval = rc;
	exitset();
}

BOOL		nosubst;

trim(at)
char	*at;
{
	register char	*p;
	register char 	*ptr;
	register char	c;
	register char	q = 0;

	if (p = at)
	{
		ptr = p;
		while (c = *p++)
		{
			if (*ptr = c & STRIP)
				++ptr;
			q |= c;
		}

		*ptr = 0;
	}
	nosubst = q & QUOTE;
}

char *
mactrim(s)
char	*s;
{
	register char	*t = macro(s);

	trim(t);
	return(t);
}

char **
scan(argn)
int	argn;
{
	register struct argnod *argp = (struct argnod *)(Rcheat(gchain) & ~ARGMK);
	register char **comargn, **comargm;

	comargn = (char **)getstak(BYTESPERWORD * argn + BYTESPERWORD);
	comargm = comargn += argn;
	*comargn = ENDARGS;
	while (argp)
	{
		*--comargn = argp->argval;

		trim(*comargn);
		argp = argp->argnxt;

		if (argp == 0 || Rcheat(argp) & ARGMK)
		{
			gsort(comargn, comargm);
			comargm = comargn;
		}
		/* Lcheat(argp) &= ~ARGMK; */
		argp = (struct argnod *)(Rcheat(argp) & ~ARGMK);
	}
	return(comargn);
}

static int
gsort(from, to)
char	*from[], *to[];
{
	int	k, m, n;
	register int	i, j;

	if ((n = to - from) <= 1)
		return;
	for (j = 1; j <= n; j *= 2)
		;
	for (m = 2 * j - 1; m /= 2; )
	{
		k = n - m;
		for (j = 0; j < k; j++)
		{
			for (i = j; i >= 0; i -= m)
			{
				register char **fromi;

				fromi = &from[i];
				if (cf(fromi[m], fromi[0]) > 0)
				{
					break;
				}
				else
				{
					char *s;

					s = fromi[m];
					fromi[m] = fromi[0];
					fromi[0] = s;
				}
			}
		}
	}
}

/*
 * Argument list generation
 */
getarg(ac)
struct comnod	*ac;
{
	register struct argnod	*argp;
	register int		count = 0;
	register struct comnod	*c;

	if (c = ac)
	{
		argp = c->comarg;
		while (argp)
		{
			count += split(macro(argp->argval));
			argp = argp->argnxt;
		}
	}
	return(count);
}

static int
split(s)		/* blank interpretation routine */
register char	*s;
{
	register char	*argp;
	register int	c;
	int		count = 0;

	for (;;)
	{
		sigchk();
		staktop = argp = locstak() + BYTESPERWORD;
		while ((c = *s++, !any(c, ifsnod.namval.val) && c))
			pushstak(c);
		if (argp == staktop)
		{
			if (c)
			{
				continue;
			}
			else
			{
				staktop = stakbot;
				return(count);
			}
		}
		else if (c == 0)
			s--;
		/*
		 * file name generation
		 */

		argp = fixstak();

		if ((flags & nofngflg) == 0 && 
			(c = expand(((struct argnod *)argp)->argval, 0)))
			count += c;
		else
		{
			makearg(argp);
			count++;
		}
		gchain = (struct argnod *)((int)gchain | ARGMK);
	}
}

#ifdef ACCT
#include	<sys/types.h>
#include	"acctdef.h"
#include	<sys/acct.h>
#include 	<sys/times.h>

struct acct sabuf;
struct tms buffer;
extern long times();
static long before;
static int shaccton;	/* 0 implies do not write record on exit
			   1 implies write acct record on exit
			*/


/*
 *	suspend accounting until turned on by preacct()
 */

suspacct()
{
	shaccton = 0;
}

preacct(cmdadr)
	char *cmdadr;
{
	char *simple();

	if (acctnod.namval.val && *acctnod.namval.val)
	{
		sabuf.ac_btime = time((long *)0);
		before = times(&buffer);
		sabuf.ac_uid = getuid();
		sabuf.ac_gid = getgid();
		movstrn(simple(cmdadr), sabuf.ac_comm, sizeof(sabuf.ac_comm));
		shaccton = 1;
	}
}

#include	<fcntl.h>

doacct()
{
	int fd;
	long int after;

	if (shaccton)
	{
		after = times(&buffer);
		sabuf.ac_utime = compress(buffer.tms_utime + buffer.tms_cutime);
		sabuf.ac_stime = compress(buffer.tms_stime + buffer.tms_cstime);
		sabuf.ac_etime = compress(after - before);

		if ((fd = open(acctnod.namval.val, O_WRONLY | O_APPEND | O_CREAT, 0666)) != -1)
		{
			write(fd, &sabuf, sizeof(sabuf));
			close(fd);
		}
	}
}

/*
 *	Produce a pseudo-floating point representation
 *	with 3 bits base-8 exponent, 13 bits fraction
 */

compress(t)
	register time_t t;
{
	register exp = 0;
	register rund = 0;

	while (t >= 8192)
	{
		exp++;
		rund = t & 04;
		t >>= 3;
	}

	if (rund)
	{
		t++;
		if (t >= 8192)
		{
			t >>= 3;
			exp++;
		}
	}

	return((exp << 13) + t);
}
#endif
