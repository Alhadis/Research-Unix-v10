#include <sys/types.h>
#include <a.out.h>
#include <stdio.h>

/*
 * nlist - retrieve attributes from name list (string table version)
 * tangled in search of speed and buffering
 */

#define	NBSIZE	32768

nlist(name, list)
	char *name;
	struct nlist *list;
{
	register struct nlist *p, *q;
	register n, m;
	int max, nreq;
	int f, g;
	off_t ss;		/* start of strings */
	off_t sc;		/* current string offset */
	register int soff;	/* current place in strings buffer */
	int smax;		/* number of bytes in strungs buffer */
	struct exec buf;
	struct nlist space[BUFSIZ/sizeof(struct nlist)];
	char names[NBSIZE+2];

	names[NBSIZE] = 'X';	/* border protection */
	names[NBSIZE+1] = 0;
	max = 0;
	for (q = list, nreq = 0; q->n_un.n_name && q->n_un.n_name[0]; q++, nreq++) {
		q->n_type = 0;
		q->n_value = 0;
		q->n_desc = 0;
		q->n_other = 0;
		if ((n = strlen(q->n_un.n_name)) > max)
			max = n;
	}
	if ((f = open(name, 0)) < 0)
		return (-1);
	if (read(f, (char *)&buf, sizeof(buf)) != sizeof(buf)
	||  N_BADMAG(buf)) {
		close(f);
		return (-1);
	}
	if ((g = open(name, 0)) < 0) {
		close(f);
		return (-1);
	}
	lseek(f, (off_t)N_SYMOFF(buf), 0);
	ss = N_SYMOFF(buf) + buf.a_syms;
	sc = 0;
	smax = 0;
	for (n = buf.a_syms; n > 0 && nreq > 0;) {
		m = read(f, (char *)space, sizeof(space));
		if (m <= 0)
			goto out;
		n -= m / sizeof(struct nlist);
		for (q = space; nreq > 0 && m > 0; q++, m-= sizeof(space[0])) {
			if (q->n_un.n_strx == 0 || q->n_type & N_STAB)
				continue;
			if (sc <= q->n_un.n_strx
			&&  q->n_un.n_strx + max <= sc + smax)
				soff = q->n_un.n_strx - sc;
			else {
				lseek(g, ss+q->n_un.n_strx, 0);
				smax = read(g, names, NBSIZE);
				if (smax <= 0)
					goto out;
				sc = q->n_un.n_strx;
				soff = 0;
			}
			for (p = list; p->n_un.n_name[0]; p++) {
				if (strcmp(p->n_un.n_name, &names[soff]) == 0) {
					p->n_value = q->n_value;
					p->n_type = q->n_type;
					p->n_desc = q->n_desc;
					p->n_other = q->n_other;
					--nreq;
					break;
				}
			}
		}
	}
out:
	close(f);
	close(g);
	return (0);
}

