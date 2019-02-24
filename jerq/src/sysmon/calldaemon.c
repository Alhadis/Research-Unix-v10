#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <dkmgr.h>
#include "defs.h"

#define DEBUG if (FALSE)

/* imported */
extern int errno;
extern int dkp_ld;
extern char *sname();
extern char *strcpy();
extern unsigned int alarm();

/* global */
int rxpid;

/* come here if the "rx" times out */
static timeout()
{
	kill (rxpid, SIGKILL);
}

/* call up a daeon on a particular machine */
calldaemon (sysname, daemonname)
char *sysname;
char *daemonname;
{
	char *name;
	int fd, i;

	name = sname(sysname, daemonname);
	fd = tdkdial(name, 0);
	if (fd >= 0) {
		if (dkproto(fd, dkp_ld) < 0) {
			(void)close(fd);
			fd = -1;
		}
	}
	if (fd < 0) {
		/* try starting the daemon */
		switch (rxpid = fork()) {
		case -1:
			break;
		case 0:
			for (i = 0; i < 20; i++)
			    close (i);
			execlp("rx", "rx", sysname, "/usr/jerq/lib/sysmond", 0);
			_exit (0);
		default:
			(void)signal (SIGALRM, timeout);
			alarm (60);
			while ((i = wait(&i)) != rxpid && i >= 0)
			    ;
			alarm (0);
			(void)signal (SIGALRM, SIG_IGN);
			name = sname(sysname, daemonname);
			fd = tdkdial(name, 0);
			if (fd >= 0) {
				if (dkproto(fd, dkp_ld) < 0) {
					(void)close (fd);
					fd = -1;
				}
			}
			break;
		}
	}
	return (fd);
}

