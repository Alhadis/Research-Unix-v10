/* Copyright 1990, AT&T Bell Labs */
#include <stdlib.h>
#include <string.h>
#include "fsort.h"

char *disorder = "key out of order";
char *duplicate = "duplicate key";
char *space = "out of space for merging";

enum { IEOF, IOK };
enum { IPRE = 01000, IFOL = 02000 }; /* see find() */
enum { NMERGE = 16 };

int nmerge = NMERGE;	/* max number of files to merge at once */
int nextfile;

struct merge {		/* current record in an open file */
	char *name;	/* name of file (for diagnostics) */
	FILE *file;
	struct rec *rec; /* pointer to the data (and key) */
	short serial;	/* file no., breaks ties for stable sort */
	short del;	/* delete at close */
};

struct merge *mfile;	/* one struct per open file */
struct merge **f;	/* pointers to mfile, ordered by content */

static void mergephase(int, char*);
static int find(int, int*);
static int fillrec(struct merge*);
static int compare(struct merge*, struct merge*);
static void syncerr(struct merge*, char*);
extern int link(char*, char*);
extern int unlink(char*);
static void recinit(int nf, int nm, int n);
static void mv(int, int);

static void
recalloc(struct merge *m)
{
	if(m->rec)
		return;		/* hygiene; doesn't happen */
	m->rec = (struct rec*)malloc(MINREC);
	if(m->rec == 0)
		fatal(space, "", 0);
	m->rec->dlen = 0;
	m->rec->next = (struct rec*)((uchar*)m->rec + MINREC);
}

/* misfortune : all fields are parsed and converted 
   before comparison.  Lazy parsing might be in order,
   but it's too hard to contemplate */

/* Merge strategy, to merge N inputs in groups of at most M.
   Numbers, like (1), are keyed to comments in code.
   If N <= M, merge everything. (4)
   If N > M*(M-1)+1
   greedily merge M at a time (1) to reduce to the next case.
   Merge x bunches of M (3) and one bunch of y (2),
   then merge the x+1 new files (4) with the remaining z files
   in an exactly M-way merge. Calculate x,y,z thus:
	Mx + y + z = N	file count
	x + 1 + z = M   final merge
        2 <= y <= M
	x >= 0
	z >= 0
   by algebra
	(M-1)x + y = N-M+1
        y == (N-M+1) mod (M-1),   2 <= y <= M
        x == (n-M+1-y)/(M-1)
   Total cost is Mx + y + N.  (Strategy due to P. McIlroy.) */

/* merge n files.  the first unmerged temp file is nm (i.e.
   nm is the number of already merged files).  the first
   unmerged input file is nf.
   rename temp files to keep a dense set beginning at 0
   (It seems silly to rename 100 files after merging
   every 1; how much time does that actually cost?) */
void
merge1(int nf, int nm, int n)
{
	char *name;
	int i, j;
	int flag = nf<nfiles;
	recinit(nf, nm, n);
	name = filename(nextfile++);
	mergephase(n, name);
	free(name);
	if(flag)
		return;
	mv(--nextfile, nm);
	for(i=nm+n, j=nm+1; i<nextfile; i++, j++)
		mv(i, j);
	nextfile -= n - 1;
}

/* if flag = 0 merge input files; else temps only.
   this program is specialized to two levels of merge;
   should fix that some day. */

void
merge(int flag)
{
	char buf[BUFSIZ];
	FILE *input, *output;
	int n, y;
	char *name;
	int nf = 0;		/* next input file to merge */
	int nm = 0;		/* total already merged once */
	int nt = nfiles;	/* files as yet unmerged */
	if(flag) {
		nf = nfiles;
		nt = nextfile;
	}
	if(mfile == 0)
		mfile = (struct merge*)calloc(nmerge+1,
					      sizeof(*mfile));
	if(f == 0)
		f = (struct merge**)calloc(nmerge+1,
					       sizeof(*f));
	if(mfile==0 || f==0)
		fatal(space,"",0);

	if(nt > nmerge*(nmerge-1)+1) {		/* (1) */
		for(nm=0; nt>0; ) {
			n = nt>nmerge? nmerge: nt;
			merge1(nf, nm, n);
			if(flag) nf += n;
			nt -= n;
			nm++;
		}
		merge(1);
		return;
	}
	if(nt > nmerge) {			/* (2) */
		y = (nt-nmerge+1)%(nmerge-1);
		if(y <= 1) y += nmerge-1;
		merge1(nf, nm, y);
		nf = nf+y>=nfiles? nfiles: nf+y;
		nt -= y;
		nm++;
	}
	while(nt+nm > nmerge) {			/* (3) */
		merge1(nf, nm, nmerge);
		nf = nf+nmerge>=nfiles? nfiles: nf+nmerge;
		nt -= nmerge;
		nm++;
	}

	n = nm+nt;				/* (4) */
	recinit(nf, 0, n);
	if(!overwrite(nf))
		name = oname;
	else
		name = filename(nextfile++);
	mergephase(n, name);
	if(name == oname)
		return;
	input = fileopen(name, "r");
	output = fileopen(oname, "w");
	while(n = fread(buf, 1, sizeof(buf), input))
		if(fwrite(buf, 1, n, output) != n)
			fatal("error writing", oname, 0);
	fileclose(input, name);
	unlink(name);
	free(name);
	fileclose(output, oname);
}

