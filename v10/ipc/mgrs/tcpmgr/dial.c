#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <ipc.h>
#include <sys/filio.h>
#include <sys/inio.h>
#include <sys/inet/tcp_user.h>
#include <sys/inet/tcp_timer.h>
#include <sys/inet/tcp_var.h>
#include <libc.h>
#include <ctype.h>
#include "defs.h"

#define CSPORT 1

#define CALLIN 0
#define CALLOUT 1
#define ANNOUNCE 2
#define CALLINCS 3

/* imported */
extern char *in_ntoa();
extern in_addr in_address();
extern int tcp_connect();
extern int tcp_accept();
extern int tcp_listen();
extern char *in_host();
extern in_addr in_aton();
extern int rmesg_ld;
extern char *mydomain;

Qset *domainorder();

/* dial a service on the internet - try first via cs, then direct */
int
net_dial(ip)
	ipcinfo *ip;
{
	char *service;
	int lport, fport;
	int i, n, fd;
	char *fields[16];
	Qset *sp, *tsp;
	Qtuple *tp;
	int tries;
	char *dom, *in, *firstdom;
	char dname[256];
	int gooddom;

	lport = 0;
	tries = 0;

	/* break parameter list */
	n = breakparams(ip, fields, 16);
	for(i=0; i<n; i++){
		if(strncmp(fields[i], "port=", 5)==0){
			lport=atoi(fields[i]+5);
			if(lport<1024 && ip->uid!=0)
			    return ABORT(ENOENT, "illegal port", NULLINFO);
			break;
		}
	}

	/* get the service */
	service = strchr(ip->name, '!');
	if (service != NULL)
		*service++ = '\0';
	else
		return ABORT(ENOENT, "unassigned service", NULLINFO);
	fport = fstotcp(service);

	/* look up the system */
	strcpy(dname, ip->name);
	strcat(dname, ",dom");
	sp = qset(dname, (char *)NULL);
	if (sp==NULL) {
		sp = qset(ip->name, (char *)NULL);
		if(sp==NULL){
			/* no such system - assume we have an ip number */
			if(isdigit(*ip->name))
				return dialip(ip->name, lport, fport, fields, n,
						 ip, service);
			else
				return ABORT(ENOENT, "unassigned destination",
				 NULLINFO);
		}
	}

	/*  call each INET address in succession.  If a domain name is
	 *  encountered, try no entry of another doain name.
	 */
	firstdom = NULL;
	for(tsp=sp; tsp; tsp=tsp->next) {
		gooddom = 0;
		dom = in = NULL;
		for(tp=tsp->this; tp; tp=tp->next)
			if(tp->type && strcmp(tp->type, "in")==0)
				in = tp->value;
			else if(tp->type && strcmp(tp->type, "dom")==0){
				dom = tp->value;
				if(firstdom==NULL) {
					firstdom = dom;
					gooddom = 1;
				} else {
					if(strcmp(firstdom, dom)==0)
						gooddom = 1;
				}
			}
		if(in){
			if(!gooddom && dom)
				continue;
			tries++;
			fd = dialip(in, lport, fport, fields, n, ip,
					service);
			if(fd>=0){
				freeQset(sp);
				return(fd);
			}
		}
	}

	/* none worked */
	freeQset(sp);
	if(tries)
		return ABORT(errno, errstr, NULLINFO);
	else
		return ABORT(ENOENT, "unassigned destination", NULLINFO);
}

/*
 *  order tuples relative to a domain address
 */
int
matchval(np, dp)
	char *np;
	char *dp;
{
	char *cnp, *cdp;
	int rv = 0;

	cnp = np+strlen(np);
	cdp = dp+strlen(dp);
	do {
		while(--cdp>dp)
			if(*cdp=='.')
				break;
		while(--cnp>np)
			if(*cnp=='.')
				break;
		if(strcmp(cdp, cnp)!=0)
			break;
		rv++;
	} while(cdp>dp);
	return rv;
}

dialalarm()
{
}

/*
 *  dial one ip address
 *
 *  Use a port < 1024 if:
 *	a) the caller is root
 *	b) we are calling the connection server
 *	c) the caller requests us to perform bsd authentication for him
 */
