#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/net/in.h>
#include <sys/net/inet.h>
#include <sys/net/netdb.h>
#include <sysexits.h>
#include <string.h>

extern int ipcdebug;

#define htons(x)	(x)

int
tcp_connect(dest, param)
char *dest, *param;
{
	struct hostent *hp;
	char	*inet_ntoa();
	int s, port, i;
	long l=0;
	unsigned long addr;
	char buf[100];
	char *host, *service;

	if (ipcdebug)
		fprintf(stderr, "connecting via tcp `%s'\n", dest);

	strcpy(buf, dest);
	host = buf;

	if ((port = tcp_service(host)) < 0) {
		ipcabort(EX_UNAVAILABLE, "unknown service");
		return(-1);
	}

	if (ipcdebug)
		fprintf(stderr, "TCP port number is %d\n", port);

	if ((addr = inet_addr(host)) != (unsigned long) -1) {
		if (ipcdebug)
			fprintf(stderr, "Numeric address: %s %lx\n",
				host, addr);

		if((s = s5sockbcon(inet_ntoa(l),port,"/dev/tcp"))>=0)
			return(s);
		ipcabort(EX_OSERR, "can't open connection");
		t_close(s);
		return -1;
	}

	if((hp = gethostbyname(host)) == (struct hostent *)0){
		ipcabort(EX_NOHOST, "unknown host");
		return(-1);
	}

	memcpy(&l,hp->h_addr,hp->h_length);
	if (ipcdebug)
		fprintf(stderr, "Trying address %s\n",inet_ntoa(l));

	s = s5sockbcon(inet_ntoa(l),port,"/dev/tcp");
	if(s < 0){
		ipcabort(EX_OSERR, "can't open connection");
	} else return(s);

	t_close(s);

	ipcabort(EX_NOHOST, "can't connect to host");
	return -1;
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
		return htons(atoi(service));
	else if ((sp = getservbyname (service, "tcp")) != NULL)
		return sp->s_port;
	return -1;
}

/*
 * The following is a fixed version of the inet_addr() routine, since
 * the one delivered with UTS 5.2.6b has a bug that truncates the
 * passed string.  Sigh !!
 */

int
inet_addr(str)
{
	int val;
	char *b;
	char *ptr;
	int byte[4];
	int nb, bb;
	char buf[16];

	strcpy(buf,str);
	nb = 0;
	for (b = strtok(buf,"."); b; b = strtok(NULL,".")) {
		if (nb == 4) return -1;
		byte[nb++] = strtol(b,&ptr,0);
		if (ptr == b || *ptr) return -1;
	}
	if (nb == 0) return -1;
	if (nb == 1) return byte[0];
	val = 0;
	for (bb = 0; bb < nb-1; bb++) val = (val << 8) | byte[bb];
	return (val << 8*(4-bb)) | byte[bb];
}
