#include <stdio.h>
#include <signal.h>
#include "mail.h"
#include "string.h"
#include "message.h"

/* imported */
char *getenv();

/*
 * auxiliary routines used by commands
 */

/* drop the newline off a string */
static char *
dropleadingwhite(cmd)
	char *cmd;
{
	while(*cmd==' ' || *cmd=='\t')
		cmd++;
	return cmd;
}

/* drop the newline off a string */
static void
dropnewline(cmd)
	char *cmd;
{
	cmd = cmd+strlen(cmd)-1;
	if (*cmd=='\n')
		*cmd = '\0';
}

/* make sure an argument exists */
static int
needargument(cmd)
	char *cmd;
{
	if (*cmd=='\0') {
		fprintf(stderr, "?argument missing\n");
		return -1;
	}
	return 0;
}

/* make the header line for a message */
char *
header(mp)
	message *mp;
{
	static char hd[256];

	sprintf(hd, "%3d %c %4d %s %.30s", mp->pos, mp->status&DELETED?'d':' ',
		mp->size, s_to_c(mp->sender), s_to_c(mp->date));
	return hd;
}

/*
 *	command routines 
 */

/* current message */
whereis(mp)
	message *mp;
{
	printf("%d\n", mp->pos);
	return 0;
}

/* print header */
prheader(mp)
	message *mp;
{
	printf("%s\n", header(mp));
	return 0;
}

/* change message status */
delete(mp)
	message *mp;
{
	mp->status |= DELETED;
	return 0;
}
undelete(mp)
	message *mp;
{
	mp->status &= ~DELETED;
	return 0;
}

/* store a message into a file */
store(mp, cmd, header)
	message *mp;
	char *cmd;
{
	static string *mbox=NULL;
	FILE *fp;
	char *home;

	dropnewline(cmd);
	cmd = dropleadingwhite(cmd);
	if (*cmd=='\0') {
		if (mbox==NULL){ 
			mbox=s_new();
			if ((home=getenv("HOME"))!=NULL) {
				s_append(mbox, home);
				s_append(mbox, "/");
			}
			s_append(mbox, "mbox");
		}
		cmd = s_to_c(mbox);
	}
	fp = fopen(cmd, "a");
	if (fp == NULL) {
		fprintf(stderr, "?can't open %s\n", cmd);
		return -1;
	}
	m_print(mp, fp, header, header);
	fclose(fp);
	return 0;
}

static interrupt = 0;

SIGRETURN
remember_int(s)
	int s;
{
	signal(SIGINT, remember_int);
	interrupt = 1;
}

/* pipe a message (and possible tty input) to a command */
static int
pipecmd(mp, cmd, fromtty, mailinput)
	message *mp;
	char *cmd;
	int fromtty;
	int mailinput;
{
	FILE *fp;
	char buf[128];
	int status, rv=0;
	SIG_TYP istat, qstat;
	int onatty;

	onatty = isatty(fileno(stdin));
	if (fromtty){
		printf("!%s\n", cmd);
		fflush(stdout);
	}
	fp = popen(cmd, "w");
	if (fp==NULL) {
		fprintf(stderr, "?can't exec %s\n", cmd);
		return -1;
	}
	interrupt = 0;
	istat = signal(SIGINT, remember_int);
	qstat = signal(SIGQUIT, remember_int);
	if (fromtty) {
		while (fgets(buf, sizeof(buf), stdin)!=NULL) {
			if (interrupt!=0)
				break;
			if (onatty && strcmp(buf, ".\n")==0)
				break;
			fputs(buf, fp);
			fflush(fp);
		}
		clearerr(stdin);
	}
	if (!interrupt && mailinput)
		m_print(mp, fp, 0, 1);
	if (status=pclose(fp))
		rv = -1;
	signal(SIGINT, istat);
	signal(SIGQUIT, qstat);
	if (fromtty)
		printf("!\n");
	return rv;
}

/* pass a message to someone else */
remail(mp, cmd, ttyinput)
	message *mp;
	char *cmd;
	int ttyinput;
{
	static string *cmdstring=NULL;

	dropnewline(cmd);
	if(cmdstring==NULL)
		cmdstring = s_new();
	s_append(s_restart(cmdstring), "/bin/mail ");
	s_append(cmdstring, cmd);
	if (pipecmd(mp, s_to_c(cmdstring), ttyinput, 1)<0)
		return -1;
	return 0;
}

/* output a message */
print(mp)
	message *mp;
{
	m_print(mp, stdout, 0, 1);
	return 0;
}

/* pipe mail into a command */
pipemail(mp, cmd, ttyinput)
	message *mp;
	char *cmd;
{
	dropnewline(cmd);
	if (pipecmd(mp, cmd, ttyinput, 1)<0)
		return -1;
	return 0;
}

/* escape to the shell */
escape(cmd)
	char *cmd;
{
	char *cp;

	cp = cmd+strlen(cmd)-1;
	if (*cp=='\n')
		*cp = '\0';
	system(cmd);
	printf("!\n");
	return 0;
}

/* reply to a message */
reply(mp, mailinput)
	message *mp;
	int mailinput;
{
	static string *cmdstring=NULL;

	if (cmdstring==NULL)
		cmdstring = s_new();
	s_append(s_restart(cmdstring), "/bin/mail ");
	s_append(cmdstring, s_to_c(mp->sender));
	if (pipecmd(mp, s_to_c(cmdstring), 1, mailinput)<0)
		return -1;
	return 0;
}

/* print out the command list */
help()
{
	printf("Commands are of the form [range] command [argument].\n");
	printf("The commmands are:\n");
	printf("b\tprint the next ten headers\n");
	printf("d\tmark for deletion\n");
	printf("h\tprint message header (,h to print all headers)\n");
	printf("m addr\tremail message to addr\n");
	printf("M addr\tremail message to addr preceded by user input\n");
	printf("p\tprint the message\n");
	printf("q\texit from mail, and save messages not marked for deletion\n");
	printf("r\treply to sender\n");
	printf("R\treply to sender with original message appended\n");
	printf("s file\tappend message to file\n");
	printf("u\tunmark message for deletion\n");
	printf("x\texit without changing mail box\n");
	printf("| cmd\tpipe mail to command\n");
	printf("! cmd\tescape to commmand\n");
	printf("?\tprint this message\n");
	return 0;
}
