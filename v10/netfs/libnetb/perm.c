/*
 * permission-mapping primitives
 *
 * for each of user- and group-ids,
 * there is a map of (client, server) id pairs
 * ids not in the map are mapped to RFNOID
 * if rfotherdeny != -0, RFNOID is denied all access
 * RFNOID may not create files in any case (who would own them?)
 *	(answer: defaultuser and defaultgroup.)
 */

#include "rf.h"
#include "perm.h"

#define	NULL	0

#define	SUSER	0	/* super-user ID */

int rfotherdeny;
int rfuid = RFNOID;	/* mapped uid of the client */
int rfgid = RFNOID;	/* mapped gid of the client */
int cuid;		/* client's uid (unmapped) */
int cgid;


Tuid *_rfuids, *_rfgids;
int _rfuhp, _rfghp;	/* hash primes */
int *_rfsu, *_rfsg, *_rfcu, *_rfcg;	/* hash tables */

extern char *malloc();
extern int defaultuid;
extern int defaultgid;

/*
 * high-level routines to check particular sorts of access
 * return nonzero if access OK
 */

/*
 * read access to a regular file
 * -- execute implies read;
 * a file server can't tell the difference,
 * and the data ends up inside the client anyway
 */
_rfpread(f, u, g)
register Rfile *f;
int u, g;
{
	register int bits;

	bits = RFPRD|RFPEX;
	if (u == RFNOID && defaultuid != RFNOID)
		u = defaultuid;
	if (g == RFNOID && defaultgid != RFNOID)
		g = defaultgid;
	if (u == RFNOID && rfotherdeny)
		return (0);
	if (f->uid == u && u != RFNOID)
		bits <<= RFPOWNER;
	else if (f->gid == g && g != RFNOID)
		bits <<= RFPGROUP;
	else
		bits <<= RFPOTHER;
	return (f->mode & bits);
}

/*
 * read entries from a directory
 * -- only read access is relevant
 */
_rfpdread(f, u, g)
register Rfile *f;
int u, g;
{
	register int bits;

	if (u == SUSER)
		return (1);
	if (u == RFNOID && defaultuid != RFNOID)
		u = defaultuid;
	if (g == RFNOID && defaultgid != RFNOID)
		g = defaultgid;
	bits = RFPRD;
	if (u == RFNOID && rfotherdeny)
		return (0);
	if (f->uid == u && u != RFNOID)
		bits <<= RFPOWNER;
	else if (f->gid == g && g != RFNOID)
		bits <<= RFPGROUP;
	else
		bits <<= RFPOTHER;
	return (f->mode & bits);
}

/*
 * write a regular file
 */
_rfpwrite(f, u, g)
register Rfile *f;
int u, g;
{
	register int bits;

	if (u == SUSER)
		return (1);
	if (u == RFNOID && defaultuid != RFNOID)
		u = defaultuid;
	if (g == RFNOID && defaultgid != RFNOID)
		g = defaultgid;
	bits = RFPWR;
	if (u == RFNOID && rfotherdeny)
		return (0);
	if (f->uid == u && u != RFNOID)
		bits <<= RFPOWNER;
	else if (f->gid == g && g != RFNOID)
		bits <<= RFPGROUP;
	else
		bits <<= RFPOTHER;
	return (f->mode & bits);
}

/*
 * write a directory (change entries therein)
 */
_rfpdwrite(f, u, g)
register Rfile *f;
int u, g;
{
	register int bits;

	if (u == SUSER)
		return (1);
	if (u == RFNOID && defaultuid != RFNOID)
		u = defaultuid;
	if (g == RFNOID && defaultgid != RFNOID)
		g = defaultgid;
	bits = RFPWR;
	if (u == RFNOID && rfotherdeny)
		return (0);
	if (f->uid == u && u != RFNOID)
		bits <<= RFPOWNER;
	else if (f->gid == g && g != RFNOID)
		bits <<= RFPGROUP;
	else
		bits <<= RFPOTHER;
	return (f->mode & bits);
}

/*
 * search a directory for a single entry
 */
