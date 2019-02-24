#include "defs"

#ifdef SDB
#	include <a.out.h>
extern int types2[];
#	ifndef N_SO
#		include <stab.h>
#	endif
#endif

#include "pccdefs"

/*
	VAX-11/780 - SPECIFIC ROUTINES
*/


int maxregvar = MAXREGVAR;
int regnum[] =  { 
	11, 10, 9, 8, 7, 6 };
static int regmask[] = { 
	0, 0x800, 0xc00, 0xe00, 0xf00, 0xf80, 0xfc0 };



ftnint intcon[14] =
{ 
	2, 2, 2, 2,
	    15, 31, 24, 56,
	    -128, -128, 127, 127,
	    32767, 2147483647 };

#if HERE == VAX
/* then put in constants in octal */
long realcon[6][2] =
{
	{ 0200, 0 },
	{ 0200, 0 },
	{ 037777677777, 0 },
	{ 037777677777, 037777777777 },
	{ 032200, 0 },
	{ 022200, 0 }
};	

#else
double realcon[6] =
{
	2.9387358771e-39,
	2.938735877055718800e-39
	1.7014117332e+38,
	1.701411834604692250e+38
	5.960464e-8,
	1.38777878078144567e-17,
};
#endif




prsave(proflab)
int proflab;
{
	if(profileflag)
	{
		fprintf(asmfile, "L%d:\t.space\t4\n", proflab);
		p2pi("\tmovab\tL%d,r0", proflab);
		p2pass("\tjsb\tmcount");
	}
	p2pi("\tsubl2\t$LF%d,sp", procno);
}



goret(type)
int type;
{
#ifdef SDB
	char *stabdline();

	if (sdbflag)
		p2pass(stabdline(N_RFUN, lineno));
#endif
	p2pass("\tret");
}




/*
 * move argument slot arg1 (relative to ap)
 * to slot arg2 (relative to ARGREG)
 */

mvarg(type, arg1, arg2)
int type, arg1, arg2;
{
	p2pij("\tmovl\t%d(ap),%d(fp)", arg1+ARGOFFSET, arg2+argloc);
}




prlabel(fp, k)
FILEP fp;
int k;
{
	fprintf(fp, "L%d:\n", k);
}



prconi(fp, type, n)
FILEP fp;
int type;
ftnint n;
{
	fprintf(fp, "\t%s\t%ld\n", (type==TYSHORT ? ".word" : ".long"), n);
}



prcona(fp, a)
FILEP fp;
ftnint a;
{
	fprintf(fp, "\t.long\tL%ld\n", a);
}



#ifndef vax
prconr(fp, type, x)
FILEP fp;
int type;
float x;
{
	fprintf(fp, "\t%s\t0f%e\n", (type==TYREAL ? ".float" : ".double"), x);
}
#endif

#ifdef vax
prconr(fp, type, x)
FILEP fp;
int type;
double x;
{
	/* non-portable cheat to preserve bit patterns */
	union { 
		double xd; 
		long int xl[2]; 
	} cheat;
	cheat.xd = x;
	if(type == TYREAL)
	{
		float y = x; 
		fprintf(fp, "\t.long\t0x%X\n", *(long *) &y); 
	}
	else
		fprintf(fp, "\t.long\t0x%X,0x%X\n", cheat.xl[0], cheat.xl[1]);
}
#endif



praddr(fp, stg, varno, offset)
FILE *fp;
int stg, varno;
ftnint offset;
{
	char *memname();

	if(stg == STGNULL)
		fprintf(fp, "\t.long\t0\n");
	else
	{
		fprintf(fp, "\t.long\t%s", memname(stg,varno));
		if(offset)
			fprintf(fp, "+%ld", offset);
		fprintf(fp, "\n");
	}
}




preven(k)
int k;
{
	register int lg;

	if(k > 4)
		lg = 3;
	else if(k > 2)
		lg = 2;
	else if(k > 1)
		lg = 1;
	else
		return;
	fprintf(asmfile, "\t.align\t%d\n", lg);
}



vaxgoto(index, nlab, labs)
expptr index;
register int nlab;
struct Labelblock *labs[];
{
	register int i;
	register int arrlab;

	putforce(TYINT, index);
	p2pi("\tcasel\tr0,$1,$%d", nlab-1);
	p2pi("L%d:", arrlab = newlabel() );
	for(i = 0; i< nlab ; ++i)
		if( labs[i] )
			p2pij("\t.word\tL%d-L%d", labs[i]->labelno, arrlab);
}


