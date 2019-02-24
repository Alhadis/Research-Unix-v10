/*
 * A reasonable interface to the system calls
 */
#include <stdio.h>
error(a, b, c, d, e)
	char *a;
{
	extern int errno;
	extern char *sys_errlist[];
	extern int sys_nerr;
	extern char *progname;

	fprintf(stderr, "%s: ", progname);
	fprintf(stderr, a, b, c, d, e);
	if(errno && errno<sys_nerr)
		fprintf(stderr, ": %s\n", sys_errlist[errno]);
	else
		fprintf(stderr, "\n");
	exit(1);
}
Read(f, a, n)
	char *a;
{
	int nr, total=0;
	while(total<n){
		nr = read(f, a+total, n-total);
		if(nr<0) error("read error");
		if(nr==0) error("premature EOF");
		total += nr;
	}
}
EOFRead(f, a, n)
	char *a;
{
	int nr, total=0;
	while(total<n){
		nr = read(f, a+total, n-total);
		if(nr<0) error("read error");
		if(nr==0){
			if(total>0) error("premature EOF");
			else return(0);
		}
		total += nr;
	}
	return(total);
}
Write(f, a, n)
	char *a;
{
	int m;
	if((m=write(f, a, n))!=n)
		error("write error, wanted %d got %d", n, m);
}
long
Lseek(f, n, w)
	long n;
{
	long p, lseek();
	if((p=lseek(f, n, w))==-1)
		error("lseek error");
	return(p);
}
Creat(f, m)
	char *f;
{
	int fd=creat(f, m);
	if(fd<0)
		error("can't create %s", f);
	return fd;
}
Open(f, m)
	char *f;
{
	int fd=open(f, m);
	if(fd<0)
		error("can't open %s", f);
	return fd;
}
char *
Malloc(n)
	unsigned n;
{
	char *malloc(), *p=malloc(n);
	if(p==0)
		error("malloc failed");
	return p;
}


paws(n)
  int n;
{
  /*  pause.   n=100 is a typical value  */
  double x, y;
  int i;
  x=1;
  for(i=0; i<n; i++){
    y=sqrt(x);
    y=log(x);
    y=exp(x);
  }
}
double
pow2 ( v )
  short v;
{
  /* this is only here because the system pow is so bad */
  double s = 1.0;
  while( v>0 ){ v--; s *= 2; }
  while( v<0 ){ v++; s /= 2; }
  return(s);
}
