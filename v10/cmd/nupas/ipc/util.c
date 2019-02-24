#include <stdio.h>
#include <sysexits.h>

static char is_sep[256], is_field[256];
static init = 0;

extern int errno;
extern char *errstr;

void
setfields(arg)
	char *arg;
{
	register unsigned char *s = (unsigned char *)arg;

	memset(is_sep, 0, sizeof is_sep);
	memset(is_field, 1, sizeof is_field);
	while(*s){
		is_sep[*s] = 1;
		is_field[*s++] = 0;
	}
	is_field[0] = 0;
	init = 1;
}

getfields(ss, sp, nptrs)
	char *ss;
	char **sp;
{
	register unsigned char *s = (unsigned char *)ss;
	register unsigned char **p = (unsigned char **)sp;
	register unsigned c;

	if(init == 0)
		setfields(" \t");
	for(;;){
		if(--nptrs < 0) break;
		*p++ = s;
		while(is_field[c = *s++]);
		if(c == 0) break;
		s[-1] = 0;
	}
	if(nptrs > 0)
		*p = 0;
	else if(--s >= (unsigned char *)ss)
		*s = c;
	return(p - (unsigned char **)sp);
}

getmfields(ss, sp, nptrs)
	char *ss;
	char **sp;
{
	register unsigned char *s = (unsigned char *)ss;
	register unsigned char **p = (unsigned char **)sp;
	register unsigned c;
	unsigned char *goo;

	if(init == 0)
		setfields(" \t");
	if(*s){
		while(nptrs-- > 0){
			*p++ = s;
			while(is_field[*s++]);
			goo = s-1;
			if((c = *goo) == 0)
				break;
			*goo = 0;
			while(is_sep[*s]) s++;
			if(*s == 0) break;
		}
	}
	if(nptrs > 0)	/* plenty of room */
		*p = 0;
	else if(*s)	/* no room and we found a trailing non-is_seper */
		*goo = c;
	return(p - (unsigned char **)sp);
}

/*
 * Crude version of BSD socket recv command
 */
int
recv(fd, buf, n, flags)
int fd;
char *buf;
int n, flags;
{
	if (flags != 0) {
		ipcseterror(EX_SOFTWARE, "flags not allowed", "recv");
		return(-1);
	}
	return(read(fd, buf, n));
}

/*
 * Crude version of BSD socket send command
 */
int
send(fd, buf, n, flags)
int fd;
char *buf;
int n, flags;
{
	if (flags != 0) {
		ipcseterror(EX_SOFTWARE, "flags not allowed", "send");
		return(-1);
	}
	return(write(fd, buf, n));
}