prarif(p, neg, zer, pos)
expptr p;
int neg, zer, pos;
{
	int vtype = p->headblock.vtype; /* putforce() will free p */
	putforce(p->headblock.vtype, p);
	if( ISINT(vtype) )
		p2pass("\ttstl\tr0");
	else
		p2pass("\ttstd\tr0");
	p2pi("\tjlss\tL%d", neg);
	p2pi("\tjeql\tL%d", zer);
	p2pi("\tjbr\tL%d", pos);
}




char *memname(stg, mem)
int stg, mem;
{
	static char s[20];

	switch(stg)
	{
	case STGCOMMON:
	case STGEXT:
		sprintf(s, "_%s", varstr(XL, extsymtab[mem].extname) );
		break;

	case STGBSS:
	case STGINIT:
		sprintf(s, "v.%d", mem);
		break;

	case STGCONST:
		sprintf(s, "L%d", mem);
		break;

	case STGEQUIV:
		sprintf(s, "q.%d", mem+eqvstart);
		break;

	default:
		badstg("memname", stg);
	}
	return(s);
}

/*
 * this shouldn't be here (or memname shouldn't)
 * but i'm too lazy to fix it
 */

char *
ftnname(stg, name)
char *name;
{
	static char s[40];

	switch (stg) {
	case STGCOMMON:
	case STGEXT:
		sprintf(s, "_%s", varstr(XL, name) );
		break;
	default:
		badstg("ftnname", stg);
	}
	return (s);
}




prlocvar(s, len)
char *s;
ftnint len;
{
	fprintf(asmfile, "\t.lcomm\t%s,%ld\n", s, len);
}



prext(name, leng, init)
char *name;
ftnint leng;
int init;
{
	if(leng == 0)
		fprintf(asmfile, "\t.globl\t_%s\n", name);
	else
		fprintf(asmfile, "\t.comm\t_%s,%ld\n", name, leng);
}





prendproc()
{
}




prtail()
{
}





