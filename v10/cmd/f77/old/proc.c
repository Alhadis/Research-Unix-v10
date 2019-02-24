#include "defs"

#ifdef SDB
#	include <a.out.h>
char *stabline();
#	ifndef N_SO
#		include <stab.h>
#	endif
char *stabdline();
#endif
char *memname();
#define ESNULL (struct Extsym *)0
#define EXNULL (union Expression *)0

LOCAL dobss(), docomleng(), docommon(), doentry(),
	epicode(), nextarg(), procode(), retval(); 

/* start a new procedure */

newproc()
{
if(parstate != OUTSIDE)
	{
	execerr("missing end statement", CNULL);
	endproc();
	}

parstate = INSIDE;
procclass = CLMAIN;	/* default */
}



/* end of procedure. generate variables, epilogs, and prologs */

endproc()
{
struct Labelblock *lp;
#if SDB
char elab[10];
int elnum;
#endif

if(parstate < INDATA)
	enddcl();
if(ctlstack >= ctls)
	err("DO loop or BLOCK IF not closed");
for(lp = labeltab ; lp < labtabend ; ++lp)
	if(lp->stateno!=0 && lp->labdefined==NO)
		errstr("missing statement number %s", convic(lp->stateno) );

epicode();
procode();
donmlist();
dobss();
prdbginfo();
#if SDB
if (sdbflag && procclass != CLBLOCK) {
	sprintf(elab, "L%d", elnum = newlabel());
	putlabel(elnum);
	prstab(procclass == CLMAIN ? "MAIN_" : nounder(XL, procname),
		N_EFUN, lineno, elab);
}
#endif
#if FAMILY == PCC
	putbracket();
#endif
fixlwm();
procinit();	/* clean up for next procedure */
}



/* End of declaration section of procedure.  Allocate storage. */

enddcl()
{
register struct Entrypoint *ep;

#ifdef SDB
		if( sdbflag )
			{
#	ifdef UCBVAXASM
			p2pass( stabdline(N_SLINE, lineno) );
#	else
			char buff[10];
			sprintf(buff,"LL%d", ++dbglabel);
			p2pass( stabline(0, N_SLINE, lineno, buff) );
			p2pi("LL%d:\n", dbglabel);
#	endif
			}
#endif
parstate = INEXEC;
docommon();
doequiv();
docomleng();
for(ep = entries ; ep ; ep = ep->entnextp)
	doentry(ep);
}

/* ROUTINES CALLED WHEN ENCOUNTERING ENTRY POINTS */

/* Main program or Block data */

startproc(progname, class)
struct Extsym * progname;
int class;
{
register struct Entrypoint *p;
char *ftnname();

p = ALLOC(Entrypoint);
if(class == CLMAIN)
	puthead("MAIN__", CLMAIN);
else
	puthead(CNULL, CLBLOCK);
if(class == CLMAIN)
	newentry( mkname(5, "MAIN_") );
p->entryname = progname;
p->entrylabel = newlabel();
entries = p;

procclass = class;
retlabel = newlabel();
fprintf(diagfile, "   %s", (class==CLMAIN ? "MAIN" : "BLOCK DATA") );
if(progname)
	fprintf(diagfile, " %s", nounder(XL, procname = progname->extname) );
fprintf(diagfile, ":\n");
#ifdef SDB
if(sdbflag && class==CLMAIN)
	{
	prstab("MAIN__", N_BFUN, lineno, ftnname(STGEXT, "MAIN__"));
#if NOTDEF
	p2pass( stabline("MAIN_", N_FNAME, 0, 0) );
#endif
	if(progname)
		{
		prstab(nounder(XL,progname->extname), N_ENTRY, lineno,
			ftnname(STGEXT, progname->extname));
/*		p2pass(stabline(nounder(XL,progname->extname),N_FNAME,0,0));	*/
		}
	}
#endif
}

/* subroutine or function statement */

struct Extsym *newentry(v)
register Namep v;
{
register struct Extsym *p;

p = mkext( varunder(VL, v->varname) );

if(p==NULL || p->extinit || ! ONEOF(p->extstg, M(STGUNKNOWN)|M(STGEXT)) )
	{
	if(p == 0)
		dclerr("invalid entry name", v);
	else	dclerr("external name already used", v);
	return(0);
	}
v->vstg = STGAUTO;
v->vprocclass = PTHISPROC;
v->vclass = CLPROC;
p->extstg = STGEXT;
p->extinit = YES;
return(p);
}


