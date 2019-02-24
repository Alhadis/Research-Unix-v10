/*
 *  link editor	for the PDP11		(modified by S. Hartwell)
 *	Phase I: overkill with the short's: (every int --> short)
 *					Notables are commented "god"
 */

#include <signal.h>
#include "sys/types.h"
typedef unsigned short ushort;	/*god: do this if not in sys/types.h*/
#include "sys/stat.h"
#include "ar.h"

#ifndef LIBPATH
#define LIBPATH	"/lib:/usr/lib"	/*god: fix dependencies. is tweakable*/
#endif				/*god: may someday be taken from env */
#ifndef LIBPREFIX
#define LIBPREFIX "/lib"	/*god: for constructing lib{arg}.a and such*/
#endif				/*god: VAX looks for 11libc.a*/
#ifndef A_DOT_OUT
#define A_DOT_OUT "a.out"
#endif

char Libbuf[100];			/*god: store lib name here */

/*	Layout of a.out file :
 *
 *	header of 8 words	magic number 405, 407, 410, 411
 *				text size	)
 *				data size	) in bytes but even
 *				bss size	)
 *				symbol table size
 *				entry point
 *				{unused}
 *				flag set if no relocation
 *
 *
 *	header:		0
 *	text:		16
 *	data:		16+textsize
 *	relocation:	16+textsize+datasize
 *	symbol table:	16+2*(textsize+datasize) or 16+textsize+datasize
 *
 */
#define TRUE	1
#define FALSE	0


#define OMAGIC	0405
#define	FMAGIC	0407
#define	NMAGIC	0410
#define	IMAGIC	0411

#define	EXTERN	040
#define	UNDEF	00
#define	ABS	01
#define	TEXT	02
#define	DATA	03
#define	BSS	04
#define	COMM	05	/* internal use only */

#define	RABS	00
#define	RTEXT	02
#define	RDATA	04
#define	RBSS	06
#define	REXT	010

#define NOVLY	16
#define	RELFLG	01
#define	NROUT	256
#define	NSYM	1103
#define	NSYMPR	1000

char	premeof[] = "Premature EOF";
char	goodnm[] = "__.SYMDEF";

/* table of contents stuff */
#define TABSZ	900
struct tab
{	char cname[8];
	long cloc;
} tab[TABSZ];
short tnum;


/* overlay management */
short	vindex;
struct overlay {
	short	argsav;
	short	symsav;
	struct liblist	*libsav;
	char	*vname;
	ushort ctsav, cdsav, cbsav;
	ushort offt, offd, offb, offs;
} vnodes[NOVLY];

/* input management */
struct page {
	short	nuser;
	short	bno;
	short	nibuf;
	short	buff[256];
} page[2];

struct {
	short	nuser;
	short	bno;
} fpage;

struct stream {
	short	*ptr;
	short	bno;
	short	nibuf;
	short	size;
	struct page	*pno;
};

struct stream text;
struct stream reloc;

struct	ar_hdr archdr;

struct {
	short	fmagic;
	ushort tsize;
	ushort dsize;
	ushort bsize;
	ushort ssize;
	short	entry;
	short	pad;
	short	relflg;
} filhdr;


/* one entry for each archive member referenced;
 * set in first pass; needs restoring for overlays
 */
struct liblist {
	long	loc;
};

struct liblist	liblist[NROUT];
struct liblist	*libp = liblist;


/* symbol management */
struct symbol {
	char	sname[8];
	char	stype;
	char	spare;
	ushort svalue;
};

struct local {
	short locindex;		/* index to symbol in file */
	struct symbol *locsymbol;	/* ptr to symbol table */
};

struct symbol	cursym;			/* current symbol */
struct symbol	symtab[NSYM];		/* actual symbols */
struct symbol	**symhash[NSYM];	/* ptr to hash table entry */
struct symbol	*lastsym;		/* last symbol entered */
short	symindex;		/* next available symbol table entry */
struct symbol	*hshtab[NSYM+2];	/* hash table for symbols */
struct local	local[NSYMPR];

/* internal symbols */
struct symbol	*p_etext;
struct symbol	*p_edata;
struct symbol	*p_end;
struct symbol	*entrypt;

