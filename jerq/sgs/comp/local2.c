/*	@(#) local2.c : 1.6 3/14/84	*/
/*	local2.c - machine dependent grunge for back end
 *
 *		Bellmac32
 *			R. J. Mascitti
 */

# include "mfile2.h"
# define istnode(p) (p->in.op==REG && istreg(p->tn.rval))
# define STHRESH 6	/* max # of words to unroll copy loop */

static int strcall;	/* have we made a call to a structure function? */



lineid(l, fn)			/* identify line l and file fn */
int l;
char *fn;
{
	printf("#	line %d, file %s\n", l, fn);
}


eobl2()				/* end of function stuff */
{
	strcall = 0;
	eobl2dbg();			/* very end debug code */
}


char *rnames[]=
{
	"%r0",
	"%r1",
	"%r2",
	"%r3",
	"%r4",
	"%r5",
	"%r6",
	"%r7",
	"%r8"
};

static char * ccbranches[] =
{
	"je",
	"jne",
	"jle",
	"jl",
	"jge",
	"jg",
	"jleu",
	"jlu",
	"jgeu",
	"jgu"
};

static char * arithjump[] =
{
	"jz",
	"jnz",
	"jnpos",
	"jneg",
	"jnneg",
	"jpos",
	"jnpos",
	"jneg",
	"jnneg",
	"jpos"
};

static int cmpflag = 0;		/* have we done a cmp? */

zzzcode(pnode, ppc, q)		/* hard stuff escaped from templates */
NODE *pnode;
char **ppc;
OPTAB *q;			/* not used in m32 zzzcode */
{
	int stlab, i;
	OPTAB *q;
	NODE *pl, *pr;

	switch (*++(*ppc))
	{

	case 'H':    
	/* put out word relative bit shift for insv and extv
	/* instruction.  Similar to case 'H' case in expand(),
	/* but no difference for different types
	*/
	    {
		extern int fldsz, fldshf;

		pl = getlr( pnode, *++(*ppc) );
		if (pl->tn.op != FLD ) cerror( "bad FLD for ZH" );
		fldsz = UPKFSZ(pl->tn.rval);
		fldshf = SZINT - fldsz - UPKFOFF(pl->tn.rval);
		printf( "%d", fldshf );
	    }
		break;


	case 'D':	/* output address of second half of double */
		dbladrput( pl = getlr(pnode,*++(*ppc)) );
		break;


	case 'B':
		bycode(pnode->in.left->tn.lval, 0);
		break;

	case 'b':
		++cmpflag;
		break;

	case 'C':
		printf("%s\t.L%d\n",
		    cmpflag ? ccbranches[pnode->bn.lop - EQ] :
		    arithjump[pnode->bn.lop - EQ], pnode->bn.label);
		cmpflag = 0;
		break;

	case 'c':		/* # of word-length args */
		printf("%d", pnode->stn.argsize / SZINT);
		break;

	case 'h':		/* deal with the BM32's weird treatment
				   of half-word compares */
		/* If a 16 bit positive constant is used, it will
		   not be sign extended, but the othe operand will */
		{
		register NODE *r, *l;
		int lchg = 0, rchg = 0;
		l = pnode ->in.left;
		if ((l->tn.op == ICON) &&
		    ((l->tn.type == TUSHORT) ||
		     (l->tn.type == TSHORT)) &&
		    (l->tn.lval > 0x7fff))  {
			lchg = 1;
		}
		r = pnode->in.right;
		if ((r->tn.op == ICON) &&
		    ((r->tn.type == TUSHORT) ||
		    (r->tn.type == TSHORT)) &&
		    (r->tn.lval > 0x7fff))  {
			rchg = 1;
		}
		if ((!rchg && !lchg) || (rchg && lchg))  {
			expand(pnode, FORCC, "cmph	AL,AR", q);
		} else if (rchg)  {
			expand(pnode, FORCC,
			    "movzhw	AL,A1\n	cmpw	A1,AR", q);
		} else {
			expand(pnode, FORCC,
			    "movzhw	AR,A1\n	cmpw	AL,A1", q);
		}
		break;
		}

	case 'S':		/* STASG */
		q = ophead[STASG];
		if (istnode(pnode->in.left))
			pl = pnode->in.left;
		else
		{
			pl = &resc[1];
			if (pnode->in.left->in.op == UNARY AND)
				expand(pnode, FOREFF, "	movaw	A(LL),A2\n", q);
			else
				expand(pnode, FOREFF, "	movw	AL,A2\n", q);
		}
		if (istnode(pnode->in.right))
			pr = pnode->in.right;
		else
		{
			pr = &resc[0];
			if (pnode->in.right->in.op == UNARY AND)
				expand(pnode, FOREFF, "	movaw	A(RL),A1\n", q);
			else
				expand(pnode, FOREFF, "	movw	AR,A1\n", q);
		}
		if ((i = pnode->stn.stsize / SZINT) <= STHRESH)
			while (i--)
				printf("\tmovw\t%d(%s),%d(%s)\n", i << 2,
				    rnames[pr->tn.rval], i << 2,
				    rnames[pl->tn.rval]);
		else
		{
			int lastlab = getlab();

			printf("	movw	&%d,", i);
			expand(pnode, FOREFF, "A3\n.\\L", q);
			printf("%d:\tmovw\t0(%s),0(%s)\n\taddw2\t&4,%s\n\taddw2\t&4,%s\n",
			    lastlab, rnames[pr->tn.rval], rnames[pl->tn.rval],
			    rnames[pr->tn.rval], rnames[pl->tn.rval]);
			expand(pnode,FOREFF, "\tsubw2\t&1,A3\n\tjpos\t.\\L", q);
			printf("%d\n", lastlab);
		}
		break;

	case 's':		/* STARG */
		q = ophead[STARG];
		if (istnode(pnode->in.left))
			pl = pnode->in.left;
		else
		{
			pl = &resc[0];
			if (pnode->in.left->in.op == UNARY AND)
				expand(pnode, FOREFF, "	movaw	A(LL),A1\n", q);
			else
				expand(pnode, FOREFF, "	movw	AL,A1\n", q);
		}
		printf("	addw2	&%d,%%sp\n	movaw	-%d(%%sp),",
			pnode->stn.stsize / SZCHAR, pnode->stn.stsize / SZCHAR);
		expand(pnode, FOREFF, "A2\n", q);
		if ((i = pnode->stn.stsize / SZINT) <= STHRESH)
			while (i--)
			{
				printf("	movw	%d(%s),%d(", i << 2,
				    rnames[pl->tn.rval], i << 2);
				expand(pnode, FOREFF, "A2)\n", q);
			}
		else
		{
			int lastlab = getlab();

			printf("	movw	&%d,", i);
			expand(pnode, FOREFF, "A3\n.\\L", q);
			printf("%d:\tmovw\t0(%s),0(", lastlab,
			    rnames[pl->tn.rval]);
			expand(pnode, FOREFF, "A2)\n\taddw2\t&4,", q);
			printf("%s\n\taddw2\t&4,", rnames[pl->tn.rval]);
			expand(pnode, FOREFF,
			    "A2\n\tsubw2\t&1,A3\n\tjpos\t.\\L", q);
			printf("%d\n", lastlab);
		}
		break;

	default:
		cerror("botched Z macro %c", **ppc);
		/*NOTREACHED*/
	}
}


