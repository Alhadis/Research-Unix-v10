/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * gid number -> name
 */

#include <stdio.h>
#include <hash.h>
#define getgrgid	_SYS_getgrgid
#include <grp.h>
#undef	getgrgid

extern char*		strdup();

extern struct group*	getgrgid();

/*
 * return gid name given gid number
 */

char*
fmtgid(gid)
int	gid;
{
	register char*		name;
	register struct group*	gr;

	static HASHTABLE*	gidtab;
	static char		buf[sizeof(int) * 3 + 1];

	if (!gidtab && !(gidtab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_namesize, sizeof(gid), HASH_name, "gidnum", 0))) (void)sprintf(name = buf, "%d", gid);
	else if (!(name = hashget(gidtab, &gid)))
	{
		if (gr = getgrgid(gid)) name = gr->gr_name;
		else (void)sprintf(name = buf, "%d", gid);
		(void)hashput(gidtab, (char*)0, name = strdup(name));
	}
	return(name);
}
