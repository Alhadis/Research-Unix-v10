/* a6 -- pdp-11 assembler pass 2 */
#include "as2.h"
#define BR 0400
#define JMP 0100

struct adr {
	int xval; 	/* index value */
	int xrel; 	/* index reloc. */
	struct expr *xsym; 	/* index global */
} adrbuf[2];
struct adr *adrp;

opline(op)
OP op;
{
	struct expr x;
	register int w,src,dst,rlimit,swapf,t;

	if (ISCHAR(op)) {
		if (op.v==5) {/* file name of source */
			register char *cp=argb;
			line=1;
			while ((unsigned)(t=getw())<0200) if (cp<(argb+sizeof(argb))) *cp++=t;
			*cp++='\n'; *cp++='\0'; return;
		}
		if (op.v=='<') goto opl17;
	xpr:
		x=expres(op); outw(x.typ,x.val); return;
	}
	t=op.xp->typ;
	if (t==T_REG || t==T_ESTTXT || t==T_ESTDAT || t<T_FLOPD || t>T_JXX) goto xpr;
	w=op.xp->val;
	op=readop(); adrp=adrbuf; swapf=0; rlimit= 07777;
	switch (t) {
	case T_REG:
	case T_ESTTXT:
	case T_ESTDAT:
		goto xpr;

	case T_MUL: swapf++; rlimit=01000; goto opl13;
	case T_FLOPF: swapf++;
	case T_FLOPD: rlimit=0400;
	case T_DOUBLE:
	  opl13: src=addres();
	  op2a: readop(); 	/* prime the pump */
	  op2b: dst=addres();
		if (swapf) {t=src; src=dst; dst=t;}
		if ((src<<=6)>=rlimit) error('x');
		outw(0,w|src|dst);
		{
			register struct adr *p;
			for (p=adrbuf; p<adrp; p++) {
				xsymbol=p->xsym; outw(p->xrel,p->xval);
			}
		}
		return;
	case T_SINGLE: src=0; goto op2b;

	case T_MOVF:
		rlimit=0400;
		if ((src=addres())>=4) swapf++; /* src is freg */
		else w=0174000;
		goto op2a;
	case T_JBR:
	case T_JXX:
		x=expres(op);
		if (passno==0) {
			if (0!=(t=setbr(x.val)) && w!=BR) t+=2;
			*dot+=t+2; return;
		} else {
			if (getbr()==0) goto dobranch;
			if (w!=BR) outw(T_ABS,w^0402); 	/* flip jump to .+6 */
			outw(T_ABS,JMP+037); outw(x.typ,x.val); return;
		}
	case T_SOB:
		x=expres(op); chkreg(&x); w |= x.val<<6; x=expres(readop());
		if (passno==0) {outw(0,w|x.val); return;}
		if ((x.val= *dot-x.val)<-2 || x.val>0175) {error('b'); outw(0,w); return;}
		x.val+=4; goto f1;
	case T_BRANCH:
		x=expres(op);
		if (passno!=0) {
		  dobranch:
			if ((x.val-= *dot)<-254 || x.val>256) {error('b'); outw(0,w); return;}
		  f1:
			if (x.val&1 || x.typ!= *dotrel) {error('b'); outw(0,w); return;}
			outw(0,w|(((x.val>>1)-1)&0377)); return;
		}
		outw(0,w|x.val); return;
	case T_JSR: x=expres(op); chkreg(&x); src=x.val; goto op2a;
	case T_RTS: x=expres(op); chkreg(&x); outw(0,w|x.val); return;
	case T_SYS:
		x=expres(op); if ((unsigned)x.val>=256 || x.typ>T_ABS) errora();
		outw(x.typ,w|x.val); return;
	case T_BYTE:
		for (;;) {
			x=expres(op); outb(x.typ,x.val); if (!LAST(',')) return;
			op=readop();
		}
	case T_STRING:
	  opl17:
		for (;;) {
			if ((unsigned)(t=getw())>=0600) {getw(); return;}
			outb(T_ABS,t&0377);
		}		
	case T_EVEN:
		if (*dot&1) {
			if (*dotrel!=T_BSS) outb(0,0);
			else (*dot)++;
		}
		return;

	case T_IF: x=expres(op);
	case T_ENDIF: return;

	case T_GLOBL:
		for (;;) if (!ISCHAR(op)) {
			op.xp->typ |= T_EXTERN;
			op=readop(); if (!LAST(',')) break;
			op=readop();
		} else break;
		return;

	case T__TEXT:
	case T__DATA:
	case T__BSS:
		*dot=(*dot+1)&~1;
		savdot[*dotrel-T_TEXT] = *dot;
		if (passno!=0 && t!=T__BSS) {
			fseek(txtf,tseek[t-T__TEXT],0);
			fseek(relf,rseek[t-T__TEXT],0);
		}
		*dot = savdot[t-T__TEXT];
		*dotrel = t-T__TEXT+T_TEXT;
		return;

	case T_COMM:
		if (!ISCHAR(op)) {
			readop(); x=expres(readop());
			if ((op.xp->typ&037)==T_UNDEF) {
				op.xp->typ |= T_EXTERN; op.xp->val=x.val;
			}
		}
		return;
	}
}