entrypt(class, type, length, entry, args)
int class, type;
ftnint length;
struct Extsym *entry;
chainp args;
{
register Namep q;
register struct Entrypoint *p, *ep;
extern int types3[];

if(class != CLENTRY)
	puthead( varstr(XL, procname = entry->extname), class);
if(class == CLENTRY)
	fprintf(diagfile, "       entry ");
fprintf(diagfile, "   %s:\n", nounder(XL, entry->extname));
q = mkname(VL, nounder(XL,entry->extname) );

if( (type = lengtype(type, (int) length)) != TYCHAR)
	length = 0;
if(class == CLPROC)
	{
	procclass = CLPROC;
	proctype = type;
	procleng = length;

	retlabel = newlabel();
	if(type == TYSUBR)
		ret0label = newlabel();
	}

p = ALLOC(Entrypoint);

if(entries)	/* put new block at end of entries list */
	{
	for(ep = entries; ep->entnextp; ep = ep->entnextp)
		;
	ep->entnextp = p;
	}
else
	entries = p;

p->entryname = entry;
p->arglist = args;
p->entrylabel = newlabel();
p->enamep = q;

#ifdef SDB
if(sdbflag)
	{
/*	prstab(nounder(XL, entry->extname),
 *		(class==CLENTRY ? N_ENTRY : N_BFUN),
 *		lineno, ftnname(STGEXT, entry->extname));
 */
	prstab(entry->extname,
		(class==CLENTRY ? N_ENTRY : N_BFUN),
		lineno, ftnname(STGEXT, entry->extname));
	if (class != CLENTRY)
		prstab(entry->extname, N_GSYM, types3[type], CNULL);
	if(class != CLENTRY) {
#if NOTDEF
/*		p2pass( stabline( nounder(XL,entry->extname), N_FNAME, 0, 0) );
 */
		p2pass( stabline( entry->extname, N_FNAME, 0, 0) );
#endif
		}
	}
#endif

if(class == CLENTRY)
	{
	class = CLPROC;
	if(proctype == TYSUBR)
		type = TYSUBR;
	}

q->vclass = class;
q->vprocclass = PTHISPROC;
settype(q, type, (int) length);
/* hold all initial entry points till end of declarations */
if(parstate >= INDATA)
	doentry(p);
}

/* generate epilogs */

LOCAL epicode()
{
register int i;

if(procclass==CLPROC)
	{
	if(proctype==TYSUBR)
		{
		putlabel(ret0label);
		if(substars)
			putforce(TYINT, ICON(0) );
		putlabel(retlabel);
		goret(TYSUBR);
		}
	else	{
		putlabel(retlabel);
		if(multitype)
			{
			typeaddr = autovar(1, TYADDR, EXNULL);
			putbranch( cpexpr(typeaddr) );
			for(i = 0; i < NTYPES ; ++i)
				if(rtvlabel[i] != 0)
					{
					putlabel(rtvlabel[i]);
					retval(i);
					}
			}
		else
			retval(proctype);
		}
	}

else if(procclass != CLBLOCK)
	{
	putlabel(retlabel);
	goret(TYSUBR);
	}
}


/* generate code to return value of type  t */

LOCAL retval(t)
register int t;
{
register Addrp p;

switch(t)
	{
	case TYCHAR:
	case TYCOMPLEX:
	case TYDCOMPLEX:
		break;

	case TYLOGICAL:
		t = tylogical;
	case TYADDR:
	case TYSHORT:
	case TYLONG:
		p = (Addrp) cpexpr(retslot);
		p->vtype = t;
		putforce(t, p);
		break;

	case TYREAL:
	case TYDREAL:
		p = (Addrp) cpexpr(retslot);
		p->vtype = t;
		putforce(t, p);
		break;

	default:
		badtype("retval", t);
	}
goret(t);
}


/* Allocate extra argument array if needed. Generate prologs. */

