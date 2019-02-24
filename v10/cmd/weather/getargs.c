# include "ctype.h"
getargs(s, arps)
	char *s, *arps[];
{
	int i, c;
i = 0;
while (isspace(*s))
	s++;
while (1)
	{
	arps[i++]=s;
	while ( isalnum(c= *s) ||
		(c==',' && !isspace(s[1])) ||
		c=='-' || c=='=' || c=='%')
			s++;
	if (*s==0) break;
	*s++ =0;
	while ((c= *s) && !isalnum(c) && c != '.' && c !='%' && c != '=' && c != '-') s++;
	if (*s==0)break;
	}
return(i);
}
