#include "mgr.h"
#include <sys/param.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <sys/filio.h>
#include <libc.h>
#include <pwd.h>

extern int debug;

static fd_set	listenset;	/* set of fd's on which we are listening */
static Service *svchead;	/* head of services */
static Service *nsvchead;	/* new head of services */
static int	toannounce;	/* true if there's anything to announce */

/*
 *  Parse a service definition line.  The line is of the form:
 *  `service action+action+action+... source'
 */
Service *
newservice(cp)
	char *cp;
{
#	define MAXACTS 32
	char *arg;
	char *acts[MAXACTS];
	Service *sp=(Service *)malloc(sizeof(Service));
	int i, n, plus;
	Action *lap;

	if(sp==NULL) {
		logevent("out of memory parsing service\n");
		return NULL;
	}
	sp->ap = NULL;
	sp->listen = -1;
	sp->name = NULL;
	sp->next = (Service *)NULL;
	sp->accept = 0;
	sp->lasttime = 1;
	sp->from = NULL;
	sp->same = NULL;

	/* find service name */
	for(; isspace(*cp); cp++)
		;
	for(arg=cp; *arg && !isspace(*arg); arg++)
		;
	if(isspace(*arg))
		*arg++ = '\0';
	sp->name = strdup(cp);

	/* 
	 *  separate actions.  actions are separated by '+'s and
	 *  whitespace.  actions may have arguments in ()'s.
	 */
	for(; isspace(*arg); arg++)
		;
	for(plus=1, n=0; *arg && n<MAXACTS && plus; n++) {
		/*  find next action */
		acts[n] = arg;
		for(plus = 0;;arg++)
			if(*arg=='(') {
				for(;;arg++)
					if(*arg=='\\' && *(arg+1)==')')
						arg++;
					else if(*arg==')')
						break;
			} else if(*arg=='+') {
				plus = 1;
				*arg++ = '\0';
				break;
			} else if(isspace(*arg)) {
				*arg++ = '\0';
				break;
			} else if(*arg=='\0')
				break;

		/* dump whitespace */
		for(;; arg++)
			if(*arg=='+')
				plus = 1;
			else if(!isspace(*arg))
				break;
	}
	if (n <= 0) {
		logevent("service with no action `%s'\n", cp);
		freeservice(sp);
		return NULL;
	}

	/* parse actions */
	for(lap=NULL, i=0; i<n; i++) {
		if (lap==NULL)
			sp->ap = lap = newaction(acts[i]);
		else
			lap = lap->next = newaction(acts[i]);
		if (lap==NULL) {
			freeservice(sp);
			return NULL;
		}
		lap->next = NULL;
		sp->accept |= lap->accept;
	}

	/* look for source specifier */
	if(*arg){
		acts[0] = arg;
		while(*arg && !isspace(*arg))
			arg++;
		*arg = 0;
		if((sp->from=newre(acts[0]))==NULL){
			logevent("illegal reg exp in service `%s %s'\n", sp->name,
				acts[0]);
			freeservice(sp);
			return NULL;
		}
	}

	if(sp->from)
		logevent("newservice(%s for %s)\n", sp->name, acts[0]);
	else
		logevent("newservice(%s)\n", sp->name);
	return sp;
}

freeservice(sp)
	Service *sp;
{
	if(sp==NULL)
		return;
	if(sp->listen>=0) {
		logevent("denouncing %s\n", sp->name);
		close(sp->listen);
		FD_CLR(sp->listen, listenset);
	}
	if(sp->name!=NULL)
		free(sp->name);
	for(; sp->ap!=NULL; sp->ap=sp->ap->next)
		freeaction(sp->ap);
	if(sp->from)
		freere(sp->from);
	if(sp->same)
		freeservice(sp->same);
	free((char *)sp);
}

/*
 *  Add a service to the ones for which we are listening
 */
addservice(sp)
	Service *sp;
{
	Service *p;

	/* look through new list for a previous version */
	for(p=nsvchead; p; p=p->next)
		if (strcmp(sp->name, p->name)==0){
			/* string after previous versions */
			for(; p->same; p=p->same)
				;
			p->same = sp;
			return 0;
		}

	/* look for an announced version of the service */
	for(p=svchead; p; p=p->next)
		if (strcmp(sp->name, p->name)==0)
			break;

	/* inherit fd from old service */
	if (p) {
		sp->listen = p->listen;
		p->listen = -1;
	} else
		toannounce=1;

	/* add the new service */
	sp->next = nsvchead;
	nsvchead = sp;
	return 0;
}

/*
 *  Start the listening process on any services not already listening.
 */