LOCAL procode()
{
register struct Entrypoint *p;
Addrp argvec;

#if TARGET==GCOS
	argvec = autovar(lastargslot/SZADDR, TYADDR, PNULL);
#else
	if(lastargslot>0 && nentry>1)
#if TARGET == VAX
		argvec = autovar(1 + lastargslot/SZADDR, TYADDR, EXNULL);
#else
		argvec = autovar(lastargslot/SZADDR, TYADDR, EXNULL);
#endif
	else
		argvec = NULL;
#endif


#if TARGET == PDP11
	/* for the optimizer */
	if(fudgelabel)
		putlabel(fudgelabel);
#endif

for(p = entries ; p ; p = p->entnextp)
	prolog(p, argvec);

#if FAMILY == PCC
	putrbrack(procno);
#endif

prendproc();
}

/*
   manipulate argument lists (allocate argument slot positions)
 * keep track of return types and labels
 */

LOCAL doentry(ep)
struct Entrypoint *ep;
{
register int type;
register Namep np;
chainp p;
register Namep q;
Addrp mkarg();

++nentry;
if(procclass == CLMAIN)
	{
#ifdef SDB
	if (sdbflag)
		prstab(CNULL, N_LBRAC, 0, "2");
#endif
	putlabel(ep->entrylabel);
	return;
	}
else if(procclass == CLBLOCK) {
#ifdef SDB
	if (sdbflag)
		prstab(CNULL, N_LBRAC, 0, "2");
#endif
	return;
	}

impldcl( np = mkname(VL, nounder(XL, ep->entryname->extname) ) );
type = np->vtype;
if(proctype == TYUNKNOWN)
	if( (proctype = type) == TYCHAR)
		procleng = (np->vleng ? np->vleng->constblock.const.ci : (ftnint) (-1));

if(proctype == TYCHAR)
	{
	if(type != TYCHAR)
		err("noncharacter entry of character function");
	else if( (np->vleng ? np->vleng->constblock.const.ci : (ftnint) (-1)) != procleng)
		err("mismatched character entry lengths");
	}
else if(type == TYCHAR)
	err("character entry of noncharacter function");
else if(type != proctype)
	multitype = YES;
if(rtvlabel[type] == 0)
	rtvlabel[type] = newlabel();
ep->typelabel = rtvlabel[type];

if(type == TYCHAR)
	{
	if(chslot < 0)
		{
		chslot = nextarg(TYADDR);
		chlgslot = nextarg(TYLENG);
		}
	np->vstg = STGARG;
	np->vardesc.varno = chslot;
	if(procleng < 0)
		np->vleng = (expptr) mkarg(TYLENG, chlgslot);
#ifdef SDB
	if (sdbflag) {
		prstab("ret_val", N_PSYM, 34, "4");
		prstab("ret_val_len", N_PSYM, 5, "8");
		}
#endif
	}
else if( ISCOMPLEX(type) )
	{
	np->vstg = STGARG;
	if(cxslot < 0)
		cxslot = nextarg(TYADDR);
	np->vardesc.varno = cxslot;
#ifdef SDB
	if (sdbflag) {
		prstab("ret_val", N_PSYM, 40, "4");
		prstab(type == TYCOMPLEX ? "complex" : "dcomplex",
			N_TYID, 0, CNULL);
		}
#endif
	}
else if(type != TYSUBR)
	{
	if(nentry == 1)
		retslot = autovar(1, TYDREAL, EXNULL);
	np->vstg = STGAUTO;
	np->voffset = retslot->memoffset->constblock.const.ci;
	}

for(p = ep->arglist ; p ; p = p->nextp)
	if(! (( q = (Namep) (p->datap) )->vdcldone) )
		q->vardesc.varno = nextarg(TYADDR);

for(p = ep->arglist ; p ; p = p->nextp)
	if(! (( q = (Namep) (p->datap) )->vdcldone) )
		{
		impldcl(q);
		q->vdcldone = YES;
#ifdef SDB
		if(sdbflag)
			prstabtype(ESNULL, q, N_PSYM,
				convic(q->vardesc.varno + ARGOFFSET));
#endif
		if(q->vtype == TYCHAR)
			{
			if(q->vleng == NULL)	/* character*(*) */
				q->vleng = (expptr)
						mkarg(TYLENG, nextarg(TYLENG) );
			else if(nentry == 1)
				nextarg(TYLENG);
			}
		else if(q->vclass==CLPROC && nentry==1)
			nextarg(TYLENG) ;
		}
#ifdef SDB
if (sdbflag)
	prstab(CNULL, N_LBRAC, 0, "2");
#endif

putlabel(ep->entrylabel);
}



