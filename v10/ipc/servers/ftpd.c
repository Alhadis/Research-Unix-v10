#ifndef lint
static char sccsid[] = "@(#)ftpd.c	4.28 (Berkeley) 9/22/83";
#endif

#define	CHOOSE_PORT
#define DEBUG
/*
 * FTP server.
 */
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <wait.h>

#include <sys/inet/in.h>
#include <sys/inet/tcp_user.h>

#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <setjmp.h>
#include <errno.h>
#include <utsname.h>

#include <libc.h>

#include "ftp.h"

/*
 * File containing login names
 * NOT to be used on this machine.
 * Commonly used to disallow uucp.
 */
#define CTLPORT		21

extern	int errno;
extern	char *sys_errlist[];
extern	char *crypt();
extern	FILE *popen(), *fopen();
extern	int pclose(), fclose();

char	*defdest;
extern char	dest[];

int	data;
jmp_buf	errcatch;
int	logged_in;
struct	passwd *pw;
int	debug;
int	timeout;
int	logging;
int	type;
int	form;
int	stru;			/* avoid C keyword */
int	mode;
int	usedefault = 1;		/* for data transfers */
char	hostname[32];
char	remotehost[32];
int	pasv;
char	*home;		/* pointer to home directory for glob */

/*
 * Timeout intervals for retrying connections
 * to hosts that don't accept PORT cmds.  This
 * is a kludge, but given the problems with TCP...
 */
#define	SWAITMAX	90	/* wait at most 90 seconds */
#define	SWAITINT	5	/* interval between retries */

int	swaitmax = SWAITMAX;
int	swaitint = SWAITINT;

int	lostconn();
int	reapchild();
FILE	*dataconn();

main(argc, argv)
	int argc;
	char *argv[];
{
	int ctrl, s, options = 0;
	char *cp;
	int dev;
	int status;
	struct utsname uts;

	if(argc==2 && strcmp(argv[1], "-d")==0)
		debug = 1;

	signal(SIGPIPE, SIG_IGN); /* used to be lostconn */
	signal(SIGHUP, SIG_IGN);
	signal (SIGCHLD, SIG_IGN);

	/*
	 *  get source address
	 */
	getsource();

	/*
	 * Set up default state
	 */
	logged_in = 0;
	data = -1;
	type = TYPE_A;
	form = FORM_N;
	stru = STRU_F;
	mode = MODE_S;
	uname(&uts);
	strcpy(hostname, uts.sysname);
	reply(220, "%s FTP server ready.", hostname);
	for (;;) {
		setjmp(errcatch);
		yyparse();
		fprintf(stderr, "out of yyparse\n");
	}
}

/*
 *  parse the source string and extract system and network
 */
getsource()
{
	char *s;
	char *e;

	e = getenv("CSOURCE");
	if(e == 0){
		fprintf(stderr, "Can't get source address\n");
		exit(1);
	}
	s = strchr(e, ' ');
	if(s)
		*s = 0;
	defdest = e;
}

reapchild()
{
	union wait status;

	while (wait3(&status, WNOHANG, 0) > 0)
		;
}

lostconn()
{

	if (debug)
		fprintf(stderr, "Lost connection.\n");
	dologout(-1);
}

pass(passwd)
	char *passwd;
{
	char *xpasswd, *savestr();
	static struct passwd save;

	if (logged_in) {
		reply(503, "already logged in.");
		return;
	}
	xpasswd = crypt(passwd, pw->pw_passwd);
	if (*pw->pw_passwd == '\0' || strcmp(xpasswd, pw->pw_passwd)) {
		reply(530, "Login incorrect.");
		pw = NULL;
		return;
	}
	dologin(pw);
	setgid(pw->pw_gid);
	setuid(pw->pw_uid);
	if (chdir(pw->pw_dir)) {
		reply(550, "User %s: can't change directory to $s.",
			pw->pw_name, pw->pw_dir);
		dologout(-1);
		exit(1);
	}
	logged_in = 1;
	/*
	 * Save everything so globbing doesn't
	 * clobber the fields.
	 */
	save = *pw;
	save.pw_name = savestr(pw->pw_name);
	save.pw_passwd = savestr(pw->pw_passwd);
	save.pw_comment = savestr(pw->pw_comment);
	save.pw_gecos = savestr(pw->pw_gecos, &save.pw_gecos);
	save.pw_dir = savestr(pw->pw_dir);
	save.pw_shell = savestr(pw->pw_shell);
	pw = &save;
	home = pw->pw_dir;		/* home dir for globbing */
	ack("login");
	return;
bad:
	pw = NULL;
}

