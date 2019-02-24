#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../pkg/ar.h"
#include "dist.h"

/* determine if we can create the given pathname.  not a generally useful
   function since it destroys the pathname in the process... */
static int
cancreate(char *path, int recurse)
{
	char *slash;

	for (;;) {
		slash = strrchr(path, '/');
		if (! slash)
			return 0;	/* supposed to be absolute */
		*slash = '\0';
		if (!*path)
			break;
		if (access(path, W_OK) == 0)
			return 1;
		if (!recurse)		/* for remove we check only one directory */
			return 0;
		if (access(path, F_OK) == 0)
			return 0;
	}
	return access("/", W_OK) == 0;
}
		
/*
 * try the instructions of the package on the given fd against the
 * permissions currently in the file system.  return 1 if we think we
 * ought to go ahead and try inspkg.
 */
int
tryperm(int fd)
{
	char armag[SARMAG];
	struct ar_hdr arhdr;
	long size;
	char temp[L_tmpnam];
	int tfd;
	FILE *tfp;
	int cmd, c, nf, retval;
	char *path;

	/* return 1 so inspkg will be called and barf for us */
	if (read(fd, armag, sizeof armag) != SARMAG || strncmp(armag, ARMAG, SARMAG))
		return 1;
	if (read(fd, (char *) &arhdr, sizeof arhdr) != sizeof arhdr)
		return 1;
	size = strtol(arhdr.ar_size, 0, 10);
	tmpnam(temp);
	close(creat(temp, 0600));	/* ape deficiency */
	tfd = open(temp, O_RDWR);
	if (tfd < 0) {
		eprintf("can't create temp file %s for tryperm", temp);
		remove(temp);		/* just in case something's *real* weird */
		exit(1);
	}
	if (fdcopy(tfd, fd, size, 0) != size) {
		close(tfd);
		remove(temp);
		eprintf("can't extract Instructions in tryperm");
		return 1;		/* inspkg will (presumably) barf for us */
	}
	lseek(tfd, 0L, 0);
	tfp = fdopen(tfd, "r");

	/* now that we have the instructions file, do the real meat.  */
	retval = 1;
	while ((cmd = getc(tfp)) != EOF) {
		switch (cmd) {
		case 'b':	/* b <mode> <dmaj> <dmin> <uid> <gid> <path> */
		case 'c':	/* c <mode> <dmaj> <dmin> <uid> <gid> <path> */
			nf = 6;
			break;
		case 'd':	/* d <mode> <uid> <gid> <path> */
		case 'f':	/* f <component> <uid> <gid> <path> */
			nf = 4;
			break;
		case 'l':	/* l <path1> <path2> */
		case 's':	/* s <path1> <path2> */
			nf = 2;
			break;
		case 'r':	/* r <path> */
		case 'x':	/* x <command> */
		case 'X':	/* X <path> */
			nf = 1;
			break;
		default:
			goto reterr;
		}

		/* get the last field of the line */
		while (nf--) {
			while ((c = getc(tfp)) != EOF)
				if (!isspace(c))
					break;
			if (c == EOF)
				goto reterr;
			ungetc(c, tfp);
			path = getpath(tfp);
		}

		/* eat the newline */
		if (getc(tfp) != '\n')
			goto reterr;

		switch (cmd) {
		case 'X':
		case 'x':
			break;

		case 'r':
			if (access(path, F_OK) != 0)
				break;
			if (cancreate(path, 0))
				break;
			retval = 0;
			goto out;

		default:
			if (cancreate(path, 1))
				break;
			retval = 0;
			goto out;
		}
	}

 out:
	fclose(tfp);
	remove(temp);
	return retval;

 reterr:
	/* there was some kind of bug in the format of the package.  return 1
	   and later on inspkg will generate an appropriate error message to
	   be returned to the remote system */
	fclose(tfp);	
	remove(temp);
	return 1;
}
