/*
 * DCON
 * Connect terminal to Datakit network
 * Operation is line-at-a-time with remote echo.
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sgtty.h>
#include <signal.h>
#include <dk.h>
#include <errno.h>

#define	CEOT	04
#define	NULL	0


struct	sgttyb	locvec, savloc;
struct	tchars savechars;
struct	ltchars lsavechars;
struct	ltchars nlchars = { -1, -1, -1, -1, -1, -1 };

int	rem;	/* remote descriptor */
int	rembit;	/* 1<<rem; for select */
extern	int dkp_ld, dt_ld, pk_ld, mesg_ld;
int	which_ld;
char	serv[32];		/* .login, .dcon, etc. */
int	autologin;
int	noflush;
fd_set	rdfd_set;
#define	NSELFD	(rem+1)
#define LINSIZ 128
char *space = " \r\t\n";
int	neofs = 0;		/* die when greater than N */

int	sigint();
extern	dt_ld;	/* TDK protocol line discipline */
extern	int	dkverbose ;
char	*mytty;
extern char *strtok();
extern char *strchr();
extern char *strcpy();
extern char *strcat();
extern char *malloc();
extern FILE *scriptopen();
SIG_TYP savint, savquit;

intcatch(){	/* catch interrupts, turn them into rubouts */
	signal(SIGINT, intcatch);
	ioctl(rem, TIOCFLUSH, 0);
	write(rem, &savechars.t_intrc, 1);
	ioctl(rem, TIOCFLUSH, 0);
	ioctl(0, TIOCFLUSH, 0);
	rdfd_set.fds_bits[0] = 0;
}

quitcatch(){	/* catch quits, turn them into FS's */
	ioctl(0, TIOCFLUSH, 0);
	signal(SIGQUIT, quitcatch);
	write(rem, &savechars.t_quitc, 1);
}

main(argc,argv)
	char **argv;
{
	int	i, traffic;
	char dialstr[64] ;
 	int scriptlogin;
	FILE *script;
	extern char *getenv();
	extern char *ttyname();

	ioctl(0, TIOCGETC, &savechars);
	ioctl(0, TIOCGLTC, &lsavechars);
	ioctl(0, TIOCGETP, &savloc);
	locvec = savloc;
	which_ld = dkp_ld;
	strcpy(serv, ".dcon");
	if(strcmp(argv[0], "tdcon") == 0 || getenv("TDCON") != NULL){
		strcpy(serv, ".tdcon");
		which_ld = dkp_ld;
		if(strcmp(argv[1], "-x") == 0){
			argv[1][1] = 't';
		}
	}
	traffic = 0;
	autologin = 1;
	scriptlogin = 0;
	for(i=1; i<argc && argv[i][0]=='-'; i++) {
		switch(argv[i][1]) {
		case 'f':
			noflush++;
			continue;
		case 's':
			scriptlogin = 1;
		case 'l':
			serv[0] = '\0';
			autologin = 0;
			which_ld = dt_ld;
			continue;
		case 'x':	/* deprecated */
			strcpy(serv, ".dcon");
			which_ld = dkp_ld;
			autologin = 1;
			break;
		case 't':
			which_ld = dkp_ld;
			strcpy(serv, ".tdcon");
			break;
		case 'd':
		case 'b':
			which_ld = dkp_ld;
			break;
		case 'c':
			which_ld = dt_ld;
			break;
		case 'p':
		case 'g':
			which_ld = pk_ld;
			break;
		case 'v':
			dkverbose++ ;
			continue ;
		default:
			goto Usage;
		}
	}
	if (i>=argc){
    Usage:
		/* don't use quit() because ioctl's aren't set up */
		printf("usage: dcon [-lvs] hostname\n");
		exit(1) ;
	}
	savint = signal(SIGINT, intcatch);
	savquit = signal(SIGQUIT, quitcatch);
	/*
	 * request circuit to host.
	 */
	strcpy(dialstr, argv[i]) ;
	if(index(dialstr, '.')){
		serv[0] = '\0';
	}
	if(scriptlogin)  {
		script = scriptopen(dialstr);
		if(script == NULL)
			quit("bad script");
	}
	strcat(dialstr, serv);
	rem = tdkdial(dialstr, traffic);
	if (rem < 0) {
		char msg[64];
		extern char *dkerror;
		sprintf(msg, "%s; call failed", dkerror);
		quit(msg);
	}
	if(dkverbose) printf("cut through\n");
	rembit = 1<<rem;
	/*
	 * turn on line discipline according to protocol.
	 */
	if(dkverbose) printf("pushing %d...\n", which_ld);
	if(dkproto(rem, which_ld) < 0)
		quit("can't turn on datakit protocol");
	if (autologin){
		if (tdklogin(rem) < 0)
			quit("can't log in") ;
		if(dkverbose) printf("logged in\n");
	} else if(scriptlogin) {
		if (dkscript(rem,script) < 0)
			quit("can't log in") ;
		if(dkverbose) fprintf(stderr,"logged in\n");
	}
	if(strcmp(serv, ".tdcon") == 0){
		dotdcon(rem);
		if(dkverbose) printf("set terminal\n");
	}
	locvec.sg_flags &= ~(CRMOD|ECHO|XTABS);
	locvec.sg_flags |= CBREAK;
	ioctl(0, TIOCSETP, &locvec);
	ioctl(0, TIOCSLTC, &nlchars);


	/*
	 * main loop.
	 */
	ioctl(rem, DIOCSTREAM, (char *)0);
	do; while(scan() != -1);
	quit("select failed");
	/*NOTREACHED*/
}
scan()
{
	extern errno;
    Loop:
	rdfd_set.fds_bits[0] = 1|(1<<rem);
	if(select(NSELFD, &rdfd_set, (fd_set *)0, 2000) == -1)
		if(errno == EINTR)
			goto Loop;
		else
			return -1;
	if(rdfd_set.fds_bits[0] == 0)	/* timeout */
		goto Loop;
	if(rdfd_set.fds_bits[0] & 1)
		keyboard();
	if(rdfd_set.fds_bits[0] & rembit)
		remote();
	return 0;
}

