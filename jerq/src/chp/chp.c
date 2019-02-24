#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/stream.h>
#include <stdio.h>
#include <ctype.h>
#include "struct.h"

#define N32LD "/usr/jerq/bin/32ld"

#if TEST
#define CHPTERM "/n/grigg/usr/ark/src/crunch/chpterm"
#else
#define CHPTERM "/usr/jerq/mbin/chp.m"
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
	char p[PTLEN];
	int n, evenfd, oddfd, s, pid;
	struct sgttyb tbuf;
	struct tchars tc;
	char **ep;

	crunchinit();

	/* Try to load the terminal program */
	switch (pid = fork()) {
	case 0:		/* child */
		execl (N32LD, "32ld", CHPTERM, (char *) 0);
		exit (1);
	case -1:	/* error */
		perror ("fork");
		exit (1);
	default:	/* parent */
		while ((s = wait (&n)) >= 0 && s != pid)
			;
		if (p < 0) {
			perror ("wait");
			exit (1);
		}
		if (n != 0) {
			fprintf (stderr, "32ld status %d\n", n);
			exit (1);
		}
	}

	/* locate an even/odd pt pair */
	oddfd = ptopen(p);
	if (oddfd < 0) {
		perror ("odd ptopen");
		exit (1);
	}

	ioctl (oddfd, FIOPUSHLD, &mesg_ld);

	/* set up the logical terminal for the child */
	evenfd = open (p, 2);
	if (evenfd < 0) {
		perror ("even ptopen");
		exit (1);
	}

	/* save physical terminal parameters */
	ioctl (0, TIOCGETP, &tbuf);
	ioctl (0, TIOCGETC, &tc);

	/* put the message line discipline on the physical terminal */
	ioctl (0, FIOPOPLD, 0);
	ioctl (0, FIOPUSHLD, &mesg_ld);

	switch (pid = fork()) {

	case -1:
		perror ("fork");
		exit (1);
	
	case 0:		/* child */
		ioctl (evenfd, FIOPUSHLD, &tty_ld);
		ioctl (evenfd, TIOCSETN, &tbuf);
		ioctl (evenfd, TIOCSETC, &tc);

		close (oddfd);
		ioctl (evenfd, TIOCSPGRP, (char *) NULL);
		close (0); close (1); close (2);
		dup (evenfd); dup (evenfd); dup (evenfd);
		close (evenfd);

		/* force the environment to indicate the right terminal */
		for (ep = environ; *ep; ep++) {
			if (strncmp (*ep, "TERM=", 5) == 0) {
				*ep = "TERM=2621";
				break;
			}
		}

		execl ("/bin/sh", "sh", (char *) NULL);
		exit (1);

	default:	/* parent */
		close (evenfd);
		do {
			struct fd_set readfd;
			int fdmask = 1 << oddfd;
			struct mesgb buf;

			readfd.fds_bits[0] = 1 | fdmask;

			select (NFD, &readfd, NULL, 100000);

			
			/* Keyboard input has priority over terminal output */
			if (readfd.fds_bits[0] & 1) {
				n = read (0, &buf, sizeof (buf));
				write (oddfd, &buf, n);
			} else if (readfd.fds_bits[0] & fdmask) {
				n = read (oddfd, &buf, sizeof (buf));
				while (buf.h.type == M_DATA) {
					emit (buf.buf, buf.h.losize | (buf.h.hisize << 8));
					readfd.fds_bits[0] = 1 | fdmask;
					select (NFD, &readfd, NULL, 0);
					if (readfd.fds_bits[0] != fdmask)
						break;
					n = read (oddfd, &buf, sizeof (buf));
				}
				
				emitcurr();
				flush();

				if (buf.h.type != M_DATA && buf.h.type != M_FLUSH)
					write (1, &buf, n);
			}
		} while (n > 0);

		/* clean up and exit */
		close (oddfd);
		ioctl (0, FIOPOPLD, 0);
		ioctl (0, FIOPUSHLD, &tty_ld);
		ioctl (0, TIOCSETN, &tbuf);
		ioctl (0, TIOCSETC, &tc);

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