addres() 	/* returns 6-bit pdp-11 addressing mode */
{
	register int indir=0;
	struct expr x;

	again:
	if (LAST('(')) {
		x=expres(readop()); chkreg(&x); chkrp();
		if (!LAST('+')) {
			if (indir!=0) {/* concession for "*(r)" meaning "*0(r)" */
				adrp->xval=0; adrp->xrel=0; adrp->xsym=xsymbol; adrp++;
				return(x.val|070);
			} else return(010|x.val);
		}
		readop(); return(indir|020|x.val);
	} else if (LAST('-')) {
		readop();
		if (!LAST('(')) {
			savop=lastop; lastop.v='-';		/* fall through to end of else-if */
		} else {
			x=expres(readop()); chkreg(&x); chkrp();
			return(indir|040|x.val);
		}
	} else if (LAST('$')) {
		x=expres(readop());
		adrp->xval=x.val; adrp->xrel=x.typ; adrp->xsym=xsymbol; adrp++;
		return(indir|027);
	} else if (LAST('*')) {
		if (indir) error('*');
		indir=010; readop(); goto again;
	}
	x=expres(lastop);
	if (LAST('(')) {
		adrp->xval=x.val; adrp->xrel=x.typ; adrp->xsym=xsymbol; adrp++;
		x=expres(readop()); chkreg(&x); chkrp();
		return(indir|060|x.val);
	}
	if (x.typ==T_REG) {
		chkreg(&x); return(indir|x.val);
	} else {
		x.typ|=PCREL; x.val=x.val-*dot-4; if (adrp!=adrbuf) x.val-=2;
		adrp->xval=x.val; adrp->xrel=(unsigned short)x.typ; adrp->xsym=xsymbol; adrp++;
		return(indir|067);
	}
}

errora()
{
	error('a');
}

chkreg(p)
register struct expr *p;
{
	if (((unsigned)p->val)>7 || (p->typ!=T_ABS && ((unsigned)p->typ)<=T_BSS)) {
		errora(); p->typ=0; p->val=0;
	}
}

errore()
{
	error('e');
}

chkrp()
{
	if (!LAST(')')) {error(')'); return;}
	readop();
}

#define BRLEN 1024
char brtab[BRLEN/8];
char bit[8] = {1,2,4,8,16,32,64,128};
int brtabp;

setbr(d)
register int d;
{
	register int t;

	if (brtabp>=BRLEN) return(2);
	t=brtabp++;
	if ((d-= *dot)>0) d-=brdelt;
	if (d>=-254 && d<=256) return(0);
	brtab[t>>3] |= bit[t&7];
	return(2);
}

getbr()
{
	register int t;

	if (brtabp>=BRLEN) return(1);
	t=brtabp++;
	return(brtab[t>>3]&bit[t&7]);
}
