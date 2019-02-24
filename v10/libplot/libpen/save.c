#include "pen.h"
save(){
	int i;

	fprintf(pltout,"OC;\n");
	fflush(pltout);
	e1->copyx = (float) readInt(pltin);
	e1->copyy = (float) readInt(pltin);
	i = readInt(pltin);
	sscpy(e1,e1+1);
	if(++e1 > &E[8])
		fprintf(stderr,"stack overflow\n");
	ckerr(1);
}
