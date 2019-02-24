#include <stdio.h>
#include <dk.h>
#include <string.h>
#include <sys/types.h>

/*
 * program to run a command
 * on another cpu on Datakit
 */

#define MAXCHARS 8192
char	args[MAXCHARS];
char	buf[4096];
extern	char *dkerror;
char	*bldargs(), *malloc();
extern	errno;
fd_set	rbits, bits;

#define	MAXEOF	10

main(argc, argv)
char **argv;
{
	char *host;
	char *service = "exec";
	char *cp;
	register int rem, n;
	register int neof;

	host = argv[0];
	if (strrchr(host, '/'))
		host = strrchr(host, '/')+1;
	if(strcmp(host, "rx")==0 || strcmp(host, "rexec")==0){
		host = argv[1];
		argv++;
		argc--;
	}
	if (cp = strchr(host, '.')) {
		service = cp+1;
		cp = malloc(strlen(host)+1);
		strcpy(cp, host);
		host = cp;
		*strchr(host, '.') = '\0';
	}
	else if(argc<=1){
		execl("/usr/bin/dcon", "dcon", host, 0);
		execl("/bin/dcon", "dcon", host, 0);
		fprintf(stderr, "rx: cannot exec dcon\n");
		exit(1);
	}
	rem = _tdkexec(host, bldargs(argc-1, &argv[1]), service);
	if (rem<0) {
		fprintf(stderr, "rx: call to %s failed: %s\n", host, dkerror);
		exit(1);
	}
	FD_SET(0, bits);
	FD_SET(rem, bits);
	neof = 0;
	for (;;) {
		rbits = bits;
		if (select(rem+1, &rbits, (fd_set *)NULL, 20000) < 0)
			exit(1);
		if (FD_ISSET(0, rbits)) {
			n = read(0, buf, sizeof(buf));
			if (n > 0) {
				if (write(rem, buf, n) != n)
					exit(1);
				neof = 0;
			}
			else {
				if (++neof >= MAXEOF)
					FD_CLR(0, bits);
				write(rem, buf, 0);
			}
		}
		if (FD_ISSET(rem, rbits)) {
			n = read(rem, buf, sizeof(buf));
			if (n <= 0)
				exit(0);
			if (write(1, buf, n) != n)
				exit(1);
		}
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
