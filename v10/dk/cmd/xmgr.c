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
#include <string.h>
#include <dkwindow.h>

#define CTLFILE	"/etc/procctl"
#define	CTL2FILE "/etc/procctl.local"
#define	UIDFILE	"/etc/procuid"
#define	UID2FILE "/etc/procuid.local"
#define LOGLVL	1

#define	SYSERR	010		/* system error, "something is wrong" */
#define	BUSY	011		/* destination busy */
#define	NOCMC	012		/* remote node not answering */
#define	NODEST	013		/* destination not answering */
#define	INTERT	014		/* unassigned number */
#define	REORT	015		/* system overload */
#define	PERMISSION  017		/* permission denied */

/*
 *	format of procctl file entry --
 *
 *	field 1 - source name, or * for any, of requester for this entry
 *	field 2 - service type requested, character string
 *	field 3 - converted service name for local use in this program
 *	field 4 - parameters, use depends on service type
 *
 */
struct sprocctl {
	char *	p_sname ;	/* source name */
	char *	p_code ;	/* service type */
	char *	p_ncode ;	/* case stmt type */
	char *	p_param ;	/* params for sub-case */
} procctl[64] ;

/*
 *	format of procuid file entry ---
 *
 *	field 1 - source name, or * for any, or requester for this entry
 *	field 2 - converted service name requested
 *	field 3 - uid as passed from source nachine, of * for any
 *	field 4 - uid to use locally, or * for same as source
 */
struct sprocuid {
	char *	u_sname ;	/* source name */
	char *	u_code ;	/* service type */
	char *	u_uid ;		/* source provided uid */
	char *	u_nuid ;	/* local uid */
} procuid[256] ;

FILE	*fprocctl, *f2procctl, *fprocuid, *f2procuid ;	/* file descriptor for control file */
long	tprocctl, t2procctl, tprocuid, t2procuid ;		/* last ctime of file */
int	nprocctl, nprocuid ;			/* number of entries read from file */
int	zprocctl, zprocuid ;			/* size of file at last look */
char 	*bprocctl, *bprocuid ;			/* where in memory the file lives */

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

char	*ncode ;		/* converted service code */
char	uid[16] ;		/* uid if special */
char	dkname[32] ;		/* system name from /etc/whoami */

char 	*params ;		/* parameters for sub-case handling */
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
int	rootable ;		/* should we allow root here? */
char	pwline[256];		/* line from password file */
int	proctab[512];
void	chdies(), rmut() ;

char	*logfile = "/usr/dk/LOGPROC" ;
char	logbuf[BUFSIZ];
int	loglvl = LOGLVL ;
FILE	*logf ;
struct	sgttyb term ;
char	FROGS[] = "\n&;^|<>(){}";	/* disallowed argument characters */

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
	int msg, tty, wargs;
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
	loadfiles() ;
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
		if (imsg->m_uid == NULL || imsg->m_uid[0] == 0)
			imsg->m_uid = imsg->m_origin ;
		if (imsg->m_source == NULL)
			imsg->m_source = "(NULL)" ;
		dolog(1, "REQUEST c=%d, t=%s, UID=%s, from %s traf %x\n",
		  imsg->m_chan, imsg->m_service, imsg->m_uid, imsg->m_source,
		  imsg->m_traffic&0xFFFF) ;
		for (cp=imsg->m_service; *cp; cp++)
			if (*cp == '.')
				*cp = '\0' ;
		loadfiles() ;
		for (i=0; i<nprocctl; i++) {
			if (strcmp(procctl[i].p_code, imsg->m_service) == 0 &&
			    cksource(procctl[i].p_sname, imsg->m_source)) {
				ncode = procctl[i].p_ncode ;
				params = procctl[i].p_param ;
				goto gotit ;
			}
		}
		dolog(0, "ILLEGAL REQUEST  chan %d\n", imsg->m_chan) ;
		dkmgrnak(imsg->m_chan, INTERT) ;
		continue ;

