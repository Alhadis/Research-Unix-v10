#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sgtty.h>
#include <sys/ethernet.h>
#include <sys/inet/in.h>
#include "config.h"

extern errno;
extern int ip_ld;

main()
{
	char *me="loopback";
	unsigned long myaddr;
	int ld, i;
	int pfd[2];

	myaddr = in_address(me);
	if(myaddr == 0){
		fprintf(stderr, "ipconfig: unknown host %s\n", me);
		exit(1);
	}
	signal(SIGHUP, SIG_IGN);
	if (pipe(pfd)<0) {
		perror("ipconfig");
		exit(1);
	}
	if(ioctl(pfd[0], FIOPUSHLD, &ip_ld) < 0){
		perror("PUSHLD");
		exit(1);
	}
	if(ioctl(pfd[1], FIOPUSHLD, &ip_ld) < 0){
		perror("PUSHLD");
		exit(1);
	}
	if(ioctl(pfd[0], IPIOLOCAL, &myaddr) < 0){
		perror("IPIOLOCAL");
		exit(1);
	}
	if(ioctl(pfd[0], IPIOHOST, &myaddr) < 0){
		perror("IPIOHOST");
		exit(1);
	}
	if(ioctl(pfd[1], IPIOLOCAL, &myaddr) < 0){
		perror("IPIOLOCAL");
		exit(1);
	}
	if(ioctl(pfd[1], IPIOHOST, &myaddr) < 0){
		perror("IPIOHOST");
		exit(1);
	}
	pause();	/* forever, hopefully */
}
