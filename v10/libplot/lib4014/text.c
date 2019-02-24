#include <stdio.h>
#include "tek.h"
#define N 0104
#define E 0101
#define NE 0105
#define S 0110
#define W 0102
#define SW 0112
/*	arrange by incremental plotting that an initial
 *	character such as +, X, *, etc will fall
 *	right on the point, and undo it so that further
 *	labels will fall properly in place
 */
#define POINTPL	036
#define BACKSP	''
char	lbl_mv[] = {
	POINTPL,040,S,S,S,S,S,S,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,PLOTOUT,0
};
char	lbl_umv[] = {
	POINTPL,040,N,N,N,N,N,N,NE,NE,NE,NE,NE,NE,NE,NE,NE,NE,PLOTOUT,0
};
char	plotout[] = {
	PLOTOUT,0
};
text(s)
char	*s;
{
	register int	n;
	register char *p;
	int centered, right, newline, more, tweek;
	double y;

	while(1){
	n = centered = right = newline = more = tweek = 0;
	for(p=s; *p != '\0'; p++){
		if(*p == '\\'){
			switch(*(++p)){
			case 'C': centered++;
				s = p+1;
				continue;
			case 'R': right++;
				s = p+1;
				continue;
			case 'n': newline++;
				*(p-1) = '\0';
				if(*(p+1) != '\0')more++;
				goto output;
			case 'L': s=p+1;
				continue;
			}
		}
		else if( (n==0) && (*p=='+' || *p == '*' || *p == 'x' ||
			*p == 'X' || *p == '.' || *p == 'o'))tweek++;
		else n = 1;
	}
output:
	if(tweek)
		printf("%s",lbl_mv);
	else printf("%s",plotout);
	n = 0;
	if(centered) n = (p - s)/2 + 1;
	else if(right)n = p - s +1;
	if(n > 0)
		while(--n)
			putchar(BACKSP);
	*p = '\0';
	printf("%s", s);
	if(tweek)
		printf("%s",lbl_umv);
	if(newline){
		y = SCY(e1->copyy) ;
		y -= 34.;
		e1->copyy = (y - e1->bottom)/e1->scaley + e1->ymin;
	}
	move(e1->copyx, e1->copyy);
	if(!more)break;
	s = p+1;
	}
}