staradr(p)				/* dereference p */
NODE *p;
{
	NODE *pp;

	pp = talloc();
	pp->tn.op = STAR;
	pp->in.left = p;
	upput(pp);
	pp->tn.op = FREE;
}

conput(p)			/* name for a known */
register NODE *p;
{
	switch(p->in.op)
	{
	case ICON:
		acon(p);
		break;
	case REG:
		fputs(rnames[p->tn.rval], stdout);
		break;
	default:
		cerror("confused node in conput");
		/*NOTREACHED*/
	}
}

insput(p)			/* we don't have any */
NODE *p;
{
	cerror("insput");
}

upput(p)			/* generate address incl. indirect, offset */
NODE *p;
{
	register NODE *r, *l, *pp;

	sideff = 0;
	if (p->in.op == STAR)
		pp =  p->in.left;
	else
		pp = p;
again:
	switch (pp->tn.op)
	{
	case NAME:
	case TEMP:
	case VAUTO:
	case VPARAM:
		if (p->in.op == STAR)
			putchar('*');
		adrput(pp);
		break;

	case REG:
		if (p->in.op == STAR)
		{
			fputs("0(", stdout);
			conput(pp);
			putchar(')');
		}
		else
			conput(pp);
		break;

	case ICON:
		if (p->in.op == STAR)
			putchar('*');
		acon(pp);
		break;

	case STAR:
		putchar('*');
		p = pp;
		pp = pp->in.left;
		goto again;

	case PLUS:
	case MINUS:
	case INCR:
	case DECR:
		if (p->in.op == STAR && pp->in.left->in.op != REG)
			putchar('*');
		if (pp->in.op == MINUS)
			putchar('-');
		acon(pp->in.right);
		putchar('(');
		conput(pp->in.left);
		putchar(')');
		break;
	
	default:
		cerror("upput: confused addressing mode matched");
		/*NOTREACHED*/
	}
}

adrput(p)		/* output address from p */
NODE *p;
{
	sideff = 0;
	while (p->in.op == FLD || p->in.op == CONV)
		p = p->in.left;
	switch (p->in.op)
	{
	case ICON:		 /* value of the constant */
		putchar('&');
	case NAME:
		acon(p);
		break;
	case REG:
		conput(p);
		break;
	case STAR:
		upput(p);
		break;
	case TEMP:
		printf("%d(%%fp)", p->tn.lval + maxboff / SZCHAR);
		break;
	case VAUTO:
		printf("%d(%%fp)", p->tn.lval);
		break;
	case VPARAM:
		printf("%d(%%ap)", p->tn.lval);
		break;
	default:
		cerror("adrput: illegal address");
		break;
	}
}




dbladrput(p)
NODE *p;
/* output address of word after p.  used for double moves */
{
	while (p->in.op == FLD || p->in.op == CONV)
		p = p->in.left;
	switch (p->in.op)
	{
	case NAME:
	    printf("%s+%ld", p->in.name, p->tn.lval + 4);
	    break;
	case REG:
	    fputs(rnames[p->tn.rval + 1],stdout);
	    break;
	case TEMP:
	    printf("%d(%%fp)", (p->tn.lval+maxboff/SZCHAR)+4);
	    break;
	case VAUTO:
	    printf("%d(%%fp)", p->tn.lval + 4);
	    break;
	case VPARAM:
	    printf("%d(%%ap)", p->tn.lval + 4);
	    break;
	default:
	    cerror("dbladrput:  illegal double address substitution");
	    /*NOTREACHED*/
	}
}




acon(p)			/* print out a constant */
register NODE *p;
{

	if (p->in.name == (char *) 0)	/* constant only */
	{
		if (p->in.op == ICON &&
		    (p->in.type == TCHAR || p->in.type == TUCHAR))
			p->tn.lval &= 0xff;
		printf("%ld", p->tn.lval);
	}
	else if (p->tn.lval == 0) 	/* name only */
		printf("%s", p->in.name);
	else 				/* name + offset */
		printf ("%s+%ld", p->in.name, p->tn.lval);
}



special(sc,p)
NODE *p;
{
	cerror("special shape used");
	return (INFINITY);
}

