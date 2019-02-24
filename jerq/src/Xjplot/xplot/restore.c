#include <stdio.h>
#include "jcom.h"
#include "jplot.h"

restore(){
	struct penvir *e;
	int *t;
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
		for(t=e1->texture; t<e1->texture+16;t++){
			graphic((*t>>8)&0377);
			graphic(*t&0377);
		}
		cmd_sent();
	}
}
