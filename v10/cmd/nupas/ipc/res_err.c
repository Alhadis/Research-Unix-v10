#include <stdio.h>
#include <errno.h>
#include <sysexits.h>
#include <string.h>
#include <ipc.h>

/*
 * A known place to keep a copy of the system-supplied
 * error message.
 */
int syserrno;
char syserrstr[128];

int ipcdebug = 0;
int ipcerrno = 0;

extern char *ipcpath();	/* make sure these are loaded in */
extern int ipcopen();


/*
 * globals for ipc
 */
ipcperror(mess)
char *mess;
{
	syserrno = errno;
	strcpy(syserrstr, errstr);
	ipcerrno = ipcmaperror();
	fprintf(stderr, "%s: %s\n", mess, syserrstr);
}

call()
{
	ipcpath();  ipcopen();
}

/*
 * map the system error in syserrno into one of the
 * errors in sysexits.h
 */
int
ipcmaperror()
{
	switch (syserrno) {
	case ENOENT:	return EX_NOHOST;
	case EINTR:	return EX_TEMPFAIL;
	case EACCES:	return EX_UNAVAILABLE;
	case ENODEV:	return EX_TEMPFAIL;
	case EIO:	return EX_TEMPFAIL;

	default:	return EX_OSERR;
	}
}
