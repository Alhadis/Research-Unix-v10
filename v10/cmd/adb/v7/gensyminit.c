/*
 * set up the symbol table
 * cheap hack version: let nm do the work
 */

#include "defs.h"
#include <a.out.h>
#include "sym.h"
#include <stdio.h>

#define	NMCMD	"PATH=/bin:/usr/bin: nm -p %s"
#define	NMFMT	"%lx %s %s"	/* value type name */

#define	NSYMS	20

extern char *symfil;

syminit(h)
struct exec *h;
{
	FILE *fp;
	register struct sym *p;
	struct sym *syms;
	char buf[ARB];
	long val;
	char type[10];
	char name[ARB];
	char *malloc();
	FILE *popen();

	symtab = NULL;
	if (h->a_syms == 0)
		return;		/* stripped */
	syms = NULL;
	sprintf(buf, NMCMD, symfil);
	if ((fp = popen(buf, "r")) == NULL)
		return;
	while (fgets(buf, ARB, fp)) {
		if (sscanf(buf, NMFMT, &val, type, name) != 3)
			continue;
		if (type[0] != 'A' && type[0] != 'D' && type[0] != 'B' && type[0] != 'T')
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
		strcpy(p->y_name, name);
		p->y_value = val;
		switch (type[0]) {
		case 'A':
			p->y_type = S_ABS;
			break;

		case 'B':
		case 'D':
			p->y_type = S_DATA;
			break;


		case 'T':
			p->y_type = S_TEXT;
			break;
		}
		p->y_next = symtab;
		symtab = p;
		p++;
	}
	while (fgets(buf, ARB, fp))
		;
	pclose(fp);
}

/*
 * is symbol table entry s == name n?
 * this may depend on awful symbol conventions
 * e.g. _
 */
int
eqsym(s, n)
register struct sym *s;
char *n;
{

	if (strcmp(s->y_name, n) == 0)
		return (1);
	if (s->y_name[0] == '_' && strcmp(&s->y_name[1], n) == 0)
		return (1);
	return (0);
}
