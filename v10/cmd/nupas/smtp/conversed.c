#ifndef lint
static char *sccsid = "@(#)converse.c	1.9 87/07/31";
#endif lint
/*  Copyright 1984 Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this program
for any purpose and without fee is hereby granted, provided
that this copyright and permission notice appear on all copies
and supporting documentation, the name of M.I.T. not be used
in advertising or publicity pertaining to distribution of the
program without specific prior permission, and notice be given
in supporting documentation that copying and distribution is
by permission of M.I.T.  M.I.T. makes no representations about
the suitability of this software for any purpose.  It is pro-
vided "as is" without express or implied warranty.		*/

/*
 *  I've raped this code severely.  Bugs could be
 *  MIT's or mine. -- presotto
 *
 *  Me too.  -- ches
 */

/*
 * smtpd - World's most trivial SMTP server.  Only accepts the MAIL, FROM,
 * RCPT, and DATA commands.  Generates a data file for the mail
 * daemon and kicks the mail daemon off.
 */

#include <stdio.h>
#include <signal.h>
#include <ctype.h>

#include <fcntl.h>
#include <string.h>
#include <sysexits.h>

#include "mail.h"
#include "smtp.h"
#include "cmds.h"
#include "string.h"

/* fundamental constants */
#define TRUE 1
#define FALSE 0
#define SECONDS		1
#define MINUTES		60
#define HOURS		(60 * MINUTES)

/* tunable constants */
#define	SHORTTIME	(5 * MINUTES)	/* enough time for easy stuff */
#define	LONGTIME	(2 * HOURS)	/* max time, DATA to `.' */


static string *rcvrs;

FILE	*datafd;			/* data file descriptor */
FILE	*fi;				/* fd from remote host */
FILE	*fo;				/* fd to remote host */

char	dataname[NAMSIZ], rcptto[BUFSIZ];		/* data file name */

typedef int event;

static void terminate();

extern char *convertaddr();
extern char *UPASROOT;
extern int death();
extern int alarmsend();
extern char *helohost;
extern char *thishost;
extern int queuing;
extern int norun;
extern char *spoolsubdir[];

int	n_rcpt = 0;
long	nbytes = 0;
int	virus = 0;

static char mailfrom[BUFSIZ];
static char *fromaddr;


#ifndef NOFILE
#define NOFILE 32
#endif /*NOFILE*/

SIGRETURN
alarmtr(s)
	int s;
{
	Syslog(LOG_INFO, "timed out");
	death(EX_TEMPFAIL);
}

/*
 * This is the routine which processes incoming smtp commands from the
 * user.  It goes to sleep awaiting network input.  When a complete
 * command is received, the tcp receiver task awakens us to process it.
 * Currently only the commands listed in the command table are accepted.
 * This routine never returns.
 */
converse(rfi, rfo)
	FILE *rfi, *rfo;
{
	char greeting[MAXSTR];

	fo = rfo;
	fi = rfi;

	(void) signal(SIGALRM, alarmtr);
	(void) alarm(SHORTTIME);	/* make sure we eventually go away */
	(void) sprintf(greeting, "220 %s SMTP\n", helohost);
	csend(LOG_DEBUG, greeting);

	do_helo(fi, fo);		/* wait for the hello */

	/*
	 *  avoid annoying interuptions
	 */
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);

	for (;;) {			/* until QUIT */
		n_rcpt = 0;
		rcvrs = s_reset(rcvrs);
		*dataname = *rcptto = 0;
		fromaddr = 0;
		if (!do_mail(fi, fo))
			continue;	/* wait for the mail command */
		while (do_rcpt(fi, fo))	/* do all the recipients */
			n_rcpt++;
		(void) alarm(LONGTIME);
		do_data(fi, fo);	/* do the data */
	}
}

/*
 *  Wait for the user to send the HELO command.  Punt out if he sends
 *  QUIT or RSET.
 *
 *  The spooling directory depends on the calling host.  The host name
 *  is used to connect to the appropriate spool directory.
 */