short	trace;
/* flags */
short	xflag;		/* discard local symbols */
short	Xflag;		/* discard locals starting with 'L' */
short	Sflag;		/* discard all except locals and globals*/
short	rflag;		/* preserve relocation bits, don't define common */
short	arflag;		/* original copy of rflag */
short	sflag;		/* discard all symbols */
short	nflag;		/* pure procedure */
short	Oflag;		/* set magic # to 0405 (overlay) */
short	dflag;		/* define common even with rflag */
short	iflag;		/* I/D space separated */
short	vflag;		/* overlays used */
short	mflag;		/* monitor routines loaded */
char	*Libpath = LIBPATH;	/*god: search path for libraries */
				/*god: reset by -L option */
char	*progname;	/*god: holds argv[0] */

short	libcnt;
short	ofilfnd;
char	*ofilename = "l.out";
short	infil;
char	*filname;

/* cumulative sizes set in pass 1 */
ushort tsize;
ushort dsize;
ushort bsize;
ushort ssize;

/* symbol relocation; both passes */
ushort ctrel;
ushort cdrel;
ushort cbrel;

short	errlev;
short	delarg	= 4;
char	tfname[] = "/tmp/ldaXXXXX";


/* output management */
struct buf {
	short	fildes;
	short	nleft;
	short	*xnext;
	short	iobuf[256];
};
struct buf	toutb;
struct buf	doutb;
struct buf	troutb;
struct buf	droutb;
struct buf	soutb;

long	atol();
long	lseek();

char	*mktemp();

ushort add();
struct symbol	**lookup();
struct symbol	**slookup();
struct symbol	*lookloc();

delexit()
{
	unlink("l.out");
	if (delarg==0)
		chmod(ofilename, 0777 & ~umask(0));
	exit(delarg);
}

main(argc, argv)
char **argv;
{
	register short c, i; 
	short num;
	register char *ap, **p;
	short found; 
	short vscan; 
	char save;

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, delexit);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		signal(SIGTERM, delexit);

	progname = argv[0];	/*god: for error messages */

	if (argc == 1)
		exit(4);
	p = argv+1;

	/* scan files once to find symdefs */
	for (c=1; c<argc; c++) {
		if (trace) printf("%s:\n", *p);
		filname = 0;
		ap = *p++;

		if (*ap == '-') {
			for (i=1; ap[i]; i++) {
			switch (ap[i]) {

			case 'm':
				mflag++;
				continue;

			case 'o':
				if (++c >= argc)
					error(2, "Bad output file");
				ofilename = *p++;
				ofilfnd++;
				continue;

			case 'u':
			case 'e':
				if (++c >= argc)
					error(2, "Bad 'use' or 'entry'");
				enter(slookup(*p++));
				if (ap[i]=='e')
					entrypt = lastsym;
				continue;

			case 'v':
				if (++c >= argc)
					error(2, "-v: arg missing");
				vflag=TRUE;
				vscan = vindex; 
				found=FALSE;
				while (--vscan>=0 && found==FALSE)
					found = eq(vnodes[vscan].vname, *p);
				if (found) {
					endload(c, argv);
					restore(vscan);
				} else
					record(c, *p);
				p++;
				continue;

			case 'D':
				if (++c >= argc)
					error(2, "-D: arg missing");
				num = atoi(*p++);
				if (dsize>num)
					error(2, "-D: too small");
				dsize = num;
				continue;

			case 'L':	/*god: user specified LIBPATH */
				Libpath = &ap[2];
				break;

			case 'l':
				save = ap[--i]; 
				ap[i]='-';
				load1arg(&ap[i]); 
				ap[i]=save;
				break;

			case 'x':
				xflag++;
				continue;

			case 'X':
				Xflag++;
				continue;

			case 'S':
				Sflag++; 
				continue;

			case 'r':
				rflag++;
				arflag++;
				continue;

			case 's':
				sflag++;
				xflag++;
				continue;

			case 'n':
				nflag++;
				continue;

			case 'd':
				dflag++;
				continue;

			case 'i':
				iflag++;
				continue;

			case 'O':
				Oflag++;
				continue;

			case 't':
				trace++;
				continue;

			default:
				error(2, "bad flag");
			} /*endsw*/
			break;
			} /*endfor*/
		} else
			load1arg(ap);
	}
	endload(argc, argv);
}

/* used after pass 1 */
short	nsym;
ushort torigin;
ushort dorigin;
ushort borigin;

