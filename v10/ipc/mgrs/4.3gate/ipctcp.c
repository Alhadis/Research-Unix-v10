#include <sys/types.h>		/* needed for socket.h */
#include <sys/uio.h>		/* needed for socket.h */
#include <sys/socket.h>
#include <sysexits.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include "ipc.h"

int
tcpdial(ip, host, service)
	ipcinfo *ip;
{
	struct hostent *hp;
	struct servent *sp;
	struct sockaddr_in sin;
	struct sockaddr_in me;
	int fd, lport, n;
	char field[5];
	int melen;
	char *dotaddr();
	static char nipcname[128];

	bzero((char *)&sin, sizeof(sin));
	bzero((char *)&me, sizeof(me));

	/*
	 *  translate host name 
	 */
	if (strcmp(host, "*")!=0) {
		if ((hp = gethostbyname(host)) == (struct hostent *) NULL) {
			errstr = "unknown host";
			errno = ENOENT;
			return(-1);
		}
		bcopy(hp->h_addr, &sin.sin_addr, hp->h_length);
		sin.sin_family = hp->h_addrtype;
	}

	/*
	 *  translate service/port name
	 */
	if(strcmp(service, "*")==0)
		sin.sin_port = 0;
	else if(strncmp(service, "tcp.", 4)==0)
		sin.sin_port = htons(atoi(service+4));
	else if(atoi(service)!=0)
		sin.sin_port = htons(atoi(service));
	else if ((sp = getservbyname (service, "tcp")) == NULL)
		sin.sin_port = sp->s_port;
	else {
		errstr = "unknown service";
		errno = ENOENT;
		return(-1);
	}

	/*
	 *  get a socket
	 */
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		errstr = "can't open socket";
		return(-1);
	}

	/*
	 *  connect or listen
	 */
	if(ip->params & IPC_CREAT) {
		/*
		 *  get a local port number
		 */
		if (bind(fd, &sin, sizeof(sin))<0) {
			errstr = "can't bind to requested address";
			close(fd);
			return(-1);
		}
		/*
		 *  specify number of incoming calls
		 */
		if (listen(s, 10) < 0) {
			close(fd);
			return(-1);
		}
	} else {
		/*
		 *  look through params for local port number
		 */
		lport=0;
		setfields(" \t");
		n = getmfields(ip->param, field, 5);
		for(n--; n>=0; n--)
			if (strncmp(field[n], "port=", 5)==0)
				lport = atoi(field[n]+5);
		me.sin_port = htons(lport);
		/*
		 *  connect to a local port
		 */
		if (bind(fd, &me, sizeof(me))<0) {
			errstr = "can't bind to requested address";
			close(fd);
			return(-1);
		}
		/*
		 *  dial the other end
		 */
		if (connect(fd, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
			errstr = "can't connect";
			close(fd);
			return(-1);
		}
	}

	/*
	 *  remember my name
	 */
	melen = sizeof(me);
	getsockname(nfd, &me, melen);
	ipcname = dotaddr(&me, nipcname);
	return(fd);
}

/*
 *  listen for a connection
 */
ipcinfo *
tcplisten(fd, ip)
	int fd;
	ipcinfo *ip;
{
	int nfd;
	static char machine[32];
	static char myname[32];
	struct sockaddr_in from;
	struct sockaddr_in me;
	struct hostent *hp;
	int fromlen;
	int melen;
	char *dotaddr();

	/*
	 *  accept the connection
	 */
	fromlen = sizeof(from);
	nfd = accept(fd, (struct sockaddr *)&from, &fromlen);
	if(nfd<0) {
		close(fd);
		return(NULL);
	}

	/*
	 *  figure out the caller's true name
	 */
	hp = gethostbyaddr((char *)&from, fromlen);
	if(hp != NULL)
		ip->machine = hp->h_name;
	else
		ip->machine = dotaddr(&from, machine);

	/*
	 *  figure out our true name
	 */
	melen = sizeof(me);
	getsockname(nfd, &me, melen);
	ip->myaddr = dotaddr(&me, machine);
	ip->cfd = nfd
	ipcname = machine;
	return(ip);
}

/*
 *  create dot form of the address (byte by arduous byte)
 */
char *
dotaddr(ap, np)
	struct sockaddr_in *ap;
	char *np;
{
	int port;
	unsigned char *cp;

	cp = (char *)&ap->sin_addr;
	port = ntohs(ap->sin_port);
	sprintf(np, "%d.%d.%d.%d!%d", cp[0], cp[1], cp[2], cp[3], port);
	return(np);
}

/*
 *  accept a call
 */
tcpaccept(ip)
	ipcinfo *ip;
{
	return(ip->cfd);
}

/*
 *  reject a call
 */
tcpreject(ip, no, str)
	ipcinfo *ip;
	int no;
	char *str;
{
	close(ip->cfd);
	ip->cfd = -1;
	return(0);
}
