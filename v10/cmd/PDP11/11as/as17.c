/*  a7 -- pdp-11 assembler pass 1 */
#include "as1.h"
int opfound;

struct expr
expres(op)
OP op;
{
	register int ltyp,rtyp;
	register short lval,rval;
	int opprev = '+';	/* previous operator */
	struct expr x;

	opfound=0;
	lval=0; ltyp=T_ABS;
	for (;;) {
		if (!ISCHAR(op)) {
			rtyp=op.xp->typ; rval=op.xp->val;
		} else if (op.v>='a') {
			if (op.v>=('a'+10)) {lval=0; ltyp=0;}
			else {
				rtyp=curfbr[op.v-'a'];
				if ((lval=curfb[op.v-'a'])== -1) error('f');
			}
		} else switch (op.v) {
			default:
				if (opfound==0) errore();
				x.typ=ltyp; x.val=lval;
				return(x);
			case T_ABS:
				rval=numval; rtyp=T_ABS; break;
			case '+': case '-': case '*': case '/':
			case '&': case 037: case 035: case 036:
			case '%': case '^': case '!':
				if (opprev!='+') errore();
				opprev=op.v;
				goto advanc;
			case '[':
				x=expres(readop());
				if (!LAST(']')) error(']');
				rtyp=x.typ; rval=x.val;
		}
		++opfound;
		switch (opprev) {
			case '+': ltyp=combin(0,rtyp,ltyp); lval+=rval; break;
			case '-': ltyp=combin(1,rtyp,ltyp); lval-=rval; break;
			case '*': ltyp=combin(0,rtyp,ltyp); lval*=rval; break;
			case '/': ltyp=combin(0,rtyp,ltyp); lval/=rval; break;
			case 037: ltyp=combin(0,rtyp,ltyp); lval|=rval; break;
			case '&': ltyp=combin(0,rtyp,ltyp); lval&=rval; break;
			case 035: ltyp=combin(0,rtyp,ltyp); lval<<=rval; break;
			case 036: ltyp=combin(0,rtyp,ltyp); lval>>=rval; break;
			case '%': ltyp=combin(0,rtyp,ltyp); lval%=rval; break;
			case '!': ltyp=combin(0,rtyp,ltyp); lval= ~rval; break;
			case '^': ltyp=rtyp; break;
		}
		opprev='+';
	  advanc:
		op=readop();
	}
}

combin(subtract,rtyp,ltyp)
register int rtyp,ltyp;
{
	register int t;

	t=(rtyp|ltyp)&T_EXTERN;
	rtyp&=037;
	ltyp&=037;
	if (rtyp>ltyp) {register int x=rtyp; rtyp=ltyp; ltyp=x;}
	if (rtyp!=T_UNDEF) {
		if (subtract) {
			if (rtyp!=ltyp) return(t|ltyp);
			return(T_ABS|t);
		} else return(t|ltyp);
	} else return(t);
}
