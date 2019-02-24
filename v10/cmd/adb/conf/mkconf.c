#include <stdio.h>
#include <ctype.h>

#define	NFILES	500
#define	NRULES	50
#define	NTAGS	50
#define	MAXFLDS	20
#define	MAXRULES 10

#define	ARB	200

typedef struct file {
	char *f_obj;
	char *f_src;
	char *f_type;
} File;

File files[NFILES];

typedef struct rule {
	char *r_type;
	char *r_dep;
	char *r_rules[MAXRULES];
} Rule;

Rule rules[NRULES];

char *wanted[NTAGS];

main(argc, argv)
int argc;
char **argv;
{
	char *templ, *flist, *out;
	register char **p;

	/* hack */
	templ = "templ";
	flist = "files";
	out = NULL;
	p = wanted;
	while (--argc > 0) {
		if (p >= &wanted[NTAGS-1]) {
			fprintf(stderr, "mkconf: too many tags\n");
			exit(1);
		}
		*p++ = *++argv;
	}
	*p = NULL;
	readlist(flist);
	putmake(templ, out);
	exit(0);
}

/*
 * read the list of files and stuff
 * colon-separated fields:
 *	object: sources: type: tags
 */

readlist(list)
char *list;
{
	FILE *fp;
	register File *f;
	char *fld[MAXFLDS];
	char buf[ARB], xbuf[ARB];
	char *save();

	if ((fp = fopen(list, "r")) == NULL) {
		fprintf(stderr, "mkconf: %s: cannot open\n", list);
		exit(1);
	}
	f = files;
	while (getline(buf, ARB, fp)) {
		if (buf[0] == 0 || buf[0] == '#')
			continue;
		strcpy(xbuf, buf);
		if (crack(xbuf, ':', fld) != 4) {
			fprintf(stderr, "bad files line: %s\n", buf);
			continue;
		}
		if (wanttag(fld[3]) == 0)
			continue;
		if (f >= &files[NFILES-1]) {
			fprintf(stderr, "mkconf: too many files\n");
			exit(1);
		}
		f->f_obj = save(fld[0]);
		f->f_src = save(fld[1]);
		f->f_type = save(fld[2]);
		f++;
	}
	f->f_obj = NULL;
	fclose(fp);
}

wanttag(tags)
char *tags;
{
	char *t[MAXFLDS];
	register int i, j;
	register int n;

	detab(tags);
	n = crack(tags, ' ', t);
	for (i = 0; wanted[i]; i++)
		for (j = 0; j < n; j++)
			if (strcmp(wanted[i], t[j]) == 0)
				return (1);
	return (0);
}

/*
 * print the makefile
 *	read the template, store the rules
 *	spit out the literal stuff
 *	spit out the rules
 */

putmake(templ, out)
char *templ, *out;
{
	FILE *tf, *of;
	char buf[ARB];
	register File *f;

	if ((tf = fopen(templ, "r")) == NULL) {
		fprintf(stderr, "mkconf: %s: cannot open\n", templ);
		exit(1);
	}
	if (out == NULL)
		of = stdout;
	else if ((of = fopen(out, "w")) == NULL) {
		fprintf(stderr, "mkconf: %s: cannot create\n", out);
		exit(1);
	}
	getrules(tf);
	fprintf(of, "FILES=");
	for (f = files; f->f_obj; f++)
		fprintf(of, " %s", f->f_obj);
	putc('\n', of);
	while (getline(buf, ARB, tf))
		fprintf(of, "%s\n", buf);
	fclose(tf);
	for (f = files; f->f_obj; f++) {
		fprintf(of, "%s: %s", f->f_obj, f->f_src);
		putrule(f, of);
	}
	fclose(of);
}

