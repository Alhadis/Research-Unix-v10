#include "pen.h"
text(s)
char *s;
{
	if (*s=='\\') 
		switch(*(s+1)) {
		case 'C':
			fprintf(pltout,"LO5 ");
			s += 2;
			break;
		case 'R':
			fprintf(pltout,"LO8 ");
			s += 2;
			break;
		case '\\':
			fprintf(pltout,"LO2 ");
			s++;
			break;
		default:
			fprintf(pltout,"LO2 ");
			s += 2;
			break;
		}
	else fprintf(pltout,"LO2 ");
	fprintf(pltout,"LB%s\3\n",s);
	ckerr(0);
}
