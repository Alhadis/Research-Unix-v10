#include <stdio.h>
#include <dk.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <libc.h>
#include <ipc.h>

/*
 * program to run a command
 * on another cpu on Datakit
 */

#define MAXCHARS 8192
char	args[MAXCHARS];
char	buf[4096];
char	*bldargs(), *malloc();
extern	errno;

#define	MAXEOF	10

main(argc, argv)
char **argv;
{
	char *host, *av0;
	int ignore0=0;
	int pid, rv;
	register int rem;

	rem = -1;
	av0 = host = argv[0];
	if (strrchr(av0, '/'))
		av0 = host = strrchr(av0, '/')+1;
	if(strcmp(host, "rx")==0 || strcmp(host, "rsh")==0){
		if(argc<2){
			fprintf(stderr, "usage: %s [-n] host cmd\n", av0);
			exit(1);
		}
		if(strcmp(argv[1], "-n")==0){
			ignore0 = 1;
			argv++, argc--;
		}
		if(argc<2){
			fprintf(stderr, "usage: %s [-n] host cmd\n", av0);
			exit(1);
		}
		host = argv[1];
		argv++, argc--;
	} else if(argc<=1){
		execl("/usr/bin/con", "con", host, 0);
		fprintf(stderr, "rx: cannot exec con\n");
		exit(1);
	}
	if (strcmp(av0, "rsh")!=0)
		rem = ipcexec(host, "heavy", bldargs(argc-1, &argv[1]));
	if (rem<0)
		rem = ipcrexec(host, "heavy", bldargs(argc-1, &argv[1]));
	if (rem<0) {
		fprintf(stderr, "%s: call to %s failed: %s\n", av0, host, errstr);
		exit(1);
	}
	signal(SIGPIPE, SIG_IGN);	/* sometimes happens unreasonably in EOF handling */

	if(!ignore0){
		switch(pid = fork()){
		case -1:
			perror("can't fork\n");
			exit(1);
		case 0:
			return fromstdin(rem);
		default:
			rv = fromnet(rem);
			kill(pid, 9);
			return rv;
		}
	} else
		return fromnet(rem);
}

fromstdin(rem)
{
	register int neof, n;

	close(1);
	close(2);
	close(3);
	neof = 0;
	for (;;) {
		n = read(0, buf, sizeof(buf));
		if (n > 0) {
			if (write(rem, buf, n) != n)
				return(1);
			neof = 0;
		} else {
			if (++neof >= MAXEOF)
				write(rem, buf, 0);
		}
	}
	/* not reached */
}

fromnet(rem)
{
	register int n;

	close(0);
	close(2);
	close(3);
	for (;;) {
		n = read(rem, buf, sizeof(buf));
		if (n <= 0)
			return(0);
		if (write(1, buf, n) != n)
			return(1);
	}
}

char *
bldargs(argc, argv)
register char **argv;
{
	register char *s = args, *t;

	while(argc--) {
		*s++ = ' ';
		t = *argv++;
		while(*s = *t++) {
			if(s++ >= &args[MAXCHARS-2]){
				fprintf(stderr, "rx: arg list too long\n");
				exit(1);
			}
		}
	}
	*s = '\0';
	return(args+1);
}
