#include <stdio.h>
#include "ts.h"

#define HSIZE 307
#define ALSIZE 2040

static struct obj **objhash, *cobj = 0, *lastobj;
static struct ref **refhash;
static struct group **grouphash, *group0;
struct ref *undefrefs = 0, *defedrefs = 0;
struct group *cgroup = 0; /* current group */
struct obj *obj0 = 0; /* special null object */

unsigned lasthash = 0, ngroup = 0, nrefc = 0;
int nextseq;

char *malloc(), *strcpy();
extern char *progname;
extern int cyclep, verbose, verify;

init(){
	int i;

	objhash = (struct obj **) malloc(HSIZE*sizeof(struct obj *));
	refhash = (struct ref **) malloc(HSIZE*sizeof(struct ref *));
	grouphash = (struct group **) malloc(HSIZE*sizeof(struct group *));
	if (!objhash || !refhash || !grouphash)
		scream("initial mallocs fail\n","");
	for (i = 0; i < HSIZE; i++)
		{ objhash[i] = 0; refhash[i] = 0; grouphash[i] = 0;}
	cgroup = 0;
	newgroup("");
	group0 = cgroup;
	newobj("",1);
	obj0 = cobj;
	}

unsigned
hash(s)
unsigned char *s;
{
	unsigned x = 0;
	unsigned char c;

	while(c = *s++) x = (x<<1) + c;
	return x % HSIZE;
	}

char *
newstruct(n)
unsigned n;
{
	static char *next = 0, *last = 0;
	char *s;

	if (next + n > last) {
		next = malloc(ALSIZE);
		if (!next) scream("malloc fails in newstruct\n","");
		last = next + ALSIZE;
		}
	s = next;
	next += n;
	return s;
	}

char *
newstring(s)
char *s;
{
	static char *next = 0, *last = 0;
	char *s1;
	unsigned n, strlen();

	n = strlen(s) + 1;
	if (next + n > last) {
		next = malloc(ALSIZE);
		if (!next) scream("malloc fails in newstring\n","");
		last = next + ALSIZE;
		}
	s1 = next;
	next += n;
	strcpy(s1,s);
	return s1;
	}

newgroup(s)
char *s;
{
	struct group *g, *gnext = 0, *gprev;

	cobj = 0;
	gprev = (struct group *) &grouphash[hash(s)];
	g = gprev->next;
	for(;; gprev = g, g = g->next) {
		if (!g) {
			g = (struct group *)
				newstruct(sizeof(struct group));
			g->name = newstring(s);
			g->next = 0;
			g->gseq = ngroup++;
			gprev->next = g;
			break;
			}
		if (!strcmp(s,g->name)) break;
		}
	cgroup = g;
	}

struct ref *
lookup(s,add)
char *s;
int add;
{
	struct ref *h, *hprev;

	hprev = (struct ref *) &refhash[lasthash = hash(s)];
	h = hprev->next;
	while(h) {
		if (!strcmp(h->name,s)) return h;
		hprev = h;
		h = h->next;
		}
	if (add) {
		h = (struct ref *) newstruct(sizeof(struct ref));
		hprev->next = h;
		h->next = 0;
		h->name = cobj && !strcmp(cobj->name,s) ? cobj->name : newstring(s);
		h->robj = 0;
		}
	return h;
	}

newobj(s,dup)
char *s;
int dup;
{
	struct obj *h, *hnext = 0, *hprev, *o;
	struct ref *r1;

	hprev = (struct obj *) &objhash[hash(s)];
	h = hprev->next;
	while(h) {
		if (!strcmp(h->name,s) && h->ogroup == cgroup) {
			if (dup || h == obj0) { cobj = h; return; }
			if (verbose) {
				fprintf(stderr, "duplicate %s", s);
				if (*cgroup->name) fprintf(stderr, " in %s", cgroup->name);
				fputs(" ignored\n", stderr);
				}
			cobj = 0;
			return;
			}
		hprev = h;
		h = h->next;
		}
	cobj = o = (struct obj *) newstruct(sizeof(struct obj));
	hprev->next = o;
	o->next = hnext;
	r1 = lookup(s,0);
	o->name = r1 ? r1->name : newstring(s);
	o->ogroup = cgroup;
	o->seq = o->iptr = o->jct = 0;
	}

defobjref(s,dup)
char *s;
int dup;
{
	rdefobjref(lookup(s,1), s, dup);
	}

rdefobjref(r,s,dup)
struct ref *r;
char *s;
int dup;
{
	struct obj *o;

	if (o = r->robj) {
		newobj(s,dup);
		if (!dup || o->ogroup != cgroup) dupmsg(0,r,s);
		}
	else {
		r->robj = cobj = o = (struct obj *) newstruct(sizeof(struct obj));
		o->next = objhash[lasthash];
		objhash[lasthash] = o;
		o->name = r->name;
		o->ogroup = cgroup;
		o->seq = o->iptr = o->jct = 0;
		}
	}

dupmsg(oprt,r,s)
int oprt;
struct ref *r;
char *s;
{
	struct obj *o = r->robj;
	char *in;

	if (verbose && o != obj0) {
		fprintf(stderr, "def of %s", s);
		if (oprt && strcmp(s,cobj->name)) {
			fprintf(stderr, " in %s", cobj->name);
			in = "of";
			}
		else in = "in";
		if (*cgroup->name) fprintf(stderr, " %s lib %s", in, cgroup->name);
		else in = "of";
		fprintf(stderr, " ignored; first defined in %s", o->name);
		if (*(s = o->ogroup->name)) fprintf(stderr, " of lib %s", s);
		putc('\n', stderr);
		}
	}


