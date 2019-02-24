#include "decl.h"
#include <signal.h>
#include <errno.h>
#include <pwd.h>

extern "C" {
	extern int dup2 (int, int);
	extern int fork();
	extern int setupshares (int, void(*)() = 0);
	extern void perror (char*);
	extern int errno;
	extern int sys_nerr;
	extern char *sys_errlist[];
}

static const int bufsize = 4096;
static const int timeout = 60*10;
static const int wtimeout = 15;

static void
alarmcatch(int)
{
	signal (SIGALRM, alarmcatch);
}

static void
die (String s)
{
	String msg = s;
	if ((unsigned) errno < sys_nerr)
		msg += ":" + String(sys_errlist[errno]);
	msg += "\n";
	cout << msg;
	exit (1);
}

static void
twrite (char* buf, int len)
{
	alarm (timeout);
	int n = write (1, buf, len);
	alarm (0);
	if (n != len)
		die ("remote write failed");
}

static int
tread (char* buf, int len)
{
	alarm (timeout);
	int n = read (0, buf, len);
	alarm (0);
	if (n < 0)
		die ("remote read failed");
	return n;
}

static void
send (char* s)
{
	twrite (s, strlen (s));
}

static int
mkfile (String file)
{
	unlink (file);
	int fd = creat (file, 0600);
	if (fd < 0)
		die ("temp file creat");
	if (close (fd) < 0)
		die ("temp file close");
	fd = open (file, 2);
	if (fd < 0)
		die ("temp file reopen");
	if (unlink (file) < 0)
		die ("temp file unlink");
	return fd;
}

main()
{
	signal (SIGALRM, alarmcatch);

	/* announce our presence */
	send ("asd\n");

	/* use daemon's share instead of root */
	struct passwd *pw;
	pw = getpwnam ("daemon");
	if (pw)
		setupshares (pw->pw_uid);
	char buf[bufsize];
	int n;
	checksum cs, rcs;

	umask (077);

	/* make a temp file to hold the data */
	Path tfile = Path(tmpdir) & "asd" + String(dec(getpid()));
	int infd = mkfile (tfile);
	int outfd = mkfile (tfile);

	/* copy the data to the input temp file and checksum it */
	while ((n = tread (buf, bufsize)) > 0) {
		cs.combine (buf, n);
		if (write (infd, buf, n) != n)
			die ("temp file write");
	}
	lseek (infd, 0, 0);

	/* Check for normal end of data */
	if (n < 0)
		die ("remote read");

	/* Try to read the checksum */
	alarm (timeout);
	n = read (0, rcs);
	alarm (0);

	/* Do the checksums match? */
	if (n != cksize || cs != rcs) {
		cout << "checksum error\n";
		exit (1);
	}

	/* Assert we got the stuff successfully */
	twrite ("", 0);

	/* Run inspkg and put the results in the output temp file */
	int rc, status;
	int pid = fork();

	switch (pid) {
	case -1:
		die ("fork");

	case 0:			// child
		dup2 (infd, 0);
		dup2 (outfd, 1);
		close (infd);
		close (outfd);
		dup2 (1, 2);
		dup2 (1, 3);
		execl (inspkg, "inspkg", (char*) 0);
		die ("execl");

	default:		// parent
		do {
			alarm (wtimeout);
			rc = wait (&status);
			alarm (0);
			if (rc == -1) {
				if (errno == EINTR)
					twrite ("\n", 1);
				else
					die ("wait");
			}
		} while (rc != pid);
	}

	/* Send the output temp file to the other end */
	lseek (outfd, 0, 0);
	while ((n = read (outfd, buf, bufsize)) > 0)
		twrite (buf, n);

	/* Check for normal EOF */
	if (n < 0)
		die ("reading inspkg output");

	/* Report the inspkg return code */
	cout << "return code " << status << "\n";

	return 0;
}
