/*  a7 -- pdp-11 assembler pass 2 */
#include "as2.h"

#define M -1
#define X -2
char reltp2[6][6] = {
	T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF,
	T_UNDEF, M,       T_TEXT,  T_DATA,  T_BSS,   T_EXTERN,
	T_UNDEF, T_TEXT,  X,       X,       X,       X,
	T_UNDEF, T_DATA,  X,       X,       X,       X,
	T_UNDEF, T_BSS,   X,       X,       X,       X,
	T_UNDEF, T_EXTERN,X,       X,       X,       X,
};
char reltm2[6][6] = {
	T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF,
	T_UNDEF, M,       T_TEXT,  T_DATA,  T_BSS,   T_EXTERN,
	T_UNDEF, X,       T_ABS,   X,       X,       X,
	T_UNDEF, X,       X,       T_ABS,   X,       X,
	T_UNDEF, X,       X,       X,       T_ABS,   X,
	T_UNDEF, X,       X,       X,       X,       X,
};
char relte2[6][6] = {
	T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF, T_UNDEF,
	T_UNDEF, M,       X,       X,       X,       X,
	T_UNDEF, X,       X,       X,       X,       X,
	T_UNDEF, X,       X,       X,       X,       X,
	T_UNDEF, X,       X,       X,       X,       X,
	T_UNDEF, X,       X,       X,       X,       X,
};
	
struct expr
expres(op)
OP op;
{
	register int ltyp,rtyp;
	register short lval,rval;
	int opprev = '+';	/* previous operator */
	struct expr x;
	struct expr *savxsym;

	xsymbol=0;
	lval=0; ltyp=T_ABS;
	for (;;) {
		if (!ISCHAR(op)) {
			if ((rtyp=op.xp->typ)==T_UNDEF && passno) error('u');
			if (rtyp==T_EXTERN) {xsymbol=op.xp; rval=0;}
			else rval=op.xp->val;
		} else if (op.v>='a') {
			rtyp=curfb[op.v-'a']->typ;
			rval=curfb[op.v-'a']->val;
		} else switch (op.v) {
			default:
				x.typ=ltyp; x.val=lval;
				return(x);
			case T_ABS:
				rval=getw(); rtyp=T_ABS; break;
			case 2:
				rval=numval; rtyp=T_ABS; break;
			case '+': case '-': case '*': case '/':
			case '&': case 037: case 035: case 036:
			case '%': case '^': case '!':
				if (opprev!='+') errore();
				opprev=op.v;
				goto advanc;
			case '[':
				savxsym=xsymbol; x=expres(readop()); if (xsymbol==0) xsymbol=savxsym;
				if (!LAST(']')) error(']');
				rtyp=x.typ; rval=x.val;
		}
		switch (opprev) {
			case '+': ltyp=combin(reltp2,rtyp,ltyp); lval+=rval; break;
			case '-': ltyp=combin(reltm2,rtyp,ltyp); lval-=rval; break;
			case '*': ltyp=combin(relte2,rtyp,ltyp); lval*=rval; break;
			case '/': ltyp=combin(relte2,rtyp,ltyp); lval/=rval; break;
			case 037: ltyp=combin(relte2,rtyp,ltyp); lval|=rval; break;
			case '&': ltyp=combin(relte2,rtyp,ltyp); lval&=rval; break;
			case 035: ltyp=combin(relte2,rtyp,ltyp); lval<<=rval; break;
			case 036: ltyp=combin(relte2,rtyp,ltyp); lval>>=rval; break;
			case '%': ltyp=combin(relte2,rtyp,ltyp); lval%=rval; break;
			case '!': ltyp=combin(relte2,rtyp,ltyp); lval= ~rval; break;
			case '^': ltyp=rtyp; break;
		}
		opprev='+';
	  advanc:
		op=readop();
	}
}

int maxtyp;

combin(tab,rtyp,ltyp)
register int rtyp,ltyp;
char tab[6][6];
{
	register int t;

	if (passno==0) {
		t=(ltyp|rtyp)&T_EXTERN;
		rtyp&=037; ltyp&=037;
		if (rtyp>ltyp) {register int x=rtyp; rtyp=ltyp; ltyp=x;}
		if (rtyp!=T_UNDEF) {
			if (tab==reltm2) {
				if (rtyp!=ltyp) return(t|ltyp);
				return(T_ABS|t);
			} else return(t|ltyp);
		} else return(t);
	} else {
		maxtyp=0;
		if ((ltyp=tab[maprel(rtyp)][maprel(ltyp)])>=0) return(ltyp);
		if (ltyp!=M) error('r');
		return(maxtyp);
	}
}

maprel(t)
register int t;
{
	if (t==T_EXTERN) return(R_EXTERN);
	if ((t&=037)>maxtyp) maxtyp=t;
	if (t<R_EXTERN) return(t);
	return(T_ABS);
}
