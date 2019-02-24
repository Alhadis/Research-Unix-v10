#include "asd.h"
#include <pwd.h>

#define CHUNK 16

struct passwd *getpwent();

static struct utab {
	unsigned uid;
	char *name;
} *utab;

static int size;

char *
struid (uid)
	register unsigned short uid;
{
	register int i;
	static char buf[12];

	/* search the cache for the uid */
	for (i = 0; i < size; i++)
		if (utab[i].uid == uid)
			return utab[i].name;

	/* search the file, caching */
	while (expand())
		if (utab[size-1].uid == uid)
			return utab[size-1].name;

	/* failure, invent a string */
	sprintf (buf, "#%u", uid);
	return buf;
}

int
numuid (name)
	register char *name;
{
	register int i;

	/* if it starts with a #, use the number */
	if (name[0] == '#')
		return atoi (name + 1);

	/* search the cache */
	for (i = 0; i < size; i++)
		if (strcmp (utab[i].name, name) == 0)
			return utab[i].uid;

	/* search the file */
	while (expand())
		if (strcmp (utab[size-1].name, name) == 0)
			return utab[size-1].uid;

	/* failure, invent a value */
	return getuid();
}

static int
expand()
{
	register struct passwd *pw;
	static ended;

	if (!ended) {
		pw = getpwent();
		if (pw == NULL) {
			ended = 1;
			endpwent();
			return 0;
		}
		if (size % CHUNK == 0) {
			utab = (struct utab *) ralloc ((char *) utab,
			    (unsigned) ((size + CHUNK) * sizeof (*utab)));
		}
		utab[size].uid = pw->pw_uid;
		utab[size].name = copy (pw->pw_name);
		size++;
		return 1;
	}

	return 0;
}
