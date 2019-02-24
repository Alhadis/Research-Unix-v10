#include <stdio.h>
#include <ipc.h>
#include <sysexits.h>

extern int ipcdebug;

/*
 *  dial a name and return an fd to the connection
 */
int
dk_connect(dest, param)
char *dest, *param;
{
	ipcinfo info;
	int ret;
	char fulldest[256];	/* stub - limit this */

	strcpy(fulldest, "/cs/dk!");
	strcat(fulldest, dest);

	info.rfd = info.cfd = -1;
	info.myname = info.user = info.machine = NULL;
	info.uid = info.gid = -1;
	info.name = fulldest;
	info.param = param;
	info.flags = IPC_OPEN;
	ret = ipcdial(&info);
	if (ret < 0) {
		if (ipcdebug != 0) {
			perror("mipsdk connect error");
			fprintf(stderr, "mips ipcdial returned %d, %d\n",
				ret, errno);
			fprintf(stderr, "myname=%s user=%s name=%s param=%s\n",
				info.myname, info.user, info.name, info.param);
			fprintf(stderr, "rfd=%d cfd=%d uid=%d gid=%d flags=%d\n",
				info.rfd, info.cfd, info.uid, info.gid,
				info.flags);
			fprintf(stderr, "machine=%s\n", info.machine);
		}
		ipcseterror(EX_IOERR, "dk_connect",
			"can't connect to machine/service");
	}
	return ret;
}

int
dk_close(fd)
int fd;
{
	write(fd, &fd, 0);	/* stub - is this useful or necessary? */
	return close(fd);
}