LOCAL nextarg(type)
int type;
{
int k;
k = lastargslot;
lastargslot += typesize[type];
return(k);
}

/* generate variable references */

/* the following computes an expression that would address an equivalenced
 * variable -- if only one could have expressions in stab addrs...
 *
 * LOCAL char *memplusoff(q)
 * register Namep q;
 * {	static char buf[32];
 *	char *s;
 *	ftnint offset;
 *	s = memname(STGEQUIV, q->vardesc.varno);
 *	if (!(offset = q->voffset))
 *		return s;
 *	sprintf(buf, offset > 0 ? "%s+%ld" : "%s%ld", s, offset);
 *	return buf;
 *	}
 */

LOCAL dobss()
{
register struct Hashentry *p;
register Namep q;
register int i;
int align;
ftnint leng, iarrl, i1arrlen();
int qstg, qclass, qtype;
char buf[20];

pruse(asmfile, USEBSS);

for(p = hashtab ; p<lasthash ; ++p)
    if(q = p->varp)
	{
	qstg = q->vstg;
	qtype = q->vtype;
	qclass = q->vclass;

#ifdef SDB
                if(sdbflag && qclass==CLVAR) switch(qstg)
                        {
                        case STGAUTO:
				prstabtype(ESNULL, q, N_LSYM,
					convic(-q->voffset));
                                break;

                        case STGBSS:
				prstabtype(ESNULL, q, N_LCSYM,
					memname(qstg,q->vardesc.varno));
                                break;

                        case STGINIT:
				prstabtype(ESNULL, q, N_STSYM,
					memname(qstg,q->vardesc.varno));
                                break;

			case STGEQUIV:
				i = eqvclass[q->vardesc.varno].eqvinit
					? N_STSYM : N_LCSYM;
				/* prstabtype(ESNULL, q, i, memplusoff(q)); */
				if (!q->voffset)
				   prstabtype(ESNULL, q, i,
					memname(qstg,q->vardesc.varno));
				break;
                        }
#endif

	if( (qclass==CLUNKNOWN && qstg!=STGARG) ||
	    (qclass==CLVAR && qstg==STGUNKNOWN) )
		warn1("local variable %s never used", varstr(VL,q->varname) );
	else if(qclass==CLVAR && qstg==STGBSS)
		{
		align = (qtype==TYCHAR ? ALILONG : typealign[qtype]);
		if(bssleng % align != 0)
			{
			bssleng = roundup(bssleng, align);
			preven(align);
			}
		prlocvar(memname(STGBSS,q->vardesc.varno), iarrl = iarrlen(q) );
		bssleng += iarrl;
		}
	else if(qclass==CLPROC && q->vprocclass==PEXTERNAL && qstg!=STGARG)
		mkext(varunder(VL, q->varname)) ->extstg = STGEXT;

	if(qclass==CLVAR && qstg!=STGARG)
		{
		if(q->vdim && !ISICON(q->vdim->nelt) )
			dclerr("adjustable dimension on non-argument", q);
		if(qtype==TYCHAR && (q->vleng==NULL || !ISICON(q->vleng)))
			dclerr("adjustable leng on nonargument", q);
		}
	}
#ifdef SDB
if (sdbflag) {
	struct Equivblock *p1;
	struct Eqvchain *q1;
	Namep np1;

	for(i = 0, p1 = eqvclass; i < nequiv ; ++i, ++p1) {
		if (p1->eqvtop && (q1 = p1->equivs)) {

			/* put out null-named common block describing	*/
			/* EQUIVALENCED variables with nonzero offsets	*/

			do if ((np1 = q1->eqvitem.eqvname)
					&& np1->voffset) {
				prstab(CNULL, N_BCOMM, 0, CNULL);
				prstab(CNULL, p1->eqvinit ? N_STSYM : N_LCSYM,
					0, memname(STGEQUIV,
						np1->vardesc.varno));
				do if ((np1 = q1->eqvitem.eqvname)
					    && np1->voffset)
						prstabtype(ESNULL, np1, N_SSYM,
							convic(np1->voffset));
					while(q1 = q1->eqvnextp);
				prstab(CNULL, N_ECOML, 0, CNULL);
				break;
				}
			    while(q1 = q1->eqvnextp);
			}
		freqchain(p1);
		}
	}
#endif

for(i = 0 ; i < nequiv ; ++i)
	if(eqvclass[i].eqvinit==NO && (leng = eqvclass[i].eqvleng)!=0 )
		{
		bssleng = roundup(bssleng, ALIDOUBLE);
		preven(ALIDOUBLE);
		prlocvar( memname(STGEQUIV, i), leng);
		bssleng += leng;
		}
#ifdef SDB
if (sdbflag)
	prstab(CNULL, N_RBRAC, 0, "2");
#endif
}



