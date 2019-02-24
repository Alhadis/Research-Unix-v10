/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pwd library support
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

#ifndef ERANGE
#define ERANGE		E2BIG
#endif

#define ERROR(e)	{errno=e;goto error;}

#define elements(x)	(sizeof(x)/sizeof(env[0]))

struct dirlist				/* long path chdir(2) component	*/
{
	struct dirlist*	next;		/* next component		*/
	int		index;		/* index from end of buf	*/
};

static struct dirlist*	pushdir();
static int		popdir();

extern int	errno;

extern void	free();

extern int	chdir();
extern int	fstat();
extern int	stat();

extern char*	getenv();
extern char*	malloc();
extern char*	memcpy();
extern char*	realloc();

/*
 * return a pointer to the absolute path name of .
 * this path name may be longer than PATH_MAX
 *
 * a few environment variables are checked before the search algorithm
 * return value is placed in buf of len chars
 * if buf is 0 then space is allocated via malloc() with
 * len extra chars after the path name
 * 0 is returned on error with errno set as appropriate
 */

char*
getcwd(buf, len)
char*	buf;
int	len;
{
	register char*	d;
	register char*	p;
	register char*	s;
	DIR*		dirp = 0;
	int		n;
	int		x;
	int		namlen;
	int		extra = -1;
	struct dirent*	entry;
	struct dirlist*	dirstk = 0;
	struct stat*	cur;
	struct stat*	par;
	struct stat*	tmp;
	struct stat	curst;
	struct stat	parst;
	struct stat	tstst;
	char		dots[PATH_MAX];

	static struct
	{
		char*		name;
		char*		path;
		unsigned long	dev;
		unsigned long	ino;
	}		env[] =
	{
		{ "PWD",	0, 0, 0 },
		{ "HOME",	0, 0, 0 },
	};

	if (len < 0 || buf && len <= 0) ERROR(EINVAL);
	cur = &curst;
	par = &parst;
#if defined(FS_3D)
	{
		extern char*	pathcwd();

		if (p = pathcwd()) goto easy;
	}
#endif
	if (stat(".", par)) ERROR(errno);
	for (n = 0; n < elements(env); n++)
	{
		if ((p = getenv(env[n].name)) && *p == '/' && !stat(p, cur))
		{
			env[n].path = p;
			env[n].dev = cur->st_dev;
			env[n].ino = cur->st_ino;
			if (cur->st_ino == par->st_ino && cur->st_dev == par->st_dev)
			{
#if defined(FS_3D)
			easy:
#endif
				namlen = strlen(p) + 1;
				if (buf)
				{
					if (len < namlen) ERROR(ERANGE);
				}
				else if (!(buf = malloc(namlen + len))) ERROR(ENOMEM);
				return(memcpy(buf, p, namlen));
			}
		}
	}
	if (!buf)
	{
		extra = len;
		len = PATH_MAX;
		if (!(buf = malloc(len + extra))) ERROR(ENOMEM);
	}
	d = dots;
	p = buf + len - 1;
	*p = 0;
	n = elements(env);
	for (;;)
	{
		tmp = cur;
		cur = par;
		par = tmp;
		if ((d - dots) > (PATH_MAX - 4))
		{
			if (!(dirstk = pushdir(dirstk, dots, p, buf + len - 1))) ERROR(ERANGE);
			d = dots;
		}
		*d++ = '.';
		*d++ = '.';
		*d++ = '/';
		*d = 0;
		if (!(dirp = opendir(dots)) || fstat(dirp->dd_fd, par)) ERROR(errno);
		if (par->st_dev == cur->st_dev)
		{
			if (par->st_ino == cur->st_ino)
			{
				closedir(dirp);
				*--p = '/';
			pop:
				if (p != buf)
				{
					d = buf;
					while (*d++ = *p++);
					len = d - buf;
					if (extra >= 0 && !(buf = realloc(buf, len + extra))) ERROR(ENOMEM);
				}
				if (dirstk && popdir(dirstk, buf + len - 1))
				{
					dirstk = 0;
					ERROR(errno);
				}
				return(buf);
			}
			while (entry = readdir(dirp))
				if (entry->d_ino == cur->st_ino)
				{
					namlen = DIRNAMLEN(entry);
					goto found;
				}

			/*
			 * this fallthrough handles logical naming
			 */

			rewinddir(dirp);
		}
		do
		{
			if (!(entry = readdir(dirp))) ERROR(ENOENT);
			namlen = DIRNAMLEN(entry);
			if ((d - dots) > (PATH_MAX - 1 - namlen))
			{
				*d = 0;
				if (namlen >= PATH_MAX || !(dirstk = pushdir(dirstk, dots + 3, p, buf + len - 1))) ERROR(ERANGE);
				d = dots + 3;
			}
			(void)memcpy(d, entry->d_name, namlen + 1);
			if (stat(dots, &tstst)) ERROR(errno);
		} while (tstst.st_ino != cur->st_ino || tstst.st_dev != cur->st_dev);
	found:
		if (*p) *--p = '/';
		while ((p -= namlen) <= (buf + 1))
		{
			x = (buf + len - 1) - (p += namlen);
			s = buf + len;
			if (extra < 0 || !(buf = realloc(buf, (len += PATH_MAX) + extra))) ERROR(ERANGE);
			p = buf + len;
			while (p > buf + len - 1 - x) *--p = *--s;
		}
		if (n < elements(env))
		{
			(void)memcpy(p, env[n].path, namlen);
			goto pop;
		}
		(void)memcpy(p, entry->d_name, namlen);
		closedir(dirp);
		dirp = 0;
		for (n = 0; n < elements(env); n++)
			if (env[n].ino == (unsigned long)par->st_ino && env[n].dev == par->st_dev)
			{
				namlen = strlen(env[n].path);
				goto found;
			}
	}
 error:
	if (buf)
	{
		if (dirstk) (void)popdir(dirstk, buf + len - 1);
		if (extra >= 0) free(buf);
	}
	if (dirp) closedir(dirp);
	return(0);
}

/*
 * push long dir component onto stack
 */

static struct dirlist*
pushdir(d, dots, path, end)
register struct dirlist*	d;
char*				dots;
char*				path;
char*				end;
{
	register struct dirlist*	p;

	if (!(p = (struct dirlist*)malloc(sizeof(struct dirlist))) || chdir(dots))
	{
		if (p) free(p);
		if (d) (void)popdir(d, end);
		return(0);
	}
	p->index = end - path;
	p->next = d;
	return(p);
}

/*
 * pop long dir component chdir stack
 */

static int
popdir(d, end)
register struct dirlist*	d;
register char*			end;
{
	register struct dirlist*	dp;
	int				v;

	v = 0;
	while (dp = d)
	{
		d = d->next;
		if (!v)
		{
			if (d) *(end - d->index - 1) = 0;
			v = chdir(end - dp->index);
			if (d) *(end - d->index - 1) = '/';
		}
		free(dp);
	}
	return(v);
}
