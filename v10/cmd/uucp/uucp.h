#include "parms.h"

#ifdef BSD4_2
#define V7
#undef NONAP
#undef FASTTIMER
#endif

#ifdef FASTTIMER
#undef NONAP
#endif

#ifdef V8
#define V7
#endif V8

#include <stdio.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/param.h>

#if defined (ATTSV) && ! defined (CDLIMIT)
#include <sys/fmgr.h>
#endif

/*
 * param.h includes types.h and signal.h in 4bsd
 */
#ifdef V7
#include <sys/timeb.h>
#else
#include <termio.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#endif

#include <sys/stat.h>
#ifdef READDIR
#include <ndir.h>
#endif
#ifdef V8
#define	V8DIRSIZ 14	/* to truncate, to make legal filenames */
#endif

#ifdef BSD4_2
#include <sys/time.h>
#else
#include <time.h>
#endif

#include <sys/times.h>
#include <errno.h>

#ifdef ATTSV
#include <sys/sysmacros.h>
#endif ATTSV

#ifdef	RT
#include "rt/types.h"
#include "rt/unix/param.h"
#include "rt/stat.h"
#include <sys/ustat.h>
#endif RT

/* what mode should D. files have upon creation? */
#define DFILEMODE 0600

/* what mode should C. files have upon creation? */
#define CFILEMODE 0644

/* define the value of DIRMASK, for umask call */
/* used for creating system subdirectories */
#define DIRMASK 0022

#define MAXSTART	300	/* how long to wait on startup */

/* define the last characters for ACU  (used for 801/212 dialers) */
#define ACULAST "<"

/*  caution - the fillowing names are also in Makefile 
 *    any changes here have to also be made there
 *
 * it's a good idea to make directories .foo, since this ensures
 * that they'll be ignored by processes that search subdirectories in SPOOL
 *
 *  XQTDIR=/usr/spool/uucp/.Xqtdir
 *  CORRUPT=/usr/spool/uucp/.Corrupt
 *  LOGDIR=/usr/spool/uucp/.Log
 *  SEQDIR=/usr/spool/uucp/.Sequence
 *  STATDIR=/usr/spool/uucp/.Status
 *  
 */

/* where to put the STST. files? */
#define STATDIR		"/usr/spool/uucp/.Status"

/* where should logfiles be kept? */
#define LOGUUX		"/usr/spool/uucp/.Log/uux"
#define LOGUUXQT	"/usr/spool/uucp/.Log/uuxqt"
#define LOGUUCP		"/usr/spool/uucp/.Log/uucp"
#define LOGCICO		"/usr/spool/uucp/.Log/uucico"
#define CORRUPTDIR	"/usr/spool/uucp/.Corrupt"

/* some sites use /usr/lib/uucp/.XQTDIR here */
/* use caution since things are linked into there */
#define XQTDIR		"/usr/spool/uucp/.Xqtdir"

/* how much of a system name can we print in a [CX]. file? */
/* MAXBASENAME - 1 (pre) - 1 ('.') - 1 (grade) - 4 (sequence number) */
#define SYSNSIZE (MAXBASENAME - 7)

#ifdef USRSPOOLLOCKS
#define LOCKPRE		"/usr/spool/locks/LCK."
#else
#define LOCKPRE		"/usr/spool/uucp/LCK."
#endif USRSPOOLLOCKS

#define SQFILE		"/usr/lib/uucp/SQFILE"
#define SQTMP		"/usr/lib/uucp/SQTMP"
#define SLCKTIME	5400	/* system/device timeout (LCK.. files) */
#define SYSFILE		"/usr/lib/uucp/Systems"
#define DEVFILE		"/usr/lib/uucp/Devices"
#define DIALERFILE	"/usr/lib/uucp/Dialers"
#define DIALFILE	"/usr/lib/uucp/Dialcodes"
#define PFILE		"/usr/lib/uucp/Permissions"

#define SPOOL		"/usr/spool/uucp"
#define SEQDIR		"/usr/spool/uucp/.Sequence"

#define X_LOCKTIME	3600
#ifdef USRSPOOLLOCKS
#define SEQLOCK		"/usr/spool/locks/LCK.SQ."
#define SQLOCK		"/usr/spool/locks/LCK.SQ"
#define X_LOCK		"/usr/spool/locks/LCK.X"
#define S_LOCK		"/usr/spool/locks/LCK.S"
#define X_LOCKDIR	"/usr/spool/locks"	/* must be dir part of above */
#else
#define SEQLOCK		"/usr/spool/uucp/LCK.SQ."
#define SQLOCK		"/usr/spool/uucp/LCK.SQ"
#define X_LOCK		"/usr/spool/uucp/LCK.X"
#define S_LOCK		"/usr/spool/uucp/LCK.S"
#define X_LOCKDIR	"/usr/spool/uucp"	/* must be dir part of above */
#endif USRSPOOLLOCKS
#define X_LOCKPRE	"LCK.X"		/* must be last part of above */