prolog(ep, argvec)
struct Entrypoint *ep;
Addrp  argvec;
{
	int i, argslot;
	int size;
	register chainp p;
	register Namep q;
	register struct Dimblock *dp;
	expptr tp;

	p2pass("\t.align\t1");


	if(procclass == CLMAIN) {
		if(fudgelabel)
		{
			if(ep->entryname) {
				p2ps("_%s:",  varstr(XL, ep->entryname->extname));
				p2pi("\t.word\tLWM%d", procno);
			}
			putlabel(fudgelabel);
			fudgelabel = 0;
			fixlwm();
		}
		else
		{
			p2pass( "_MAIN__:" );
			if(ep->entryname == NULL)
				p2pi("\t.word\tLWM%d", procno);
		}

	} else if(ep->entryname)
		if(fudgelabel)
		{
			putlabel(fudgelabel);
			fudgelabel = 0;
			fixlwm();
		}
		else
		{
			p2ps("_%s:",  varstr(XL, ep->entryname->extname));
			p2pi("\t.word\tLWM%d", procno);
			prsave(newlabel());
		}

	if(procclass == CLBLOCK)
		return;
	if(argvec)
	{
		argloc = argvec->memoffset->constblock.Const.ci + SZINT;
		/* first slot holds count */
		if(proctype == TYCHAR)
		{
			mvarg(TYADDR, 0, chslot);
			mvarg(TYLENG, SZADDR, chlgslot);
			argslot = SZADDR + SZLENG;
		}
		else if( ISCOMPLEX(proctype) )
		{
			mvarg(TYADDR, 0, cxslot);
			argslot = SZADDR;
		}
		else
			argslot = 0;

		for(p = ep->arglist ; p ; p =p->nextp)
		{
			q = (Namep) (p->datap);
			mvarg(TYADDR, argslot, q->vardesc.varno);
			argslot += SZADDR;
		}
		for(p = ep->arglist ; p ; p = p->nextp)
		{
			q = (Namep) (p->datap);
			if(q->vtype==TYCHAR && q->vclass!=CLPROC)
			{
				if(q->vleng && ! ISCONST(q->vleng) )
					mvarg(TYLENG, argslot,
					    q->vleng->addrblock.memno);
				argslot += SZLENG;
			}
		}
		p2pi("\taddl3\t$%d,fp,ap", argloc-ARGOFFSET);
		p2pi("\tmovl\t$%d,(ap)\n", lastargslot/SZADDR);
	}

	for(p = ep->arglist ; p ; p = p->nextp)
	{
		q = (Namep) (p->datap);
		if(dp = q->vdim)
		{
			for(i = 0 ; i < dp->ndim ; ++i)
				if(dp->dims[i].dimexpr)
					puteq( fixtype(cpexpr(dp->dims[i].dimsize)),
					    fixtype(cpexpr(dp->dims[i].dimexpr)));
			size = typesize[ q->vtype ];
			if(q->vtype == TYCHAR)
				if( ISICON(q->vleng) )
					size *= q->vleng->constblock.Const.ci;
				else
					size = -1;

			/* on VAX, get more efficient subscripting if subscripts
		   have zero-base, so fudge the argument pointers for arrays.
		   Not done if array bounds are being checked.
		*/
			if(dp->basexpr)
				puteq( 	cpexpr(fixtype(dp->baseoffset)),
				    cpexpr(dp->basexpr = fixtype(dp->basexpr)));

			if(! checksubs)
			{
				if(dp->basexpr)
				{
					if(size > 0)
						tp = (expptr) ICON(size);
					else
						tp = (expptr) cpexpr(q->vleng);
					putforce(TYINT,
					    fixtype( mkexpr(OPSTAR, tp,
					    cpexpr(dp->baseoffset)) ));
					p2pi("\tsubl2\tr0,%d(ap)",
					    p->datap->nameblock.vardesc.varno +
					    ARGOFFSET);
				}
				else if(dp->baseoffset->constblock.Const.ci != 0)
				{
					char buff[25];
					if(size > 0)
					{
						sprintf(buff, "\tsubl2\t$%ld,%d(ap)",
						    dp->baseoffset->constblock.Const.ci * size,
						    p->datap->nameblock.vardesc.varno +
						    ARGOFFSET);
					}
					else	{
						putforce(TYINT, mkexpr(OPSTAR, cpexpr(dp->baseoffset),
						    cpexpr(q->vleng) ));
						sprintf(buff, "\tsubl2\tr0,%d(ap)",
						    p->datap->nameblock.vardesc.varno +
						    ARGOFFSET);
					}
					p2pass(buff);
				}
			}
		}
	}

	if(typeaddr)
		puteq( cpexpr(typeaddr), mkaddcon(ep->typelabel) );
	/* replace to avoid long jump problem
putgoto(ep->entrylabel);
*/
	p2pi("\tjmp\tL%d", ep->entrylabel);
}

fixlwm()
{
	extern lwmno;
	if (lwmno == procno)
		return;
	fprintf(asmfile, "\t.set\tLWM%d,0x%x\n",
	    procno, regmask[highregvar]);
	lwmno = procno;
}


prhead(fp)
FILEP fp;
{
#if FAMILY==PCC
	p2triple(P2LBRACKET, ARGREG-highregvar, procno);
	p2word( (long) (BITSPERCHAR*autoleng) );
	p2flush();
#endif
}



prdbginfo()
{
}

#ifdef SDB


#	ifdef UCBVAXASM
char *stabdline(code, type)
int code;
int type;
{
	static char buff[30];

	sprintf(buff, "\t.stabd\t0x%x,0,%d", code, type);
	return(buff);
}
#	endif


prstab(s, code, type, loc)
char *s, *loc;
int code, type;
{
	char * stabline();

	if(sdbflag)
		p2pass( stabline(s,code,type,loc) );
}



char *stabline(s, code, type, loc)
register char *s;
int code;
int type;
char *loc;
{
	static char buff[512]	= "\t.stab\t\t";
	register char *t;
	register int i = 0;

#ifdef UCBVAXASM
	t = buff + 8;
	if(s == NULL)
		buff[6] = 'n';	/* .stabn line */
	else
	{
		buff[6] = 's';  /* .stabs line */
		*t++ = '"';
		while(*s!='\0' && *s!=' ')
		{
#ifndef UCBPASS2
			if (i == 8)
				break;
#endif
			*t++ = *s++;
			++i;
		}
		*t++ = '"';
		*t++ = ',';
	}

#else
	t = buff + 7;
	if(s)
		while( *s!='\0' && *s!=' ' && i<8 )
		{
			*t++ = '\'';
			*t++ = *s++;
			*t++ = ',';
			++i;
		}
	for( ; i<8 ; ++i)
	{
		*t++ = '0';
		*t++ = ',';
	}
#endif


	sprintf(t, "0x%x,0,%d,%s", code,  type, (loc? loc : "0") );
	return(buff);
}


