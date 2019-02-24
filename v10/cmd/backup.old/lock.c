#include	"backup.h"
#include	<libc.h>

static int lockfd = -1;

lock(dir)
	char *dir;
{
	char buf[256];
	long done;

	if(lockfd >= 0){
		fprint(2, "double lock!!\n");
		abort();
	}
	sprint(buf, "%s/%s", dir, LOCK);
	if(access(buf, 0) < 0)
		execl("/usr/backup/bin/mklock", "mklock", 0);
	done = 3600L*12 + time((long *)0);		/* 12 hours */
	while(time((long *)0) < done){
		if((lockfd = open(buf, 1)) >= 0)
			break;
		sleep(3);
	}
	if(lockfd < 0){
		fprint(2, "lock timeout on '%s'\n", buf);
		logprint("+lock timeout on '%s'\n", buf);
		exit(5);
	}
	fprint(lockfd, "%6.6d\n", getpid());
}

unlock()
{
	if(lockfd < 0){
		fprint(2, "unlock: lockfd < 0\n");
		exit(6);
	}
	lseek(lockfd, 0L, 0);
	fprint(lockfd, "%6s", "");
	close(lockfd);
	lockfd = -1;
}
