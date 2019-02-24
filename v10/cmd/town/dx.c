# include "stdio.h"
# define X 57.2957795785
/* 180 over pi */
main()
{
char sa[20], sb[20], c1[20], c2[20];
double lat1, lng1, lat2, lng2;
char line[100];
while (gets(line))
	{
	sscanf(line, "%*s %*s %s %s %s %s %lf %lf %lf %lf",
		sa, c1, sb, c2, &lng1, &lat1, &lng2, &lat2);
	printf("%s %s %s %s\t%.3f %.3f %.3f %.3f\n",
		sa, c1, sb, c2, lat1*X, lng1*X, lat2*X, lng2*X);
	}
}
