#include <sys/param.h>
#include <signal.h>
#include <sysent.h>
#include <libc.h>
#include <fio.h>
#include <ipc.h>
#include <string.h>
#include <errno.h>
#include "dbtypes.h"

/*
 *  system calls not in sysent.h
 */
extern "C" int select(int, fd_set*, fd_set*, int);
#define WNOHANG	1	/* don't hang in wait */

/*
 *  imported
 */
extern Ordered *parsefiles(char *, Ordered *);
extern "C" int detach(char *);
extern int optind;
extern char *optarg;
extern "C" int getopt(int, char *[], char *);
extern "C" long time(long *);

/*
 *  predeclared
 */
int announce(int, char *);
Ordered *parse(Set **, Ordered *);
int doset(int, char *, Ordered *, Set *);
int dovalue(int, char *, char *, Ordered *, Set *);
void dohelp(int);
void initclient();
void newclient(int);
void dropclient(int);
int clientreq(int, Ordered *, Set *);

/*
 *  global
 */
char *av0;
int debug;

/*
 *  return codes from clientreq
 */
#define OK 0
#define DOPARSE 1
#define DROPCLIENT 2

/*
 *  client data
 */
fd_set cvec;
struct client {
	int fd;
	char *bp;
} client[NOFILE];
int lastclient = -1;

main(int ac, char *av[])
{
	int i, n;
	fd_set vec;
	char errbuf[sizeof(Fbuffer)];
	int doannounce, doparse;
	Set *origin;
	Ordered *o = 0;
	char *mtpt = "ns";
	int afd = -1;
	long lastcheck = 0;

	av0 = av[0];
	chdir("/cs");

	/*
	 *  parse arguments
	 */
	while((i = getopt(ac, av, "dm:")) != -1)
		switch(i){
		case 'd':
			debug = 1;
			break;
		case 'm':
			mtpt = optarg;
			break;
	}

	/*
	 * Don't die if pipe breaks
	 */

	signal(SIGPIPE, SIG_IGN);

	/*
	 *  let go of the console
	 */
	if(!debug)
		detach(mtpt);
	Finit(2, errbuf);
	initclient();

	/*
	 *  loop forever, announcing and listening for requests.  We
	 *  reannounce whenever a problem occurs with the announcement
	 *  fd.
	 */
	for(doannounce=doparse=1;;){
		if(doannounce){
			afd = announce(afd, mtpt);
			doannounce = 0;
		}
		if(doparse || time((long *)0) - lastcheck > 8*60){
			o = parse(&origin, o);
			doparse = 0;
			lastcheck = time((long *)0);
		}

		for(; !doannounce && !doparse;){
			/*
			 *  wait for a request
			 */
			vec = cvec;
			FD_SET(afd, vec);
			switch(n=select(NOFILE, &vec, 0, 2*60*1000)){
			case 0:
				/*
				 *  check to reparse if the system is
				 *  quiescent
				 */
				doparse = 1;
				continue;
			case -1:
				doannounce = 1;	/* all fd's bad */
				continue;
			}

			/*
			 *  new client?  A <0 return means that the 
			 *  announcement has gone sour.
			 */
			if(FD_ISSET(afd, vec)){
				n--;
				newclient(afd);
			}

			/*
			 *  client request?  A <0 return means the client
			 *  has gone sour.
			 */
			for(i=0; n>0 && i<NOFILE && client[i].fd>=0; i++){
				if(FD_ISSET(client[i].fd, vec)){
					n--;
					switch(clientreq(i, o, origin)){
					case DROPCLIENT:
						dropclient(i);
						break;
					case DOPARSE:
						doparse = 1;
						break;
					}
				}
			}
		}
	}
}

/*
 *  announce a service
 */
announce(int afd, char *mtpt)
{
	if(afd>=0)
		close(afd);
	sync();
	for(afd=-1; afd<0; ){
		afd = ipccreat(mtpt, "");
		if(afd<0){
			logconsole("%s: can't announce (%s)\n",av0,errstr);
			sleep(10);
		}
	}
	chmod(mtpt, 0666);
	logconsole("%s: announced as %s\n", av0, mtpt);
	return afd;
}

/*
 *  parse database and return a skip list	
 */
Ordered *
parse(Set **origin, Ordered *o)
{
	/*
	 *  parse the database
	 */
	o = parsefiles("ns.db", o);
	
	/*
	 *  get the origin tuples
	 */
	if(*origin)
		delete *origin;
	*origin = lookup("local,origin", o);
	return o;
}

/*
 *  set all clients to off
 */
void
initclient()
{
	int i;

	for(i=0; i<NOFILE; i++)
		client[i].fd = -1;
}

void
ding(int x)
{
	signal(SIGALRM, ding);
	alarm(20);
}

/*
 *  get a request for a new client
 */
