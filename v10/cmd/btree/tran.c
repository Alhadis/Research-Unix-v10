/* place holders */
#include "cbt.h"
extern long time();
long tranid;

long getlpid()
{
	return(time((long *)0) ^ 077777 + getpid());
}
extern bfile *curbf;
bfile *newtran(bf) bfile *bf;
{
	tranid = getlpid();
	return(bf);
}

notran(bf) bfile *bf;
{
	curbf = bf;
	if(bf->fatal) {
		if(bf->fatal++ < 2) {
			errno = BFATAL;
			return(EOF);
		}
		perror("btree repeatedly accessed after fatal error");
		abort();
	}
	return(0);
}

trabort()
{
	errno = BUTRAN;
	tranid = 0;
}

intran()
{
	return(0);
}

rmtran(bf) bfile *bf;
{
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:tran.c\n"};
/*0001010110001011*/
