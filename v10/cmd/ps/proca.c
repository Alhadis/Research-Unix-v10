#include <stdio.h>
#include "ps.h"

inita()
{
	if (drumf == NULL)
		drumf = "/dev/drum";
	if (memf == NULL)
		memf = "/dev/kmem";
	if ((drum = open(drumf, 0)) < 0) {
		perror(drumf);
		exit(1);
	}
	if ((memory = open(memf, 0)) < 0) {
		perror(memf);
		exit(1);
	}
}

getproca(s, pp)
char *s; register struct proc *pp;
{
	register int fd;
	char sstr[32];
	extern char stack[];

	if((fd = open(strcat(strcpy(sstr, "/proc/"), s), 0)) == -1)
		return -1;
	if(ioctl(fd, PIOCGETPR, pp) < 0) {
		close(fd);
		fprintf(stderr, "%s %s: /proc ioctl error\n", progname, s);
		return -1;
	}
	return fd;
}

char *
getuareaa(fd, pp, up)
register struct proc *pp; register struct user *up;
{
	if (up->u_procp)
		return 0;
	if (pp->p_stat == SZOMB) {
		memset((char *)up, 0, sizeof(*up));
		return (0);
	}
	if (pp->p_flag & SLOAD) {
		register ntry = 4;
		do
			if (memrd(fd, (char *)up, sizeof(*up), UBASE) == sizeof(*up))
				return 0;
		while (errno == EBUSY && --ntry > 0);
		close(fd);
		return "/proc read error";
	}
	if (memrd(drum, (char *)up, sizeof(*up), ctob(pp->p_swaddr)) == sizeof(*up))
		return 0;
	close(fd);
	return "drum read error";
}

getstacka(f, pp, up, stack)
struct proc *pp;
struct user *up;
char *stack;
{
	int nstack;
	int nspte;
	int szpt;
	register int i;
	struct pte pte[NPTEPG];

	nstack = ctob(up->u_ssize);
	if (nstack <= 0)
		return (0);
	if (nstack > NSTACK)
		nstack = NSTACK;
	if (pp->p_flag & SLOAD) {
		lseek(f, UBASE-nstack, 0);
		return (read(f, stack, nstack));
	} else {
		nspte = btoc(nstack);
		szpt = clrnd(ctopt(UPAGES+pp->p_tsize+pp->p_dsize+pp->p_ssize));
		lseek(drum, ctob(pp->p_swaddr+UPAGES+szpt-1), 0);
		if (read(drum, (char *)pte, sizeof(pte)) != sizeof(pte))
			return (0);
		for (i = nspte-1; i >= 0; stack += NBPG, --i)
			rdpage(up, i, &pte[NPTEPG-UPAGES-1-i], stack);
		return (nstack);
	}
}

List *
getprlista()
{
	register List *prlist;

	if ((prlist = getdir("/proc", (List *)0)) == 0)
		error("can't getdir /proc");
	return prlist;
}

kreada(addr, buf, size)
long addr;
char *buf;
int size;
{

	return (memrd(memory, buf, size, addr) == size);
}