/* type cookies for pi */

#define	BASIC	5	/* width of basic type */
#define	PTR	01	/* pointer */
#define	FTN	02	/* function */
#define	ARY	03	/* array */
#define	OF	2	/* shift for `ftn ary ptr of' */

/*
 * extra table of our types to sdb's types
 * our code generator is still pcc1, but sdb expects pcc2
 * to be safe, say exactly what we mean here
 */

int sdbtypes[] = {
	0,	/* TYUNKNOWN 0 */
	PTR|4,	/* TYADDR 1 == ptr to int. eh? */
	3,	/* TYSHORT 2 */
	5,	/* TYLONG 3 */
	6,	/* TYREAL 4 */
	7,	/* TYDREAL 5 */
	6,	/* TYCOMPLEX 6 == float */
	7,	/* TYDCOMPLEX 7 == double */
	5,	/* TYLOGICAL 8 == long? */
	2,	/* TYCHAR 9 */
	4,	/* TYSUBR 10 == int */
	0,	/* TYERROR 11 */
};

#define STRTY 8
#define VOID 16

stabtype(p)
register Namep p;
{
	register int func = 0, type = 0, vt = p->vtype;
	if(p->vstg == STGARG) {
		type = PTR;
		if(p->vclass == CLPROC) {
			type = (FTN << OF) | PTR;
			func = 1;
		}
	}
	else if(p->vdim)
		type = ARY;
	else if(p->vclass == CLPROC) {
		type = FTN;
		func = 1;
	}
	type <<= BASIC;
	if (func && (ISCOMPLEX(vt) || vt == TYCHAR))
		type |= VOID;
	else
		type |= ISCOMPLEX(vt) ? STRTY : types2[vt];
	if (type == 2) type |= (ARY << BASIC);
	return type;
}

prstabtype(xp, q, stype, loc)
struct Extsym *xp;
register Namep q;
char *loc;
{
	int i;
	char *name, *tyid = 0;
	ftnint nelt = 0;
	static char *dc[2] = {
		"complex", "dcomplex"	};

	prstab(name = varstr(VL,q->varname), stype, i = stabtype(q), loc);
	if (ISCOMPLEX(q->vtype))
		prstab(tyid = dc[q->vtype - TYCOMPLEX], N_TYID, 0, CNULL);
	if (ISARRAY(i))
		p2pass(stabdline(N_DIM, (int)(nelt = i1arrlen(q))));
	if (xp)
		commvar(name, nelt, q, tyid, i, xp);
}



prcomssym(np, xp)
register Namep np;
register struct Extsym *xp;
{
	char nbuf[40];

	sprintf(nbuf, "%d", np->voffset);
	prstabtype(xp, np, N_SSYM, nbuf);
}

#define Sgulp 2040
#define CVgulp 120

/* For each common block, save the first appearance of each */
/* variable, along with its offset and type, for adding common */
/* block structures to pi's global menu */
commvar(name, nelt, p, tyid, type, v)
register char *name;
ftnint nelt;
Namep p;
char *tyid;
register struct Extsym *v;
{
	static char *slast, *snext;
	static struct Comvar *cvlast, *cvnext;
	char *malloc(), *strcpy();
	char *s;
	struct Comvar *cv, *cv0, *ncv;
	int k;

	for (cv = cv0 = v->cv; cv; cv0 = cv, cv = cv->next)
		if (!strcmp(name, cv->name))
			return;
	k = strlen(name) + 1;
	s = snext;
	snext += k;
	if (snext > slast) {
		if (!(s = malloc(Sgulp)))
mfailure:
			fatal("Out of memory in commvar");
		snext = s + k;
		slast = s + Sgulp;
	}
	if (cvnext >= cvlast) {
		cvnext = (struct Comvar *)
		    malloc(CVgulp*sizeof(struct Comvar));
		if (!cvnext)
			goto mfailure;
		cvlast = cvnext + CVgulp;
	}
	ncv = cvnext++;
	if (cv0) cv0->next = ncv;
	else v->cv = ncv;
	ncv->next = 0;
	ncv->name = strcpy(s,name);
	ncv->type = type;
	ncv->tyid = tyid;
	ncv->offset = p->voffset;
	ncv->nelt = nelt;
}
#endif
