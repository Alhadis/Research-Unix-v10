# include "stdio.h"
# include "math.h"
# include "assert.h"
# define radp  57.2957795
main()
{
/* reads ascii patch file; converts to binary and does cosine conversion. */
char line[100], *s, opat[10];
long pos;
int k;
float a[2];
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
	sscanf(s, "%lf %lf", a+0, a+1);
	/* need to multiply longitudes by cosine of latitude */
	a[1] = a[1]*correct;
	write (1, a, 2*sizeof(float));
	for(s=line; *s; s++)
		;
	s -= 3;
	write (1, s, 1);
	for(s=line; *s && *s!=','; s++)
		;
	*s=0;
	k = s-(line+6);
	if (k<0) k=0;
	write (1, line+6, k);
	write (1, "\n", 1);
	pos+= 2*sizeof(float);
	pos += k+2;
	}
fprintf(stderr, "ENDED %ld\n", pos);
}
