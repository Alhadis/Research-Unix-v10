 *	Remote dialer routine.
 */
#include <sys/types.h>
#include <stdio.h>
#include <libc.h>
#include <sys/utsname.h>
#include <errno.h>
#include <sys/filio.h>
#include <ipc.h>
#include "defs.h"

/* export */
char *av0;
char *param;
int debug;

/* cheap foiegn imports */
extern void dodialout(), dodialin();

usage(name)
	char *name;
{
	fprintf(stderr, "usage: %s [-m mount-pt] [-n netname] gate1 [gate2 ...]\n", name);
	exit(1);
}

/*
 *  Gate calls to/from a remote network.
 */
main(ac, av)
	int ac;
	char *av[];
{
	char *netname=NULL, *mtpt="rem", *cp;
	int ai;

	av0 = av[0];
	chdir("/cs");

	/* get args */
	for (ai=1; ai<ac; ai++) {
		if (av[ai][0] == '-')
			for (cp=&av[ai][1]; *cp; cp++) {
				switch(*cp) {
				case 'm':
					if (ai+1>=ac)
						usage(av[0]);
					mtpt = av[++ai];
					break;
				case 'n':
					if (ai+1>=ac)
						usage(av[0]);
					netname = av[++ai];
					break;
				case 'd':
					debug = 1;
					break;
				default:
					usage(av0);
				}
			}
		else
			break;
	}

	/* get gateways */
	if(ai>=ac)
		usage(av0);

	/* detach from process group, terminal, etc. */
	if (!debug)
		detach(mtpt);

	/* get defaults */
	if (netname==NULL) {
		struct utsname name;

		uname(&name);
		netname = name.nodename;
	}

	/* create dialer and listener */
	switch (fork()) {
	case -1:
		perror(av0);
		exit(1);
	case 0:
		for(;;)
			dodialout(mtpt);
	default:
		for(;;)
			dodialin(mtpt, netname, "heavy");
	}
}

static int
dingaling()
{
	signal(SIGALRM, dingaling);
}

/*
 *  Dial out to a gateway.  Return -1 if no more gates are to be tried.
 *  Return 0 if more gates are to be tried.
 */
gateout(rp, ap)
	Request *rp;
	Action *ap;
{
	int fd;
	extern int rmesg_ld;
	char netname[ARB];
	char *cp;

	if (rp->i->cfd>=0) {
		ipcreject(rp->i, EINVAL, "gate-through disallowed");
		return -1;
	}

	/* call `gateway' */
	fd = ipcopen(ap->arg, rp->i->param);

	/* send original request */
 	if (fd<0 || _info_write(fd, rp->i)<0) {
		/* if there are any more gateout's, keep trying */
		if (ap->next==NULL)
			ipcreject(rp->i, errno, errstr);
		close(fd);
		return 0;
	}

	/* see if the gateway could place the call */
	if (_reply_read(fd)<0 || errno!=0) {
		/* call was rejected, don't try any more gateouts */
		ipcreject(rp->i, errno, errstr);
		close(fd);
		return -1;
	}

	/* gateway and call were accepted */
	if(rp->i->flags == IPC_CREAT){
		/* for remote announcements, this process becomes a mux */
		lfd = ipcaccept(rp->i);
		localmux(fd, lfd);
		return -1;
	} else {
		/* for remote dials, just pass the net fd back to the process */
		ipcdaccept(rp->i, fd, ipcname);
		return -1;
	}
}

/*
 *  Acccept a gateway call
 */
gateway(rp, ap)
	Request *rp;
	Action *ap;
{
	int caller, callee;
	ipcinfo info;
	fd_set fds;
	char newname[ARB];
	extern int mesg_ld;
	char *mapuser();
	int (*oldsig)();

	/* see if we gateway for this requestor */
	if (mapuser(rp->s->name, rp->i->machine, rp->i->user)==NULL) {
		ipcreject(rp->i, EACCES, "gateway disallowed");
		return -1;
	}
	if ((caller=ipcaccept(rp->i))<0)
		return -1;

	/* get the original request */
	info.uid = info.gid = 0;
	info.user="";
	if (_info_read(caller, &info)<0)
		return -1;

	/* make the call */
	if(rp->i->flags == IPC_CREAT){
		/* for remote announcements, this process becomes a mux */
		lfd = ipcaccept(rp->i);
		localmux(fd, lfd);
		return -1;
	} else {
		/* for remote dials, just shuttle bytes back and forth */
		ipcdaccept(rp->i, fd, "who_cares");
		return -1;
	}

	/* dial the number */
	sprintf(newname, "%s!%s", ap->arg, info.name);
	info.name = newname;
	info.rfd = info.cfd = -1;
	info.flags = IPC_OPEN;
	oldsig=signal(SIGALRM, dingaling);
	alarm(30);
	callee = ipcdial(&info);
	alarm(0);
	signal(SIGALRM, oldsig);
	if (callee<0) {
		_reply_write(caller, errno, errstr);
		close(caller);
		return -1;
	}

	/* tell gateout that it worked */
	if (_reply_write(caller, 0, "")<0) {
		close(caller);
		close(callee);
		return -1;
	}

	/* shuttle bytes back and forth */
	FD_ZERO(fds);
	for(;;) {
		FD_SET(caller, fds);
		FD_SET(callee, fds);
		switch(select(NOFILE, &fds, (struct fd_set *)0, 1000)) {
		case -1:
			return -1;
		case 0:
			continue;
		}
		if (FD_ISSET(caller, fds))
			if (pass(caller, callee)<0)
				return -1;
		if (FD_ISSET(callee, fds))
			if (pass(callee, caller)<0)
				return -1;
	}
}

pass(from, to)
	int from, to;
{
	char buf[4096];
	int n;

	if ((n=read(from, buf, sizeof(buf)))<=0)
		return -1;
	if (write(to, buf, n)!=n)
		return -1;
	return 0;
}
