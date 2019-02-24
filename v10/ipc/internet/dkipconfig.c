#include <sys/param.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/ttyio.h>
#include <sys/inio.h>
#include <sys/filio.h>
#include <sys/inet/in.h>
#include <ipc.h>


#define RESETTIME 15*60
#define DEFMTU 1500

static char *cmdname;
static in_addr myaddr;
static in_addr hisaddr;

int dontcheck;
FILE *fp;
char measnum[32];
char himasnum[32];

#define GOSTR "OK - send me your poor, your wretched, your huddled packets waiting to be free\n"

extern char *optarg;
extern int optind;

usage(cmd)
	char *cmd;
{
	fprintf(stderr, "Usage: %s [-d] [-m #] dk-address my-ip-addr his-ip-addr\n",
			cmd);
}

main(argc, argv)
char *argv[];
{
	int other_end, my_pid, c;
	char buf[256];
	unsigned int mtu;
	int n;

	signal(SIGHUP, SIG_IGN);
	cmdname = argv[0];

	mtu = DEFMTU;
	while((c=getopt(argc, argv, "dm:")) != -1)
		switch(c){
		case 'd':
			dontcheck = 1;
			break;
		case 'm':
			mtu = atoi(optarg);
			break;
		default:
			usage(cmdname);
			break;
		}

	switch(argc-optind){
	case 2:
		close(2);
		translate(argv[optind], argv[optind+1]);
		ipconfig(0, mtu);
		write(0, GOSTR, sizeof(GOSTR)); 
		waitforhup(0);
		break;
	case 3:
		detach("dkipconfig");
		translate(argv[optind+1], argv[optind+2]);
		while(1) {
			other_end = callup(argv[optind], himasnum, measnum, mtu);
			if (other_end < 0) {
				logevent("%s: %s\n", argv[optind], errstr);
				sleep(60);
				continue;
			}
			if (ioctl(other_end, TIOCSPGRP, 0) < 0) {
				logevent("TIOCSPGRP %d\n", errno);
				close(other_end);
				sleep(60);
				continue;
			}
			while((n=read(other_end, buf, sizeof(buf)))>0){
				if(n == sizeof(GOSTR) && strcmp(GOSTR, buf) == 0)
					break;
			}
			ipconfig(other_end, mtu);
			waitforhup(other_end);
			close(other_end);
		}
		break;
	default:
		usage(cmdname);
		exit(1);
	}
}

/*
 *  translate names to ip addresses
 */
translate(me, it)
	char *me;
	char *it;
{
	fp = fopen("/usr/ipc/log/dkip", "a");
	if(fp==0){
		fprintf(stderr, "dkipconfig: can't open log\n", it);
		fp = stderr;
	}
	hisaddr = in_address(it);
	if(hisaddr == 0){
		logevent("dkipconfig: unknown host/net %s\n", it);
		fclose(fp);
		exit(1);
	}
	strcpy(himasnum, in_ntoa(hisaddr));
	if(dontcheck)
		myaddr = in_address(me);
	else
		myaddr = in_addronnet(me, hisaddr, 0);
	if(myaddr == 0){
		logevent("unknown host %s on net %s\n", me,
			in_ntoa(hisaddr));
		exit(1);
	}
	strcpy(measnum, in_ntoa(myaddr));
	logevent("connecting as %s to %s\n", measnum, himasnum);
	fclose(fp);
}

static int
ipconfig(ipfd, mtu)
	int ipfd;
	unsigned int mtu;
{
	extern int ip_ld;

	if(ioctl(ipfd, FIOPUSHLD, &ip_ld) < 0){
		logevent("can't push ip\n");
		exit(1);
	}
	if(ioctl(ipfd, IPIOLOCAL, &myaddr) < 0){
		logevent("can't set local address\n");
		exit(1);
	}
	ioctl(ipfd, IPIOMTU, &mtu);
	if(in_netof(hisaddr) != hisaddr){
		if(ioctl(ipfd, IPIOHOST, &hisaddr) < 0){
			logevent("can't set remote address\n");
			exit(1);
		}
	} else {
		if(ioctl(ipfd, IPIONET, &hisaddr) < 0){
			logevent("can't set network address");
			exit(1);
		}
	}
}

static int
callup(host, a1, a2, mtu)
	char *host, *a1, *a2;
	unsigned int mtu;
{
	char cmd[512];
	int fd;

	fd=ipcopen(ipcpath(host, "dk", "dkip"), "heavy");
	if(fd<0)
		return(fd);
	sprintf(cmd, "-m %d %s %s", mtu, a1, a2);
	write(fd, cmd, strlen(cmd)+1);
	return(fd);
}

waitforhup(fd)
{
	char buf[128];

	while(read(fd, buf, sizeof(buf))>0)
		;
}
