/* symbol.c:
	General: functions for mapping {string} x {int} -> {int}
	In pass 1: {name} x {viewnum} -> {int file line num}
	In pass 2: {} x {int file line num} -> {slot num}
		   Therefore be careful with blank strings
 */

#include "fdevelop.h"

#define	steq(s, n, p) (p->innum == n && ((p->instr == s) || eq(p->instr,s)))

#define SIZE	2053
Symbol	*head[SIZE];

int hash(s, n)	/* form hash value */
	register char *s;
	register int n;
{
	register int hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = (*s + 31 * hashval) % SIZE;
	hashval = (31 * hashval + 1259 * n) % SIZE;
	return hashval;
}

Symbol *lookup(s, n)	/* return element with s, n */
	register char *s;
	register int n;
{
	register Symbol *p;
	for (p = head[hash(s, n)]; p != NULL; p = p->next)
		if (steq(s, n, p))
			return p;
	return NULL;
}

insert(s, n, v)	/* insert s, n with value v */
	register char *s;
	register int n;
	int v;
{
	register Symbol *p;
	char *q;
	int i;

	/* fprintf(stderr, "Inserting: |%s|, %d with hash %d\n",
		s, n, hash(s,n)); */
	if (*s == '\0')
		q = NULLSTR;
	else {
		q = emalloc(strlen(s)+1);
		strcpy(q, s);
	}
	p = (Symbol *) emalloc(sizeof(Symbol));
	p->instr = q;
	p->innum = n;
	p->outnum = v;
	i = hash(s, n);
	p->next = head[i];
	head[i] = p;
}

delete(s, n)	/* remove s, n */
	char *s;
	int n;
{
	Symbol *p, *pp;
	int i;

	/* fprintf(stderr, "Deleting:  |%s|, %d with hash %d\n",
		s, n, hash(s,n)); */
	i = hash(s, n);
	pp = NULL;
	for (p = head[i]; p != NULL; p = p->next) {
		if (steq(s, n, p))
			break;
		pp = p;
	}
	if (p == NULL)
		error(FATAL, "symtab bug: bad delete");
	if (p->instr != NULLSTR)
		efree(p->instr);
	if (pp == NULL) {
		head[i] = p->next;
	} else {
		pp->next = p->next;
	}
	efree((char *) p);
}

opensymtab()	/* init table */
{
	int i;

	for (i = 0; i < SIZE; i++)
		head[i] = NULL;
}

closesymtab()	/* reclaim storage */
{
	int i;
	Symbol *p, *np;

	for (i = 0; i < SIZE; i++)
		for (p = head[i]; p != NULL; p = np) {
			if (p->instr != NULLSTR)
				efree(p->instr);
			np = p->next;
			efree((char *) p);
		}
}
