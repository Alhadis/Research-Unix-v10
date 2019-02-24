/*
 *	processor manager for UNIX systems
 */

#include <dk.h>
#include <dkmgr.h>
#include <pwd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <utmp.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <wait.h>

#define CTLFILE	"/etc/procctl"
#define LOGLVL	1

#define	SYSERR	010		/* system error, "something is wrong" */
#define	BUSY	011		/* destination busy */
#define	NOCMC	012		/* remote node not answering */
#define	NODEST	013		/* destination not answering */
#define	INTERT	014		/* unassigned number */
#define	REORT	015		/* system overload */

/*
 *	format of procctl file entry --
 *
 *	field 1 - source name, or * for any, of requester for this entry
 *	field 2 - service type requested, character string
 *	field 3 - converted service name for local use in this program
 *
 /

/*
 * defines for a character string "switch" statement
 *		SSWITCH(string) {
 *			SCASE("value")
 *				statements ;
 *			SCASE("value")
 *				statements ;
 *		}
 */
#define SSWITCH(c)  	SSTR=c;if(0)
#define	SCASE(c)	}else if (strcmp(SSTR,c)==0){
char * SSTR ;

char	sname[32] ;		/* system name from ctl file */
char	code[16] ;		/* service code from ctl file */
char 	ncode[16] ;		/* converted service code */
char	uid[16] ;		/* uid if special */
char	remuid[16] ;		/* uid from remote system to map */
char	dkname[32] ;		/* system name from /etc/whoami */

char	parmbuf[512] ;		/* for additional parameters */
short	parmlen ;		/* length of additional stuff */
char	env1[64];
extern	char **environ ;
static	char *envinit[] = {
	env1,
	0
} ;

char	*oursrv ;		/* pointer to our server name */
struct	mgrmsg *imsg ;		/* pointer to request message from remote */
struct	passwd *pwent, *pwsearch();  /* password entry */
struct	passwd *getnam();
char	pwline[256];		/* line from password file */
char	*strchr();
int	proctab[512];
void	chdies(), rmut() ;

char	*logfile = "/usr/dk/LOGPROC" ;
char	logbuf[BUFSIZ];
int	loglvl = LOGLVL ;
FILE	*logf ;
struct	sgttyb term ;

extern	int getopt(), optind;
extern	char *optarg;
extern	char *dkfilename();

