#include <stdio.h>
#include "ps.h"

struct proc proc;

union { struct user user; char userchars[UPAGES*NBPG]; } usp;
#define u usp.user
#define upages usp.userchars

static char tmpstr[4096]; static Xflag;

char *memf;
char *drumf;
char *nlistf;

main(argc, argv)
int argc; char **argv;
{
	register Entry *ep;
	register Psline *ps; Psline *psbegin, *psend;
	register char *cp; register ncp, fd;

	progname=argv[0];
	initttys();
	myuid = getuid();
	mypid = getpid();
	if (argc > 1)
		selbeg = selend = Malloc(Select, argc-1);
	doarg(*++argv);	/* eh? */
	Xflag = xflag && !tflag;
	while (--argc > 1)
		doarg(*++argv);
	openfiles();
	if (hflag)
		hprint();
	if (Nflag && !tflag && !Fflag) {
		register Select *sp;
		Nflag = tflag = 0;
		++aflag; ++xflag;
		for (sp=selbeg; sp<selend; sp++) {
			if ((fd = doselect(sp->id, &proc, &u)) < 0)
				continue;
			if (printp(tmpstr, fd, &proc, &u) > tmpstr)
				printf("%s", tmpstr);
		}
		exit(0);
	}

	if ((prlist = getprlist()) == 0)
		error("can't getdir /proc");
	if ((psbegin = Malloc(Psline, prlist->nentry)) == 0)
		error("can't malloc ps line table");

	for (ps = psbegin, ep = prlist->begin; ep; ep = ep->next) {
		if ((fd = doselect(ep->e_name, &proc, &u)) < 0)
			continue;
		if ((cp = printp(tmpstr, fd, &proc, &u)) <= tmpstr)
			continue;
		if (nflag) {
			printf("%s", tmpstr);
			continue;
		}
		ncp = cp - tmpstr + 1;
		ps->string = memcpy(malloc(ncp), tmpstr, ncp);
		ps->weight = proc.p_pid;
		if (uflag)
			ps->weight += (1001 - (int)(1000.0*proc.p_pctcpu+0.5))<<16;
		ps++;
	}
	if (!nflag) {
		if((psend = ps) > psbegin)
			qsort((char *)psbegin, psend - psbegin, sizeof(Psline), Pslcomp);
		for (ps = psbegin; ps < psend; ps++)
			printf("%s", ps->string);
	}
	exit(0);
}

doarg(cp)
register char *cp;
{
	struct stat fds;
	register Select *sp;
	char *strdup();
	xflag = 0;
	if (cp) do switch (*cp) {
		case 0:
			cp = 0; break;
		case '-':
			break;
		case 't':
			if (*++cp == '.') {
				if (fstat(0, &fds) == 0) {
					cp = gettty(fds.st_dev, fds.st_ino);
					if (*cp == '?')
						cp = 0;
					else
						cp = strdup(cp);
				} else {
					fprintf(stderr, "0: cannot fstat\n");
					cp = 0;
				}
			}
			if (cp) {
				++tflag;
				sp = selend++;
				sp->flag = SELTTY;
				if (Xflag || xflag)
					sp->flag |= SELXFL;
				sp->id = cp;
				cp = 0;
			}
			break;
		case 'a':
			aflag++; break;
		case 'F':
			if (stat(++cp, &fds) == 0) {
				Fflag++;
				sp = selend++;
				sp->flag = SELFIL;
				sp->id = cp;
				sp->dev = fds.st_dev;
				sp->ino = fds.st_ino;
			} else
				fprintf(stderr, "%s: cannot stat\n", cp);
			cp = 0;
			break;
		case 'f':
			fflag++, getfstab();
			break;
		case 'h':
			hflag++; break;
		case 'l':
			lflag++; break;
		case 'n':
			nflag++; break;
		case 'r':
			rflag++; break;
		case 'u':
			uflag++; getlogins(); break;
		case 'x':
			xflag++; break;
		case 'T':
			Tflag++; break;

		case 'o':
			mode = 1;
			break;

		case 'M':
			memf = cp+1;
			cp = 0;
			break;

		case 'D':
			drumf = cp+1;
			cp = 0;
			break;

		case 'N':
			nlistf = cp+1;
			cp = 0;
			break;
		
		default:
			if (*cp < '0' || *cp > '9')
				usage();
			++Nflag;
			sp = selend++;
			sp->flag = SELXFL;
			sp->id = cp;
			cp = 0;
	} while (cp++);
	return 0;
}

Pslcomp(a, b)
Psline *a, *b;
{
	return a->weight - b->weight;
}

error(s)
char *s;
{
	fprintf(stderr, "%s: %s\n", progname, s);
	exit(1);
}

usage()
{
	fprintf(stderr, "Usage: %s [afhlnruxFt#] ...\n", progname);
	exit(1);
}
