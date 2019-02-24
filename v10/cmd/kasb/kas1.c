/* @(#)kas1.c	1.2 */
#undef	vax
#define	pdp11	1
#include <stdio.h>
#include "a.out.h"
#include <sys/kmc.h>
#include "kas.h"

FILE	*txtfil;
int	hshused;
int	lineno	= 1;
struct	symtab *nextsym = {symtab};
#ifdef vax
struct filehdr hdr;
struct aouthdr aout;
struct scnhdr scn;
struct syment sym;
#endif
#ifdef pdp11
struct exec hdr;
#endif
extern char yytext[];

main(argc, argv)
char **argv;
{
	register struct symtab *sp, *ip;
	int c;
	register struct symtab **hp;
	int infound = 0;

	outfile = "a.out";
	while (argc > 1) {
		if (argv[1][0]=='-' && argv[1][1]=='o') {
			if (argc <3) {
				fprintf(stderr, "missing -o file\n");
				exit(1);
			}
			outfile = argv[2];
			argc -= 2;
			argv += 2;
			continue;
		}
		if (argv[1][0]=='-' && argv[1][1]=='d') {
			if (argc <3) {
				fprintf(stderr, "missing -d file");
				exit(1);
			}
			dbfile = argv[2];
			dbmode++;
			if ((dbfd = open(dbfile, 2))<0) {
				fprintf(stderr, "can not open %s\n", dbfile);
				exit(2);
			}
			signal(2,1);
			argc -= 2;
			argv += 2;
			continue;
		}
		if (infound) {
			fprintf(stderr, "too many arguments\n");
			exit(1);
		}
		infound++;
		infile = argv[1];
		if (freopen(infile, "r", stdin) == NULL) {
			fprintf(stderr, "can not open %s\n", infile);
			exit(2);
		}
		argc--;
		argv++;
	}
	for (ip=instab; ip->name[0]!=0; ip++) {
		register char *p1, *p2;
		for (p1=ip->name,p2=yytext; p2<yytext+NCPS;)
			*p2++ = *p1++;
		*p2++ = 0;
		hp = lookup(0);
		if (*hp==NULL) {
			*hp = ip;
		}
	}
	yytext[0] = '.'; yytext[1] = '\0';
	dot = *lookup(1);
	yyparse();
	if (dbmode)
		exit(0);
	txtfil = fopen(outfile, "w");
	if (txtfil==NULL) {
		fprintf(stderr, "can not create %s\n", outfile);
		exit(1);
	}
#ifdef vax
	hdr.f_magic = VAXROMAGIC;
	hdr.f_nscns = 2;
	aout.magic = 0410;
	aout.tsize = tsize*2;
	aout.dsize = (dsize+1)&~1;
	aout.bsize = 0;
	hdr.f_timdat = time(NULL);
	hdr.f_symptr = sizeof(struct filehdr)
		 + (2 * sizeof(struct scnhdr))
		 + sizeof(struct aouthdr)
		 + aout.tsize
		 + aout.dsize ;
	hdr.f_nsyms = nextsym-symtab;
	hdr.f_opthdr = sizeof(struct aouthdr);
	hdr.f_flags = F_AR32WR|F_LNNO|F_RELFLG;
	fwrite(&hdr, sizeof hdr, 1, txtfil);
	fwrite(&aout, sizeof aout, 1, txtfil);
	strncpy(scn.s_name,_TEXT,sizeof(scn.s_name));
	scn.s_size = aout.tsize;
	scn.s_scnptr = sizeof(struct filehdr) + sizeof(struct aouthdr) +
		(2 * sizeof(struct scnhdr));
	fwrite(&scn, sizeof scn, 1, txtfil);
	strncpy(scn.s_name,_DATA,sizeof(scn.s_name));
	scn.s_size = aout.dsize;
	scn.s_scnptr += aout.tsize;
	fwrite(&scn, sizeof scn, 1, txtfil);
	fwrite(ispace, aout.tsize, 1, txtfil);
	fwrite(dspace, aout.dsize, 1, txtfil);
#endif
#ifdef pdp11
	hdr.a_magic = 0410;
	hdr.a_text = tsize*2;
	hdr.a_data = (dsize+1)&~1;
	hdr.a_syms = (int)nextsym-(int)symtab;
	hdr.a_flag = 1;
	fwrite(&hdr, sizeof hdr, 1, txtfil);
	fwrite(ispace, hdr.a_text, 1, txtfil);
	fwrite(dspace, hdr.a_data, 1, txtfil);
#endif
	for (sp=symtab; sp<nextsym; sp++) {
#ifdef pdp11
		sp->type >>= 12;
#endif
#ifdef vax
		strncpy(sym.n_name, sp->name, SYMNMLEN);
		sym.n_value = sp->value;
		if (sp->type == XTEXT)
			sym.n_scnum = 1;
		else
			sym.n_scnum = 2;
		sym.n_sclass = C_EXT;
		fwrite(&sym, 1, SYMESZ, txtfil);
#endif
		if (sp->type == XUNDEF)
			fprintf(stderr, "%.8s undefined\n", sp->name);
	}
#ifdef pdp11
	fwrite(symtab, sizeof(symtab[0]), nextsym-symtab, txtfil);
#endif
	exit(anyerrs!=0);
}