main(argc, argv)
char **argv ;
{
	register short i ;
	register short fi ;
	short f2 ;
	FILE * fip ;
	extern int dkmgropen ;
	struct mgrmsg *dkmgr() ;
	extern int errno, dkp_ld, tty_ld, cdkp_ld, rmesg_ld ;
	register char *cp, *filename;
	int msg, tty;
	int traffic = 2;

	fi = open("/etc/whoami", 0) ;
	if (fi < 0) {
		perror("mgrproc: open /etc/whoami: ") ;
		exit(1) ;
	}
	i = read(fi, dkname, sizeof(dkname)) ;
	if (i <= 0) {
		printf("bad read of /etc/whoami\n") ;
		exit(1) ;
	}
	dkname[i] = '\0' ;
	if ((cp = strchr(dkname, '\n')))
		*cp = '\0';
	close(fi) ;
	oursrv = dkname;
	while ((i = getopt(argc, argv, "s:t:v:l:")) != EOF) {
		switch(i) {
		case 's':	/* server */
			oursrv = optarg;
			continue;

		case 't':	/* traffic class */
			traffic = atoi(optarg);
			continue;

		case 'v':	/* verbosity of logfile comments */
			loglvl = atoi(optarg);
			continue;

		case 'l':	/* name of logfile */
			logfile = optarg;
			continue;

		default:	
			exit(1);

		}
	}
	if (i = fork()) {
		printf("mgrproc:  starting server %s on system %s, pid %d\n",
		    oursrv, dkname, i) ;
		exit(0) ;	/* parent exits, child continues */
	}
	logf = fopen(logfile, "a") ;
	if (logf == NULL)
		printf("cannot open/create log file\n") ;
	else
		setbuf(logf, logbuf);


	signal(SIGINT, SIG_IGN) ;
	signal(SIGQUIT, SIG_IGN) ;
	signal(SIGHUP, SIG_IGN) ;
	signal(SIGTERM, SIG_IGN) ;
	signal(SIGPIPE, SIG_IGN) ;
	signal(SIGALRM, SIG_IGN) ;
	signal(SIGCHLD, chdies) ;
	pwsearch("root", -1, pwline);	/* prime passwd file lookup */
	fip = fopen(CTLFILE, "r") ;
	if (fip == NULL) {
		printf("mgrproc: can't open /etc/procctl\n");
		exit(1);
	}
	for (;;) {
		imsg = dkmgr(oursrv, traffic) ;
		if (imsg == NULL) {
			if (errno == EINTR) {
#				define INULL (int *)NULL
				while ((i = wait3(INULL, WNOHANG, INULL)) > 0) {
					register j;
					for (j=0; j<512; j++)
						if (proctab[j]==i) {
							rmut(j);
							proctab[j] = 0;
							break;
						}
					dolog(3, "CHILD DIES c=%d\n", j) ;
				}
				continue ;
			}
			perror("mgrproc error in dkmgr: ") ;
			exit(1) ;
		}
		if (imsg->m_service == NULL)
			imsg->m_service = "(NULL)" ;	/* default service */
		if (imsg->m_uid == NULL)
			imsg->m_uid = "(NULL)" ;
		if (imsg->m_source == NULL)
			imsg->m_source = "(NULL)" ;
		dolog(1, "REQUEST c=%d, t=%s, UID=%s, from %s\n",
		  imsg->m_chan, imsg->m_service, imsg->m_uid, imsg->m_source) ;
		for (cp=imsg->m_service; *cp; cp++)
			if (*cp == '.')
				*cp = '\0' ;
		fseek(fip, 0L, 0);
		while (fscanf(fip, "%s %s %s %[^\n]\n",
		      sname, code, ncode, parmbuf) != EOF) {
			if (strcmp(code, imsg->m_service) == 0 &&
			    cksource(sname, imsg->m_source) )
				goto gotit ;
		}
		dolog(0, "ILLEGAL REQUEST  chan %d\n", imsg->m_chan) ;
		dkmgrnak(imsg->m_chan, INTERT) ;
		continue ;

gotit:
		if (ncode[0] == '*')
			strcpy(ncode, code) ;
		pwent = NULL;
		if (strcmp(imsg->m_uid, "(NULL)"))
			pwent = pwsearch(imsg->m_uid, -1, pwline);
		if ((i = fork()) > 0) {
			proctab[imsg->m_chan] = i;
			continue ;
		} else if (i < 0) {
			dolog(0, "ERROR can't fork");
			dkmgrnak(imsg->m_chan, NODEST);
			continue;
		}
		filename = dkfilename(imsg->m_chan);
		if (filename == NULL) {
			dolog(0, "Can't find file for chan %d\n", imsg->m_chan);
			dkmgrnak(imsg->m_chan, NODEST);
			exit(1);
		}
		f2 = open(filename, 2) ;
		if (f2 < 0) {
			dolog(0, "ERROR cannot open %s\n", filename);
			dkmgrnak(imsg->m_chan, NODEST) ;	/* error */
			exit(1) ;
		}
		dolog(7, "DEBUG ncode %s\n", ncode) ;
		environ = envinit ;
		sprintf(environ[0], "DKSOURCE=%s.%s", imsg->m_source,
			imsg->m_uid);
 
		SSWITCH(ncode) {

		SCASE("login")
			if (dkproto(f2, cdkp_ld) < 0 ||
			  ioctl(f2, FIOPUSHLD, &tty_ld) < 0) {
				dolog(0, "FAILED PUSHLD %s\n", ncode) ;
				dkmgrnak(imsg->m_chan, REORT) ;
				exit(1) ;
			}
			dkmgrack(imsg->m_chan) ;
			setfd(f2) ;
			execl("/etc/login", "login", 0) ;
			execl("/bin/login", "login", 0) ;
			dolog(0, "FAILED EXEC login\n") ;
			exit(1) ;

		SCASE("dcon")
			msg = 0;
			goto dc;

		SCASE("mesgdcon")
			msg = 1;

		dc:
			if (dkproto(f2, dkp_ld) < 0) {
				dolog(0, "FAILED PUSHLD %s\n", ncode) ;
				dkmgrnak(imsg->m_chan, REORT) ;
				exit(1) ;
			}
			dkmgrack(imsg->m_chan) ;
			pwent = getnam(imsg->m_uid, f2, pwent) ;
			if (pwent == NULL) {
				dolog(0,"FAILED passwd %s\n",imsg->m_uid);
				exit(1) ;
			}
			setfd(f2) ;
			if (msg)
				ioctl(0, FIOPUSHLD, &rmesg_ld);
			else
				ioctl(0, FIOPUSHLD, &tty_ld);
			execl("/etc/login", "login", "-p", pwline, 0) ;
			execl("/bin/login", "login", "-p", pwline, 0) ;
			dolog(0, "FAILED EXEC login\n") ;
			exit(1);

		SCASE("mesgexec")
			msg = 1;
			tty = 0;
			goto ex;

		SCASE("exec")
			msg = 0;
			tty = 0;
			goto ex;

		SCASE("ttyexec")
			msg = 0;
			tty = 1;
		ex:
			if (dkproto(f2, dkp_ld)<0) {
				dolog(0, "FAILED PUSHLD %s\n", ncode) ;
				dkmgrnak(imsg->m_chan, REORT) ;
				exit(1) ;
			}
			dkmgrack(imsg->m_chan) ;
			pwent = getnam(imsg->m_uid, f2, pwent) ;
			if (pwent == NULL)
				exit(0) ;
			setfd(f2) ;
			if (rparm(0) < 0)
				exit(0) ;
			if (msg) {
				if (ioctl(0, FIOPUSHLD, &rmesg_ld) < 0) {
					dolog(0, "FAILED PUSHLD(rmesg)\n");
					exit(1) ;
				}
			}
			if (tty) {
				if (ioctl(0, FIOPUSHLD, &tty_ld)<0) {
					dolog(0, "FAILED PUSHLD(tty)\n");
					exit(1) ;
				}
			}
			execl("/etc/login", "login", "-p", pwline, parmbuf, 0);
			execl("/bin/login", "login", "-p", pwline, parmbuf, 0);
			dolog(0, "FAILED EXEC login\n");
			exit(1) ;

		SCASE("cmd")
			/* first param is uid, rest go to sh */
			dolog(7, "DEBUG cmd %s\n", parmbuf) ;
			if (dkproto(f2, dkp_ld)<0) {
				dolog(0, "FAILED PUSHLD %s\n", ncode) ;
				dkmgrnak(imsg->m_chan, REORT) ;
				exit(1) ;
			}
			dkmgrack(imsg->m_chan) ;
			setfd(f2) ;
			cp = parmbuf ;
			while (*cp != ' ' && *cp != '\t' && *cp != '\0')
				cp++ ;
			*cp++ = '\0' ;
			while (*cp == ' ' || *cp == '\t')
				cp++ ;
			dolog(7, "DEBUG cmd uid %s cmd %s\n", parmbuf, cp) ;
			execl("/etc/login", "login", "-f", parmbuf, cp, 0) ;
			execl("/bin/login", "login", "-f", parmbuf, cp, 0) ;
			dolog(0, "FAILED EXEC login %s\n", cp) ;
			exit(1) ;

		}
		dolog(0, "ILLEGAL CODE %s\n", ncode) ;
		dkmgrnak(imsg->m_chan, INTERT) ;
		exit(1) ;
	}
}