char *
savestr(s)
	char *s;
{
	char *malloc();
	char *new = malloc(strlen(s) + 1);
	
	if (new != NULL)
		strcpy(new, s);
	return (new);
}


openfile(name)
char *name;
{
	reply(550, "%s: can't open", name);
}

pipette()
{
	signal(SIGPIPE, pipette);
}

retrieve(cmd, name)
	char *cmd, *name;
{
	FILE *fin, *dout;
	struct stat st;
	int (*closefunc)();

	if(!logged_in){
		reply(550, "%s: login required.", name);
		return;
	}
	if (cmd == 0) {
		fin = fopen(name, "r"), closefunc = fclose;
	} else {
		char line[BUFSIZ];

		sprintf(line, cmd, name), name = line;
		if(debug)
			fprintf(stderr,"cmd is %s\n", line);
		fin = popen(line, "r"), closefunc = pclose;
	}
	if (fin == NULL) {
		if(debug)
			fprintf(stderr, "bad popen()\n");
		if (errno != 0)
			reply(550, "%s: %s.", name, sys_errlist[errno]);
		return;
	}

	st.st_size = 0;
	if (cmd == 0
	&&  (stat(name, &st) < 0 || (st.st_mode&S_IFMT) != S_IFREG)) {
		reply(550, "%s: not a plain file.", name);
		goto done;
	}
	dout = dataconn(name, st.st_size, "w");
	if (dout == NULL)
		goto done;
	if (send_data(fin, dout) || ferror(dout))
		reply(550, "%s: %s.", name, sys_errlist[errno]);

	else {
		fclose(dout);
		delay(timeout);
		reply(226, "Transfer complete.");
	}
	data = -1;
done:
	(*closefunc)(fin);
}

store(name, mode)
	char *name, *mode;
{
	FILE *fout, *din;
	int (*closefunc)(), dochown = 0;

	if(!logged_in){
		reply(550, "%s: login required.", name);
		return;
	}
	fout = fopen(name, mode), closefunc = fclose;
	if (fout == NULL) {
		reply(550, "%s: %s.", name, sys_errlist[errno]);
		return;
	}
	din = dataconn(name, (off_t)-1, "r");
	if (din == NULL)
		goto done;
	if (receive_data(din, fout) || ferror(fout))
		reply(550, "%s: %s.", name, sys_errlist[errno]);
	else {
		fflush(fout);
		delay(timeout);
		reply(226, "Transfer complete.");
	}
	fclose(din);
	data = -1;
done:
	(*closefunc)(fout);
}

FILE *
dataconn(name, size, mode)
	char *name;
	off_t size;
	char *mode;
{
	char sizebuf[32];
	char dialstr[128];
	char params[32];
	FILE *file;
	int retries;

	if (size >= 0)
		sprintf (sizebuf, " (%ld bytes)", size);
	else
		(void) strcpy(sizebuf, "");

	if (data >= 0) {
		reply(125, "Using existing data connection for %s%s.",
		    name, sizebuf);
		usedefault = 1;
		return (fdopen(data, mode));
	}
	if (usedefault)
		sprintf(dialstr, "/cs/%s", defdest);
	else
		sprintf(dialstr, "/cs/%s", dest);

	/* make the connection */
	/*sprintf(params, "port=%d", pasv?0:(CTLPORT - 1));/**/
	for(retries=0; retries<5; retries++){
		data = ipcopen(dialstr, params);
		if(data>=0)
			break;
		sleep(1);
	}
	if(data<0 || (file=fdopen(data, mode))==NULL){
		reply(425, "Can't open data socket (%s): %s.",
			dialstr,
			sys_errlist[errno]);
		return (NULL);
	}
	reply(150, "Opening data connection for %s (%s,%d)%s.",
		name,
		dialstr,
		sizebuf);
	return (file);
}

/*
 * Tranfer the contents of "instr" to
 * "outstr" peer using the appropriate
 * encapulation of the date subject
 * to Mode, Structure, and Type.
 *
 * NB: Form isn't handled.
 */
