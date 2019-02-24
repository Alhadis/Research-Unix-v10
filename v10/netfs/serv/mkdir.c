/*
 * mkdir, rmdir
 * for systems without the system calls
 */
#include <errno.h>
#include <rf.h>

mkdir(s, mode)
char *s;
int mode;
{
	int npid, pid, sts;

	if ((pid = fork()) < 0) {
		rflog("can't mkdir %s: fork failed: %d\n", s, errno);
		errno = ENOTDIR;	/* or something */
		return (-1);
	}
	if (pid == 0) {		/* child */
		setgid(rfgid);
		setuid(rfuid);
		close(0);
		close(1);
		close(2);
		open("/dev/null", 2);
		dup(0);
		dup(0);
		execl("/bin/mkdir", "mkdir", s, (char *)0);
		execl("/usr/bin/mkdir", "mkdir", s, (char *)0);
		rflog("can't mkdir %s: can't exec mkdir\n", s);
		_exit(127);
	}
	while ((npid = wait(&sts)) != pid)
		if (npid < 0) {
			rflog("can't mkdir %s: bad wait: %d\n", s, errno);
			errno = ENOTDIR;
			return (-1);
		}
	if (sts) {
		errno = EPERM;		/* probably */
		return (-1);
	}
	if (chmod(s, mode) < 0) {
		rmdir(s);		/* what if it fails? */
		return (-1);
	}
	return (0);
}

rmdir(s)
char *s;
{
	int npid, pid, sts;

	if ((pid = fork()) < 0) {
		rflog("can't rmdir %s: fork failed: %d\n", s, errno);
		errno = ENOTDIR;	/* or something */
		return (-1);
	}
	if (pid == 0) {		/* child */
		setgid(rfgid);
		setuid(rfuid);
		close(0);
		close(1);
		close(2);
		open("/dev/null", 2);
		dup(0);
		dup(0);
		execl("/bin/rmdir", "rmdir", s, (char *)0);
		execl("/usr/bin/rmdir", "rmdir", s, (char *)0);
		rflog("can't rmdir %s: can't exec rmdir\n", s);
		_exit(127);
	}
	while ((npid = wait(&sts)) != pid)
		if (npid < 0) {
			rflog("can't rmdir %s: bad wait: %d\n", s, errno);
			errno = ENOTDIR;
			return (-1);
		}
	if (sts) {
		errno = EPERM;		/* probably */
		return (-1);
	}
	return (0);
}
