#include "jcom.h"
#include "jplot.h"
#include <stdio.h>
#include <math.h>
void
cfill(s)
char *s;
{
	register int  *m, c;
	m = t1;
	while(*s != NULL){
		if(*s == 'B'){
			s++;
			if(*s >= '0' && *s <= '9'){
				graphic(TEXTURE);
				while(*s >= '0' && *s <= '9'){
					*m = 0;
					while(*s >= '0' && *s <= '9'){
						c = *s++ - '0';
						*m <<= 3;
						*m |= c;
					}
					graphic((*m>>8)&0377);
					graphic(*m&0377);
					m++;
					while(*s == ' ' || *s == ',')s++;
				}
				while(m <= &t1[15]){
					*m++ = 0;
					graphic(0);
					graphic(0);
				}
				e1->texture = t1;
				cmd_sent();
				return;
			}
		}
		while(*++s != NULL){
			if(*s == '/'){
				s++;
				break;
			}
		}
	}
}
