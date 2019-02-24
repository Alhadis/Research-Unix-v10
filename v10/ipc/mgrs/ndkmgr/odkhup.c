#include <stdio.h>
#include <sys/dkio.h>
#include <signal.h>
#include <ctype.h>

char buf[64] ;
char *netfiles = "/dev/dk/dk";

main(argc, argv)
char **argv ;
{
	int f, i;
	int n ;
	int traffic = 2;
	int unit = 'a';
	extern rdk_ld;


	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	while (argc > 1) {
		if (strcmp(argv[1], "-t") == 0)
			;
		else if (isdigit(*argv[1]))
			unit = *argv[1];
		else if (strcmp(argv[1], "-N") == 0) {
			netfiles = argv[2];
			argv++;
			argc--;
		}
		else
			printf("dkhup: argument %s ignored\n");
		argc--;
		argv++;
	}
	if (fork())
		return(0) ;
restart:
	if ((f = dkctlchan(traffic, unit)) < 0) {
		printf("dkhup: cannot open ctlr channel for %d\n", traffic);
		exit(1);
	}
	if (argc < 3) {
		ioctl(f, DIOCHUP, 0);
		ioctl(f, DIOCHUP, 0);
		ioctl(f, DIOCHUP, 0);
	}
OK:
	ioctl(f, DIOCNXCL, 0);
	for (;;) {
		n = read(f, buf, sizeof buf) ;
		if (n <= 0) {
			for (i=0; i < 5; i++) {
				sleep(1);
				n = read(f, buf, sizeof(buf));
				if (n > 0)
					goto OK;
			}
			printf("dkhup: shutting down kmc %d\n", traffic);
			ioctl(f, KIOCSHUT, (char *)0);
			close(f);
			sleep(45);
			printf("dkhup: reloading kmc %d\n", traffic);
			dumpkmc(traffic);
			loadkmc(traffic);
			sleep(5);
			goto restart;
		}
/*
		printf("dkhup: read (size %d) = ", n) ;
		for (i=0; i<n; i++)
			printf("%o ", buf[i]&0377) ;
		printf("\n\r") ;
*/
	}
}

dumpkmc(traffic)
{

	sprintf(buf, "cd /usr/dk; /etc/kmcdump %d", traffic);
	system(buf);
}

loadkmc(traffic)
{

	sprintf(buf, "/etc/kdiload %d", traffic);
	system(buf);
}
