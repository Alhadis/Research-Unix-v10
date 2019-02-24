/*
 * nexus device routines;
 * both sbi and vaxbi nexus
 */

#include "mkconf.h"

/*
 * addressing fields
 */

static char *sbifields[] = {
	"bus", "tr", "voff", NULL
};

#define	a_adp	addr[0]	/* adapter: which sbi, which vaxbi */
#define	a_tr	addr[1]	/* nexus number within that adapter */
#define	a_svoff	addr[2]	/* sbi devices (like unibus adapter): vector offset */

static char *vbifields[] = {
	"bi", "node", "vec", "voff", NULL
};

#define	a_bvec	addr[2]	/* vaxbi: vector */
#define	a_bvoff	addr[3]	/* vaxbi adapter devices (like dw): vector offset */

#define	VFMT	"X%s%d%d"	/* name of vector interface routine */

#define	MAXNEX	20	/* this many nexus, not highest hardware tr */

struct nextab {
	char adp;
	char nex;
} nextab[MAXNEX];

int nextnex;

sbisyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	char *p;

	dp->a_svoff = 0;
	if ((p = keyread(dp->addr, ap, n, sbifields)) != NULL) {
		fprint(STDERR, "%s %d: bad field %s\n", dp->mdev->name, dp->id, p);
		errs++;
		return (1);
	}
	dp->vecoff = dp->a_svoff;
	return (0);
}

vbisyntax(dp, ap, n)
register Dev *dp;
char **ap;
int n;
{
	char *p;

	dp->a_bvoff = 0;
	if ((p = keyread(dp->addr, ap, n, vbifields)) != NULL) {
		fprint(STDERR, "%s %d: bad field %s\n", dp->mdev->name, dp->id, p);
		errs++;
		return (1);
	}
	dp->vecoff = dp->a_bvoff;
	return (0);
}

/*
 * fixup routine:
 * compute and insert our (fixed) interrupt vectors
 * insert our entry in the table of nexus
 * and replace our `nexus' address with the appropriate index
 */
int
sbifixup(dp)
register Dev *dp;
{
	register int vec;
	register int i;
	register Mdev *mp;
	char vname[20];

	nexparent(dp);
	mp = dp->mdev;
	if (mp->nvec) {
		vec = dp->a_tr*4;	/* honest nexus number, mod 16 */
		vec += 0x100;		/* -> br4 vector */
		if (dp->parent)
			vec += dp->parent->vecoff;
		sprint(vname, VFMT, mp->tag, dp->id, 0);
		for (i = 0; i < 4; i++) {
			newvec(vec, SINTR, vname);
			vec += 0x40;
		}
	}
	newnex(dp);
}

int
vbifixup(dp)
register Dev *dp;
{
	register int vec;
	register Mdev *mp;
	register int i;
	char vname[20];

	nexparent(dp);
	mp = dp->mdev;
	vec = dp->a_bvec;
	if (dp->parent)		/* && vec > 0x200? */
		vec += dp->parent->vecoff;
	for (i = 0; i < mp->nvec; i++, vec += 4) {
		sprint(vname, VFMT, mp->tag, dp->id, i);
		newvec(vec, SINTR, vname);
	}
	newnex(dp);
}

newnex(dp)
register Dev *dp;
{
	if (nextnex >= MAXNEX) {
		fprint(STDERR, "%s %d: too many nexus\n", dp->mdev->name, dp->id);
		errs++;
		return;
	}
	nextab[nextnex].adp = dp->a_adp;
	nextab[nextnex].nex = dp->a_tr;
	dp->addr[1] = nextnex;
	nextnex++;
}

nexparent(dp)
register Dev *dp;
{
	register Dev *ep;

	for (ep = dlist; ep; ep = ep->next)
		if (ep->mdev->adptype == dp->mdev->atype
		&&  ep->id == dp->a_adp)
			break;
	dp->parent = ep;
}

sbiputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	nexputtab(fd);
	mp = dp->mdev;
	count = 0;
	fprint(fd, "struct nxaddr %saddr[] = {\n", mp->tag);
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next) {
		while (count++ < dp->id)
			fprint(fd, "\t{-1, 0},\n");
		fprint(fd, "\t{%ld, %ld, 0x%lx},\n", dp->a_tr, dp->a_adp, dp->a_svoff);
	}
	count *= mp->rept;
	fprint(fd, "};\nint %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);
}

vbiputaddr(fd, dp)
int fd;
register Dev *dp;
{
	register int count;
	register Mdev *mp;

	nexputtab(fd);
	mp = dp->mdev;
	count = 0;
	fprint(fd, "struct biaddr %saddr[] = {\n", mp->tag);
	for (mp = dp->mdev; dp && dp->mdev == mp; dp = dp->next) {
		while (count++ < dp->id)
			fprint(fd, "\t{-1},\n");
		fprint(fd, "\t{%ld, 0x%lx, 0x%lx, %ld},\n",
			dp->a_tr, dp->a_bvec, dp->a_bvoff, dp->a_adp);
	}
	count *= mp->rept;
	fprint(fd, "};\nint %scnt = %d;\n", mp->tag, count);
	putstrs(fd, mp, count);
}

/*
 * put out nexus tables
 */

nexputtab(fd)
int fd;
{
	register int i;
	static int done = 0;

	if (done++)
		return;
	fprint(fd, "\nstruct nextab nextab[] = {\n");
	for (i = 0; i < nextnex; i++)
		fprint(fd, "\t%d, %d,\n", nextab[i].adp, nextab[i].nex);
	fprint(fd, "\t-1\n};\n");
}

/*
 * put out vectors
 */

nexputvec(fd, dp)
int fd;
register Dev *dp;
{
	register int i;
	char vname[30];

	for (i = 0; i < dp->mdev->nvec; i++) {
		sprint(vname, VFMT, dp->mdev->tag, dp->id, i);
		stdputvec(fd, dp, vname, i);
	}
}
