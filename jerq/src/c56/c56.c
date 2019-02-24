#include <sys/types.h>
#include <sys/ttyio.h>
#include <sys/filio.h>
#include <sys/stream.h>
#include <stdio.h>
#include <ctype.h>
#include "struct.h"

#define N32LD "/usr/jerq/bin/32ld"

#if TEST
#define C56TERM "/n/grigg/usr/ark/src/crunch/c56term"
#else
#define C56TERM "/usr/jerq/mbin/c56.m"
#endif

#define PTLEN 30
#define NFD 20
#define numelts(a) (sizeof(a)/sizeof(*(a)))

/* number of output chars to write at a time -- must be a multiple of 3 */
#define OUTBSIZE 60

extern int tty_ld, mesg_ld;

struct mesgb {
	struct mesg h;
	char buf[128];
};

static struct {
	struct mesg h;
	char buf[OUTBSIZE];
} b;

static char *outp = b.buf;
static int outl = 0, parity = 0, byte = 0;
static nodep curr = ROOTNODE;

nodep prevnode;
extern char **environ;

main()
{
	int pfd[2];
	int n, s, pid;
	struct sgttyb tbuf;
	struct tchars tc;
	int ttysaved;
	fd_set readfd;
	static fd_set fdmask;

	crunchinit();

	/* Try to load the terminal program */
	switch (pid = fork()) {
	case 0:		/* child */
		execl (N32LD, "32ld", C56TERM, (char *) 0);
		exit (1);
	case -1:	/* error */
		perror ("fork");
		exit (1);
	default:	/* parent */
		while ((s = wait (&n)) >= 0 && s != pid)
			;
		if (s < 0) {
			perror ("wait");
			exit (1);
		}
		if (n != 0) {
			fprintf (stderr, "32ld status %d\n", n);
			exit (1);
		}
	}

	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}
	ioctl(pfd[1], FIOPUSHLD, &mesg_ld);

	/* save physical terminal parameters; put mesg discipline there */
	ttysaved = 0;
	ioctl (0, TIOCGETP, &tbuf);
	ioctl (0, TIOCGETC, &tc);
	if (ioctl(0, FIOLOOKLD, (char *)0) == tty_ld) {
		ttysaved = 1;
		ioctl (0, FIOPOPLD, (char *)0);		/* discard ttyld */
	}
	ioctl (0, FIOPUSHLD, &mesg_ld);

	switch (pid = fork()) {

	case -1:
		perror ("fork");
		exit (1);
	
	case 0:		/* child */
		ioctl (pfd[0], FIOPUSHLD, &tty_ld);
		ioctl (pfd[0], TIOCSETN, &tbuf);
		ioctl (pfd[0], TIOCSETC, &tc);

		close (pfd[1]);
		ioctl (pfd[0], TIOCSPGRP, (char *) NULL);
		for (n = 0; n < NSYSFILE; n++) {
			close (n);
			dup (pfd[0]);
		}
		close (pfd[0]);

		execl ("/bin/sh", "sh", (char *) NULL);
		exit (1);

	default:	/* parent */
		close (pfd[0]);
		FD_SET(0, fdmask);
		FD_SET(pfd[1], fdmask);
		do {
			struct mesgb buf;

			readfd = fdmask;
			select (NFD, &readfd, NULL, 100000);

			/* Keyboard input has priority over terminal output */
			if (FD_ISSET(0, readfd)) {
				n = read (0, &buf, sizeof (buf));
				write (pfd[1], &buf, n);
			} else if (FD_ISSET(pfd[1], readfd)) {
				n = read (pfd[1], &buf, sizeof (buf));
				while (buf.h.type == M_DATA) {
					emit (buf.buf, buf.h.losize | buf.h.hisize << 8);
					readfd = fdmask;
					select (NFD, &readfd, NULL, 0);
					if (FD_ISSET(0, readfd))
						break;
					n = read (pfd[1], &buf, sizeof (buf));
				}
				
				emitcurr();
				flush();

				if (buf.h.type != M_DATA && buf.h.type != M_FLUSH)
					write (1, &buf, n);
			}
		} while (n > 0);

		/* clean up and exit */
		close (pfd[1]);
		ioctl (0, FIOPOPLD, 0);		/* mesg_ld */
		if (ttysaved) {
			ioctl (0, FIOPUSHLD, &tty_ld);
			ioctl (0, TIOCSETN, &tbuf);
			ioctl (0, TIOCSETC, &tc);
		}

		if (n < 0)
			perror ("read loop");
	}
}

/* Compress a string and send it to the terminal */
emit (buf, len)
	char *buf;
	int len;
{

	register char *in = buf, *lim = in + len;

	while (in < lim) {
		register nodep next;

		setchild (next, curr, *in);

		if (next != NULLNODE) {
			curr = next;
			in++;
		} else
			emitcurr();
	}
}

/* write the current node and update the cache */
emitcurr()
{
	if (curr != ROOTNODE) {
		if (parity ^= 1) {
			*outp++ = curr;
			outl++;
			byte = curr>>4 & 0xf0;
		} else {
			*outp++ = byte | curr>>8 & 0xf;
			*outp++ = curr;
			outl += 2;
			if (outl >= OUTBSIZE)
				flush();
		}

		extend (prevnode, curr);
		prevnode = curr;
		curr = ROOTNODE;
	}
}

flush()
{
	if (outl) {

		/* force out a dummy token if necessary */
		if (parity) {
			*outp++ = byte;
			*outp++ = 0;
			outl += 2;
			parity = 0;
		}

		b.h.type = M_DATA;
		b.h.losize = outl & 0xff;
		b.h.hisize = outl >> 8;
		b.h.magic = MSGMAGIC;
		write (1, &b, outl + sizeof (b.h));
		outl = 0;
		outp = b.buf;
	}
}
