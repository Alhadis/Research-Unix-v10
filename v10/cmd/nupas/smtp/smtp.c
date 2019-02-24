/*
 * smtp -- client, send mail to remote smtp server
 * TODO:
 *	allow partial delivery to multiple recipients when only some
 *		fail (maybe)
 * 	send stuff from cmds.h instead of hard-coded here
 */

#define	USAGE "usage: %s [-u] [-H helohost] [-d domain] [-a addr] [-f] [-D] [-L loglevel] sender targethost recip1 recip2 ...\n"

#include <stdio.h>
#include <ctype.h>
#include <sysexits.h>
#include "addrformat.h"
#include "smtp.h"
#include "string.h"
#include "aux.h"
#include "sys.h"

#ifndef DIALER
#define DIALER		"tcp"
#endif

#ifndef SERVNAME
#define SERVNAME	"smtp"		/* service we wanna talk to */
#endif

char *progname;
int debug;
char *helohost;
int kludgepause = 0;

char *strcat(), *strcpy();
extern char *convertaddr();
extern int ipcdebug;
extern char *ipcpath();

static int errno;
static char errstr[128];

char *convertto();

/*
 * main - parse arguments and handle options
 */
main(argc, argv)
int argc;
char *argv[];
{
	register int c;
	int errflg = 0;
	int unixformat = 0;
	int filter = 0;
	char *domain = 0;
	char *sender = 0;
	char *host = 0;
	char *addr = 0;
	namelist *recips, *newname(), *appendname();
	FILE *sfi, *sfo;
	string *replyaddr=s_new();
	string *hh;

	extern int optind;
	extern char *optarg;

	umask(2);

	progname = argv[0];
	Openlog("smtp", LOG_PID, LOG_SMTP);
	setlogmask(LOG_UPTO(DEFAULT_LOG_LEVEL));
	while ((c = getopt(argc, argv, "ga:uDd:H:fL:K")) != EOF)
		switch (c) {
		case 'a':	addr = optarg;		break;
		case 'u':	unixformat++;		break;
		case 'D':	debug++;  ipcdebug++;	break;
		case 'd':	domain = optarg;	break;
		case 'H':	helohost = optarg;	break;
		case 'f':	filter++;		break;
		case 'K':	kludgepause++;		break;
		case 'L':	setloglevel(optarg);	break;
		case '?':
		default:
			errflg++;
			break;
		}
	if (errflg || (argc - optind) < 3) {
		(void) fprintf(stderr, USAGE, progname);
		Syslog(LOG_WARNING, "SMTP illegal usage.");
		bomb(EX_USAGE);
	}

	/*
	 *  figure out what to call ourselves
	 */
	if (helohost==NULL)
		helohost=s_to_c(s_copy(sysname_read()));

	/*
	 *  if there is no domain in the helo host name
	 *  and we the -d option is specified, domainify
	 *  the helo host
	 */
	if(strchr(helohost, '.')==0 && domain){
		hh = s_copy(helohost);
		s_append(hh, domain);
		helohost = s_to_c(hh);
	}

	/*
	 *  put our address onto the reply address
	 */
	if(strchr(argv[optind], '!')==0 || !domain){
		s_append(replyaddr, helohost);
		s_append(replyaddr, "!");
		s_append(replyaddr, argv[optind]);
	} else {
		s_append(replyaddr, argv[optind]);
	}
	optind++;

	/*
	 *  convert the arguments to 822 form
	 */
	sender = convertaddr(s_to_c(replyaddr), domain, SOURCEROUTE);
	host = argv[optind++];
	recips = newname(convertto(argv[optind++], unixformat, host));
	for (; optind < argc; optind++)
		recips = appendname(recips, convertto(argv[optind], unixformat, host));

	/*
	 * run as a filter
	 */
	if ( filter ) {
		do_data(unixformat, stdin, stdout, sender, recips, domain);
		exit(0);
	}

	/*
	 *  open connection
	 */
	setup(addr ? addr : host, &sfi, &sfo);

	/*
	 *  hold the conversation
	 */
	converse(unixformat, sender, recips, domain, sfi, sfo, stdin);
	/* converse terminates with the appropriate exit code */
}