#define PUBDIR		"/usr/spool/uucppublic"
#define ADMIN		"/usr/spool/uucp/.Admin"
#define ERRLOG		"/usr/spool/uucp/.Admin/errors"
#define SYSLOG		"/usr/spool/uucp/.Admin/xferstats"
#define RMTDEBUG	"/usr/spool/uucp/.Admin/audit"
#define CLEANUPLOGFILE	"/usr/spool/uucp/.Admin/uucleanup"

#define	WORKSPACE	"/usr/spool/uucp/.Workspace"

#define SQTIME		60
#define TRYCALLS	2	/* number of tries to dial call */
#define MINULIMIT	(1L<<11)	/* minimum reasonable ulimit */

/*
 * CDEBUG is for communication line debugging 
 * DEBUG is for program debugging 
 * #define SMALL to compile without the DEBUG code
 */
#define CDEBUG(l, f, s) if (Debug >= l) fprintf(stderr, f, s)
#ifndef SMALL
#define DEBUG(l, f, s) if (Debug >= l) fprintf(stderr, f, s)
#else
#define DEBUG(l, f, s)
#endif SMALL

/*
 * VERBOSE is used by cu and ct to inform the user
 * about the progress of connection attempts.
 * In uucp, this will be NULL.
 */

#ifdef STANDALONE
#define VERBOSE(f, s) if (Verbose > 0) fprintf(stderr, f, s); else
#else
#define VERBOSE(f, s)
#endif

#define PREFIX(pre, str)	(strncmp((pre), (str), strlen(pre)) == SAME)
#define BASENAME(str, c) ((Bnptr = strrchr((str), c)) ? (Bnptr + 1) : (str))
#define EQUALS(a,b)	((a) && (b) && (strcmp((a),(b))==SAME))
#define EQUALSN(a,b,n)	((a) && (b) && (strncmp((a),(b),(n))==SAME))
#define LASTCHAR(s)	(s+strlen(s)-1)

#define SAME 0
#define ANYREAD 04
#define ANYWRITE 02
#define FAIL -1
#define SUCCESS 0
#define NULLCHAR	'\0'
#define CNULL (char *) 0
#define STBNULL (struct sgttyb *) 0
#define MASTER 1
#define SLAVE 0
#define MAXBASENAME 14 /* should be DIRSIZ but 4.2bsd prohibits that */
#define MAXFULLNAME BUFSIZ
#define MAXNAMESIZE	64	/* /usr/spool/uucp/<14 chars>/<14 chars>+slop */
#define MAXMSGTIME 33
#define MAXEXPECTTIME 45
#define MAXCHARTIME 15
#define NAMESIZE MAXBASENAME+1
#define	SIZEOFPID	10		/* maximum number of digits in a pid */
#define EOTMSG "\004\n\004\n"
#define CALLBACK 1

/* manifest for chkpth flag */
#define CK_READ		0
#define CK_WRITE	1

/*
 * commands
 */
#define SHELL		"/bin/sh"
#define UUCICO		"/usr/lib/uucp/uucico"
#define UUSCHED		"/usr/lib/uucp/uusched"
#define UUXQT		"/usr/lib/uucp/uuxqt"
#define UUCP		"uucp"
#ifdef V7
#define	UUPS		"/usr/lib/uucp/uups"
#endif


/* system status stuff */
#define SS_OK			0
#define SS_NO_DEVICE		1
#define SS_TIME_WRONG		2
#define SS_INPROGRESS		3
#define SS_CONVERSATION		4
#define SS_SEQBAD		5
#define SS_LOGIN_FAILED		6
#define SS_DIAL_FAILED		7
#define SS_BAD_LOG_MCH		8
#define SS_LOCKED_DEVICE	9
#define SS_ASSERT_ERROR		10
#define SS_BADSYSTEM		11
#define SS_CANT_ACCESS_DEVICE	12
#define SS_DEVICE_FAILED	13	/* No longer used */
#define SS_WRONG_MCH		14
#define SS_CALLBACK		15
#define SS_RLOCKED		16
#define SS_RUNKNOWN		17
#define SS_RLOGIN		18
#define SS_UNKNOWN_RESPONSE	19
#define SS_STARTUP		20
#define SS_CHAT_FAILED		21

