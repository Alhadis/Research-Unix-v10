#include "sys/types.h"
#include "sys/stat.h"
#include "a.out.h"
/* it wouldn't be better to get this out of sys/param.h */
#define HZ 60

typedef unsigned short Prof;	/* what the counts are stored in! */
/* a mon.out consists of a phdr, cnt structs pcnt, and the rest are
	Profs of pc clicks */
struct phdr {
	char	*lopc, *hipc;	/* void *? */
	int	cnt;
};
struct phdr hsv;
struct pcnt {
	char *	value;
	long	calls;
};
char flags[128];
char *xfile, *file;
struct nlist *names;
char *strings, *pbuf;
Prof *accum;
int accumsz;
typedef struct {
	char *loc;
	char *name;
	long calls;
	long clicks;
	unsigned char type;
} sym;
int nsym;
sym *syms, nilsym;
long lopct, hipct;	/* plot stuff */
double rscale, roffset;

main(argc, argv)
char **argv;
{	int c;
	extern int optind;
	extern char **optarg;
	char *p;
	
	while((c = getopt(argc, argv, "alnsvz")) != -1)
	switch(c) {
	case 'l': case 'n': case 's': case 'z':
		flags[c] = 1;
		continue;
	case 'v':
		flags['v'] = 1;
		p = argv[optind];
		if(*p != '-' || p[1] > '9' || p[1] < '0')
			continue;
		lopct = atoi(p+1);
		p = argv[++optind];
		if(*p != '-' || p[1] > '9' || p[1] < '0')
			continue;
		hipct = atoi(p+1);
		optind++;
		continue;
	case '?':
		fatal("usage: %s -[alnsz] files\n", argv[0]);
	}
	if(optind < argc)
		xfile = argv[optind++];
	else
		xfile = "a.out";
	readsyms();
	if(optind >= argc)
		doit("mon.out");
	else
		for(; optind < argc; optind++)
			doit(argv[optind]);
	output();
	exit(0);
}

doit(s)
char *s;
{	struct stat stb;
	int fd;

	file = s;
	fd = open(s, 0);
	if(fd < 0)
		fatal("couldn't open %s\n", s);
	fstat(fd, &stb);
	pbuf = (char *) malloc(stb.st_size);
	if(pbuf == 0)
		fatal("couldn't alloc %d bytes for %s\n", stb.st_size, s);
	if(read(fd, pbuf, stb.st_size) != stb.st_size)
		fatal("read of %s failed\n", s);
	if(stb.st_size <= sizeof(struct phdr))
		fatal("%s not a mon.out\n");
	close(fd);
	addcalls();
	addclicks(stb.st_size);
	free(pbuf);
}

addclicks(top)
{	int i, n, cnt, j;
	char *x;
	Prof *p;
	struct phdr *h;
	double scale;	/* the kernel uses 16 bit fractions */

	h = (struct phdr *)pbuf;
	n = ((struct phdr *)pbuf)->cnt;
	p = (Prof *) (pbuf + sizeof(struct phdr) + n * sizeof(struct pcnt));
	top -= sizeof(struct phdr) + n * sizeof(struct pcnt);
	cnt = top/sizeof(Prof);	/* number of counters */
	if(flags['s'] && !accumsz) {
		accum = (Prof *) malloc(cnt*sizeof(Prof));
		(void) memset(accum, 0, cnt*sizeof(Prof));
		accumsz = cnt;
		hsv = *h;
	}	/* we assume h is fixed across the files */
	scale = (h->hipc-h->lopc)/(double)cnt;
	/* in mcrt0.s it's 8 */
	for(j = i = 0; i < cnt; i++) {
		x = (char *)(h->lopc + (int)(scale*i));
		while(j < nsym && syms[j+1].loc < x)
			j++;
		if(j > nsym)
			fatal("cnts out of range %s\n", file);
		syms[j].clicks += p[i];
	}
	if(flags['s'])
	for(i = 0; i < accumsz; i++) {
		n = accum[i] + p[i];
		accum[i] = n;
		if(n != accum[i]) {
			mesg("overflow in accumulating counts, -s ignored\n");
			flags['s'] = 0;
			return;
		}
	}
}

addcalls()
{	int n, i;
	struct pcnt *p;

	p = (struct pcnt *)(pbuf + sizeof(struct phdr));
	n = ((struct phdr *)pbuf)->cnt;
	for(i = 0; i < n; i++, p++)
		addcall(p->value, p->calls);
}

addcall(x, n)
char *x;
{	int i;
	for(i = 0; i < nsym && syms[i+1].loc < x; i++)
		;
	if(i >= nsym) {
		mesg("cntr out of range (%s), ignored\n", file);
		return;
	}
	if(syms[i+1].loc == x)
		syms[i+1].calls += n;
	else
		syms[i].calls += n;
}

symnum(a, b)
register sym *a, *b;
{
	return(a->loc - b->loc);
}

