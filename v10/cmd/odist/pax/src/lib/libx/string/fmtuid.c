/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * uid number -> name
 */

#include <stdio.h>
#include <hash.h>
#define getpwuid	_SYS_getpwuid
#include <pwd.h>
#undef	getpwuid

extern char*		strdup();

extern struct passwd*	getpwuid();

/*
 * return uid name given uid number
 */

char*
fmtuid(uid)
int	uid;
{
	register char*		name;
	register struct passwd*	pw;

	static HASHTABLE*	uidtab;
	static char		buf[sizeof(int) * 3 + 1];

	if (!uidtab && !(uidtab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_namesize, sizeof(uid), HASH_name, "uidnum", 0))) (void)sprintf(name = buf, "%d", uid);
	else if (!(name = hashget(uidtab, &uid)))
	{
		if (pw = getpwuid(uid)) name = pw->pw_name;
		else (void)sprintf(name = buf, "%d", uid);
		(void)hashput(uidtab, (char*)0, name = strdup(name));
	}
	return(name);
}
