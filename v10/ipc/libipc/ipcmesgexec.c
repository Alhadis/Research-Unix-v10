#include <ipc.h>
#include "defs.h"

extern int mesg_ld;

/*
 *  Perform a remote execution of a command with message line discipline.
 */
ipcmesgexec(name, param, cmd)
	char *name;
	char *param;
	char *cmd;
{
	int fd, len;
	char ds[PATHLEN];

	strcpy(ds, name);
	strcat(ds, "!");
	strcat(ds, "mesgexec");
	fd = ipcopen(ds, param);
	if (fd<0)
		return -1;
	if (ipclogin(fd)<0) {
		close(fd);
		return -1;
	}
	len = strlen(cmd)+1;
	if (write(fd, cmd, len)!=len) {
		errstr = "write error";
		close(fd);
		return -1;
	}
	return fd;
}
