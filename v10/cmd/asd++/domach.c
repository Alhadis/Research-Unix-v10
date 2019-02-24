/*
 *	domach - install a file on a (single) machine.
 *
 *	Output from the installation is appended to the
 *	"output" list.  The result is a null string on success,
 *	non-null on failure.
 */

#include "decl.h"
#include <signal.h>
#include <errno.h>
#include <ipc.h>

static const int bufsize = 4096;
static const int timeout = 60*10;

static void
alarmcatch (int)
{
	signal (SIGALRM, alarmcatch);
}

static void
pipecatch (int)
{
	signal (SIGPIPE, pipecatch);
}

static int
twrite (int fd, char* buf, int n)
{
	SIG_TYP pipesave = signal (SIGPIPE, pipecatch);
	alarm (timeout);
	int r = write (fd, buf, n);
	alarm (0);
	signal (SIGPIPE, pipesave);
	return r;
}

static int
tread (int fd, char* buf, int n)
{
	alarm (timeout);
	int r = read (fd, buf, n);
	alarm (0);
	return r;
}

String
domach (String machname, Path file, String_list& output)
{
	checksum cs;
	signal (SIGALRM, alarmcatch);

	/* Open the input file */
	int infd = open (file, 0);
	if (infd < 0) {
		output += "cannot open package";
		return "";
	}

	/* Establish a connection to the remote machine */
	errno = 0;
	int remfd = ipcopen(ipcpath(charstr(machname),"dk","asd"), "heavy delim");
	if (remfd < 0) {
		close (infd);

		switch (errno) {

		/* permanent errors */
		case ENOENT:		/* illegal destination name */
		case EACCES:		/* permission denied */
			if (errstr)
				output += errstr;
			return "";

		default:
			if (errstr)
				return errstr;
			return "cannot connect";

		case 0:
			;
		}
	}

	char buf[bufsize];
	int n, w;

	/* verify that we are talking to a dkinstall process */
	n = tread (remfd, buf, bufsize);
	if (n != 4 || strncmp (buf, "asd\n", 4) != 0) {
		close (infd);
		close (remfd);
		output += "cannot ship to this destination";
		output += dec(n) + (": " + String(buf,n>0?n-1:0));
		return "";
	}

	/* copy the file data to the remote system */
	while ((n = read (infd, buf, bufsize)) > 0) {
		cs.combine (buf, n);
		w = twrite (remfd, buf, n);
		if (w != n) {
			close (infd);
			close (remfd);
			return "error while writing to remote";
		}
	}

	close (infd);
	if (n != 0) {
		close (remfd);
		return "input error";
	}

	/* Indicate end of file */
	if (twrite (remfd, "", 0) != 0) {
		close (remfd);
		return "cannot write eof";
	}

	/* Send the checksum */
	SIG_TYP pipesave = signal (SIGPIPE, pipecatch);
	alarm (timeout);
	n = write (remfd, cs);
	alarm (0);
	signal (SIGPIPE, pipesave);
	if (n != cksize) {
		close (remfd);
		return "cannot send checksum";
	}

	/*
	 * Read the acknowledgment.  If the acknowledgment begins
	 * with a null record, it indicates success or permanent
	 * failure.  Otherwise it is a temporary failure.
	 */

	if ((n = tread (remfd, buf, bufsize)) != 0) {
		close (remfd);
		if (n < 0)
			return "error reading ack";
		String msg (buf, n);
		if ((n = msg.strchr('\n')) >= 0)
			msg = msg (0, n);
		return msg;
	}

	/*
	 * We got here only if we saw a null record from the remote.
	 *
	 * Read and handle the stuff coming in from the remote.
	 * Discard leading null lines, which are probably
	 * keep-alive messages but are good to remove even
	 * even if they aren't.
	 */
	String remnant;
	int gotdata = 0;
	while ((n = tread (remfd, buf, bufsize)) > 0) {
		register char *p = buf;
		char *lastp = p, *lim = p + n;
		while (p < lim) {
			if (*p == '\n') {
				String line = remnant +
					String (lastp, p - lastp);
				if (gotdata || line.length() != 0) {
					output += line;
					gotdata = 1;
				}
				remnant = "";
				lastp = p + 1;
			}
			p++;
		}
		remnant += String (lastp, p - lastp);
	}

	if (remnant.length() != 0)
		output += remnant;

	/* remove trailing normal status message */
	String s;
	if (output.lastX (s)) {
		if (s == "return code 0")
			output.unput();
	}

	close (remfd);

	if (n < 0)
		return "error reading return status";

	return "";
}
