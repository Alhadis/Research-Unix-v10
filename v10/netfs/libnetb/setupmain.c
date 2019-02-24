/*
 * main program for server called by setup
 */
#include <stdio.h>
#include <signal.h>
#include "rf.h"

#define	CFD	0	/* filesystem connection */

int fserrno;

/*
 * setup handshake:
 * receive 16-byte message with initial parameters
 * send 1
 * receive maxsize bytes: uid map
 *	user : means list is incomplete;
 *	send 012 and read another maxsize bytes
 * send 2
 * receive maxsize bytes: gid map
 *	group : means list is incomplete;
 *	send 013 and read another maxsize bytes
 * send 3
 */
#define	MSG1LEN	16

extern char *_rfwhocalled();
char *malloc();

char *rfclient;
int rfdebug;

main(argc, argv)
int argc;
char **argv;
{
	char *inbuf;
	char msg[MSG1LEN];
	int n;
	int maxsize;

	umask(0);
	signal(SIGHUP, SIG_IGN);
	if ((rfclient = _rfwhocalled(CFD)) == NULL)
		rfclient = "";
	/*
	 * setup message 1
	 */
	if ((n = _rfgread(CFD, msg, MSG1LEN, MSG1LEN)) != MSG1LEN)
		rfpanic("setup 1: %d\n", n);
	maxsize = msg[0] * 1024;
	rfdebug = msg[3];
	if (_rfinit(argc, argv) == 0)
		rfpanic("can't init\n");
	if ((inbuf = malloc(maxsize)) == NULL)
		rfpanic("can't alloc %d bytes\n", maxsize);
	if (write(CFD, "\1", 1) != 1)
		rfpanic("setup 1: write error\n");
	/*
	 * setup message 2
	 */
	for (;;) {
		if ((n = _rfgread(CFD, inbuf, maxsize, maxsize)) != maxsize)
			rfpanic("setup 2: %d\n", n);
		inbuf[n-1] = 0;	/* safety */
		if (_rfmapuid(inbuf) != 0)
			break;
		if (write(CFD, "\012", 1) != 1)
			rfpanic("setup 2 (12): write error\n");
	}
	if (write(CFD, "\2", 1) != 1)
		rfpanic("setup 2: write error\n");
	/*
	 * setup message 3
	 */
	for (;;) {
		if ((n = _rfgread(CFD, inbuf, maxsize, maxsize)) != maxsize)
			rfpanic("setup 3: %d\n", n);
		inbuf[n-1] = 0;	/* safety */
		if (_rfmapgid(inbuf) != 0)
			break;
		if (write(CFD, "\013", 1) != 1)
			rfpanic("setup 3 (13): write error\n");
	}
	if (write(CFD, "\3", 1) != 1)
		rfpanic("setup 3: write error\n");
	rflog("ok; buf %d\n", maxsize);
	_rfscan(CFD, inbuf, maxsize);
	rflog("eof\n");
	exit(0);
}