newref(s)
char *s;
{ newrefr(lookup(s,1)); }

newrefr(r)
struct ref *r;
{
	struct refc rc;
	struct obj *o1;
	unsigned i;

	if (!cobj) return;
	if (verify && r->robj && cobj != obj0) {
		fputs(cobj->name, stderr);
		if (cgroup != group0) fprintf(stderr, " of %s", cgroup->name);
		fprintf(stderr, " needs %s\n", r->name);
		}
	rc.rcnext = cobj->iptr;
	rc.rcref = r;
	o1 = rc.rcref->robj;
	if (o1 != cobj && o1 != obj0) { /* discard self references */
									/* and refs to null object */
		i = cobj->iptr = ++nrefc;
		refstore(i, &rc);
		}
	}

defref(s)
char *s;
{ defrefr(lookup(s,1), s); }

defrefr(r,s)
struct ref *r;
char *s;
{
	if (!cobj) return;
	if (r->robj) dupmsg(1,r,s);
	else r->robj = cobj;
	}

struct obj *
ts0()
{
	int i;
	struct obj *firstobj, *o, *onext;
	struct ref *r, *rnext;

	/* chain objects together */

	firstobj = 0;
	for (i = 0; i < HSIZE; i++) {
		for(o = objhash[i]; o; o = onext) {
			onext = o->next;
			o->next = firstobj;
			firstobj = o;
			}
		}

	/* chain undefined refs together */

	defedrefs = undefrefs = 0;
	for (i = 0; i < HSIZE; i++) {
		for (r = refhash[i]; r; r = rnext) {
			rnext = r->next;
			if (!r->robj) { r->next = undefrefs; undefrefs = r; }
			else { r->next = defedrefs; defedrefs = r; }
			}
		}

	free((char *) grouphash);
	free((char *) refhash);
	free((char *) objhash);
	return firstobj;
	}

struct obj *
ts(all)
{
	int i, i0, k;
	unsigned j;
	struct obj *firstobj, *o, *o1, *onext, *oprev, **ready, **rs;
	struct refc rc;

	firstobj = ts0();

	/* do depth-first search (to break cycles) */

	nextseq = 1;
	for (o = firstobj; o; o = o->next)
		if (!o->seq && (all || o->ogroup == group0)) dfs(1,o);

	/* discard unreached objects */

	if (!all) {
		oprev = (struct obj *) &firstobj;
		for (o = firstobj; o; o = o->next)
			if (o->seq) { oprev->next = o; oprev = o; }
		oprev->next = 0;
		}

	/* set up for funny breadth-first search */

	ready = (struct obj **) malloc(ngroup * sizeof(struct obj *));
	if (!ready) scream("malloc of ready list fails\n","");
	
	obj0->jct = 0; /* force initial inclusion of obj0 */
	for (i = 0; i < ngroup; i++) ready[i] = 0;
	for (o = firstobj; o; o = onext) {
		onext = o->next;
		if (!o->jct) {
			rs = ready + o->ogroup->gseq;
			o->next = *rs;
			*rs = o;
			}
		}
	firstobj = lastobj = 0;
	i = i0 = 0;
	for(;;) {
		if (!(o = ready[i])) {
			i = i0;
			while(!(o = ready[i]))
				if (++i >= ngroup) {
					if (lastobj) lastobj->next = 0;
					return firstobj;
					}
			i0 = i;
			}
		ready[i] = o->next;
		if (!lastobj) firstobj = o;
		else lastobj->next = o;
		lastobj = o;
		for (j = o->iptr; j; ) {
			reffetch(j, &rc);
			j = rc.rcnext;
			if (j >= nrefc) { j -= nrefc; continue; }
			o1 = rc.rcref->robj;
			if (!o1) continue;
			if (!--o1->jct) {
				k = o1->ogroup->gseq;
				o1->next = ready[k];
				ready[k] = o1;
				if (i0 > k) i0 = k;
				}
			}
		}
	}

dfs(level, o)
int level;
struct obj *o;
{
	struct refc rc;
	struct obj *o1;
	int level0, s;
	static int cycle = 0;
	unsigned i, nextrc;

	o->seq = -level;
	level0 = -(level + 1);
	for (i = o->iptr; i; i = nextrc) {
		reffetch(i, &rc);
		nextrc = rc.rcnext;
		o1 = rc.rcref->robj;
		if (!o1 || o == o1 || o1 == obj0) continue;
		s = o1->seq;
		if (s >= 0) {
			o1->jct++;
			if (s > 0) continue; /* cross link */
			s = dfs(level+1, o1);
			}
		else { /* back link */
			if (cyclep && !cycle) {
				fprintf(stderr, "%s: cycle...\n", progname);
				cycle = 1;
				}
			rc.rcnext += nrefc;
			refstore(i, &rc);
			}
		if (level0 < s) level0 = s;
		}
	o->seq = nextseq;
	level = -level;
	if (level <= level0 && cyclep) {
		fprintf(stderr, "%s\n", o->name);
		if (level == level0) { cycle = 0; putc('\n',stderr); }
		else cycle = 1;
		}
	if (level > level0 || level == -1) { nextseq++; level0 = level; }
	return level0;
	}

scream(s, s1)
char *s, *s1;
{
	fprintf(stderr, "%s: ", progname);
	fprintf(stderr, s, s1);
	exit(1);
	}

undef(s,infile)
char *s;
FILE *infile;
{
	newgroup("");
	newobj("", 1);
	newref(s);
	}
