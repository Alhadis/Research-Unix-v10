#include <sys/types.h>
#include <signal.h>
#include <ipc.h>
#include <stdio.h>
#include "faces.h"

static fd_set cfds;
static int maxfd;
static char *pfile = "/usr/net/face/people.tab";
static char *mfile = "/usr/net/face/machine.tab";

main(argc, argv)
int argc;
char **argv;
{

	if (argc < 2) {
		fprintf(stderr, "usage: faces server-name ...\n");
		exit(1);
	}
	maketree(pfile, mfile);
	for (;;) {
		listen(argc-1, argv+1);
		log("network restart\n");
	}
}

catchalarm()
{
	signal(SIGALRM, catchalarm);
}

/*
 * the main loop, listening for calls and requests
 */

listen(nname, namelist)
int nname;
char **namelist;
{
	static int *namefd;
	fd_set rfds;
	register int i;
	ipcinfo *ip;

	if (namefd == NULL)
		namefd = (int *)emalloc(sizeof(int)*nname);
	for (i = 0; i < nname; i++)
		namefd[i] = -1;
	FD_ZERO(cfds);
	for (;;) {
		for (i = 0; i < nname; i++) {
			if (namefd[i] < 0) {
				namefd[i] = ipccreat(ipcpath("", "", namelist[i]), "");
				if (namefd[i] < 0)
					log("can't announce %s: %s\n", namelist[i], errstr);
				else {
					log("fd %d: announced %s\n", namefd[i], namelist[i]);
					FD_SET(namefd[i], cfds);
				}
			}
			if (namefd[i] >= 0 && namefd[i] > maxfd)
				maxfd = namefd[i];
		}
		rfds = cfds;
		if (select(maxfd+1, &rfds, (fd_set *)0, 60*1000) < 0) {
			log("select failed\n");
			for (i = 0; i < maxfd; i++)
				if (FD_ISSET(i, cfds))
					close(i);
			return;
		}
		for (i = 0; i < nname; i++) {
			if (namefd[i] < 0)
				continue;
			if (FD_ISSET(namefd[i], rfds)) {
				FD_CLR(namefd[i], rfds);
				if ((ip = ipclisten(namefd[i])) == NULL) {
					log("fd %d: %s: listen failed\n", namefd[i], errstr);
					FD_CLR(namefd[i], cfds);
					close(namefd[i]);
					namefd[i] = -1;
					continue;
				}
				newclient(ip);
			}
		}
		for (i = 0; i <= maxfd; i++)
			if (FD_ISSET(i, rfds) && FD_ISSET(i, cfds)) {
				signal(SIGALRM, catchalarm);
				alarm(60);
				if (dorequest(i) == 0)
					dropclient(i);
				alarm(0);
			}
		checknew();
	}
}

/* add client to the list */
newclient(ip)
register ipcinfo *ip;
{
	int fd;

	if ((fd = ipcaccept(ip)) < 0) {
		log("ipcaccept failed\n");
		return;
	}
	FD_SET(fd, cfds);
	if (fd > maxfd)
		maxfd = fd;
	log("fd %d: client %s\n", fd, ip->machine);
}

dropclient (fd)
int fd;
{
	FD_CLR(fd, cfds);
	close (fd);
	log("fd %d: off\n", fd);
}

/*
 * check for new data, rebuild tree if necessary
 */

#define	MINCHK	120	/* at most once in this many seconds */

checknew()
{
	static time_t lastcheck;
	time_t now;

	now = time((time_t *)0);
	if (now + MINCHK < lastcheck)
		return;
	if (newdata(pfile, mfile)) {
		log("new data, reinitializing\n");
		maketree(pfile, mfile);
		now = time((time_t *)0);
	}
	lastcheck = now;
}
