#include "mgr.h"
#include <sys/param.h>
#include <sys/filio.h>
#include <sys/inio.h>
#include <pwd.h>
#include <ctype.h>
#include "defs.h"

int debug;

/*
 *  get parameters from caller
 */
parms(rp, ap)
	Request *rp;
	Action *ap;
{
	char *s;
	char *rdline();

	USE(ap);
	if ((s = rdline(rp->i->cfd)) == NULL)
		return -1;
	rp->args = strdup(s);
	return 0;
}

/*
 *  get system V parameters from caller
 */
char *
rdstring(fd)
	int fd;
{
	static char buf[ARB];
	int len;

	/* get count */
	if (read(fd, buf, 2)!=2)
		return NULL;
	len = (((unsigned char)buf[1])<<8)+((unsigned char)buf[0]);
	if(len>=ARB||len<0)
		return NULL;
	if (len>0 && read(fd, buf, len)!=len)
		return NULL;
	buf[len] = '\0';
	return buf;
}

s5parms(rp, ap)
	Request *rp;
	Action *ap;
{
#include <sys/ttyio.h>
	struct sgttyb ttyb;
	char *s;
	char *cp;

	USE(ap);

	/*
	 *  name is flag string for service
	 */
	for (cp=rp->i->name; *cp && *cp!='.'; cp++){
		switch(*cp){
		case 'v':
		case 'd':
			/* ignore an input string */
			rdstring(rp->i->cfd);
			break;
		case 'a':
			/* get the arg string */
			if ((s = rdstring(rp->i->cfd)) == NULL)
				return -1;
			rp->args = strdup(s);
			break;
		case 't':
			if(ttyld(rp, ap)!=0)
				return -1;
			ioctl(rp->i->cfd, TIOCGETP, &ttyb);
			ttyb.sg_flags &= ~ECHO;
			ioctl(rp->i->cfd, TIOCSETP, &ttyb);
			break;
		}
	}
	if(*cp=='.')
		cp++;

	/*
	 *  cp now points to field after flag field
	 */
	return 0;
}

/*
 *  get terminal type(speed, etc) from caller
 */
term(rp, ap)
	Request *rp;
	Action *ap;
{
	char pbuf[ARB];
	char *s, *sl;
	char *rdline();
	char *strchr();

	USE(ap);
	if (ap->arg!=NULL && *(ap->arg)!='\0') {
		rp->term = strdup(ap->arg);
	} else {
		if((s = rdline(rp->i->cfd)) == NULL)
			return -1;
		if(sl=strchr(s, '/'))
			*sl = '\0';
		rp->term = strdup(s);
	}
	return 0;
}

/*
 *  action routines that end in execs
 */

doconn(rp, ap)
	Request *rp;
	Action *ap;
{
	USE(ap);
	ioctl(rp->i->cfd, TCPIOHUP, 0);		/* hang-up TCP on FIN */
	login(rp, rp->line, (char *)NULL);
}

#define SNDMSG(x) write(rp->i->cfd, x, strlen(x))
doexec(rp, ap)
	Request *rp;
	Action *ap;
{
	USE(ap);
	if(rp->line==0 || *rp->line==0) {
		SNDMSG("access denied\n");
		_exit(1);
	}
	login(rp, rp->line, rp->args);
}

docmd(rp, ap)
	Request *rp;
	Action *ap;
{
	register char *p;
	char buf[ARB];
	int execit;

	/*
	 *  protect against sh security holes
	 */
	for(p=rp->args; *p; p++)
		if (strchr("\n&;^|<>(){}`", *p))
			*p = ' ';
	/*
	 *  avoid an extra process
	 */
	for(execit=1,p=ap->arg; *p; p++)
		if (strchr("=\n&;^|(){}`", *p))
			execit=0;
	if(execit)
		strcpy(buf, "exec ");
	else
		buf[0]=0;
	strcat(buf, ap->arg);
	strcat(buf, " ");
	/*
	for(p=rp->args; *p && !isspace(*p); p++)
		;
	 */
	strcat(buf, rp->args);
	login(rp, rp->line, buf);
}

dologin(rp, ap)
	Request *rp;
	Action *ap;
{
	USE(ap);
	login(rp, (char *)0, (char *)0);
}

login(rp, pw, cmd)
	Request *rp;
	char *pw, *cmd;
{
	char *args[5];
	char buf[512];
	char *cp;
	register char **ap;
	register int i;
	char **inienv;
	char **srcenv(), **termenv();

	if(debug)
		logtime("login:\n");
	errno = 0; errstr = "";
	buf[0] = '\0';
	logstatus("in", rp->i);
	ap = args;
	*ap++ = "login";
	if (pw) {
		*ap++ = "-p";
		*ap++ = pw;
		strcpy(buf, " -p ");
		for(cp=buf+4; *pw && *pw!=':';)
			*cp++ = *pw++;
		*cp = '\0';
	}
	if (cmd) {
		*ap++ = cmd;
		strcat(buf, " ");
		strcat(buf, cmd);
	}
	*ap = NULL;
	pwclose();
	logevent("login %s\n", buf);
	for (i = 0; i < NSYSFILE; i++)
		dup2(rp->i->cfd, i);
	if (rp->errfd>=0)
		dup2(rp->errfd, 2);
	if(rp->i->cfd>=NSYSFILE)
		close(rp->i->cfd);
	if(rp->errfd>=NSYSFILE)
		close(rp->errfd);
	if (rp->term)
		termenv(rp);
	inienv = srcenv(rp);
	ioctl(0, TIOCSPGRP, 0);
	execve("/etc/login", args, inienv);
	execve("/bin/login", args, inienv);
	_exit(1);
}

char **newep;	/* set up by init */
char *newenv[5];

char **
srcenv(rp)
	Request *rp;
{
	static char cbuf[ARB];
	static char nbuf[ARB];
	char param[ARB];
	char *field[10];
	char *line;
	int i;

	line="";
	strcpy(param, rp->i->param);
	setfields(" ");
	i=getfields(param, field, 10);
	for(; i; i--)
		if(strncmp(field[i-1], "PSOURCE=", 8)==0){
			line=field[i-1]+8;
			break;
		}
	sprintf(cbuf, "CSOURCE=source=%s user=%s line=%s",
		rp->i->machine, rp->i->user, line);
	*newep++ = cbuf;
	sprintf(nbuf, "CDEST=%s!%s", rp->i->myname, rp->i->name);
	*newep++ = nbuf;
	*newep = NULL;
	return (newenv);
}

char **
termenv(rp)
	Request *rp;
{
	static char buf[100];

	sprintf(buf, "TERM=%s", rp->term);
	*newep++ = buf;
	*newep = NULL;
	return (newenv);
}
