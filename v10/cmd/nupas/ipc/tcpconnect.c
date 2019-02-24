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

void do_gethosterror();

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
		do_gethosterror();
		return -1;
	}

	for(i=0; hp->h_addr_list[i]; i++){
		bzero((char *)&sin, sizeof(sin));
		bcopy(hp->h_addr_list[i], &sin.sin_addr, hp->h_length);
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
	return 0;
}

#ifdef HOST_NOT_FOUND
/*
 * This processes the new gethostbyname error returns.
 */
void
do_gethosterror() {
	extern int h_errno;

	if (ipcdebug)
		fprintf(stderr, "gethostbyname returns h_errno = %d\n",
			h_errno);
	switch (h_errno) {
  	case HOST_NOT_FOUND:
		ipcseterror(EX_NOHOST,  "unknown host", "tcp_connect");
		break;
	case TRY_AGAIN:
		ipcseterror(EX_TEMPFAIL,  "name server timeout", "tcp_connect");				break;
	/* On inet, the following was returned for the address `pente..msd' */
	case NO_RECOVERY:
		ipcseterror(EX_USAGE,  "unrecoverable error", "tcp_connect");				break;
	case NO_ADDRESS:
		ipcseterror(EX_TEMPFAIL,  "IP address unavailable",
			"tcp_connect");
		break;
	default:
		ipcseterror(EX_SOFTWARE,  "Unknown h_errno",
			"tcp_connect");
		break;
	}
}
#else /*HOST_NOT_FOUND*/

/*
 * This processes the old dumb gethostbyname error returns.
 */
void
do_gethosterror() {
	ipcseterror(EX_NOHOST,  "unknown host", "tcp_connect");
	return;
}
#endif

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
		return htons(atoi(service));
	else if ((sp = getservbyname (service, "tcp")) != NULL)
		return sp->s_port;
	return -1;
}

int
tcp_close(fd)
int fd;
{
	struct linger l;
	int lingersize=sizeof(l), n;

	l.l_onoff = SO_LINGER; l.l_linger = 4000;
	setsockopt(fd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
	return close(fd);
}
