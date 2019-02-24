#include <sys/types.h>
#include <sys/stat.h>

#define	MSFS	3	/* mounted stream filesystem */

namept(fd, linkname, mode)
int fd; char *linkname; int mode;
{
	struct stat st;
	funmount(linkname);
	unlink(linkname);
	if (fd < 0)
		return 0;
	if (close(creat(linkname, mode)) < 0
	    || stat(linkname, &st) < 0
	    || fmount(MSFS, fd, linkname, 0) < 0) {
		unlink(linkname);
		return -1;
	}
	chmod(linkname, st.st_mode & 0777);
	return 0;
}
