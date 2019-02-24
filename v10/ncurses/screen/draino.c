/*
 * Code for various kinds of delays.  Most of this is nonportable and
 * requires various enhancements to the operating system, so it won't
 * work on all systems.  It is included in curses to provide a portable
 * interface, and so curses itself can use it for function keys.
 */

/*	@(#) draino.c: 1.1 10/15/83	(1.10	3/6/83)	*/

#include "curses.ext"
#include <signal.h>

#define NAPINTERVAL 100
#define HZ 60

/* From early specs - this may change by 4.2BSD */
struct _timeval {
	long tv_sec;
	long tv_usec;
};

/*
 * Wait until the output has drained enough that it will only take
 * ms more milliseconds to drain completely.
 * Needs Berkeley TIOCOUTQ ioctl.  Returns ERR if impossible.
 */
int
draino(ms)
int ms;
{
	int ncthere;	/* number of chars actually in output queue */
	int ncneeded;	/* number of chars = that many ms */
	int rv;		/* ioctl return value */

#ifdef TIOCOUTQ
# define _DRAINO
	/* 10 bits/char, 1000 ms/sec, baudrate in bits/sec */
	ncneeded = baudrate() * ms / (10 * 1000);
	for (;;) {
		ncthere = 0;
		rv = ioctl(cur_term->Filedes, TIOCOUTQ, &ncthere);
#ifdef DEBUG
		fprintf(outf, "draino: rv %d, ncneeded %d, ncthere %d\n",
			rv, ncneeded, ncthere);
#endif
		if (rv < 0)
			return ERR;	/* ioctl didn't work */
		if (ncthere <= ncneeded) {
			return 0;
		}
		napms(NAPINTERVAL);
	}
#endif

#ifdef TCSETAW
# define _DRAINO
	/*
	 * USG simulation - waits until the entire queue is empty,
	 * then sets the state to what it already is (e.g. no-op).
	 * Unfortunately this only works if ms is zero.
	 */
	if (ms <= 0) {
		ioctl(cur_term->Filedes, TCSETAW, cur_term->Nttyb);
		return OK;
	}
#endif

#ifndef _DRAINO
	/* No way to fake it, so we return failure. */
	/* Used #else to avoid warning from compiler about unreached stmt */
	return ERR;
#endif
}
