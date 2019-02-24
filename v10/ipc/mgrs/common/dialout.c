#include <sys/param.h>
#include <stdio.h>
#include <sys/filio.h>
#include <signal.h>
#include <ipc.h>
#include <libc.h>
#include <wait.h>
#include "defs.h"

/* preeclared */
void dodialout();
void dodialin();
void doopen();
void docreat();
void doredial();

/* global */
int pid;
int debug;
int zombie;
int children;

/* imported */
extern char *av0;
extern int net_dial();
extern int net_redial();
extern int net_announce();
extern void net_accept();
extern void net_reject();
extern ipcinfo *net_listen();
extern char *getlogin();

static
deadbaby()
{
	zombie = 1;
}

static
ding()
{
}

/* loop on calls out of the CPU */
void
dodialout(mtpt)
	char *mtpt;
{
	int fd;
	long next;
	long last, now;
	int status;
	int npid;

	signal(SIGPIPE, SIG_IGN);
	pid = getpid();

	/*
	 *  plug into local name space.  Perform exponential backoff for
	 *  reporting failure.  Console paper is finite.
	 */
	last = time((long *)0);
	next = 30;
	for(;;){
		fd = ipccreat(mtpt, "");
		if (fd>=0)
			break;
		now = time((long *)0);
		if(now-last >= next){
			logconsole("%s: can't announce as %s (%s)\n", av0, mtpt, errstr);
			next = 2*next;
		}
		sleep(30);
	}
	chmod(mtpt, 0666);
	logconsole("%s: announced to fs as %s\n", av0, mtpt);
	fflush(stdout);

	/*
	 *  loop on outgoing requests
	 */
	for(;;) {
		ipcinfo *ip;
		int errs;

		/*
		 *  eat zombies
		 */
		if (zombie) {
			zombie = 0;
			while(children){
				signal(SIGALRM, ding);
				alarm(2);
				npid = wait(&status);
				signal(SIGCHLD, deadbaby);
				alarm(0);
				if(npid<0)
					break;
				children--;
			}
		}

		/*
		 *  wait for a request, continue on interrupts
		 */
		errs = 0;
		while((ip = ipclisten(fd))==NULL){
			if(errno!=EINTR && errs++>5) {
				logevent("dialout: out broken listen\n");
				close(fd);
				return;
			}
		}
		if(debug)
			logevent("dialout: rcved\n");

		/*
		 *  create new process for request
		 */
		signal(SIGCHLD, deadbaby);
		switch(fork()) {
		case -1:		/* whoops */
			ipcreject(ip, errno, "no more processes");
			logstatus("out", ip);
			continue;
		case 0:
			close(fd);
			break;
		default:
			children++;
			(void)ABORT(0, "", ip);
			continue;
		}
		if(debug)
			logevent("dialout: forked\n");

		/*
		 *  perform request
		 */
		if (ip->flags & IPC_CREAT)
			docreat(ip, mtpt);
		else if (ip->flags & IPC_REDIAL)
			doredial(ip, mtpt);
		else
			doopen(ip, mtpt);
		if(debug)
			logevent("dialout: done\n");
		exit(0);
	}
}

/* establish a connection to a net name */
void
doopen(ip, mtpt)
	ipcinfo *ip;
	char *mtpt;
{
	int fd;
	static stretch myname;

	*av0 = 'D';

	/*
	 *  dial out over the network
	 */
	fd = net_dial(ip);
	if (fd < 0) {
		ipcreject(ip, errno, errstr);
		logstatus("out", ip);
		return;
	}

	/*
	 *  accept the request, passing back the network file descriptor
	 *  and our local name in the form "network!machine!service".
	 */
	_strcat(&myname, mtpt, "!", ipcname);
	ipcdaccept(ip, fd, myname.ptr);

	errno = 0; errstr = "";
	logstatus("out", ip);
	close(fd);
}