send_data(instr, outstr)
	FILE *instr, *outstr;
{
	register int c;
	int netfd, filefd, cnt = 0;
	char buf[BUFSIZ];

	signal(SIGPIPE, pipette);
	switch (type) {

	case TYPE_A:
		while ((c = getc(instr)) != EOF) {
			if (c == '\n') {
				if (ferror (outstr))
					return (1);
				putc('\r', outstr);
			}
			putc(c, outstr);
			if (c == '\r')
				putc ('\0', outstr);
/*			cnt++;
			if (!(cnt%1000)) { fprintf(stderr, "%d ", cnt); fflush(stderr); } */
		}
		if (ferror (instr) || ferror (outstr)) {
			fprintf(stderr,"error: inst %d, outstr %d\n", ferror(instr), ferror(outstr));
			return (1);
		}
		return (0);
		
	case TYPE_I:
	case TYPE_L:
		netfd = fileno(outstr);
		filefd = fileno(instr);

		while ((cnt = read(filefd, buf, sizeof (buf))) > 0)
			if (write(netfd, buf, cnt) < 0) {
				fprintf(stderr, "write error, cnt=%d\n", cnt);
				return (1);
		}
		return (cnt < 0);
	}
	reply(504,"Unimplemented TYPE %d in send_data", type);
	return (1);
}

/*
 * Transfer data from peer to
 * "outstr" using the appropriate
 * encapulation of the data subject
 * to Mode, Structure, and Type.
 *
 * N.B.: Form isn't handled.
 */
receive_data(instr, outstr)
	FILE *instr, *outstr;
{
	register int c;
	int cnt;
	char buf[BUFSIZ];


	signal(SIGPIPE, pipette);
	switch (type) {

	case TYPE_I:
	case TYPE_L:
		while ((cnt = read(fileno(instr), buf, sizeof buf)) > 0)
			if (write(fileno(outstr), buf, cnt) < 0)
				return (1);
		return (cnt < 0);

	case TYPE_E:
		reply(504, "TYPE E not implemented.");
		return (1);

	case TYPE_A:
		while ((c = getc(instr)) != EOF) {
			if (c == '\r') {
				if (ferror (outstr))
					return (1);
				if ((c = getc(instr)) != '\n')
					putc ('\r', outstr);
				if (c == '\0')
					continue;
			}
			putc (c, outstr);
		}
		if (ferror (instr) || ferror (outstr))
			return (1);
		return (0);
	}
	fatal("Unknown type in receive_data.");
	/*NOTREACHED*/
}

fatal(s)
	char *s;
{
	reply(451, "Error in server: %s\n", s);
	reply(221, "Closing connection due to server error.");
	dologout(0);
}

reply(n, s, args)
	int n;
	char *s;
{

	printf("%d ", n);
	_doprnt(s, &args, stdout);
	printf("\r\n");
	fflush(stdout);
	if (debug) {
		fprintf(stderr, "<--- %d ", n);
		_doprnt(s, &args, stderr);
		fprintf(stderr, "\n");
		fflush(stderr);
	}
}

lreply(n, s, args)
	int n;
	char *s;
{
	printf("%d-", n);
	_doprnt(s, &args, stdout);
	printf("\r\n");
	fflush(stdout);
	if (debug) {
		fprintf(stderr, "<--- %d-", n);
		_doprnt(s, &args, stderr);
		fprintf(stderr, "\n");
	}
}

replystr(s)
	char *s;
{
	printf("%s\r\n", s);
	fflush(stdout);
	if (debug)
		fprintf(stderr, "<--- %s\n", s);
}

ack(s)
	char *s;
{
	reply(200, "%s command okay.", s);
}

nack(s)
	char *s;
{
	reply(502, "%s command not implemented.", s);
}

yyerror()
{
	reply(500, "Command not understood.");
}

delete(name)
	char *name;
{
	struct stat st;

	if(!logged_in){
		reply(550, "delete: login required.");
		return;
	}
	if (stat(name, &st) < 0) {
		reply(550, "%s: %s.", name, sys_errlist[errno]);
		return;
	}
	if ((st.st_mode&S_IFMT) == S_IFDIR) {
		if (rmdir(name) < 0) {
			reply(550, "%s: %s.", name, sys_errlist[errno]);
			return;
		}
		goto done;
	}
	if (unlink(name) < 0) {
		reply(550, "%s: %s.", name, sys_errlist[errno]);
		return;
	}
done:
	ack("DELE");
}