/* VARARGS2 */
dolog(level, fmt, a1, a2, a3, a4, a5)
char *fmt;
{
	long clock ;
	long time() ;
	char *ctime() ;

	if (loglvl<level || logf==NULL)
		return;
	clock = time(0) ;
	fseek(logf, 0L, 2);
	fprintf(logf, "%.15s-%d(%d)  ", ctime(&clock)+4, getpid(), loglvl) ;
	fprintf(logf, fmt, a1, a2, a3, a4, a5);
	fflush(logf);
}


/*
 * Interrupt routine for child death
 */
void
chdies()
{
	signal(SIGCHLD, chdies);
}

/*
 * delete entry from utmp file
 */
void
rmut(i)
{
	register f;
	register char *line;
	struct utmp wtmp;

	line = dkfilename(i);
	if (line==0)
		return;
	line += sizeof("/dev/") - 1;
	f = open("/etc/utmp", 2);
	if(f >= 0) {
		while(read(f, (char *)&wtmp, sizeof(wtmp)) == sizeof(wtmp)) {
			if (strncmp(wtmp.ut_line, line, sizeof(wtmp.ut_line)))
				continue;
			lseek(f, -(long)sizeof(wtmp), 1);
			strncpy(wtmp.ut_name, "", sizeof(wtmp.ut_name));
			time(&wtmp.ut_time);
			write(f, (char *)&wtmp, sizeof(wtmp));
		}
		close(f);
	}
	f = open("/usr/adm/wtmp", 1);
	if (f >= 0) {
		strncpy(wtmp.ut_line, line, sizeof(wtmp.ut_line));
		strncpy(wtmp.ut_name, "", sizeof(wtmp.ut_name));
		time(&wtmp.ut_time);
		lseek(f, (long)0, 2);
		write(f, (char *)&wtmp, sizeof(wtmp));
		close(f);
	}
}

