#include "authmgr.h"

char admins[128];
int max_failures = MAX_FAILURES;
struct disallow* badlist;
int pushed_ld;
int gnotflg = 0;		/* set if we shouldn't ask for a new number */
int dconflg = 0;		/* set if real dest is dcon/mesgdcon */
int usepasswd = 0;		/* set if we should ignore the key file */

char dialbuf[128];
char* dialstring;		/* dialstring from CSOURCE params */
char physource[128];		/* the switch.mod.chan stuff from CSOURCE */
char srcid[128]="";		/* the first field in CSOURCE */
char username[16]="";		/* the user's login name from CSOURCE */
char eusername[30]="";		/* effective login name */
char nettype[16];		/* shouldn't get bigger than this */
char options[16] ;
char redialbuf[128] ;
char defsvc[16]="";		/* default service */
FILE *log=0;			/* log file */

struct secmap *secidlist;	/* list of security ID mappings */

#define MLIST_SIZE 10
regsubexp mlist[MLIST_SIZE];	/* a match list for regexec */

char* SYNTAX = "authmgr: Syntax error at line %d of control file\n";
#define DCON_CHALLENGE "CH"	/* the dcon challenge response */

int init(); 		/*forward*/
extern struct keyinfo* getkeyinfo();
char *makesourceid();	/*forward*/
char *getenv();
char *getpass();

/*
 * security - an authentication server
 *
 * For a given user, this locates that user's key in the
 * key database, challenges the user to encrypt some data,
 * and compares that user's result with the result of
 * encrypting the data with the user's real key.  If the
 * results are the same, the user is authenticated, otherwise,
 * no.
 */
main(argc, argv)
	int argc;
	char* argv[];
{
	int i, rc;
	char buf[128];
	char logname[30], logtries[128];
	char *bp, *dp;
	struct keyinfo *kp;
	unsigned data, l, r;
	ipcinfo	info;
	struct tm *tm = localtime(time((long *) 0));
	long now = tm->tm_mday + 100*(tm->tm_mon) + 10000*(tm->tm_yday);

	if(rc=init(argc, argv) != 0) {
	    puts("REFUSED");
	    exit(rc);
	}

	
	/* main protocol section */
	for(i=0; i<max_failures; i++) {
		if(dconflg || gnotflg) {
			printf("%s", DCON_CHALLENGE);
			if(safegets(buf, sizeof(buf)) == NULL)
				exit(0);
			logname[8] = '\0';
			strncpy(logname, buf, 8);
			kp = getkeyinfo(logname, usepasswd);
			bp = (*kp->kt->chal)(kp);
			if(bp)
				printf("%s\n", bp);	/* send the challenge */
			else
				putchar('\n');
			if(safegets(buf, sizeof(buf)) == NULL)
				exit(0);
		} else {
			if(*eusername != '\0') {
				strcpy(logname, eusername);
				*eusername = '\0';
				printf("login: %s\n", logname);
			} else {
				printf("login: ");
				if(safegets(logname, sizeof(logname)) == NULL || *logname == '\0')
					exit(0);
			}
			kp = getkeyinfo(logname, usepasswd);
			bp = (*kp->kt->chal)(kp);
			if(bp) {
				printf("Enter response code for %s: ", bp);
				if(safegets(buf, sizeof(buf)) == NULL || buf[0] == '\0')
					exit(0);
			} else {
				printf("Password: ");
				readnoecho(buf, sizeof buf);
			}
		}
		(void) strcat(logtries, logname);
		(void) strcat(logtries, " ");
		strcpy(eusername, logname);

		if((kp->expire >= now) && (*kp->kt->comp)(kp, buf) == 0) {
			goto success;
		}
	}

	doadmin(logtries);
	puts("REFUSED");
	exit(1);
success:
	/* if no dialstring, ask for one */
	/* if dconflg is set, dialstring isn't 0 */
	if(dialstring == 0 || *dialstring == 0) {
		i = 0;
		while(i < max_failures) {
			printf("\nDestination please: ");
			fflush(stdout);
			if(safegets(dialbuf, sizeof dialbuf) == NULL)
				exit(0);
			dialstring = dialbuf;
			while(*dialstring == ' ' || *dialstring == '\t')
				*dialstring++;
			for(dp = dialstring;
				*dp != '\0' && *dp != ' ' && *dp != '\t'; dp++)
				;
			*dp = '\0';
			if(*dialstring != '\0')
				break;
			i++ ;
		}
		if(i == max_failures) {
			puts("\nNo new dialstring.  Goodbye.\n");
			exit(1);
		}
	}

	if (gnotflg)
		options[0] = 'U' ;
	else
		options[0] = '-' ;
	options[1] = 0 ;

	/* using "dialstring", send a redial msg to mgr */
	strcpy(redialbuf, options) ;
	strcat(redialbuf, ">") ;
	strcat(redialbuf, dialstring) ;

	info.rfd = -1;
	info.cfd = 0;	/* pass back fd 0 */
	info.myname = NULL;
	info.user = logname;
	info.machine = "";
	info.uid = info.gid = -1;
	info.name = ipcpath(redialbuf, nettype, defsvc);
	info.param = "light";	/* bogus */
	info.flags = 0;
	if (log != 0) {
		lognow();
		fprintf(log, "%s %s (%s) redialing to %s\n", 
			srcid, username, eusername, info.name);
		fclose(log);
	}
	if(pushed_ld)
		ioctl(0, FIOPOPLD, (void *)0);
	if(ipcredial(&info) != 0)
		;	/* should complain somehow! */
	exit(0);
}

