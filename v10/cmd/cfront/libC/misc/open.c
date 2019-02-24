// The following code tries to simulate the mode argument
// of the System V open().
// Supports only O_RDONLY O_WRONLY O_RDWR O_CREAT O_TRUNC O_EXCL

#include <errno.h>
#include <osfcn.h>

// The following defines are from System V release 2.

#define	O_RDONLY 0
#define	O_WRONLY 1
#define	O_RDWR	 2
#define	O_NDELAY 04	/* Non-blocking I/O */
#define	O_APPEND 010	/* append (writes guaranteed at the end) */
#define O_SYNC	 020	/* synchronous write option */

/* Flag values accessible only to open(2) */
#define	O_CREAT	00400	/* open with file create (uses third open arg)*/
#define	O_TRUNC	01000	/* open with truncation */
#define	O_EXCL	02000	/* exclusive open */

int
open(const char* path, int kind, int mode)
{
	// guard against arguments we cannot handle
	if (kind & ~(O_RDONLY|O_WRONLY|O_RDWR|O_CREAT|O_TRUNC|O_EXCL)) {
		errno = EINVAL;
		return -1;
	}

	// If O_CREAT and O_EXCL both set and the file exists, fail.
	if ((kind & (O_CREAT|O_EXCL)) == (O_CREAT|O_EXCL)
	    && access(path, 0) >= 0) {
		errno = EEXIST;
		return -1;
	}

	register int fd;
	register int io = kind & (O_RDONLY|O_WRONLY|O_RDWR);

	switch (io) {
	case O_RDONLY:
	case O_RDWR:
		fd = open(path, io);
		if (fd >= 0) {
			if (kind & O_TRUNC)
				close(creat(path, 0));
		} else if (kind & O_CREAT) {
			int save = umask(0);
			if ((fd = creat(path, 0600)) >= 0) {
				close(fd);
				if ((fd = open(path, io)) >= 0)
					chmod(path, mode & ~save);
			}
			umask(save);
		}
		break;

	case O_WRONLY:
		if (kind & O_TRUNC)
			fd = creat(path, mode);
		else {
			fd = open(path, io);
			if (fd < 0 && (kind & O_CREAT))
				fd = creat(path, mode);
		}
		break;

	default:
		errno = EINVAL;
		fd = -1;
	}
	return fd;
}
