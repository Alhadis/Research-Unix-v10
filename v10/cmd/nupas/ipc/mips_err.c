/*
	Error codes on SGI MIPS system:

EX_USAGE	64	command line usage error
EX_DATAERR	65	data format error
EX_NOINPUT	66	cannot open input
EX_NOUSER	67	addressee unknown
EX_NOHOST	68	host name unknown:
				unknown service
					gethostbyname if host not found.)
				connection timed out
EX_UNAVAILABLE	69	service unavailable
EX_SOFTWARE	70	internal software error
EX_OSERR	71	system error (e.g., can't fork)
EX_OSFILE	72	critical OS file missing
EX_CANTCREAT	73	can't create (user) output file
EX_IOERR	74	input/output error
EX_TEMPFAIL	75	temp failure; user is invited to retry
				Connection refused (code returned by
EX_PROTOCOL	76	remote error in protocol
EX_NOPERM	77	permission denied
*/

#include <errno.h>
#include <stdio.h>
#include <sysexits.h>
#include <string.h>

extern char *sys_errlist[ ];
extern int sys_nerr;
extern int errno;

extern int ipcdebug;

/*
 * A known place to keep a copy of the system-supplied
 * error message.
 */
int syserrno;
char syserrstr[128];


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
 * map the system error in syserrno into one of the
 * errors in sysexits.h
 */
int
ipcmaperror()
{
	switch (syserrno) {
	case EAGAIN:
	case ENFILE:
	case EMFILE:
	case ENOSPC:
	case ENETDOWN:
	case ENETUNREACH:
	case ENETRESET:
	case ECONNABORTED:
	case ECONNRESET:
	case ENOBUFS:
	case ETIMEDOUT:
	case ECONNREFUSED:
				return EX_TEMPFAIL;
	case EHOSTDOWN:
	case EHOSTUNREACH:
				return EX_NOHOST;

	default:		return EX_OSERR;
	}
}
