#include "mgr.h"
#include "defs.h"
#include <errno.h>
#include <sys/param.h>
#include <sys/filio.h>
#include <signal.h>

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
	char *cp;
	static stretch newipcname;
	extern int rmesg_ld;

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

	/* see if we can push the rmesg ld */
/*
	if (ioctl(fd, FIOPUSHLD, &rmesg_ld)<0) {
		ipcreject(rp->i, errno, "can't push rmesg_ld");
		close(fd);
		return -1;
	}
*/

	/* gateway and call were accepted -- pass back a corrected ipcname */
	cp = strchr(ipcname, '!');
	_strcat(&newipcname, rp->i->myname, "!", cp);
	ipcdaccept(rp->i, fd, newipcname.ptr);
	return -1;
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
	ipcinfo *ip;
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

	/* the next info_read would destroy these fields */
	rp->i->machine = strdup(rp->i->machine);
	rp->i->user = strdup(rp->i->user);

	/* get the original request */
	info.uid = info.gid = 0;
	info.user="";
	if (_info_read(caller, &info)<0)
		return -1;

	/* dial the number */
	sprintf(newname, "%s!%s", ap->arg, info.name);
	info.name = newname;
	info.rfd = info.cfd = -1;
	oldsig=signal(SIGALRM, dingaling);
	alarm(30);
	callee = ipcdial(&info);
	alarm(0);
	signal(SIGALRM, oldsig);

	/* log it */
	rp->i->name = info.name;
	rp->i->myname = info.myname;
	logstatus("gate", rp->i);
	if (callee<0) {
		_reply_write(caller, errno, errstr);
		close(caller);
		return -1;
	}

	/* try to push mesg ld */
/*
	if(ioctl(callee, FIOPUSHLD, &mesg_ld)<0){
		_reply_write(caller, errno, "can't push mesg ld");
		close(caller);
		close(callee);
		return -1;
	}
*/

	/* tell gateout that it worked */
	if (_reply_write(caller, 0, ipcname)<0) {
		close(caller);
		close(callee);
		return -1;
	}

	/*
	 *  For creat's, we accept only one call per creat.  This
	 *  makes life a lot simpler since we can use the current
	 *  connection to the other end.
	 */
	if(info.flags & IPC_CREAT) {
		int nfd;
		struct fd_set fds;

		/* wait for call or hangup */
		FD_ZERO(fds);
		FD_SET(callee, fds);
		FD_SET(caller, fds);
		switch(select(NOFILE, &fds, (struct fd_set *)0, 10000000)){
		case 0:
		case -1:
			exit(0);
		}
		if(FD_ISSET(caller, fds))
			exit(0);

		/* get call info */
		if((ip = ipclisten(callee)) == NULL)
			exit(0);

		/* pass it creator */
		if(_info_write(caller, ip) < 0)
			exit(0);
		if(_reply_read(caller) < 0)
			exit(0);
		if(errno) {
			ipcreject(ip, errno, errstr);
			exit(0);
		} else {
			nfd = ipcdaccept(ip, -1, ipcname);
			close(callee);
			callee = nfd;
		}
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
				exit(0);
		if (FD_ISSET(callee, fds))
			if (pass(callee, caller)<0)
				exit(0);
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
