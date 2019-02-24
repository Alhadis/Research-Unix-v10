#include "mgr.h"
#include <pwd.h>
#include <sys/filio.h>
#include "defs.h"

extern int debug;

/*
 *  execute as a specific user
 */
asuser(rp, ap)
	Request *rp;
	Action *ap;
{
	char line[ARB];
	struct passwd *pwsearch();

	if(pwsearch(ap->arg, -1, line)==NULL) {
		ipcreject(rp->i, EACCES, "default user illegal");
		return -1;
	}
	rp->line = strdup(line);
	ipcaccept(rp->i);
	return 0;
}

/*
 *  simple authentication
 */
auth(rp, ap)
	Request *rp;
	Action *ap;
{
	struct passwd *pw;
	static char line[ARB];
	struct passwd *pwsearch();
	char *mapuser();
	char *u;

	if(debug)
		logtime("auth:\n");
	USE(ap);

	/*
	 *  do the mapping from the authentication files
	 */
	u = mapuser(rp->s->name, rp->i->machine, rp->i->user);
	if(u!=NULL) {
		if ((pw = pwsearch(u, -1, line)) != NULL
		&&   strcmp(pw->pw_name, "root") != 0) {
			ipcaccept(rp->i);
			rp->line = line;
			return 0;
		}
	}
	ipcreject(rp->i, EACCES, "authentication failure");
	return -1;
}

/*
 *  v9 authentication
 */
v9auth(rp, ap)
	Request *rp;
	Action *ap;
{
	struct passwd *pw;
	static char line[ARB];
	register char *u, *p;
	struct passwd *pwsearch();
	char *mapuser();
	char *rdline();

	if(debug)
		logtime("v9auth:\n");
	USE(ap);
	ipcaccept(rp->i);

	/*
	 *  do the mapping from the authentication files
	 */
	if(ap->arg && *ap->arg!=0)
		u = ap->arg;
	else
		u = mapuser(rp->s->name, rp->i->machine, rp->i->user);
	if(u!=NULL) {
		if ((pw = pwsearch(u, -1, line)) != NULL
		&&   pw->pw_uid != 0) {
			write(rp->i->cfd, "OK", 2);
			rp->line = line;
			return 0;
		}
	}

	/*
	 *  mapping didn't work, ask for password etc.
	 */
	for (;;) {
		write(rp->i->cfd, "NO", 2);
		if ((u = rdline(rp->i->cfd))==NULL){
			errno = EACCES; errstr = "authentication failure";
			return -1;
		}
		p = strchr(u, ',');
		if (p)
			*p++ = '\0';
		if ((pw = pwsearch(u, -1, line)) == NULL)
			continue;
		if (strcmp(crypt(p, pw->pw_passwd), pw->pw_passwd) == 0)
			break;
	}
	write(rp->i->cfd, "OK", 2);
	rp->line = strdup(line);
	return 0;
}

/* 4.2BSD inet stye authentication */
#define SNDMSG(x) write(rp->i->cfd, x, strlen(x))
inauth(rp, ap)
	Request *rp;
	Action *ap;
{
	static char line[ARB];
	struct passwd *pw;
	struct passwd *pwsearch();
	char *port;
	char *u;
	char buf[ARB];
	char *rdline();
	char *mapuser();
	char *tcptofs();

	if(debug)
		logtime("inauth:\n");
	USE(ap);

	/*
	 *  get port number for stderr and connect to it
	 */
	ipcaccept(rp->i);
	port = rdline(rp->i->cfd);
	if(port==NULL) {
		errno = EACCES; errstr = "protocol botch";
		return -1;
	}
	if(*port!='\0') {
		rp->errfd = ipcopen(ipcpath(rp->i->machine, "tcp",
					tcptofs(atoi(port))), "light");
	}

	/*
	 *  get remuser, locuser
	 */
	u = rdline(rp->i->cfd);
	if(u==NULL) {
		SNDMSG("protocol botch\n");
		errno = EACCES; errstr = "protocol botch";
		return -1;
	}
	strcpy(buf, u);
	u = rdline(rp->i->cfd);
	if(u==NULL) {
		SNDMSG("protocol botch\n");
		errno = EACCES; errstr = "protocol botch";
		return -1;
	}
	if(strcmp(buf, u)!=0) {
		SNDMSG("-l option not supported\n");
		errno = EACCES; errstr = "-l attempted";
		return -1;
	}

	/*
	 *  don't trust user name if call didn't come from root
	 */
	if(strcmp(rp->i->user, "root")!=0)
		u = rp->i->user;
	write(rp->i->cfd, "", 1);

	/* authenticate */
	if(ap->arg && *ap->arg!=0)
		u=ap->arg;
	else
		u=mapuser(rp->s->name, rp->i->machine, u);
	if (u!=NULL
	&&  (pw = pwsearch(u, -1, line)) != NULL
	&&   pw->pw_uid != 0)
		rp->line = strdup(line);
	else
		SNDMSG("cannot authenticate caller (.rhosts unsupported)\r\n");
	return 0;
}