startsvcs()
{
	Service *p, *np;

	/* denounce old services */
	for(p=svchead; p; p=np) {
		np = p->next;
		freeservice(p);
	}

	/* install new services */
	svchead = nsvchead;
	nsvchead = (Service *)NULL;

	announcesvcs();
}

/*
 *  Announce any services not already listening
 */
announcesvcs()
{
	Service *p;


	/* announce new services */
	toannounce=0;
	for(p=svchead; p; p=p->next) {
		if (p->listen>=0)
			continue;
		logevent("announcing %s\n", p->name);
		p->listen = ipccreat(p->name, "light");
		if (p->listen<0) {
			logevent("failed\n");
			toannounce=1;
			continue;
		}
		chmod(p->name, 0666);
		FD_SET(p->listen, listenset);
	}
}

/*
 *  Reset all services
 */
resetsvcs()
{
	Service *p, *np;

	/* shut down all listeners */
	logevent("resetsrvcs()\n");
	for(p=svchead; p; p=np) {
		logevent("retracting %s\n", p->name);
		np = p->next;
		freeservice(p);
	}
	svchead = (Service *)NULL;
	readfiles();
}

/*
 *  An alarm during a listen 
 */
int
dingaling()
{
	signal(SIGALRM, dingaling);
	logevent("timout in listen\n");
	alarm(5);
}

/*
 *  A Zombie exists
 */
int
sigchild()
{
}

/*
 *  Get a request and vector to the appropriate service
 */
Request *
listen()
{
	fd_set readset;
	Service *sp;
	static Request rp;
	ipcinfo *ip;
	int n;
	int (*sigalrm)();
	long lastcheck = 0;
	static char line[ARB];
	struct passwd *pwsearch();

	for(;;) {
		/*
		 *  clean up utmp and make entries to wtmp
		 */
		checkkids();

		/*
		 *  wait for 
		 *	- a child to die
		 *	- a call in
		 *	- 30 seconds to elapse
		 */
		signal(SIGCHLD, sigchild);
		readset = listenset;
		n = select(NOFILE, &readset, (fd_set *)NULL, 30*1000);
		signal(SIGCHLD, SIG_IGN);

		/*
		 *  every 2 minutes see if the configuration has
		 *  changed.  this is fairly costly so don't do it too
		 *  often.
		 */
		if(time((long *)0) - lastcheck > 2*60){
			pwsearch((char *)0, 0, line); /* prime the pump */
			if(checkfiles())
				readfiles();
			lastcheck = time((long *)0);
		}
		if(toannounce)
			announcesvcs();
		if(n<=0)
			continue;

		/*
		 *  a call has arrived
		 */
		if(debug)
			logtime("request\n");
		for(sp=svchead; sp; sp=sp->next) {
			if (sp->listen<0)
				continue;
			if (FD_ISSET(sp->listen, readset))
				break;
		}
		if (!sp) {
			logevent("listen on bad fd\n");
			resetsvcs();
			continue;
		}
		sigalrm=signal(SIGALRM, dingaling);
		alarm(5);
		if ((ip = ipclisten(sp->listen)) == NULL) {
			logevent("%s: bad listen: %s %d\n", sp->name,
				errstr, errno);
			close(sp->listen);
			FD_CLR(sp->listen, listenset);
			sp->listen = -1;
			toannounce=1;
			signal(SIGALRM, SIG_IGN);	/* close race */
			alarm(0);
			signal(SIGALRM, sigalrm);
			continue;
		}
		if(sp->ap->func == 0){
			close(ipcaccept(ip));
			logstatus("in", ip);
			readfiles();
			continue;
		}
		signal(SIGALRM, SIG_IGN);	/* close race */
		alarm(0);
		signal(SIGALRM, sigalrm);
		if (ip->machine==NULL || ip->user==NULL) {
			ipcreject(ip, EACCES, "no machine or user name");
			logstatus("in", ip);
			continue;
		}
		for(; sp; sp = sp->same){
			if(!sp->from)
				break;
			if(execre(sp->from, ip->machine, (regsubexp *)0, 0))
				break;
		}
		if(sp==0){
			ipcreject(ip, EACCES, "no such service");
			logstatus("in", ip);
			continue;
		}
		if(!(sp->accept) && ipcaccept(ip)<0) {
			logstatus("in", ip);
			continue;
		}
#ifdef PEX
		ioctl(ip->cfd, FIOANPX, 0);
#endif PEX
		return(newrequest(ip, sp));
	}
}

/*
 *  Close all service fds
 */
closesvcs()
{
	Service *p;

	toannounce=1;
	for(p=svchead; p; p=p->next)
		close(p->listen);
}
