/*	/sccs/src/cmd/uucp/s.gwd.c
	gwd.c	1.3	8/30/84 17:37:28
*/
#include "uucp.h"
VERSION(@(#)gwd.c	1.3);

/*
 *	gwd - get working directory
 *	Uid and Euid are global
 *	return
 *		0 - ok
 *	 	FAIL - failed
 */

gwd(wkdir)
char *wkdir;
{
	FILE *fp;
	char cmd[BUFSIZ];

	*wkdir = '\0';
	(void) sprintf(cmd, "%s pwd 2>&-", PATH);

#ifndef V7
	(void) setuid(Uid);
	fp = popen(cmd, "r");
	(void) setuid(Euid);
#else
	fp = popen(cmd, "r");
#endif

	if (fp==NULL)
		return(FAIL);
	if (fgets(wkdir, MAXFULLNAME, fp) == NULL) {
		(void) pclose(fp);
		return(FAIL);
	}
	if (wkdir[strlen(wkdir)-1] == '\n')
		wkdir[strlen(wkdir)-1] = '\0';
	(void) pclose(fp);
	return(0);
}


/*
 * uidstat(file, &statbuf)
 * This is a stat call with the uid set from Euid to Uid.
 * Used from uucp.c and uux.c to permit file copies
 * from directories that may not be searchable by other.
 * return:
 *	same as stat()
 */

#ifndef uidstat

int
uidstat(file, buf)
char *file;
struct stat *buf;
{
#ifndef V7
	register ret;

	(void) setuid(Uid);
	ret = stat(file, buf);
	(void) setuid(Euid);
	return(ret);
#else
	int	ret;
	register pid, rpid;

	if ((pid = vfork()) == 0) {
		(void) setuid(Uid);
		_exit(stat(file, buf));
	}
	ret = 1;
	while ((rpid = wait(&ret)) != pid && rpid != -1)
		;
	return (rpid == -1 ? 1 : ret);
#endif
}
#endif
