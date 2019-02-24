#include	<signal.h>
#include	<libc.h>

char buf[1000];

main(argc, argv)
	char **argv;
{
	int ifd, ofd, n;

	if(argc != 2){
		fprint(2, "usage: startclient ipc-name\n");
		exit(1);
	}
	signal(SIGPIPE, SIG_IGN);
	if(rx(argv[1], "bkpclient", (char *)0, "light", &ifd, &ofd))
		exit(1);
	sleep(5);
	n = read(ifd, buf, sizeof buf);
	if(n < 0){
		perror("startclient read");
		exit(1);
	} else if(n){
		do {
			write(1, buf, n);
		} while((n = read(ifd, buf, sizeof buf)) > 0);
	}
	exit(0);
}
