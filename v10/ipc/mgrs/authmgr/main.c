#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "auth.h"

#define MAXTRYS	3
#define DESTTRYS 8
#define TIMEOUT	(60*5)	/*seconds 'til we go away*/

char *keyfile = "/usr/guard/keys";
char *logfile = "/usr/guard/log/auth";

char line[BUFSIZE];
char logbuf[BUFSIZE*MAXTRYS + BUFSIZE*DESTTRYS + BUFSIZE] = "";
char caller[BUFSIZE];

SIG_TYP timeout();

main(argc, argv)
int argc;
char *argv[];
{
	char *chal;
	int ok, tries;
	char user[10];

	extern int optind;
	extern char *optarg;
	char c;

	while ((c = getopt(argc, argv, "k:l:")) != EOF) {
		switch (c) {
		case 'k':	keyfile = optarg;	break;
		case 'l':	logfile = optarg;	break;
		default:
			printf("auth argument error, get help!\n");
			exit(100);
		}
	}

	if (freopen(logfile, "a", stderr) == (FILE *)0) {
			printf("could not open log file, get help!\n");
			exit(100);
	}

	readkeyfile();
	chdir("/usr/guard");	/* in case of core dumps; I wanna find them */
	/*getcaller(caller);*/
	printf("\nAuthentication Server.\n\n");
	signal(SIGALRM, timeout);
	alarm(TIMEOUT);
	settty();

	for (tries=MAXTRYS, ok=FALSE; tries > 0; tries--) {
		int echo;

		printf("Id? ");
		getline(TRUE);
		strncpy(user, line, sizeof(user));
		user[sizeof(user) - 1] = '\0';
		log(line);  log(", ");
		if ((chal = getchallenge(line, "", &echo)) == NULL)
			continue;
		printf("%s", chal);
		getline(echo);
		if (responseok(line)) {
			ok = TRUE;
			break;
		}
		printf("Incorrect.\n");
	}
	if (!ok) {
		printf("Authentication failed.\n");
		dowarning("Failed");
		exit(1);
	}

	printf("\n");
	log(",   dest= ");
	alarm(TIMEOUT);
	for (tries=DESTTRYS; tries > 0; tries--) {
		printf("\nDestination? ");
		getline(TRUE);
		log(line);  log(", ");
		if (doconnect(line, user))
			exit(0);
	}
	if (!ok) {
		printf("Too many tries, bye bye.\n");
		dowarning("Destination");
		exit(1);
	}
}

SIG_TYP
timeout()
{
	printf("\n\n** Timeout **\n\n");
	dowarning("Timeout");
	exit(2);
}

/*
 * Read a useful line into `line'.
 */
getline(echo)
	int echo;
{
	char *cp;

	line[0] = '\0';

	if (!echo)
		setecho(FALSE);

	fflush(stdout);
	if(fgets(line, sizeof(line), stdin)==NULL) {
		printf("Eof\n");
		dowarning("Eof");
		exit(10);
	}
	if ((cp=strpbrk(line, "\n\r")) != NULL)
		*cp = '\0';
	if (!echo)
		setecho(TRUE);
}

/*
 * dowarning - send a warning message to stderr (for now).
 */
dowarning(mess)
	char *mess;
{
	/*
	 * WARNING:  this log is dangerous!  People often get out of
	 *	sync on logins.  We only log the id, but some passwords
	 *	will appear!
	 */
	fprintf(stderr, "Authentication warning: %s\n", mess);
	fprintf(stderr, "	Session:  `%s'\n", logbuf);
}

log(mess)
	char *mess;
{
	strncat(logbuf, mess, sizeof(logbuf) - strlen(mess) - 1);
	logbuf[sizeof(logbuf)-1] = '\0';
}