cwd(path)
	char *path;
{

	if(!logged_in){
		reply(550, "cwd: login required.");
		return;
	}
	if (chdir(path) < 0) {
		reply(550, "%s: %s.", path, sys_errlist[errno]);
		return;
	}
	ack("CWD");
}

makedir(name)
	char *name;
{
	struct stat st;
	int dochown = stat(name, &st) < 0;

	if(!logged_in){
		reply(550, "makedir: login required.");
		return;
	}
	if (mkdir(name, 0777) < 0) {
		reply(550, "%s: %s.", name, sys_errlist[errno]);
		return;
	}
	ack("MKDIR");
}

removedir(name)
	char *name;
{

	if(!logged_in){
		reply(550, "removedir: login required.");
		return;
	}
	if (rmdir(name) < 0) {
		reply(550, "%s: %s.", name, sys_errlist[errno]);
		return;
	}
	ack("RMDIR");
}

#define	MAXPATHLEN	256
pwd()
{
	char path[MAXPATHLEN + 1];

	if (getwd(path) == NULL) {
		reply(451, "%s.", path);
		return;
	}
	reply(251, "\"%s\" is current directory.", path);
}

char *
renamefrom(name)
	char *name;
{
	struct stat st;

	if(!logged_in){
		reply(550, "rename: login required.");
		return;
	}
	if (stat(name, &st) < 0) {
		reply(550, "%s: %s.", name, sys_errlist[errno]);
		return ((char *)0);
	}
	reply(350, "File exists, ready for destination name");
	return (name);
}

renamecmd(from, to)
	char *from, *to;
{

	if(!logged_in){
		reply(550, "rename: login required.");
		return;
	}
	if (rename(from, to) < 0) {
		reply(550, "rename: %s.", sys_errlist[errno]);
		return;
	}
	ack("RNTO");
}

#include <utmp.h>

#define	SCPYN(a, b)	strncpy(a, b, sizeof (a))
struct	utmp utmp;

/*
 * Record login in wtmp file.
 */

#define	O_WRONLY	1
#define O_APPEND	1
dologin(pw)
	struct passwd *pw;
{
	int wtmp;
	char line[32];

	pasv = 0;
	wtmp = open("/usr/adm/wtmp", O_WRONLY|O_APPEND);
	if (wtmp >= 0) {
		/* hack, but must be unique and no tty line */
		sprintf(line, "ftp%d", getpid());
		SCPYN(utmp.ut_line, line);
		SCPYN(utmp.ut_name, pw->pw_name);
		utmp.ut_time = time(0);
		(void) write(wtmp, (char *)&utmp, sizeof (utmp));
		(void) close(wtmp);
	}
}

/*
 * Record logout in wtmp file
 * and exit with supplied status.
 */
dologout(status)
	int status;
{
	int wtmp;

	if (!logged_in)
		_exit(status);
	wtmp = open("/usr/adm/wtmp", O_WRONLY|O_APPEND);
	if (wtmp >= 0) {
		SCPYN(utmp.ut_name, "");
		utmp.ut_time = time(0);
		(void) write(wtmp, (char *)&utmp, sizeof (utmp));
		(void) close(wtmp);
	}
	/* beware of flushing buffers after a SIGPIPE */
	_exit(status);
}

static char *
nextarg(cpp)
	char *cpp;
{
	register char *cp = cpp;

	if (cp == 0)
		return (cp);
	while (*cp && *cp != ' ' && *cp != '\t')
		cp++;
	if (*cp == ' ' || *cp == '\t') {
		*cp++ = '\0';
		while (*cp == ' ' || *cp == '\t')
			cp++;
	}
	if (cp == cpp)
		return ((char *)0);
	return (cp);
}

rename(from, to)
{
	if (-1 == link(from, to))
		return(-1);
	return(unlink(from));
}
inet_ntoa(addr)
long addr;
{
	static char b[32];
	unsigned char *xp = (unsigned char *)&addr;

	sprintf(b, "%d.%d.%d.%d", xp[3], xp[2], xp[1], xp[0]);
	return(b);
}
delay()
{
}