quit(s)
	char *s;
{
	printf("dcon: %s\n", s);
	ioctl(0, TIOCSETP, &savloc);
	ioctl(0, TIOCSLTC, &lsavechars);
	signal(SIGINT, SIG_DFL);
	if (noflush==0) {
		ioctl(rem, TIOCFLUSH, 0);
		savloc.sg_ispeed = savloc.sg_ospeed = 0;	/* hangup */
		ioctl(rem, TIOCSETP, &savloc);
	}
	close(rem);
	exit(strcmp(s, "eof"));
}

/*
 * Scan data from keyboard, looking for escape lines.
 */
keyboard()
{
	register c;
	register cc;
	register char *bp;
	register char *be, *obp;
	char buf[1024];
	static char line[128];
	static char *linep = &line[0];
	static col = 0;
	long time();
	static long timev[2];

	cc = read(0, buf, sizeof buf);
	if(cc <0){
		if(errno == EINTR)
			return;
	}
	if(cc <= 0)
		quit("read error on file descriptor 0");
	be = buf+cc;
	bp = obp = buf;
	while(bp < be) {
		c = *bp++;
		if (col==0 && c=='~') {
			*linep++ = c;
			col = 1;
			locvec.sg_flags |= savloc.sg_flags&(CRMOD|ECHO);
			ioctl(0, TIOCSETP, &locvec);
			write(1, linep-1, 1);
			continue;
		}
		col++;
		if (c=='\r')
			c = '\n';
		if (linep>line) {
			*linep++ = c;
			if (c==savloc.sg_kill)
				write(1, "\n", 1);
			if (c==savloc.sg_erase)
				linep -= 2;
			if (c==savloc.sg_kill || linep<=line) {
				linep = line;
				locvec.sg_flags &= ~(CRMOD|ECHO);
				ioctl(0, TIOCSETP, &locvec);
				col = 0;
				obp = bp;
			}
		}
		if (c=='\n') {
			col = 0;
			if (linep > line) {
				*linep = '\0';
				if (escape(line+1, linep))
					write(rem, line+1, linep-line-1);
				obp = bp;
				linep = line;
				locvec.sg_flags &= ~(CRMOD|ECHO);
				ioctl(0, TIOCSETP, &locvec);
			}
		}
	}
	if (bp>obp && linep==line)
		write(rem, obp, bp-obp);
}

/*
 * Send data from remote machine to standard output (trivial)
 */
remote(){
	char buf[1024];
	register n;

	n = read(rem, buf, sizeof buf);
	if(n < 0 && errno == EINTR)
		return;
	if(n <= 0){
		if(dkverbose) printf("EOF %d\r\n", neofs);
		if(neofs++ > 4){
			quit("Eof\r");
		}
		return;
	}
	neofs = 0;
	write(1, buf, n);
}

escape(line, end)
	register char *line;
{
	int cc;

	switch(*line++) {
	case '!':
		cunix(line);
		return(0);

	case '.':
	case CEOT:
		quit("eof");

	case 'b':
		ioctl(rem, TIOCSBRK, 0);
		return(0);
	default:
		return(1);
	}
}