namelist *
newname(s)
	char *s;
{
	namelist *np;

	np = (namelist *)malloc(sizeof(namelist));
	if (np == NULL) {
		Syslog(LOG_WARNING, "could not alloc (newname)");
		bomb(1);
	}
	np->name = s;
	np->next = NULL;
	return np;
}

/* could add at beginning, but let's maintain original order */
namelist *
appendname(nl, s)
	char *s;
	namelist *nl;
{
	register namelist *tl;

	if (nl == NULL)
		bomb(1);	/* shouldn't happen */
	for (tl=nl; tl->next!=NULL; tl=tl->next)
		;
	tl->next = newname(s);
	return nl;
}

/*
 *  convert a destination address to outgoing format
 *
 *	if unixformat, just leave it alone
 *
 *	if not add the destination host name.
 */
char *
convertto(recip, unixformat, desthost)
	char *recip;
	char *desthost;
{
	static string *buf;

	if(unixformat)
		return recip;
	
	buf = s_reset(buf);
	s_append(buf, desthost);
	s_append(buf, "!");
	s_append(buf, recip);
	return convertaddr(s_to_c(buf), 0, SOURCEROUTE);
}


/*
 * setup -- setup tcp/ip connection to/from server
 */
setup(host, sfip, sfop)
	char *host;
	FILE **sfip, **sfop;
{
	int s;
	char *path;
	int localerr;

	path = ipcpath(host, DIALER, SERVNAME);
	Syslog(LOG_DEBUG, "Opening connection to %s\n", path);
	if ((s = ipcopen(path, "")) < 0) {
		extern int ipcerrno;
		extern char syserrstr[];

		char errbuf[256];
		sprintf(errbuf, "SMTP connect error to %s", host);
		ipcperror(errbuf);
		Syslog(LOG_INFO, "%s: %s\n", errbuf, syserrstr);
		bomb(ipcerrno);
	}

	if (((*sfip = fdopen(s, "r")) == (FILE *) NULL) ||
	    ((*sfop = fdopen(s, "w")) == (FILE *) NULL)) {
		perror("setup - fdopen");
		Syslog(LOG_INFO, "setup - fdopen");
		bomb(EX_IOERR);
	}
}


/*
 * bomb(code) - exit program, map smtp error code into mailsystem code
 * Codes with EX_ are from <sysexits.h>
 * Lines with FOO are placeholders until we decrypt more appropriate codes.
 */
bomb(code)
int code;
{
	switch(code) {
	case 451:			/* some temporary error */
		exit(EX_TEMPFAIL);	/* try later */
		/*NOTREACHED*/
	case 554:			/* syntax error in address */
	case 501:			/* data format error */
		exit(EX_DATAERR);
		/*NOTREACHED*/
	case 550:			/* no such user */
		exit(EX_NOUSER);
		/*NOTREACHED*/
	case EX_USAGE:
	case EX_DATAERR:
	case EX_NOINPUT:
	case EX_NOUSER:
	case EX_NOHOST:
	case EX_UNAVAILABLE:
	case EX_SOFTWARE:
	case EX_OSERR:
	case EX_OSFILE:
	case EX_CANTCREAT:
	case EX_IOERR:
	case EX_TEMPFAIL:
	case EX_PROTOCOL:
	case EX_NOPERM:
		exit(code);
		/*NOTREACHED*/
	default:			/* can't happen? */
		if ((code >= 400) && (code <= 499))
			exit(EX_TEMPFAIL);
		else {
			Syslog(LOG_WARNING, "SMTP protocol error %d\n", code);
			exit(EX_PROTOCOL);	/* unknown error */
		}
	}
}
