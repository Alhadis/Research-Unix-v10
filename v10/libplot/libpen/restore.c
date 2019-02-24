#include "pen.h"
restore(){
	if(--e1 == e0)
		fprintf(stderr,"stack underflow\n");
	fprintf(pltout,"SR %8.2f,%8.2f \n",
		e1->charsz*CH_RATIO,e1->charsz);
	if(noscale){
	fprintf(pltout,"IP %8.2f,%8.2f %8.2f,%8.2f\n",
	    	e1->p1x,e1->p1y,e1->p2x,e1->p2y);
	}
	fprintf(pltout,"SC %8.2f,%8.2f %8.2f,%8.2f\n",
	    	e1->xmin,e1->xmax,e1->ymin,e1->ymax);
	penUp();
	fprintf(pltout,"SP %d PA %8.2f,%8.2f\n",
		(e1->pen>0)?e1->pen:1,
		e1->copyx, e1->copyy);
	if(!e1->pentype)
		fprintf(pltout,"LT\n");
	else fprintf(pltout,"LT %d\n",e1->pentype);
	ckerr(1);
}