endload(argc, argv)
short argc; 
char **argv;
{
	register short c, i; 
	short dnum;
	register char *ap, **p;
	filname = 0;
	middle();
	setupout();
	p = argv+1;
	libp = liblist;
	for (c=1; c<argc; c++) {
		ap = *p++;
		if (trace) printf("%s:\n", ap);
		if (*ap == '-') {
			for (i=1; ap[i]; i++) {
			switch (ap[i]) {
			case 'D':
				for (dnum = atoi(*p); dorigin<dnum; dorigin += 2) {
					putw(0, &doutb);
					if (rflag)
						putw(0, &droutb);
				}
			case 'u':
			case 'e':
			case 'o':
			case 'v':
				++c; 
				++p;

			default:
				continue;

			case 'L':	/*god*/
				ap[i--] = '\0';		/*god: skip over -L*/
				continue;

			case 'l':
				ap[--i]='-'; 
				load2arg(&ap[i]);
				break;
			} /*endsw*/
			break;
			} /*endfor*/
		} else
			load2arg(ap);
	}
	finishout();
}

record(c, nam)
short c; 
char *nam;
{
	register struct overlay *v;

	v = &vnodes[vindex++];
	v->argsav = c;
	v->symsav = symindex;
	v->libsav = libp;
	v->vname = nam;
	v->offt = tsize; 
	v->offd = dsize; 
	v->offb = bsize; 
	v->offs = ssize;
	v->ctsav = ctrel; 
	v->cdsav = cdrel; 
	v->cbsav = cbrel;
}

restore(vscan)
short vscan;
{
	register struct overlay *v;
	register short saved;

	v = &vnodes[vscan];
	vindex = vscan+1;
	libp = v->libsav;
	ctrel = v->ctsav; 
	cdrel = v->cdsav; 
	cbrel = v->cbsav;
	tsize = v->offt; 
	dsize = v->offd; 
	bsize = v->offb; 
	ssize = v->offs;
	saved = v->symsav;
	while (symindex>saved)
		*symhash[--symindex]=0;
}

/* scan file to find defined symbols */
load1arg(acp)
char *acp;
{
	register char *cp;
	long nloc;

	cp = acp;
	libcnt = 0;
	switch (getfile(cp)) {
	case 0:
		load1(0, 0L);
		break;

	/* regular archive */
	case 1:
		nloc = SARMAG/2;
		while (step(nloc))
			nloc += (atol(archdr.ar_size) + sizeof(archdr) + 1) >> 1;
		break;

	/* table of contents */
	case 2:
		tnum = atol(archdr.ar_size) / sizeof(struct tab);
		if (tnum >= TABSZ) {
			error(2, "fast load buffer too small");
		}
		lseek(infil, (long)(SARMAG+sizeof(archdr)), 0);
		read(infil, (char *)tab, tnum * sizeof(struct tab));
		while (ldrand())
			;
		libp->loc = -1;
		libp++;
		break;
	/* out of date table of contents */
	case 3:
		error(0, "out of date (warning)");
		for(nloc = SARMAG/2+((atol(archdr.ar_size)+sizeof(archdr)+1) >> 1); step(nloc);
			   nloc += (atol(archdr.ar_size) + sizeof(archdr) + 1) >> 1)
			;
		break;
	}
	close(infil);
}

step(nloc)
long nloc;
{
	dseek(&text, nloc, sizeof(archdr));
	if (text.size <= 0) {
		libp->loc = -1;
		libp++;
		return(0);
	}
	archget();
	if (load1(1, nloc + (sizeof archdr) / 2)) {
		libp->loc = nloc;
		libp++;
	}
	return(1);
}

ldrand()
{
	register short i;
	register struct symbol *sp;
	register struct liblist *oldp = libp;
	for(i = 0; i<tnum; i++) {
		if ((sp = *slookup(tab[i].cname)) == 0)
			continue;
		if (sp->stype != EXTERN+UNDEF)
			continue;
		step(tab[i].cloc >> 1);
	}
	return(oldp != libp);
}

ushort
add(a,b,s)
ushort a, b;
char *s;
{
	long r;

	r = (long)a + b;
	if (r >= 0200000)
		error(1,s);
	return(r);
}


