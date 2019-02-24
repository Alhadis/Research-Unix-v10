/*
 *	getlog() -- paranoid version of getlogin.
 *	Unnecessary but harmless in V9; may be essential in Sys V
 *		with botched layers implementations
 *
 *	Try getlogin().
 *	If that fails, look in the password file.
 *	if that fails, give up.
 */

#include <pwd.h>

extern int getuid();
extern char *getlogin(), *getenv();

#define NULL 0

char *
getlog()
{
	char *p;
	struct passwd *pw;

	if ((p = getlogin()) != NULL)
		return p;
	
	/* If LOGNAME is set, and it matches getuid(), use it */
	p = getenv ("LOGNAME");
	if (p != NULL && *p != '\0') {
		pw = getpwnam(p);
		if (pw != NULL && pw->pw_uid == getuid())
			return p;
	}

	/* Try to get the password file entry for getuid() */
	if ((pw = getpwuid(getuid())) != NULL)
		return pw->pw_name;
	
	/* Give up */
	return NULL;
}
