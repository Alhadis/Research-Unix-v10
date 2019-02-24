#include "asd.h"
#include <grp.h>

#define CHUNK 16

struct group *getgrent();

static struct gtab {
	unsigned gid;
	char *name;
} *gtab;

static int size;

char *
strgid (gid)
	register unsigned short gid;
{
	register int i;
	static char buf[12];

	/* search the cache for the gid */
	for (i = 0; i < size; i++)
		if (gtab[i].gid == gid)
			return gtab[i].name;

	/* search the file, caching */
	while (expand())
		if (gtab[size-1].gid == gid)
			return gtab[size-1].name;

	/* failure, invent a string */
	sprintf (buf, "#%u", gid);
	return buf;
}

int
numgid (name)
	register char *name;
{
	register int i;

	/* if it starts with a #, use the number */
	if (name[0] == '#')
		return atoi (name + 1);

	/* search the cache */
	for (i = 0; i < size; i++)
		if (strcmp (gtab[i].name, name) == 0)
			return gtab[i].gid;

	/* search the file */
	while (expand())
		if (strcmp (gtab[size-1].name, name) == 0)
			return gtab[size-1].gid;

	/* failure, invent a value */
	return getgid();
}

static int
expand()
{
	register struct group *gr;
	static ended;

	if (!ended) {
		gr = getgrent();
		if (gr == NULL) {
			ended = 1;
			endgrent();
			return 0;
		}
		if (size % CHUNK == 0) {
			gtab = (struct gtab *) ralloc ((char *) gtab,
			    (unsigned) ((size + CHUNK) * sizeof (*gtab)));
		}
		gtab[size].gid = gr->gr_gid;
		gtab[size].name = copy (gr->gr_name);
		size++;
		return 1;
	}

	return 0;
}
