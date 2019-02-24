#include <stdio.h>
#include <string.h>
#include <signal.h>
#if research
#include <ipc.h>
#include <sys/ttyio.h>
#endif

int ppid;
void abortpass();
char * ipcpath();

/*
 *  Connect to given datakit port
 */
main(argc, argv)
	char *argv[];
{
	int gatefd;
	int ppid, cpid;
	char *cp;

	if (argc != 4) {
		fprintf(stderr, "usage: %s destination network service\n", argv[0]);
		exit(1);
	}
	if ((gatefd = ipcopen((cp = ipcpath(argv[1], argv[2], argv[3])), "heavy tcphup hup")) < 0){
#if research
		fprintf(stderr, "ipcopen(%s):%s\n", cp, errstr);
#endif
		exit(2);
	}

	/*
	 * Initialize the input fd, and copy bytes.
	 */

	ppid = getpid();
	switch(cpid = fork()){
	case -1:
		perror("fork error");
		exit(10);
	case 0:
		close(0);
		pass(gatefd, 1);	/* child - from remote */
		close(1);
#if !research
		shutdown(gatefd, 0);
#endif
		close(gatefd);
/*		fprintf(stderr, "from remote - exiting\n"); */
		exit(0);
	default:
		close(1);
		pass(0, gatefd);	/* parent - to remote */
		close(0);
#if !research
		shutdown(gatefd, 1);
#endif
		close(gatefd);
/*		fprintf(stderr, "to remote - exiting\n"); */
		while(wait(0) != cpid);
		exit(0);
	}
}

void
rdtimout()
{
/*	fprintf(stderr, "read timeout\n"); */
	exit(0);
}

#define RDTIMOUT 10
#define SIGALRM 14

pass(from, to)
	int from, to;
{
	char buf[1024];
	int n; 

	signal(SIGALRM, rdtimout);
	while (alarm(RDTIMOUT),((n=read(from, buf, sizeof(buf))) > 0)) {
		if (write(to, buf, n)!=n) {
			perror("pass write error");
			exit(6);
		}
	}
	alarm(0);
}
