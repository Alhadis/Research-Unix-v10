#include "asd.h"
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK 16

static struct gtab {
	unsigned gid;
	char *name;
} *gtab;

static int size, salloc;

char *
gidstr (gid_t gid)
{
	register int i;
	static char buf[12];
	struct group *g;

	/* search the cache for the gid */
	for (i = 0; i < size; i++)
		if (gtab[i].gid == gid)
			return gtab[i].name;

	/* try to find it in the system's database */
	if (g = getgrgid(gid)) {
		if (size % CHUNK == 0)
			gtab = (struct gtab *) ralloc((char *) gtab, salloc += CHUNK);
		gtab[size].gid = g->gr_gid;
		gtab[size].name = copy(g->gr_name);
		++size;
		return gtab[size - 1].name;
	}

	/* failure, invent a string */
	sprintf (buf, "#%u", gid);
	return buf;
}

gid_t
gidnum (char *name)
{
	register int i;
	struct group *g;

	/* if it starts with a #, use the number */
	if (name[0] == '#')
		return atoi (name + 1);

	/* search the cache */
	for (i = 0; i < size; i++)
		if (strcmp (gtab[i].name, name) == 0)
			return gtab[i].gid;

	/* try to find it in the system's database */
	if (g = getgrnam(name)) {
		if (size % CHUNK == 0)
			gtab = (struct gtab *) ralloc((char *) gtab, salloc += CHUNK);
		gtab[size].gid = g->gr_gid;
		gtab[size].name = copy(g->gr_name);
		++size;
		return gtab[size - 1].gid;
	}


	/* failure, invent a value */
	return getgid();
}