donmlist()
{
register struct Hashentry *p;
register Namep q;

pruse(asmfile, USEINIT);

for(p=hashtab; p<lasthash; ++p)
	if( (q = p->varp) && q->vclass==CLNAMELIST)
		namelist(q);
}


doext()
{
struct Extsym *p;

for(p = extsymtab ; p<nextext ; ++p)
	prext( varstr(XL, p->extname), p->maxleng, p->extinit);
}




ftnint iarrlen(q)
register Namep q;
{
ftnint leng;

leng = typesize[q->vtype];
if(leng <= 0)
	return(-1);
if(q->vdim)
	if( ISICON(q->vdim->nelt) )
		leng *= q->vdim->nelt->constblock.const.ci;
	else	return(-1);
if(q->vleng)
	if( ISICON(q->vleng) )
		leng *= q->vleng->constblock.const.ci;
	else 	return(-1);
return(leng);
}


ftnint i1arrlen(q)
register Namep q;
{
ftnint leng;

leng = 1;
if(q->vdim)
	if( ISICON(q->vdim->nelt) )
		leng = q->vdim->nelt->constblock.const.ci;
	else	return(-1);
if(q->vleng)
	if( ISICON(q->vleng) )
		leng *= q->vleng->constblock.const.ci;
	else 	return(-1);
return(leng);
}

/* This routine creates a static block representing the namelist.
   An equivalent declaration of the structure produced is:
	struct namelist
		{
		char namelistname[16];
		struct namelistentry
			{
			char varname[16];
			char *varaddr;
			int type; # negative means -type= number of chars
			struct dimensions *dimp; # null means scalar
			} names[];
		};

	struct dimensions
		{
		int numberofdimensions;
		int numberofelements
		int baseoffset;
		int span[numberofdimensions];
		};
   where the namelistentry list terminates with a null varname
   If dimp is not null, then the corner element of the array is at
   varaddr.  However,  the element with subscripts (i1,...,in) is at
   varaddr - dimp->baseoffset + sizeoftype * (i1+span[0]*(i2+span[1]*...)
*/

namelist(np)
Namep np;
{
register chainp q;
register Namep v;
register struct Dimblock *dp;
int type, dimno, dimoffset;
flag bad;


preven(ALILONG);
fprintf(asmfile, LABELFMT, memname(STGINIT, np->vardesc.varno));
putstr(asmfile, varstr(VL, np->varname), 16);
dimno = ++lastvarno;
dimoffset = 0;
bad = NO;

for(q = np->varxptr.namelist ; q ; q = q->nextp)
	{
	vardcl( v = (Namep) (q->datap) );
	type = v->vtype;
	if( ONEOF(v->vstg, MSKSTATIC) )
		{
		preven(ALILONG);
		putstr(asmfile, varstr(VL,v->varname), 16);
		praddr(asmfile, v->vstg, v->vardesc.varno, v->voffset);
		prconi(asmfile, TYINT,
			type==TYCHAR ?
			    -(v->vleng->constblock.const.ci) : (ftnint) type);
		if(v->vdim)
			{
			praddr(asmfile, STGINIT, dimno, (ftnint)dimoffset);
			dimoffset += 3 + v->vdim->ndim;
			}
		else
			praddr(asmfile, STGNULL,0,(ftnint) 0);
		}
	else
		{
		dclerr("may not appear in namelist", v);
		bad = YES;
		}
	}

if(bad)
	return;

putstr(asmfile, "", 16);

if(dimoffset > 0)
	{
	fprintf(asmfile, LABELFMT, memname(STGINIT,dimno));
	for(q = np->varxptr.namelist ; q ; q = q->nextp)
		if(dp = q->datap->nameblock.vdim)
			{
			int i;
			prconi(asmfile, TYINT, (ftnint) (dp->ndim) );
			prconi(asmfile, TYINT,
				(ftnint) (dp->nelt->constblock.const.ci) );
			prconi(asmfile, TYINT,
				(ftnint) (dp->baseoffset->constblock.const.ci));
			for(i=0; i<dp->ndim ; ++i)
				prconi(asmfile, TYINT,
					dp->dims[i].dimsize->constblock.const.ci);
			}
	}

}

