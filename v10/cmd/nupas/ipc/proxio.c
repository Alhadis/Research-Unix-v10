/*
 * proxy - connect to the external Internet through a proxy machine.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <string.h>
#include <sysexits.h>

#include "proxy.h"

char ipcname[100];

extern char *ipcpath();
extern int  ipcdebug;

int
proxy_connect(dest, param, prhost)
char *dest, *param, *prhost;
{
	int s;
	char *f[5], *reply, buf[PATHLEN];
	int i, ipcflag;

	if ((s = ipcopen(prhost, param)) < 0)
		return s;

	if (strcmp(param, "create") == 0) /* stub - not V10 parameter! */
		ipcflag = IPC_CREAT;
	else
		ipcflag = IPC_OPEN;

	strcpy(buf, dest);
	setfields("!");
	if (getfields(buf, f, 3) < 3) {
		strcpy(buf, "tcp!");
		strcat(buf, dest);
	}

	if ((send_ipcinfo(s, buf, ipcflag) != 0) ||
	    (read_ipcreply(s, &reply)      != 0)) {
		ipcsyserr("proxy_connect");
		close(s);
		return -1;
	}

	/* *reply should hold "/cs/tcp!<ip address>!port#", which is
	   address of the connection on proxy machine.  Save the conport. */
	strncpy(ipcname, reply, sizeof(ipcname));
	setfields("!");
	if (getfields(reply, f, 3) != 3) {
		ipcseterror(EX_PROTOCOL, "gateway protocol error 2",
			"proxy_connect");
		close(s);
		return -1;
	}
	if (ipcdebug)
		fprintf(stderr, "reply = %s\n", ipcname);
	return s;
}

int
send_ipcinfo(fd, name, flag)
	int fd;
	char *name;
	int flag;
{
	char b[INFOLEN], *user, *getlogin();
	char hostname[256];
	int namelen = sizeof(hostname);
	int n;

	if (name==NULL)
		name = "";
	user = getlogin();
	if (user==NULL)
		user = "";
	gethostname(hostname, namelen);	/* stub - this needs simulation in Sys V */
	sprintf(b, "\n%s\n\n%s\n%s\n%d\n-1\n-1\n", name, hostname, user, flag);
	n = strlen(b);
	if (ipcdebug)
		fprintf(stderr, "send_ipcinfo (%d): |%s||%s|%s|%d|-1|-1|\n",
			n, name, hostname, user, flag);
	if (write(fd, b, n) != n) {
		ipcseterror(EX_PROTOCOL, "send_ipcinfo",
			"sending to gateway machine");
		return -1;
	}
	return 0;
}

int
send_ipcreply(fd, code, answer)
	int fd;
	int code;
	char *answer;
{
	char b[PATHLEN];
	int n;

	if (answer==NULL)
		answer = "";
	sprintf(b, "%d\n%s\n", code, answer);
	n = strlen(b);
	if (ipcdebug)
		fprintf(stderr, "send_ipcreply (%d): %d|%s|\n",
			n, code, answer);
	if (write(fd, b, n) != n) {
		ipcseterror(EX_PROTOCOL, "send_ipcreply",
			"sending to gateway machine");
		return -1;
	}
	return 0;
}

int
read_ipcinfo(fd)
	int fd;
{
	static char b[INFOLEN];
	int n;

	n = read(fd, b, sizeof(b));
	if (n <= 0) {
		ipcseterror(EX_PROTOCOL, "read_ipcinfo",
			"can't read from proxy gateway");
		return -1;
	}
	if (ipcdebug) {
		char buf[PATHLEN], *cp;
		strncpy(buf, b, n);
		buf[n] = '\0';
		for (cp = buf; *cp; cp++)
			if (*cp == '\n')
				*cp = '|';
		fprintf(stderr, "read_ipcinfo (%d): %s\n", n, buf);
	}
	/* could crack the fields if we wanted to */
	return 0;
}

int
read_ipcreply(fd, answer)
	int fd;
	char **answer;
{
	static char b[PATHLEN];
	char *f[2], *ptr;
	int c, n;

	*answer = "";
	n = read(fd, b, sizeof(b));
	if (n <= 0) {
		ipcseterror(EX_PROTOCOL, "read_ipcreply",
			"can't read from proxy gateway");
		return -1;
	}
	b[n] = '\0';
	if (ipcdebug) {
		char buf[PATHLEN], *cp;
		strncpy(buf, b, n);
		buf[n] = '\0';
		for (cp = buf; *cp; cp++)
			if (*cp == '\n')
				*cp = '|';
		fprintf(stderr, "read_ipcreply (%d): %s\n", n, buf);
	}
	setfields("\n");
	if (getfields(b, f, 2) != 2) {
		ipcseterror(EX_PROTOCOL, "gateway protocol botch",
			"can't read from proxy gateway");
		return -1;
	}
	if ((ptr=strchr(f[1], '\n'))!=NULL)
		*ptr = '\0';
	*answer = f[1];
	c = atoi(f[0]);
	if (c != 0) {
		ipcseterror(c, f[1], "gateway error");
		return -1;
	}
	return 0;
}