do_helo(fi, fo)
FILE *fi, *fo;
{
	char	cmdbuf[MAXSTR];
	char	greeting[MAXSTR], *nlptr;
	int	buflen;
	char	*hp;
	char	*parse_hello();

	for (;;) {		/* until HELO, or QUIT */
		buflen = crecv(cmdbuf, sizeof cmdbuf, fi);	/* wait for command */
		switch (cmdparse(cmdbuf, buflen)) {
		case QUIT:
			quit(fi, fo);
		case RSET:
		case NOOP:
			csend(LOG_DEBUG, "250 OK\n");
			continue;
		case HELO:
			hp = parse_hello(cmdbuf, sizeof(cmdbuf));
			Syslog(LOG_DEBUG, "HELO from %s", hp);
			if(gotodir(hp)<0){	
				csend(LOG_ALERT, "451 Transaction failed -- Could not access spool directory.\n");
				death(EX_OSERR);
			}
			(void) sprintf(greeting, "250 %s\n", helohost);
			csend(LOG_DEBUG, greeting);
			return;
		case DEBG:
			Syslog(LOG_ALERT, "DEBUG attempt");
			csend(LOG_DEBUG, "200 OK\n");
			virus = 1;
			continue;
		case NONE:
			bitch(cmdbuf, fo);
			continue;
		default:
			csend(LOG_DEBUG, "503 Expecting HELO\n");
			continue;
		}
	}
}

/*
 * Wait for the user to send the MAIL command.  Punt out if he sends
 * QUIT.  Return false if he said RSET, so we can start over.
 */
do_mail(fi, fo)
FILE *fi, *fo;
{
	char	cmdbuf[MAXSTR];
	char	gripe[MAXSTR], *nlptr;
	int	buflen;

	for (;;) {		/* until MAIL, QUIT, or RSET */
		buflen = crecv(cmdbuf, sizeof cmdbuf, fi);	/* wait for command */
		switch (cmdparse(cmdbuf, buflen)) {
		case QUIT:
			quit(fi, fo);
		case NOOP:
			csend(LOG_DEBUG, "250 OK\n");
			continue;
		case MAIL:
			strcpy(mailfrom, cmdbuf);
			csend(LOG_DEBUG, "250 OK\n");
			return(TRUE);
		case DEBG:
			Syslog(LOG_ALERT, "DEBUG attempt");
			csend(LOG_WARNING, "200 OK\n");
			virus = 1;
			continue;
		case VRFY:
			csend(LOG_INFO, "252 Cannot VRFY user\n");
			continue;
		case NONE:
			bitch(cmdbuf, fo);
			continue;
		case RSET:
			csend(LOG_DEBUG, "250 OK\n");
			return(FALSE);
		default:
			csend(LOG_DEBUG, "503 Expecting MAIL\n");
			continue;
		}
	}
}

/*
 * Wait for the user to send the RCPT command.  Punt out if he sends
 * QUIT or RSET.  Returns TRUE if a RCPT command was received, FALSE
 * if a DATA command was received.
 */
do_rcpt(fi, fo)
FILE *fi, *fo;
{
	char	cmdbuf[MAXSTR];
	char	gripe[MAXSTR], *nlptr;
	int	buflen;
	int	i;

	for (;;) {		/* until RCPT, DATA, QUIT, or RSET */
		buflen = crecv(cmdbuf, sizeof cmdbuf, fi);	/* wait for command */
		switch (cmdparse(cmdbuf, buflen)) {
		case QUIT:
			quit(fi, fo);
		case NOOP:
			csend(LOG_DEBUG, "250 OK\n");
			continue;
		case RCPT:
			if (!parse_rcpt(cmdbuf, buflen)) {
				csend(LOG_DEBUG, "501 Syntax error in recipient name\n");
				continue;
			}
			csend(LOG_DEBUG, "250 OK\n");
			return(TRUE);
		case DATA:
			if (*s_to_c(rcvrs) == 0) {
				csend(LOG_DEBUG, "503 Expecting RCPT\n");
				continue;
			}
			if ((i=init_xfr()) < 0) {	/* set up data file */
				char buf[100];
				sprintf(buf, "451 Can't initialize files in spool directory  %s(%d)\n",
					dataname, i);
				csend(LOG_ALERT, buf);
				death(EX_CANTCREAT);
			}
			csend(LOG_DEBUG, "354 Start mail input; end with <CRLF>.<CRLF>\n");
			return(FALSE);
		case VRFY:
			csend(LOG_INFO, "252 Cannot VRFY user\n");
			continue;
		case DEBG:
			Syslog(LOG_ALERT, "DEBUG attempt");
			csend(LOG_WARNING, "200 OK\n");
			virus = 1;
			continue;
		case RSET:  /* this code doesn't handle this here.  Feign ignorance. */
		case NONE:
			bitch(cmdbuf, fo);
			continue;
		default:
			csend(LOG_DEBUG, "503 Expecting RCPT or DATA\n");
			continue;
		}
	}
}

/*
 *  input a line at a time till a <cr>.<cr>.  return the count of the characters
 *  input.  if EOF is reached, return -1.  if <cr>.<cr> is reached, return 0.
 */
static int atend;		/* true when <cr>.<cr> is reached */

