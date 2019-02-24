#include <stdio.h>
#include <signal.h>
#include <sys/pioctl.h>

main(argc,argv)
int argc; char **argv;
{
	int pfd;
	char procnam[16];
	long mask = (1<<(SIGSTOP-1));
	FILE *ttyerr;

	ttyerr = fopen("/dev/tty", "w");
	if (argc <= 1) {
		if( ttyerr )
			fprintf(ttyerr,"Usage: %s cmd [args...]\n",*argv);
		exit(1);
	}
	sprintf(procnam,"/proc/%05d",getpid());
	if ((pfd=open(procnam,0)) < 0) {
		if( ttyerr )
			fprintf(ttyerr,"cannot open %s\n",procnam);
		exit(1);
	}
	ioctl(pfd,PIOCSMASK,&mask);
	ioctl(pfd,PIOCSEXEC,0);
	close(pfd);
	if( ttyerr ){
		fprintf(ttyerr,"%s\n",procnam);
		fclose(ttyerr);
	}
	execvp(argv[1], argv+1);
	perror(argv[1]);
	exit(1);
}
