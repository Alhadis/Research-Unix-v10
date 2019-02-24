#include "ipc.h"

/*
 *  Read the connection info.  If an error occurs, ip->reply and ip->conn are
 *  closed.
 */
int
_info_read(fd, ip)
	int fd;
	ipcinfo *ip;
{
	static char b[BUFLEN];
	char *f[8];
	int n;

	n=read(fd, b, sizeof(b));
	if (n <= 0) 
		return _ipcabort(errno, "error reading request", ip);
	b[n] = '\0';
	setfields("\n");
	if (getfields(b, f, 8)!=8)
		return _ipcabort(EINVAL, "protocol botch", ip);
	ip->myname = f[0];
	ip->name = f[1];
	ip->param = f[2];
	ip->machine = f[3];
	ip->flags = atoi(f[5]);

	/* supply a system name */
	if (ip->uid!=ROOTUID || ip->machine[0]=='\0')
		ip->machine = "";

	/* supply a user name */
	if (ip->uid==ROOTUID && *(f[4])!='\0')
		ip->user = f[4];

	/* supply uid/gid cruft */
	if (ip->uid==ROOTUID && atoi(f[6])!=-1) {
		ip->uid = atoi(f[6]);
		ip->gid = atoi(f[7]);
	}
	return 0;
}

/*
 *  Send a reply to a connection request
 */
_reply_write(fd, no, str)
	int fd;
	int no;
	char *str;
{
	char b[BUFLEN];
	int n;

	if (str==NULL)
		str = "";
	sprintf(b, "%d\n%s\n", no, str);
	n = strlen(b);
	if (write(fd, b, n)!=n)
		return -1;
	return 0;
}

/*
 *  Get a reply to a connection request.
 */
int
_reply_read(fd)
	int fd;
{
	static char b[BUFLEN];
	char *f[2];
	char *ptr;
	int n;

	while((n=read(fd, b, sizeof(b)))<0 && errno==EINTR)
		;
	if (n <= 0) 
		return _ipcabort(errno, "error reading request", NULLINFO);
	b[n] = '\0';
	parselines(b, f, 2);
	errno = atoi(f[0]);
	if ((ptr=strchr(f[1], '\n'))!=NULL)
		*ptr = '\0';
	if (errno!=0)
		errstr = f[1];
	else
		ipcname = f[1];
	return 0;
}
