/* a3 -- pdp-11 assembler pass 2 */
#include "as2.h"
#define checkeos() (LAST('\n') || LAST(';') || LAST('\004'))

int brdelt;
struct expr *curfb[20];

assem()
{
	OP op1,op2;
	struct expr x;

	for (;;) {
	op1=readop();
	if (op1.v==5 || op1.v=='<') {
		opline(op1); goto dotmax;
	} else if (!checkeos()) {
		if (op1.v==T_ABS) {op1.v=2; numval=getw();}
		op2=readop();
		if (LAST('=')) {
			x=expres(readop());
			if ((&op1.xp->typ)==dotrel) {		/* as in ".=.+2" */
				if ((x.typ&=~T_EXTERN)!=*dotrel) error('.');
				else if (x.typ==T_BSS) {*dot=x.val; goto dotmax;}
				else {
					register short t = x.val-*dot;
					if (t<0) error('.');
					else {while (--t>=0) outb(T_ABS,0); goto dotmax;}
				}
			} else {
				if (x.typ==T_EXTERN) error('r');
				op1.xp->typ&=~037;
				if ((x.typ&=037)==0) x.val=0;
				op1.xp->typ |= x.typ; op1.xp->val=x.val;
			}
		} else if (LAST(':')) {
			register int t;
			if (ISCHAR(op1)) {
				if (op1.v!=2) {error('x'); continue;}
				t=fbadv(numval);
				curfb[t]->typ= (curfb[t]->typ&~037) | *dotrel;
				brdelt=curfb[t]->val-*dot;
				curfb[t]->val= *dot;
			} else {
				if (passno==0) {
					if ((t=op1.xp->typ&037)!=0 && t!=T_ESTTXT && t!=T_ESTDAT) error('m');
					op1.xp->typ=(op1.xp->typ&~037)|*dotrel;
					brdelt=op1.xp->val-*dot;
					op1.xp->val= *dot;
				} else if (*dot!=op1.xp->val) error('p');
			}
			continue;
		} else {
			register short *psiz;
			savop=op2; opline(op1);
		  dotmax:
			psiz=(&hdr.txtsiz)+(*dotrel-T_TEXT);
			if (passno==0 && *dot>*psiz) *psiz= *dot;
			}
		}
	if (LAST('\n')) ++line;
	else if (LAST('\004')) return;	/* sole exit */
	}
}

fbadv(t)
{
	register struct expr *p;
	extern struct expr *fbbuf;

	if (curfb[t]=p=curfb[t+10]) p++;
	else p=fbbuf;
	for (;;) {
		if ((p->typ>>8)==t || p->typ<0) break;
		p++;
	}
	curfb[t+10]=p;
	return(t);
}
