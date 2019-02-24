#include <stdio.h>
#include <sysexits.h>
#include <string.h>

/*
 * globals for ipc
 */

int	ipcdebug = 0;

extern int ipcerrno;

extern int tcp_connect();
extern int dk_connect();
extern int mx_connect();
extern int proxy_connect();

extern int tcp_close();
extern int dk_close();

#define INETPROXY	"tcp!inet.research.att.com!402"
/* #define INETPROXY	"dk!nj/astro/inet.proxy"*/

#define ATTPROXY	"tcp!att-in.att.com!402"
/* #define ATTPROXY	"dk!oh/cbl/att-in.proxy"*/

#ifndef PROXY
#define PROXY		ATTPROXY
#endif

struct	conntype {
	char	*prefix;
	int	(*connector)();
	int	(*closer)();
	char	*prhost;
} conntype[] = {
	{"tcp",   tcp_connect,   tcp_close,   NULL},
	{"dk",    dk_connect,    dk_close,    NULL},
	{"mx",    mx_connect,    tcp_close,   NULL},
	{"proxy", proxy_connect, NULL,        PROXY},
	{"inet",  proxy_connect, NULL,        INETPROXY},
	{"att",   proxy_connect, NULL,        ATTPROXY},
	{NULL,    NULL,		 NULL,	      NULL}
};

#define MAXFD	32		/* maximum file descriptors we handle */
int (*closelist[MAXFD])();

/*
 * ipcopen - determine the network and call the open processor for it.
 *	name is of the form	network!host!service
 *      param is passed on, for what it is worth.
 */
int
ipcopen(name, param)
char *name, *param;
{
	int fd;
	struct	conntype *ct;
	char buf[100];
	char *dialer = buf, *dialno;

	if (ipcdebug) {
		setbuf(stderr, (char *)0);
		fprintf(stderr, "ipcopen `%s` `%s`\n", name, param);
	}

	strncpy(buf, name, sizeof(buf));
	buf[sizeof(buf)] = '\0';
	dialno = strchr(buf, '!');
	if (dialno != NULL)
		*dialno++ = '\0';

	if ((*dialer) == '\0') {
		ipcseterror(EX_USAGE, "Dialer name missing", "Can't dial");
		return -1;
	}
	for (ct = conntype; ct->prefix; ct++)
		if (strcmp(ct->prefix, dialer) == 0)
			break;
	if (ct->prefix == NULL) {
		ipcseterror(EX_USAGE, "Unknown network type", "Can't dial");
		return -1;
	}

	if (ipcdebug)
		fprintf(stderr, "Connecting to network %s %s\n",
			dialer, dialno);
	if (ct->prhost == NULL)	{	/*not proxy connect*/
		fd = (ct->connector)(dialno, param);
		if ((fd >= 0) && (fd <= MAXFD))
			closelist[fd] = ct->closer;
	} else
		fd = proxy_connect(dialno, param, ct->prhost);

	if (ipcdebug)
		fprintf(stderr, "Ipcopen returning %d, err=%d\n",
			fd, ipcerrno);
	return fd;
}

int
ipcclose(fd)
int fd;
{
	if (ipcdebug)
		fprintf(stderr, "Closing fd %d\n", fd);
	if ((fd >= 0) && (fd <= MAXFD) && (closelist[fd] != 0)) {
		int rc = (closelist[fd])(fd);
		closelist[fd] = 0;
		return rc;
	} else
		return close(fd);
}