int
init(ac, av)
	int ac;
	char* av[];
{
	FILE* cf;
	char line[128] ;
#define NFLDS	10
	char *fld[NFLDS];
	int l, i, j, errflg = 0;
	struct disallow *d = 0, *dp;
	struct secmap *sp, *slp = 0;
	regexp *prog;
	char *c, *c2 = 0;
	char *cntlfile = CONTROL_FILE;
	int seen_defmap = 0;
	char *malloc();
	extern char *optarg;
	extern int optind;

	while((i = getopt(ac, av, "nf:")) != -1)
		switch(i) {
		case 'n':
			gnotflg++;
			break;
		case 'f':
			cntlfile = optarg;
			break;
		case '?':
			errflg++;
			break;
		}
	if(errflg) {
		fprintf(stderr, "authmgr: bad arglist\n");
		return 1;
	}
	strcpy(physource, "auth1.1.1.F");
	chdir("/tmp");	/* in case of core dumps; I wanna find them */
	c = getenv("CSOURCE");
	if(c) {
		/* this "knows" the format of a CSOURCE */
		if(strncmp(c, "source=", 7) == 0) {
			c += 7;
			c2 = nettype;
			while(*c != '!')
				*c2++ = *c++;
			*c2 = '\0';

			c++;
			c2 = srcid;
			while(*c != ' ')
				*c2++ = *c++;
			*c2 = '\0';
		} else
			strcpy(nettype, "dk");	/* XXX */

		while(c = strchr(c, ' ')) {
			c++;
			if(strncmp(c, "user=", 5) == 0) {
				c += 5;
				c2 = username;
				i = 0;
				while(*c != ' ') {
					if(*c >= '0' && *c <= '9')
						i++;
					*c2++ = *c++;
				}
				*c2 = '\0';
				c++;
				/*
				 * ignore the user name if it's numeric or
				 * if it's the magic unknown user
				 */
				if(i == strlen(username) || strcmp(username, "_unknown_") == 0)
					username[0] = '\0';
			}
			if(strncmp(c, "line=", 5) == 0) {
				strcpy(physource, c+5);
				break;
			}
		}
	}
	strcpy(eusername, username);
	c = getenv("CDEST");
	if(c) {
		strcpy(line, c);
		c = line;
		setfields("!");
		i = getfields(c, fld, NFLDS);
		if(i >= 4) {
			/* check if it matches my service; if so, ignore it */
			j = strlen(fld[2]);
			if((i = strlen(fld[3])) <= j ||
				strcmp(&fld[3][i-j], fld[2]) != 0) {
				strcpy(dialbuf, fld[3]);
				dialstring = dialbuf;
			}
		}
	}

	if(dialstring != NULL) {
		if((i = strlen(dialstring)) > 4 &&
			strcmp(&dialstring[i-4], "dcon") == 0)
			dconflg++;
	}
	if(!dconflg && !gnotflg)
		printf("Security Authentication check\n\n");

	if((cf = fopen(cntlfile, "r")) == NULL) {
	    fprintf(stderr, "authmgr: No control file\n");
	    return 2;
	}

	setfields(" \t");
	l = 0;
	while(fgets(line, 128, cf) != NULL) {
	    l++;
	    for(i=0; line[i] == ' ' || line[i] == '\t'; i++)
		;
	    if(line[i] == '#') /* ignore comments */
		continue;
	    if((c = strchr(&line[i], '\n')) != NULL) /* strip newline */
		*c = '\0';

	    i = getmfields(&line[i], fld, NFLDS);
	    if(i == 0)	/* blank line */
		continue;

	    if(strcmp(fld[0], "admin") == 0) {
		if(i < 2) {
		    fprintf(stderr, SYNTAX, l);
		    return 3;
		}
		(void) strcpy(admins, fld[1]);
		for(j=2; j<i; j++) {
			strcat(admins, " ");
			strcat(admins, fld[j]);
		}
	    } else if(strcmp(fld[0], "failures") == 0) {
		if(i != 2 || (max_failures = atoi(fld[1])) < 3) {
		    fprintf(stderr, SYNTAX, l);
		    return 4;
		}
	    } else if(strcmp(fld[0], "disallow") == 0) {
		if(i != 2) {
		    fprintf(stderr, SYNTAX, l);
		    return 5;
		}
		dp = (struct disallow*) malloc(sizeof(struct disallow));
		if(dp == 0) {
		    fprintf(stderr, "authmgr: out of memory\n");
		    return 6;
		}
		(void) strncpy(dp->logname, fld[1], 8);
		dp->logname[8] = '\0';
		dp->next = NULL;
		if(d == 0) {
		    badlist = d = dp;
		} else {
		    d->next = dp;
		    d = dp;
		}
	    } else if(strcmp(fld[0], "usepasswd") == 0) {
		if(i != 2) {
		    fprintf(stderr, SYNTAX, l);
		    return 7;
		}
		if((prog = regcomp(fld[1])) == NULL) {
		    fprintf(stderr, SYNTAX, l);
		    free((char*)prog);
		    return 8;
		}
		if(regexec(prog, srcid, mlist, MLIST_SIZE) != 0) {
		    usepasswd++;
		}
	    } else if(strcmp(fld[0], "setuser") == 0) {
		if((i < 2) | (i > 3)) {
		    fprintf(stderr, SYNTAX, l);
		    return 9;
		}
		if((prog = regcomp(fld[1])) == NULL) {
		    fprintf(stderr, SYNTAX, l);
		    free((char*)prog);
		    return 10;
		}
		if(regexec(prog, srcid, mlist, MLIST_SIZE) != 0) {
			if (i == 2)
		    		eusername[0] = '\0';
			else {
				strncpy(eusername, fld[2], 8);
				eusername[8] = '\0';
			}
		}
	    } else if(strcmp(fld[0], "setsvc") == 0) {
		if(i != 3) {
		    fprintf(stderr, SYNTAX, l);
		    return 11;
		}
		if((prog = regcomp(fld[1])) == NULL) {
		    fprintf(stderr, SYNTAX, l);
		    free((char*)prog);
		    return 12;
		}
		if(regexec(prog, srcid, mlist, MLIST_SIZE) == 0)
			continue;
		strncpy(defsvc, fld[2], 16);
		defsvc[16]='\0';
	    } else if(strcmp(fld[0], "setlog") == 0) {
		if(i != 3) {
		    fprintf(stderr, SYNTAX, l);
		    return 13;
		}
		if((prog = regcomp(fld[1])) == NULL) {
		    fprintf(stderr, SYNTAX, l);
		    free((char*)prog);
		    return 13;
		}
		if(regexec(prog, srcid, mlist, MLIST_SIZE) == 0)
			continue;
		if((log = fopen(fld[2], "a")) == NULL) {
			fprintf(stderr, "Can't open log file %s\n", fld[2]);
			return 20;
		}
	    } else if(strcmp(fld[0], "secidmap") == 0) {
		if(i != 3) {
		    fprintf(stderr, SYNTAX, l);
		    return 14;
		}
		sp = (struct secmap*) malloc(sizeof(struct secmap));
		if(strcmp(fld[1], ".*") == 0)
			seen_defmap = 1;
		else if(seen_defmap) {
		    fprintf(stderr, "authmgr: Warning; default secidmap is not last secidmap at line %d\n", l);
		    free((char *)sp);
		    continue;
		}
		if((sp->prog = regcomp(fld[1])) == NULL) {
		    fprintf(stderr, SYNTAX, l);
		    free((char *)sp);
		    return 15;
		}
		if(slp == 0)
			slp = secidlist = sp;
		else {
			slp->next = sp;
			slp = sp;
		}
		slp->secid = strdup(fld[2]);
		slp->next = NULL;
	    } else {
		fprintf(stderr, SYNTAX, l);
		return 16;
	    }
	}
	if(!seen_defmap) {
		fprintf(stderr, "authmgr: No default ID configured. Help!\n");
		return 17;
	}
	if(!dconflg && !gnotflg) {
		struct tchars tcbuf;
		extern int tty_ld;
		/* make sure interface is usable by humans */
		if(ioctl(0, TIOCGETC, &tcbuf) < 0) {
			/* no tty ld (of any type) present; push one */
			if(ioctl(0, FIOPUSHLD, &tty_ld) < 0)
				printf("No tty processing supported; sorry\n");
			pushed_ld++;
		}
	}
	return 0;
}