_rfpdsearch(f, u, g)
register Rfile *f;
int u, g;
{
	register int bits;

	if (u == SUSER)
		return (1);
	if (u == RFNOID && defaultuid != RFNOID)
		u = defaultuid;
	if (g == RFNOID && defaultgid != RFNOID)
		g = defaultgid;
	bits = RFPDS;
	if (u == RFNOID && rfotherdeny)
		return (0);
	if (f->uid == u && u != RFNOID)
		bits <<= RFPOWNER;
	else if (f->gid == g && g != RFNOID)
		bits <<= RFPGROUP;
	else
		bits <<= RFPOTHER;
	return (f->mode & bits);
}

/*
 * is this the file's owner?
 * only the owner may change attribute like the mode
 */
_rfpowner(f, u, g)
register Rfile *f;
int u, g;
{
	if (u == SUSER)
		return (1);
	if (u == RFNOID && defaultuid != RFNOID)
		u = defaultuid;
	if (g == RFNOID && defaultgid != RFNOID)
		g = defaultgid;
	if (u == RFNOID)
		return (0);
	return (f->uid == u);
}

/*
 * is this a privileged user?
 */
int
_rfpsuper(f, u, g)
Rfile *f;
int u, g;
{
	return (u == SUSER);
}

/*
 * given one kind of id (password or group, client or server)
 * return another
 */

int
_rfcuid(id)
int id;
{
	register int i;

	if (id < 0)	/* safety */
		return (RFNOID);
	for (i = id % _rfuhp; _rfsu[i] != RFNOID; i++)
		if (_rfuids[_rfsu[i]].sid == id)
			return (_rfuids[_rfsu[i]].cid);
	return (RFNOID);
}

int
_rfcgid(id)
int id;
{
	register int i;

	if (id < 0)	/* safety */
		return (RFNOID);
	for (i = id % _rfghp; _rfsg[i] != RFNOID; i++)
		if (_rfgids[_rfsg[i]].sid == id)
			return (_rfgids[_rfsg[i]].cid);
	return (RFNOID);
}

int
_rfsuid(id)
int id;
{
	register int i;

	if (id < 0)	/* safety */
		return (RFNOID);
	for (i = id % _rfuhp; _rfcu[i] != RFNOID; i++)
		if (_rfuids[_rfcu[i]].cid == id)
			return (_rfuids[_rfcu[i]].sid);
	return (RFNOID);
}

int
_rfsgid(id)
int id;
{
	register int i;

	if (id < 0)	/* safety */
		return (RFNOID);
	for (i = id % _rfghp; _rfcg[i] != RFNOID; i++)
		if (_rfgids[_rfcg[i]].cid == id)
			return (_rfgids[_rfcg[i]].sid);
	return (RFNOID);
}

/*
 * make hash tables
 */

#define	SLOP	4

static short primes[] = {23, 47, 97, 199, 397, 797, 1597, 3191, 6397, 12799, 31991, 0};

_rfmkhash(tab, size, php, pchash, phhash)
Tuid *tab;
int size;
int *php;
int **pchash, **phhash;
{
	register int i, j, hp;
	register int *ch, *cc;
	int max;

	for (i = 0; primes[i] && primes[i] < size * 2; i++)
		;
	hp = primes[i];
	*php = hp;
	max = hp + SLOP;
	if ((ch = (int *)malloc(max*sizeof(int))) == NULL
	||  (cc = (int *)malloc(max*sizeof(int))) == NULL)
		rfpanic("no memory for hash tables (%d ints)\n", hp);
	*phhash = ch;
	*pchash = cc;
	for (i = 0; i < max; i++)
		ch[i] = cc[i] = RFNOID;
	for (i = 0; tab[i].sid != RFNOID; i++) {
		j = tab[i].sid % hp;
		while (ch[j] != RFNOID && j < max)
			j++;
		if (j >= max)
			rfpanic("hash overflow\n");
		ch[j] = i;
		j = tab[i].cid % hp;
		while (cc[j] != RFNOID && j < max)
			j++;
		if (j >= max)
			rfpanic("hash overflow\n");
		cc[j] = i;
	}
}

/*
 * look up a name in an Idmap list
 * used only in building id tables
 */
int
_rflookid(p, s)
register Idmap *p;
register char *s;
{

	for (; p->name[0]; p++) {
		if (p->name[0] != s[0])	/* shortcut */
			continue;
		if (strncmp(p->name, s, sizeof(p->name)-1) == 0)
			return (p->id);
	}
	return (RFNOID);
}
