#include <sys/types.h>
#include <sys/inet/tcp_user.h>
#include <sys/inio.h>
#include <sys/filio.h>
#include <stdio.h>
#include <ipc.h>

/*
 *  This program gates between a tcp network and connection server
 *  machines that don't run the darpa Internet suite.
 */

/* imports */
extern char *in_host();
extern in_addr in_address();
extern int ip_ld;
extern char *strchr();
extern int errno;
extern char *errstr;

/* global */
char *me;

usage()
{
	fprintf(stderr, "usage: %s tcp-address\n", me);
	exit(1);
}

/*
 *  Prime function - shuffle bytes twixt two fd's
 */
gate(caller, callee)
	int caller;
	int callee;
{
	fd_set fds, bfds;

	/* shuttle bytes back and forth */
	FD_ZERO(bfds);
	FD_SET(caller, bfds);
	FD_SET(callee, bfds);
	for(;;) {
		fds = bfds;
		switch(select(NOFILE, &fds, (struct fd_set *)0, 1000)) {
		case -1:
			log("select failed %d\n", errno);
			return;
		case 0:
			continue;
		}
		if (FD_ISSET(caller, fds))
			switch(pass(caller, callee)){
			case -1:
				log("caller io error %d\n", errno);
				return;
			case 0:
				FD_CLR(caller, bfds);
				break;
			default:
				break;
			}
		if (FD_ISSET(callee, fds))
			switch(pass(callee, caller)){
			case -1:
				log("callee io error %d\n", errno);
			case 0:
				return;
			default:
				break;
			}
	}
}

pass(from, to)
	int from, to;
{
	char buf[4096];
	int n;

	if((n=read(from, buf, sizeof(buf)))<0)
		return -1;
	if (write(to, buf, n)!=n)
		return -1;
	return n;
}

/*
 *  Map an entry - for now use a simple mechanism
 *	a.b.c.att.com.,port x -> dk!c/b/a!tcp.x
 */
#define TAIL1 ".att.com"
#define TAIL2 ".att.com."

char *
tcptodk(inaddr, port)
	in_addr inaddr;
	int port;
{
	static char buf[256];
	char dkname[128];
	char *fields[16];
	char *host;
	int n;

	host = in_host(inaddr);
	n = strlen(host);
	if (n > sizeof(TAIL1)-1){
		if(strcmp(&host[n-sizeof(TAIL1)+1], TAIL1)==0)
			host[n-sizeof(TAIL1)+1] = '\0';
		else if(strcmp(&host[n-sizeof(TAIL2)+1], TAIL2)==0)
			host[n-sizeof(TAIL2)+1] = '\0';
	}
	setfields(".");
	n = getmfields(host, fields, 16);
	strcpy(dkname, fields[--n]);
	for(n--; n>=0; n--) {
		strcat(dkname, "/");
		strcat(dkname, fields[n]);
	}
	sprintf(buf, "dk!%s!tcp.%d", dkname, port);
	return buf;
}

/*
 *  create a pipe and push ip onto each side
 */
in_addr
ipconfig(netname)
	char *netname;
{
	in_addr addr, myaddr;
	int ippipe[2];

	addr = in_address(netname);
	if (addr == 0){
		fprintf(stderr, "%s: bad ip address %s\n", netname);
		exit(1);
	}
	myaddr = addr+255;

	/*
	 *  push the ip ld on a pipe to config the network
	 */
	pipe(ippipe);
	if(ioctl(ippipe[0], FIOPUSHLD, &ip_ld) < 0){
		perror(me);
		exit(1);
	}
	if(ioctl(ippipe[0], IPIOLOCAL, &myaddr) < 0){
		perror("IPIOLOCAL");
		exit(1);
	}
	ioctl(ippipe[0], IPIONET, &addr);
	if(ioctl(ippipe[1], FIOPUSHLD, &ip_ld) < 0){
		perror(me);
		exit(1);
	}
	if(ioctl(ippipe[1], IPIOLOCAL, &addr) < 0){
		perror("IPIOLOCAL");
		exit(1);
	}
	ioctl(ippipe[1], IPIONET, &myaddr);
	return(addr);
}

/*
 *  Listen to calls from tcp.  Transfer call via connection server.
 */
fromtcp(netname, addr)
	char *netname;
	in_addr addr;
{
	int fd;
	char *np;
	struct tcpuser tu;
	char name[128];
	int caller, callee;
	int pid;

	/*
	 *  request all tcp calls to the given network
	 */
	fd = tcp_sock();
	if (fd < 0){
		fprintf(stderr, "%s: no tcp devices\n", me);
		return;
	}
	tu.lport = TCPPORT_ANY;
	tu.laddr = addr;
	tu.fport = 0;
	tu.faddr = 0;
	tu.param = 0;
	if (tcp_listen(fd, &tu)<0) {
		fprintf(stderr, "%s: server already exists\n", me);
		exit(1);
	}

	/*
	 *  keep grabbing calls till something nasty happens
	 */
	for(;;){
		if ((caller = tcp_accept(fd, &tu))<0) {
			log("bad accept %d\n", errno);
			break;
		}
		if ((pid = fork()) < 0) {
			log("fork failed %d\n", errno);
			close(caller);	/* drop connection */
		}
		else if (pid) {
			while (wait((int *)0) != pid)
				;
			close(caller);
		} else {
			close(fd);
			switch (fork()) {	/* so main tcpgate not our parent */
			case -1:
				log("second fork failed %d\n", errno);
			default:
				exit(0);

			case 0:			/* child */
				break;
			}
			np = tcptodk(tu.laddr, tu.lport);
			log("call %s\n", np);
			callee = ipcopen(np, "");
			if (callee < 0)
				log("failed: %s\n", errstr);
			else
				gate(caller, callee);
			exit(0);
		}
	}
	close(fd);
}

main(ac, av)
	int ac;
	char *av[];
{
	in_addr addr;

	me = av[0];
	if(ac<2)
		usage();

	/* where the cs name space starts */
	chdir("/cs");

	addr = ipconfig(av[1]);
	for(;;){
		fromtcp(av[1], addr);
		sleep(10);
	}
}

log(s, a, b, c)
char *s;
{
	long now;

	time(&now);
	fprintf(stderr, "%.15s %d ", ctime(&now)+4, getpid());
	fprintf(stderr, s, a, b, c);
}
