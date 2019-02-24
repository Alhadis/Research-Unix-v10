#include <stdio.h>
#include "ps.h"
#include <nlist.h>

struct nlist nl[] = {
	{ "_proc" },
#define	X_PROC		0
	{ "_proccnt" },
#define	X_NPROC		1
	{ "_physspt" },
#define	X_PHYSSPT	2
	{ 0 },
};

struct proc *kproc;
static int nproc, lastproc;
long physspt;

long vtophys();

initk()
{

	if (memf == NULL)
		memf = "/dev/mem";
	else {
		if (drumf == NULL)	/* and memf was specified */
			drumf = "/dev/null";
	}
	if (drumf == NULL)
		drumf = "/dev/drum";
	if (nlistf == NULL)
		nlistf = "/unix";
	if ((drum = open(drumf, 0)) < 0) {
		perror(drumf);
		exit(1);
	}
	if ((memory = open(memf, 0)) < 0) {
		perror(memf);
		exit(1);
	}
	nlist(nlistf, nl);
	if(nl[0].n_type == 0) {
		fprintf(stderr, "%s: No namelist\n", nlistf);
		exit(1);
	}
	if (memrd(memory, (char *)&physspt, sizeof(physspt),
	    (long)nl[X_PHYSSPT].n_value-KSTART) != sizeof(physspt)) {
		fprintf(stderr, "can't read physspt\n");
		exit(1);
	}
}

setkproc()
{
	if (memrd(memory, (char *)&nproc, sizeof(nproc),
	    vtophys(nl[X_NPROC].n_value)) != sizeof(nproc)) {
		fprintf(stderr, "can't read nproc\n");
		exit(1);
	}
	if((kproc = (struct proc *)malloc(nproc*sizeof(struct proc))) == 0) {
		fprintf(stderr, "no memory for proc table\n");
		exit(1);
	}
	if (memrd(memory, (char *)kproc, nproc*sizeof(struct proc),
	    vtophys(nl[X_PROC].n_value)) != nproc*sizeof(struct proc)) {
		fprintf(stderr, "can't read proc table\n");
		exit(1);
	}
	lastproc = nproc-1;
}

getprock(s, pp)
char *s;
struct proc *pp;
{
	register int pid, x;

	if (nproc == 0)
		setkproc();
	pid = atoi(s);
	for(x = lastproc+1; x != lastproc; x++) {
		if(x >= nproc)
			x = 0;
		if (kproc[x].p_stat == 0)
			continue;
		if(kproc[x].p_pid == pid) {
			*pp = kproc[x];
			lastproc = x;
			return 0;
		}
	}
	return -1;
}

char *
getuareak(fd, pp, up)
register struct proc *pp;
register struct user *up;
{
	struct pte upte[UPAGES];
	register int i;

	if (pp->p_stat == SZOMB) {
		memset((char *)up, 0, sizeof(*up));
		return (0);
	}
	if (pp->p_flag & SLOAD) {
		if (memrd(memory, (char *)upte, sizeof(upte), vtophys((long)pp->p_addr)) != sizeof(upte))
			return ("can't read user ptes");
		for (i = 0; i < UPAGES; i++)
			if (memrd(memory, ((char *)up)+i*NBPG, NBPG,
			    ctob(upte[i].pg_pfnum)) != NBPG)
				return ("can't read user");
		return (0);
	} else {
		if (memrd(drum, (char *)up, sizeof(*up), ctob(pp->p_swaddr)) != sizeof(*up))
			return ("can't read user block");
		return (0);
	}
}

getstackk(f, pp, up, stack)
register struct proc *pp;
register struct user *up;
char *stack;
{
	register int i;
	int nstack, nspte;
	int szpt;	/* swapped pt size */
	struct pte pte[NPTEPG];

	nstack = ctob(up->u_ssize);
	if (nstack <= 0)
		return (0);
	if (nstack > NSTACK)
		nstack = NSTACK;
	nspte = btoc(nstack);
	szpt = clrnd(ctopt(UPAGES+pp->p_tsize+pp->p_dsize+pp->p_ssize));
	if (pp->p_flag & SLOAD) {
		if (memrd(memory, (char *)pte, sizeof(pte),
		    vtophys((long)pp->p_addr+UPAGES*sizeof(struct pte)-NBPG)) != sizeof(pte))
			return (0);
	} else {
		if (memrd(drum, (char *)pte, sizeof(pte),
		    ctob(pp->p_swaddr+UPAGES+szpt-1)) != sizeof(pte))
			return (0);
	}
	for (i = nspte-1; i >= 0; stack += NBPG, --i)
		rdpage(up, i, &pte[NPTEPG-UPAGES-1-i], stack);
	return (nstack);
}

List *
getprlistk()
{
	register List *lp;
	register Entry *ep;
	register int x;

	if (nproc == 0)
		setkproc();
	if((lp = Malloc(List, 1)) == 0)
		return 0;
	lp->next = 0;
	lp->begin = 0;
	lp->nentry = 0;
	for(x = 0; x < nproc; x++) {
		if(kproc[x].p_stat == 0)
			continue;
		if((ep = Malloc(Entry, 1)) == 0)
			break;
		ep->next = lp->begin;
		lp->begin = ep;
		lp->nentry++;
		ep->e_val = kproc[x].p_pid;
		sprintf(ep->e_name, "%d", kproc[x].p_pid);
	}
	return lp;
}

long
vtophys(loc)
long loc;
{
	struct pte pte;
	register long nloc;

	nloc = loc - KSTART;
	if (nloc < 0) {
		fprintf(stderr, "bad kernel address %x\n", loc);
		return (0);
	}
	if (memrd(memory, (char *)&pte, sizeof(pte),
	    physspt + (nloc/NBPG)*sizeof(struct pte)) != sizeof(pte)) {
		fprintf(stderr, "can't get spt for %x\n", loc);
		return (0);
	}
	if (pte.pg_v == 0) {
		fprintf(stderr, "invalid spt for %x\n", loc);
		return (0);
	}
	return (pte.pg_pfnum * NBPG + (nloc & PGOFSET));
}

kreadk(addr, buf, size)
long addr;
char *buf;
int size;
{

	return (memrd(memory, buf, size, vtophys(addr)) == size);
}