readsyms()
{	int fd, n, i, j;
	struct exec x;
	struct stat stb;
	struct nlist *np;

	fd = open(xfile, 0);
	if(fd < 0)
		fatal("couldn't read %s\n", xfile);
	if(read(fd, (char *)&x, sizeof(x)) != sizeof(x))
		fatal("can't read header of %s\n", xfile);
	if(!x.a_syms)
		fatal("%s has no symbol table\n", xfile);

	/* read the symbol table in */
	lseek(fd, N_SYMOFF(x), 0);
	names = (struct nlist *) malloc(x.a_syms);
	read(fd, (char *)names, x.a_syms);
	nsym = N_STROFF(x) - N_SYMOFF(x);
	nsym /= sizeof(struct nlist);	/* over estimated */

	/* read the wretched strings in */
	lseek(fd, N_STROFF(x), 0);
	fstat(fd, &stb);
	n = stb.st_size - N_STROFF(x);
	strings = (char *) malloc(n);
	if(read(fd, strings, n) != n)
		fatal("couldn't read string table from %s (%d)\n", xfile, n);

	/* copy the text symbols into the symbol table */
	syms = (sym *) malloc(nsym * sizeof(sym));
	for(i = 0, np = names; np < names + nsym; np++) {
		if((np->n_type & N_TYPE) != N_TEXT)
			continue;
		if(np->n_type & N_STAB)
			continue;
		syms[i] = nilsym;
		syms[i].type = np->n_type;
		syms[i].loc = (char *)np->n_value;
		syms[i].name = strings + np->n_un.n_strx;
		i++;
	}
	nsym = i;
	qsort((char *)syms, nsym, sizeof(sym), symnum);

	/* now decide which of the ones with the same loc we like */
	for(i = 0; i < nsym; i = j) {
		for(j = i+1; j < nsym; j++)
			if(syms[i].loc != syms[j].loc)
				break;
		choose(i, j);
	}
	/* and squash it down */
	for(i = j = 0; i < nsym; i++)
		if(syms[i].type)
			syms[j++] = syms[i];
	nsym = j;
	syms[nsym].loc = (char *)0x8fffffff;	/* a very big end */
	close(fd);
}

/* the expert system */
choose(i, j)
{	int n;
	if(j-i <= 1)
		return;
	/* if there is an external text one, take it */
	for(n = i; n < j; n++)
		if(syms[n].type & N_EXT)
			goto found;
	/* if there is one that starts with _, take it */
	for(n = i; n < j; n++)
		if(syms[n].name[0] == '_')
			goto found;
	/* if there is one without a . in it, take it */
	for(n = i; n < j; n++)
		if(!strchr(syms[n].name, '.'))
			goto found;
	/* first must be right, or at least ok */
	n = i;
found:
	for(; i < j; i++)
		if(i != n)
			syms[i].type = 0;
}

char ebuf[256];
mesg(s, a, b, c, d, e, f)
char *s;
{
	sprintf(ebuf, s, a, b, c, d, e, f);
	write(2, ebuf, strlen(ebuf));
}

fatal(s, a, b, c, d, e, f)
char *s;
{
	mesg(s, a, b, c, d, e, f);
	exit(1);
}

symcalls(a, b)
register sym *a, *b;
{
	return(b->calls - a->calls);
}

symcnt(a, b)
register sym *a, *b;
{
	return(b->clicks - a->clicks);	/* largest first */
}

output()
{	int i, tot, fd;
	double x, y;

	if(flags['v']) {
		plotout();
		return;
	}
	for(tot = i = 0; i < nsym; i++)
		tot += syms[i].clicks;
	if(tot == 0)
		tot = 1;
	if(flags['n'])
		qsort((char *)syms, nsym, sizeof(sym), symcalls);
	else if(!flags['l'])
		qsort((char *)syms, nsym, sizeof(sym), symcnt);
	printf("  %%   Time\tCalls\tName\n");
	for(i = 0; i < nsym; i++) {
		if(!syms[i].calls && !syms[i].clicks && !flags['z'])
			continue;
		x = syms[i].clicks;
		x /= HZ;
		y = syms[i].clicks/(double)tot;
		printf("%4.1f%8.3f %8d\t%s\n", (y*100), x, syms[i].calls,
			syms[i].name);
	}
	if(!flags['s'])
		return;
	fd = creat("mon.sum", 0666);
	if(fd < 0)
		fatal("can't create mon.sum\n");
	hsv.cnt = nsym;
	if(write(fd, (char *)&hsv, sizeof(hsv)) != sizeof(hsv))
		fatal("write failed\n");
	for(i = 0; i < nsym; i++) {	/* this is very inefficient */
		write(fd, (char *)&syms[i].loc, 4);	/* 4! */
		write(fd, (char *)&syms[i].calls, 4);
	}
	if(write(fd, (char *)accum, accumsz*sizeof(Prof)) != accumsz*sizeof(Prof))
		fatal("write failed\n");
	close(fd);
}

/* most of this is witchcraft */
plotout()
{	int i;
/*	double lastx, lasty;
	if(lopct >= 100 || lopct < 0)
		lopct = 0;
	if(hipct > 100 || hipct <= 0)
		hipct = 100;
	rscale = 100./(hipct-lopct);
	roffset = 2040.+40.8*lopct*rscale;
	openpl();
	erase();
	range(-2048, -2048, 2048, 2048);
	line(-2040, -2040, -2040, 2040);
	for(i = 0; i < 11; i++)
		line(-2040, 2040-i*408, 0, 2040-i*408);
	*/
}
