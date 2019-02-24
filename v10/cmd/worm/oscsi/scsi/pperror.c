#include	"../scsi.h"

void
pperror(char *buf, char *mesg)
{
	extern int sys_nerr;
	extern char *sys_errlist[];
	extern int errno;

	if((errno < 0) || (errno >= sys_nerr))
		sprintf(buf, "%s: unknown errno %d", mesg, errno);
	else
		sprintf(buf, "%s: %s", mesg, sys_errlist[errno]);
}
