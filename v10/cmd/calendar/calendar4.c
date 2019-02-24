#include <stdio.h>
extern char *strchr();
main()
{
	char s[100], *t;
	while(fgets(s,sizeof s,stdin)) {
		t = strchr(s, '\n');
		if(t) *t = 0;
		if(access(s,04)==0 && access(s,01)==-1)
			puts(s);
	}
}
