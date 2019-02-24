#include	<fio.h>
#include	<string.h>
#include	<signal.h>
#include	"backup.h"
#include	<ipc.h>
#include	<errno.h>

char *doprint(), *getenv();
char bdir[FNAMELEN];
char logfile[FNAMELEN];
char source[FNAMELEN];
int bye = 0;

static char *dateit();

main()
{
	register fd, n;
	ipcinfo *ip;
	int nfd;
	char buf[256];
	char *s;

	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	chdir("/tmp");
	for(n = 1; n < 128; n++)
		close(n);
	backupdir(bdir);
	sprint(logfile, "%s/%s", bdir, LOG);
	for(n = 0; n < 10; n++){
		if((fd = ipccreat("/cs/backuplog", 0)) >= 0)
			break;
		sleep(5);
	}
	if(fd < 0){
		write(0, "Error", 1);
		sprint(buf, "logger couldn't do the ipccreat: %s", errstr);
		addlog(dateit(buf));
		exit(1);
	}
	write(0, "Okay", 1);
	close(0);
	if(s = getenv("CSOURCE")){
		while(*s){
			if(strncmp(s, "source=", 7) == 0){
				strcpy(source, s+7);
				if(s = strchr(source, ' '))
					*s = 0;
				break;
			}
			s++;
		}
	} else
		source[0] = 0;
	while(ip = ipclisten(fd)){
		nfd = ipcaccept(ip);
		Finit(nfd, (char *)0);
		if(s = Frdline(nfd))
			addlog(s);
		if(write(nfd, "Okay", 1) != 1){
			sprint(buf, "write failed to %s!%s, errno=%d\n",
				ip->machine, ip->user, errno);
			addlog(dateit(buf));
		}
		sleep(1);
		close(nfd);
		if(bye)
			exit(0);
	}
	addlog(dateit("backuplog: lost the announcement connection somehow"));
	exit(2);
}

addlog(s)
	char *s;
{
	int fd;

	if(s[strlen(s)-1] == '@'){
		char buf[256];

		sprint(buf, "%: logger exiting", s);
		addlog(buf);
		bye = 1;
		return;
	}
	if((fd = open(logfile, 1)) >= 0){
		lseek(fd, 0L, 2);
		if(s[25] == '+'){
			s[25] = 0;
			fprint(fd, "%s%s: %s\n", s, source, s+26);
		} else
			fprint(fd, "%s\n", s);
		close(fd);
	} else
		perror(logfile);
}

static char *
dateit(s)
	char *s;
{
	long t;
	char *out, *ctime();
	static char buf[512];

	(void)time(&t);
	out = ctime(&t);
	out[strlen(out)-1] = 0;
	sprint(buf, "%s %s\n", out, s);
	return(buf);
}
