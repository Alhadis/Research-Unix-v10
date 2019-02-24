#include <stdio.h>
#include <ctype.h>
char this[50];
char that[50];
main()
{
	char *p, *q;
	while(fgets(this,sizeof(this),stdin) != 0) {
		for(p=this;*p && *p!='\n'; p++) ;
		*p = 0;
		p = this;
		q = that;
		if(tolower(*p)==tolower(*q))
			p++, q++;
		for( ; *p && *p==*q; p++,q++)
			continue;
		printf("%c%s",0200+0100*isupper(*this)+p-this,p);
		strcpy(that,this);
	}
}
