/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * touch file access and modify times of file
 * if force>0 then file will be created if it doesn't exist
 * if force<0 then times are taken verbatim
 * times have one second granularity
 *
 *	(time_t)(-1)	retain old time
 *	0		use current time
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "FEATURE/utime"

#if _hdr_utime && _lib_utime
#include <utime.h>
#endif

extern int	errno;

int
touch(file, atime, mtime, force)
char*	file;
time_t	atime;
time_t	mtime;
int	force;
{
	int		n;
	int		fd;
	int		oerrno = errno;
#if _lib_utime
	time_t		now;
	struct stat	st;
#if _hdr_utime
	struct utimbuf	ut;
#else
	time_t		ut[2];
#endif

	extern int	close();
	extern int	creat();
	extern int	stat();
	extern time_t	time();
	extern int	utime();

	if (force >= 0)
	{
		if (atime == (time_t)(-1) || mtime == (time_t)(-1))
		{
			if (stat(file, &st)) st.st_atime = st.st_mtime = 0;
			if (atime == (time_t)(-1)) atime = st.st_atime;
			if (mtime == (time_t)(-1)) mtime = st.st_mtime;
		}
		if (!atime || !mtime)
#if _hdr_utime
		if (atime || mtime)
#endif
		{
			(void)time(&now);
			if (!atime) atime = now;
			if (!mtime) mtime = now;
		}
	}
#if _hdr_utime
	ut.actime = atime;
	ut.modtime = mtime;
	n = utime(file, (force < 0 || atime || mtime) ? &ut : (struct utimbuf*)0);
#else
	ut[0] = atime;
	ut[1] = mtime;
	n = utime(file, ut);
#endif
	if (n)
#else
	if (mtime)
	{
		/*
		 * NOTE: the emulation allows atime to change
		 *	 for mtime only requests
		 */

		errno = EINVAL;
		return(-1);
	}
#endif
	{
#if _lib_utime
		if (errno == ENOENT)
#else

		extern int	access();

		if (access(file, 0))
#endif
		{
			if (!force || (fd = creat(file, 0666)) < 0) return(-1);
			close(fd);
			errno = oerrno;
#if _lib_utime
#if _hdr_utime
			return((force < 0 || atime || mtime) ? utime(file, &ut) : 0);
#else
			return((atime != now || mtime != now) ? utime(file, ut) : 0);
#endif
#else
			return(0);
#endif
		}
#if !_hdr_utime || !_lib_utime
#if _lib_utime
		if (atime == now && mtime == now && (fd = open(file, 2)) >= 0)
#else
		if ((fd = open(file, 2)) >= 0)
#endif
		{
			char	c;

			extern long	lseek();
			extern int	open();
			extern int	read();
			extern int	write();

			if (read(fd, &c, 1) == 1)
			{
				if (lseek(fd, 0L, 0) == 0L && write(fd, &c, 1) == 1)
				{
					errno = oerrno;
					n = 0;
				}
				close(fd);
			}
			else
			{
				close(fd);
				if ((fd = creat(file, 0666)) >= 0)
				{
					close(fd);
					errno = oerrno;
					n = 0;
				}
			}
		}
#endif
	}
	return(n);
}
