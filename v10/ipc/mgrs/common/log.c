#include <sys/types.h>
#include <stdio.h>
#include <ipc.h>
#include <libc.h>
#include "defs.h"

static char buf[1024];
static char logname[128];
static int nolog;

openlog(mtpt)
	char *mtpt;
{
	int fd;
	char *cp;

	/* set up log file */
	if ((cp=strrchr(mtpt, '/'))!=NULL)
		mtpt = cp+1;
	sprintf(logname, "/usr/ipc/log/%s", mtpt);
	fd = open(logname, 1);
	if (fd<0 && errno==ENOENT)
		fd = creat(logname, 0666);
	if (fd<0) {
		fprintf(stderr, "cannot open log file, %s\n", logname);
		nolog=1;
		close(2);
		return;
	}
	dup2(fd, 2);
	close(fd);
	fd = open("/dev/console", 1);
	dup2(fd, 3);
	close(fd);
}

/* logging events */
logevent(format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
	char *format;
{
	long now=time((long *)0);
	char msg[1024];

	if(nolog)
		return;
	
	sprintf(msg, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	sprintf(buf, "%.15s %s", ctime(&now)+4, msg);
	lseek(2, 0L, 2);
	write(2, buf, strlen(buf));
}

/* logging events on both log and console */
logconsole(format, a1, a2, a3, a4, a5, a6)
	char *format;
{
	long now=time((long *)0);
	char msg[1024];
	int len;

	if(nolog)
		return;
	sprintf(msg, format, a1, a2, a3, a4, a5, a6);
	sprintf(buf, "%.15s %s", ctime(&now)+4, msg);
	len = strlen(buf);
	lseek(2, 0L, 2);
	write(2, buf, len);
	write(3, buf, len);
}

/* log call status */
logstatus(msg, ip)
	char *msg;
	ipcinfo *ip;
{
	long now=time((long *)0);

	if(nolog)
		return;
	if(errno){
		sprintf(buf, "%.15s %s reject %s(%s!%s) from %s %s (%d) %s\n",
			ctime(&now)+4, msg, (ip->flags&IPC_CREAT)?"creat":"open",
			ip->myname, ip->name, ip->machine, ip->user, errno,
			errstr);
	} else {
		sprintf(buf, "%.15s %s accept %s(%s!%s) from %s %s\n",
			ctime(&now)+4, msg, (ip->flags&IPC_CREAT)?"creat":"open",
			ip->myname, ip->name, ip->machine, ip->user);
	}
	lseek(2, 0L, 2);
	write(2, buf, strlen(buf));
}

logtime(msg)
	char *msg;
{
#include <sys/timeb.h>
	struct timeb tb;

	if(nolog)
		return;
	ftime(&tb);
	sprintf(buf, "%d %s", tb.time*1000L + tb.millitm, msg);
	lseek(2, 0L, 2);
	write(2, buf, strlen(buf));
}
