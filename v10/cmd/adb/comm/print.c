/*
 *
 *	UNIX debugger
 *
 */
#include "defs.h"
#include "regs.h"
#include "sym.h"
#include "bkpt.h"
#include "map.h"
#include "space.h"

extern	int	infile;
extern	int	outfile;
extern	int	maxpos;
extern	int	radix;
extern	char	lastc;
extern	int	maxargs;

static int redirin(), redirout();
static char *getfname();

/* general printing routines ($) */

char	*Ipath = INCDIR;

printtrace(modif)
{
	int	i;
	register BKPT *bk;
	register struct sym *sp;
	int	stack;
	char	*fname;

	if (cntflg==0)
		cntval = -1;
	switch (modif) {

	case '<':
		if (cntval == 0) {
			while (readchar() != EOR)
				;
			reread();
			break;
		}
		if (rdc() == '<')
			stack = 1;
		else {
			stack = 0;
			reread();
		}
		fname = getfname();
		redirin(stack, fname);
		break;

	case '>':
		fname = getfname();
		redirout(fname);
		break;

	case 'p':
		kmproc();
		break;

	case 'k':
		kmsys();
		break;

	case 'd':
		if (adrflg) {
			if (adrval != 0
			&&  (adrval<2 || adrval>16))
				error("radix should be between 2 and 16, or 0");
			radix = adrval;
			if (radix)
				printf("radix=%d base ten",radix);
			else
				printf("radix=magic");
		}
		break;

	case 'q':
	case 'Q':
		done();

	case 'w':
		maxpos=(adrflg?adrval:MAXPOS);
		break;

	case 'S':
		printsym();
		break;

	case 's':
		maxoff=(adrflg?adrval:MAXOFF);
		break;

	case 'v':
		for (i=0;i<NVARS;i++) {
			if (var[i])
				printf("%-8R >%c\n", var[i],
					(i<=9 ? '0' : 'a'-10) + i);
		}
		break;

	case 'm':
		printmap("? map", symmap);
		printmap("/ map", cormap);
		break;

	case 0:
	case '?':
		if (pid)
			printf("pcs id = %d\n",pid);
		else
			prints("no process\n");
		sigprint();
		flushbuf();

	case 'r':
	case 'R':
		printregs(modif);
		return;

	case 'c':
	case 'C':
		ctrace(modif);
		break;

	case 'a':
		if (adrflg)
			maxargs = adrval;
		else
			printf("%R$a\n", maxargs);
		return;

		/*print externals*/
	case 'e':
		for (sp = symtab; sp; sp = sp->y_next) {
			if (sp->y_type==S_DATA)
				printf("%s/%12t%R\n", sp->y_name,
					ltow(lget(sp->y_value,CORF|DATASP)));
		}
		break;

		/*print breakpoints*/
	case 'b':
	case 'B':
		for (bk=bkpthead; bk; bk=bk->nxtbkpt)
			if (bk->flag) {
				psymoff((WORD)bk->loc,INSTSP,"");
				if (bk->count != 1)
					printf(",%d", bk->count);
				printf(":%c %s", bk->flag == BKPTTMP ? 'B' : 'b', bk->comm);
			}
		break;

	default:
		error("bad `$' command");
	}

}

static
char *
getfname()
{
	static char fname[ARB];
	register char *p;

	if (rdc() == EOR) {
		reread();
		return (NULL);
	}
	p = fname;
	do {
		*p++ = lastc;
		if (p >= &fname[ARB-1])
			error("filename too long");
	} while (rdc() != EOR);
	*p = 0;
	reread();
	return (fname);
}

static
redirin(stack, file)
int stack;
char *file;
{
	char pfile[ARB];

	if (file == NULL) {
		iclose(-1, 0);
		return;
	}
	iclose(stack, 0);
	if ((infile = open(file, 0)) < 0) {
		strcpy(pfile, Ipath);
		strcat(pfile, "/");
		strcat(pfile, file);
		if ((infile = open(pfile, 0)) < 0) {
			infile = STDIN;
			error("cannot open");
		}
	}
	if (cntflg)
		var[9] = cntval;
	else
		var[9] = 1;
}

static
redirout(file)
char *file;
{

	oclose();
	if (file == NULL)
		return;
	if ((outfile = open(file, 1)) >= 0)
		lseek(outfile, 0L, 2);
	else if ((outfile = creat(file, 0666)) < 0) {
		outfile = STDOUT;
		error("cannot create");
	}
}

printmap(s,mp)
char *s;
register MAP *mp;
{
	char *maptype();

	if (mp == symmap)
		printf("%s%12t`%s'\n", s, fsym < 0 ? "-" : symfil);
	else if (mp == cormap)
		printf("%s%12t`%s'\n", s, fcor < 0 ? "-" : corfil);
	else
		printf("%s\n", s);
	for (; mp->flag & MPINUSE; mp++)
		printf("%-8s %-16R %-16R %-16R\n",
		    maptype(mp->sp), (WORD)mp->b, (WORD)mp->e, (WORD)mp->f);
}

char *
maptype(sp)
int sp;
{

	switch (sp & SPTYPE) {
	case INSTSP:
		return ("text");
	case DATASP:
		return ("data");
	case UBLKSP:
		return ("user");
	default:
		return ("nonsense");
	}
}