/* Initialize merge records for n files, beginning with
   input file number nf and temp file number nm.

   There are nfiles-nf input files; any files beyond that
   number must be temporaries, which come from already
   merged inputs.  (The only time that both kinds of
   files are present is the very last merge, when for
   stability, the temps are regarded as earlier.) */

static void
recinit(int nf, int nm, int n)
{
	int i;
	struct merge *m;
	int last = nfiles-nf + nextfile-nm == n;
	for(i=0; i<=n; i++)	/* one extra, for mergephase() */
		recalloc(&mfile[i]);
	for(i=0; i<n; i++) {
		m = &mfile[i];
		m->del = nf>=nfiles || last && i<nextfile;
		m->name = m->del? filename(nm++): files[nf++];
		m->file = fileopen(m->name, "r");
		m->serial = i;
		f[i] = m;
	}
	f[n] = &mfile[n];
}

static void
mv(int i, int j)
{
	char *old, *new;
	if(i == j) return;	/* believed not to happen */
	old = filename(i);
	new = filename(j);
	unlink(new);
	if(link(old,new) == -1 || unlink(old) == -1)
		fatal("cannot move", old, 0);
	free(old);
	free(new);
}

static void
emit(FILE *output)
{
	struct merge *m = f[0];
	int n = m->rec->dlen;
	uchar *p = data(m->rec);	/* hanky panky for speed */
	uchar *e = p + n++;		/* use fwrite instead of */
	int c = *e;			/* printf */
	*e = '\n';
	if(fwrite((char*)p, 1, n, output) != n)
		fatal("error writing", m->name, 0);
	*e = c;
}

/* Merge files in mfile[0]..mfile[n-1]. 
   f[0]..f[l-1] points to files in increasing order
   of their current records.  All the current records
   are strictly ordered, either by tie-breaking, or
   by skipping records that compare equal under -u.
   There are two phases: (a) initialization (reading
   from a file when there is no record at hand) and
   (b) continuation (reading the next record with the
   intent of displacing the current record).
   These conditions arise, at places numbered in comments
   (1) the record compares equal to another, and would follow
   that other if ties were broken
   (2) the record compares equal to another, and would precede
   that other if ties were broken
   (3) the record falls between two others

   At all times the keys pointed to by f[0]..f[l]
   are distinct (perhaps due to tie-breaking), as
   are the files they come from.  When a new key
   is read, its proper home (in tie-broken order) 
   is between f[j-1] and f[j].
   The picture below shows the hardest case (2b),
   where key a from file F is about to be emitted.
   All keys in the sequence axby are distinct, as
   are the files they come from.  A new key is read
   from file F into the extra space pointed to by
   f[i].  (Keeping one extra key allows disorder to
   be detected almost for free.)  The
   new key is equal to key b pointed to by f[j]
   and file F precedes file G in stability order.
   Thus b(F) deserves to be kept and b(G) discarded.
    0                 j                 l=n
   --------------------------------------------
   | a(F) |     x    | b(G) |     y    | b(F) |
   --------------------------------------------
  
   We emit the record at 0, and displace the key
   at j, from whose file there is now no record
   present; l decreases by one.  
    0                 j          l      n
   --------------------------------------------
   |     x    | b(F) |     y    | ?(G) |      |
   --------------------------------------------

   This takes us back to initialization.  From
   this configuration it is possible to reach
   case (2a), like (2b) except that the key at 0
   is not to be emitted; (2a) cannot happen during
   the original initialization.
*/

#define moveup(a,n) memmove(a+1, a, (n)*sizeof(*a))
#define movedown(a,n) memmove(a-1, a, (n)*sizeof(*a))

