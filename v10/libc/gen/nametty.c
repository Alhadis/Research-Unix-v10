#include <sys/types.h>
#include <sys/stat.h>
/*
 * give a name to a tty
 * name must be an extant file
 */

#define	MSFS	3	/* mounted stream filesystem */

nametty(fd, name)
int fd;
char *name;
{
	struct stat st;

	if (fmount(MSFS, fd, name, 0) < 0)
		return (0);
	/*
	 * make fake pipe mode match mounted fd;
	 * silly annoyance
	 */
	if (fstat(fd, &st) >= 0)
		chmod(name, st.st_mode & 0777);
	return (1);
}
