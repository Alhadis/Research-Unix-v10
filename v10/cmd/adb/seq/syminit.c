/*
 * init the symbol table
 */

#include "defs.h"
#include "sym.h"
#include <a.out.h>
#include <stab.h>
#include <sys/types.h>

#define	NSYMS	300
#define	BIGBUF	(4*4096)

extern struct sym *symtab;

syminit(h)
struct exec *h;
{
	register struct nlist *q;
	register n, m;
	int strsiz;
	struct nlist space[BIGBUF/sizeof(struct nlist)];
	struct sym *sbuf;
	register struct sym *cur;
	struct sym *curgbl;
	register char *names;
	register int type, ltype;
	char *malloc();

	symtab = NULL;
	if (h->a_syms == 0)
		return;		/* stripped */
	lseek(fsym, (off_t)N_STROFF(*h), 0);
	if (read(fsym, &strsiz, sizeof(strsiz)) != sizeof(strsiz)) {
		printf("can't get string table size\n");
		return;
	}
	if ((names = malloc(strsiz)) == NULL) {
		printf("no mem for strings\n");
		return;
	}
	lseek(fsym, (off_t)N_STROFF(*h), 0);
	if (read(fsym, names, strsiz) != strsiz) {
		printf("error reading strings\n");
		return;
	}
	lseek(fsym, (off_t)N_SYMOFF(*h), 0);
	curgbl = sbuf = cur = NULL;
	for (n = h->a_syms; n > 0 ;) {
		m = read(fsym, (char *)space, min(n, sizeof(space)));
		if (m <= 0)
			break;
		n -= m;
		for (q = space; m > 0; q++, m-= sizeof(space[0])) {
			if (q->n_un.n_strx == 0)
				continue;
			switch (q->n_type) {
			case N_ABS:
			case N_ABS|N_EXT:
				type = S_ABS;
				break;

			case N_TEXT:
				if (names[q->n_un.n_strx] != '_')
					continue;
				/* fall in */
#if NOTDEF
			case N_BFUN:
#endif
			case N_TEXT|N_EXT:
				type = S_TEXT;
				break;

			case N_DATA:
			case N_DATA|N_EXT:
			case N_BSS:
			case N_BSS|N_EXT:
				type = S_DATA;
				break;

#if NOTDEF
			case N_LSYM:
				ltype = S_LSYM;
				type = S_STAB;
				break;

			case N_RSYM:
				ltype = S_RSYM;
				type = S_STAB;
				break;

			case N_PSYM:
				ltype = S_PSYM;
				type = S_STAB;
				break;

			case N_STSYM:
			case N_LCSYM:
				ltype = S_STSYM;
				type = S_STAB;
				break;
#endif

			default:
				continue;
			}
			if (sbuf == NULL || ++cur >= sbuf + NSYMS) {
				if ((sbuf = (struct sym *)malloc(sizeof(struct sym) * NSYMS)) == NULL) {
					printf("out of mem for syms\n");
					return;
				}
				cur = sbuf;
			}
			cur->y_type = type;
			cur->y_ltype = ltype;
			cur->y_value = q->n_value;
			cur->y_name = &names[q->n_un.n_strx];
			cur->y_locals = NULL;
			if (q->n_type == N_FUN) {
				cur->y_next = curgbl;
				curgbl = cur;
			}
			else if (cur->y_type == S_STAB) {
				if (curgbl == NULL)
					continue;
				cur->y_next = curgbl->y_locals;
				curgbl->y_locals = cur;
			}
			else {
				cur->y_next = symtab;
				symtab = cur;
			}
		}
	}
	for (; curgbl; curgbl = curgbl->y_next) {
		if (curgbl->y_locals == NULL)
			continue;
		for (cur = symtab; cur; cur = cur->y_next) {
			if (cur->y_type != S_TEXT)
				continue;
			if (cur->y_value == curgbl->y_value) {
				cur->y_locals = curgbl->y_locals;
				break;
			}
		}
	}
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

static
min(a, b)
{
	if (a < b)
		return (a);
	else
		return (b);
}
