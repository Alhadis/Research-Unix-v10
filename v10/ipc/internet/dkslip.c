#include <stdio.h>
#include <sgtty.h>
#include <signal.h>

main(argc, argv)
char *argv[];
{
	char buf[512];
	int n, pid, cpid, ld, ptfd, rem;
	extern cdkp_ld;

	if(argc != 3){
		fprintf(stderr, "Usage: %s dk-host pt\n", argv[0]);
		exit(1);
	}
	if((ptfd = open(argv[2], 2)) < 0){
		perror(argv[2]);
		exit(1);
	}
	rem = tdkdial(argv[1], 0);
	if(rem < 0){
		perror("tdkdial");
		exit(1);
	}
	if(dkproto(rem, cdkp_ld) < 0){
		perror("dkproto cdkp");
		exit(1);
	}

	if(read(rem, buf, 1) != 1){
		perror("init read");
		exit(1);
	}
	write(rem, "slip\r", 5);

	sleep(3);
/*
	read(rem, buf, sizeof(buf));
*/
	write(rem, "forbes sl0\r", 11);

	signal(SIGHUP, SIG_IGN);
	pid = getpid();
	cpid = fork();
	if(cpid == 0){
		rint(rem, ptfd);
		kill(pid, 9);
		exit(0);
	}
	out(ptfd, rem);
	kill(cpid, 9);
}
#define END 0300
#define ESC 0333
#define TRANS_END 0334
#define TRANS_ESC 0335
rint(rfd, wfd)
{
	static escaped = 0;
	int count = 0, len;
	unsigned int c;
	unsigned char buf[4096];
	unsigned char xbuf[512];
	int i, n;

	len = 0;
	while((n = read(rfd, xbuf, sizeof(xbuf))) > 0)
	for(i = 0; i < n; i++){
		c = xbuf[i];
		if(escaped){
			if(c == TRANS_ESC){
				c = ESC;
			} else {
				c = END;
			}
			escaped = 0;
		} else {
			if(c == ESC){
				escaped = 1;
				continue;
			} else if(c == END){
				if(write(wfd, buf, len) != len){
					perror("write");
					return;
				}
				escaped = count = len = 0;
				continue;
			}
		}
		buf[len++] = c;
	}
}

out(rfd, wfd)
{
	char buf[4096];
	char xbuf[5000];
	int xi;
	int n;
	unsigned char c;
	int i;

	while((n = read(rfd, buf, sizeof(buf))) > 0){
		xi = 0;
		for(i = 0; i < n; i++){
			c = buf[i];
			if(c == ESC || c == END){
				xbuf[xi++] = '\333';
				c = (c == ESC ? TRANS_ESC : TRANS_END);
			}
			xbuf[xi++] = c;
		}
		xbuf[xi++] = '\300';
		write(wfd, xbuf, xi);
	}
}
