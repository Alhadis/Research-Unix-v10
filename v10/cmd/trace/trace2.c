#include <stdio.h>
#include <errno.h>
#include "trace.h"
#include "trace.d"

 extern struct TBL       *tbl;
 extern struct LBT	 *lbt;
 extern struct MBOX      *mbox;
 extern struct MNAME     *fullname;
 extern struct REVPOL    **expr;
 extern struct PROCSTACK **procstack;

 extern struct VARPARS	 *procpars;
 extern struct TBLPARS	 *tblpars;

 extern struct LOCVARS   *tblvars;
 extern struct TBLPARS	 *tablpars;

 extern int *reftasks, *processes, *basics;
 extern int *globvars, *inits, *xob, *effnrstates;

 extern char qoverride;
 extern int QMAX, msgbase, maxcol, assertbl, errortbl;

#define tell(s)	fprintf(stderr, s)

usage(str)
	char *str;
{	fprintf(stderr, "trace: %s\n", str);
	tell("usage: trace [-?] [N]\n");
	tell("\t-a show all prefixes leading into old states\n");
	tell("\t-b `blast mode' (quick, very partial search)\n");
	tell("\t-c  N  perform class N validation (N: 0..5) \n");
	tell("\t-f or -F alternative formats for printing queue histories\n");
	tell("\t-j stop at the first buffer lock found\n");
	tell("\t-k  N  restrict the state space cache to N thousand states\n");
	tell("\t-l show normal execution sequences and loops, but no prefixes\n");
	tell("\t-m  N  restrict search depth to N steps\n");
	tell("\t-n don't use timeout heuristics\n");
	tell("\t-q  N  set bound N on maximum queue size used\n");
	tell("\t-r  N  restrict the runtime to  N minutes\n");
	tell("\t-R  N  report on progress every N minutes\n");
	tell("\t-s show the transition tables (different if combined with `v')\n");
	tell("\t-v verbose - print execution times, etc.\n");
	tell("\t-x perform a scatter search\n");
	tell("\tno flag: try a sensible partial search\n");
	exit(1);
}

/*
 * calls on Emalloc and Realloc go straight to the library malloc
 * it is used for data that may be realloced but is never released
 * - Smalloc claims memory that is never realloced and never released
 * - emalloc and efree handle memory that is never realloced but often released
 * - talloc and tfree are direct calls on the tac-package (used via emalloc)
 */

char *
Stake(n)
{ char *try, *sbrk(), *Emalloc();
  extern int errno;

	do {
		try = sbrk(n);
	} while ((int) try == -1 && errno == EINTR);

	if ((int) try == -1)
		return Emalloc(n);	/* maybe some mem left here */

	return try;
}

#define CHUNK	4096

char *have;
long left = 0;

char *
Smalloc(n)
	unsigned n;
{ char *try;

	if (n == 0)
		return (char *) NULL;
	if (left < n)
	{	unsigned grow = (n < CHUNK) ? CHUNK : n;
		have = Stake(grow);
		left = grow;
	}
	try = have;
	have += n;
	left -= n;

	return try;
}

char *
Emalloc(n)
	unsigned n;
{ char *try, *malloc();

	if (n == 0)
		return (char *) NULL;
	if ((try = malloc(n)) == NULL)
		whoops("malloc fault");

	return try;
}

char *
Realloc(a, b)
	char *a; unsigned b;
{ char *try, *realloc();

	if (b == 0)
		return (char *) NULL;
	if ((try = realloc(a, b)) == NULL)
		whoops("realloc returns 0");

	return try;
}

char *
emalloc(n)
	unsigned n;
{ char *try;
  char *talloc();

	if (n == 0)
		return (char *) NULL;
	if ((try = talloc(n)) == NULL)
		whoops("talloc fault");

	return try;
}

efree(at)
	char *at;
{
	if (at) tfree(at);
}

alloc1(x, y, z)
{ int n = x+y;
	tbl      = (struct TBL *)
		Smalloc(n * sizeof(struct TBL));
	tblpars  = (struct TBLPARS *)
		Smalloc(n * sizeof(struct TBLPARS));
	tblvars  = (struct LOCVARS *)
		Smalloc(n * sizeof(struct LOCVARS));
	reftasks = (int *)
		Smalloc(x * sizeof(int));
	processes = (int *)
		Smalloc(y * sizeof(int));
	lbt       = (struct LBT *)
		Smalloc(y * sizeof(struct LBT));
	procpars  = (struct VARPARS *)
		Smalloc(y * sizeof(struct VARPARS));

	tablpars  = (struct TBLPARS *)
		Smalloc(y * sizeof(struct TBLPARS));

	basics    = (int *)
		Smalloc(y * sizeof(int));
	procstack = (struct PROCSTACK **)
		Smalloc(y * sizeof(struct PROCSTACK *));
	mbox      = (struct MBOX *)
		Smalloc(z * sizeof(struct MBOX));

	effnrstates = (int *)
		Smalloc(n * sizeof(int));
}

