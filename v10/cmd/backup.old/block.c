#include	<fio.h>
#include	"backup.h"
#include	<signal.h>

main(argc, argv)
	char **argv;
{
	char bdir[FNAMELEN];
	int pid;
	long t, done;

	if(argc != 2){
		fprint(2, "usage: block file\n");
		exit(1);
	}
	backupdir(bdir);
	lock(bdir);
	signal(SIGHUP, SIG_IGN);
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	if(pid)
		exit(0);
	(void)time(&t);
	done = t+3600L*24;	/* 24 hrs should be enough */
	while(access(argv[1], 0) >= 0){
		(void)time(&t);
		if(t >= done){
			fprint(2, "block timeout!\n");
			break;
		}
		sleep(10);
	}
	unlock();
	exit(0);
}
