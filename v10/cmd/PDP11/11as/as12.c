/* a2 -- pdp-11 assembler pass 1 */
#include "as1.h"

error(t)
{
	++errflg;
	if (*curarg) {filerr(*curarg,"\n"); *curarg=0;}	/* print file name once */
	printf("%c %.4d\n",t,line);
}

putw(t)
OP t;
{
	if (ifflg==0 || (t.v=='\n'))
		if (EOF==fwrite(&t,2,1,pof)) wrterr();
}
