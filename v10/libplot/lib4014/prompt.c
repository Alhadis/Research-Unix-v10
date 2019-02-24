#include <stdio.h>
#include "tek.h"
prompt(s, k) 
char	*s; 
int	k; 
{
	fflush(stdout);
	esave = e1;
	e1 = e0;
	if (k) 
		move(e1->sidex + 5, e1->sidey - 30);
	else 
		move(e1->sidex + 5, e1->sidey - 100);
	putchar(PLOTOUT);
	printf(s);
	e1 = esave;
}
