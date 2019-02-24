#define _POSIX_SOURCE
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "lib.h"

struct idmap {
	char *str;
	int num;
	struct idmap *next;
};

static struct idmap *umap, *gmap;

static struct idmap *
idnum(struct idmap **map, char *str)
{
	while (*map) {
		if ((*map)->str && strcmp((*map)->str, str) == 0)
			return *map;
		map = &(*map)->next;
	}
	return 0;
}

static struct idmap *
idstr(struct idmap **map, int num)
{
	while (*map) {
		if ((*map)->num == num)
			return *map;
		map = &(*map)->next;
	}
	return 0;
}

static struct idmap *
idset(struct idmap **map, char *str, int num)
{
	struct idmap *mapent;

	mapent = xmalloc(sizeof (struct idmap));
	if (str)
		mapent->str = xstrdup(str);
	else
		mapent->str = 0;
	mapent->num = num;
	mapent->next = *map;
	*map = mapent;
	return mapent;
}

int
uidnum(char *str)
{
	struct idmap *id;
	struct passwd *pw;
	int num;

	id = idnum(&umap, str);
	if (!id) {
		pw = getpwnam(str);
		id = idset(&umap, str, pw ? pw->pw_uid : -1);
	}
	return id->num;
}

char *
uidstr(int num)
{
	struct idmap *id;
	struct passwd *pw;
	char *str;

	id = idstr(&umap, num);
	if (!id) {
		pw = getpwuid(num);
		id = idset(&umap, pw ? pw->pw_name : 0, num);
	}
	return id->str;
}

int
gidnum(char *str)
{
	struct idmap *id;
	struct group *gr;
	int num;

	id = idnum(&gmap, str);
	if (!id) {
		gr = getgrnam(str);
		id = idset(&gmap, str, gr ? gr->gr_gid : -1);
	}
	return id->num;
}

char *
gidstr(int num)
{
	struct idmap *id;
	struct group *gr;
	char *str;

	id = idstr(&gmap, num);
	if (!id) {
		gr = getgrgid(num);
		id = idset(&gmap, gr ? gr->gr_name : 0, num);
	}
	return id->str;
}
