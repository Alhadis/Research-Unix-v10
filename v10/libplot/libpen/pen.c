#include "pen.h"
pen(s)
char *s;
{
	register int i;
	i = -1;
	while(*s != NULL){
		switch(*s){
		case 'l':
			if(!strncmp(s, "long", 4))
				i=3;
			break;
		case 'd':
			if(!strncmp(s,"dotd",4))
				i=4;
			else if(!strncmp(s,"dott",4))
				i=1;
			else if(!strncmp(s,"dash",4))
				i=5;
			else if(!strncmp(s,"dd",2))
				i=6;
			break;
		case 's':
			if(!strncmp(s,"solid",5))
				i=0;
			else if(!strncmp(s,"short",5))
				i = 2;
			break;
		case 'G':
			e1->pgap = atof(s+1);
			break;
		case 'A':
			e1->pslant = atof(s+1)/RADIANS;
			break;
		}
		while(*s++ != NULL)
			if(*s == '/'){
				s++;
				break;
			}
	}
	if(i == 0)fprintf(pltout,"LT; ");
	else if(i > 0)
		fprintf(pltout,"LT%d; ",i);
	if(i >= 0)
		e1->pentype = i;
	ckerr(0);
}
