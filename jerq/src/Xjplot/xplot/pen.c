#include "jcom.h"
#include "jplot.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
void
pen(s)
char *s;
{
	register int di;
	while(*s != NULL){
		if(*s == 'B'){
			s++;
			if(isdigit(*s)){
				graphic(LINETH);
				di = atoi(s);
				graphic(di);
				e1->pen = di;
				cmd_sent();
				break;
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
