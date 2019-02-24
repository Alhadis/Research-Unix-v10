#ifdef BPRINT
#define dclproto(func,args) func args;
#else
#include "c.h"
#define FILE int
#define EOF -1
#endif

/*
 * read prof.out; format:
 *    #files
 *    name
 *    ... (#files-1 times)
 *    #functions
 *    name file# x y count
 *    ... (#functions-1 times)
 *    #points
 *    file# x y count
 *    ... (#points-1 times)
 */

struct file {
	struct file *link;
	char *name;
	int size, count;
	struct count {
		int x, y;
		int count;
	} *counts;
	struct func {
		struct func *link;
		char *name;
		struct count count;
	} *funcs;
} *filelist;

dclproto(static void afunction,(char *, char *, int, int, int))
dclproto(static void apoint,(int, char *, int, int, int))
dclproto(static struct file *findfile,(char *))
dclproto(static int gather,(FILE *))

#ifdef BPRINT
/* alloc - allocate n bytes or die */
static char *alloc(int n) {
	char *new = (char *)malloc(n);

	assert(new);
	return new;
}

/* string - save a copy of str, if necessary */
char *string(char *str) {
	static struct string { char *str; struct string *link; } *list;
	struct string *p;

	for (p = list; p; p = p->link)
		if (strcmp(p->str, str) == 0)
			return p->str;
	p = (struct string *)alloc(sizeof *p);
	p->str = strcpy((char *)alloc(strlen(str) + 1), str);
	p->link = list;
	list = p;
	return p->str;
}
#else
dclproto(extern void qsort,(struct count *, int, int, int (*)(const void *, const void *)))
dclproto(static int fscanf,(FILE *, char *, Generic, Generic, Generic, Generic, Generic))
dclproto(extern int sscanf,(char *, char *, Generic, Generic, Generic, Generic, Generic))

/* fscanf - read a line of input according to fmt */
static int fscanf(fp, fmt, a1, a2, a3, a4, a5) FILE *fp; char *fmt; Generic a1, a2, a3, a4, a5; {
	&fp;
	while (*cp) {
		if (limit - cp < MAXTOKEN)
			fillbuf();
		sscanf(cp, fmt, a1, a2, a3, a4, a5);
		while (*cp && *cp != '\n')
			cp++;
		if (*cp == '\n') {
			cp++;
			nextline();
		}
		return 0;
	}
	return EOF;
}
#endif

/* afunction - add function name and its data to file's function list */
static void afunction(name, file, x, y, count) char *name, *file; {
	struct file *p = findfile(file);
	struct func **q;

	assert(p);
	for (q = &p->funcs; *q && name != (*q)->name; q = &(*q)->link)
		;
	if (*q == 0) {
		*q = (struct func *)alloc(sizeof **q);
		(*q)->name = name;
		(*q)->count.x = x;
		(*q)->count.y = y;
		(*q)->count.count = 0;
		(*q)->link = 0;
	}
	(*q)->count.count += count;
}

/* apoint - append execution point i to file's data */ 
static void apoint(i, file, x, y, count) char *file; {
	struct file *p = findfile(file);

	assert(p);
	if (i >= p->size) {
		int j;
		if (p->size == 0) {
			p->counts = (struct count *)alloc(200*sizeof(struct count));
			p->size = 200;
		} else {
#ifdef BPRINT
			p->counts = (struct count *)realloc(p->counts, 2*p->size*sizeof(struct count));
#else
			struct count *new = (struct count *)alloc(2*p->size*sizeof(struct count));
			for (j = 0; j < p->count; j++)
				new[j] = p->counts[j];
			p->counts = new;
#endif
			p->size = 2*p->size;
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
static int gather(FILE *fp) {
	int i, nfiles, nfuncs, npoints;
	char *files[64], buf[120];

	if (fscanf(fp, "%d\n", &nfiles, 0, 0, 0, 0) == EOF)
		return 0;
	assert(nfiles < sizeof files/sizeof files[0]);
	for (i = 0; i < nfiles; i++) {
		fscanf(fp, "%s\n", buf, 0, 0, 0, 0);
		files[i] = string(buf);
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
	fscanf(fp, "%d\n", &nfuncs, 0, 0, 0, 0);
	for (i = 0; i < nfuncs; i++) {
		int f, x, y, count;
		fscanf(fp, "%s %d %d %d %d\n", buf, &f, &x, &y, &count);
		afunction(string(buf), files[f-1], x, y, count);
	}
	fscanf(fp, "%d\n", &npoints, 0, 0, 0, 0);
	for (i = 0; i < npoints; i++) {
		int f, x, y, count;
		fscanf(fp, "%d %d %d %d\n", &f, &x, &y, &count, 0);
		if (f)
			apoint(i, files[f-1], x, y, count);
	}
	return 1;
}

/* process - read prof.out data from file */
int process(file) char *file; {
	struct file *p;
	FILE *fp;

#ifdef BPRINT
	if ((fp = fopen(file, "r"))) {
		while (gather(fp))
			;
		fclose(fp);
#else
	int fd;
	if ((fd = open(file, 0)) >= 0) {
		inputInit(fd);
		while (gather(fp))
			;
		close(fd);
#endif
		for (p = filelist; p; p = p->link)
			qsort(p->counts, p->count, sizeof *p->counts,
#ifdef PROTO
				(int (*)(const void *, const void *))
#endif
				compare);
		return 1;
	}
	return 0;
}
