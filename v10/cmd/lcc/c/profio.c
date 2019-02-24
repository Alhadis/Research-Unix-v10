/* C compiler: prof.out input
 *
 * prof.out format:
 *    #files
 *    name
 *    ... (#files-1 times)
 *    #functions
 *    name file# x y count caller file x y 
 *    ... (#functions-1 times)
 *    #points
 *    file# x y count
 *    ... (#points-1 times)
 */

struct file {			/* per-file prof.out data: */
	struct file *link;		/* link to next file */
	char *name;			/* file name */
	int size;			/* size of counts[] */
	int count;			/* counts[0..count-1] hold valid data */
	struct count {			/* count data: */
		int x, y;			/* source coordinate */
		int count;			/* associated execution count */
	} *counts;
	struct func {			/* function data: */
		struct func *link;		/* link to next function */
		char *name;			/* function name */
		struct count count;		/* total number of calls */
		struct caller {		/* caller data: */
			struct caller *link;	/* link to next caller */
			char *name;		/* caller's name */
			char *file;		/* call site: file, x, y */
			int x, y;
			int count;		/* number of calls from this site */
		} *callers;
	} *funcs;			/* list of functions */
} *filelist;

#ifdef BPRINT
#define MAXTOKEN 64
#define dclproto(func,args) func args
#define ngetc() getc(fp)
static FILE *fp;
#else
#include "c.h"
#define EOF -1
#define ngetc() (*cp == 0 ? EOF : *cp == '\n' ? (cp++, nextline(), '\n') : *cp++)
dclproto(extern void qsort,(struct count *, int, int, int (*)(const void *, const void *)));
#endif

dclproto(static void acaller,(char *, char *, int, int, int, struct func *));
dclproto(static int compare,(struct count *, struct count *));
dclproto(static struct func *afunction,(char *, char *, int, int, int));
dclproto(static void apoint,(int, char *, int, int, int));
dclproto(static struct file *findfile,(char *));
dclproto(static int gather,(void));
dclproto(static int getd,(void));
dclproto(static char *getstr,(void));

/* acaller - add caller and site (file,x,y) to callee's callers list */
static void acaller(caller, file, x, y, count, callee) char *caller, *file; struct func *callee; {
	struct caller *q;

	assert(callee);
	for (q = callee->callers; q && (caller != q->name
		|| file != q->file || x != q->x || y != q->y); q = q->link)
		;
	if (!q) {
		struct caller **r;
		q = (struct caller *)alloc(sizeof *q);
		q->name = caller;
		q->file = file;
		q->x = x;
		q->y = y;
		q->count = 0;
		for (r = &callee->callers; *r && (strcmp(q->name, (*r)->name) > 0
			|| strcmp(q->file, (*r)->file) > 0 || q->y > (*r)->y || q->y > (*r)->y); r = &(*r)->link)
			;
		q->link = *r;
		*r = q;
	}
	q->count += count;
}

/* afunction - add function name and its data to file's function list */
static struct func *afunction(name, file, x, y, count) char *name, *file; {
	struct file *p = findfile(file);
	struct func *q;

	assert(p);
	for (q = p->funcs; q && name != q->name; q = q->link)
		;
	if (!q) {
		struct func **r;
		q = (struct func *)alloc(sizeof *q);
		q->name = name;
		q->count.x = x;
		q->count.y = y;
		q->count.count = 0;
		q->callers = 0;
		for (r = &p->funcs; *r && compare(&q->count, &(*r)->count) > 0; r = &(*r)->link)
			;
		q->link = *r;
		*r = q;
	}
	q->count.count += count;
	return q;
}

/* apoint - append execution point i to file's data */ 
static void apoint(i, file, x, y, count) char *file; {
	struct file *p = findfile(file);

	assert(p);
	if (i >= p->size) {
		int j;
		if (p->size == 0) {
			p->size = i >= 200 ? 2*i : 200;
			p->counts = (struct count *)alloc(p->size*sizeof(struct count));
		} else {
			struct count *new;
			p->size = 2*i;
			new = (struct count *)alloc(p->size*sizeof(struct count));
			for (j = 0; j < p->count; j++)
				new[j] = p->counts[j];
			p->counts = new;
		}
		for (j = p->count; j < p->size; j++) {
			static struct count z;
			p->counts[j] = z;
		}
	}
	p->counts[i].x = x;
	p->counts[i].y = y;
	p->counts[i].count += count;
	if (i >= p->count)
		p->count = i + 1;
}

