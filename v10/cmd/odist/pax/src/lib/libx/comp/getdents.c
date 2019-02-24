#include "FEATURE/getdents"
#ifdef _lib_getdents

int	_lib_dummy;

#else

/*
 * getdents
 *
 * read directory entries into directory block
 *
 * NOTE: directory entries must fit within DIRBLKSIZ boundaries
 */

#include <fsinfo.h>
#include <dirent.h>
#include <errno.h>

#define round(x,y)	(((x)+((y)-1))&~((y)-1))

#ifndef	DIRBLKSIZ
#ifdef	DIRBLK
#define DIRBLKSIZ	DIRBLK
#else
#ifdef	DIRBUF
#define DIRBLKSIZ	DIRBUF
#else
#ifdef	DEV_BSIZE
#define DIRBLKSIZ	DEV_BSIZE
#else
#define DIRBLKSIZ	4096
#endif
#endif
#endif
#endif

#ifndef MAXNAMLEN
#define MAXNAMLEN	255
#endif

int
getdents(fd, buf, siz)
int	fd;
char*	buf;
int	siz;
{
	struct stat		st;

	extern int		errno;
	
	extern int		fstat();

	if (siz < DIRBLKSIZ)
	{
		errno = EINVAL;
		return(-1);
	}
	if (fstat(fd, &st)) return(-1);
	if ((st.st_mode & S_IFMT) != S_IFDIR)
	{
#ifdef ENOTDIR
		errno = ENOTDIR;
#else
		errno = EBADF;
#endif
		return(-1);
	}
#include "FEATURE/getdirentries"
#ifdef _lib_getdirentries
	{
		long		off;

		extern int	getdirentries();

		return(getdirentries(fd, buf, siz, &off));
	}
#else
#include "FEATURE/dirread"
#ifdef _lib_dirread
	{
		register char*		sp;	/* system */
		register struct dirent*	up;	/* user */
		char*			u;
		int			n;
		int			m;
		int			i;

		extern int		dirread();

		m = (siz * 6) / 10;
		m = round(m, 8);
		sp = buf + siz - m - 1;
		if (!(n = dirread(fd, sp, m))) return(0);
		if (n > 0)
		{
			up = (struct dirent*)buf;
			sp[n] = 0;
			while (sp < buf + siz - m + n)
			{
				i = 0;
				while (*sp >= '0' && *sp <= '9')
					i = 10 * i + *sp++ - '0';
				while (*sp && *sp != '\t') sp++;
				if (*sp++)
				{
					up->d_ino = i;
					u = up->d_name;
					while ((*u = *sp++) && u < up->d_name + MAXNAMLEN) u++;
					*u = 0;
					up->d_reclen = sizeof(struct dirent) - sizeof(up->d_name) + (
#if defined(d_fileno) || defined(d_ino)
						up->d_namlen =
#endif
						u - up->d_name) + 1;
					up->d_reclen = round(up->d_reclen, 8);
					up = (struct dirent*)((char*)up + up->d_reclen);
				}
			}
			return((char*)up - buf);
		}
	}
#endif
	{
#ifndef _dir_fixed
#include "FEATURE/readdir"
#ifndef _lib_readdir
#define _dir_fixed
#endif
#endif

#ifdef _dir_fixed
#ifndef DIRSIZ
#include "FEATURE/dir"
#ifdef _sys_dir
#include <sys/dir.h>
#endif
#endif

#define MAXREC	round(sizeof(*up)-sizeof(up->d_name)+sizeof(sp->d_name)+1,8)

		register struct direct*	sp;	/* system */
		register struct dirent*	up;	/* user */
		register char*		s;
		register char*		u;
		int			n;
		int			m;
		char			tmp[sizeof(sp->d_name) + 1];

		extern int		read();
		extern int		strcpy();
	
		/*
		 * we assume sizeof(struct dirent) > sizeof(struct direct)
		 */

		up = (struct dirent*)buf;
		n = (siz / MAXREC) * sizeof(struct direct);
		if ((!(m = n & ~511) || m < MAXREC) && (!(m = n & ~255) || m < MAXREC)) m = n;
		do
		{
			if ((n = read(fd, buf + siz - m, m)) <= 0) break;
			sp = (struct direct*)(buf + siz - m);
			while (sp < (struct direct*)(buf + siz - m + n))
			{
				if (sp->d_ino)
				{
					up->d_ino = sp->d_ino;
					s = sp->d_name;
					u = tmp;
					while (s < sp->d_name + sizeof(sp->d_name) && *s)
						*u++ = *s++;
					*u = 0;
					(void)strcpy(up->d_name, tmp);
					up->d_reclen = sizeof(struct dirent) - sizeof(up->d_name) + (
#if defined(d_fileno) || defined(d_ino)
						up->d_namlen =
#endif
						u - tmp) + 1;
					up->d_reclen = round(up->d_reclen, 8);
					up = (struct dirent*)((char*)up + up->d_reclen);
				}
				sp++;
			}
		} while (up == (struct dirent*)buf);
		return((char*)up - buf);
#else
		extern int		read();

		return(read(fd, buf, siz));
#endif
	}
#endif
}

#endif