char*
makesourceid()
{
	static char sbuf[128];
	struct secmap *sp = secidlist;

	while(sp != NULL) {
		if(regexec(sp->prog, srcid, mlist, MLIST_SIZE) != 0) {
			sprintf(sbuf, "%s.%s", sp->secid, physource);
			return sbuf;
		}
		sp = sp->next;
	}
}

readnoecho(rbuf, n)
	char *rbuf;
	int n;
{
	struct sgttyb sg, noecho;
	int failed = 0;

	failed = ioctl(0, TIOCGETP, &sg);
	if(failed >= 0) {
		noecho = sg;
		noecho.sg_flags &= ~ECHO;
		ioctl(0, TIOCSETP, &noecho);
	}
	safegets(rbuf, n);
	if(failed >= 0)
		ioctl(0, TIOCSETP, &sg);
}

void
regerror(msg)
	char *msg;
{
	fprintf(stderr, "authmgr: %s\n", msg);
	return;
}

#include <utsname.h>

/*
 * doadmin -
 *  do whatever administrative muck is necessary when a login
 *  fails to authenticate itself after max_failures tries.
 */
doadmin(logname)
	char* logname;
{
	char buf[256];
	FILE* f;
	struct utsname un;

	if (log != NULL) {
		lognow();
		fprintf(log, "%s %s (%s) failed\n",
			srcid, username, eusername);
		fclose(log);
	}

	(void) uname(&un);
	sprintf(buf, "/bin/mail %s", admins);
	if((f = popen(buf, "w")) == NULL)
		return;	/* XXX */

	fprintf(f, "\
Subject: security alert from server %s\n\
\n\
The login(s) ``%s'' failed to correctly authenticate\n\
itself after %d consecutive attempts.\n\
The source of the attempts was:\n\
	%s\n\
Recommendation: disable and investigate the login(s).\n",
		un.nodename, logname, max_failures, getenv("CSOURCE"));

	(void) pclose(f);
}


/*
 *  a safe version of gets
 */
safegets(bp, n)
	char *bp;
	int n;
{
	char *cp;

	if(fgets(bp, n, stdin)==NULL)
		return NULL;
	for(cp=bp; *cp; cp++)
		if(*cp=='\r' || *cp =='\n'){
			*cp = 0;
			break;
		}
	return bp;
}

lognow()
{
	struct	tm *now;
	long	seconds;

	(void) time(&seconds); 
	now = localtime(&seconds);
	fprintf(log, "%02d/%02d %02d:%02d:%02d	",
		now->tm_mon+1, now->tm_mday,
		now->tm_hour, now->tm_min, now->tm_sec);
}
