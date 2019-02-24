#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include "filehdr.h"
#include "aouthdr.h"
#include "scnhdr.h"
#include "storclass.h"
#include "syms.h"

/* global */
int nflg, gflg, uflg, rflg, pflg, oflg, aflg;
#define SYMINC(sp,n) (struct syment *)(((u_int)sp)+(n)*SYMESZ)
#define AUXINC(ap,n) (struct auxent *)(((u_int)ap)+(n)*SYMESZ)
char section[256];	/* section types ('T', 'D', 'B', or '?') */
struct symbol {
	char *name;
	u_long value;
	char sclass;
};

/* predeclared */
struct symbol *dosymtab();
char *dofile();
char *namelist();

main(argc, argv)
char **argv;
{
	int errs;
	char *rv;
	int printname;

	rflg = 1;
	if (--argc>0 && argv[1][0]=='-' && argv[1][1]!=0) {
		argv++;
		while (*++*argv) switch (**argv) {

		case 'n':
			nflg = 1;
			continue;
		case 'g':
			gflg = 1;
			continue;
		case 'u':
			uflg = 1;
			continue;
		case 'r':
			rflg = -1;
			continue;
		case 'p':
			pflg = 1;
			continue;
		case 'o':
			oflg = 1;
			continue;
		case 'a':
			aflg = 1;
			continue;
		default:
			fprintf(stderr, "nm: invalid argument -%c\n",
			    *argv[0]);
			exit(2);
		}
		argc--;
	}
	if (argc == 0) {
		argc = 1;
		argv[1] = "a.out";
	}
	printname = argc > 1 && !oflg;
	while (argc--) {
		if (printname)
			printf("%s:\n", argv[1]);
		if ((rv = dofile(argv[1])) != NULL) {
			errs++;
			fprintf(stderr, "nm: %s (%s)\n", rv, argv[1]);
		}
		argv++;
	}
	exit(errs);
}

char *
dofile(file)
	char *file;
{
	int fd;
	char *rv;

	if ((fd = open(file, 0)) < 0)
		return "can't open";
	rv = namelist(file, fd);
	close(fd);
	return rv;
}

char *
namelist(file, fd)
	char *file;
	int fd;
{
	struct filehdr fh;
	struct aouthdr ah;
	struct syment *syms;
	struct symbol *symtab;
	struct scnhdr sh;
	char *strings;
	struct stat sbuf;
	unsigned int nel, i, size;

	/* get file length */
	if (fstat(fd, &sbuf) < 0)
		return "problem stating";

	/* get file header and a.out header */
	if (read(fd, &fh, sizeof(fh)) != sizeof(fh))
		return "problem reading";
	if (read(fd, &ah, sizeof(ah)) != sizeof(ah))
		return "error reading a.out header";
	if (fh.f_magic != FBOMAGIC)
		return "bad magic";

	/* read and classify section headers */
	for (i = 1; i <= fh.f_nscns; i++) {
		if (read(fd, &sh, sizeof(sh)) != sizeof(sh))
			return "error reading section header";
		if (strncmp(sh.s_name, _TEXT, sizeof(sh.s_name)) == 0)
			section[i] = 'T';
		else if (strncmp(sh.s_name, _DATA, sizeof(sh.s_name)) == 0)
			section[i] = 'D';
		else if (strncmp(sh.s_name, _BSS, sizeof(sh.s_name)) == 0)
			section[i] = 'B';
		else 
			section[i] = 'O';
	}

	/* read in the symbols */
	size = fh.f_nsyms*SYMESZ;
	if (lseek(fd, fh.f_symptr, 0) < 0)
		return "problem seeking ";
	syms = (struct syment *)malloc(size);
	if (syms == NULL)
		return "not enough memory";
	if (read(fd, syms, size) != size) {
		free(syms);
		return "problem reading";
	}

	/* read in string table */
	size = sbuf.st_size - size - fh.f_symptr;
	strings = (char *)malloc(size);
	if (strings == NULL) {
		free(syms);
		return "not enough memory";
	}
	if (read(fd, strings, size) != size) {
		free(syms); free(strings);
		return "problem reading";
	}

	/* create symbol table */
	symtab = dosymtab(syms, strings, fh.f_nsyms, &nel);
	if (symtab == NULL) {
		free(syms); free(strings);
		return "not enough memory";
	}

	/* sort it */
	if (!pflg) {
		int ncompare(), scompare();
		if (nflg)
			qsort(symtab, nel, sizeof(struct symbol), ncompare);
		else
			qsort(symtab, nel, sizeof(struct symbol), scompare);
	}

	/* print out symbols */
	doprint(file, symtab, nel);
	free(syms);
	free(strings);
	free(symtab);
	return NULL;
}