cunix(prog)
char *prog;
{
	register int upid;
	int retcode;

	if ((upid = fork()) == 0) {
		signal(SIGINT, savint);
		signal(SIGQUIT, savquit);
		ioctl(0, TIOCSETN, &savloc);
		ioctl(0, TIOCSLTC, &lsavechars);
		if (*prog == '\n')
			execl("/bin/sh", "sh", "-i", 0);
		else
			execl("/bin/sh","sh","-c",prog,0);
		exit(0100);
	}
	if (upid < 0) {
		printf("can't fork\n");
	} else {
		while((wait(&retcode) !=upid))
			;
	}
	signal(SIGINT, intcatch);
	signal(SIGQUIT, quitcatch);
	ioctl(0, TIOCSETN, &locvec);
	ioctl(0, TIOCSLTC, &nlchars);
	printf("!!\n");
}

int tfd;

dotdcon(fd)
{
	char *p, *getenv();
	char buf[3000];
	char c;

	tfd = fd;
	if((p = getenv("TERM")) != NULL && strlen(p) < (sizeof(buf) - 20)){
		sprintf(buf, "TERM=%s", p);
		tsend("env", buf);
	}

	qsend("erase", savloc.sg_erase, '\b');
	qsend("kill", savloc.sg_kill, '@');
	qsend("intrc", savechars.t_intrc, '\177');
	qsend("quitc", savechars.t_quitc, '\034');
	qsend("startc", savechars.t_startc, 'q' & 037);
	qsend("stopc", savechars.t_stopc, 's' & 037);
	qsend("eofc", savechars.t_eofc, '\004');
	qsend("brkc", savechars.t_brkc, '\377');

	if(savloc.sg_flags & XTABS) tsend("XTABS", "yes");
	if((savloc.sg_flags & CRMOD) == 0) tsend("CRMOD", "no");
	if((savloc.sg_flags & ECHO) == 0) tsend("ECHO", "no");
	if(savloc.sg_flags & CBREAK) tsend("CBREAK", "yes");
	if(savloc.sg_flags & LCASE) tsend("LCASE", "yes");

	tsend("env", "TDCON=YES");

	if((p = getenv("TDKBAG")) && strlen(p) < (sizeof(buf) - 20)){
		sprintf(buf, "TDKBAG=%s", p);
		tsend("env", buf);
	}
	/* TERMCAP is last because it might have messed up characters */
	if((p = getenv("TERMCAP")) != NULL){
		if(strlen(p) < (sizeof(buf) - 20)){
			sprintf(buf, "TERMCAP=%s", p);
			tsend("env", buf);
		}
	}

	write(fd, "\n", 1);
}

tsend(a, b)
char *a, *b;
{
	char *p;

	p = (char *) malloc(strlen(a) + strlen(b) + 10);
	if(p == NULL) return;
	sprintf(p, "%s=%s\n", a, b);
	write(tfd, p, strlen(p));
	if(dkverbose > 1) write(1, p, strlen(p));
	free(p);
}

ctsend(s, c)
char *s;
char c;
{
	char buf[50];

	sprintf(buf, "%o", (int) c);
	tsend(s, buf);
}

qsend(s, c, wc)
char *s;
char c, wc;
{
	if(c != wc)
		ctsend(s, c);
}

dkscript(rem,f)
FILE *f;
{
	char sline[LINSIZ];
	while(fgets(sline,100,f)) {
		if(rget(rem,strtok(sline,space)) < 0)
			return(-1);
		if(fgets(sline,100,f)==0)
			return(-1);
		if(dkverbose) fprintf(stderr,"sending %s",sline);
		write(rem,sline,strlen(sline));
	}
	return(1);
}

rget(rem,s)
char *s;
{
	int i;
	char buf[LINSIZ];
	int brkchr = s[strlen(s)-1];
	for(;;) {
		for(i=0; ; i++) {
			if(i>=LINSIZ-2) {
				buf[i] = 0;
				strcpy(buf,&buf[LINSIZ/2]);
				i = strlen(buf);
			}
			if(read(rem,&buf[i],1) <= 0)
				return(-1);
			buf[i] &= 0177;
			if(buf[i] == brkchr)
				break;
		}
		buf[i+1] = 0;
		while(i>=0 && buf[i] && !strchr(space,buf[i]))
			i--;
		i++;
		if(dkverbose) fprintf(stderr,"check '%s' vs '%s'\n",s,&buf[i]);
		if(strcmp(s,&buf[i]) == 0)
			return(1);
	}
	/*NOTREACHED*/
}

FILE *
scriptopen(s)
char *s;
{
	FILE *script = fopen(s, "r");
	if(script == NULL)
		return(NULL);
	if(fgets(s,LINSIZ,script) == NULL)
		return(NULL);
	if(dkverbose) fprintf(stderr,"calling %s\n",s);
	strtok(s,"\n");
	return script;
}
