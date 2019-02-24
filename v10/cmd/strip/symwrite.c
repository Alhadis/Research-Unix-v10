#include "strip.h"
#include "hash.h"

symwrite(a)
register Adotout *a;
{
	int utmp;
	struct stx {
		char *lo, *hi;
		int base;
	} *stxtab, *stxend;
	char *rindex(), *mktemp(), *strcpy();

	register struct nlist *sym;
	register struct stx *stabx; register char *cp;

	struct st *stab; int strcount = 4;
	int tofd, error = 0;

	mktemp(strcpy(tmpnam, "/tmp/stripXXXXXX"));
	tofd = creat(tmpnam, 0666);
	if (tofd < 0)
		return failure("cannot creat ", tmpnam);

	lseek(a->fd, 0, 0);
	error = fcopy(tofd, a->fd, N_SYMOFF(a->hd));

	if (a->symtab) {
		stabx = stxtab = Malloc(struct stx, stcount);
		stxend = stxtab + stcount;
		for (stab = sroot; stab; stab = stab->next, stabx++) {
			stabx->lo = stab->strtab;
			stabx->hi = stabx->lo + stab->nused;
			stabx->base = (long)(stabx->lo) - strcount;
			strcount += stab->nused;
		}
		for (sym=a->symtab; sym<a->symend; sym++)
			if (cp = sym->n_un.n_name) {
				stabx = stxtab;
				while (cp < stabx->lo || cp >= stabx->hi) ++stabx;
				sym->n_un.n_name -= stabx->base;
			}
		free(stxtab);
		a->hd.a_syms = (char *)a->symend - (char *)a->symtab;
	} else
		a->hd.a_syms = 0;

	lseek(tofd, 0, 0);
	error |= Write(tofd, (char *)&a->hd, sizeof(struct exec));

	if (a->symtab) {
		lseek(tofd, N_SYMOFF(a->hd), 0);
		error |= Write(tofd, a->symtab, a->hd.a_syms);
		error |= Write(tofd, &strcount, 4);
		for (stab = sroot; stab; stab = stab->next)
			error |= Write(tofd, stab->strtab, stab->nused);
	}
	error |= close(tofd);
	close(a->fd);
	if (error)
		return failure("cannot write ", tmpnam);
	if ((a->fd = open(tmpnam, 0)) < 0)
		return failure("cannot reopen ", tmpnam);
	fstat(a->fd, &a->filstat);
	utmp = umask(0);
	tofd = creat(a->name, a->filstat.st_mode);
	(void)umask(utmp);
	if (tofd < 0)
		return failure("cannot creat ", a->name);
	++copyrace;
	fcopy(tofd, a->fd, a->filstat.st_size);
	close(tofd);
	close(a->fd);
	--copyrace;
	if (unlink(tmpnam) < 0)
		return failure("cannot unlink ", tmpnam);
	return 0;
}

failure(m1, m2)
char *m1, *m2;
{
	if (m1)
		fprintf(stderr,"%s: %s%s\n", prognam, m1, m2);
	if (!copyrace && tmpnam[0])
		unlink(tmpnam);
	return 1;
}
