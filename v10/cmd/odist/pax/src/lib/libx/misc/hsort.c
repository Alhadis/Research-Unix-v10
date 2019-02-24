/*
 *  hsort - sort an array pointers using fn
 *
 *	fn follows strcmp(3) conventions
 *
 *   David Korn
 *   AT&T Bell Laboratories
 *
 *  derived from Bourne Shell
 */

void
hsort(argv,n,fn)
char *argv[];
int (*fn)();
{
	register int 	i, j, m;
	int  k;
	for(j=1; j<=n; j*=2);
	for(m=2*j-1; m/=2;)
	{
		k=n-m;
		for(j=0; j<k; j++)
		{
			for(i=j; i>=0; i-=m)
			{
				register char **ap;
				ap = &argv[i];
				if((*fn)(ap[m],ap[0])>=0)
					break;
				else
				{
					char *s;
					s=ap[m];
					ap[m]=ap[0];
					ap[0]=s;
				}
			}
		}
	}
}
