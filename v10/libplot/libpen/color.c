#include "pen.h"
color(s)
char *s;
{
	register float f;
	register char **p;
	char *sp, *bp;
	if(s == (char *)0) {
		fprintf(pltout,"SP0\n");
		return;
	}
	while(*s != NULL){
		if(*s >= '0' && *s <= '9'){
			e1->pen = atoi(&s[1]);
			penUp();
			fprintf(pltout,"SP %d ",(e1->pen>0)?e1->pen:1);
		}
		else if(*s == 'S'){
			s++;
			f = atof(s);
			e1->charsz = f;
			fprintf(pltout,"SR %8.2f,%8.2f ",
			    ((f * CH_RATIO)),(f));
		}
		else if(*s == 'P'){
			s++;
			continue;
		}
		else if(*s == 'A'){
			e1->pslant = atof(++s)/RADIANS;
		}
		else if(*s == 'G'){
			e1->pgap = atoi(++s);
		}
		else if(*s == 's'){
			/* scribe or scratch pen */
			s++;
			e1->pen = 8;
			penUp();
			fprintf(pltout, "SP 8\nVS 5,8\nFS 8,8\n");
		}
		else {
			if(*s == 'T')sp=s+1;
			else sp = s;
			if(*sp == 'b')sp+=2;
			for(p=pens; p<&pens[4];p++){
				bp = *p;
				if(*bp == 'b')bp+=2;
				if(*sp == *bp){
					e1->pen = p - pens;
					e1->pen++;
					if(*s == 'T')e1->pen += 4;
					penUp();
					fprintf(pltout,"SP %d ",e1->pen);
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
