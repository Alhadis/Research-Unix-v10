#include "ipc.h"
#include "mgr.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/filio.h>

/* buffer definition */
#define BUFLEN 512

/* our name */
char *ipcname;

/*
 *  timeout routine
 */
static int
dingaling()
{
	signal(SIGALRM, dingaling);
}

/*
 *  gate a call to the arpanet
 */
main()
{
	int caller, callee;
	ipcinfo info;
	ipcinfo *ip;
	fd_set fds;
	char newname[BUFLEN];
	int (*oldsig)();

	/*
	 *  get the original request
	 */
	info.uid = info.gid = 0;
	info.user="root";
	if (_info_read(caller, &info)<0)
		return -1;

	/*
	 *  dial the number
	 */
	sprintf(newname, "%s!%s", "/cs/tcp", info.name);
	info.name = newname;
	info.rfd = info.cfd = -1;
	oldsig=signal(SIGALRM, dingaling);
	alarm(30);
	callee = ipcdial(&info);
	alarm(0);
	signal(SIGALRM, oldsig);

	/*
	 * return status 
 	 */
	if (callee<0) {
		_reply_write(caller, errno, errstr);
		close(caller);
		return -1;
	}
	if (_reply_write(caller, 0, ipcname)<0) {
		close(caller);
		close(callee);
		return -1;
	}

	/*
	 *  For creat's, we accept only one call per creat.  This
	 *  makes life a lot simpler though less general.
	 */
	if(info.flags & IPC_CREAT) {
		int nfd;

		if((ip = ipclisten(callee)) == NULL)
			exit(0);
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

	/*
	 *  shuttle bytes back and forth
	 */
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

/*
 *  Send the connection info.
 */
int
_info_write(fd, ip)
	int fd;
	ipcinfo *ip;
{
	char b[BUFLEN];
	int n;

	if (ip->name==NULL)
		ip->name = "";
	if (ip->param==NULL)
		ip->param = "";
	if (ip->machine==NULL)
		ip->machine = "";
	if (ip->user==NULL)
		ip->user = "";
	if (ip->myname==NULL)
		ip->myname = "";
	sprintf(b, "%s\n%s\n%s\n%s\n%s\n%d\n%d\n%d\n", ip->myname, ip->name,
		ip->param, ip->machine, ip->user, ip->flags, ip->uid, ip->gid);
	n = strlen(b);
	if (write(fd, b, n)!=n)
		return ABORT(errno, "can't send request", NULLINFO);
	return 0;
}


/* dial a number or plug into the name space */
ipcdial(ip)
	ipcinfo *ip;
{
	/*
	 *  decode the call
	 */
	if(
	if (ip->flags && IPC_CREAT){
		/* plug into the name space */
	} else {
	}
}

/* listen for a connection */
ipcinfo *
ipclisten(fd)
	int fd;
{
	static ipcinfo info;
	static char buf[BUFSIZE];
	static char user[32];

	/* accept only one connection */
	info.rfd = fd;
	close(fd);

	/* get the request */
	if (_info_read(info.rfd, &info)<0) {
		/* requestor gave up */
		close(info.rfd);
		return NULL;
	}

	return &info;
}

/*
 *  Accept a connection.  Close all except ip->cfd.
 */
int
ipcaccept(ip)
	ipcinfo *ip;
{
	ipcdaccept(ip, -1, "who_cares");
}

/*
 *  Accept a connection, and supply a source address and communications fd
 */
int
ipcdaccept(ip, commfd, source)
	ipcinfo *ip;
	int commfd;
	char *source;
{
	if (commfd >= 0) {

		/* supply our own channel for communications */
		if (_fd_write(ip->rfd, commfd) < 0) {
			close(commfd);
			return ABORT(errno, "can't pass conection", ip);
		}
		_reply_write(ip->rfd, 0, source);
		ABORT(0, "", ip);
		ip->cfd = commfd;
	} else if (ip->cfd >= 0) {

		/* use client supplied channel for communications */
		_reply_write(ip->rfd, 0, "");
		close(ip->rfd);
		ip->rfd = -1;
	} else {

		/* use reply channel for communications */
		_reply_write(ip->rfd, 0, "");
		ip->cfd = ip->rfd;
		ip->rfd = -1;
	}
	return(ip->cfd);
}

/*  Reject a connection.
 */
int
ipcreject(ip, no, str)
	ipcinfo *ip;
	int no;		/* error number */
	char *str;	/* error string */
{
	_reply_write(ip->rfd, no, str);
	ABORT(no, str, ip);
	return 0;
}