#define MAXPH	60	/* maximum phone string size */
#define	MAXC	BUFSIZ

#define	TRUE	1
#define	FALSE	0
#define NAMEBUF	32

/* structure of an Systems file line */
#define F_MAX	50	/* max number of fields in Systems file line */
#define F_NAME 0
#define F_TIME 1
#define F_TYPE 2
#define F_CLASS 3	/* an optional prefix and the speed */
#define F_PHONE 4
#define F_LOGIN 5

/* structure of an Devices file line */
#define D_TYPE 0
#define D_LINE 1
#define D_CALLDEV 2
#define D_CLASS 3
#define D_CALLER 4
#define D_ARG 5
#define D_MAX	50	/* max number of fields in Devices file line */

#define D_ACU 1
#define D_DIRECT 2
#define D_PROT 4

/* past here, local changes are not recommended */
#define CMDPRE		'C'
#define DATAPRE		'D'
#define XQTPRE		'X'

/*
 * stuff for command execution
 */
#define X_RQDFILE	'F'
#define X_STDIN		'I'
#define X_STDOUT	'O'
#define X_CMD		'C'
#define X_USER		'U'
#define X_BRINGBACK	'B'
#define X_MAILF		'M'
#define X_RETADDR	'R'
#define X_COMMENT	'#'
#define X_NONZERO	'Z'
#define X_SENDNOTHING	'N'
#define X_SENDZERO	'n'


/* This structure describes call routines */
struct caller {
	char	*CA_type;
	int	(*CA_caller)();
};

/* This structure describes dialing routines */
struct dialer {
	char	*DI_type;
	int	(*DI_dialer)();
};

struct nstat {
	int	t_pid;		/* process id				*/
	long	t_start;	/* process id				*/
	time_t	t_beg;		/* start  time				*/
	time_t	t_scall;	/* start call to system			*/
	time_t	t_ecall;	/* end call to system			*/
	time_t	t_tacu;		/* acu time				*/
	time_t	t_tlog;		/* login time				*/
	time_t	t_sftp;		/* start file transfer protocol		*/
	time_t	t_sxf;		/* start xfer 				*/
	time_t	t_exf;		/* end xfer 				*/
	time_t	t_eftp;		/* end file transfer protocol		*/
	time_t	t_qtime;	/* time file queued			*/
	int	t_ndial;	/* # of dials				*/
	int	t_nlogs;	/* # of login trys			*/
	struct tms t_tbb;	/* start execution times		*/
	struct tms t_txfs;	/* xfer start times			*/
	struct tms t_txfe;	/* xfer end times 			*/
	struct tms t_tga;	/* garbage execution times		*/
};

/* external declarations */

extern int Ifn, Ofn;
extern int Debug, Verbose;
extern int Bspeed;
extern int Uid, Euid;		/* user-id and effective-uid */
extern char Wrkdir[];
extern long Retrytime;
extern char **Env;
extern char Uucp[];
extern char Pchar;
extern struct nstat Nstat;
extern char Dc[];			/* line name			*/
extern char Fwdname[];		/* foward name			*/
extern int Seqn;			/* sequence #			*/
extern int Role;
extern char Logfile[];
extern int linebaudrate;	/* adjust sleep time on read in pk driver */
extern char Rmtname[];
extern char User[];
extern char Loginuser[];
extern char *Thisdir;
extern char *Spool;
extern char *Pubdir;
extern char Myname[];
extern char Progname[];
extern char RemSpool[];
extern char *Bnptr;		/* used when BASENAME macro is expanded */
extern char *sys_errlist[];

extern char Jobid[];		/* Jobid of current C. file */
extern int Uerror;		/* global error code */
extern char *UerrorText[];	/* text for error code */

/*	Some global I need for section 2 and section 3 routines */
extern errno;
extern char *optarg;	/* for getopt() */
extern int optind;	/* for getopt() */


#define UERRORTEXT		UerrorText[Uerror]
#define UTEXT(x)		UerrorText[x]

/* things get kind of gross beyond this point -- please stay out */

#define index strchr
#define rindex strrchr 
#ifdef ATTSV
#define vfork fork
#define ATTSVKILL
#define UNAME
#endif

#ifdef lint
#define VERSION(x)	;
#define ASSERT(e, s1, s2, i1)	;

#else

#define VERSION(x)	static	char	sccsid[] = "x";
#define ASSERT(e, s1, s2, i1) if (!(e)) {\
	assert(s1, s2, i1, sccsid, __FILE__, __LINE__);\
	cleanup(FAIL);};
