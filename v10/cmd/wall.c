/*
 *
 * This program is not related to David Wall, whose Stanford Ph.D. thesis
 * is entitled "Mechanisms for Broadcast and Selective Broadcast".
 */

#include <stdio.h>
#include <utmp.h>
#include <time.h>
#include <utsname.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

char	*strcpy(), *strncpy();
char	*strcat(), *strncat();
long	time();
struct tm *localtime();

main()
{
	register int c;
	register char *p;
	struct utmp utmp;
	struct tm *tm;
	long clock;
	FILE *f;
	char *who, *mytty;
	struct utsname node;
	char mesg[3000];
	char *getlogin(), *ttyname();
	long time();
	struct tm *localtime();

	if((f = fopen("/etc/utmp", "r")) == NULL) {
		fprintf(stderr, "Cannot open /etc/utmp\n");
		exit(1);
	}
	if ((who = getlogin()) == NULL)
		who = "greg";
	if ((mytty = ttyname(2)) == NULL)
		mytty = "???";
	else if (strncmp(mytty, "/dev/", 5) == 0)
		mytty += 5;		/* for prettiness */
	uname(&node);
	clock = time((long *)0);
	tm = localtime(&clock);
	sprintf(mesg,  "\nBroadcast Message from %s!%s (%s) at %d:%02d ...\r\n",
		node.nodename, who, mytty, tm->tm_hour, tm->tm_min);
	p = &mesg[strlen(mesg)];
	while((c = getchar()) != EOF) {
		if (p >= &mesg[sizeof(mesg)]) {
			fprintf(stderr, "Message too long\n");
			exit(1);
		}
		if (c == '\n')
			*p++ = '\r';
		*p++ = c;
	}
	while (fread((char *)&utmp, sizeof(utmp), 1, f) == 1) {
		if (utmp.ut_name[0] == 0)
			continue;
		/* reject people not really logged in */
		for (c = 0; c < sizeof(utmp.ut_name); c++)
			if (utmp.ut_name[c] == '*')
				break;
		if (c < sizeof(utmp.ut_name))
			continue;
		sendmes(utmp.ut_line, sizeof(utmp.ut_line), mesg, p - mesg);
	}
	exit(0);
}

sendmes(tty, len, mesg, mlen)
char *tty, *mesg;
int len, mlen;
{
	int i;
	char t[50];
	int fd;

	while ((i = fork()) == -1)
		if (wait((int *)0) == -1) {
			fprintf(stderr, "Try again\n");
			return;
		}
	if(i)
		return;
	sprintf(t, "/dev/%.*s", len, tty);
	if (ckmode(t) == 0)
		exit(0);
	signal(SIGALRM, SIG_DFL);	/* blow away if open hangs */
	alarm(10);
	if((fd = open(t, 1)) < 0) {
		fprintf(stderr,"cannot open %s\n", t);
		exit(1);
	}
	write(fd, mesg, mlen);
	close(fd);

	/*
	 * Bitchin'.
	 */

	exit(0);
}

/*
 * don't write on ttys whose mode is 0
 * hack to prevent hanging ndcon
 */

ckmode(tty)
char *tty;
{
	struct stat sb;

	if (stat(tty, &sb) < 0)
		return (1);	/* eh? */
	if (sb.st_mode & ~S_IFMT)
		return (1);
	return (0);
}
