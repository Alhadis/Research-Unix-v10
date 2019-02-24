/* needed for unresolved references (see ../mbox)
	from res_err.c & ultrix_err.c ..  wmb */

#include <stdio.h>
#include <errno.h>
#include <sysexits.h>
#include <string.h>

/*
 * A known place to keep a copy of the system-supplied
 * error message.
 */
int syserrno;
char syserrstr[128];

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


extern char *sys_errlist[ ];
extern int sys_nerr;

extern int ipcdebug;

/*
 * Copy the actual OS-supplied errno and string in a known
 * place.
 */
ipcgetsyserr()
{
	syserrno = errno;
	if (ipcdebug)
		fprintf(stderr, "ipcgetsyserr: errno=%d\n", syserrno);
	if (errno < sys_nerr)
		strcpy(syserrstr, sys_errlist[errno]);
	else
		sprintf(syserrstr, "Unknown error number %d", errno);
}

/*
 * Dummy gethostname (hostname supplied at gateway)
 */
int
gethostname(hostname,namelen)
char *hostname;
int namelen;
{
	*hostname = '\0';
	return 0;
}
