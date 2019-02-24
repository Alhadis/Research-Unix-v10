#include	<stdio.h>
#include	<ctype.h>
#include	"pic.h"
#include	"y.tab.h"
extern	char	*tostring();

getvar(s)	/* return value of variable s */
char *s;
{
	struct symtab *p;

	p = lookup(s);
	if (p == NULL) {
		if (islower(s[0]))
			yyerror("no such variable as %s", s);
		else
			yyerror("no such place as %s", s);
		return(0);
	}
	return(p->s_val);
}

struct symtab *makevar(s, t, v)	/* make variable named s in table */
char *s;			/* assumes s is static or from tostring */
int t;
int v;
{
	int i;
	struct symtab *p;

	for (p = stack[nstack].p_symtab; p != NULL; p = p->s_next)
		if (strcmp(s, p->s_name) == 0)
			break;
	if (p == NULL) {	/* it's a new one */
		p = (struct symtab *) malloc(sizeof(struct symtab));
		if (p == NULL) {
			yyerror("out of symtab space with %s", s);
			exit(1);
		}
		p->s_next = stack[nstack].p_symtab;
		stack[nstack].p_symtab = p;	/* stick it at front */
	}
	p->s_name = s;
	p->s_type = t;
	p->s_val = v;
	return(p);
}

struct symtab *lookup(s)	/* find s in symtab */
char *s;
{
	int i;
	struct symtab *p;

	for (i = nstack; i >= 0; i--)	/* look in each active symtab */
		for (p = stack[i].p_symtab; p != NULL; p = p->s_next)
			if (strcmp(s, p->s_name) == 0)
				return(p);
	return(NULL);
}

freesymtab(p)	/* free space used by symtab at p */
struct symtab *p;
{
	for ( ; p != NULL; p = p->s_next) {
		free(p->s_name);	/* assumes done with tostring */
		free(p);
	}
}
