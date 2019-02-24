#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dir.h>
#include "defs.h"
#include "load.h"

/* globals for mail scanning */
FILE *mlfp = NULL;
struct stat s;
struct stat os;
int doscan = FALSE;
char *user[NOFILE];

/* imported */
extern int errno;

/* initialize */
init()
{
	initload();
}

/* return my name */
char *
myname()
{
	return ("mon");
}

/* send the current information to a client */
sendinfo (fd)
int fd;
{
	char *p, *strchr();
	char buf[128];
	char line[128];
	char token[128];
	int len, n;

	/* send the load */
	sprintf(buf, "load %f time %d vec %d %d %d %d %d\n",
		load.l_runq, load.l_time, load.l_cp[0], load.l_cp[1],
		load.l_cp[2], load.l_cp[3], load.l_cp[4]);
	len = strlen(buf);
	if (write (fd, buf, len) != len)
		return (-1);

	/* check for mail if its time */
	strcpy(token, "delivered ");
	strcat(token, user);
	strcat(token, " From ");
	n = strlen(token);
	if (doscan) {
		if (fseek(mlfp, os.st_size, 0) < 0) {
			os.st_ino = 0;
			return 0;
		}
		while (fgets(line, sizeof(line), mlfp)!=NULL) {
			if (strncmp(token, line, n)!=0)
				continue;
			p = strchr(&line[n], ' ');
			if (p==NULL)
				continue;
			*p = '\0';
			sprintf(buf, "from %s\n", &line[n]);
			len = strlen(buf);
			if (write (fd, buf, len) != len)
				return (-1);
		}
	}
	return (0);
}

/* generate the current information */
generate()
{
	long now;
	static long lastscan;
#define MAILNAME "/usr/spool/mail/mail.log"

	/* get load info */
	genload ();

	/* find out if we should scanmn the mail */
	now = time (NULL);
	doscan = (now - lastscan) > 60;
	if (doscan) {
		os = s;
		lastscan = now;
		if (stat(MAILNAME, &s)<0) {
			doscan = FALSE;
		} else if (os.st_dev != s.st_dev || os.st_ino != s.st_ino) {
			if (mlfp)
				fclose(mlfp);
			mlfp = fopen (MAILNAME, "r");
			os.st_size = 0;
		} else if (os.st_size == s.st_size) {
			doscan = FALSE;
		}
	}
}

/* add a client */
void
add (fd, who)
	int fd;
	char *who;
{
	char *strdup();

	if (user[fd])
		free(user[fd]);
	user[fd] = strdup(who);
}

/* drop a client */
void
drop (fd)
{
}
