#include	<signal.h>
#include	<libc.h>
#include	"backup.h"

char buf[1000];
char bdir[FNAMELEN];

main(argc, argv)
	char **argv;
{
	int ifd, ofd, n;
	char *s;

	signal(SIGPIPE, SIG_IGN);
	backupdir(bdir);
	for(s = bdir; *s++;)
		;
	if(rx(s, "bkpmunge", (char *)0, "light", &ifd, &ofd))
		exit(1);
	sleep(5);
	n = read(ifd, buf, sizeof buf);
	if(n < 0){
		perror("startmunge read");
		exit(1);
	} else if(n){
		do {
			write(1, buf, n);
		} while((n = read(ifd, buf, sizeof buf)) > 0);
		exit(1);
	}
	exit(0);
}
