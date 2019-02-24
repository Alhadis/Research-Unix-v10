#include <signal.h>
#include "mail.h"
#include "string.h"
#include "message.h"
#include <sys/stat.h>
#include <utmp.h>

SIGRETURN
ding(s)
	int s;
{
	signal(SIGALRM, ding);
}

/* output message to one terminal */
static void
tell(dev, sender, date)
	char *dev, *sender, *date;
{
	int fd;
	static string *msg=NULL;

	signal(SIGALRM, ding);
	alarm(30);
	if((fd = open(dev, 1))<0)
		return;
	if(msg==NULL)
		msg=s_new();
	s_append(s_restart(msg), "[\007From ");
	s_append(msg, sender);
	s_append(msg, " ");
	s_append(msg, date);
	s_append(msg, "\007]\n");
	write(fd, s_to_c(msg), strlen(s_to_c(msg)));
	alarm(0);
	close(fd);
}

/* loop through all logins */
notify(user, mp)
	char *user;
	message *mp;
{
	int fd;
	struct utmp u;
	char dev[128];
	struct stat sbuf;

	if((fd=open("/etc/utmp", 0))<0)
		return;
	while(read(fd, (char *)&u, sizeof(u))==sizeof(u)) {
		if (strncmp(u.ut_name, user, sizeof(u.ut_name))!=0)
			continue;
		strcpy(dev, "/dev/");
		strncat(dev, u.ut_line, sizeof(u.ut_line));
		dev[sizeof(u.ut_line)+sizeof("/dev/")-1] = '\0';
		if (stat(dev, &sbuf)<0)
			continue;
		if (sbuf.st_mode&S_IEXEC)
			tell(dev, s_to_c(mp->sender), s_to_c(mp->date));
	}
	close(fd);
}
