#include <stdio.h>
#include "jcom.h"
#include "jplot.h"

void
text(s)
register char *s;
{
	register char *p;
	register int nchar;
	short more, newline, centered, right;

	newline = 0;
	while(1){
		centered =  more = right = 0;
	for(p=s; *p != '\0';p++){
		if(*p == '\\'){
			switch(*(++p)){
			case 'C':
				centered++;
				s = p+1;
				continue;
			case 'R':
				right++;
				s = p+1;
				continue;
			case 'L':
				s = p+1;
				continue;
			case 'n':
				newline++;
				*(p-1) = '\0';
				if(*(p+1) != '\0')more++;
				goto output;
			}
		}
	}
output:
	nchar = 0;
	if(centered) nchar = (p - s - 1)/2;
	else if(right)nchar = p - s - 1;
	graphic(TEXT);
	intout(nchar);
	nchar = p - s + 1;
	intout(nchar);
	while(*s)
		graphic(*s++);
	if(!more)break;
	move(e1->copyx,e1->copyy);
	nchar=newline;
	while(nchar--)
		graphic('\n');
	s = p + 1;
	}
	cmd_sent();
}
