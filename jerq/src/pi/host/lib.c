#include <CC/sys/types.h>
#include <CC/sys/stat.h>
#include <sgtty.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <errno.h>
#include "lib.h"
#include <pads.pub>
#include <ctype.h>
SRCFILE("lib.c")

char *index(register char *sp, char c)
{
        do {
                if (*sp == c) return sp;
        } while (*sp++);
        return 0;
}

char *rindex(register char *sp, char c)
{
        register char *r = 0;

        do {
                if (*sp == c)
                        r = sp;
        } while (*sp++);
        return r;
}

char *basename(char *path)
{
	char *lsi = rindex(path, '/');
	return lsi ? lsi+1 : path; 
}

char *slashname(char *path)
{
	return sf("%0.*s", strlen(path)-strlen(basename(path)),  path);
}

int alldigits(char *p)
{
	if( !p ) return 0;
	while( isdigit(*p) ) ++p;
	return !*p;
}

char *Name(char *s, int i)
{
	static char buf[8][32];
	static cycle;
	char *t = buf[(cycle++,cycle%=8)];
	sprintf( t, s, i );
	return t;
}
	
char *SysErr(char *s)
{
	extern int errno, sys_nerr;
	extern char *sys_errlist[];

	if( errno>=sys_nerr || errno<1 ) return sf( "%s errno=%d", s, errno);
	return sf( "%s %s", s, sys_errlist[errno] );
}

char *strcatfmt(char *s, char *f ... )
{
	int *a = (int*)&f;
	sprintf( &s[strlen(s)], f, a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8] );
	return s;
}

int ReadOK( int fd, char *addr, int req )
{
	int got;

	got = read(fd,addr,req);
	trace( "ReadOK(%d,%d,%d) %d", fd, addr, req, got );
	return got == req;
}

int WriteOK( int fd, char *addr, int req )
{
	int got;

	got = write(fd,addr,req);
	trace( "WriteOK(%d,%d,%d) %d", fd, addr, req, got );
	return got == req;
}

long modified( int fd )
{
	struct stat buf;

	fstat( fd, &buf );
	trace( "fd=%d buf.st_mtime=%d", fd, buf.st_mtime );
	return buf.st_mtime;
}
