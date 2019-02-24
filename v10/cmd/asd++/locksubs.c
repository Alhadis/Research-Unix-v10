#include "decl.h"
#include <signal.h>

static lockcount = 0;
static SIG_TYP intsav, quitsav, hupsav, pipesave, termsav;

void
lock (Path& dir)
{
	Path com = dir & "L." + String(dec(getpid()));
	int snooze = 0;

	if (lockcount++ == 0) {
		intsav = signal (SIGINT, SIG_IGN);
		quitsav = signal (SIGQUIT, SIG_IGN);
		hupsav = signal (SIGHUP, SIG_IGN);
		pipesave = signal (SIGPIPE, SIG_IGN);
		termsav = signal (SIGTERM, SIG_IGN);
	}

	int fd;
	if ((fd = creat (com, 0)) < 0) {
		cout << "cannot create lock file " << com << "\n";
		exit (1);
	}
	close (fd);

	while (link (com, dir & "lock") < 0)
		sleep (snooze++);
}

void
unlock (Path& dir)
{
	unlink (dir & "L." + String(dec(getpid())));
	unlink (dir & "lock");

	if (--lockcount == 0) {
		signal (SIGINT, intsav);
		signal (SIGQUIT, quitsav);
		signal (SIGHUP, hupsav);
		signal (SIGPIPE, pipesave);
		signal (SIGTERM, termsav);
	}
}
