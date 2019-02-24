/*	/sccs/src/cmd/uucp/s.utility.c
	utility.c	1.1	8/30/84 17:37:47
*/
#include "uucp.h"
VERSION(@(#)utility.c	1.1);

static void logError();

#define TY_ASSERT	1
#define TY_ERROR	2

/*
 *	produce an assert error message
 * input:
 *	s1 - string 1
 *	s2 - string 2
 *	i1 - integer 1 (usually errno)
 *	sid - sccsid string of calling module
 *	file - __FILE of calling module
 *	line - __LINE__ of calling module
 */
void
assert(s1, s2, i1, sid, file, line)
char *s1, *s2, *file, *sid;
{
	logError(s1, s2, i1, TY_ASSERT, sid, file, line);
}


/*
 *	produce an assert error message
 * input: -- same as assert
 */
void
errent(s1, s2, i1, sid, file, line)
char *s1, *s2, *sid, *file;
{
	logError(s1, s2, i1, TY_ERROR, sid, file, line);
}

#define EFORMAT	"%sERROR (%.9s)  pid: %d (%s) %s %s (%d) [SCCSID: %s, FILE: %s, LINE: %d]\n"

static
void
logError(s1, s2, i1, type, sid, file, line)
char *s1, *s2, *sid, *file;
{
	register FILE *errlog;
	char text[BUFSIZ];
	int pid;

	if (Debug)
		errlog = stderr;
	else {
		errlog = fopen(ERRLOG, "a");
	}
	if (errlog == NULL)
		return;
	(void) chmod(ERRLOG, 0666);

	pid = getpid();

	(void) fprintf(errlog, EFORMAT, type == TY_ASSERT ? "ASSERT " : " ",
	    Progname, pid, timeStamp(), s1, s2, i1, sid, file, line);

	(void) fclose(errlog);
	(void) sprintf(text, " %sERROR %.100s %.100s (%.9s)",
	    type == TY_ASSERT ? "ASSERT " : " ",
	    s1, s2, Progname);
	if (type == TY_ASSERT)
	    systat(Rmtname, SS_ASSERT_ERROR, text, Retrytime);
	return;
}


/* timeStamp - create standard time string
 * return
 *	pointer to time string
 */

char *
timeStamp()
{
	register struct tm *tp;
	time_t clock;
	static char str[20];

	(void) time(&clock);
	tp = localtime(&clock);
	(void) sprintf(str, "%d/%d-%d:%2.2d:%2.2d", tp->tm_mon + 1,
	    tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
	return(str);
}
