#include <stdio.h>
#include "jcom.h"
#include "jplot.h"

restore(){
	struct penvir *e;
	int *pt;
	e = e1;
	e1--;
	move(e1->copyx, e1->copyy);
	if(e1->pen != e->pen){
		graphic(LINETH);
		graphic(e1->pen);
		cmd_sent();
	}
	if((e->texture != e1->texture) ||
		(e->texture != t0)){
		graphic(TEXTURE);
		for(pt=e1->texture; pt<e1->texture+17;pt++){
			graphic((*pt>>8)&0377);
			graphic(*pt&0377);
		}
		cmd_sent();
	}
}
