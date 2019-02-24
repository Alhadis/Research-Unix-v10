#include "asd.h"
#include <sys/wait.h>
#include <unistd.h>
#include "ftw.h"

#ifndef S_ISLNK
#ifdef S_IFLNK
#define S_ISLNK(M) (((M)&S_IFMT) == S_IFLNK)
#endif
#define S_ISREG(M) (((M)&S_IFMT) == S_IFREG)
#define S_ISDIR(M) (((M)&S_IFMT) == S_IFDIR)
#define S_ISCHR(M) (((M)&S_IFMT) == S_IFCHR)
#define S_ISBLK(M) (((M)&S_IFMT) == S_IFBLK)
#endif

/*
 *	mkd function -- tries to make a directory whose name is "d".
 *	returns 0 if successful or if d already exists and is a
 *	directory.  On failure, returns mkdir's return code.
 */

int
mkd (char *d)
{
	register int pid, w;
	int status;
	struct stat sb;

	if (stat (d, &sb) >= 0)
		return !S_ISDIR(sb.st_mode);

	switch (pid = fork()) {
	case 0:
		/* we might be executed from a setuid program */
		setgid (getegid());
		setuid (geteuid());
		execl ("/bin/mkdir", "mkdir", d, 0);
		/* No break */
	case -1:
		return 1;

	default:
		do w = wait (&status);
		while (w != pid && w > 0);
		if (w == pid)
			return status;
		return w;
	}
}


static int
rm (char *name, struct stat *sb, int type, struct FTW *ftw)
{
	register int r;

	switch (type) {

	case FTW_F:
	case FTW_SL:
		r = unlink (name);
		if (r < 0) {
			perror (name);
			return r;
		}
		break;
	
	case FTW_D:
		break;
	
	case FTW_DNR:
		fprintf (stderr, "cannot read directory %s\n", name);
		exit (1);
	
	case FTW_NS:
		fprintf (stderr, "cannot stat %s\n", name);
		exit (1);
	
	case FTW_DP:
		r = rmdir (name);
		if (r != 0) {
			fprintf (stderr,
			    "trouble removing directory %s\n", name);
			return r;
		}
	}

	return 0;
}

/*
 *	rmdir function -- tries to remove a directory whose name is "d".
 *	returns 0 if successful.  On failure, returns rmdir's return code.
 */

int
rmdir (char *d)
{
	register int pid, w;
	int status;
	struct stat sb;

	if (stat (d, &sb) >= 0 && !S_ISDIR(sb.st_mode))
		return -1;

	switch (pid = fork()) {
	case 0:
		/* we might be executed from a setuid program */
		setgid (getegid());
		setuid (geteuid());
		execl ("/bin/rmdir", "rmdir", d, 0);
		/* No break */
	case -1:
		return 1;

	default:
		do w = wait (&status);
		while (w != pid && w > 0);
		if (w == pid)
			return status;
		return w;
	}
}

/* rmall (s) recursively removes the object named s */
int
rmall (char *s)
{
	if (access(s, 0) < 0)
		return 0;
	return ftw (s, rm, 8);
}
