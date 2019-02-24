#include "defs"
#if FAMILY == DMR
#	include "dmrdefs"
#endif
#if FAMILY==PCC
#	include "pccdefs"
#endif

/*
      PDP 11-SPECIFIC ROUTINES
*/

int maxregvar = 0;
int regnum[] = { 3, 2 };

ftnint intcon[14] =
	{ 2, 2, 2, 2,
	  15, 31, 24, 56,
	  -128, -128, 127, 127,
	  32767, 2147483647 };

#if HERE == PDP11
	/* then put in constants in octal */
long realcon[6][2] =
	{
		{ 040000000, 0 },
		{ 040000000, 0 },
		{ 017777777777, 0 },
		{ 017777777777, 037777777777 },
		{ 06440000000, 0 },
		{ 04440000000, 0 }
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




prsave()
{
}



goret(type)
int type;
{
#if  FAMILY == DMR
	p2op(P2RETURN);
#endif
#if FAMILY==PCC
	p2pass("\tjmp\tcret");
#endif
}




/*
 * move argument slot arg1 (relative to ap)
 * to slot arg2 (relative to ARGREG)
 */

mvarg(type, arg1, arg2)
int type, arg1, arg2;
{
mvarg1(arg1+4, arg2);
if(type == TYLONG)
	mvarg1(arg1+6, arg2+2);
}




mvarg1(m, n)
int m, n;
{
#if FAMILY == DMR
	p2reg(ARGREG, P2SHORT|P2PTR);
	p2op2(P2ICON, P2SHORT);
	p2i(n);
	p2op2(P2PLUS, P2SHORT|P2PTR);
	p2op2(P2INDIRECT, P2SHORT);
	p2reg(AUTOREG, P2SHORT|P2PTR);
	p2op2(P2ICON, P2SHORT);
	p2i(m);
	p2op2(P2PLUS, P2SHORT|P2PTR);
	p2op2(P2INDIRECT, P2SHORT);
	p2op2(P2ASSIGN, P2SHORT);
	putstmt();
#endif
#if FAMILY == PCC
	p2pij("\tmov\t%d.(r5),%d.(r4)", m, n);
#endif
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
register int *np;
np = &n;
if(type == TYLONG)
	fprintf(fp, "\t%d.;%d.\n", np[0], np[1]);
else
	fprintf(fp, "\t%d.\n", np[1]);
}



prcona(fp, a)
FILEP fp;
ftnint a;
{
fprintf(fp, "L%ld\n", a);
}



#if HERE!=PDP11
BAD NEWS
#endif

#if HERE==PDP11
prconr(fp, type, x)
FILEP fp;
int type;
double x;
{
register int k, *n;
n = &x;	/* nonportable cheat */
k = (type==TYREAL ? 2 : 4);
fprintf(fp, "\t");
while(--k >= 0)
	fprintf(fp, "%d.%c", *n++, (k==0 ? '\n' : ';') );
}
#endif




preven(k)
int k;
{
if(k > 1)
	fprintf(asmfile, "\t.even\n", k);
}



#if FAMILY == PCC

prcmgoto(p, nlab, skiplabel, labarray)
expptr p;
int nlab, skiplabel, labarray;
{
int regno;

putforce(p->vtype, p);

if(p->vtype == TYLONG)
	{
	regno = 1;
	p2pass("\ttst\tr0");
	p2pi("\tbne\tL%d", skiplabel);
	}
else
	regno = 0;

p2pij("\tcmp\tr%d,$%d.", regno, nlab);
p2pi("\tbhi\tL%d", skiplabel);
p2pi("\tasl\tr%d", regno);
p2pij("\tjmp\t*L%d(r%d)", labarray, regno);
}


prarif(p, neg,zer,pos)
expptr p;
int neg, zer, pos;
{
register int ptype;

putforce( ptype = p->vtype, p);
if( ISINT(ptype) )
	{
	p2pass("\ttst\tr0");
	p2pi("\tjlt\tL%d", neg);
	p2pi("\tjgt\tL%d", pos);
	if(ptype != TYSHORT)
		{
		p2pass("\ttst\tr1");
		p2pi("\tjeq\tL%d", zer);
		}
	p2pi("\tjbr\tL%d", pos);
	}
else
	{
	p2pass("\ttstf\tr0");
	p2pass("\tcfcc");
	p2pi("\tjeq\tL%d", zer);
	p2pi("\tjlt\tL%d", neg);
	p2pi("\tjmp\tL%d", pos);
	}
}

#endif




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
		fatali("memname: invalid vstg %d", stg);
	}
return(s);
}


prlocvar(s, len)
char *s;
ftnint len;
{
fprintf(asmfile, "%s:", s);
prskip(asmfile, len);
}



prext(name, leng, init)
char *name;
ftnint leng;
int init;
{
if(leng==0 || init)
	fprintf(asmfile, "\t.globl\t_%s\n", name);
else
	fprintf(asmfile, "\t.comm\t_%s,%ld.\n", name, leng);
}