struct symbol *
dosymtab(syms, strings, nsyms, np)
	struct syment *syms;
	char *strings;
	unsigned int nsyms;
	unsigned int *np;
{
	struct symbol *symtab, *stp;
	struct syment *sp;
	int nel=0;

	/* build table */
	stp = symtab = (struct symbol *)malloc(nsyms*sizeof(struct symbol));
	if (symtab == NULL)
		return NULL;
	for (sp = syms; sp < SYMINC(syms,nsyms); sp=SYMINC(sp, sp->n_numaux+1)) {
		switch(sp->n_sclass) {
		case C_REG: case C_REGPARM:
			if (!aflg || uflg || gflg) continue;
			stp->sclass = 'r';
			break;
		case C_AUTO: case C_MOS: case C_ARG:
		case C_MOU:  case C_MOE: case C_FIELD:
			if (!aflg || uflg || gflg) continue;
			stp->sclass = 'a';
			break;
		case C_EXT:
			stp->sclass = class(sp);
			if (uflg && stp->sclass != 'U') continue;
			break;
		case C_STAT: case C_HIDDEN:
			if (uflg || gflg) continue;
			stp->sclass = tolower(class(sp));
			break;
		case C_USTATIC:
			if (!aflg || gflg) continue;
			stp->sclass = 'u';
			break;
		case C_LABEL: case C_BLOCK: case C_FCN:
			if (!aflg || uflg || gflg) continue;
			stp->sclass = tolower(class(*symtab));
			break;
		case C_ULABEL:
			if ((!aflg && !uflg) || gflg) continue;
			stp->sclass = 'u';
			break;
		case C_EXTDEF:
			if (uflg) continue;
			stp->sclass = class(*symtab);
			break;
		case C_NULL: case C_STRTAG:
		case C_UNTAG: case C_TPDEF: case C_ENTAG: case C_EOS:
			if (!aflg || uflg || gflg) continue;
			stp->sclass = 'a';
			break;
		case C_FILE:
			if (uflg) continue;
			stp->sclass = 'f';
			if (sp->n_numaux > 0) {
				struct auxent *ap;
				ap = AUXINC(sp, 1);
				stp->name = ap->x_file.x_fname;
				ap->x_file.x_fname[sizeof(ap->x_file.x_fname)] = 0;
			}
			break;
		default:
			continue;
		}
		stp->value = sp->n_value;
		if (stp->sclass != 'f') {
			if (sp->n_zeroes == 0) {
				/* long name */
				stp->name = strings + sp->n_offset;
			} else {
				/* short name */
				stp->name = sp->n_name;
				sp->n_name[sizeof(sp->n_name)] = '\0';
			}
			if (*(stp->name) == '.') continue;
		}
		stp++;
	}
	*np = stp - symtab;
	return symtab;
}

ncompare(s1, s2)
	struct symbol *s1, *s2;
{
	return rflg*(s1->value - s2->value);
}

scompare(s1, s2)
	struct symbol *s1, *s2;
{
	return rflg*strcmp(s1->name, s2->name);
}

class(sp)
	struct syment *sp;
{
	if (sp->n_scnum == 0) {
		if (sp->n_value == 0)
			return 'U';
		else
			return 'C';
	}else if (sp->n_scnum == -1)
		return 'A';
	return section[sp->n_scnum];
}

doprint(file, sp, nel)
	char *file;
	struct symbol *sp;
	int nel;
{
	while (nel-- > 0) {
		if (oflg)
			printf("%s:%08x %c %s\n", file, sp->value, sp->sclass,
				sp->name);
		else
			printf("%08x %c %s\n", sp->value, sp->sclass, sp->name);
		sp++;
	}
}
