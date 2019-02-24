#include "sam.h"

bcopy(s1, s2, d, dir)
	register uchar *s1, *s2;
	register uchar *d;
	int dir;
{
	register long n=s2-s1;
	extern char *memcpy();
	if(dir<0){
		if(n>0){
			register long m=(n+7)/8;
			d+=n;
			switch((int)(n&7)){
			case 0:	do{	*--d = *--s2;
			case 7:		*--d = *--s2;
			case 6:		*--d = *--s2;
			case 5:		*--d = *--s2;
			case 4:		*--d = *--s2;
			case 3:		*--d = *--s2;
			case 2:		*--d = *--s2;
			case 1:		*--d = *--s2;
				}while(--m>0);
			}
		}
	}else
		memcpy((char *)d, (char *)s1, (int)(s2-s1));
}
Posn
getnum(){
	register Posn n=0;
	register c;
	if((c=nextc())<'0' || '9'<c)	/* no number defaults to 1 */
		return 1;
	while('0'<=(c=getc()) && c<='9')
		n=n*10+c-'0';
	ungetc();
	return n;
}
skipbl(){
	register c;
	do c=getc(); while(c==' ' || c=='\t');
	if(c>=0)
		ungetc();
	return c;
}