void
newclient(int fd)
{
	int cfd;
	int i;
	ipcinfo *ip;

	/*
	 *  since we're single stream, don't let listening for
	 *  a call take forever.  The alarm will abort any reads
	 *  in ipclisten and ipcaccept.
	 */
	signal(SIGALRM, ding);
	alarm(20);
	ip = ipclisten(fd);
	if(ip==0)
		return;
	cfd = ipcaccept(ip);
	alarm(0);
	if(cfd<0)
		return;
	for(i=0; i<NOFILE; i++)
		if(client[i].fd<0){
			client[i].fd = cfd;
			FD_SET(cfd, cvec);
			if(i>lastclient)
				lastclient = i;
			if(!client[i].bp)
				client[i].bp = malloc(sizeof(Fbuffer));
			Finit(cfd, client[i].bp);
			return;
		}
	close(cfd);
}

/*
 *  drop a client
 */
void
dropclient(int c)
{
	/*
	 *  close off this client
	 */
	FD_CLR(client[c].fd, cvec);
	close(client[c].fd);
	client[c].fd = -1;

	/*
	 *  move last client to this spot, lastclient may equal c
	 */
	client[c] = client[lastclient];
	client[lastclient].fd = -1;
	lastclient--;
}

/*
 *  service a client request
 */
int
clientreq(int c, Ordered *o, Set *origin)
{
	char buf[512];
	int n;
	char *fields[3];
	int fd = client[c].fd;

	/*
	 *  read a line and split command and arguments
	 */
	if((n=read(fd, buf, sizeof(buf)-1))<=0)
		return DROPCLIENT;
	buf[n] = '\0';
	setfields(" \t\n");
	n = getmfields(buf, fields, 2);
	if(n<1) {
		fprint(fd, "ILL null command\n");
		return OK;
	}

	/*
	 *  act on command
	 */
	if(fstrcmp("set", fields[0])==0){
		if(n<2)
			fprint(fd, "ILL no search key\n");
		else
			return doset(fd, fields[1], o, origin);
	} else if(fstrcmp("value", fields[0])==0) {
		if(n<2) {
			fprint(fd, "ILL no value types, no search key\n");
		} else {
			n = getmfields(fields[1], fields, 2);
			if(n<2)
				fprint(fd, "ILL no search key\n");
			else
				dovalue(fd, fields[0], fields[1], o, origin);
		}
	} else if(fstrcmp("help", fields[0])==0) {
		dohelp(fd);
	} else if(fstrcmp("quit", fields[0])==0) {
		return DROPCLIENT;
	} else if(fstrcmp("reset", fields[0])==0) {
		Fprint(fd, "OK\n");
		Fprint(fd, "DONE\n");
		Fflush(fd);
		return DOPARSE;
	} else
		fprint(fd, "ILL\n");
	return OK;
}

/*
 *  Return a `set' of tuples matching allattributes in the request.  The
 *  request is "set value[,type] value[,type] ..."
 */
int
doset(int fd, char *key, Ordered *o, Set *origin)
{
	Set *s;
	int status;

	s = lookup(key, o);
	if (Fprint(fd, "OK\n") < 0) {
		if (s) delete s;
		return DROPCLIENT;
	}
	if(s){
		if(origin)
			s->sort(origin->first);
		status = s->print(fd);
		delete s;
		if (status < 0)
			return DROPCLIENT;
	}
	if (Fprint(fd, "DONE\n") < 0)
		return DROPCLIENT;
	if (Fflush(fd) < 0)
		return DROPCLIENT;
	return 0;
}

/*
 *  Return a single value.  The value is of one of the types listed in
 *  the first argument of the request.  The value comes from a tuple
 *  matching all attributes in the request.  The request is
 *  "value type1|type2|type3|... value[,type] value[,type] ..."
 */
int
dovalue(int fd, char *typelist, char *key, Ordered *o, Set *origin)
{
	int n;
#define MAXTYPES 10
	char *types[MAXTYPES+1];
	Set *s;

	setfields("|");
	n = getmfields(typelist, types, MAXTYPES);
	if(n<=0) {
		fprint(fd, "ILL bad types\n");
		return -1;
	}
	types[MAXTYPES] = 0;

	s = lookup(key, o);
	Fprint(fd, "OK\n");
	if(s){
		if(origin)
			s->sort(origin->first);
		s->printvalue(fd, types);
		delete s;
	}
	Fprint(fd, "DONE\n");
	Fflush(fd);
	return 0;
}

/*
 *  return a usage menu
 */
void
dohelp(int fd)
{
	fprint(fd, "OK\n");
	fprint(fd, "\tset value[,type] value[,type] ...\n");
	fprint(fd, "\tvalue [type|type|]type value[,type] value[,type] ...\n");
	fprint(fd, "\thelp\n");
	fprint(fd, "\tquit\n");
	fprint(fd, "\treset\n");
	fprint(fd, "DONE\n");
}
