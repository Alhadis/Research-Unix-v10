#include <stdio.h>
#include <math.h>
#include "tr.h"
color(s) 
char	*s; 
{
	register int k;
	while (*s != NULL) {
		switch (*s) {
		case 'F':
			k = 0;
			s++; 
			e1->pfont[k++] = *s++;
			if (*s != NULL && *s != '/') {
				e1->pfont[k++] = *s;
			}
			e1->pfont[k] = NULL; 
			PUTFONT;
			return(1);
		case 'P':
			e1->psize = atoi(++s); 
			PUTSIZE;
			return(1);
		case 'G': 
			e1->pgap = atoi(++s); 
			return(1);
		case 'A': 
			e1->pslant = (180. - atof(++s)) / RADIANS; 
			return(1);
		}
		while (*++s != NULL) 
			if (*s == '/') {
				s++;
				break;
			}
	}
	return(0);
}