struct symtab **
lookup(instflg)
{
	int ihash;
	register struct symtab **hp;
	register char *p1, *p2;

	ihash = 0;
	p1 = yytext;
	while (*p1) {
		ihash <<= 1;
		ihash += *p1++;
	}
	ihash &= 077777;
	while (p1<yytext+NCPS)
		*p1++ = 0;
	hp = &hshtab[ihash%NHASH];
	while (*hp) {
		p2 = (*hp)->name;
		for (p1=yytext; p1<yytext+NCPS;)
			if (*p1++ != *p2++)
				goto no;
		return(hp);
	no:
		if (++hp >= &hshtab[NHASH])
			hp = hshtab;
	}
	if(++hshused >= NHASH) {
		yyerror("Symbol table overflow");
		exit(2);
	}
	if (instflg) {
		for (p1=yytext,p2=nextsym->name; p1<yytext+NCPS;)
			*p2++ = *p1++;
		*hp = nextsym++;
	}
	return(hp);
}

cksrc(s, d)
{
	switch(d&(7<<8)) {
	case DSTREGH:
	case DSTREGL:
	case DSTREG:
	case DSTBREG:
		if ((s&017) != (d&017)) {
			yyerror("illegal source");
			return(d);
		}
	}
	return(s|d);
}

ckreg(s, d)
{
	switch(d&(7<<8)) {
	case DSTREGH:
	case DSTREGL:
		if ((s&017) != 0) {
			yyerror("illegal source");
			return(d);
		}
		break;
	case DSTREG:
	case DSTBREG:
		if ((s&017) != (d&017)) {
			yyerror("illegal source");
			return(d);
		}
	}
	return(s|d);
}

ckdst(a, b)
{
	if ((a&DSTMARI) && (b&DSTMARI)) {
		yyerror("illegal destination");
		return(a);
	}
	if ((a&DSTBREG) && (b&DSTBREG)) {
		if ((((a&DSTBREG)!=DSTBRG) || ((b&DSTBREG)!=DSTREG))
		 && (((b&DSTBREG)!=DSTBRG) || ((a&DSTBREG)!=DSTREG))) {
			yyerror("illegal destination");
			return(a);
		}
	}
	return(a|b);
}

putins(ins)
{
	char	csr[8];
	register p;

	p = dot->value++;
	if (dot->value==NKMCI) {
		dot->value = 0;
	}
	if ((p&01777)==01777) {
		yyerror("warning: text 1k boundary crossing");
	}
	if (dbmode) {
		if (reloc[p]&RLCBR) {
			yyerror("undefined trace instruction");
			reloc[p] = 0;
			return;
		}
		if (ksty(KMS, csr, ins)==0)
			fprintf(stdout, "%4o%4o%4o%4o%7o\n",
			csr[2]&0377,csr[3]&0377,csr[4]&0377,csr[5]&0377,ins);
		return;
	}
	if (reloc[p]&RLUSE) {
		yyerror("text location redefined");
		return;
	}
	ispace[p] = ins;
	reloc[p] |= RLUSE;
	if (++p>tsize)
		tsize = p;
}

putdat(ins)
{
	register p;

	p = dot->value++;
	if (dot->value==NKMCD)
		dot->value = 0;
	if (dbmode) {
		lseek(dbfd, (long)(p+NKMCI*2), 0);
		write(dbfd, &ins, 1);
		return;
	}
	if (dspace[p]) {
		yyerror("data location redefined");
		return;
	}
	dspace[p] = ins;
	if (++p>dsize)
		dsize = p;
}

backup(sp, val)
register struct symtab *sp;
{
	register backp, next;

	for (backp = sp->value; backp; backp = (reloc[next]&RLBUP)) {
		next = backp&RLNEXT;
		switch(backp&RLCMASK) {
		case RLCBR:
			if ((next&~01777) != (val&~01777))
				yyerror("forward branch error");
			ispace[next] |= (val&01400)<<3;
		case RLCMV:
			ispace[next] |= val&0377;
			break;
		case RLCPOP:
			ispace[next] |= (val>>8)&0377;
			break;
		case RLCPG:
			ispace[next] |= (val&01400)<<3;
			break;
		}
	}
	sp->value = val;
}