gotit:
		pwent = NULL;
		rootable = 0 ;
		if (strcmp(imsg->m_uid, "(NULL)")) {
			for (i=0; i<nprocuid; i++) {
				if (cksource(procuid[i].u_sname, imsg->m_source) &&
				    cksource(procuid[i].u_code, imsg->m_service) &&
				    cksource(procuid[i].u_uid, imsg->m_uid)) {
					if (procuid[i].u_nuid[0] != '*') {
						dolog(1, "UID %s.%s -> %s\n", imsg->m_source, imsg->m_uid, procuid[i].u_nuid) ;
						imsg->m_uid = procuid[i].u_nuid ;
						rootable = 1 ;
					}
					goto gotuid ;
				}
			}
			imsg->m_uid = "**BAD**" ;
gotuid:
			pwent = pwsearch(imsg->m_uid, -1, pwline);
		}
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
		ioctl(f2, DIOCNXCL, 0);
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
			setwins(f2, imsg->m_traffic);
			pwent = getnam(imsg->m_uid, f2, pwent, 0) ;
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
			setwins(f2, imsg->m_traffic);
			pwent = getnam(imsg->m_uid, f2, pwent, 0) ;
			if (pwent == NULL)
				exit(0) ;
			setfd(f2) ;
			if (rparm(0, "") < 0)
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

		SCASE("cmda")	/* fixed cmd, user args */
			wargs = 1;
			goto cm;

		SCASE("cmd")	/* fixed cmd, fixed args */
			wargs = 0;
			goto cm;
		cm:
			/* first param is uid, rest go to sh */
			dolog(7, "DEBUG cmd %s\n", params) ;
			if (dkproto(f2, dkp_ld)<0) {
				dolog(0, "FAILED PUSHLD %s\n", ncode) ;
				dkmgrnak(imsg->m_chan, REORT) ;
				exit(1) ;
			}
			cp = params ;
			while (*cp != ' ' && *cp != '\t' && *cp != '\0')
				cp++ ;
			*cp++ = '\0' ;
			while (*cp == ' ' || *cp == '\t')
				cp++ ;
			if (params[0] == '*') {
				params = imsg->m_uid ;
				if (pwent == NULL || (pwent->pw_uid==0 && rootable==0)) {
					dkmgrnak(imsg->m_chan, PERMISSION) ;
					dolog(1, "FAILED procuid %s.%s\n", imsg->m_source, imsg->m_uid) ;
					exit(1) ;
				}
			} else {
				pwent = pwsearch(params, -1, pwline);
				rootable = 1 ;
			}
			dkmgrack(imsg->m_chan) ;
			setwins(f2, imsg->m_traffic);
			setfd(f2) ;
			dolog(7, "DEBUG cmd uid %s cmd %s\n", params, cp) ;
			if (wargs) {
				register char *cp1;
				char tparmbuf[786];

				pwent = getnam(params, 0, pwent, rootable);
				if (rparm(0, FROGS) < 0) {
					dolog(0, "bad args, cmda\n");
					exit(0);
				}
				/* copy cmd name from procctl, args from parmbuf */
				cp1 = tparmbuf;
				while (*cp != ' ' && *cp != '\t' && *cp != '\0')
					*cp1++ = *cp++ ;
				*cp1++ = ' ';
				cp = parmbuf;
				while ((*cp1++ = *cp++) != '\0')
					;
				strcpy(parmbuf, tparmbuf);
				cp = parmbuf;
				dolog(7, "cmda <%s>\n", cp);
			}
			execl("/etc/login", "login", "-p", pwline, cp, 0) ;
			execl("/bin/login", "login", "-p", pwline, cp, 0) ;
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
getnam(try1, f2, pw, root)
char * try1;
register struct passwd *pw ;
int root ;
{
	register char * cp ;

	if (pw && (pw->pw_uid!=0 || root!=0)) {
		write(f2, "OK", 2) ;
		return pw ;
	}
	write(f2, "NO", 2);
	while (1) {
		if (rparm(f2, "") < 0) {
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
		if (pw && pw->pw_uid != 0 && (pw->pw_passwd==NULL
		  || strcmp(crypt(cp, pw->pw_passwd), pw->pw_passwd)==0))
			break;
		write(f2, "NO", 2) ;
	}
	dolog(4, "TRACE UID %s\n", parmbuf) ;
	write(f2, "OK", 2) ;
	return pw ;
}

/* read command parameters, delete funny ones */
rparm(f, frogs)
register char *frogs;
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
		if (*cp == '\n' || *cp == '\r') {
			*cp = '\0' ;
			cp = parmbuf;
			while (*cp) {
				*cp &= 0177;
				if (strchr(frogs, *cp))
					*cp = 'X';
				cp++;
			}
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

/* use large transmit window? */
setwins(f, traffic)
{
	char ws[5];
	long wins;
	if (W_VALID(traffic) && W_TRAF(traffic)) {
		wins = W_VALUE(W_ORIG(traffic));
/****/		dolog(7, "DEBUG wins %d\n", wins) ;
		/* try 3 X 1/4 */
		wins >>= 2;
		ws[0] = wins;
		ws[1] = wins>>8;
		ws[2] = 0;
		ws[3] = 0;
		ws[4] = 3;
/****/		dolog(7, "DEBUG ws0-%d 1-%d 4-%d\n", ws[0], ws[1], ws[4]) ;
		ioctl(f, DIOCXWIN, ws);
	}
}

char * adv(cp)
register char *cp ;
{
	while (*cp != '\0' && *cp != '\n' && *cp != ' ' && *cp != '\t') cp++ ;
	if (*cp == ' ' || *cp == '\t')	*cp++ = '\0' ;
	while (*cp == ' ' || *cp == '\t') cp++ ;
	return (cp) ;
}

char * advn(cp)
register char *cp ;
{
	while (*cp != '\0' && *cp != '\n') cp++ ;
	if (*cp == '\n') *cp++ = '\0' ;
	while (*cp == ' ' || *cp == '\t' || *cp == '\n') cp++ ;
	return (cp) ;
}


loadfiles()
{
struct stat statb ;
struct stat statb2 ;
register char *cp ;
register struct sprocctl *ctlptr ;
register struct sprocuid *uidptr ;
register i ;

f0:
	if (fprocctl == NULL || f2procctl == NULL) {
		if (fprocctl == NULL)
			fprocctl = fopen(CTLFILE, "r") ;
		if (f2procctl == NULL)
			f2procctl = fopen(CTL2FILE, "r") ;
		if (fprocctl == NULL) {
			fprintf(stderr, "mgrproc, can't open %s\n", CTLFILE) ;
			nprocctl = 0 ;
			goto f2 ;
		}
		if (f2procctl == NULL) {
			fprintf(stderr, "mgrproc, can't open %s\n", CTL2FILE) ;
			nprocctl = 0 ;
			goto f2 ;
		}
		fstat(fileno(fprocctl), &statb) ;
		fstat(fileno(f2procctl), &statb2) ;
	} else {
		fstat(fileno(fprocctl), &statb) ;
		fstat(fileno(f2procctl), &statb2) ;
		if (statb.st_ctime == tprocctl && statb2.st_ctime == t2procctl)
			goto f2 ;
		fclose(fprocctl) ;
		fclose(f2procctl) ;
		fprocctl = f2procctl = NULL ;
		goto f0 ;
	}
	tprocctl = statb.st_ctime ;
	t2procctl = statb2.st_ctime ;
	i = statb.st_size + statb2.st_size ;
	if (i > zprocctl) {
		if (zprocctl > 0)
			free(bprocctl) ;
		bprocctl = (char *)malloc(i+4) ;
		zprocctl = i ;
	}
	rewind(fprocctl) ;
	rewind(f2procctl) ;
	fread(bprocctl, statb2.st_size, 1, f2procctl) ;
	if (statb2.st_size)
		fread(&bprocctl[statb2.st_size], statb.st_size, 1, fprocctl) ;
	bprocctl[i] = '\0' ;
	cp = bprocctl ;
	ctlptr = procctl ;
	while (*cp) {
		while (*cp == '#')
			cp = advn(cp) ;
		if (*cp == 0)
			break ;
		ctlptr->p_sname = cp ;
		cp = adv(cp) ;
		ctlptr->p_code = cp ;
		cp = adv(cp) ;
		ctlptr->p_ncode = cp ;
		if (*cp == '*')
			ctlptr->p_ncode = ctlptr->p_code ;
		cp = adv(cp) ;
		ctlptr->p_param = cp ;
		cp = advn(cp) ;
		ctlptr++ ;
	}
	nprocctl = ctlptr - procctl ;
	dolog(7, "Reloaded %s; %d entries\n", CTLFILE, nprocctl) ;
f2:
	if (fprocuid == NULL || f2procuid == NULL) {
		if (fprocuid == NULL)
			fprocuid = fopen(UIDFILE, "r") ;
		if (f2procuid == NULL)
			f2procuid = fopen(UID2FILE, "r") ;
		if (fprocuid == NULL) {
			fprintf(stderr, "mgrproc: can't open %s\n", UIDFILE) ;
			nprocuid = 0 ;
			return ;
		}
		if (f2procuid == NULL) {
			fprintf(stderr, "mgrproc: can't open %s\n", UID2FILE) ;
			nprocuid = 0 ;
			return ;
		}
		fstat(fileno(fprocuid), &statb) ;
		fstat(fileno(f2procuid), &statb2) ;
	} else {
		fstat(fileno(fprocuid), &statb) ;
		fstat(fileno(f2procuid), &statb2) ;
		if (statb.st_ctime == tprocuid && statb2.st_ctime == t2procuid)
			return ;
		fclose(fprocuid) ;
		fclose(f2procuid) ;
		fprocuid = f2procuid = NULL ;
		goto f2 ;
	}
	tprocuid = statb.st_ctime ;
	t2procuid = statb2.st_ctime ;
	i = statb.st_size + statb2.st_size ;
	if (i > zprocuid) {
		if (zprocuid)
			free(bprocuid) ;
		bprocuid = (char *)malloc(i+4) ;
		zprocuid = i ;
	}
	rewind(fprocuid) ;
	rewind(f2procuid) ;
	fread(bprocuid, statb2.st_size, 1, f2procuid) ;
	fread(&bprocuid[statb2.st_size], statb.st_size, 1, fprocuid) ;
	bprocuid[i] = '\0' ;
	cp = bprocuid ;
	uidptr = procuid ;
	while (*cp) {
		while (*cp == '#')
			cp = advn(cp) ;
		if (*cp == '\0')
			break ;
		uidptr->u_sname = cp ;
		cp = adv(cp) ;
		uidptr->u_code = cp ;
		cp = adv(cp) ;
		uidptr->u_uid = cp ;
		cp = adv(cp) ;
		uidptr->u_nuid = cp ;
		cp = adv(cp) ;
		cp = advn(cp) ;
		uidptr++ ;
	}
	nprocuid = uidptr - procuid ;
	dolog(7, "Reloaded %s; %d entries\n", UIDFILE, nprocuid) ;
}

