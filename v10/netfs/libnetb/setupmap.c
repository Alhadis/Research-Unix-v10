/*
 * make userid and groupid maps
 * combine the client ids sent by setup
 * with the server ids left around by the user code
 */

#include <ctype.h>
#include "rf.h"
#include "perm.h"

#define	NULL	0

Idmap *rfuidmap, *rfgidmap;

char *defaultuser = 0;
char *defaultgroup = 0;

int defaultuid = RFNOID;
int defaultgid = RFNOID;

static int mapbuild();
static char *getpair();

extern char *malloc(), *realloc();

_rfmapuid(rbuf)
char *rbuf;
{
	static int nuids;

	if (defaultuser)
		defaultuid = _rflookid(rfuidmap, defaultuser);
	if (mapbuild(rbuf, rfuidmap, &_rfuids, &nuids, defaultuser) == 0)
		return (0);
	_rfmkhash(_rfuids, nuids, &_rfuhp, &_rfcu, &_rfsu);
	return (1);
}

_rfmapgid(rbuf)
char *rbuf;
{
	static int ngids;

	if (defaultgroup)
		defaultgid = _rflookid(rfgidmap, defaultgroup);
	if (mapbuild(rbuf, rfgidmap, &_rfgids, &ngids, defaultgroup) == 0)
		return (0);
	_rfmkhash(_rfgids, ngids, &_rfghp, &_rfcg, &_rfsg);
	return (1);
}

/*
 * combine server names and ids (locmap)
 * with client names and ids (rdata)
 * to make an id-id map (returned, length in *lsize)
 * return nonzero if this was the last buffer,
 * zero if more data are expected.  If the remote name
 * doesn't map to a local name, and defaultlocal is non-zero,
 * map it to the defaultlocal name.
 */
static int
mapbuild(rdata, locmap, lmap, lsize, defaultlocal)
char *rdata;
Idmap *locmap;
Tuid **lmap;
int *lsize;
char *defaultlocal;
{
	char *cname;
	char *p;
	int cid, sid;
	Tuid *map;
	register Tuid *cur, *last;
	int size;
	int curoff;
	Idmap nilmap;
	int done;

	if (locmap == NULL) {
		locmap = &nilmap;
		nilmap.name[0] = 0;
	}
	if ((size = *lsize) != 0) {	/* already started */
		map = *lmap;
		cur = &map[size];
		while (--cur >= map && cur->sid == RFNOID)
			;
		cur++;		/* point to first unused slot */
	} else {
		size = 100;	/* too big for groups, but too bad */
		if ((map = (Tuid *)malloc(size * sizeof(Tuid))) == NULL)
			rfpanic("out of memory making id map\n");
		cur = map;
	}
	last = &map[size-1];
	p = rdata;
	done = 1;		/* assume we'll get whole buffer */
	while ((p = getpair(p, &cname, &cid)) != NULL) {
		if (cname[0] == 0 || cid == RFNOID)
			continue;
		if (cname[0] == ':' && cname[1] == 0) {	/* continuation mark */
			done = 0;
			break;
		}
		if ((sid = _rflookid(locmap, cname)) == RFNOID)
#ifdef notdef
			if (defaultlocal == 0 ||
			   (sid = _rflookid(locmap, defaultlocal)) == RFNOID)
#endif
				continue;
		if (cur >= last) {
			curoff = cur - map;
			size += 100;
			if ((map = (Tuid *)realloc((char *)map, size*sizeof(Tuid))) == NULL)
				rfpanic("out of memory making id map\n");
			last = &map[size-1];
			cur = &map[curoff];
		}
		cur->sid = sid;
		cur->cid = cid;
		cur++;
	}
	cur->sid = cur->cid = RFNOID;	/* end marker */
	if (done == 0) {	/* i don't especially like this */
		for (cur++; cur <= last; cur++)
			cur->sid = RFNOID;
		/* now (cur-map) == size == allocated size */
	}
	*lsize = cur - map;
	*lmap = map;
	return (done);
}

static char *
getpair(line, pname, pnum)
char *line;
char **pname;
int *pnum;
{
	register char *p;

	p = line;
	while (isspace(*p))
		p++;
	*pname = p;
	while (!isspace(*p)) {
		if (*p == '\n' || *p == 0)
			return (NULL);
		p++;
	}
	while (isspace(*p))
		*p++ = 0;
	*pnum = atoi(p);
	if (*p == '-' || *p == '+')	/* well ... */
		p++;
	while (isdigit(*p))
		p++;
	if (*p != '\n')
		return (NULL);
	return (p+1);
}
