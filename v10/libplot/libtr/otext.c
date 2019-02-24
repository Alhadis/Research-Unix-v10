#include "tr.h"
#include <stdio.h>
text(s)
char *s;
{
	register char type = 'L';
	if(*s == '\\'){
		type = *(++s);
		s++;
	}
	printf("\\h'-0.m'\\v'.2m'"); /*shift down & left */
	switch (type) {
	case 'L':
	default:
		printf("%s", s);
		break;
	case 'C':
		printf("\\h\\(ts-\\w\\(ts%s\\(tsu/2u\\(ts%s\\h\\(ts-\\w\\(ts%s\\(tsu/2u\\(ts", s, s, s);
		break;
	case 'R':
		printf("\\h\\(ts-\\w\\(ts%s\\(tsu\\(ts%s", s, s);
		break;
	}
	move(e1->copyx, e1->copyy);
}
