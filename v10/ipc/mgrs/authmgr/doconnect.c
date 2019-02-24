#include <stdio.h>
#include <signal.h>
#include <ipc.h>
#include <string.h>

#include "auth.h"

#define RLOGIN	"/usr/bin/rlogin"

int callee = -1;
int cpid = -1;

int
doconnect(path, user)
	char *path;
	char *user;
{
	char *cp;
	int status;


	if (strncmp(path, "tcp!", 4) != 0) {
		char *svc;		/* this service stuff is a hack */ 
		if ((strchr(path, '/') == NULL) ||
		    (strncmp(path, "phone/", 6) == 0) ||
		    (strncmp(path, "homer/", 6) == 0) ||
		    (strncmp(path, "mercury/", 8) == 0))
			svc = "dcon";	/* dcon requires no passwd on V10 unix */
		else
			svc = "";
		restoretty();
		redial(path, user, svc);
		settty();
		return FALSE;
	}

	signal(SIGALRM, SIG_IGN);
	switch (cpid=fork()) {
	case -1:
		perror("Fork error");
		return FALSE;
	case 0:
		close(2);
		dup(1, 2);
		setlogname(user);
		execl(RLOGIN, RLOGIN, path, NULL);
		perror("exec failed");
		exit(100);
	default:
		/* do we need a timeout? */
		wait(&status);
		if (((status >> 8) & 0xFF) != 0) {
			
			return FALSE;
		}
		return TRUE;
	}
}

redial(path, user, svc)
char *path, *user, *svc;
{
	ipcinfo	info;
	char buf[100];

	/* The options are U (for gnots) and - (for us) */

	strcpy(buf, "->");
	strcat(buf, path);

	strcpy(buf, "->");
	if (strncmp(path, "dk!", 3) == NULL)
		path += 3;
	strcat(buf, path);

	info.rfd = -1;
	info.cfd = 0;	/* pass back fd 0 */
	info.myname = NULL;
	info.user = user;
	info.machine = "";
	info.uid = info.gid = -1;
	info.name = ipcpath(buf, "/cs/dk", svc);
	info.param = "light";				/* bogus */
	info.flags = 0;
	if(ipcredial(&info) != 0) {
		fprintf(stderr, "Redial failed, destination was %s\n", info.name);
	}
}


#ifdef notused
int
dkcall(path)
char *path;
{
	char *fullpath, *service;

	fullpath = ipcpath(path, "dk", "dcon");
	service = strrchr(fullpath, "!");
	service++;
	
	if (strcmp(service, "dcon") == 0) {	/* try a dcon */
		char answer[2];

		callee = ipcopen(fullpath, "light");
		if (callee >= 0) {
			read(callee, answer, 2);
			if (strcmp(answer, "OK") == 0)
				return callee;		/* dcon likes us */
		}
	}

	fullpath = ipcpath(path, "dk", "");
	callee = ipcopen(fullpath, "light");
	return callee;
}
#endif
