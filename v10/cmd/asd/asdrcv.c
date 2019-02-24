/*
 *	asdrcv - receive sealed packages
 *
 *	This program will normally be installed as /etc/asd/asdrcv
 *	and will be run as root (!).  It is invoked as follows:
 *
 *		/etc/asdrcv machine uid
 *
 *	where the standard input is a sealed package.  First it
 *	verifies that machine and uid are valid: they must consist
 *	entirely of letters and digits and may not be empty.
 *	Next we look at the file /etc/asd/perm/machine/uid.
 *	If this file doesn't exist, reject.  If it does exist,
 *	it represents the uid and gid on the receiving machine
 *	to use.  The default is the uid/gid under which asdrcv
 *	invoked, usually (root, other).
 *
 *
 *	We next arrange to send all stderr output to machine!uid.
 *	Finally, operating as the appropriate user, we create the
 *	following pipeline:
 *
 *		unseal -K /etc/asd/keys/machine/uid | inspkg -v
 */

#include "asd.h"

static char *keybase = "/etc/asd/keys";
static char *permbase = "/etc/asd/perm";
static char *buildname();

main (argc, argv)
	int argc;
	char **argv;
{
	register int i;
	char *permfile, *keyfile, *user;
	register FILE *permfd;
	int pipefd[2];

	/* check for exactly two arguments */
	if (argc != 2 + 1) {
		fprintf (stderr, "asdrcv: arg count\n");
		exit (1);
	}

	/*
	 * each argument must be nonempty,
	 * begin with a letter or digit, and
	 * contain only letters, digits, '.', or '-'
	 */
	for (i = 1; i < argc; i++) {
		register char *p = argv[i];
		if (*p == '\0') {
			fprintf (stderr, "asdrcv: null argument\n");
			exit (1);
		}

		if (*p == '.' || *p == '-') {
			fprintf (stderr, "asdrcv: invalid argument\n");
			exit (1);
		}

		do {
			register c = *p++;
			if (!isalpha (c) && !isdigit (c) &&
			    c != '.' && c != '-') {
				fprintf (stderr, "asdrcv: invalid argument\n");
				exit (1);
			}
		} while (*p != '\0');
	}

	/* develop the names of the permission file and keyfile */
	permfile = buildname (permbase, argv[1], argv[2]);
	keyfile = buildname (keybase, argv[1], argv[2]);

	/* now try to read the permission file */
	permfd = fopen (permfile, "r");
	if (permfd == NULL) {
		perror (permfile);
		exit (1);
	}

	/* the optional line represents a uid/gid pair */
	i = getc (permfd);
	if (i != EOF) {
		int uid, gid;
		ungetc (i, permfd);
		uid = numuid (getfield (permfd));
		gid = numgid (getfield (permfd));
		setgid (gid);
		setuid (uid);
	}

	/* we're done with the file */
	fclose (permfd);

	/* build the stderr pipe */
	if (pipe (pipefd) < 0) {
		perror ("stderr pipe");
		exit (1);
	}

	/* fork off a mail process */
	switch (fork()) {

	case 0:			/* child */
		close (pipefd[1]);		/* close the write end */
		dup2 (pipefd[0], 0);		/* stdin := read end */
		if (pipefd[0] != 0)
		close (pipefd[0]);		/* close the read end */

		/* make a string for machine!user */
		user = alloc (strlen (argv[1]) + strlen (argv[2]) + 2);
		strcpy (user, argv[1]);
		strcat (user, "!");
		strcat (user, argv[2]);

		/* send mail */
		execl ("/bin/mail", "mail", user, (char *) 0);
		execl ("/usr/bin/mail", "mail", user, (char *) 0);
		exit (1);
	
	case -1:
		perror ("stderr fork");
		exit (1);
	
	default:		/* parent */
		close (pipefd[0]);		/* close the read end */
		dup2 (pipefd[1], 2);		/* stderr := write end */
		dup2 (pipefd[1], 1);		/* stdout := write end also */
		close (pipefd[1]);		/* close the write end */
	}

	/* build the next pipe */
	if (pipe (pipefd) < 0) {
		perror ("pipe");
		exit (1);
	}

	/* create the processes */
	fflush (stdout);
	switch (fork()) {

	case -1:			/* error */
		perror ("fork");
		exit (1);

	case 0:				/* child */
		close (pipefd[0]);
		if (dup2 (pipefd[1], 1) < 0) {
			perror ("stdout dup");
			exit (1);
		}
		close (pipefd[1]);
		execl ("/bin/unseal", "unseal", "-K", keyfile, (char *) 0);
		execl ("/usr/bin/unseal", "unseal", "-K", keyfile, (char *) 0);
		fprintf (stderr, "asdrcv: cannot exec unseal\n");
		exit (1);
	
	default:			/* parent */
		close (pipefd[1]);
		if (dup2 (pipefd[0], 0) < 0) {
			perror ("stdin dup");
			exit (1);
		}
		close (pipefd[0]);
		execl ("/bin/inspkg", "inspkg", "-v", (char *) 0);
		execl ("/usr/bin/inspkg", "inspkg", "-v", (char *) 0);
		fprintf (stderr, "asdrcv: cannot exec inspkg\n");
		exit (1);
	}
}

/* return a name of the form dir/comp1/comp2 */
static char *
buildname (dir, comp1, comp2)
	char *dir, *comp1, *comp2;
{
	register char *r;

	r = alloc (
		strlen (dir) +		/* name of directory */
		strlen (comp1) +	/* first component */
		strlen (comp2) +	/* second component */
		3			/* two slashes and ending null */
	    );

	strcpy (r, dir);
	strcat (r, "/"); strcat (r, comp1);
	strcat (r, "/"); strcat (r, comp2);

	return r;
}
