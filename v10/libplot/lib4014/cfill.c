#include <stdio.h>
#include "tek.h"
cfill(s) 
char	*s; 
{  
	register char a;
	if((a=bcolor(s))!= '\0')
		e1->pbrush = bcolor(s); 
}