/* single file or archive member */
load1(libflg, loc)
long loc;
{
	register struct symbol *sp;
	short savindex;
	short ndef, type, mtype;
	ushort nloc;

	readhdr(loc);
	ctrel = tsize;
	cdrel += dsize;
	cbrel += bsize;
	ndef = 0;
	nloc = sizeof cursym;
	savindex = symindex;
	if ((filhdr.relflg&RELFLG)==1) {
		error(1, "No relocation bits");
		return(0);
	}
	loc += (sizeof filhdr)/2 + filhdr.tsize + filhdr.dsize;
	dseek(&text, loc, filhdr.ssize);
	while (text.size > 0) {
		mget((short *)&cursym, sizeof cursym);
		type = cursym.stype;
		if (Sflag) {
			mtype = type&037;
			if (mtype==1 || mtype>4) {
				continue;
			}
		}
		if ((type&EXTERN)==0) {
			if (Xflag==0 || cursym.sname[0]!='L')
				nloc += sizeof cursym;
			continue;
		}
		symreloc();
		if (enter(lookup()))
			continue;
		if ((sp = lastsym)->stype != EXTERN+UNDEF)
			continue;
		if (cursym.stype == EXTERN+UNDEF) {
			ushort oval = sp->svalue;
			if (cursym.svalue > sp->svalue)
				sp->svalue = cursym.svalue;
			if (oval || cursym.svalue==0)
				continue;
		}
		if (sp->svalue != 0 && cursym.stype == EXTERN+TEXT)
			continue;
		if (ndef==0 && libflg)
			announce(cursym.sname);
		ndef++;
		sp->stype = cursym.stype;
		sp->svalue = cursym.svalue;
	}
	if (libflg==0)
		announce((char *)0);
	if (libflg==0 || ndef) {
		tsize = add(tsize,filhdr.tsize,"text overflow");
		dsize = add(dsize,filhdr.dsize,"data overflow");
		bsize = add(bsize,filhdr.bsize,"bss overflow");
		ssize = add(ssize,nloc,"symbol table overflow");
		return((short) 1);
	}
	/*
	 * No symbols defined by this library member.
	 * Rip out the hash table entries and reset the symbol table.
	 */
	while (symindex>savindex)
		*symhash[--symindex]=0;
	return(0);
}

announce(symb)
char *symb;
{
	if (mflag==0)
		return;
	if (symb) {
		if (libcnt==0)
			printf("%s:\n", filname);
		printf(" %.14s (%.8s)\n", archdr.ar_name, symb);
		libcnt++;
	} else
		printf("%s\n", filname);
}

middle()
{
	register struct symbol *sp, *symp;
	register ushort t, csize, corigin;
	short nund;

	torigin=0; 
	dorigin=0; 
	borigin=0;

	p_etext = *slookup("_etext");
	p_edata = *slookup("_edata");
	p_end = *slookup("_end");
	/*
	 * If there are any undefined symbols, save the relocation bits.
	 */
	symp = &symtab[symindex];
	if (rflag==0) {
		for (sp = symtab; sp<symp; sp++)
			if (sp->stype==EXTERN+UNDEF && sp->svalue==0
				&& sp!=p_end && sp!=p_edata && sp!=p_etext) {
				rflag++;
				dflag = 0;
				break;
			}
	}
	if (rflag)
		nflag = sflag = iflag = Oflag = 0;
	/*
	 * Assign common locations.
	 */
	csize = 0;
	if (dflag || rflag==0) {
		ldrsym(p_etext, tsize, EXTERN+TEXT);
		ldrsym(p_edata, dsize, EXTERN+DATA);
		ldrsym(p_end, bsize, EXTERN+BSS);
		for (sp = symtab; sp<symp; sp++)
			if (sp->stype==EXTERN+UNDEF && (t = sp->svalue)!=0) {
				t = (t+1) & ~01;
				sp->svalue = csize;
				sp->stype = EXTERN+COMM;
				csize = add(csize, t, "bss overflow");
			}
	}
	/*
	 * Now set symbols to their final value
	 */
	if (nflag || iflag)
		tsize = (tsize + 077) & ~077;
	dorigin = tsize;
	if (nflag)
		dorigin = (tsize+017777) & ~017777;
	if (iflag)
		dorigin = 0;
	corigin = dorigin + dsize;
	borigin = corigin + csize;
	nund = 0;
	for (sp = symtab; sp<symp; sp++) switch (sp->stype) {
	case EXTERN+UNDEF:
		errlev |= 01;
		if (arflag==0 && sp->svalue==0) {
			if (nund==0)
				printf("Undefined:\n");
			nund++;
			printf("%.8s\n", sp->sname);
		}
		continue;

	case EXTERN+ABS:
	default:
		continue;

	case EXTERN+TEXT:
		sp->svalue += torigin;
		continue;

	case EXTERN+DATA:
		sp->svalue += dorigin;
		continue;

	case EXTERN+BSS:
		sp->svalue += borigin;
		continue;

	case EXTERN+COMM:
		sp->stype = EXTERN+BSS;
		sp->svalue += corigin;
		continue;
	}
	if (sflag || xflag)
		ssize = 0;
	bsize = add(bsize, csize, "bss overflow");
	nsym = ssize / (sizeof cursym);
}

