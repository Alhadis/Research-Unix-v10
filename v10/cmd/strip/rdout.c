#include "strip.h"

Adotout *
rdout(name, sflag)
register char *name;
{
	register Adotout *a; Adotout *rderr();
	if ((a = Calloc(Adotout, 1)) == 0)
		return rderr(a, name);
	a->name = name;
	if ((a->fd = open(a->name, 0)) < 0)
		return rderr(a, "cannot open file");
	fstat(a->fd, &a->filstat);
	if (Read(a->fd, (char *)&a->hd, sizeof(struct exec)))
		return rderr(a, "cannot read header");
	if (N_BADMAG(a->hd))
		return rderr(a, "bad magic number");

	if (a->nsymbols = a->hd.a_syms/sizeof(struct nlist)) {
		lseek(a->fd, N_STROFF(a->hd), 0);
		if (Read(a->fd, (char *)&a->strsize, sizeof(long)) ||
		    a->strsize < 4)
			return rderr(a, "read failed on string table size");
	}

	if (sflag && a->nsymbols) {
		if ((a->symtab = Malloc(struct nlist, a->nsymbols+1)) == 0)
			return rderr(a, "cannot malloc symbol table");
		lseek(a->fd, N_SYMOFF(a->hd), 0);
		if (Read(a->fd, (char *)a->symtab, a->hd.a_syms))
			return rderr(a, "read failed on symbol table");
		a->symend = (struct nlist *)((char *)a->symtab + a->hd.a_syms);
		a->symend->n_un.n_name = 0;
		a->symend->n_type = 0;
		a->symend->n_other = 0;
		a->symend->n_desc = 0;
		a->symend->n_value = 0;

		if ((a->strtab = Malloc(char, a->strsize)) == 0)
			return rderr(a, "cannot malloc string table");
		lseek(a->fd, N_STROFF(a->hd), 0);
		if (Read(a->fd, (char *)a->strtab, a->strsize))
			return rderr(a, "read failed on string table");
	}
	return a;
}

freeout(a)
register Adotout *a;
{
	if (a) {
		if (a->fd)
			close(a->fd);
		Free(a->symtab);
		Free(a->strtab);
		free(a);
	}
}

Adotout *
rderr(a, s)
register Adotout *a; register char *s;
{
	if (a) {
		fprintf(stderr, "%s: file %s\n", s, a->name);
		freeout(a);
	} else {
		fprintf(stderr, "cannot allocate struct Adotout: file %s\n", s);
	}
	return 0;
}

prtout(a)
register Adotout *a;
{
	printf("magic number	= 0%o\n", a->hd.a_magic);
	printf("text size	= %d	= 0x%x\n", a->hd.a_text, a->hd.a_text);
	printf("data size	= %d	= 0x%x\n", a->hd.a_data, a->hd.a_data);
	printf("bss size	= %d	= 0x%x\n", a->hd.a_bss, a->hd.a_bss);
	printf("sym tab size	= %d (%d)\n", a->hd.a_syms, a->nsymbols);
	printf("entry point	= 0x%x\n", a->hd.a_entry);
	printf("text rel size	= %d\n", a->hd.a_trsize);
	printf("data rel size	= %d\n", a->hd.a_drsize);
	printf("str tab size	= %d\n", a->strsize);
}
