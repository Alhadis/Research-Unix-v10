/*
 * AT&T Bell Laboratories
 * return the real absolute pathname of the preroot dir for cmd
 * if cmd==0 then current preroot path returned
 */

#include <preroot.h>

#if FS_PREROOT

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

#ifndef ERANGE
#define ERANGE		E2BIG
#endif

#define ERROR(e)	{errno=e;goto error;}

extern int		errno;

extern char*		strdup();

char*
getpreroot(path, cmd)
char*	path;
char*	cmd;
{
	register int	c;
	register char*	p;
	register FILE*	fp;
	char		buf[PATH_MAX];

	if (!path) path = buf;
	if (cmd)
	{
		(void)sprintf(buf, "set x `%s= %s - </dev/null 2>&1`\nwhile :\ndo\nshift\ncase $# in\n[012]) break ;;\nesac\ncase \"$1 $2\" in\n\"+ %s\")	echo $3; break ;;\nesac\ndone\n", PR_SILENT, cmd, PR_COMMAND);
		if (!(fp = popen(buf, "rug"))) return(0);
		for (p = path; (c = getc(fp)) != EOF && c != '\n'; *p++ = c);
		*p = 0;
		(void)pclose(fp);
		if (path == p) return(0);
		return(path == buf ? strdup(path) : path);
	}
	else
	{
		char*		d;
		DIR*		dirp = 0;
		int		namlen;
		int		euid;
		int		ruid;
		struct dirent*	entry;
		struct stat*	cur;
		struct stat*	par;
		struct stat*	tmp;
		struct stat	curst;
		struct stat	parst;
		struct stat	tstst;
		char		dots[PATH_MAX];

		cur = &curst;
		par = &parst;
		if ((ruid = getuid()) != (euid = geteuid())) (void)setuid(ruid);
		if (stat(PR_REAL, cur) || stat("/", par) || cur->st_dev == par->st_dev && cur->st_ino == par->st_ino) ERROR(ENOTDIR);

		/*
		 * like getcwd() but starting at the preroot
		 */

		d = dots;
		*d++ = '/';
		p = path + PATH_MAX - 1;
		*p = 0;
		for (;;)
		{
			tmp = cur;
			cur = par;
			par = tmp;
			if ((d - dots) > (PATH_MAX - 4)) ERROR(ERANGE);
			*d++ = '.';
			*d++ = '.';
			*d = 0;
			if (!(dirp = opendir(dots)) || fstat(dirp->dd_fd, par)) ERROR(errno);
			*d++ = '/';
			if (par->st_dev == cur->st_dev)
			{
				if (par->st_ino == cur->st_ino)
				{
					closedir(dirp);
					*--p = '/';
					if (ruid != euid) (void)setuid(euid);
					if (path == buf) return(strdup(p));
					if (path != p)
					{
						d = path;
						while (*d++ = *p++);
					}
					return(path);
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
				if ((d - dots) > (PATH_MAX - 1 - namlen)) ERROR(ERANGE);
				(void)memcpy(d, entry->d_name, namlen + 1);
				if (stat(dots, &tstst)) ERROR(errno);
			} while (tstst.st_ino != cur->st_ino || tstst.st_dev != cur->st_dev);
		found:
			if (*p) *--p = '/';
			if ((p -= namlen) <= (path + 1)) ERROR(ERANGE);
			(void)memcpy(p, entry->d_name, namlen);
			closedir(dirp);
			dirp = 0;
		}
	error:
		if (dirp) closedir(dirp);
		if (ruid != euid) (void)setuid(euid);
	}
	return(0);
}

#else

int	_lib_dummy;

#endif