ldrsym(asp, val, type)
struct symbol *asp;
ushort val;
{
	register struct symbol *sp;

	if ((sp = asp) == 0)
		return;
	if (sp->stype != EXTERN+UNDEF || sp->svalue) {
		printf("%.8s: ", sp->sname);
		error(1, "Multiply defined");
		return;
	}
	sp->stype = type;
	sp->svalue = val;
}

setupout()
{
	tcreat(&toutb, 0);
	mktemp(tfname);
	tcreat(&doutb, 1);
	if (sflag==0 || xflag==0)
		tcreat(&soutb, 1);
	if (rflag) {
		tcreat(&troutb, 1);
		tcreat(&droutb, 1);
	}
	filhdr.fmagic = (Oflag ? OMAGIC :( iflag ? IMAGIC : ( nflag ? NMAGIC : FMAGIC )));
	filhdr.tsize = tsize;
	filhdr.dsize = dsize;
	filhdr.bsize = bsize;
	filhdr.ssize = sflag? 0: (ssize + (sizeof cursym)*symindex);
	if (entrypt) {
		if (entrypt->stype!=EXTERN+TEXT)
			error(1, "Entry point not in text");
		else
			filhdr.entry = entrypt->svalue | 01;
	} else
		filhdr.entry=0;
	filhdr.pad = 0;
	filhdr.relflg = (rflag==0);
	mput(&toutb, (short *)&filhdr, sizeof filhdr);
}

tcreat(buf, tempflg)
struct buf *buf;
{
	register short ufd; 
	char *nam;
	nam = (tempflg ? tfname : ofilename);
	if ((ufd = creat(nam, 0666)) < 0)
		error(2, tempflg?"cannot create temp":"cannot create output");
	close((int) ufd); 
	buf->fildes = open(nam, 2);
	if (tempflg)
		unlink(tfname);
	buf->nleft = sizeof(buf->iobuf)/sizeof(short);
	buf->xnext = buf->iobuf;
}

load2arg(acp)
char *acp;
{
	register char *cp;
	register struct liblist *lp;

	cp = acp;
	if (getfile(cp) == 0) {
		while (*cp)
			cp++;
		while (cp >= acp && *--cp != '/');
		mkfsym(++cp);
		load2(0L);
	} else {	/* scan archive members referenced */
		for (lp = libp; lp->loc != -1; lp++) {
			dseek(&text, lp->loc, sizeof archdr);
			archget();
			mkfsym(archdr.ar_name);
			load2(lp->loc + (sizeof archdr) / 2);
		}
		libp = ++lp;
	}
	close(infil);
}

load2(loc)
long loc;
{
	register struct symbol *sp;
	register struct local *lp;
	register short symno;
	short type, mtype;

	readhdr(loc);
	ctrel = torigin;
	cdrel += dorigin;
	cbrel += borigin;
	/*
	 * Reread the symbol table, recording the numbering
	 * of symbols for fixing external references.
	 */
	lp = local;
	symno = -1;
	loc += (sizeof filhdr)/2;
	dseek(&text, loc + filhdr.tsize + filhdr.dsize, filhdr.ssize);
	while (text.size > 0) {
		symno++;
		mget((short *)&cursym, sizeof cursym);
		symreloc();
		type = cursym.stype;
		if (Sflag) {
			mtype = type&037;
			if (mtype==1 || mtype>4) continue;
		}
		if ((type&EXTERN) == 0) {
			if (!sflag&&!xflag&&(!Xflag||cursym.sname[0]!='L'))
				mput(&soutb, (short *)&cursym, sizeof cursym);
			continue;
		}
		if ((sp = *lookup()) == 0)
			error(2, "internal error: symbol not found");
		if (cursym.stype == EXTERN+UNDEF) {
			if (lp >= &local[NSYMPR])
				error(2, "Local symbol overflow");
			lp->locindex = symno;
			lp++->locsymbol = sp;
			continue;
		}
		if (cursym.stype!=sp->stype || cursym.svalue!=sp->svalue) {
			printf("%.8s: ", cursym.sname);
			error(1, "Multiply defined");
		}
	}
	dseek(&text, loc, filhdr.tsize);
	dseek(&reloc, loc + half(filhdr.tsize + filhdr.dsize), filhdr.tsize);
	load2td(lp, ctrel, &toutb, &troutb);
	dseek(&text, loc+half(filhdr.tsize), filhdr.dsize);
	dseek(&reloc, loc+filhdr.tsize+half(filhdr.dsize), filhdr.dsize);
	load2td(lp, cdrel, &doutb, &droutb);
	torigin += filhdr.tsize;
	dorigin += filhdr.dsize;
	borigin += filhdr.bsize;
}

