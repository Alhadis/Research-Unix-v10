#define DKMSGS 1
#include <sys/types.h>
#include <stdio.h>
#include <dk.h>
#include <pwd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "dkdial.h"
#include <sys/stat.h>
#include <dkwindow.h>

void	alarmcatch();
char	*getlogin();
struct dialout reply;
int pipefd;

main (argc, argv)
int argc;
char **argv;
{
	int fd, server;
	int retval, listener;
	struct listenin d;
	register char **p;
	struct stat st;
	char dialstr[64];
	char dialtone[2];
	int rc;
	int traffic;
	register char *cp;

	/* pick up and decode arguments */
	if (argc != 6)
		bugout(ILLEG);
	p = argv + 1;
	fd = atoi (*p++);
	pipefd = atoi (*p++);
	server = atoi (*p++);
	if (server==1)
		server = 3;
	traffic = atoi(*p++);
	strcpy(dialstr, *p);
	cp = dialstr;
	while (*cp) {
		if (*cp == '\n')
			*cp = '\0';
		cp++;
	}

	/* set up the parameter block for the initial request */
	fstat(fd, &st);
	d.l_type = T_SRV;
	d.l_lchan = minor(st.st_rdev);
	d.l_srv = 256 - server;
	d.l_param0 = traffic;
/*
	d.l_param0 = WINDOW(WS_4K, WS_4K, traffic);
*/
	d.l_param1 = 0;
	d.l_param2 = 0;
	d.l_param3 = 0;
	d.l_param4 = 0;
	d.l_param5 = 0;
	strcat(dialstr, "\n");
	cp = getlogin();
	if (cp == 0)
		bugout(NOUID);
	strcat(dialstr, cp);
	strcat(dialstr, "\n");
	signal (SIGALRM, alarmcatch);
	alarm (15);
	listener = dkctlchan(traffic);
	if (listener < 0)
		bugout(NOMGR);
	rc = write(listener, (char *)&d, sizeof(d));
	if (rc != sizeof(d))
		bugout(NODIALT);
	close(listener);
	alarm (15);
	rc = read(fd, dialtone, 1);
	if (rc < 0)
		bugout(NODIALT);
	if (dialtone[0] != 'O')
		bugout(NODIALT);
	ioctl(fd, DIOCSTOP, 0);
	write(fd, dialstr, strlen(dialstr));
	alarm(60);
	rc = read(fd, &reply, sizeof reply);
	alarm (0);
	if (rc <= 0)
		bugout(NOREP);
	write(pipefd, &reply, sizeof reply);

	/* assume normal exit */
	retval = NORM;

	/* successful reply, analyze it */
	switch (reply.srv) {
	case D_OPEN:
		if (W_VALID(reply.param4)) {	/* check window size */
			char ws[5];
			long wins = W_VALUE(W_DEST(reply.param4))>>2;
			/* use 3 X wins/4 */
			ws[0] = wins;
			ws[1] = wins>>8;
			ws[2] = wins>>16;
			ws[3] = wins>>24;
			ws[4] = 3;
			ioctl(fd, DIOCXWIN, ws);
		}
		break;

	case D_FAIL:
		exit(ERRBASE + (reply.param1&0x7f));
	}
	return retval;
}

void
alarmcatch()
{
}

/*
 * write some sort of reply
 * (obsolete, but some old callers use it)
 */
bugout(r)
{
	reply.srv = D_FAIL;
	write(pipefd, (char *)&reply, sizeof(reply));
	exit(r);
}
