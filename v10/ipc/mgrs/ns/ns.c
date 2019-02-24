#include <sys/param.h>
#include <signal.h>
#include <sysent.h>
#include <libc.h>
#include <fio.h>
#include <ipc.h>
#include <string.h>
#include <errno.h>
#include <utsname.h>
#include <pwd.h>
#include "dbtypes.h"

/*
 *  system calls not in sysent.h
 */
extern "C" int select(int, fd_set*, fd_set*, int);
#define WNOHANG	1	/* don't hang in wait */

/*
 *  imported
 */
extern "C" int detach(char *);
extern int optind;
extern char *optarg;
extern "C" int getopt(int, char *[], char *);
extern "C" long time(long *);
extern "C" int uname(utsname *);

/*
 *  predeclared
 */
int announce(int, char *);
int doset(int, char *, int *);
int dovalue(int, char *, char *, int *);
void dohelp(int);
void initclient();
void newclient(int);
void dropclient(int);
int clientreq(int);
Db *getdb(char *, int *);
void basefiles(char *, char *, char *, char **, char **);

/*
 *  global
 */
char *av0;
int debug;
char *locsysname = 0;

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
	char *machine;		/* Client's machine from ipcinfo (0 if local) */
	char *user;		/* Client's login from ipcinfo */
	int dbhint;		/* Hint for finding the needed user db */
} client[NOFILE];
int lastclient = -1;

