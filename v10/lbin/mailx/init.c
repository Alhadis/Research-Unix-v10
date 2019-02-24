#ident "@(#)init.c	1.4 'attmail mail(1) command'"
#ident	"@(#)mailx:init.c	1.1"
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident "@(#)init.c	2.24 'attmail mail(1) command'"

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * A bunch of global variable declarations lie herein.
 *
 * All global externs are declared in def.h. All variables are initialized
 * here!
 *
 * !!!!!IF YOU CHANGE (OR ADD) IT HERE, DO IT THERE ALSO !!!!!!!!
 *
 */

#include	"rcv.h"
#include	<grp.h>
#include	<pwd.h>
#include	<utmp.h>

struct hdr header[] = {
		"",				FALSE,
		"Auto-Forward-Count:",		FALSE,
		"Auto-Forwarded-From:",		FALSE,
		"Content-Length:",		TRUE,
		"Content-Type:",		FALSE,
		"Date:",			TRUE,
		"Default-Options:",		FALSE,
		"End-of-Header:",		FALSE,
		"From ",			TRUE,
		">From ",			TRUE,
		"From:",			TRUE,
		"MTS-Message-ID:",		FALSE,
		"Message-Type:",		FALSE,
		"Message-Version:",		FALSE,
		"Message-Service:",		TRUE,
		"Received:",			FALSE,
		"Report-Version:",		FALSE,
		"Status:",			FALSE,
		"Subject:",			TRUE,
		"To:",				TRUE,
		">To:",				FALSE,
		"Transport-Options:",		FALSE,
		"UA-Content-ID:",		FALSE,
		/*Dummy place holders for H_DAFWDFROM,*/
		/*H_DTCOPY and H_RECEIVED. Should */
		/* match above first...*/
		"Auto-Forwarded-From:",		FALSE,
		">To:",				FALSE,
		"Received:",			FALSE,
};

char	binmsg[] = "*** Message content is not printable: delete, write or save it to a file ***";

NODE	*fplist = NOFP;
NODE	*curptr = NOFP;
gid_t	myrgid;
gid_t	myegid;
uid_t	myruid;
uid_t	myeuid;
int	msgCount;			/* Count of messages read in */
pid_t	mypid;				/* Current process id */
int	rcvmode;			/* True if receiving mail */
int	sawcom;				/* Set after first command */
int	Fflag = 0;			/* -F option (followup) */
int	hflag;				/* Sequence number for network -h */
int	Hflag = 0;			/* print headers and exit */
char	*rflag;				/* -r address for network */
char	*Tflag;				/* -T temp file for netnews */
int	UnUUCP = 0;			/* -U flag */
int	exitflg = 0;			/* -e for mail test */
int	newsflg = 0;			/* -I option for netnews */
char	nosrc;				/* Don't source /etc/mail/mailx.rc */
char	noheader;			/* Suprress initial header listing */
int	selfsent;			/* User sent self something */
int	senderr;			/* An error while checking */
int	edit;				/* Indicates editing a file */
int	readonly;			/* Will be unable to rewrite file */
int	noreset;			/* String resets suspended */
int	sourcing;			/* Currently reading variant file */
int	loading;			/* Loading user definitions */
int	cond;				/* Current state of conditional exc. */
FILE	*itf;				/* Input temp file buffer */
FILE	*otf;				/* Output temp file buffer */
FILE	*pipef;				/* Pipe file we have opened */
int	image;				/* File descriptor for image of msg */
FILE	*input;				/* Current command input file */
char	*editfile;			/* Name of file being edited */
char	*sflag;				/* Subject given from non tty */
int	outtty;				/* True if standard output a tty */
int	intty;				/* True if standard input a tty */
int	baud;				/* Output baud rate */
char	homedir[PATHSIZE];		/* Name of home directory */
char	mailname[PATHSIZE];		/* Name of system mailbox */
char	origname[PATHSIZE];		/* Name of mailfile before expansion */
uid_t	uid;				/* The invoker's user id */
char	myname[PATHSIZE];			/* My login id */
off_t	mailsize;			/* Size of system mailbox */
int	lexnumber;			/* Number of TNUMBER from scan() */
char	lexstring[STRINGLEN];		/* String from TSTRING, scan() */
int	regretp;			/* Pointer to TOS of regret tokens */
int	regretstack[REGDEP];		/* Stack of regretted tokens */
char	*stringstack[REGDEP];		/* Stack of regretted strings */
int	numberstack[REGDEP];		/* Stack of regretted numbers */
struct	message	*dot;			/* Pointer to current message */
struct	message	*message;		/* The actual message structure */
struct	var	*variables[HSHSIZE];	/* Pointer to active var list */
struct	grouphead	*groups[HSHSIZE];/* Pointer to active groups */
struct	ignore		*ignore[HSHSIZE];/* Pointer to ignored fields */
char	**altnames;			/* List of alternate names for user */
int	debug;				/* Debug flag set */
int	rmail;				/* Being called as rmail */
char	*prompt = NOSTR;		/* prompt string */
int	space;				/* Current maximum number of messages */
int	maxfiles;			/* Maximum number of open files */

jmp_buf	srbuf;
int	ismail = TRUE;		/* default to program=mail */

#ifdef	USR_SPOOL_MAIL
char	maildir[] = "/usr/spool/mail/";	/* directory for mail files */
#else
# ifdef preSVr4
char	maildir[] = "/usr/mail/";	/* directory for mail files */
# else
char	maildir[] = "/var/mail/";	/* directory for mail files */
# endif
#endif

char	*progname;	/* program name (argv[0]) */
int	sending;	/* TRUE==>sending mail; FALSE==>printing mail */
int	askme;
char	tempMail[14];
char	tempQuit[14];
char	tempEdit[14];
char	tempSet[14];
char	tempResid[PATHSIZE];		/* temp file in :saved || adb: put it back in /tmp */
char	tempMesg[14];
char	tempZedit[14];
static struct utimbuf	utimeb;
struct utimbuf	*utimep = &utimeb;

/*
 * The pointers for the string allocation routines,
 * there are NSPACE independent areas.
 * The first holds STRINGSIZE bytes, the next
 * twice as much, and so on.
 */

struct strings stringdope[NSPACE];