char *
smfgets(buf, len, fi)
	char *buf;
	int len;
	FILE *fi;
{
	int n;
	int i;

	if(atend)
		return NULL;
	n = tgets(buf, len, fi);
	if (n < 0)
		return NULL;
	if (buf[0] == '.') {
		if(buf[1] == '\n'){
			atend = 1;
			return NULL;
		} else if(buf[1] == '.'){
			for(i=1; i<=n; i++)
				buf[i-1] = buf[i];
		}
	}
	nbytes += n;
	return buf;
}

do_data(fi, fo)
FILE *fi, *fo;
{
	string *cc;
	int pid, wpid;
	char gripe[MAXSTR];
	char cmd[MAXSTR];
	char ctlfile[MAXSTR];
	int ac, i;
	char *cp;

	clearerr(fi);
	clearerr(datafd);

	/*
	 *  read data message
	 */
	atend = nbytes = 0;
	from822(thishost, smfgets, fi, datafd, fromaddr, helohost);
	fflush(datafd);
	if(ferror(datafd) || ferror(fi)){
		fclose(datafd);
		unlink(dataname);
		csend(LOG_ALERT, "451 Transaction failed -- error writing data file\n");
		death(EX_IOERR);
	}
	fclose(datafd);

	/*
	 *  create a control file.  the two lines are
	 *	<reply-address> <recipients>
	 *	<recipients>
	 */
	cc = s_new();
	if (fromaddr == (char *)0 || *fromaddr == '\0')
		fromaddr = "postmaster";
	s_append(cc, fromaddr);
	s_append(cc, " ");
	s_append(cc, s_to_c(rcvrs));
	s_append(cc, "\n");
	s_append(cc, s_to_c(rcvrs));
	s_append(cc, "\n");
	if(mkctlfile('X', dataname, s_to_c(cc))<0){
		unlink(dataname);
		csend(LOG_ALERT, "451 Transaction failed -- can't make control file\n");
		death(EX_CANTCREAT);
	}
	s_free(cc);
	csend(LOG_DEBUG, "250 OK\n");
	Syslog(LOG_INFO, "%s  sent %d bytes to  %s\n",
		fromaddr ? fromaddr : "postmaster", nbytes,
		s_to_c(rcvrs));
	

	/*
	 *  reinitialize all the data pointers
	 */
	rcvrs = s_reset(rcvrs);
	nbytes = 0;
	*dataname = *rcptto = 0;
	fromaddr = 0;

}

/*
 * Create the data file for the transfer.  Get unique
 * names and create the files.
 */
init_xfr()
{
	int	dfd;			/* file desc. for data file */
	char	*cp;

	strcpy(dataname, "D.xxxxxxxxxxxx");
	if((dfd = mkdatafile(dataname)) < 0)
		return -1;
	datafd = fdopen(dfd, "w");	/* make stdio descriptor */
	if (datafd == NULL)
		return -2;

	/*
	 *  find the sender name if any
	 */
	if(*mailfrom){

		/* skip noise */
		for(cp=mailfrom+sizeof("MAIL FROM:")-1; *cp; cp++)
			if(strchr(";<>{}()\n| \t", *cp)==NULL)
				break;
		fromaddr = cp;

		/* find address */
		for(; *cp; cp++)
			if(strchr(";<>{}()\n| \t", *cp)!=NULL){
				*cp = '\0';
				break;
			}
	}
	if(fromaddr)
		fromaddr = convertaddr(fromaddr);
	
	return 1;
}

/*
 * Give up on the transfer.  Unlink the data file (if any),
 * close the tcp connection, and exit.
 */
quit(fi, fo)
FILE *fi, *fo;
{
	int i;
	char greeting[MAXSTR];

	(void) sprintf(greeting, "221 %s Terminating\n", helohost);
	csend(LOG_DEBUG, greeting);
	(void) fclose(fi);
	(void) fclose(fo);
	Syslog(LOG_DEBUG, "finished.\n");

	/*
	 *  run the queue from this caller
	 */
	for(i=0; i<NOFILE; i++)
		close(i);
	open("/dev/null", 0);
	open("/dev/null", 1);
	open("/dev/null", 1);
	if(!norun)
		smtpsched("Dsmtpsched", spoolsubdir);

	exit(0);
}

/*
 * Parse the command part off the specified buffer.  Return the strchr
 * of the command in the command table(or 0 if the command is not
 * recognized).
 * The commands and indices accepted are listed in the include file
 * "cmds.h".
 * If the len parameter is -1 (as returned by tgets), issue the QUIT command.
 * This non-protocol extension was added to cool the jets of sail.stanford.edu.
 */
