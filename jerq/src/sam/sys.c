#include "sam.h"
/*
 * A reasonable interface to the system calls
 */
syserror(a)
	char *a;
{
	extern int errno;
	extern char *sys_errlist[];
	extern int sys_nerr;
	dprint("%s: ", a);
	error_s(Eio, (errno && errno<sys_nerr)? sys_errlist[errno] : "error 0");
}
Read(f, a, n)
	uchar *a;
{
	if(read(f, (char *)a, n)!=n)
		syserror("read");
	return n;
}
Write(f, a, n)
	uchar *a;
{
	int m;
	if((m=write(f, (char *)a, n))!=n)
		syserror("write");
	return m;
}
Lseek(f, n, w)
	long n;
{
	long lseek();
	if(lseek(f, n, w)==-1)
		syserror("lseek");
}