LOCAL docommon()
{
register struct Extsym *p;
register chainp q;
struct Dimblock *t;
expptr neltp;
register Namep v;
ftnint size;
int type;

for(p = extsymtab ; p<nextext ; ++p)
	if (p->extstg == STGCOMMON && (q = p->extp))
		{
#ifdef SDB
		if(sdbflag)
			prstab(varstr(XL, p->extname), N_BCOMM, 0,
				ftnname(STGCOMMON, p->extname));
#endif
		for(; q ; q = q->nextp)
			{
			v = (Namep) (q->datap);
			if(v->vdcldone == NO)
				vardcl(v);
			type = v->vtype;
			if(p->extleng % typealign[type] != 0)
				{
				dclerr("common alignment", v);
				p->extleng = roundup(p->extleng, typealign[type]);
				}
			v->voffset = p->extleng;
			v->vardesc.varno = p - extsymtab;
			if(type == TYCHAR)
				size = v->vleng->constblock.const.ci;
			else	size = typesize[type];
			if(t = v->vdim)
				if( (neltp = t->nelt) && ISCONST(neltp) )
					size *= neltp->constblock.const.ci;
				else
					dclerr("adjustable array in common", v);
#ifdef SDB
			if(sdbflag)
				prcomssym(v, p);
#endif
			p->extleng += size;
			}
		frchain( &(p->extp) );
#ifdef SDB
		if(sdbflag)
			prstab(varstr(XL,p->extname), N_ECOMM, 0, ftnname(STGCOMMON, p->extname));
#endif
		}
}

#ifdef SDB
#define todata() if(first){p2pass(USEDATA);first=0;}

commstruct()	/* put common blocks into pi's GLOBAL menu */
		/* also emit complex and dcomplex struct defs if needed */
{
	register struct Extsym *p;
	register struct Comvar *cv;
	int first = 1;
	char cstbuf[XL+10], lenbuf[16], *s;

	for(p = extsymtab ; p<nextext ; ++p)
		if (p->extstg == STGCOMMON && (cv = p->cv))
			{
			todata();
			s = varstr(XL, p->extname);
			strcpy(cstbuf, s);
			strcat(cstbuf, "COMMON_");
			prstab(s, N_GSYM, 8, CNULL);
			prstab(cstbuf, N_TYID, 0, CNULL);
			prstab(cstbuf, N_BSTR, 8, CNULL);
			do {
				sprintf(lenbuf, "%ld", cv->offset);
				prstab(cv->name, N_SSYM, cv->type, lenbuf);
				if (cv->tyid)
					prstab(cv->tyid, N_TYID, 0, CNULL);
				if (cv->nelt)
					p2pass(stabdline(N_DIM, cv->nelt));
				} while(cv = cv->next);	
			sprintf(lenbuf, "%ld", p->maxleng);
			prstab(cstbuf, N_ESTR, 8, lenbuf);
			}
	if (complex_seen) {
		todata();
		prstab("complex", N_BSTR, 8, CNULL);
		prstab("real", N_SSYM, 6, CNULL);
		prstab("imag", N_SSYM, 6, "4");
		prstab("complex", N_ESTR, 8, "8");
		}
	if (dcomplex_seen) {
		todata();
		prstab("dcomplex", N_BSTR, 8, CNULL);
		prstab("real", N_SSYM, 7, CNULL);
		prstab("imag", N_SSYM, 7, "8");
		prstab("dcomplex", N_ESTR, 8, "16");
		}
	if (!first)
		p2pass(USETEXT);
	}
#endif



LOCAL docomleng()
{
register struct Extsym *p;

for(p = extsymtab ; p < nextext ; ++p)
	if(p->extstg == STGCOMMON)
		{
		if(p->maxleng!=0 && p->extleng!=0 && p->maxleng!=p->extleng
		    && !eqn(XL,"_BLNK__ ",p->extname) )
			warn1("incompatible lengths for common block %s",
				nounder(XL, p->extname) );
		if(p->maxleng < p->extleng)
			p->maxleng = p->extleng;
		p->extleng = 0;
	}
}




