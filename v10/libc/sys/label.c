/* interfaces to "secure unix" system calls, designed to
   "work" on systems with or without these calls */

#include <errno.h>
#include "label.h"		/* SHOULD BE <sys/label.h> */

extern int errno;
#ifndef ENOSYS
#define ENOSYS 39
#endif

static struct label lbot = {0, L_BITS };
static struct label lfloor = { 0, L_BITS };
static int labels = 1;

static iflabeled()
{
	if(errno==100 || errno==ENOSYS)
		return labels = errno = 0;
	else
		return -1;
}
getflab(name, lp)
char *name;
struct label *lp;
{
	register r;

	r = labels? syscall(64+30, name, lp): 0;
	if(r == -1)
		r = iflabeled();
	if(!labels)
		*lp = lbot;
	return(r);
}
fgetflab(fd, lp)
struct label *lp;
{
	register r;

	r = labels? syscall(64+31, fd, lp): 0;
	if(r == -1)
		r = iflabeled();
	if(!labels)
		*lp = lbot;
	return(r);
}
setflab(name, lp)
char *name;
struct label *lp;
{
	register r;

	r = labels? syscall(64+32, name, lp): 0;
	if(r == -1)
		r = iflabeled();
	return(r);
}
fsetflab(fd, lp)
struct label *lp;
{
	register r;

	r = labels? syscall(64+33, fd, lp): 0;
	if(r == -1)
		r = iflabeled();
	return(r);
}
flabclr(fd)
{
	if(getflab("/etc/floor", &lfloor) == -1)
		iflabeled();
	lfloor.lb_priv = lbot.lb_priv;
	fsetflab(fd, &lfloor);
}
getplab(lp, cp)
struct label *lp, *cp;
{
	register r;

	r = labels? syscall(64+34, lp, cp): 0;
	if(r == -1)
		r = iflabeled();
	if(!labels) {
		*lp = lbot;
		*cp = lbot;
	}
	return(r);
}
setplab(lp, cp)
struct label *lp, *cp;
{
	register r;

	r = labels? syscall(64+35, lp, cp): 0;
	if(r == -1)
		r = iflabeled();
	return(r);
}
nochk(fd, flag)
{
	register r;

	r = labels? syscall(64+29, fd, flag): 0;
	if(r == -1)
		r = iflabeled();
	return(r);
}
#include <sys/types.h>
#include <sys/filio.h>

#ifndef FIOGSRC
# define	FIOGSRC		(('f'<<8)|106)
# define	FIOSSRC		(('f'<<8)|107)
#endif

static char buf[100]; /* <sys/stream.h>: sizeof( (struct stdata *)->source) */

setstsrc(fd, name)
char *name;
{
	strncpy(buf, name?name:"", sizeof(buf));
	return ioctl(fd, FIOSSRC, buf);
}

set2stsrc(fd, s1, s2)
char *s1;
char *s2;
{
	strncpy(buf, s1?s1:"", sizeof(buf)-1);
	strncat(buf, s2?s2:"", sizeof(buf)-strlen(buf));
	return ioctl(fd, FIOSSRC, buf);
}

char *
getstsrc(fd) 
{
	if(ioctl(fd, FIOGSRC, buf) == -1)
		return 0;
	else
		return buf;
}