dialip(number, lport, fport, fields, n, ip, service)
	char *number;
	int lport, fport;
	char *fields[];
	int n;
	ipcinfo *ip;
	char *service;
{
	struct tcpuser tu;
	in_addr faddr;
	int fd;
	int (*oldsig)();
	int ba;

	logevent("trying: %s\n", number);
	faddr = in_aton(number);
	if(faddr==INADDR_ANY)
		return ABORT(ENOENT, "unassigned destination", NULLINFO);
	ba = bsdauth(fields, n);

	/* get a channel for the connection */
	fd = tcp_sock();
	if (fd < 0)
		return ABORT(EBUSY, "out of output channels", NULLINFO);

	/*
	 *  connect to the remote connection server if there is not a well known
	 *  port for this service.
	 */
	if(fport<=0){
		tu.laddr = INADDR_ANY;
		tu.lport = 0;
		tu.faddr = faddr;
		tu.fport = CSPORT;
		tcpoptions(&tu, fields, n, ba, ip);
		errstr = "destination not answering";

		/*
		 *  give connection servers 5 seconds to answer (if it can)
		 */
		oldsig = signal(SIGALRM, dialalarm);
		alarm(5);
		if (tcp_connect(fd, &tu)>=0) {
			alarm(0);
			signal(SIGALRM, oldsig);

			/*
			 *  send the request
			 */
			ip->name = service;
			if (_info_write(fd, ip)==0)
				if (_reply_read(fd)==0 && errno==0) {
					condition(fd, fields, n, ip->user);
					filloutnames(&tu, ip, CALLOUT);
					return fd;
				}
		}
		alarm(0);
		signal(SIGALRM, oldsig);
		close(fd);
		return -1;
	} else {

		/*
		 *   connect to the well known port
		 */
		fd = tcp_sock();
		if (fd < 0)
			return ABORT(EBUSY, "out of output channels", NULLINFO);
		tu.laddr = INADDR_ANY;
		tu.lport = lport;
		tu.faddr = faddr;
		tu.fport = fport;
		tcpoptions(&tu, fields, n, ba, ip);
		if (tu.fport<=0) {
			close(fd);
			return ABORT(ENOENT, "unknown service", NULLINFO);
		}
	
		/*
		 *  give other system 15 seconds to answer (if it can)
		 */
		oldsig = signal(SIGALRM, dialalarm);
		alarm(15);
		if (tcp_connect(fd, &tu)<0) {
			alarm(0);
			signal(SIGALRM, oldsig);
			close(fd);
			return -1;
		}
		alarm(0);
		signal(SIGALRM, oldsig);
		condition(fd, fields, n, ip->user);
		filloutnames(&tu, ip, CALLOUT);
		return fd;
	}
}

/* return true if bsd authentication is requested */
bsdauth(fields, n)
	char *fields[];
	int n;
{
	int i;

	for (i=0; i<n && fields[i]; i++)
		if (strcmp(fields[i], "bsdauth")==0)
			return 1;
	return 0;
}

/* create options */
tcpoptions(tp, fields, n, bsdauth, ip)
	struct tcpuser *tp;
	char *fields[];
	ipcinfo *ip;
{
	int i;

	if(bsdauth)
		tp->param = SO_TRUSTED;
	else
		tp->param = ip->uid==0 ? SO_TRUSTED : 0;

	for (i=0; i<n && fields[i]; i++) {
		if (strcmp(fields[i], "keepalive")==0)
			tp->param |= SO_KEEPALIVE;
	}
}

/* condition the connection as requested */
condition(fd, fields, n, user)
	int fd;
	char *fields[];
	char *user;
{
	int i;
	int dohup=0;
	int dobsd=0;
	int domesg=0;

	for (i=0; i<n && fields[i]; i++) {
		if (strcmp(fields[i], "hup")==0)
			dohup = 1;
		if (strcmp(fields[i], "delim")==0)
			domesg = 1;
		if (strcmp(fields[i], "bsdauth")==0){
			dobsd = 1;
		}
	}
	if (dobsd){
		if(!user)
			user = "_unknown_";
		write(fd, "", 1);
		write(fd, user, strlen(user)+1);
		write(fd, user, strlen(user)+1);
	}
	if (dohup){
		ioctl(fd, TCPIOHUP, 0);
	}
	if (domesg){
		ioctl(fd, FIOPUSHLD, &rmesg_ld);
	}
}

/* get the local name out of the connection info */
filloutnames(tp, ip, type)
	struct tcpuser *tp;
	ipcinfo *ip;
{
	static stretch newipcname;
	static stretch fmachine;
	static stretch lmachine;
	static stretch fportname;
	static stretch lportname;
	char *fields[16];

	_strcat(&lmachine, in_ntoa(tp->laddr), (char *)0, (char *)0);
	_strcat(&lportname, "xxxxxxxxxx", (char *)0, (char *)0);
	switch (type) {
	case CALLINCS:
		/*
		 *  a call from the network via connection server
		 */
		sprintf(lportname.ptr, "tcp.%d", tp->lport);
		_strcat(&fmachine, in_host(tp->faddr), (char *)0, (char *)0);
		ip->myname = lmachine.ptr;
		ip->machine = fmachine.ptr;
		break;
	case CALLIN:
		/*
		 *  a call from the network
		 */
		sprintf(lportname.ptr, "tcp.%d", tp->lport);
		_strcat(&fmachine, in_host(tp->faddr), (char *)0, (char *)0);
		ip->myname = lmachine.ptr;
		ip->machine = fmachine.ptr;
		break;
	case CALLOUT:
		/*
		 *  a call to the network
		 */
		sprintf(lportname.ptr, "%d", tp->lport);
		_strcat(&fmachine, in_ntoa(tp->faddr), (char *)0, (char *)0);
		_strcat(&fportname, "xxxxxxxxxx", (char *)0, (char *)0);
		sprintf(fportname.ptr, "%d", tp->fport);
		ip->myname = fmachine.ptr;
		ip->name = fportname.ptr;
		ip->machine = lmachine.ptr;
		break;
	case ANNOUNCE:
		/*
		 * an announcement
		 */
		sprintf(lportname.ptr, "%d", tp->lport);
		ip->myname = lmachine.ptr;
		ip->name = lportname.ptr;
		ip->machine = lmachine.ptr;
		break;
	}
	_strcat(&newipcname, lmachine.ptr, "!", lportname.ptr);
	ipcname = newipcname.ptr;
}