/* ROUTINES DEALING WITH AUTOMATIC AND TEMPORARY STORAGE */

frtemp(p)
Addrp p;
{
/* restore clobbered character string lengths */
if(p->vtype==TYCHAR && p->varleng!=0)
	{
	frexpr(p->vleng);
	p->vleng = ICON(p->varleng);
	}

/* put block on chain of temps to be reclaimed */
holdtemps = mkchain(p, holdtemps);
}




/* allocate an automatic variable slot */

Addrp autovar(nelt, t, lengp)
register int nelt, t;
expptr lengp;
{
ftnint leng;
register Addrp q;

if(t == TYCHAR)
	if( ISICON(lengp) )
		leng = lengp->constblock.const.ci;
	else	{
		fatal("automatic variable of nonconstant length");
		}
else
	leng = typesize[t];
autoleng = roundup( autoleng, typealign[t]);

q = ALLOC(Addrblock);
q->tag = TADDR;
q->vtype = t;
if(t == TYCHAR)
	{
	q->vleng = ICON(leng);
	q->varleng = leng;
	}
q->vstg = STGAUTO;
q->ntempelt = nelt;
#if TARGET==PDP11 || TARGET==VAX
	/* stack grows downward */
	autoleng += nelt*leng;
	q->memoffset = ICON( - autoleng );
#else
	q->memoffset = ICON( autoleng );
	autoleng += nelt*leng;
#endif

return(q);
}


Addrp mktmpn(nelt, type, lengp)
int nelt;
register int type;
expptr lengp;
{
ftnint leng;
chainp p, oldp;
register Addrp q;

if(type==TYUNKNOWN || type==TYERROR)
	badtype("mktmpn", type);

if(type==TYCHAR)
	if( ISICON(lengp) )
		leng = lengp->constblock.const.ci;
	else	{
		err("adjustable length");
		return( (Addrp) errnode() );
		}
/*
 * if an temporary of appropriate shape is on the templist,
 * remove it from the list and return it
 */

for(oldp=CHNULL, p=templist  ;  p  ;  oldp=p, p=p->nextp)
	{
	q = (Addrp) (p->datap);
	if(q->vtype==type && q->ntempelt==nelt &&
	    (type!=TYCHAR || q->vleng->constblock.const.ci==leng) )
		{
		if(oldp)
			oldp->nextp = p->nextp;
		else
			templist = p->nextp;
		free( (charptr) p);
		return(q);
		}
	}
q = autovar(nelt, type, lengp);
q->istemp = YES;
return(q);
}




Addrp mktemp(type, lengp)
int type;
expptr lengp;
{
return( mktmpn(1,type,lengp) );
}

/* VARIOUS ROUTINES FOR PROCESSING DECLARATIONS */

struct Extsym *comblock(len, s)
register int len;
register char *s;
{
struct Extsym *p;

if(len == 0)
	{
	s = BLANKCOMMON;
	len = strlen(s);
	}
p = mkext( varunder(len, s) );
if(p->extstg == STGUNKNOWN)
	p->extstg = STGCOMMON;
else if(p->extstg != STGCOMMON)
	{
	errstr("%s cannot be a common block name", s);
	return(0);
	}

return( p );
}


incomm(c, v)
struct Extsym *c;
Namep v;
{
if(v->vstg != STGUNKNOWN)
	dclerr("incompatible common declaration", v);
else
	{
	v->vstg = STGCOMMON;
	c->extp = hookup(c->extp, mkchain(v,CHNULL) );
	}
}




settype(v, type, length)
register Namep  v;
register int type;
register int length;
{
if(type == TYUNKNOWN)
	return;

if(type==TYSUBR && v->vtype!=TYUNKNOWN && v->vstg==STGARG)
	{
	v->vtype = TYSUBR;
	frexpr(v->vleng);
	}
else if(type < 0)	/* storage class set */
	{
	if(v->vstg == STGUNKNOWN)
		v->vstg = - type;
	else if(v->vstg != -type)
		dclerr("incompatible storage declarations", v);
	}
else if(v->vtype == TYUNKNOWN)
	{
	if( (v->vtype = lengtype(type, length))==TYCHAR && length>=0)
		v->vleng = ICON(length);
	}
else if(v->vtype!=type || (type==TYCHAR && v->vleng->constblock.const.ci!=length) )
	dclerr("incompatible type declarations", v);
}