/* announce a new netname */
void
docreat(ip, mtpt)
	ipcinfo *ip;
	char *mtpt;
{
	ipcinfo *netip;
	int listenfd;
	int toclient;
	int pfd[2];
	fd_set fds;
	static stretch myname;

	/*
	 *  no invisible gatewaying, it must be subject to authentication
	 */
	if (ip->cfd >= 0) {
		ipcreject(ip, EIO, "can't do remote ipccreat");
		logstatus("out", ip);
		return;
	}

	*av0 = 'L';

	/*
	 *  create a unique channel 
	 *  for communications with requestor
	 */
	if (pipe(pfd) < 0) {
		ipcreject(ip, errno, "can't create local channel");
		logstatus("out", ip);
		return;
	}
	toclient = pfd[1];

	/*
	 *  dial out on network
	 */
	listenfd = net_announce(ip);
	if (listenfd < 0) {
		close(pfd[0]);
		close(pfd[1]);
		ipcreject(ip, errno, errstr);
		logstatus("out", ip);
		return;
	}

	/*
	 *  accept the announce request.  Return our address, if we have
	 *  one in the form "network!machine!service"
	 */
	_strcat(&myname, mtpt, "!", ipcname);
	if (ipcdaccept(ip, pfd[0], myname.ptr) < 0)
		return;
	close(pfd[0]);
	errno = 0; errstr = "";
	logstatus("out", ip);

	/* loop waiting for in calls */
	FD_ZERO(fds);
	FD_SET(listenfd, fds);
	FD_SET(toclient, fds);
	while(1) {
		fd_set rfds;
		int rv;

		/* check for input or hang-up */
		rfds = fds;
		rv = select(NOFILE, &rfds, (fd_set*)0, 10000);
		if (rv == 0)
			continue;
		else if (rv < 0)
			break;
		else if (FD_ISSET(toclient, rfds))
			break;

		/* get request */
		netip = net_listen(listenfd);
		if (netip == NULL) {
			logevent("net_listen error %d %s\n", errno, errstr);
			ABORT(errno, errstr, ip);
			break;
		}

		/*
		 *  make a new channel to the listener, and pass him
		 *  one end.
		 */
		if (pipe(pfd)<0) {
			net_reject(netip, errno, "no more pipes");
			logstatus("in", netip);
			continue;
		}
		if (ioctl(toclient, FIOSNDFD, &(pfd[0]))<0) {
			net_reject(netip, errno, "protocol botch");
			logstatus("in", netip);
			break;
		}
		close(pfd[0]);

		/*
		 *  pass the request over the new channel.  include name
		 *  used to make the call.
		 */
		if (ioctl(pfd[1], FIOSNDFD, &(netip->rfd))<0) {
			net_reject(netip, errno, "protocol botch");
			close(pfd[1]);
			logstatus("in", netip);
			continue;
		}
		close(netip->rfd);
		netip->rfd = -1;
		_strcat(&myname, mtpt, "!", ip->myname);
		ip->myname = myname.ptr;
		if (_info_write(pfd[1], netip) < 0) {
			net_reject(netip, errno, "protocol botch");
			close(pfd[1]);
			logstatus("in", netip);
			continue;
		}

		/*
		 *  get the client's reply and pass it on to the net
		 */
		if (_reply_read(pfd[1]) < 0) {
			/*  didn't work, drop call */
			net_reject(netip, errno, "protocol botch");
		} else if(errno<0) {
			/*  client will handle the accept/reject */
			close(netip->cfd);
		} else if (errno) {
			net_reject(netip, errno, errstr);
		} else
			net_accept(netip);
		logstatus("in", netip);
		close(pfd[1]);
	}
	return;
}

/* redial a call */
void
doredial(ip, mtpt)
	ipcinfo *ip;
	char *mtpt;
{
	*av0 = 'R';

	/*
	 *  redial over the network
	 */
	net_redial(ip);
	ipcaccept(ip);
	ABORT(0, (char *)0, ip);
	logstatus("redial", ip);
}
