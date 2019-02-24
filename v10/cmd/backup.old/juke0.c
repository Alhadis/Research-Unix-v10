#include	<fio.h>
#include	<string.h>
#include	<signal.h>
#include	"backup.h"
#include	<ipc.h>
#include	<errno.h>
#include	"juke.h"

char me[32];

#define		ALARM		30

sigalarm()
{
	logprint("+%s bored; exiting\n", me);
	exit(0);
}

main(argc, argv)
	char **argv;
{
	ipcinfo *ip;
	Server *s, *ss;
	int nfd;
	int mountfd;
	char buf[256];
	int maxfds;
	fd_set fds;
	int fpipe[2];

	signal(SIGHUP, SIG_IGN);
	for(nfd = 1; nfd < 128; nfd++)
		close(nfd);
	chdir("/tmp");
	sprint(me, "worm");
	volmapinit();
	logprint("+%s starting\n", me);
	serverinit(argv+1);
	sprint(buf, "/cs/%s", me);
	mountfd = ipccreat(buf, 0);
	if(mountfd < 0){
		write(0, "Error", 1);
		logprint("can't announce %s: %s\n", buf, errstr);
		exit(1);
	}
logprint("here\n");
	write(0, "Okay", 1);
	close(0);
	signal(SIGPIPE, SIG_IGN);
logprint("there\n");
	FD_ZERO(fds);
	FD_SET(mountfd, fds);
	maxfds = mountfd+1;
	for(;;){
		if(select(maxfds, &fds, (fd_set *)0, 1000*(ALARM+1)) < 0){
			logprint("+%s: select fail errno=%d\n", me, errno);
			exit(1);
		}
		if(FD_ISSET(mountfd, fds)){
			ip = ipclisten(mountfd);
			if(ip == 0){
				logprint("+%s lost the announcement connection somehow\n", me);
				exit(1);
			}
			nfd = ipcaccept(ip);
			if(busy()){
				logprint("%s: reject %s!%s\n", me,
					ip->machine, ip->user);
				write(nfd, "B", 1);
				close(nfd);
				continue;
			} else {
				write(nfd, "R", 1);
				logprint("%s: accept %s!%s\n", me,
					ip->machine, ip->user);
				newclient(nfd);
				FD_SET(nfd, fds);
				maxfds = max(maxfds, nfd+1);
				sched();
			}
		}
		for(s = servers; s; s = ss){
			ss = s->next;			/* in case we go byebye */
			if(FD_ISSET(s->fd, fds))
				ackserver(s);
		}
	}
}

sched()
{
	Client *c;
	Disk *d;
	Server *s;

logprint("sched\n");
	for(c = clients; c; c = c->next)
		if(!c->busy)
			for(d = c->disks; d; d = d->next)
				if(!d->busy)
					if(s = newserver(d->name)){
						queuedisk(d, s);
						return;
					}
logprint("nothing to schedule\n");
}

busy()
{
	return(0);
}
