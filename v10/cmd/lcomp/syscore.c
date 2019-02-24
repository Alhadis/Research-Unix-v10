#include "stdio.h"
#include "sys/param.h"
#include <sys/dir.h>
#include <sys/psl.h>
#include <sys/pte.h>
#include <sys/user.h>
#include <ctype.h>
#include <a.out.h>
#include "sys/vm.h"

struct	pcb	pcb;
int	kernel;
int	kcore;
struct	pte *sbr;
int	slr;
int	masterpcbb;
int fcor, ffil;

struct nlist *cursym;
char *sysnam;

lookup(s)
char *s;
{	static struct nlist nl[2];
	nl[0].n_un.n_name = s;
	nlist(sysnam, nl);
	cursym = nl;
	if(nl[0].n_value == 0)
		fatal("lookup of %s failed\n", s);
	else
		printf("lookup of %s is 0x%x\n", s, cursym->n_value);
}

vtophys(addr)
off_t addr;
{
	int oldaddr = addr;
	int v;
	struct pte pte;

	addr &= ~0xc0000000;
	v = btop(addr);
	switch (oldaddr&0xc0000000) {

	case 0xc0000000:
	case 0x80000000:
		/*
		 * In system space get system pte.  If
		 * valid or reclaimable then physical address
		 * is combination of its page number and the page
		 * offset of the original address.
		 */
		if (v >= slr)
			goto oor;
		addr = ((long)(sbr+v)) &~ 0x80000000;
		goto simple;

	case 0x40000000:
		/*
		 * In p1 space must not be in shadow region.
		 */
		if (v < pcb.pcb_p1lr)
			goto oor;
		addr = (off_t) (pcb.pcb_p1br+v);
		break;

	case 0x00000000:
		/*
		 * In p0 space must not be off end of region.
		 */
		if (v >= pcb.pcb_p0lr)
			goto oor;
		addr = (off_t) (pcb.pcb_p0br+v);
		break;
	oor:
		return (-1);
	}
	/*
	 * For p0/p1 address, user-level page table should
	 * be in kernel vm.  Do second-level indirect by recursing.
	 */
	if ((addr & 0x80000000) == 0) {
		return (-1);
	}
	addr = vtophys(addr);
simple:
	/*
	 * Addr is now address of the pte of the page we
	 * are interested in; get the pte and paste up the
	 * physical address.
	 */
	if(lseek(fcor, addr, 0) < 0 || read(fcor, (char *)&pte, sizeof(char *)) <= 0) {
		return (-1);
	}
	/* SHOULD CHECK NOT I/O ADDRESS; NEED CPU TYPE! */
	if (pte.pg_v == 0 && (pte.pg_fod || pte.pg_pfnum == 0)) {
		return (-1);
	}
	return (ptob(pte.pg_pfnum) + (oldaddr & PGOFSET));
}

main(argc, argv)
char **argv;
{
	if(argc == 1) {
		sysnam = "/vmunix";
		fcor = open("core", 0);
	}
	else if(argc != 3) {
		fprintf(stderr, "usage: %s [core system]\n", argv[0]);
		exit(1);
	}
	else {
		sysnam = argv[2];
		fcor = open(argv[1], 0);
	}
	lookup("_Sysmap");
	sbr = cursym->n_value;
	lookup("_Syssize");
	slr = cursym->n_value;
	printf("sbr %X slr %X\n", sbr, slr);
	lookup("_masterpaddr");
	lseek(fcor, cursym->n_value & 0x7fffffff, 0);
	read(fcor, (char *)&masterpcbb, sizeof(char *));
	masterpcbb = (masterpcbb&PG_PFNUM)*512;
	getpcb();
	go();
	exit(0);
}

getpcb()
{

	lseek(fcor, masterpcbb&~0x80000000, 0);
	read(fcor, &pcb, sizeof (struct pcb));
	pcb.pcb_p0lr &= ~AST_CLR;
	printf("p0br %X p0lr %X p1br %X p1lr %X\n",
	    pcb.pcb_p0br, pcb.pcb_p0lr, pcb.pcb_p1br, pcb.pcb_p1lr);
}

struct rec {
	long len;
	struct rec *next;
	char *fname;
} proFptr;
FILE *outfd;
struct rec *x;
char buf[256];
long foo[1024];
int fd;

go()
{
	if(fcor < 0) {
		perror("/dev/kmem");
		exit(1);
	}
	outfd = fopen("prof.out", "w");
	if(outfd == 0) {
		perror("prof.out");
		exit(1);
	}
	lookup("_proFptr");
	if(vseek(fcor, cursym->n_value, 0) == -1) {
		fprintf(stderr, "vseek failed seeking to x%x\n", cursym->n_value);
		exit(1);
	}
	if(read(fcor, (char *)&x, sizeof(long)) != sizeof(long)) {
		fprintf(stderr, "read failed\n");
		exit(1);;
	}
	for(;;) {
		readit();
		if(proFptr.next == 0 || proFptr.next == x)
			break;
		x = proFptr.next;
	}
}

readit()
{	int i;
	i = vseek(fcor, (char *)x, 0);
	if(i == -1)
		fatal("first vseek in readit, seeking to x%x\n", x);
	i = read(fcor, (char *)&proFptr, sizeof(proFptr));
	if(i != sizeof(proFptr))
		fatal("reading proFptr in readit\n");
	i = read(fcor, (char *)foo, sizeof(long) * proFptr.len);
	if(i != sizeof(long) * proFptr.len)
		fatal("read %d instead of %d\n", i, sizeof(long) * proFptr.len);
	i = vseek(fcor, proFptr.fname, 0);
	if(i == -1)
		fatal("seeking to x%x\n", proFptr.fname);
	i = read(fcor, buf, sizeof(buf));
	fprintf(outfd, "%s\n", buf);
	fflush(outfd);
	for(i = 3; i < proFptr.len; i++)
		fprintf(outfd, "%d\n", foo[i-3]);
}

fatal(s, a, b, c, d, e, f, g)
char *s;
{
	fprintf(stderr, s, a, b, c, d, e, f, g);
	exit(1);
}

vseek(fd, loc, flag)
long loc;
{	long addr;
	addr = vtophys(loc);
	return(lseek(fd, addr, flag));
}
