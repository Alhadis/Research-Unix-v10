#ifndef	EPR

#ifdef	USE_STDIO
#define		PR	printf(
#define		EPR	fprintf(stderr,
#define		SPR	sprintf(
#define		WR(b,n)	fwrite(b, 1, n, stdout)
#define		FLUSH	fflush(stdout)
#else
#include <fio.h>
extern int fprint(int, char*, ...);
extern int sprint(char*, char*, ...);

#define		PR	fprint(1,
#define		EPR	fprint(2,
#define		SPR	sprint(
#define		WR(b,n)	write(1, b, (long)(n))
#define		FLUSH	Fflush(1)
#endif

#endif
