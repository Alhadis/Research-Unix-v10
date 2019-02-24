/* a6 -- pdp-11 assembler pass 1 */
#include "as1.h"

opline(op)
OP op;
{
	struct expr x;
	register int t,len;

	if (ISCHAR(op)) {
		if (op.v=='<') goto opl17;
	xpr:
		expres(op); *dot+=2; return;
	}
	t=op.xp->typ;
	if (t==T_REG || t<T_FLOPD || t>T_JXX) goto xpr;
	op=readop();
	switch (t) {
	case T_REG:
	case T_ESTTXT:
	case T_ESTDAT:
		goto xpr;

	case T_JBR: len=4; goto jxx;
	case T_JXX: len=6;
	  jxx:
		x=expres(op);
		if (*dotrel==x.typ && ((t=x.val - *dot)<0 && t>=-0376)) len=2;
		*dot+=len;
		return;

	case T_MUL:
	case T_MOVF:
	case T_DOUBLE:
	case T_FLOPD:
	case T_FLOPF:
	case T_JSR:
		addres(); if (!LAST(',')) {errora(); return;}
		op=readop();

	case T_SINGLE:
		addres(); *dot+=2; return;

	case T_SOB:
		expres(op); if (!LAST(',')) errora();
		op=readop();

	case T_BRANCH:
	case T_RTS:
	case T_SYS:
		expres(op); *dot+=2; return;

	case T_BYTE:
		for (;;) {
			expres(op); (*dot)++; if (!LAST(',')) break;
			op=readop();
		}
		return;

	case T_STRING:
	opl17:
		*dot+=numval; readop(); return;

	case T_EVEN:
		(*dot)++; *dot&=~1; return;

	case T_IF:
		x=expres(op); if (x.typ!=T_ABS) error('U');
		if (x.val==0) ifflg++;
	case T_ENDIF:
		return;

	case T_GLOBL:
		for (;;) {
			if (ISCHAR(op)) break;
			op.xp->typ |= T_EXTERN;
			op=readop(); if (!LAST(',')) break;
			op=readop();
		}
		return;

	case T__TEXT:
	case T__DATA:
	case T__BSS:
		savdot[*dotrel-T_TEXT] = *dot;
		*dot = savdot[t-T__TEXT];
		*dotrel = t-T__TEXT+T_TEXT;
		return;

	case T_COMM:
		if (ISCHAR(op)) {error('x'); return;}
		op.xp->typ |= T_EXTERN;
		op=readop();
		if (!LAST(',')) {error('x'); return;}
		expres(readop());
		return;
	}
}

addres()
{
	register int t;
	struct expr x;

	if (LAST('(')) {
		x=expres(readop()); chkreg(&x); chkrp();
		if (!LAST('+')) return(2);
		readop();
		return(0);
	} else if (LAST('-')) {
		readop();
		if (!LAST('(')) {
			savop=lastop; lastop.v='-';		/* fall through to end of else-if */
		} else {
			x=expres(readop()); chkreg(&x); chkrp();
			return(0);
		}
	} else if (LAST('$')) {
		expres(readop()); *dot+=2; return(0);
	} else if (LAST('*')) {
		readop(); if (LAST('*')) error('*');
		*dot+=(t=addres());
		return(t);
	}
	x=expres(lastop);
	if (LAST('(')) {
		x=expres(readop()); chkreg(&x); chkrp();
		*dot+=2; return(0);
	}
	if (x.typ==T_REG) {
		chkreg(&x); return(0);
	}
	*dot+=2;
	return(0);
}

errora()
{
	error('a');
}

chkreg(p)
register struct expr *p;
{
	if (((unsigned)p->val)>7 || (p->typ!=T_ABS && ((unsigned)p->typ)<=T_BSS))
		errora();
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
