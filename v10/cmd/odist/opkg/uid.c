#include "asd.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define CHUNK 16

static struct utab {
	unsigned uid;
	char *name;
} *utab;

static int size, salloc;

char *
uidstr (uid_t uid)
{
	register int i;
	static char buf[12];
	struct passwd *p;

	/* search the cache for the uid */
	for (i = 0; i < size; i++)
		if (utab[i].uid == uid)
			return utab[i].name;

	/* try to find it in the system's database */
	if (p = getpwuid(uid)) {
		if (size % CHUNK == 0)
			utab = (struct utab *) ralloc((char *) utab, salloc += CHUNK);
		utab[size].uid = p->pw_uid;
		utab[size].name = copy(p->pw_name);
		++size;
		return utab[size - 1].name;
	}

	/* failure, invent a string */
	sprintf (buf, "#%u", uid);
	return buf;
}

uid_t
uidnum (char *name)
{
	register int i;
	struct passwd *p;

	/* if it starts with a #, use the number */
	if (name[0] == '#')
		return atoi (name + 1);

	/* try to find it in the system's database */
	if (p = getpwnam(name)) {
		if (size % CHUNK == 0)
			utab = (struct utab *) ralloc((char *) utab, salloc += CHUNK);
		utab[size].uid = p->pw_uid;
		utab[size].name = copy(p->pw_name);
		++size;
		return utab[size - 1].uid;
	}

	/* search the cache */
	for (i = 0; i < size; i++)
		if (strcmp (utab[i].name, name) == 0)
			return utab[i].uid;

	/* failure, invent a value */
	return getuid();
}
