# include "stdio.h"
# include "math.h"
# include "assert.h"
# define radp  57.2957795
main()
{
/* reads ascii patch file; converts to binary and does cosine conversion. */
char line[100], *s, opat[10];
long pos;
float a[4];
double correct;
opat[0]=0;
pos=0;
correct = cos(40.0/radp);
while (gets(line))
	{
	if (strncmp(opat, line, 5))
		fprintf(stderr, "%.5s %ld\n", line, pos);
	strncpy(opat, line, 5);
	for(s=line; *s && *s!='\t'; s++)
		;
	sscanf(s, "%f %f %f %f", a+0, a+1, a+2, a+3);
	/* need to multiply longitudes by cosine of latitude */
	a[1] *= correct;
	a[3] *= correct;
	write (1, a, 4*sizeof(float));
	pos+= 4*sizeof(float);
	}
fprintf(stderr, "ENDED %ld\n", pos);
}
