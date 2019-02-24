untested - there are some minor changes in tcpconnect that have not been
moved to this routine.

#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sysexits.h>
#include <string.h>

extern int ipcdebug;

extern char syserrstr[];

/*
 * This is an older version of tcp_connect for systems that
 * use the old Berkeley 4.2 gethostbyname which returned
 * a single host entry instead of several.
 *
 * This code hasn't been tested as hard as the newer stuff.
 */
int
tcp_connect(dest, param)
char *dest, *param;
{
	struct hostent *hp;
	struct sockaddr_in sin;
	char	*inet_ntoa();
	int s, port, i;
	unsigned long addr;
	char buf[100];
	char *host, *service;
	char *h_addr_list[2];
	int errtype = 0;
	char errstr[256];

	strcpy(buf, dest);
	host = buf;

	if ((port = tcp_service(host)) < 0) {
		ipcseterror(EX_NOHOST, "unknown service", "tcp_connect");
		return(-1);
	}

	if (ipcdebug)
		fprintf(stderr, "TCP port number is %d\n", ntohs(port));

	if ((addr = inet_addr(host)) != (unsigned long) -1) {
		if (ipcdebug)
			fprintf(stderr, "Numeric address: %s %lx\n",
				host, addr);
		s = socket(AF_INET, SOCK_STREAM, 0);
		if(s < 0){
			ipcsyserr("can't open socket");
			return(-1);
		}
		bzero((char *)&sin, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = port;
		sin.sin_addr.s_addr = addr;
		if(connect(s, (struct sockaddr *)&sin, sizeof (sin)) >= 0) {
			setopt(s);
			return s;
		}
		ipcsyserr("can't connect");
		close(s);
		return -1;
	}

	if((hp = gethostbyname(host)) == (struct hostent *)0){
		ipcseterror(EX_NOHOST,  "unknown host", "tcp_connect");
		return(-1);
	}
	h_addr_list[0] = hp->h_addr;
	h_addr_list[1] = 0;
	for(i=0; h_addr_list[i]; i++ ){ 
		bzero((char *)&sin, sizeof(sin));
		bcopy(h_addr_list[i], &sin.sin_addr, hp->h_length);
		sin.sin_family = hp->h_addrtype;
		sin.sin_port = port;
		if (ipcdebug)
			fprintf(stderr, "Trying address %s\n",
				inet_ntoa(sin.sin_addr));

		s = socket(AF_INET, SOCK_STREAM, 0);
		if(s < 0){
			ipcsyserr("can't open socket");
			return(-1);
		}
		if(connect(s, (struct sockaddr *)&sin, sizeof (sin)) >= 0) {
			setopt(s);
			return s;
		}
		ipcgetsyserr();
		if (errtype == 0 || 
		   (errtype != EX_TEMPFAIL && ipcmaperror()==EX_TEMPFAIL)) {
			errtype = ipcmaperror();
			strcpy(errstr, syserrstr);
		}
		ipcsyserr("connect");
		close(s);
	}

	ipcseterror(errtype, errstr, "Can't connect to host");
	return -1;
}

setopt(s)
int s;
{
	/*
	 * for some reason, the following linger does not seem
	 * to have any effect, at least on the Sun `shamash'.
	 * I have placed a sleep before the close in ../pftp/ftp.c
	 * in the routine sendrequest.
	 *	suggestions are welcome...ches@research.att.com
	 */
	struct linger linger;

	linger.l_onoff = 1;
	linger.l_linger = 3000;
	if (setsockopt(s, SOL_SOCKET, SO_LINGER, (char *)&linger,
	    sizeof (linger)) < 0)
		ipcsyserr("error setting socket option");
	return s;
}

int
tcp_service(path)
char *path;
{
	struct servent *sp;
	char *service;

	if ((service = strchr(path, '!')) == NULL)
		service = "";
	else
		*service++ = '\0';

	/*
	 *  translate service/port name
	 */
	if(strncmp(service, "tcp.", 4)==0)
		service += 4;
	if(strncmp(service, "/cs/", 4)==0)
		service += 4;
	if(strcmp(service, "")==0)
		return 0;
	else if(atoi(service)!=0)
		return atoi(htons(service));
	else if ((sp = getservbyname (service, "tcp")) != NULL)
		return sp->s_port;
	return -1;
}

int
tcp_close(fd)
int fd;
{
	write(fd, &fd, 0);	/* stub - is this useful or necessary? */
	if (shutdown(fd, 2) < 0)
		ipcseterror(EX_SOFTWARE, "shutdown failed", "tcp_close");
	return close(fd);
}