load2td(lp, creloc, b1, b2)
struct local *lp;
struct buf *b1, *b2;
ushort creloc;
{
	register r, t;
	register struct symbol *sp;

	for (;;) {
		/*
			 * The pickup code is copied from "get" for speed.
			 */

		/* next text or data word */
		if (--text.size <= 0) {
			if (text.size < 0)
				break;
			text.size++;
			t = get(&text);
		} else if (--text.nibuf < 0) {
			text.nibuf++;
			text.size++;
			t = get(&text);
		} else
			t = *text.ptr++;

		/* next relocation word */
		if (--reloc.size <= 0) {
			if (reloc.size < 0)
				error(2, "Relocation error");
			reloc.size++;
			r = get(&reloc);
		} else if (--reloc.nibuf < 0) {
			reloc.nibuf++;
			reloc.size++;
			r = get(&reloc);
		} else
			r = *reloc.ptr++;

		switch (r&016) {

		case RTEXT:
			t += ctrel;
			break;

		case RDATA:
			t += cdrel;
			break;

		case RBSS:
			t += cbrel;
			break;

		case REXT:
			sp = lookloc(lp, r);
			if (sp->stype==EXTERN+UNDEF) {
				r = (r&01) + ((nsym+(sp-symtab))<<4) + REXT;
				break;
			}
			t += sp->svalue;
			r = (r&01) + ((sp->stype-(EXTERN+ABS))<<1);
			break;
		}
		if (r&01)
			t -= creloc;
		putw(t, b1);
		if (rflag)
			putw(r, b2);
	}
}

finishout()
{
	register short n, *p;

	if (nflag||iflag) {
		n = torigin;
		while (n&077) {
			n += 2;
			putw(0, &toutb);
			if (rflag)
				putw(0, &troutb);
		}
	}
	copy(&doutb);
	if (rflag) {
		copy(&troutb);
		copy(&droutb);
	}
	if (sflag==0) {
		if (xflag==0)
			copy(&soutb);
		for (p = (short *)symtab; p < (short *)&symtab[symindex];)
			putw(*p++, &toutb);
	}
	flush(&toutb);
	close(toutb.fildes);
	if (!ofilfnd) {
		unlink(A_DOT_OUT);
		if (link("l.out", A_DOT_OUT) < 0) {
			printf ("%s: cannot create %s\n",
				progname, A_DOT_OUT);
			delexit();
		}
		ofilename = A_DOT_OUT;
	}
	delarg = errlev;
	delexit();
}

copy(buf)
struct buf *buf;
{
	register short f, *p, n;

	flush(buf);
	lseek((int)(f = buf->fildes), (long)0, 0);
	while ((n = read(f, (char *)doutb.iobuf, sizeof(doutb.iobuf))) > 1) {
		n >>= 1;
		p = (short *)doutb.iobuf;
		do
			putw(*p++, &toutb);
		while (--n);
	}
	close(f);
}

mkfsym(s)
char *s;
{

	if (sflag || xflag)
		return;
	cp8c(s, cursym.sname);
	cursym.stype = 037;
	cursym.svalue = torigin;
	mput(&soutb, (short *)&cursym, sizeof cursym);
}

mget(aloc, an)
short *aloc;
{
	register short *loc, n;
	register short *p;

	n = an;
	n >>= 1;
	loc = aloc;
	if ((text.nibuf -= n) >= 0) {
		if ((text.size -= n) > 0) {
			p = text.ptr;
			do
				*loc++ = *p++;
			while (--n);
			text.ptr = p;
			return;
		} else
			text.size += n;
	}
	text.nibuf += n;
	do {
		*loc++ = get(&text);
	} 
	while (--n);
}

