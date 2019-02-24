#include "asd.h"
#include "ftw.h"

int rm();

/*
 *	mkdir function -- tries to make a directory whose name is "d".
 *	returns 0 if successful or if d already exists and is a
 *	directory.  On failure, returns mkdir's return code.
 */

int
mkdir (d)
	register char *d;
{
	register int pid, w;
	int status;
	struct stat sb;

	if (stat (d, &sb) >= 0)
		return (sb.st_mode & S_IFMT) != S_IFDIR;

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

/*
 *	rmdir function -- tries to remove a directory whose name is "d".
 *	returns 0 if successful.  On failure, returns rmdir's return code.
 */

int
rmdir (d)
	register char *d;
{
	register int pid, w;
	int status;
	struct stat sb;

	if (stat (d, &sb) >= 0 && (sb.st_mode & S_IFMT) != S_IFDIR)
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
rmall (s)
	register char *s;
{
	return ftw (s, rm, 8);
}

/* ARGSUSED */
static int
rm (name, sb, type)
	register char *name;
	register struct stat *sb;
	register int type;
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