prendproc()
{
}



prtail()
{
#if FAMILY == PCC
	p2pass("\t.globl\tcsv,cret");
#else
	p2op(P2EOF);
#endif
}



prolog(ep, argvec)
struct Entrypoint *ep;
struct Addrblock *argvec;
{
int i, argslot, proflab;
register chainp p;
register struct Nameblock *q;
register struct Dimblock *dp;
struct Constblock *mkaddcon();

if(procclass == CLMAIN)
	prentry("MAIN__");

if(ep->entryname)
	prentry( varstr(XL, ep->entryname->extname) );

if(procclass == CLBLOCK)
	return;
if(profileflag)
	proflab = newlabel();
#if FAMILY == PCC
	if(profileflag)
		{
		fprintf(asmfile, "L%d:\t. = .+2\n", proflab);
		p2pi("\tmov\t$L%d,r0", proflab);
		p2pass("\tjsr\tpc,mcount");
		}
	p2pass("\tjsr\tr5,csv");
	p2pi("\tsub\t$.F%d,sp", procno);
#else
	if(profileflag)
		p2op2(P2PROFILE, proflab);
	p2op(P2SAVE);
	p2op2(P2SETSTK, ( (((int) autoleng)+1) & ~01) );
#endif

if(argvec == NULL)
	addreg(argloc = 4);
else
	{
	addreg( argloc = argvec->memoffset->constblock.const.ci );
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
		q = p->datap;
		mvarg(TYADDR, argslot, q->vardesc.varno);
		argslot += SZADDR;
		}
	for(p = ep->arglist ; p ; p = p->nextp)
		{
		q = p->datap;
		if(q->vtype==TYCHAR || q->vclass==CLPROC)
			{
			if( q->vleng && ! ISCONST(q->vleng) )
				mvarg(TYLENG, argslot, q->vleng->addrblock.memno);
			argslot += SZLENG;
			}
		}
	}

for(p = ep->arglist ; p ; p = p->nextp)
	if(dp = ( (struct Nameblock *) (p->datap) ) ->vdim)
		{
		for(i = 0 ; i < dp->ndim ; ++i)
			if(dp->dims[i].dimexpr)
				puteq( fixtype(cpexpr(dp->dims[i].dimsize)),
					fixtype(cpexpr(dp->dims[i].dimexpr)));
		if(dp->basexpr)
			puteq( 	cpexpr(fixtype(dp->baseoffset)),
				cpexpr(fixtype(dp->basexpr)));
		}

if(typeaddr)
	puteq( cpexpr(typeaddr), mkaddcon(ep->typelabel) );
putgoto(ep->entrylabel);
}



prentry(s)
char *s;
{
#if FAMILY == PCC
	p2ps("_%s:", s);
#else
	p2op(P2RLABEL);
	putc('_', textfile);
	p2str(s);
#endif
}




addreg(k)
int k;
{
#if FAMILY == PCC
	p2pass("\tmov\tr5,r4");
	p2pi("\tadd\t$%d.,r4", k);
#else
	p2reg(ARGREG, P2SHORT);
	p2reg(AUTOREG, P2SHORT);
	p2op2(P2ICON, P2SHORT);
	p2i(k);
	p2op2(P2PLUS, P2SHORT);
	p2op2(P2ASSIGN, P2SHORT);
	putstmt();
#endif
}





prhead(fp)
FILEP fp;
{
#if FAMILY == PCC
	p2triple(P2LBRACKET, ARGREG-1-highregvar, procno);
	p2word( (long) (BITSPERCHAR*autoleng) );
	p2flush();
#endif
}

prdbginfo()
{
register char *s;
char *t, buff[50];
register struct Nameblock *p;
struct Hashentry *hp;

if(s = entries->entryname->extname)
	s = varstr(XL, s);
else if(procclass == CLMAIN)
	s = "MAIN__";
else
	return;

if(procclass != CLBLOCK)
	fprintf(asmfile, "~~%s = _%s\n", s, s);

for(hp = hashtab ; hp<lasthash ; ++hp)
    if(p = hp->varp)
	{
	s = NULL;
	if(p->vstg == STGARG)
		{
		sprintf(buff, "%o", p->vardesc.varno+argloc);
		s = buff;
		}
	else if(p->vclass == CLVAR)
		switch(p->vstg)
			{
			case STGBSS:
			case STGINIT:
			case STGEQUIV:
				t = memname(p->vstg, p->vardesc.varno);
				if(p->voffset)
					sprintf(buff, "%s+%o", t, p->voffset);
				else
					sprintf(buff, "%s", t);
				s = buff;
				break;

			case STGAUTO:
				sprintf(buff, "%o", p->voffset);
				s = buff;
				break;

			default:
				break;
			}
	if(s)
		fprintf(asmfile, "~%s = %s\n", varstr(VL,p->varname), s);
	}
fprintf(asmfile, "~~:\n");
}
