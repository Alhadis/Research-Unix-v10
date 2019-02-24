#include <sys/types.h>
#include <sys/filio.h>
#include <ipc.h>
#include "defs.h"

/* imports */
extern int conn_ld;
extern struct passwd *pwsearch();
extern char *strncpy();

/* global to this file */
#define ROOTUID 0

/*
 *  Attach to a name space.  If the path is a single element,
 *  just mount into the file system.  Otherwise, pass the request
 *  to a network dialer.
 */
int
ipccreat(name, param)
	char *name;	/* name being dialed */
	char *param;	/* parameters for creation */
{
	int pfd[2];
	char *path=name;
	ipcinfo info;
	int nfd;

	if (path==NULL)
		return ABORT(EINVAL, "name too long", (ipcinfo*)NULL);

	/*
	 *  if a path is specified, pass the request through a dialer
	 */
	if (strchr(path, '!')!=NULL) {
		info.rfd = info.cfd = -1;
		info.myname = info.user = info.machine = NULL;
		info.uid = info.gid = -1;
		info.name = name;
		info.param = param;
		info.flags = IPC_CREAT;
		return ipcdial(&info);
	}

	/*
	 *  Try creating a file to mount on
	 */
	if(access(path, 0) < 0){
		nfd = creat(path, 0666);
		if(nfd < 0)
			return ABORT(errno, "can't mount", (ipcinfo*)NULL);
		close(nfd);
	}

	/*
	 *  make a pipe to mount into the file system and push the
	 *  connection line discipline.  Conn_ld ensures that all
	 *  opens of the pipe will spawn a unique connection.
	 */
	if (pipe(pfd) < 0)
		return ABORT(errno, "out of pipes", (ipcinfo*)NULL);
	if (ioctl(pfd[1], FIOPUSHLD, &conn_ld) < 0) {
		close(pfd[0]);	
		close(pfd[1]);
		return ABORT(errno, "pushing line discipline", (ipcinfo*)NULL);
	}

	/* mount */
	if (fmount(3, pfd[1], path, 0) < 0) {
		close(pfd[0]);
		close(pfd[1]);
		return ABORT(errno, "can't mount", (ipcinfo*)NULL);
	}
	close(pfd[1]);
	return pfd[0];
}

/*
 *  listen for a connection
 */
ipcinfo *
ipclisten(fd)
	int fd;
{
	struct passfd pass;
	int pfd[2];
	static ipcinfo info;
	static char user[32];
	int fd1=-1, fd2=-1;

	/* 
	 *  Get a unique stream to a caller, or reuse this one if none is passed.
	 *  The reuse is typical of the arpa! gateway.
	 */
	if(_fd_read(fd, &pass) < 0) {
		if(errno==EINTR)
			return NULL;
		info.uid = -1;
		info.gid = -1;
		info.rfd = dup(fd);
		close(fd);
	} else {
		info.uid = pass.uid;
		info.gid = pass.gid;
		info.rfd = pass.fd;
	}
	strncpy(user, pass.logname, sizeof(pass.logname));
	user[sizeof(pass.logname)] = '\0';
	info.user = user;
	(void)ioctl(info.rfd, FIOACCEPT, &pass);

	/*
	 *  Get possible passed fds.  Up to two can be passed, i.e.
	 *  the fd to reply to and the one to use for communication.
	 */
	if (_fd_read(info.rfd, &pass)>=0) {
		fd1 = pass.fd;
		if (_fd_read(info.rfd, &pass)>=0) {
			fd2 = pass.fd;
		} else if(errno==EINTR){
			close(info.rfd);
			close(fd1);
			return NULL;
		}
	} else if(errno==EINTR){
		close(info.rfd);
		return NULL;
	}

	/*
	 *  get the request
	 */
	if (_info_read(info.rfd, &info)<0) {
		/* requestor gave up */
		close(info.rfd);
		if(fd1>=0)
			close(fd1);
		if(fd2>=0)
			close(fd2);
		return NULL;
	}

	if (info.flags & IPC_HANDOFF) {
		/*
		 *  This is a call passed to us
		 *  by someone else.
		 */
		close(info.rfd);
		info.rfd = fd1;
		info.cfd = fd2;
		info.flags &= ~IPC_HANDOFF;
	} else {
		/*
		 *  By default the fd on which the
		 *  request was received will be used
		 *  both for replies and communication
		 */
		info.cfd = fd1;
		if (fd2>=0)
			close(fd2);
	}
	return &info;
}

/*
 *  Accept a connection.
 *  Close all fd's except ip->cfd.
 */
int
ipcaccept(ip)
	ipcinfo *ip;
{
	return ipcdaccept(ip, -1, "who_cares");
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
		/*
		 *  supply our own channel for communications
		 */
		if (_fd_write(ip->rfd, commfd) < 0) {
			close(commfd);
			return ABORT(errno, "can't pass conection", ip);
		}
		_reply_write(ip->rfd, 0, source);
		ABORT(0, "", ip);
		ip->cfd = commfd;
	} else if (ip->cfd >= 0) {
		/*
		 *  use client supplied channel for communications
		 */
		_reply_write(ip->rfd, 0, "");
		close(ip->rfd);
		ip->rfd = -1;
	} else {
		/*
		 *  use reply channel for communications
		 */
		_reply_write(ip->rfd, 0, "");
		ip->cfd = ip->rfd;
		ip->rfd = -1;
	}
	return(ip->cfd);
}

/*
 *  Reject a connection.
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