getrules(tf)
FILE *tf;
{
	char buf[ARB];
	register Rule *r;
	register int i;
	register char *s;
	char *strchr();
	char *save();

	r = NULL;
	while (getline(buf, ARB, tf)) {
		if (buf[0] == '%' && buf[1] == '%' && buf[2] == 0)
			break;
		if (buf[0] == 0 || buf[0] == '#')
			continue;
		if (buf[0] != '\t') {
			if (r == NULL)
				r = rules;
			else if (++r >= &rules[NRULES-1]) {
				fprintf(stderr, "mkconf: too many object types: %s\n", buf);
				exit(1);
			}
			if ((s = strchr(buf, ':')) == NULL) {
				fprintf(stderr, "mkconf: bad line in template: %s\n", buf);
				continue;
			}
			if (*s)
				*s++ = 0;
			while (isspace(*s))
				s++;
			r->r_type = save(buf);
			r->r_dep = save(s);
			i = 0;
			continue;
		}
		if (r == NULL) {
			fprintf(stderr, "mkconf: bad line in template: %s\n", buf);
			continue;
		}
		if (i >= MAXRULES - 1) {
			fprintf(stderr, "mkconf: too many lines in template: %s\n", buf);
			exit(1);
		}
		r->r_rules[i] = save(buf);
	}
}

putrule(f, of)
File *f;
FILE *of;
{
	register Rule *r;
	register int i;
	char buf[ARB];
	char *deps[MAXFLDS];
	int n;

	for (r = rules; r->r_type; r++)
		if (strcmp(r->r_type, f->f_type) == 0)
			break;
	if (r->r_type == NULL) {
		putc('\n', of);
		return;
	}
	strcpy(buf, f->f_src);
	strcat(buf, " ");
	strcat(buf, r->r_dep);
	detab(buf);
	n = crack(buf, ' ', deps);
	fprintf(of, " %s\n", r->r_dep);
	for (i = 0; r->r_rules[i]; i++)
		expout(of, r->r_rules[i], deps, n);
}

/*
 * print a line of a rule
 * $n expands to the nth arg (source file)
 */

expout(fp, s, args, n)
FILE *fp;
register char *s;
register char **args;
int n;
{
	register int i;

	for (; *s; s++) {
		if (*s == '$' && isdigit(s[1])) {
			for (s++, i = 0; isdigit(*s); s++)
				i = i * 10 + *s - '0';
			s--;
			i--;
			if (i >= 0 && i < n)
				fprintf(fp, "%s", args[i]);
			continue;
		}
		putc(*s, fp);
	}
	putc('\n', fp);
}

getline(buf, n, fp)
char *buf;
register int n;
FILE *fp;
{
	register int c;
	register char *p;

	p = buf;
	while (--n >= 0) {
		if ((c = getc(fp)) == EOF || c == '\n')
			break;
		*p++ = c;
	}
	*p = 0;
	if (c == EOF && p == buf)
		return (0);
	return (1);
}

char *
save(s)
char *s;
{
	char *ss;
	char *malloc();

	if ((ss = malloc(strlen(s) + 1)) == NULL) {
		fprintf(stderr, "mkconf: out of mem\n");
		exit(1);
	}
	strcpy(ss, s);
	return (ss);
}

detab(s)
register char *s;
{

	for (; *s; s++)
		if (*s == '\t')
			*s = ' ';
}


/*
 * split a line into fields
 *	s is the line; it is destroyed
 *	c is the character that marks the end of a field;
 *	it may be followed by blanks and tabs
 *	a is an array of pointers to the fields, NULL terminated
 *	the number of fields is returned
 *	there are at most MAXFLDS fields
 */

int
crack(s, c, a)
register char *s;
register char c;
register char **a;
{
	register int n;

	n = 0;
	for (;;) {
		while (*s && isspace(*s))
			s++;
		if (*s == 0)
			break;
		if (n >= MAXFLDS - 1)
			break;	/* and lose the remainder.  oh well. */
		*a++ = s;
		n++;
		while (*s && *s != c)
			s++;
		if (*s)
			*s++ = 0;
	}
	*a = NULL;
	return (n);
}
