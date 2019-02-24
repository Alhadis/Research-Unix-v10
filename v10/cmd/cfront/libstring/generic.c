#include <stdio.h>

extern genericerror(int n, char* s)
{
	fprintf(stderr,"%s\n",s?s:"error in generic library function",n);
	abort();
	return 0;
};