static void
mergephase(int n, char *name)
{
	int j, flags;
	struct merge *mp;
	struct rec *r;
	int l = 0;
	int k = -1;		/* for spotting sync errors */
	FILE *output = fileopen(name, "w");
init:	while(l < n) {					/*(a)*/
		mp = f[l];
		if(fillrec(mp) == IEOF) {
			movedown(f+l+1, n-l);
			f[n--] = mp;
			continue;
		}
		j = find(l, &flags);
		if(j <= k)
			syncerr(mp, disorder);
		if(flags & IFOL) {			/*(1a)*/
			if(!aflag || doaccum(f[j-1]->rec, mp->rec))
				continue;
		} else if(flags & IPRE)			/*(2a)*/
			if(!aflag || doaccum(mp->rec, f[j]->rec)) {
				f[l] = f[j];
				f[j] = mp;
				k = j;
				continue;
			}
		moveup(f+j, l-j);			/*(3a)*/
		f[j] = mp;
		l++;
	}
	while(n > 0) {					/*(b)*/
		mp = f[l];
		r = mp->rec;
		*mp = *f[0];
		mp->rec = r;
		if(fillrec(mp) == IEOF) {
			emit(output);
			mp = f[0];
			movedown(f+1, l);
			f[--n] = mp;
			l = n;
			continue;
		}
		j = find(l, &flags);
		if(j == 0)
			syncerr(mp, disorder);
		if(flags & IFOL) {			/*(1b)*/
			if(!aflag || doaccum(f[j-1]->rec, mp->rec))
				continue;
		} else if(flags & IPRE)			/*(2b)*/
			if(!aflag || doaccum(mp->rec, f[j]->rec)) {
				emit(output);
				f[l] = f[j];
				f[j] = mp;
				mp = f[0];
				movedown(f+1, l);
				f[l--] = mp;
				k = j - 1;
				goto init;
			}
		emit(output);				/*(3b)*/
		mp = f[0];
		movedown(f+1, j-1);
		f[j-1] = f[l];
		f[l] = mp;
	}
		
	fileclose(output, name);
}

static int
fillrec(struct merge *mp)
{
	struct rec *r = getline(mp->rec, mp->file);
	if(r == 0)
		return IOK;
	if(r == ENDFILE) {
		fileclose(mp->file, mp->name);
		if(mp->del)
			unlink(mp->name);
		return IEOF;
	}
	mp->rec = r;
	return IOK;
}

/* 
   Return the proper index for the extra record f[l]
   to be inserted before (regardless of -u).
   Under -u, if the extra record is a duplicate, return
   the index flagged by IFOL or IPRE according as
   the insertion position follows or precedes a
   record of the same value.
*/

static int
find(int l, int *flags)
{
	int i, t;
	int bot = 0;
	int top = l;
	while(bot < top) {
		i = (bot+top)/2;
		t = compare(f[l], f[i]);
		if(t < 0)
			top = i;
		else if(t > 0)
			bot = i + 1;
		else if(uflag) {
			if(f[l]->serial < f[i]->serial) {
				*flags = IPRE;
				return i;
			} else {
				*flags = IFOL;
				return i + 1;
			}
		}
		else if(f[l]->serial < f[i]->serial)
			top = i;
		else 
			bot = i + 1;
	}
	*flags = 0;
	return top;
}	

static int		
compare(struct merge *mi, struct merge *mj)
{
	uchar *ip, *jp;
	uchar *ei, *ej;
	uchar *trans, *keep;
	int li, lj, k;
	if(simplekeyed) {
		trans = fields->trans;
		keep = fields->keep;
		ip = data(mi->rec);
		jp = data(mj->rec);
		ei = ip + mi->rec->dlen;
		ej = jp + mj->rec->dlen;
		for( ; ; ip++, jp++) {
			while(ip<ei && !keep[*ip]) ip++;
			while(jp<ej && !keep[*jp]) jp++;
			if(ip>=ei || jp>=ej) break;
			k = trans[*ip] - trans[*jp];
			if(k != 0) break;
		}
		if(ip<ei && jp<ej)
			return k*signedrflag;
		else if(ip < ei)
			return signedrflag;
		else if(jp < ej)
			return -signedrflag;
		else if(sflag)
			return 0;
	} else if(keyed) {
		ip = key(mi->rec);
		jp = key(mj->rec);
		li = mi->rec->klen;
		lj = mj->rec->klen;
		for(k=li<lj?li:lj; --k>=0; ip++, jp++)
			if(*ip != *jp)
				break;
		if(k < 0) {
			if(li != lj)
				fatal("theorem disproved","",0);
			if(sflag)
				return 0;
		} else
			return *ip - *jp;
		
	}
	li = mi->rec->dlen;
	lj = mj->rec->dlen;
	ip = data(mi->rec);
	jp = data(mj->rec);
	for(k=li<lj?li:lj; --k>=0; ip++, jp++)
		if(*ip != *jp)
			break;
	return (k<0? li-lj: *ip-*jp)*signedrflag;
}

void
check(char *name)
{
	int i, t;

	mfile = (struct merge*)calloc(2,sizeof(*mfile));
	recalloc(&mfile[0]);
	recalloc(&mfile[1]);
	mfile[0].name = mfile[1].name = name;
	mfile[0].file = mfile[1].file = fileopen(name, "r");
	if(mfile[0].file == 0)
		fatal("can't open ", name, 0);

	if(fillrec(&mfile[0]) == IEOF)
		return;
	for(i=1; fillrec(&mfile[i])!=IEOF; i^=1) {
		t = compare(&mfile[i^1], &mfile[i]);
		if(t>0)
			syncerr(&mfile[i], disorder);
		else if(t==0 && uflag)
			syncerr(&mfile[i], duplicate);
	}
}

static void
syncerr(struct merge *m, char *message)
{
	int n = m->rec->dlen;
	warn("trouble in file", m->name, 0);
	fatal(message, n?(char*)data(m->rec):"", n);
}
