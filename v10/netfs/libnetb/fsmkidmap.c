/*
 * make an ID map for the file server library
 * takes a /etc/passwd-like file and an optional `exception' list
 * of explicit mappings;
 * returns a list of local name-ID mappings to be combined
 * with those from the client to make an ID-ID map.
 */

#include <stdio.h>
#include <ctype.h>
#include "rf.h"

static Idmap *exmerge();
static int nextid();

extern char *malloc(), *realloc();

Idmap *
rfmkidmap(file, ex)
char *file;
Namemap *ex;
{
	Idmap *map;
	register Idmap *cur;
	register int size;
	register Idmap *last;
	int curoff;
	FILE *fp;

	if ((fp = fopen(file, "r")) == NULL)
		rfpanic("can't open map %s\n", file);
	size = 100;
	if ((map = (Idmap *)malloc(size*sizeof(Idmap))) == NULL)
		rfpanic("out of mem for %s map\n", file);
	last = &map[size - 1];
	for (cur = map;; cur++) {
		if (cur >= last) {
			curoff = cur - map;	/* realloc may move map */
			size += 100;
			if ((map = (Idmap *)realloc((char *)map, size*sizeof(Idmap))) == NULL)
				rfpanic("out of mem for %s map\n", file);
			last = &map[size - 1];
			cur = &map[curoff];
		}
		if (nextid(cur, fp) == 0)
			break;
	}
	cur->name[0] = 0;
	fclose(fp);
	return (exmerge(map, last, ex));
}

/*
 * pick out the next name and ID from the file
 * name:junk:number:junk until newline
 * this matches /etc/passwd and /etc/group,
 * and is simple enough to fake up
 */
static
nextid(cur, fp)
Idmap *cur;
register FILE *fp;
{
	register char *p;
	char buf[150];		/* assume big enough for now */

	for (;;) {
		if (fgets(buf, sizeof(buf), fp) == NULL)
			return (0);	/* EOF */
		p = buf;
		while (*p && *p != ':')
			p++;
		if (*p == 0)
			continue;
		*p++ = 0;
		strncpy(cur->name, buf, sizeof(cur->name)-1);
		cur->name[sizeof(cur->name)-1] = 0;
		while (*p && *p != ':')
			p++;
		if (*p == 0)
			continue;
		p++;
		if (!isdigit(*p) && *p != '-')
			continue;
		cur->id = atoi(p);
		return (1);
	}
}

static Idmap *
exmerge(map, last, ex)
Idmap *map, *last;
Namemap *ex;
{
	register Idmap *lp;
	register Namemap *ep;
	int size, curoff;
	static Namemap nilex;

	if (ex == NULL)
		ex = &nilex;
	for (ep = ex; ep->cname[0]; ep++) {
		if (ep->sname[0] == 0)
			ep->sid = RFNOID;
		else
			ep->sid = _rflookid(map, ep->sname);
	}
	for (lp = map; lp->name[0]; lp++)
		for (ep = ex; ep->cname[0]; ep++) {
			if (ep->cname[0] != lp->name[0])	/* hash */
				continue;
			if (strcmp(ep->cname, lp->name) == 0) {
				lp->id = ep->sid;
				ep->sid = RFNOID;
			}
		}
	/*
	 * add client names not in local file
	 * lp was left at the end of the map
	 */
	for (ep = ex; ep->cname[0]; ep++) {
		if (ep->sid == RFNOID)	/* no need to enter */
			continue;
		if (lp >= last) {
			curoff = lp - map;
			size = curoff + 10;
			if ((map = (Idmap *)realloc((char *)map,
			    size*sizeof(Idmap))) == NULL)
				rfpanic("out of mem merging except\n");
			last = &map[size - 1];
			lp = &map[curoff];
		}
		strncpy(lp->name, ep->cname, sizeof(lp->name)-1);
		lp->id = ep->sid;
		lp++;
	}
	lp->name[0] = 0;
	return (map);
}
