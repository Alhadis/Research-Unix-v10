#include <sys/types.h>
#include <sys/filio.h>
#include <sys/stat.h>
#include <ipc.h>
#include "defs.h"

/* exported */
char *errstr;

/* imports */
extern int atoi();
extern char *memchr();

/*
 *  dial a name and return an fd to the connection
 */
int
ipcopen(name, param)
	char *name;	/* name being dialed */
	char *param;	/* parameters for dialer */
{
	ipcinfo info;

	info.rfd = info.cfd = -1;
	info.myname = info.user = info.machine = NULL;
	info.uid = info.gid = -1;
	info.name = name;
	info.param = param;
	info.flags = IPC_OPEN;
	return ipcdial(&info);
}

/*
 *  Open a mounted steam.  ip->name points to a string of the form
 *  path!address.  Path must be a file system path to a mounted stream.
 *  One element moves from the beginning of ip->name to the end of ip->myname.
 */ 
_ipcopen(ip)
	ipcinfo *ip;
{
	static stretch path;
	static stretch myname;
	int fd;
	char *np;
	char *oname;
	struct stat sbuf;

	/*
	 *  remove next element of ip->name ...
	 */
	oname = ip->name;
	_strcat(&path, ip->name, (char *)NULL, (char *)NULL);
	np = strchr(path.ptr, '!');
	if (np) {
		*np = '\0';
		ip->name += np - path.ptr + 1;
	} else
		ip->name += strlen(ip->name);

	/*
	 *  ... and add it to the translated part
	 */
	if(ip->myname) {
		_strcat(&myname, ip->myname, "!", path.ptr);
		ip->myname = myname.ptr;
	} else
		ip->myname = path.ptr;

	/*
	 *  make sure this is a mounted file.  the link count for
	 *  such beasts is 0.  There should be a less hacky way.
	 */
	if (stat(path.ptr, &sbuf)<0 || sbuf.st_nlink) {
		ip->name = oname;
		return ABORT(ENOENT, "destination nonexistent", NULLINFO);
	}

	/*
	 *  This is a hack.  Daemon's should not blithely write on files
	 *  without checking permissions.  Setting IPC_CAREFUL makes us
	 *  check for blanket write permission before writing on a file.
	 */
	if (ip->flags&IPC_CAREFUL) {
		if (sbuf.st_mode&6 != 6){
			ip->name = oname;
			return ABORT(EACCES, "permission denied", NULLINFO);
		}
	}

	/*
 	 *  Finally, open the (presumabley mounted upon) file
	 */
	if ((fd = open(path.ptr, 2))<0) {
		ip->name = oname;
		if (errno==EACCES){
			if (*(ip->name))
			    return ABORT(EACCES, "can't access dialer", NULLINFO);
			else
			    return ABORT(EACCES, "permission denied", NULLINFO);
		} else
			return ABORT(ENOENT, "destination nonexistent", NULLINFO);
	}
	return fd;
}

/*
 *  dial a name and return an fd to the connection
 */
int
ipcdial(ip)
	ipcinfo *ip;
{
	struct passfd pass;
	int fd;

	/*
	 *  open the mounted stream, the result is a unique connection to
	 *  the mounting process.
	 */
	if ((fd = _ipcopen(ip))<0)
		return -1;

	/*
	 * pass reply channel
	 */
	if (ip->rfd >= 0) {
		if (_fd_write(fd, ip->rfd)<0) {
			close(fd);
			return ABORT(EIO, "protocol botch", ip);
		}
		close(ip->rfd);
		ip->rfd = -1;
	}

	/*
	 * pass communications channel (if not same as reply channel)
	 */
	if (ip->cfd >= 0) {
		if (_fd_write(fd, ip->cfd)<0) {
			close(fd);
			return ABORT(EIO, "protocol botch", ip);
		}
		close(ip->cfd);
		ip->cfd = -1;
	}

	/*
	 *  pass the request
	 */
	if (_info_write(fd, ip) < 0) {
		close(fd);
		return ABORT(errno, errstr, ip);
	}
	if (ip->flags&IPC_HANDOFF)
		return fd;

	/*
	 *  get the reply, it may be preceded by a
	 *  file descriptor to use as the communications 
	 *  channel.
	 */
	if (_fd_read(fd, &pass)>=0) {
		_reply_read(fd);
		if (errno != 0) {
			close(fd);
			close(pass.fd);
			return ABORT(errno, errstr, NULLINFO);
		}
		close(fd);
		return pass.fd;
	} else {
		_reply_read(fd);
		if (errno != 0) {
			close(fd);
			return ABORT(errno, errstr, NULLINFO);
		}
		return fd;
	}
}

/*
 *  Pass a request to someone else to handle.
 */
ipcpass(ip)
	ipcinfo *ip;
{
	ip->flags |= IPC_HANDOFF;
	return ipcdial(ip);
}

/*
 *  Cause network to recdial a call.  Return 0 if dialer took the redial, -1
 *  otherwise.  In any case, close off the connection.
 */
ipcredial(ip)
	ipcinfo *ip;
{
	int fd;

	if(ip->rfd){
		_reply_write(ip->rfd, -1, "redial");
		close(ip->rfd);
		ip->rfd = -1;
	}
	ip->flags = IPC_REDIAL;
	fd = ipcdial(ip);
	if(fd<0)
		return -1;
	ABORT(0, (char *)0, ip);
	close(fd);
	return 0;
}

/*
 * set error number and string and return -1
 */
int
_ipcabort(no, err, ip)
	int no;
	char *err;
	ipcinfo *ip;
{
	if (ip!=NULLINFO) {
		if (ip->cfd>0) {
			close(ip->cfd);
			ip->cfd = -1;
		}
		if (ip->rfd>0) {
			close(ip->rfd);
			ip->rfd = -1;
		}
	}
	errstr = err;
	errno = no;
	return -1;
}

