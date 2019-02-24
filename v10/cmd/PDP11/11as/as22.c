/* a2 -- pdp-11 assembler pass 2 */
#include "as2.h"

struct expr *xsymbol;

outw(typ,val)
register int typ;
{
	register int pcrel;

	if (*dotrel==T_BSS) {error('x'); return;}
	if (*dot&1) {error('o'); outb(0,val); return;}
	*dot+=2;
	if (passno==0) return;
	pcrel=typ&PCREL; typ&=~PCREL;
	if (typ==T_EXTERN) {/* external references */
		outmod=0666;	/* non executable */
		typ=((xsymbol-usymtab)<<3)|4;	/* external relocation */
	} else {
		if ((typ&=~T_EXTERN)>=R_EXTERN) {
			if (typ==T_ESTTXT || typ==T_ESTDAT) error('r');
			typ=T_ABS;
		}
		if (typ>=T_TEXT && typ<=T_BSS) {
			if (pcrel==0) val+= *dotdot;
		} else if (pcrel) val-= *dotdot;
		if (--typ<0) typ=0;
	}
	typ<<=1;
	if (pcrel) typ|=1;
	putw(val,txtf); tseek[*dotrel-T_TEXT]+=2;
	putw(typ,relf); rseek[*dotrel-T_TEXT]+=2;
}

outb(typ,val)
{
	if (*dotrel==T_BSS) {error('x'); return;}
	if ((unsigned)typ>T_ABS) error('r');
	if (passno!=0) {
		if (1!=fwrite(&val,1,1,txtf)) wrterr();
		tseek[*dotrel-T_TEXT]++;
		if (!(*dot&1)) {/* relocate immediately, so we don't forget */
			val=0; if (1!=fwrite(&val,2,1,relf)) wrterr();
			rseek[*dotrel-T_TEXT]+=2;
		}
	}
	(*dot)++;
}

char argb[20];

error(t)
{
	++errflg; outmod=0666;
	fprintf(stderr,"%s%c %.4d\n",argb,t,line);
	argb[0]='\0';
}