#endif

extern struct stat __s_;
#define READANY(f)	((stat((f),&__s_)==0) && ((__s_.st_mode&(0004))!=0) )
#define READSOME(f)	((stat((f),&__s_)==0) && ((__s_.st_mode&(0444))!=0) )

#define WRITEANY(f)	((stat((f),&__s_)==0) && ((__s_.st_mode&(0002))!=0) )
#define DIRECTORY(f)	((stat((f),&__s_)==0) && ((__s_.st_mode&(S_IFMT))==S_IFDIR) )
#define NOTEMPTY(f)	((stat((f),&__s_)==0) && (__s_.st_size!=0) )

#ifndef READDIR
#define DIR FILE
#define opendir(x) fopen((x), "r")
#define closedir(x) fclose((x))
#endif

/* standard functions used */

extern char	*strcat(), *strcpy(), *strncpy(), *strrchr();
extern char	*strchr(), *strpbrk();
extern char	*index(), *rindex(), *getlogin(), *ttyname(), *malloc();
extern char	*calloc();
extern long	times(), lseek(), atol();
extern time_t	time();
extern int	strlen(), strcmp(), strncmp();
extern int	execle(), fork(), pipe(), close(), fcntl(), getopt();
extern struct tm	*localtime();
extern FILE	*popen();

/* uucp functions and subroutine */
extern int	anlwrk(), iswrk(), gtwvec();		/* anlwrk.c */
extern void	chremdir(), mkremdir();			/* chremdir.c */
extern		void toCorrupt();			/* cpmv.c  */
extern		int xcp(), xmv();			/* cpmv.c  */

extern int	getargs();				/* getargs.c */
extern void	bsfix();				/* getargs.c */
extern char	*getprm();				/* getprm.c */

extern void	logent(), syslog(), closelog();		/* logent.c */
extern time_t	millitick();				/* logent.c */

extern char	*protoString();				/* permission.c */
extern		logFind(), mchFind();			/* permission.c */
extern		chkperm(), chkpth();			/* permission.c */
extern		cmdOK(), switchRole();			/* permission.c */
extern		callBack(), requestOK();		/* permission.c */
extern void	myName();				/* permission.c */

extern void	systat();				/* systat.c */
extern int	ulockf(), checkLock(), delock();	/* ulockf.c */
extern int	mlock();				/* ulockf.c */
extern void	rmlock(), ultouch();			/* ulockf.c */
extern char	*timeStamp();				/* utility.c */
extern void	assert(), errent();			/* utility.c */
extern void	uucpname();				/* uucpname.c */
extern int	versys();				/* versys.c */
extern void	xuuxqt(), xuucico();			/* xqt.c */

#ifdef ATTSV
unsigned	sleep();
void	exit(), setbuf();
long	ulimit();
#else
int	sleep(), exit(), setbuf(), ftime();
#endif

#ifndef NOUSTAT
#ifdef V7USTAT
struct  ustat {
	daddr_t	f_tfree;	/* total free */
	ino_t	f_tinode;	/* total inodes free */
};
#else
#include <ustat.h>
#endif
#endif

#ifdef UNAME
#include <sys/utsname.h>
#endif

#ifdef BSD4_2
char *gethostname();
#endif

/* messages */
extern char *Ct_OPEN;
extern char *Ct_WRITE;
extern char *Ct_READ;
extern char *Ct_CREATE;
extern char *Ct_ALLOCATE;
extern char *Ct_LOCK;
extern char *Ct_STAT;
extern char *Ct_CHOWN;
extern char *Ct_CHMOD;
extern char *Ct_LINK;
extern char *Ct_CHDIR;
extern char *Ct_UNLINK;
extern char *Wr_ROLE;
extern char *Ct_CORRUPT;
extern char *Ct_FORK;
extern char *Ct_CLOSE;
extern char *Fl_EXISTS;
extern char *Ue_BADSYSTEM;
extern char *Ue_TIME_WRONG;
extern char *Ue_SYSTEM_LOCKED;
extern char *Ue_NO_DEVICE;
extern char *Ue_DIAL_FAILED;
extern char *Ue_LOGIN_FAILED;
extern char *Ue_SEQBAD;
extern char *Ue_BAD_LOG_MCH;
extern char *Ue_WRONG_MCH;
extern char *Ue_LOCKED_DEVICE;
extern char *Ue_ASSERT_ERROR;
extern char *Ue_CANT_ACCESS_DEVICE;
extern char *Ue_DEVICE_FAILED;
