#include <stdio.h>
#include "tr.h"
/*	arrange by incremental plotting that an initial
 *	character such as +, X, *, etc will fall
 *	right on the point, and undo it so that further
 *	labels will fall properly in place
 */
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
		printf("\\h'-0.m'\\v'.2m'"); /*shift down & left */
	*p = '\0';
	if(right)
		printf("\\h\\(ts-\\w\\(ts%s\\(tsu\\(ts%s", s, s);
	else if(centered)
		printf("\\h\\(ts-\\w\\(ts%s\\(tsu/2u\\(ts%s\\h\\(ts-\\w\\(ts%s\\(tsu/2u\\(ts", s, s, s);
	else
		printf("%s", s);
	if(newline){
		y = SCY(e1->copyy) ;
		y -= e1->psize * linespace;
		e1->copyy = (y - e1->bottom)/e1->scaley + e1->ymin;
	}
	move(e1->copyx, e1->copyy);
	if(!more)break;
	s = p+1;
	}
}
