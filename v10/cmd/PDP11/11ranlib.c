/*
 * Insert a directory into a character archive file
 */
#include	"ar.h"		/*god: char fmt archives */
#include	"a.out.h"	/*god: pdp11 fmt a.out.h */
#include	<stdio.h>
#ifndef AR		/*god: name of archiver */
#define AR "ar"
#endif
#define	MAGIC	exp.a_magic
#define	BADMAG	MAGIC!=A_MAGIC1 && MAGIC!=A_MAGIC2  \
		&& MAGIC!=A_MAGIC3 && MAGIC!=A_MAGIC4
struct	ar_hdr	arp;
long	arsize;
struct	exec	exp;
FILE	*fi, *fo;
long	off, oldoff;
long	atol();
long	ftell();
#define TABSZ	2000
struct tab
{	char cname[8];
	long cloc;
} tab[TABSZ];
int tnum;
int new;
char	tempnm[] = "__.SYMDEF";
char	firstname[17];
long	offdelta;

char	*progname;	/*god: holds argv[0] */

main(argc, argv)
char **argv;
{
	char buf[256];
	char magbuf[SARMAG+1];

	progname = argv[0];	/*god: for error messages */
	--argc;
	while(argc--) {
		fi = fopen(*++argv,"r");
		if (fi == NULL) {
			fprintf(stderr, "%s: cannot open %s\n",
					progname, *argv);
			continue;
		}
		off = SARMAG;
		fread(magbuf, 1, SARMAG, fi);	/* get magic no. */
		if (strncmp(magbuf, ARMAG, SARMAG)) {
			fprintf(stderr, "not archive: %s\n", *argv);
			continue;
		}
		fseek(fi, 0L, 0);
		new = tnum = 0;
		if(nextel(fi) == 0)
		{	fclose(fi);
			continue;
		}
		do {
			long o;
			register n;
			struct nlist sym;

			fread((char *)&exp, 1, sizeof(struct exec), fi);
			if (BADMAG)		/* archive element not in  */
				continue;	/* proper format - skip it */
			o = (long)exp.a_text + exp.a_data;
			if ((exp.a_flag & 01) == 0)
				o *= 2;
			fseek(fi, o, 1);
			n = exp.a_syms / sizeof(struct nlist);
			if (n == 0) {
				fprintf(stderr, "%s: %s-- no name list\n",
						progname, arp.ar_name);
				continue;
			}
			while (--n >= 0) {
				fread((char *)&sym, 1, sizeof(sym), fi);
				if ((sym.n_type&N_EXT)==0)
					continue;
				switch (sym.n_type&N_TYPE) {

				case N_UNDF:
					if (sym.n_value!=0)
						stash(&sym);
					continue;

				default:
					stash(&sym);
					continue;
				}
			}
		} while(nextel(fi));
		new = fixsize();
		fclose(fi);
		fo = fopen(tempnm, "w");
		if(fo == NULL)
		{	fprintf(stderr, "can't create temporary\n");
			exit(1);
		}
		fwrite((char *)tab, tnum, sizeof(struct tab), fo);
		fclose(fo);
		if(new)
			sprintf(buf, "%s rlb %s %s %s\n",
				     AR, firstname, *argv, tempnm);/*god*/
		else	sprintf(buf, "%s rl %s %s\n",
				     AR, *argv, tempnm);/*god*/
		if(system(buf))
			fprintf(stderr, "can't execute %s\n", buf);
		else fixdate(*argv);
		unlink(tempnm);
	}
	exit(0);
}

nextel(af)
FILE *af;
{
	register r;
	register char *cp;

	oldoff = off;
	fseek(af, off, 0);
	r = fread((char *)&arp, 1, sizeof(struct ar_hdr), af);  /* read archive header */
	if (r != sizeof(struct ar_hdr))
		return(0);
	for (cp=arp.ar_name; cp < & arp.ar_name[sizeof(arp.ar_name)]; cp++)
		if (*cp == ' ')
			*cp = '\0';
	arsize = atol(arp.ar_size);
	if (arsize & 1)
		arsize++;
	off = ftell(af) + arsize;	/* offset to next element */
	return(1);
}

stash(s) struct nlist *s;
{	int i;
	if(tnum >= TABSZ)
	{	fprintf(stderr, "symbol table overflow\n");
		exit(1);
	}
	for(i=0; i<8; i++)
		tab[tnum].cname[i] = s->n_name[i];
	tab[tnum].cloc = oldoff;
	tnum++;
}

fixsize()
{	int i;
	offdelta = tnum * sizeof(struct tab) + sizeof(arp);
	off = SARMAG;
	nextel(fi);
	if(strncmp(arp.ar_name, tempnm, 14) == 0)
	{	new = 0;
		offdelta -= sizeof(arp) + arsize;
	}
	else
	{	new = 1;
		strncpy(firstname, arp.ar_name, 14);
	}
	for(i=0; i<tnum; i++)
		tab[i].cloc += offdelta;
	return(new);
}

/* patch time */
fixdate(s) char *s;
{	long time();
	char buf[24];
	int fd;
	fd = open(s, 1);
	if(fd < 0)
	{	fprintf(stderr, "can't reopen %s\n", s);
		return;
	}
	sprintf(buf, "%-*ld", sizeof(arp.ar_date), time((long *)NULL)+5);
	lseek(fd, (long)SARMAG + ((char *)arp.ar_date-(char *)&arp), 0);
	write(fd, buf, sizeof(arp.ar_date));
	close(fd);
}
