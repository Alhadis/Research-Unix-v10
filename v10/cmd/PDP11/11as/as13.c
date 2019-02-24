/* a3 -- pdp-11 assembler pass 1 */
#include "as1.h"
#define checkeos() (LAST('\n') || LAST(';') || LAST('\004'))

assem()
{
	OP op1,op2;

	for (;;) {
	op1=readop();
	if (!checkeos()) {
		if (ifflg) {
			if (ISCHAR(op1)) continue;
			if (op1.xp->typ==T_IF) ++ifflg;
			if (op1.xp->typ==T_ENDIF) --ifflg;
			continue;
		}
		op2=readop();
		if (LAST('=')) {
			struct expr x;
			x=expres(readop());
			if (ISCHAR(op1)) error('x');
			else if ((&op1.xp->typ)==dotrel		/* as in ".=.+2" */
			  && (x.typ&=~T_EXTERN)!=*dotrel) {
				error('.'); *dotrel=T_TEXT;
			} else {
				op1.xp->typ&=~037;
				if ((x.typ&=037)==0) x.val=0;
				op1.xp->typ |= x.typ; op1.xp->val=x.val;
			}
		} else if (LAST(':')) {
			register int t;
			if (ISCHAR(op1)) {
				if (op1.v!=T_ABS) {error('x'); continue;}
				t=fbcheck(numval);
				curfbr[t]= *dotrel; curfb[t]= *dot;
				nxtfb.c0= *dotrel; nxtfb.c1=t; nxtfb.val= *dot;
				if (1!=fwrite(&nxtfb,sizeof(nxtfb),1,fbfil)) wrterr();
			} else {
				if (op1.xp->typ&037) error('m');
				op1.xp->typ |= *dotrel; op1.xp->val = *dot;
			}
			continue;
		} else {
			savop=op2; opline(op1);
		}
	}
	if (LAST('\n')) ++line;
	else if (LAST(';')) ;
	else if (LAST('\004')) {
		if (ifflg) error('x');
		return;		/* sole exit */
	} else {
		error('x'); while (!checkeos()) readop();
	}
	}
}

fbcheck(t)
{
	if (((unsigned)t)>9) {error('f'); t=0;}
	return(t);
}
