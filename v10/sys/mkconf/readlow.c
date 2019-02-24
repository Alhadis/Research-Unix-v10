/*
 * read template for trap and interrupt vectors
 *	offset	type	name
 */	

#include <fio.h>
#include <libc.h>
#include <ctype.h>
#include "mkconf.h"

readlow(f)
char *f;
{
	int fd;
	register char *p;
	char *args[4];
	int off;

	if ((fd = open(f, 0)) < 0) {
		perror(f);
		exit(1);
	}
	while ((p = Frdline(fd)) != NULL) {
		while (isspace(*p))
			p++;
		if (*p == 0 || *p == '#')
			continue;
		if (crack(p, args, 4) != 3) {
			fprint(STDERR, "bad scb line: %s\n", p);
			errs++;
			continue;
		}
		off = nconv(args[0]);
		switch (args[1][0]) {
		case 't':
			newvec(off, STRAP, args[2]);
			break;

		case 'i':
			newvec(off, SINTR, args[2]);
			break;

		default:
			fprint(STDERR, "scb offset %s: unknown flag %s\n",
				args[0], args[1]);
			errs++;
			break;
		}
	}
	close(fd);
}

/*
 * new interrupt vector into scb list
 * the hard part is keeping the list ordered
 */
newvec(off, typ, name)
int off;
int typ;
char *name;
{
	register Scb *sp;
	register Scb *tp, *bp;

	sp = (Scb *)ealloc(sizeof(Scb));
	sp->name = estrdup(name);
	sp->type = typ;
	sp->offset = off;
	bp = NULL;
	for (tp = llist; tp; bp = tp, tp = tp->next) {
		if (tp->offset == sp->offset) {
			fprint(STDERR, "duplicate scb vector 0x%x %s %s\n",
				sp->offset, sp->name, tp->name);
			errs++;
			return;
		}
		if (tp->offset > sp->offset)
			break;
	}
	if (bp == NULL) {
		sp->next = llist;
		llist = sp;
	}
	else {
		sp->next = bp->next;
		bp->next = sp;
	}
}
