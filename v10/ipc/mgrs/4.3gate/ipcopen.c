#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <ipc.h>
#include "defs.h"

/* exported */
char *errstr;
cahr *ipcname;

/* imports */
extern int atoi();
extern char *memchr();

/* local only */
static int net[128];

extern int tcpdial(), tcpaccept(), tcpreject();
ipcinfo *tcplisten();

static struct network{
	char *name;
	int (*dial)();
	ipcinfo *(*listen)();
	int (*accept)();
	int (*reject)();
} nets[] = {
#define TCP 0
	{ "/cs/tcp", tcpdial, tcplisten, tcpaccept, tcpreject },
	{ 0, 0, 0, 0, 0 }
};

/*
 *  announce or connect to a detination
 */
int
ipcdial(ip)
	ipcinfo *ip;
{
	int fd;
	char name[128]; 
	char *field[3];
	struct network *np;
	static char nipcname[128];

	/* parse the name */
	strcpy(name, ip->name);
	setfields("!");
	getfields(name, field, 3);

	/* action depends on network */
	for(np=nets; np->name; np++)
		if (strcmp(field[0], np->name)==0)
			break;
	if (np->name) {
		ip->net = net[fd] = np-nets;
		fd = (*np->dial)(ip, field[1], field[2]);
		sprintf(nipcname, "%s!%s", nets[ip->net].name, ipcname);
		ipcname = nipcname;
	}

	/* unknown network/service */
	return _ipcabort("not implemented", EIO, ip);
}

/*
 *  plug into a name space
 */
int
ipccreat(name, param)
	char *name;	/* name being dialed */
	char *param;	/* parameters for creation */
{
	int pfd[2];
	char *path=name;
	ipcinfo info;

	/* remote creat */
	info.rfd = info.cfd = -1;
	info.myname = info.user = info.machine = NULL;
	info.uid = info.gid = -1;
	info.name = name;
	info.param = param;
	info.flags = IPC_CREAT;
	return ipcdial(&info);
}

/*
 *  call up an announced service
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
 *  set error number and string and return -1
 */
int
_ipcabort(no, err, ip)
	int no;
	char *err;
	ipcinfo *ip;
{
	errstr = err;
	errno = no;
	return -1;
} 

/* 
 *  listen for a connection
 */
ipcinfo *
ipclisten(fd)
	int fd;
{
	static ipcinfo info;
	static char myname[128];
	static char machine[128];
	static char nipcname[128];
	ipcinfo *ip;

	info.rfd = info.cfd = -1;
	info.myname = info.machine = "";
	info.user = "unknown";
	info.uid = info.gid = -1;
	info.name = name;
	info.param = param;
	info.flags = IPC_CREAT;
	ip = (*nets[net[fd]].listen)(fd, &info);
	if(ip == NULL)
		return(ip);
	ip->net = net[fd];
	sprintf(myname, "%s!%s", nets[ip->net].name, ip->myname);
	ip->myname = myname;
	sprintf(machine, "%s!%s", nets[ip->net].name, ip->machine);
	ip->machine = machine;
	sprintf(nipcname, "%s!%s", nets[ip->net].name, ipcname);
	ipcname = nipcname;
	return(ip);
}

/*
 *  Accept a connection.  Close all except ip->cfd.
 */
int
ipcaccept(ip)
	ipcinfo *ip;
{
	return (*nets[ip->net].accept)(ip);
}

/*
 *   Reject a connection.
 */
int
ipcreject(ip, no, str)
	ipcinfo *ip;
	int no;		/* error number */
	char *str;	/* error string */
{
	return (*nets[ip->net].reject)(ip, no, str);
	_ipcabort(no, str, ip);
	return 0;
}