/*
 *	check a source name against a prototype name
 *	in the control file
 *		return 0 if no match
 *		return 1 if ok match
 */
cksource(ck, src)
register char *ck, *src ;
{

	while (*ck == *src) {
		if (*ck == 0)
			break ;
		ck++ ; src++ ;
	}
	if (*ck == *src)
		return 1 ;
	if (*ck == '*')
		return 1 ;
	return 0 ;
}

struct passwd *
getnam(try1, f2, pw)
char * try1;
register struct passwd *pw ;
{
	register char * cp ;

	if (pw && pw->pw_uid) {
		write(f2, "OK", 2) ;
		return pw ;
	}
	write(f2, "NO", 2);
	while (1) {
		if (rparm(f2) < 0) {
			dolog(2, "HANGUP c=%d receiving uid\n", imsg->m_chan) ;
			exit(1) ;
		}
		for (cp = parmbuf; *cp; cp++) {
			if (*cp == ' ' || *cp == '.' || *cp == ',') {
				*cp++ = '\0';
				break ;
			}
		}
		pw = pwsearch(parmbuf, -1, pwline) ;
		if (pw && (pw->pw_passwd==NULL
		  || strcmp(crypt(cp, pw->pw_passwd), pw->pw_passwd)==0))
			break;
		write(f2, "NO", 2) ;
	}
	dolog(4, "TRACE UID %s\n", parmbuf) ;
	write(f2, "OK", 2) ;
	return pw ;
}


rparm(f)
{
register len ;
register rlen ;
register char *cp ;


	rlen = sizeof(parmbuf) ;
	parmlen = 0 ;
	cp = parmbuf ;
	while (1) {
		len = read(f, cp, rlen) ;
		if (len <= 0)
			return -1 ;
		parmlen += len ;
		rlen -= len ;
		cp += len - 1 ;
		if (*cp == '\n' ||
		    *cp == '\r') {
			*cp = '\0' ;
			dolog(7, "DEBUG rparam %s\n", parmbuf) ;
			return 0 ;
		}
		cp++ ;
	}
}

setfd(f)
{
int i ;

	signal(SIGTERM, SIG_DFL) ;
	signal(SIGPIPE, SIG_DFL) ;
	signal(SIGQUIT, SIG_DFL) ;
	signal(SIGINT, SIG_DFL) ;
	signal(SIGALRM, SIG_DFL) ;
	signal(SIGHUP, SIG_DFL) ;
	signal(SIGCHLD, SIG_DFL) ;
	ioctl(f, TIOCSPGRP, 0) ;
	close(0) ;
	close(1) ;
	close(2) ;
	close(3) ;
	dup(f) ;
	dup(f) ;
	dup(f) ;
	dup(f) ;
	for (i=NSYSFILE; i<9; i++)
		if (i != fileno(logf))
			close(i) ;
}
