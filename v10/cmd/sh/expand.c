/*	@(#)expand.c	1.4	*/
/*
 *	UNIX shell
 *
 *	Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#ifndef BSD4_2
#include	<ndir.h>
#else
#include	<sys/dir.h>
#endif

#ifdef	BSD4_2
#define		DIRSIZE	MAXNAMELEN
#else
#define		DIRSIZE	255
#endif
#ifndef	MAXNAMELEN
#define	MAXNAMELEN	255
#endif


static char		entry[DIRSIZE+1];

/*
 * globals (file name generation)
 *
 * "*" in params matches r.e ".*"
 * "?" in params matches r.e. "."
 * "[...]" in params matches character class
 * "[...a-z...]" in params matches a through z.
 *
 */

static	addg();

expand(as, rcnt)
	char	*as;
{
	int	count;
	DIR	*dirf;
	BOOL	dir = 0;
	char	*rescan = 0;
	register char	*s, *cs;
	struct argnod	*schain = gchain;
	BOOL	slash;

	if (trapnote & SIGSET)
		return(0);
	s = cs = as;
	/*
	 * check for meta chars
	 */
	{
		register BOOL open;

		slash = 0;
		open = 0;
		do
		{
			switch (*cs++)
			{
			case 0:
				if (rcnt && slash)
					break;
				else
					return(0);

			case '/':
				slash++;
				open = 0;
				continue;

			case '[':
				open++;
				continue;

			case ']':
				if (open == 0)
					continue;

			case '?':
			case '*':
				if (rcnt > slash)
					continue;
				else
					cs--;
				break;


			default:
				continue;
			}
			break;
		} while (TRUE);
	}

	for (;;)
	{
		if (cs == s)
		{
			s = nullstr;
			break;
		}
		else if (*--cs == '/')
		{
			*cs = 0;
			if (s == cs)
				s = "/";
			break;
		}
	}

	if ((dirf = opendir(*s ? s : ".")) != 0)
		dir = TRUE;
	
	count = 0;
	if (*cs == 0)
		*cs++ = 0200;

	if(dir)
	{
		register char *rs;
		struct direct *e;

		rs = cs;
		do
		{
			if (*rs == '/')
			{
				rescan = rs;
				*rs = 0;
				gchain = 0;
			}
		} while (*rs++);

		while ((e = readdir(dirf)) && (trapnote & SIGSET) == 0)
		{
			*(movstrn(e->d_name, entry, DIRSIZE)) = 0;

			if (entry[0] == '.' && *cs != '.')
			{
				if (entry[1] == 0)
					continue;
				if (entry[1] == '.' && entry[2] == 0)
					continue;
			}

			if (gmatch(entry, cs))
			{
				addg(s, entry, rescan);
				count++;
			}
		}
		closedir(dirf);

		if (rescan)
		{
			register struct argnod	*rchain;

			rchain = gchain;
			gchain = schain;
			if (count)
			{
				count = 0;
				while (rchain)
				{
					count += expand(rchain->argval, slash + 1);
					rchain = rchain->argnxt;
				}
			}
			*rescan = '/';
		}
	}

	{
		register char	c;

		s = as;
		while (c = *s)
			*s++ = (c & STRIP ? c : '/');
	}
	return(count);
}



gmatch(s, p)
register char	*s, *p;
{
	register int	scc;
	char		c;

	if (scc = *s++)
	{
		if ((scc &= STRIP) == 0)
			scc=0200;
	}
	switch (c = *p++)
	{
	case '[':
		{
			BOOL ok;
			int lc;
			int notflag = 0;

			ok = 0;
			lc = 077777;
			if (*p == '^')
			{
				notflag = 1;
				p++;
			}
			while (c = *p++)
			{
				if (c == ']')
					return(ok ? gmatch(s, p) : 0);
				else if (c == MINUS)
				{
					if (notflag)
					{
						if (scc < lc || scc > *(p++))
							ok++;
						else
							return(0);
					}
					else
					{
						if (lc <= scc && scc <= (*p++))
							ok++;
					}
				}
				else
				{
					lc = c & STRIP;
					if (notflag)
					{
						if (scc && scc != lc)
							ok++;
						else
							return(0);
					}
					else
					{
						if (scc == lc)
							ok++;
					}
				}
			}
			return(0);
		}

	default:
		if ((c & STRIP) != scc)
			return(0);

	case '?':
		return(scc ? gmatch(s, p) : 0);

	case '*':
		while (*p == '*')
			p++;

		if (*p == 0)
			return(1);
		--s;
		while (*s)
		{
			if (gmatch(s++, p))
				return(1);
		}
		return(0);

	case 0:
		return(scc == 0);
	}
}

static
addg(as1, as2, as3)
char	*as1, *as2, *as3;
{
	register char	*s1;
	register int	c;

	staktop = locstak() + BYTESPERWORD;
	s1 = as1;
	while (c = *s1++)
	{
		if ((c &= STRIP) == 0)
		{
			pushstak('/');
			break;
		}
		pushstak(c);
	}
	s1 = as2;
	while (c = *s1++)
		pushstak(c);
	if (s1 = as3)
	{
		pushstak('/');
		do
			pushstak(*++s1);
		while(*s1);
	}
	makearg(fixstak());
}

makearg(args)
	register struct argnod *args;
{
	args->argnxt = gchain;
	gchain = args;
}


DIR *
opendir(name)
register char *name;
{
	DIR dirbuf, *dirp;
	struct stat statb;
	char buf[MAXNAMELEN+1];
	register char *s;

	*(movstrn(name, buf, MAXNAMELEN)) = 0;
	for (s=buf; *s; s++)
		*s &= STRIP;
	if ((dirbuf.dd_fd = open(buf, 0)) < 0)
		return(NULL);
	if (fstat(dirbuf.dd_fd, &statb)!=0 || (statb.st_mode & S_IFMT)!=S_IFDIR){
		close(dirbuf.dd_fd);
		return(NULL);
	}
	dirbuf.dd_size = dirbuf.dd_loc = 0;
	dirp = (DIR *)shalloc(sizeof(DIR));
	*dirp = dirbuf;
	return(dirp);
}

void
closedir(dirp)
DIR *dirp;
{
	close(dirp->dd_fd);
	shfree((char *)dirp);
}