cmdparse(buf, len)
char *buf;
int len;
{
	register char *cmdp, *bufp;	/* command, buffer ptrs. */
	register struct	cmdtab	*ct;	/* cmd table ptr */
	register int i;			/* strchr in cmd table */
	int	clen;			/* length of this command */
	
	if (len == -1) {		/* EOF */
		buf = "QUIT";
		len = strlen(buf);
	}
	for (ct = &cmdtab[1], i = 1; ct->c_name != NULL; ct++, i++) {
		clen = ct->c_len;
		if (len < clen)		/* buffer shorter than command? */
			continue;
		/* case-insensitive matching of command names */
		for (cmdp = ct->c_name, bufp = buf;
		     clen > 0 && *cmdp == (ISLOWER(*bufp) ? TOUPPER(*bufp) : *bufp);
		     cmdp++, bufp++, clen--)
			;
		if (clen == 0) {		/* success */
			/* sendmail compatibility */
			if (i == ONEX || i == VERB)
				i = NOOP;
			return i;
		}
	}
	return 0;
}

/*
 *  Parse a hello and return a pointer to name of the last two elements
 *  of the calling machine's domain name (or last 14 chars).
 */
char *
parse_hello(buf, len)
	char *buf;
	int len;
{
	char *bp = buf;
	char *lp;
	int dots;

	/* skip command */
	bp[len-1] = 0;
	for(; *bp && !isspace(*bp); bp++)
		;
	/* skip white */
	for(; isspace(*bp); bp++)
		;
	/* skip arg */
	lp = bp;
	for(; *bp && !isspace(*bp); bp++)
		;
	/* null terminate */
	*bp = 0;

	return lp;
}

static	char	*to;			/* ptr. into request buffer */

/*
 * Parse the recipient spec in the buffer.  Start by stripping the
 * command off the front of the buffer.  Then call canon() to convert
 * the recpient name into a format acceptable to the mailer daemon
 * (ie. !-format).
 * Returns TRUE if parsed successfully, FALSE otherwise.
 */
/* ARGSUSED len */
parse_rcpt(buf, len)
char *buf;				/* command buffer */
int len;				/* size of buffer string */
{
	register char *from;		/* ptr to recipient name */
	char *end;
	char *rv;
	char *sysname_read();
	char *thissys;
	
	from = &buf[cmdtab[RCPT].c_len];
	while (*from == ' ' || *from == '\t')
		from++;
	if (*from == '<') {
		end = strchr(from++, '>');
		if (end == 0) {
			return FALSE;
		}
		*end = 0;
	}

	/*
	 *  convert to lower case (this is wrong but rfc822 is case
	 *  insensitive)
	 */
	for(rv = from; *rv; rv++)
		if(isupper(*rv))
			*rv = tolower(*rv);

	/*
	 * convert address to bang format.  Assume the first site
	 * in the list is us and take it out.
	 */
	rv=convertaddr(from);
	if(end=strchr(rv, '!')){
		thissys = sysname_read();
		*end = '\0';
		if(strcmp(rv, thissys)==0)
			rv = end+1;
		else
			*end = '!';
	}

	/*
	 *  check for address syntax
 	 */
	if(shellchars(rv)){
		Syslog(LOG_ALERT, "shell characters in address: %s", rv);
		if(virus)
			rv = "upas.security";
		else
			return FALSE;
	}

	/*
	 *  add to list of recipients
	 */
	if(*s_to_c(rcvrs))
		s_append(rcvrs, " ");
	s_append(rcvrs, rv);
	return TRUE;
}


/* Time to live elapsed or io error. */
death(weapon)
{
	(void) unlink(dataname);
	exit(1);
}

alarmsend()
{
	csend(LOG_WARNING, "451 Our mailer appears to be hung.\n");
	death(EX_TEMPFAIL);
}

funnychars(str)
register char *str;
{
	for (;;)
		switch(*str++) {
		case '^':
		case '&':
		case '>':
		case '<':
		case '`':
		case '|':
		case ';':
		case '\'':
			return TRUE;

		case 0:
			return FALSE;
		}
}

bitch(buf, fo)
char *buf;
FILE *fo;
{
	char gripe[BUFSIZ], *nlptr;

	if ((nlptr = strchr(buf, '\n')) != 0)
		*nlptr = 0;
	(void) sprintf(gripe, "502 %s ... Not recognized\n", buf);
	csend(LOG_DEBUG, gripe);
}

bomb(err)
int err;
{
	death(err);
}

csend(loglevel, message)
int loglevel;
char *message;
{
	Syslog(loglevel, "<--- %s", message);
	(void) tputs(message, fo);
}

int
crecv(buf, len, fd)
char *buf;
int len, fd;
{
	int n = tgets(buf, len, fi);
	Syslog(LOG_DEBUG, "-------> %s", buf);
	return n;
}