/* break up parameter list */
breakparams(ip, fields, n)
	ipcinfo *ip;
	char *fields[];
	int n;
{
	static stretch param;

	if (*ip->param){
		_strcat(&param, ip->param, (char *)0, (char *)0);
		setfields(" \t");
		n = getmfields(param.ptr, fields, n);
	} else
		n = 0;

	return n;
}

/* announce onto the internet */
int
net_announce(ip)
	ipcinfo *ip;
{
	int fd, n;
	struct tcpuser tu;
	static stretch dest;
	char *fields[4];
	in_addr myaddr;
	int port;

	/*
	 *  break the address into host and service
	 */
	_strcat(&dest, ip->name, (char *)0, (char *)0);
	setfields("!");
	n = getmfields(dest.ptr, fields, 4);
	switch(n){
	case 0:
		/* accept all calls not specificly asked for */
		fields[0] = "";
		fields[1] = "";
		break;
	case 1:
		/* accept specified address */
		fields[1] = "";
		break;
	case 2:
		break;
	default:
		return ABORT(ENOENT, "illegal address", NULLINFO);
	}

	/*
	 *  no address means listen for calls to any address
	 */
	if (*fields[0]) {
		myaddr = in_address(fields[0]);
		if(myaddr==0)
			return ABORT(ENOENT, "illegal address", NULLINFO);
	} else
		myaddr = INADDR_ANY;

	/*
	 *  no port means listen for calls to any port.  port '*' means
	 *  pick me a port to listen on (returned in ipcname)
	 */
	if (*fields[1]){
		if (strcmp(fields[1], "*")==0)
			port = 0;
		else {
			port = atoi(fields[1]);
			if(port<1024 && ip->uid!=0)
				return ABORT(ENOENT, "illegal port", NULLINFO);
		}
	} else
		port = TCPPORT_ANY;

	/* perform the announcement */
	fd = tcp_sock();
	if (fd < 0)
		return ABORT(EBUSY, "no more output channels", NULLINFO);
	tu.lport = port;
	tu.laddr = myaddr;
	tu.fport = 0;
	tu.faddr = 0;
	tu.param = ip->uid==0 ? SO_TRUSTED : 0;
	if (tcp_listen(fd, &tu)<0) {
		close(fd);
		return ABORT(EEXIST, "server already exists", NULLINFO);
	}
	filloutnames(&tu, ip, ANNOUNCE);
	return fd;
}

/* listen for a call in */
ipcinfo *
net_listen(fd)
	int fd;
{
	static ipcinfo info;
	static char myname[PATHLEN];
	static char name[PATHLEN];
	static char machine[PATHLEN];
	struct tcpuser tu;
	char param[128];
	char *fields[16];
	int n;
	extern char *tcptofs();

	info.flags = 0;
	for(;;) {
		tu.param = 0;
		if ((fd = tcp_accept(fd, &tu))<0) {
			logevent("listen: tcp_accept failed-- errno %d\n", errno);
			return NULL;
		}
		if (tu.lport == CSPORT) {
			if (tu.fport > 1023)
				info.uid = info.gid = -1;
			else
				info.uid = info.gid = 0;
			info.user = "_unknown_";
			if (_info_read(fd, &info)<0) {
				logevent("listen: info_read failed -- errno %d\n", errno);
				close(fd);
				return (ipcinfo *)NULL;
			}
			info.flags |= IPC_HANDOFF;

			/* break parameters */
			n = breakparams(&info, fields, 16);
			condition(fd, fields, n, (char *)0);
			filloutnames(&tu, &info, CALLINCS);
		} else {
			info.user = tu.fport>=1024 ? "_unknown_" : "root";
			info.param = "";
			strcpy(name, tcptofs(tu.lport));
			info.name = name;
			filloutnames(&tu, &info, CALLIN);
		}
		info.flags |= IPC_OPEN;
		info.rfd = fd;
		info.cfd = -1;
		return &info;
	}
}

/* accept a call */
void
net_accept(ip)
	ipcinfo *ip;
{
	USE(ip);
}

/* reject a call - null if a non-cs call */
void
net_reject(ip, no, str)
	ipcinfo *ip;
	int no;
	char *str;
{
	if (ip->flags&IPC_HANDOFF && ip->rfd>=0)
		_reply_write(ip->rfd, no, str);
	if (ip->rfd>=0) {
		close(ip->rfd);
		ip->rfd = -1;
	}
}

int
net_redial(ip)
	ipcinfo *ip;
{
	return -1;
}

