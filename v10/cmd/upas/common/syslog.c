/*
 * A first-shot at a syslog implementation on v10.  Quite crude.
 */

#include <stdio.h>
#include <libc.h>
#include <errno.h>
#include "mail.h"

static char logname[128];
static int fd = -1;
static char id[67];
static int pid;
static char machine[64] = "<hostname>";
static logmask = -1;

openlog(name, flags, logord)
	char *name;
	int flags, logord;
{
	char *cp;
	int hnfd;

	/* set up log file */
	if ((cp=strrchr(name, '/'))!=NULL)
		name = cp+1;
	sprintf(logname, "%s%s.log", SMTPQROOT, name);
	fd = open(logname, 1);
	if (fd<0 && errno==ENOENT)
		fd = creat(logname, 0666);
	if (fd<0) {
		fprintf(stderr, "cannot open log file, %s\n", logname);
		close(2);
		logmask = -1;
		return;
	}
	strcpy(id, name);
	pid = getpid();
#ifdef NOTDEF
/* real syslog has machine name here */
	hnfd = open("/etc/whoami", 0);
	if (hnfd > 0) {
		char *cp;
		read(hnfd, machine, sizeof(machine));
		if ((cp = strchr(machine, '\n')) != NULL)
			*cp = '\0';
		close(hnfd);
	}
#endif
	logmask = LOG_UPTO(DEFAULT_LOG_LEVEL);
}

syslog(level, format, a1, a2, a3, a4, a5, a6, a7, a8 ,a9, a10)
	char *format;
{
	long now=time((long *)0);
	char msg[20000], buf[20000];

	if(((1<<level) & logmask) == 0)
		return;
	
	sprintf(msg, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
#ifdef NOTDEF
	sprintf(buf, "%.15s %s %s[%d]: %s", ctime(&now)+4,
		machine, id, pid, msg);
#endif
	sprintf(buf, "%.15s %s[%d]: %s", ctime(&now)+4,
		id, pid, msg);
	if (buf[strlen(buf) - 1] != '\n')
		strcat(buf, "\n");
	lseek(fd, 0L, 2);
	write(fd, buf, strlen(buf));
}

setlogmask(mask)
int mask;
{
	int old = logmask;

	if (logmask == -1)
		return old;
	logmask = mask;
	return old;
}
