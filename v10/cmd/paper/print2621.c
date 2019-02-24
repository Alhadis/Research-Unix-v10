/*
 * paper command: filter to print on HP2621P without dropping
 *	 characters due to intervening layers of system buffering.
 *	 This version is (hopefully) independent of strap settings.
 *	 Operates by handshaking with terminal every N characters.
 */

#include <signal.h>
#include <sgtty.h>
#include <stdio.h>

#define N   25
#define ESC '\033'
#define DC1 '\021'

char buff[N+1];
char enqstat[]  = {ESC, '^', DC1, DC1};
char printon[]  = {ESC, '&', 'p', '1', '1', 'C', DC1, DC1, '\r', '\n'};
char printoff[] = {ESC, '&', 'p', '1', '3', 'C', DC1, DC1, '\r', '\n'};

char done = 0;
struct sgttyb oldtty, newtty;

interrupt ()
{
	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTERM, SIG_IGN);
	newtty.sg_flags |= RAW;
	stty (2, &newtty);
	write (2, printoff, sizeof printoff);
	sleep (3);
	stty (2, &oldtty);
	exit (1);
}

prepare ()
{
	register int n=0, c;
	static char saveesc=0;
	if (saveesc) {
		buff[n++] = ESC;
		saveesc = 0;
	}
	if (done)
		return n;
	while (n < N) {
		c = getchar();
		if (c == EOF) {
			done = 1;
			break;
		}
		if (c==ESC && n+4>=N) {
			saveesc = 1;
			break;
		}
		if (c == '\n')
			buff[n++] = '\r';
		buff[n++] = c;
	}
	return n;
}

await (c)
int c;
{
	static char got;
	while (1) {
		if (read(2, &got, 1) != 1)
			interrupt ();
		if (got == c)
			return;
	}
}

main (argc, argv)
int argc;
char **argv;
{
	register int n;
	gtty (2, &oldtty);
	signal (SIGINT, interrupt);
 	signal (SIGQUIT, interrupt);
	signal (SIGTERM, interrupt);
	newtty = oldtty;
	newtty.sg_flags = EVENP | ODDP | CBREAK;
	newtty.sg_flags |= n = oldtty.sg_flags & XTABS;
	stty (2, &newtty);
	write (2, printon, sizeof printon);

	n = 0;
	do {
		write (2, enqstat, sizeof enqstat);
		write (2, buff, n);
		n = prepare();
		await (ESC);
		await ('\r');
	} while (n != 0);

	write (2, printoff, sizeof printoff);
	write (2, enqstat, sizeof enqstat);
	await (ESC);
	await ('\r');
	stty (2, &oldtty);
}
