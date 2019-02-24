# include "stdio.h"
main()
{
char line[200], *s;
double lat, lng;
int x1, x2;
while (gets(line))
	{
	for(s=line; *s != '\t'; s++)
		;
	s++;
	sscanf(s, "%lf%lf", &lat, &lng);
	x1 = lat/4.;
	x2 = lng/4.;
	printf("%02d.%02d %s\n",x1,x2,line);
	}
}