mput(buf, aloc, an)
struct buf *buf; 
short *aloc;
{
	register short *loc;
	register short n;

	loc = aloc;
	n = an>>1;
	do {
		putw(*loc++, buf);
	} 
	while (--n);
}

dseek(asp, aloc, s)
long aloc;
struct stream *asp;
ushort s;
{
	register struct stream *sp;
	register struct page *p;
	/* register */ long b, o;
	short n;

	b = aloc >> 8;
	o = aloc & 0377;
	sp = asp;
	--sp->pno->nuser;
	if ((p = &page[0])->bno!=b && (p = &page[1])->bno!=b)
		if (p->nuser==0 || (p = &page[0])->nuser==0) {
			if (page[0].nuser==0 && page[1].nuser==0)
				if (page[0].bno < page[1].bno)
					p = &page[0];
			p->bno = b;
			lseek(infil, (aloc & ~0377L) << 1, 0);
			if ((n = read(infil, (char *)p->buff, 512)>>1) < 0)
				n = 0;
			p->nibuf = n;
	} else
		error(2, "No pages");
	++p->nuser;
	sp->bno = b;
	sp->pno = p;
	sp->ptr = p->buff + o;
	if (s != (ushort) -1)		/*god*/
		sp->size = half(s);
	if ((sp->nibuf = p->nibuf-o) <= 0)
		sp->size = 0;
}

half(i)
ushort i;
{
	return(i>>1);
}

get(asp)
struct stream *asp;
{
	register struct stream *sp;

	sp = asp;
	if (--sp->nibuf < 0) {
		dseek(sp, (long)(sp->bno + 1) << 8, (ushort) -1);/*god*/
		--sp->nibuf;
	}
	if (--sp->size <= 0) {
		if (sp->size < 0)
			error(2, premeof);
		++fpage.nuser;
		--sp->pno->nuser;
		sp->pno = (struct page *)&fpage;
	}
	return(*sp->ptr++);
}

getfile(acp)
char *acp;
{
	char *magbuf[SARMAG];
	register char *cp;
	register short c;
	struct stat x;

	cp = acp; 
	infil = -1;
	archdr.ar_name[0] = '\0';
	filname = cp;
	if (cp[0]=='-' && cp[1]=='l') {
		char libfile[20];	/*god: holds template for libname*/
		register char *Libp = Libpath;
		filname = Libbuf;
		if(cp[2] == '\0')
			cp = "-la";
		strcpy(libfile, LIBPREFIX);	/*god: lib names constructed */
		strcat(libfile, cp+2);		/*god: as LIBPREFIX{arg}.a */
		strcat(libfile, ".a");

		/*god: now loop thru LIBPATH looking for lib name*/

		while ( *Libp ) {
		    for (cp = Libbuf; *Libp && *Libp != ':'; *cp++ = *Libp++)
			;
		    *cp = '\0';
		    if ( *Libp == ':' ) Libp++;
		    strcat(Libbuf, libfile);	/*god:add path to libfile name*/
		    if ( (infil = open(Libbuf, 0)) >= 0 ) {
			    if (trace) printf("found lib %s\n", Libbuf);
			    break;
		    }/*endif*/
		}/*endwhile*/

	}/*endif*/
	if (infil == -1 && (infil = open(filname, 0)) < 0)
		error(2, "cannot open");
	page[0].bno = page[1].bno = -1;
	page[0].nuser = page[1].nuser = 0;
	text.pno = reloc.pno = (struct page *)&fpage;
	fpage.nuser = 2;
	dseek(&text, 0L, SARMAG);
	if (text.size <= 0)
		error(2, premeof);
	mget((short *)magbuf, SARMAG);
	if (strncmp(magbuf, ARMAG, SARMAG))
		return(0);	/* regular file */
	dseek(&text, (long)SARMAG/2, sizeof archdr);	/* word addressing */
	if(text.size <= 0)
		return(1);	/* regular archive */
	archget();
	if(strncmp(archdr.ar_name, goodnm, sizeof(archdr.ar_name)) != 0)
		return(1);	/* regular archive */
	else {
		fstat(infil, &x);
		if(x.st_mtime > atol(archdr.ar_date))
			return(3);
		else return(2);
	}
}