alloc2(n, p, who)
{ char x;

	x = (qoverride && p > QMAX)? QMAX : p;

	if (x >= 256)
		whoops("illegal queue size");
	if (x >= 16)
		fprintf(stderr, "warning, very large qsize (%d), queue %d\n", x, n);

	mbox[n].limit = x;
	mbox[n].owner = (who >= 0) ? who : 0;
}

alloc3(n)
{	inits = (int *)
		Smalloc(n * sizeof(int));
}

alloc4(n)
{	if (assertbl == NONE && errortbl == NONE)
		globvars = (int *)
			Smalloc(n * sizeof(int));
	else
		globvars = (int *)
			Emalloc(n * sizeof(int));
}

alloc45(n)
{ register int i;
	fullname = (struct MNAME *)
		Smalloc(n * sizeof(struct MNAME));
	xob = (int *)
		Smalloc((n+msgbase) * sizeof(int));
	for (i = 0; i < n+msgbase; i++)
		xob[i] = -1;
}

alloc5(n)
{ register int i, j, r, c;

	r = tbl[n].nrrows;
	if ((c = tbl[n].nrcols) > maxcol)
		maxcol = c;

	tbl[n].endrow  = (int *) Smalloc(r * sizeof(int));
	tbl[n].deadrow = (int *) Smalloc(r * sizeof(int));
	tbl[n].badrow  = (int *) Smalloc(r * sizeof(int));
	tbl[n].labrow  = (int *) Smalloc(r * sizeof(int));
	tbl[n].colmap  = (int *) Smalloc(c * sizeof(int));
	tbl[n].colorg  = (int *) Smalloc(c * sizeof(int));
	tbl[n].coltyp  = (int *) Smalloc(c * sizeof(int));
	tbl[n].ptr     = (struct IND **) Smalloc(r * sizeof(struct IND *));
	tbl[n].rowname = (char **) Smalloc(r * sizeof(char *));

	for (i = 0; i < r; i++)
	{	tbl[n].ptr[i] = (struct IND *)
			Smalloc(c * sizeof(struct IND));
		tbl[n].rowname[i] = (char *)
			Smalloc(128 * sizeof(char));

		for (j = 0; j < c; j++)
			tbl[n].ptr[i][j].nrpils = 0;
		tbl[n].deadrow[i] = 1;
		tbl[n].endrow[i] = tbl[n].badrow[i] = 0;
		tbl[n].labrow[i] = 0;
	}
	tbl[n].labrow[0] = 1;	/* make sure initial state is always checked */
}

alloc6(n, m, p, q)
{	tbl[n].ptr[m][p].one = (struct ELM *)
		Smalloc(q * sizeof(struct ELM));
}

alloc8(pr, p, q)
{
	tablpars[pr].nrms = (short) p;			/* available */
	tablpars[pr].nrvs = (short) q;

	procpars[pr].ms = (short *)
		Emalloc(p * sizeof(short));
	procpars[pr].vs = (short *)
		Emalloc(q * sizeof(short));

	procpars[pr].nrms = 0;				/* actually used */
	procpars[pr].nrvs = 0;
}

alloc9(in, p)
{
	tbl[in].calls = (struct CPARS *)
		Smalloc(p * sizeof(struct CPARS));
}

alloc10(in, cn, p, q, r)
{
	tbl[in].calls[cn].callwhat = (short) p;
	tbl[in].calls[cn].nrms = (short) q;
	tbl[in].calls[cn].nrvs = (short) r;

	tbl[in].calls[cn].ms = (short *)
			Smalloc(q * sizeof (short));

	tbl[in].calls[cn].vs = (short *)
			Smalloc(r * sizeof (short));
}

whoops(s)
	char *s;
{
	fprintf(stderr, "trace: %s\n", s);
	postlude();
	exit(1);
}

badinput(s)
	char *s;
{	extern char fname[];

	fflush(stdout);
	fprintf(stderr, "trace: read error `%s': %s\n", s, fname);
	exit(1);
}
