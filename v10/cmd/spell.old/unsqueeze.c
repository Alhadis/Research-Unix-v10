#include <stdio.h>
main()
{
	int c = 0;
	char last[50];
	char *p = last;
	while((c=getchar())!=EOF) {
		if(!(c&0200))
			*p++ = c;
		else if(p!=last) {
			*p = 0;
			printf("%s\n",last);
			p = last+(c&~0300);
			if(c&0100)
				*last = toupper(*last);
			else
				*last = tolower(*last);

		}
	}
	*p = 0;
	printf("%s\n",last);
}