struct symbol **
lookup()
{
	short i; 
	short clash;
	register struct symbol **hp;
	register char *cp, *cp1;

	i = 0;
	for (cp = cursym.sname; cp < &cursym.sname[8];)
		i = (i<<1) + *cp++;
	for (hp = &hshtab[(i&077777)%NSYM+2]; *hp!=0;) {
		cp1 = (*hp)->sname; 
		clash=FALSE;
		for (cp = cursym.sname; cp < &cursym.sname[8];)
			if (*cp++ != *cp1++) {
				clash=TRUE; 
				break;
			}
		if (clash) {
			if (++hp >= &hshtab[NSYM+2])
				hp = hshtab;
		} else
			break;
	}
	return(hp);
}

struct symbol **
slookup(s)
char *s;
{
	cp8c(s, cursym.sname);
	cursym.stype = EXTERN+UNDEF;
	cursym.svalue = 0;
	return(lookup());
}

enter(hp)
struct symbol **hp;
{
	register struct symbol *sp;

	if (*hp==0) {
		if (symindex>=NSYM)
			error(2, "Symbol table overflow");
		symhash[symindex] = hp;
		*hp = lastsym = sp = &symtab[symindex++];
		cp8c(cursym.sname, sp->sname);
		sp->stype = cursym.stype;
		sp->svalue = cursym.svalue;
		return(1);
	} else {
		lastsym = *hp;
		return(0);
	}
}

symreloc()
{
	switch (cursym.stype) {

	case TEXT:
	case EXTERN+TEXT:
		cursym.svalue += ctrel;
		return;

	case DATA:
	case EXTERN+DATA:
		cursym.svalue += cdrel;
		return;

	case BSS:
	case EXTERN+BSS:
		cursym.svalue += cbrel;
		return;

	case EXTERN+UNDEF:
		return;
	}
	if (cursym.stype&EXTERN)
		cursym.stype = EXTERN+ABS;
}

error(n, s)
char *s;
{
	if (errlev==0)
		printf("%s:", progname);
	if (filname) {
		printf("%s", filname);
		if (archdr.ar_name[0])
			printf("(%.14s)", archdr.ar_name);
		printf(": ");
	}
	printf("%s\n", s);
	if (n > 1)
		delexit();
	errlev = n;
}

struct symbol *
lookloc(alp, r)
struct local *alp;
{
	register struct local *clp, *lp;
	register short sn;

	lp = alp;
	sn = (r>>4) & 07777;
	for (clp = local; clp<lp; clp++)
		if (clp->locindex == sn)
			return(clp->locsymbol);
	error(2, "Local symbol botch");
	return((struct symbol *) 0);	/*god: error() doesn't return, lint*/
}

readhdr(loc)
long loc;
{
	register short st, sd;

	dseek(&text, loc, sizeof filhdr);
	mget((short *)&filhdr, sizeof filhdr);
	if (filhdr.fmagic != FMAGIC)
		error(2, "Bad format");
	st = (filhdr.tsize+01) & ~01;
	filhdr.tsize = st;
	cdrel = -st;
	sd = (filhdr.dsize+01) & ~01;
	cbrel = - (st+sd);
	filhdr.bsize = (filhdr.bsize+01) & ~01;
}

cp8c(from, to)
char *from, *to;
{
	register char *f, *t, *te;

	f = from;
	t = to;
	te = t+8;
	while ((*t++ = *f++) && t<te);
	while (t<te)
		*t++ = 0;
}

eq(s1, s2)
char *s1; 
char *s2;
{
	while (*s1==*s2++)
		if ((*s1++)==0)
			return(TRUE);
	return(FALSE);
}

putw(w, b)
register struct buf *b;
{
	*(b->xnext)++ = w;
	if (--b->nleft <= 0)
		flush(b);
}

flush(b)
register struct buf *b;
{
	register short n;

	if ((n = (char *)b->xnext - (char *)b->iobuf) > 0)
		if (write(b->fildes, (char *)b->iobuf, n) != n)
			error(2, "output error");
	b->xnext = b->iobuf;
	b->nleft = sizeof(b->iobuf)/sizeof(short);
}

archget()
{
	register char *cp;

	mget((short *)&archdr, sizeof(archdr));
	for (cp=archdr.ar_name; cp < &archdr.ar_name[sizeof(archdr.ar_name)]; cp++)
		if (*cp == ' ')
			*cp = '\0';
}

long
atol(p)
register char *p;
{
	register long n;

	n = 0;
	while(*p >= '0' && *p <= '9')
		n = n*10 + *p++ - '0';
	return(n);
}
