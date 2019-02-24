#include <stdio.h>
#include "tek.h"
color(s) 
char	*s; 
{
	register char a;
	if((a=bcolor(s))!= '\0'){
		e1->ppink = a; 
		putchar(ESC); 
		putchar(e1->ppink);
	}
}
