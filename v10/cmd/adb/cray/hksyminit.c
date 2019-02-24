/*
 * set up the symbol table
 * very cheap hack version for cray:
 * read the loader map
 */

#include "defs.h"
#include "sym.h"
#include <stdio.h>
#include "machine.h"

#define	NMFMT	"%s %lo%c"	/* name value (junk) */

#define	NSYMS	20

static lcopy();

/*
 * `fn' is the name of the a.out file.
 * if it was a valid a.out, look for a map in `fn'.map
 * else see if fn itself is a map
 */

hksyminit(fn, isexec)
char *fn;
int isexec;
{
	FILE *fp;
	register struct sym *p;
	struct sym *syms;
	char buf[ARB];
	long val;
	char flag;
	char name[ARB];
	int inmap = 0;
	char *malloc();

	symtab = NULL;
	strcpy(name, fn);
	if (isexec)
		strcat(name, ".map");
	if ((fp = fopen(name, "r")) == NULL)
		return;
	if (fgets(buf, ARB, fp) == NULL
	||  (buf[0] != '1' || buf[2] != ' ')) {
		fclose(fp);
		return;		/* probably not a loader map */
	}
	syms = NULL;
	while (fgets(buf, ARB, fp)) {
		if (inmap == 0) {
			if (maphdr(buf) == 0)
				continue;
			inmap++;
		}
		if (sscanf(buf, NMFMT, name, &val, &flag) != 3)
			continue;
		if (!isalpha(name[0]))
			continue;
		if (syms == NULL || p >= syms + NSYMS) {
			if ((syms = (struct sym *)malloc(NSYMS * sizeof(struct sym))) == NULL) {
				printf("out of mem for syms");
				break;
			}
			p = syms;
		}
		if ((p->y_name = malloc(strlen(name) + 1)) == NULL) {
			printf("out of mem for syms");
			break;
		}
		lcopy(p->y_name, name);
		p->y_value = watoba(val);
		if (flag == ' ')
			p->y_type = S_DATA;
		else
			p->y_type = S_TEXT;	/* parcel addr */
		p->y_next = symtab;
		symtab = p;
		p++;
	}
	fclose(fp);
}

/*
 * look for the beginning of the interesting part
 * this is just to cut down on specious syms
 */

maphdr(s)
register char *s;
{

	while (*s == ' ')
		s++;
	return (strncmp(s, "ENTRY POINT CROSS-", 18) == 0);
}

static
lcopy(s, t)
register char *s, *t;
{

	while (*t) {
		if (*t == '$') {
			*s++ = '_';
			t++;
		}
		else
			*s++ = *t++;
	}
	*s = 0;
}

/*
 * compare sym entry name with one the user typed
 */
eqsym(sp, n)
struct sym *sp;
char *n;
{

	return (strncmp(sp->y_name, n, 8) == 0);
}
