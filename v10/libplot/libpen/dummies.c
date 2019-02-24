#include "pen.h"
erase(){}
closepl(){
	move(e0->p1x,e0->p1y);
	color((char *)0);
	ckerr(1);
}
grade(){}
ppause(){}
cfill(s)
char *s;
{
	register char **p;
	register char *sp, *bp;
	while(*s != NULL){
		if(*s >= '0' && *s <= '8'){
			e1->brush = atoi(s);
		}
		else if(*s == 'A'){
			e1->pslant = (180. - atof(++s))/RADIANS;
		}
		else if(*s == 'G'){
			e1->pgap = atoi(++s);
		}
		else {
			if(*s == 'T')sp = s+1;
			else sp = s;
			if(*s == 'b')sp+=2;
			for(p=pens;p<&pens[4];p++){
				bp = *p;
				if(*bp == 'b')bp+=2;
				if(*bp == *sp){
					e1->brush = p-pens;
					e1->brush++;
					if(*s == 'T')e1->brush += 4;
				}
			}
		}
		while(*++s != NULL)
			if(*s == '/'){
				s++;
				break;
			}
	}
}
idle(){}
ptype(){}
