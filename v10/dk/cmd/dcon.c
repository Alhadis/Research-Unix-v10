/*
 * DCON
 * Connect terminal to Datakit network
 * Operation is line-at-a-time with remote echo.
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <dk.h>
#include <errno.h>

#define	CEOT	04

struct	sgttyb	savloc;
struct	tchars savechars;
struct	ltchars lsavechars;
struct	ltchars nlchars = { -1, -1, -1, -1, -1, -1 };
char	intrchar = 0177;
char	quitchar = '\\'&037;

int	rem;	/* remote descriptor */
extern	int dkp_ld, cdkp_ld;
#define	NOLD	(-1)	/* no line discipline */
int	which_ld = NOLD;
char	serv[32];		/* .login, .dcon, etc. */
int	autologin;
int	noflush;
#define LINSIZ 128
char *space = " \r\t\n";
int	neofs = 0;		/* die when greater than N */
int	sigint();
extern	int	dkverbose ;
extern char *strtok();
extern char *strchr();
extern char *strcpy();
extern char *strcat();
extern char *malloc();
extern FILE *scriptopen();
extern char *strchr();
SIG_TYP savint, savquit;

intcatch(){	/* catch interrupts, turn them into rubouts */
	signal(SIGINT, intcatch);
	ioctl(rem, TIOCFLUSH, 0);
	write(rem, &intrchar, 1);
	ioctl(rem, TIOCFLUSH, 0);
	ioctl(0, TIOCFLUSH, 0);
}

quitcatch(){	/* catch quits, turn them into FS's */
	ioctl(0, TIOCFLUSH, 0);
	signal(SIGQUIT, quitcatch);
	write(rem, &quitchar, 1);
}

main(argc,argv)
	char **argv;
{
	int	i, traffic;
	char dialstr[64] ;
 	int scriptlogin;
	FILE *script;

	savetty();
	which_ld = dkp_ld;
	strcpy(serv, ".dcon");
	traffic = 0;
	autologin = 1;
	scriptlogin = 0;
	for(i=1; i<argc && argv[i][0]=='-'; i++) {
		switch(argv[i][1]) {
		case 'u':
			which_ld = NOLD;
			continue;
		case 'f':
			noflush++;
			continue;
		case 's':
			scriptlogin = 1;
			continue;
		case 'l':
			serv[0] = '\0';
			autologin = 0;
			continue;
		case 'd':
		case 'b':
			which_ld = dkp_ld;
			continue;
		case 'c':
			which_ld = cdkp_ld;
			continue;
		case 'v':
			dkverbose++ ;
			continue ;
		default:
			goto Usage;
		}
	}
	if (i>=argc){
    Usage:
		quit("usage: dcon [-lvs] hostname");
	}
	savint = signal(SIGINT, intcatch);
	savquit = signal(SIGQUIT, quitcatch);
	/*
	 * request circuit to host.
	 */
	strcpy(dialstr, argv[i]) ;
	if(strchr(dialstr, '.'))
		serv[0] = '\0';
	if(scriptlogin) {
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
	/*
	 * turn on line discipline according to protocol.
	 */
	if(dkverbose) printf("pushing %d...\n", which_ld);
	if (which_ld != NOLD
	&&  dkproto(rem, which_ld) < 0)
		quit("can't turn on datakit protocol");
	if (autologin){
		if (tdklogin(rem) < 0)
			quit("can't log in") ;
		if(dkverbose) printf("logged in\n");
	}
	if(scriptlogin) {
		if (dkscript(rem,script) < 0)
			quit("can't log in") ;
		if(dkverbose) fprintf(stderr,"logged in\n");
	}
	settty();
	scan(rem);
	/*NOTREACHED*/
}

scan(f)
int f;
{
	extern errno;
	fd_set rd;

	ioctl(f, DIOCSTREAM, (char *)0);
	FD_ZERO(rd);
	for (;;) {
		FD_SET(f, rd);
		FD_SET(0, rd);
		if (select(f+1, &rd, (fd_set *)0, 2000) == -1) {
			if(errno == EINTR)
				continue;
			quit("select failed");
		}
		if (FD_ISSET(0, rd))
			keyboard();
		if (FD_ISSET(f, rd))
			remote();
	}
}

quit(s)
	char *s;
{
	printf("dcon: %s\n", s);
	signal(SIGINT, SIG_DFL);
	resettty();
	if (noflush==0)
		ioctl(rem, TIOCFLUSH, 0);
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

	cc = read(0, buf, sizeof buf);
	if (cc == 0)
		quit("eof");
	if (cc < 0) {
		if (errno == EINTR)
			return;
		quit("read error on file descriptor 0");
	}
	be = buf+cc;
	bp = obp = buf;
	while(bp < be) {
		c = *bp++;
		if (col==0 && c=='~') {
			*linep++ = c;
			col = 1;
			resettty();
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
				settty();
				col = 0;
				obp = bp;
			}
		}
		if (c=='\n') {
			col = 0;
			if (linep > line) {
				*linep = '\0';
				if (escape(line+1))
					write(rem, line+1, linep-line-1);
				obp = bp;
				linep = line;
				settty();
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

escape(line)
	register char *line;
{

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
		resettty();
		if (*prog == '\n')
			execl("/bin/sh", "sh", "-i", 0);
		else
			execl("/bin/sh","sh","-c",prog,0);
		exit(0100);
	}
	if (upid < 0)
		printf("can't fork\n");
	else {
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		while((wait(&retcode) !=upid))
			;
	}
	signal(SIGINT, intcatch);
	signal(SIGQUIT, quitcatch);
	settty();
	printf("!!\n");
}

dkscript(r,f)
FILE *f;
{
	char sline[LINSIZ];
	while(fgets(sline,100,f)) {
		if(rget(r,strtok(sline,space)) < 0)
			return(-1);
		if(fgets(sline,100,f)==0)
			return(-1);
		if(dkverbose) fprintf(stderr,"sending %s",sline);
		write(r,sline,strlen(sline));
	}
	return(1);
}

rget(r,s)
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
			if(read(r,&buf[i],1) <= 0)
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

savetty()
{
	if (ioctl(0, TIOCGETC, &savechars) >= 0) {
		intrchar = savechars.t_intrc;
		quitchar = savechars.t_quitc;
	}
	ioctl(0, TIOCGLTC, &lsavechars);
	ioctl(0, TIOCGETP, &savloc);
}

settty()
{
	struct sgttyb s;

	s = savloc;
	s.sg_flags &=~ (CRMOD|ECHO|XTABS);
	s.sg_flags |= CBREAK;
	ioctl(0, TIOCSETP, &s);
	ioctl(0, TIOCSLTC, &nlchars);
}

resettty()
{
	ioctl(0, TIOCSETP, &savloc);
	ioctl(0, TIOCSLTC, &lsavechars);
}
