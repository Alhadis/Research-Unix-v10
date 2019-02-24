/* a4 -- pdp-11 assembler pass2 */
#include "as2.h"

putw(w,fp)
FILE *fp;
{
	if (1!=fwrite(&w,2,1,fp)) wrterr();
}

wrterr()
{
	fprintf(stderr,"as: write error on output\n"); saexit();
}

OP savop;
OP lastop;

OP
readop()
{
	extern struct expr symtab[];

	if (savop.xp) {lastop=savop; savop.xp=0; return(lastop);}
	lastop.v=getw();
	if (ISCHAR(lastop)) return(lastop);
	if (lastop.v>=04000) lastop.xp=usymtab+(lastop.v-04000);
	else lastop.xp=symtab+(lastop.v-01000);
	return(lastop);
}

getw()
{
	register int t,error=0;
	if (savop.xp) {lastop=savop; savop.xp=0; return(lastop.v);}
	if (EOF==(t=getchar())) ++error;
	lastop.v=t;
	if (EOF==(t=getchar())) ++error;
	lastop.v |= t<<8;
	if (error) lastop.v=4;
	return(lastop.v);
}
