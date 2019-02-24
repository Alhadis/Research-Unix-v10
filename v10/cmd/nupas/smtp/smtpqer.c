/*
 * smtpqer -- put mail (on stdin) in an smtpq subdirectory, with a shell script
 *            to send it via smtp
 */

#define USAGE "usage: smtpqer [-n] [-H  helohost] [-d domain] [-K] [-a toaddr] [-u] from tohost to...\n"
#define DIRDOMLEVEL 2
#define SPOOLNAMSIZ 12	/* keep spool dir names no longer (12 allows L.name on v9) */
#include <stdio.h>
#include <signal.h>
#include "string.h"
#include "mail.h"
#include <ctype.h>
#include <sys/stat.h>

/* globals */
char datafile[1024];
int debug = 0;

void bomb();

/* imports (other than in .h's) */
extern spoolsubdir[];
extern void exit(), cleanlocks();
extern FILE* lockopen();
extern char *sysname_read();

/* interrupt handling */
SIGRETURN
catcher(s)
	int s;
{
	bomb("interrupted\n");
}

main(argc, argv)
int argc;
char *argv[];
{
	register int c;
	int errflg=0;
	extern int optind;
	extern char *optarg;
	char *p;
	char *prefix;
	char *domain=0;
	char *sys=0;
	int unixformat=0;
	int gateway=0;
	int helo=0;
	int norun=0;
	char *helohost=0;
	char *gatehost=0;
	char *toaddr=0;
	char *from=0;
	char *host=0;
	char *spooldir;
	string *to=s_new();

	umask(2);

	signal(SIGHUP, catcher);
	signal(SIGINT, catcher);
	while ((c=getopt(argc, argv, "H:d:ua:nD")) != EOF)
		switch(c) {
		case 'H':	helohost=optarg;	break;
		case 'd':	domain=optarg;		break;
		case 'u':	unixformat++;		break;
		case 'a':	toaddr=optarg;		break;
		case 'n':	norun++;		break;
		case 'D':	debug++;		break;
		case '?':
		default:
				errflg++;		break;
		}
	if (errflg || (argc - optind) < 3)
		bomb(USAGE);
	if(*argv[optind]==0 || *argv[optind+1]==0 || *argv[optind+2]==0)
		bomb(USAGE);
	if (helohost==NULL)
		helohost=s_to_c(s_copy(sysname_read()));

	from=argv[optind++];
	host=argv[optind++];
	for (; optind < argc; optind++){
		s_append(to, argv[optind]);
		s_append(to, " ");
	}

	/*
	 *  make spool files:
	 * 	C.xxxxxxxxxxxx	- the control file
	 * 	D.xxxxxxxxxxxx	- the data file
	 */
	if(gotodir(host)<0)
		bomb("going to spool directory %s\n", spoolsubdir);
	makedata(from, domain, s_to_c(to));
	makectl(unixformat, helohost, domain, from, toaddr, host, s_to_c(to));

	/*
	 *  run the queue for the receiver
	 */
	if(!norun)
		smtpsched("Qsmtpsched", spoolsubdir);

	exit(0);
}

/*
 *  the data file is pre-converted to rfc822
 */
makedata(from, domain, to)
	char *from;
	char *domain;
	char *to;
{
	int fd;
	FILE *fp;

	/*
	 *  create data file
	 */
	strcpy(datafile, "D.xxxxxxxxxxxx");
	fd = mkdatafile(datafile);
	if(fd<0)
		bomb("creating spool file\n");
	fp = fdopen(fd, "w");

	/*
	 *  copy data
	 */
	clearerr(fp);
	clearerr(stdin);
	copymsg(stdin, fp);
	fflush(fp);

	/*
	 *  make sure it worked
	 */
	if(ferror(fp) || ferror(stdin)){
		unlink(datafile);
		bomb("writing data file");
	}
	fclose(fp);
}

/*
 *  just copy input to output
 */
copymsg(in, out)
	FILE *in;
	FILE *out;
{
	char buf[4096];
	int n;

	while(n=fread(buf, 1, sizeof(buf), in))
		if(fwrite(buf, 1, n, out)!=n)
			bomb("writing data file");
}

/*
 *  Make a control file.  The two lines contain:
 *	<reply-addr>	<dest>
 * 	-H <hello host> -d <domain> <reply_addr> <dest> <recipients>
 */
makectl(unixformat, helo, domain, from, daddr, dest, to)
	char *from, *dest, *to;
{
	string *msg = s_new();

	s_append(msg, from);
	s_append(msg, " ");
	s_append(msg, dest);
	s_append(msg, "\n");
	if(unixformat)
		s_append(msg, "-u ");
	if(domain){
		s_append(msg, "-d ");
		s_append(msg, domain);
		s_append(msg, " ");
	}
	if(daddr){
		s_append(msg, "-a ");
		s_append(msg, daddr);
		s_append(msg, " ");
	}
	s_append(msg, "-H ");
	s_append(msg, helo);
	s_append(msg, " ");
	s_append(msg, from);
	s_append(msg, " ");
	s_append(msg, dest);
	s_append(msg, " ");
	s_append(msg, to);
	s_append(msg, "\n");
	if(mkctlfile('C', datafile, s_to_c(msg))<0)
		bomb("creating control file\n");
}

void
bomb(msg, a1, a2, a3, a4)
	char *msg;
{
	fprintf(stderr, "smtpqer: ");
	fprintf(stderr, msg, a1, a2, a3, a4);
	if (datafile[0]!=0)
		unlink(datafile);
	exit(1);
}