/* compare - return <0, 0, >0 if a<b, a==b, a>b, resp. */
static int compare(a, b) struct count *a, *b; {
	if (a->y == b->y)
		return a->x - b->x;
	return a->y - b->y;
}

/* findcount - return count associated with (file,x,y) or -1 */
int findcount(file, x, y) char *file; {
	static struct file *cursor;

	if (cursor == 0 || cursor->name != file)
		cursor = findfile(file);
	if (cursor) {
		int l, u;
		struct count *c = cursor->counts;
		for (l = 0, u = cursor->count - 1; l <= u; ) {
			int k = (l + u)/2;
			if (c[k].y > y || c[k].y == y && c[k].x > x)
				u = k - 1;
			else if (c[k].y < y || c[k].y == y && c[k].x < x)
				l = k + 1;
			else
				return c[k].count;
		}
	}
	return -1;
}

/* findfile - return file name's file list entry, or 0 */
static struct file *findfile(name) char *name; {
	struct file *p;

	for (p = filelist; p; p = p->link)
		if (p->name == name)
			return p;
	return 0;
}

/* findfunc - return count associated with function name in file or -1 */
int findfunc(name, file) char *name, *file; {
	static struct file *cursor;

	if (cursor == 0 || cursor->name != file)
		cursor = findfile(file);
	if (cursor) {
		struct func *p;
		for (p = cursor->funcs; p; p = p->link)
			if (p->name == name)
				return p->count.count;
	}
	return -1;
}

/* gather - read prof.out data from fd */
static int gather() {
	int i, nfiles, nfuncs, npoints;
	char *files[64];

	if ((nfiles = getd()) < 0)
		return 0;
	assert(nfiles < sizeof files/sizeof files[0]);
	for (i = 0; i < nfiles; i++) {
		if ((files[i] = getstr()) == 0)
			return -1;
		if (!findfile(files[i])) {
			struct file *new = (struct file *)alloc(sizeof *new);
			new->name = files[i];
			new->size = new->count = 0;
			new->counts = 0;
			new->funcs = 0;
			new->link = filelist;
			filelist = new;
		}
	}
	if ((nfuncs = getd()) < 0)
		return -1;
	for (i = 0; i < nfuncs; i++) {
		struct func *q;
		char *name, *file;
		int f, x, y, count;
		if ((name = getstr()) == 0 || (f = getd()) <= 0
		|| (x = getd()) < 0 || (y = getd()) < 0 || (count = getd()) < 0)
			return -1;
		q = afunction(name, files[f-1], x, y, count);
		if ((name = getstr()) == 0 || (file = getstr()) == 0
		|| (x = getd()) < 0 || (y = getd()) < 0)
			return -1;
		if (*name != '?')
			acaller(name, file, x, y, count, q);
	}
	if ((npoints = getd()) < 0)
		return -1;
	for (i = 0; i < npoints; i++) {
		int f, x, y, count;
		if ((f = getd()) < 0 || (x = getd()) < 0 || (y = getd()) < 0
		|| (count = getd()) < 0)
			return -1;
		if (f)
			apoint(i, files[f-1], x, y, count);
	}
	return 1;
}

/* getd - read a non-negative number */
static int getd() {
	int c, n = 0;

	while ((c = ngetc()) != EOF && (c == ' ' || c == '\n' || c == '\t'))
		;
	if (c >= '0' && c <= '9') {
		do
			n = 10*n + (c - '0');
		while ((c = ngetc()) >= '0' && c <= '9');
		return n;
	}
	return -1;
}

/* getstr - read a string */
static char *getstr() {
	int c;
	char buf[MAXTOKEN], *s = buf;

	while ((c = ngetc()) != EOF && c != ' ' && c != '\n' && c != '\t')
		if (s - buf < sizeof buf - 2)
			*s++ = c;
	*s = 0;
	return s == buf ? (char *)0 : string(buf);
}

/* process - read prof.out data from file */
int process(file) char *file; {
	int more;

#ifdef BPRINT
	if ((fp = fopen(file, "r"))) {
		while ((more = gather()) > 0)
			;
		fclose(fp);
		return more < 0 ? more : 1;
	}
#else
	int fd;
	struct file *p;

	if ((fd = open(file, 0)) >= 0) {
		inputInit(fd);
		while ((more = gather()) > 0)
			;
		close(fd);
		if (more < 0)
			return more;
		for (p = filelist; p; p = p->link)
			qsort(p->counts, p->count, sizeof *p->counts,
				(dclproto(int (*),(const void *, const void *)))
				compare);
		
		return 1;
	}
#endif
	return 0;
}
