#include <stdio.h>
#include "smtp.h"
#include "sys.h"

int norun;
int log;
int debug = 0;

char *progname, *helohost, *thishost;
extern char *sysname_read();


main(argc, argv)
char *argv[];
{
	int c;
	extern int optind;
	extern char *optarg;

	progname = argv[0];
	Openlog("smtpd", LOG_PID, LOG_SMTPD);
	setlogmask(LOG_UPTO(LOG_INFO));

	umask(2);

	while((c = getopt(argc, argv, "H:h:DnL:")) != EOF) {
		switch(c) {

		case 'H':	helohost = optarg;	break;
		case 'h':	thishost = optarg;	break;
		case 'L':	setloglevel(optarg);	break;
		case 'n':	norun++;
		case 'D':	debug++;		break;
		case '?':
		default:
			Syslog(LOG_CRIT, "%s: usage\n", progname);
			exit(2);
		}
	}
	if(helohost == NULL)
		helohost=sysname_read();
	converse(stdin, stdout);
	/* NOTREACHED */
	return 0;
}