lengtype(type, length)
register int type;
register int length;
{
switch(type)
	{
	case TYREAL:
		if(length == 8)
			return(TYDREAL);
		if(length == 4)
			goto ret;
		break;

	case TYCOMPLEX:
		if(length == 16)
			return(TYDCOMPLEX);
		if(length == 8)
			goto ret;
		break;

	case TYSHORT:
	case TYDREAL:
	case TYDCOMPLEX:
	case TYCHAR:
	case TYUNKNOWN:
	case TYSUBR:
	case TYERROR:
		goto ret;

	case TYLOGICAL:
		if(length == typesize[TYLOGICAL])
			goto ret;
		break;

	case TYLONG:
		if(length == 0)
			return(tyint);
		if(length == 2)
			return(TYSHORT);
		if(length == 4)
			goto ret;
		break;
	default:
		badtype("lengtype", type);
	}

if(length != 0)
	err("incompatible type-length combination");

ret:
	return(type);
}





setintr(v)
register Namep  v;
{
register int k;

if(v->vstg == STGUNKNOWN)
	v->vstg = STGINTR;
else if(v->vstg!=STGINTR)
	dclerr("incompatible use of intrinsic function", v);
if(v->vclass==CLUNKNOWN)
	v->vclass = CLPROC;
if(v->vprocclass == PUNKNOWN)
	v->vprocclass = PINTRINSIC;
else if(v->vprocclass != PINTRINSIC)
	dclerr("invalid intrinsic declaration", v);
if(k = intrfunct(v->varname))
	v->vardesc.varno = k;
else
	dclerr("unknown intrinsic function", v);
}



setext(v)
register Namep  v;
{
if(v->vclass == CLUNKNOWN)
	v->vclass = CLPROC;
else if(v->vclass != CLPROC)
	dclerr("invalid external declaration", v);

if(v->vprocclass == PUNKNOWN)
	v->vprocclass = PEXTERNAL;
else if(v->vprocclass != PEXTERNAL)
	dclerr("invalid external declaration", v);
}




/* create dimensions block for array variable */

setbound(v, nd, dims)
register Namep  v;
int nd;
struct { expptr lb, ub; } dims[ ];
{
register expptr q, t;
register struct Dimblock *p;
int i;

if(v->vclass == CLUNKNOWN)
	v->vclass = CLVAR;
else if(v->vclass != CLVAR)
	{
	dclerr("only variables may be arrays", v);
	return;
	}

v->vdim = p = (struct Dimblock *)
		ckalloc( sizeof(int) + (3+2*nd)*sizeof(expptr) );
p->ndim = nd;
p->nelt = ICON(1);

for(i=0 ; i<nd ; ++i)
	{
	if( (q = dims[i].ub) == NULL)
		{
		if(i == nd-1)
			{
			frexpr(p->nelt);
			p->nelt = NULL;
			}
		else
			err("only last bound may be asterisk");
		p->dims[i].dimsize = ICON(1);;
		p->dims[i].dimexpr = NULL;
		}
	else
		{
		if(dims[i].lb)
			{
			q = mkexpr(OPMINUS, q, cpexpr(dims[i].lb));
			q = mkexpr(OPPLUS, q, ICON(1) );
			}
		if( ISCONST(q) )
			{
			p->dims[i].dimsize = q;
			p->dims[i].dimexpr = (expptr) PNULL;
			}
		else	{
			p->dims[i].dimsize = (expptr) autovar(1, tyint, EXNULL);
			p->dims[i].dimexpr = q;
			}
		if(p->nelt)
			p->nelt = mkexpr(OPSTAR, p->nelt,
					cpexpr(p->dims[i].dimsize) );
		}
	}

q = dims[nd-1].lb;
if(q == NULL)
	q = ICON(1);

for(i = nd-2 ; i>=0 ; --i)
	{
	t = dims[i].lb;
	if(t == NULL)
		t = ICON(1);
	if(p->dims[i].dimsize)
		q = mkexpr(OPPLUS, t, mkexpr(OPSTAR, cpexpr(p->dims[i].dimsize), q) );
	}

if( ISCONST(q) )
	{
	p->baseoffset = q;
	p->basexpr = NULL;
	}
else
	{
	p->baseoffset = (expptr) autovar(1, tyint, EXNULL);
	p->basexpr = q;
	}
}
