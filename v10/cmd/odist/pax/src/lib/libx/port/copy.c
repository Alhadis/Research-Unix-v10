/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * copy from rfd to wfd (with conditional mmap hacks)
 */

#if sun

#include "FEATURE/mmap"

#ifdef _lib_mmap

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MAPSIZE		(1024*256)

extern int		munmap();
extern caddr_t		mmap();

#endif

#endif

#define round(x,y)	(((x)+(y)-1)&~((y)-1))

#define BUFSIZ		4096

/*
 * copy n bytes from rfd to wfd
 * actual byte count returned
 * if n<=0 then ``good'' size is used
 */

long
copy(rfd, wfd, n)
int	rfd;
int	wfd;
int	n;
{
	register long	c;
#ifdef MAPSIZE
	off_t		pos;
	off_t		mapsize;
	char*		mapbuf;
	struct stat	st;

	extern int		fstat();
	extern off_t		lseek();
#endif

	static int		bufsiz;
	static char*		buf;

	extern char*		malloc();

	if (n <= 0 || n >= BUFSIZ * 2)
	{
#if MAPSIZE
		if (!fstat(rfd, &st) && (st.st_mode & S_IFMT) == S_IFREG && (pos = lseek(rfd, (off_t)0, 1)) != ((off_t)-1))
		{
			if (pos >= st.st_size) return(0);
			mapsize = st.st_size - pos;
			if (mapsize > MAPSIZE) mapsize = (mapsize > n && n > 0) ? n : MAPSIZE;
			if (mapsize >= BUFSIZ * 2 && (mapbuf = mmap((caddr_t)0, mapsize, PROT_READ, MAP_SHARED, rfd, pos)) != ((caddr_t)-1))
			{
				if (write(wfd, mapbuf, mapsize) != mapsize || lseek(rfd, mapsize, 1) == ((off_t)-1)) return(-1);
				(void)munmap(mapbuf, mapsize);
				return(mapsize);
			}
		}
#endif
		if (n <= 0) n = BUFSIZ;
	}
	if (n > bufsiz)
	{
		if (buf) free(buf);
		bufsiz = round(n, BUFSIZ);
		if (!(buf = malloc(bufsiz))) return(-1);
	}
	if ((c = read(rfd, buf, n)) > 0 && write(wfd, buf, c) != c) c = -1;
	return(c);
}