main(int ac, char *av[])
{
	int i, n;
	fd_set vec;
	char errbuf[sizeof(Fbuffer)];
	int doannounce, doparse;
	char *mtpt = "ns";
	int afd = -1;
	long lastcheck = 0;
	char *dbloadfile;
	char *sysdbdir;
	char *sysdbfile;
	int  dummyhint;
	utsname utsn;
	Db *sysdb;
	int noparse = 0;

	av0 = av[0];
	chdir("/cs");
	dbloadfile = "/usr/ipc/lib/ns.db";

	/*
	 *  parse arguments
	 */
	while((i = getopt(ac, av, "dnm:f:")) != -1)
		switch(i){
		case 'd':
			debug = 1;
			break;
		case 'm':
			mtpt = optarg;
			break;
		case 'f':
			dbloadfile = optarg;
			break;
		case 'n':
			noparse = 1;
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

	/*
	 * Find out who we are and make the system name global
	 */
	if (!uname(&utsn))
		locsysname = utsn.sysname;

	Finit(2, errbuf);
	initclient();

	/*
	 * Initialize system database and remember root system database
	 * file in sysdb structure
	 */

	basefiles("/usr/ipc/lib", "ns.db", dbloadfile, &sysdbdir, &sysdbfile);
	sysdb = newdb((int *) 0, sysdbdir, sysdbfile, locsysname, &dummyhint);

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
			if (checkfiles(sysdb)) 
				parsefiles(sysdb);
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
				if(noparse == 0)
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
					switch(clientreq(i)){
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
 * Break infile into base directory and file in base directory.  Use
 * default directory for relative infile.  Use default directory and
 * default file if there's no infile.
 */
void
basefiles(char *defdir, char *deffile, char *infile, char **basedir, char **file)
{
	char	*fileptr;

	if (fileptr= strrchr(infile,'/')) {
		*fileptr = 0;
		*file = strdup(fileptr + 1);
		if (fileptr == infile)
			*basedir = (char *) 0;
		else if (*infile == '/')
			*basedir = strdup(infile);
		else
			*basedir = strdup(path(infile,defdir));
		*fileptr = '/';
	} else if (infile && *infile) {
		*file = strdup(infile);
		*basedir = strdup(defdir);
	} else {
		*file = strdup(deffile);
		*basedir = strdup(defdir);
	}
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
			client[i].machine = 0;
			if (ip->machine && *ip->machine)
				client[i].machine = strdup(ip->machine);
			client[i].user = strdup(ip->user);
			client[i].dbhint = -1;		/* no hint yet */
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
	if (client[c].machine)
		free(client[c].machine);
	if (client[c].user)
		free(client[c].user);

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
clientreq(int c)
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
			return doset(fd, fields[1], &client[c].dbhint);
	} else if(fstrcmp("value", fields[0])==0) {
		if(n<2) {
			fprint(fd, "ILL no value types, no search key\n");
		} else {
			n = getmfields(fields[1], fields, 2);
			if(n<2)
				fprint(fd, "ILL no search key\n");
			else
				dovalue(fd,fields[0],fields[1],
					&client[c].dbhint);
		}
	} else if(fstrcmp("help", fields[0])==0) {
		dohelp(fd);
	} else if(fstrcmp("quit", fields[0])==0) {
		return DROPCLIENT;
	} else if(fstrcmp("reset", fields[0])==0) {
		Fprint(fd, "OK\n");
		Fprint(fd, "DONE\n");
		Fflush(fd);
		logevent("reset by %s!%s\n",
			(client[c].machine? client[c].machine:locsysname),
			client[c].user);
		releasedbs();
		return DOPARSE;
	} else
		fprint(fd, "ILL\n");
	return OK;
}

/*
 *  Return a `set' of tuples matching allattributes in the request.  The
 *  request is "set value[,type] value[,type] ..."  The first valid attribute in
 *  the form: "system!login,db" is interpreted as the db to be searched.
 *  A request with no db attribute is resolved in the system db.
 */
int
doset(int fd, char *key, int *hint)
{
	Set *s = (Set *) 0;
	int status;
	Db *d;

	d = getdb(key,hint);
	key = key + strspn(key, " \t\n");

	if (d)
		s = lookup(key, d->o);

	if (Fprint(fd, "OK\n") < 0) {
		if (s) delete s;
		return DROPCLIENT;
	}
	if(s){
		if(d->origin)
			s->sort(d->origin->first);
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
 *  The first valid attribute in the form: "system!login,db" is 
 *  interpreted as the db to be searched.  A request with no db 
 *  attribute is resolved in the system db.
 */
int
dovalue(int fd, char *typelist, char *key, int *hint)
{
	int n;
#define MAXTYPES 10
	char *types[MAXTYPES+1];
	Set *s;
	Db *d;

	d = getdb(key,hint);
	setfields("|");
	n = getmfields(typelist, types, MAXTYPES);
	if(n<=0) {
		fprint(fd, "ILL bad types\n");
		return -1;
	}
	types[MAXTYPES] = 0;

	if (d)
		s = lookup(key, d->o);
	Fprint(fd, "OK\n");
	if(s){
		if(d->origin)
			s->sort(d->origin->first);
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

/*
 * get the database to be used in resolving the query qstr.  Qstr is modified
 * to remove the db identifier.  The db identifier can have any of the following
 * forms:
 *        ,db                 :  system database
 *        system,db           :  system database if system == locsysname
 *        !user,db            :  database in .nsrc file of login user
 *        system!user,db      :  same as !user,db if system == locsysname
 *        (none)              :  system database if no db attribute
 *
 * The first valid db attribute in qstr is used as the db identifier.  If no
 * db attribute is found, the system database is used.  If only invalid attributes
 * are found, no database is used, i.e. (Db *) 0 is returned.
 */
Db *
getdb(char *qstr, int *hint)
{
	char		*commaloc;
	char		*dbname;
	char		*uname;
	char		*cp;
	struct passwd 	*pw;
	char		failurepending = 0;
	char		dbnamebuf[MAXDBNAME+1];
	Db		*d;

	for (; commaloc = strchr(qstr, ','); qstr = commaloc + 1) {
		if ((commaloc[1] == 'd') && (commaloc[2] == 'b') &&
		  ((commaloc[3]=='\n')||(commaloc[3]=='\t')||(commaloc[3]==' '))) {
			*commaloc = 0;
			for (dbname = commaloc - 1; dbname >= qstr; dbname--) {
				if ((*dbname == '\t') || (*dbname == ' ')) {
					dbname++;
					break;
				}
			}
			if (dbname < qstr)
				dbname++;
			if (dbname == commaloc) {
				commaloc[0] = commaloc[1] = commaloc[2] = ' ';
				break;	/* System wildcard: ",db" */
			}
			uname = strchr(dbname,'!');
			if (uname != dbname) {
				if (uname)
					*uname = 0;
				if (fstrcmp(dbname, locsysname)) {
					if (uname) *uname = '!';
					*commaloc = ',';
					failurepending = 1;
					continue;	/* System name differs */
				}
				if (!uname) {
					for (cp = dbname; cp < commaloc + 3; cp++)
						*cp = ' ';
					break;	/* Sys name matches: "system,db" */
				}
				*uname = '!';
			}
			if (pw = getpwnam(uname+1)) {
				if (d = finddb(&pw->pw_uid, hint)) {
					if (checkfiles(d))
						parsefiles(d);
				} else {
					strncpy(dbnamebuf,locsysname,MAXDBNAME);
					strncat(dbnamebuf,uname,MAXDBNAME);
					d = newdb(&pw->pw_uid, pw->pw_dir, 
						".nsrc", dbnamebuf, hint);
					parsefiles(d);
				}
				for (cp = dbname; cp < commaloc + 3; cp++)
					*cp = ' ';
				return d;  /* User name matches:             */
					   /* "system!user,db" or "!user,db" */
			}
			*commaloc = ',';
			failurepending = 1;
			continue;			/* User name differs */
		}
		qstr = commaloc + 1;
	}
	if (failurepending) 
		return (Db *) 0;			/* No db field matched 	 */

	return finddb((int *)0, hint);			/* System name default */
}
