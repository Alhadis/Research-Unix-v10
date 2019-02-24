#include <sys/types.h>
#include <sys/stream.h>
#include <sys/stat.h>
#include <sys/ttyio.h>
#include <sys/filio.h>
#include <sys/dkio.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <ipc.h>
#include <libc.h>
#include <utsname.h>

/*
 * program to connect to
 * another cpu on Datakit w/ transparent ioctls
 */

int     rem=-1;		/* remote file descriptor */
extern	int mesg_ld;
struct	sgttyb sgbuf;
struct	tchars tcbuf;
int	ttyrem=0;	/* true if tty ld is remote */
char	*ttyn;
int	perms;

struct sigmsg {
	struct	mesg m;
	char	sig[1];
};

struct buffer {
	int	rfd,wfd;
	int	rmesgld;
	int	wmesgld;
	char	*rptr;
	char	*wptr;
	char	data[4096+MSGHLEN];
};
static struct buffer ttyb;
static struct buffer netb;

struct mesg *getmsg();
int fillbuf();

#define	msglen(mp)	((mp)->losize + ((mp)->hisize<<8))

int lfd;
char *av0;
int notdef;
int (*quitsig)();
int debug;

usage()
{
	fprintf(stderr, "usage: %s [-l] host\n", av0);
	exit(1);
}

main(argc, argv)
char **argv;
{
	char *host;
	struct stat sb;
	int all, ld, nold;
	extern int hupcatch();
	char *ttyname();

	nold = 0;
	av0 = strrchr(argv[0], '/');
	av0 = av0==NULL ? argv[0] : av0+1;
       	while(argc>1 && argv[1][0]=='-'){
		char *cp;
		for(cp=&argv[1][1]; *cp; cp++)
			switch (*cp) {
			case 'l':
				av0 = "con -l";
				break;
			case 'd':
				debug = 1;
				close(2);
				creat("/tmp/con.debug", 0666);
				break;
			case 'n':
				nold = 1;
				break;
			default:
				usage(); 
			}
		argv++;
		argc--;
	}
	host=argv[1];
	if(strcmp(host, "helix")==0
	|| strcmp(host, "spindle")==0
	|| strcmp(host, "hobocpu")==0
	|| strcmp(host, "fornax")==0)
		av0 = "dcon";
/*
	if (host==NULL)
		usage();
*/
	ioctl(0, TIOCGETP, &sgbuf);
	ioctl(0, TIOCGETC, &tcbuf);
	quitsig = signal(SIGQUIT, hupcatch);

	all = strcmp(av0, "con")==0;
	if (strcmp(av0, "nogin")==0)
		rem = trynogin(host);
	if (all || strcmp(av0, "ndcon")==0)
		rem = trymesg(host);
	if (rem<0 && (all || strcmp(av0, "dcon")==0))
		rem = trynomesg(host);
/*
	if (rem<0 && (all || strcmp(av0, "9con")==0))
		rem = try9con(host);
*/
	if (rem<0 && (all || strcmp(av0, "rogin")==0 || strcmp(av0, "rlogin")==0))
		rem = tryrogin(host);
	if (rem<0 && (all || strcmp(av0, "con -l")==0))
		rem = trysimple(host);
	if (rem<0) {
		fprintf(stderr, "%s: %s connecting to %s\n", av0, errstr, host);
		exit(1);
	}

	/*
	 *  we really have to avoid leaving mesg_ld's pushed
	 */
	signal(SIGHUP, hupcatch);
	signal(SIGPIPE, hupcatch);
	if ((ttyn = ttyname(0)) != NULL
	&&   stat(ttyn, &sb) >= 0) {
		perms = sb.st_mode & ~S_IFMT;
		chmod(ttyn, 0);
	}

	/*
	 *  push mesg_ld onto stdin and stdout if possible
	 */
	ttyb.rmesgld = ttyb.wmesgld = netb.rmesgld = netb.wmesgld = 0;
	if (!nold){
		/* race ahead possible */
		ioctl(0, TIOCFLUSH, (char *)0);
		if(ioctl(0, FIOPUSHLD, &mesg_ld)>=0)
			ttyb.rmesgld=1;
		if(ioctl(1, FIOLOOKLD, &ld)>=0
		&& (ld==mesg_ld || ioctl(1, FIOPUSHLD, &mesg_ld)>=0))
			ttyb.wmesgld=1;
	}

	/*
	 * see if remote side accepts mesg_ld packets
	 */
	netb.rmesgld = netb.wmesgld = ttyrem==0;
	if(debug)
		fprintf(stderr, "connected\n");
	go(rem);
	finish(0);
	/* NOTREACHED */
}

/* try to set up a message line discipline connection */
trymesg(host)
	char *host;
{
	int fd;

	fd = ipcopen(ipcpath(host, "dk", "mesgdcon"), "light hup");
	if (fd<0)
		return fd;
	if (ipclogin(fd)<0) {
		fprintf(stderr, "%s: can't log in\n", av0);
		exit(1);
	}
	return fd;
}

/* try to set up a no line discipline connection */
trynomesg(host)
	char *host;
{
	int fd;

	fd = ipcopen(ipcpath(host, "dk", "dcon"), "light hup");
	if (fd<0)
		return fd;
	if (ipclogin(fd)<0) {
		fprintf(stderr, "%s: can't log in\n", av0);
		exit(1);
	}
	remtty(fd);
	return fd;
}

/*
 *  try to set up an dcon-like connection with local echo
 */
try9con(host)
	char *host;
{
	int fd;
	char service[256];

	sprintf(service, "nonet!%s", host);
	fd = ipcopen(ipcpath("r70", "dk", service), "light hup bsdauth");
	if (fd<0)
		return fd;
	ttyrem = 1;
	return fd;
}

/*
 *  try to set up an rlogin-like connection with local echo
 *
 *  the authentication string is passwd by ipcopen because of the bsdauth
 *  parameter.   The ipcrogin passes the terminal type and gets the authentication
 *  reply.
 */
trynogin(host)
	char *host;
{
	int fd;
	char *term;

	fd = ipcopen(ipcpath(host, "tcp", "login"), "light hup bsdauth");
	if (fd<0)
		return fd;
	term = getenv("TERM");
	ipcrogin(fd, term?term:"");
	ttyrem = 1;
	return fd;
}

/*
 *  try to set up a rlogin-like connection with remote echo
 */
tryrogin(host)
	char *host;
{
	int fd;
	char *term;

	fd = ipcopen(ipcpath(host, "tcp", "login"), "light hup bsdauth");
	if (fd<0)
		return fd;
	term = getenv("TERM");
	ipcrogin(fd, term?term:"");
	remtty(fd);
	return fd;
}

/* try to set up a non-authenticated connection */
trysimple(host)
	char *host;
{
	int fd;

	fd = ipcopen(ipcpath(host, "dk", ""), "light hup 1200");
	if (fd<0)
		return fd;
	remtty(fd);
	return fd;
}

/* set up for tty control to be remote */
remtty(fd)
{
	struct	sgttyb sg;
	struct	tchars tc;

	sg = sgbuf;
	sg.sg_flags &= ~(CRMOD|ECHO|XTABS);
	sg.sg_flags |= CBREAK;
	ioctl(0, TIOCSETP, &sg);
	tc.t_quitc = tcbuf.t_quitc;
	tc.t_intrc = tc.t_startc = tc.t_stopc = tc.t_eofc = tc.t_brkc = -1;
	ioctl(0, TIOCSETC, &tc);
	ttyrem = 1;
	/*
	 *  HACK!!!! to make select work when connected to a
	 *  delimited stream --- turn off delimiters.
	 */
	ioctl(fd, DIOCSTREAM, 0);
}
	

go(fd)
{
	int rbits, wbits;
	struct mesg *mp;
	int dgenerated=0;
	int lasttype=0;

	wbits = 0;
	ttyb.rptr = ttyb.wptr = ttyb.data;
	ttyb.rfd = 0;
	ttyb.wfd = 1;
	netb.rptr = netb.wptr = netb.data;
	netb.rfd = fd;
	netb.wfd = fd;
	while(1){
		rbits = 1 | (1<<fd);
		if(select(20, (fd_set*)&rbits, (fd_set*)&wbits, 20000) < 0){
			if(errno != EINTR)
				return;
			continue;
		}
		if(rbits & 1)
			if (fillbuf(&ttyb)<0)
				return;
		if(rbits & (1<<fd))
			if (fillbuf(&netb)<0)
				return;
		while(mp = getmsg(&ttyb)) {
			if (mp->type==M_DELIM) {
				if (ttyrem && lasttype==M_DELIM) {
					sendmsg(&netb, M_DATA, &tcbuf.t_eofc, 1);
					continue;
				}
			}
			lasttype = mp->type;
			if (mp->type==M_SIGNAL)
				switch(((struct sigmsg *)mp)->sig[0]) {
				case SIGQUIT:
					dolocal(mp);
					continue;
				case SIGINT:
					if(!ttyrem)
						break;
					sendmsg(&netb, M_DATA, &tcbuf.t_intrc, 1);
					continue;
				}
			mp->magic = MSGMAGIC;	/* temp safety */
			if(writemsg(&netb, mp)<0)
				return;
			if(!ttyrem && mp->type == M_FLUSH) {
				remflush();
				rbits = 0;
			}
		}
		while(mp = getmsg(&netb)){
			if(mp->type == M_HANGUP)
				return;
			if(mp->type == M_IOCTL){
				doioctl(mp);
			} else if(dgenerated && mp->type==M_DELIM){
				dgenerated = 0;
			} else {
				if(writemsg(&ttyb, mp)<0)
					return;
				if(mp->type==M_DATA) {
					sendmsg(&ttyb, M_DELIM, (char *)mp, 0);
					dgenerated = 1;
				}
			}
		}
	}
}

/* read in at least a complete message */
fillbuf(bp)
	register struct buffer *bp;
{
	register int n;
	struct mesg *mp;
	static int neofs;	

	if(!bp->rmesgld) {
		bp->rptr = bp->data;
		bp->wptr = bp->data+MSGHLEN;
		mp = (struct mesg *)bp->rptr;
	} else {
		/* compact */
		n = bp->wptr-bp->rptr;
		if (n > 0 && bp->rptr!=bp->data)
			memcpy(bp->data, bp->rptr, n);
		bp->rptr = bp->data;
		bp->wptr = bp->rptr + n;
	}

	/* try reading */
	n = bp->data+sizeof(bp->data) - bp->wptr;
	if (n > 0) {
		if(debug)
			fprintf(stderr, "read(%d)\n", bp->rfd);
		n = read(bp->rfd, bp->wptr, n);
		if(debug){
			char *cp;
			fprintf(stderr, "read(%d) returns %d\n", bp->rfd, n);
			for(cp = bp->wptr; cp < bp->wptr+n; cp++)
				fprintf(stderr, "<%o>\n", *cp);
		}
		if(n<=0){
			if(n<0 || ++neofs>4)
				return -1;
			if(!bp->rmesgld)
				bp->wptr-=MSGHLEN;
		}
		bp->wptr += n;
	} else
		neofs = 0;

	if(!bp->rmesgld) {
		mp->type = M_DATA;
		mp->magic = MSGMAGIC;
		setmsgl(mp, n);
	}
	return 0;
}

/* get the next (buffered) message */
struct mesg *
getmsg(bp)
	struct buffer *bp;
{
	struct mesg *mp = (struct mesg *)bp->rptr;
	int n = bp->wptr - bp->rptr;

	if (n<MSGHLEN || n<MSGHLEN+msglen(mp)) {
		if (msglen(mp)+MSGHLEN > sizeof(bp->data))
			bp->rptr = bp->wptr = bp->data;
		return NULL;
	}
	bp->rptr += MSGHLEN + msglen(mp);
	mp->magic = MSGMAGIC;
	return mp;
}

int
writemsg(bp, mp)
	struct buffer *bp;
	struct mesg *mp;
{
	int len,n;
	char *cp;

	if(bp->wmesgld) {
		len = MSGHLEN + msglen(mp);
		cp = (char *)mp;
	} else if(mp->type==M_DATA){
		len = msglen(mp);
		cp = ((char *)mp)+MSGHLEN;
	} else
		return 0;
	if(debug)
		fprintf(stderr, "write(%d)\n", bp->wfd);
	n=write(bp->wfd, cp, len);
	if(debug)
		fprintf(stderr, "write(%d) returns %d\n", bp->wfd, n);
	if(n!=len)
		return -1;
	return 0;
}

doioctl(mp)
	struct mesg *mp;
{
	struct iofoo{
		int cmd;
		union{
			int i;
			char errno;
			struct insld insld;
		} u;
	} *iop;
	int cmd, ld;
	int n;

	iop = (struct iofoo *)(((char*)mp) + MSGHLEN);
	cmd = iop->cmd;
	n = msglen(mp);
	n -= sizeof(iop->cmd);
	switch(cmd){
	case FIOLOOKLD:
		if(n > 0)
			ld = iop->u.i;
		else
			ld = 0;
		ld++;
		if(ioctl(1, FIOLOOKLD, &ld) < 0)
			goto bad;
		iop->cmd = ld;
		n = sizeof(iop->cmd);
		break;

	case FIOPOPLD:
		if(n > 0)
			ld = iop->u.i;
		else
			ld = 0;
		ld++;
		if(ioctl(1, FIOPOPLD, &ld) < 0)
			goto bad;
		n = 0;
		break;

	case FIOPUSHLD:
		iop->u.insld.level = 0;
		/* fall through... */
	case FIOINSLD:
		iop->u.insld.level++;
		if(ioctl(1, FIOINSLD, &(iop->u.insld)) < 0)
			goto bad;
		n = 0;
		break;

	default:
		mp->magic = MSGMAGIC;	/* safety */
		writemsg(&ttyb, mp);
		return;
	}
	/* locally successful */
	mp->type = M_IOCACK;
	mp->magic = MSGMAGIC;
	setmsgl(mp, n);
	writemsg(&netb, mp);
	return;
bad:
	mp->type = M_IOCNAK;
	mp->magic = MSGMAGIC;
	setmsgl(mp, sizeof(struct iofoo));
	iop->u.errno = errno;
	writemsg(&netb, mp);
}

remflush()
{
	char buf[5000];
	struct mesg *mp;

	mp = (struct mesg *) buf;
	mp->type = M_IOCTL;
	setmsgl(mp, sizeof(int));
	mp->magic = MSGMAGIC;
	writemsg(&netb, (struct mesg *)buf);

	while(read(rem, buf, sizeof(buf)) > 0){
		if(mp->type == M_HANGUP){
			close(rem);
			return;
		}
		if(mp->type == M_IOCNAK || mp->type == M_IOCACK)
			return;
	}
}

setmsgl(mp, n)
	register struct mesg *mp;
	int n;
{
	mp->losize = n;
	mp->hisize = n >> 8;
}

hupcatch()
{
	finish(0);
}

finish(sts)
{
	struct mesg m;

	if(ioctl(0, FIOLOOKLD, 0) == mesg_ld) {
		ioctl(0, FIOPOPLD, 0);
		ioctl(0, TIOCFLUSH, (char *)0);
	}
	if(ioctl(1, FIOLOOKLD, 0) == mesg_ld)
		ioctl(1, FIOPOPLD, 0);
	if (ttyn)
		chmod(ttyn, perms);
	ioctl(0, TIOCSETP, &sgbuf);
	ioctl(0, TIOCSETC, &tcbuf);
	signal(SIGQUIT, quitsig);
	if(sts==0)
		write(2, "Eof\n", 4);
	exit(sts);
}

dolocal(mp)
	register struct mesg *mp;
{
	char lbuf[128+1];
	register char *lp;
	struct sgttyb nsgbuf;
	struct tchars ntcbuf;
	int done;
	static char prompt[32];
	struct utsname uts;

	
	ioctl(0, FIOPOPLD, (char *)0);
	signal(SIGQUIT, quitsig);
	ioctl(0, TIOCFLUSH, (char *)0);
	ioctl(0, TIOCGETP, &nsgbuf);
	ioctl(0, TIOCSETP, &sgbuf);
	ioctl(0, TIOCGETC, &ntcbuf);
	ioctl(0, TIOCSETC, &tcbuf);
	if(prompt[0]==0){
		uname(&uts);
		strncpy(prompt, uts.nodename, sizeof(uts.nodename));
		prompt[sizeof(uts.nodename)] = 0;
		strcat(prompt, ">> ");
	}
	if (ttyn)
		chmod(ttyn, perms);
	for (done=0;!done;) {
		lp = lbuf;
		printf(prompt);
		fflush(stdout);
		while (lp < &lbuf[128] && read(0, lp, 1)>0 && *lp!='\n')
			lp++;
		*lp = '\0';
		switch(*lbuf) {
		case 'b':
			if (ttyrem)
				ioctl(netb.wfd, TIOCSBRK, 0);
			else
				sendmsg(&netb, M_BREAK, (char *)NULL, 0);
			mp->type = 0;
			done = 1;
			break;
		case 'i':
			if (ttyrem) {
				sendmsg(&netb, M_DATA, &tcbuf.t_quitc, 1);
				mp->type = 0;
			}
			done = 1;
			break;
		case 'q':
		case 'x':
		case '.':
			finish(1);
		case '!':
			system(lbuf+1);
			printf("!!\n");
			fflush(stdout);
			mp->type = 0;
			done = 1;
			break;
		case '\0':
			mp->type = 0;
			done = 1;
			break;
		default:
			printf("[qx.] to exit, i for quit signal, b for break, !cmd for shell\n");
			fflush(stdout);
		}
	}
	ioctl(0, TIOCSETP, &nsgbuf);
	ioctl(0, TIOCSETC, &ntcbuf);
	signal(SIGQUIT, hupcatch);
	ioctl(0, FIOPUSHLD, &mesg_ld);
	if (ttyn)
		chmod(ttyn, 0);
	if (mp->type) {
		mp->magic = MSGMAGIC;	/* safety */
		writemsg(&netb, mp);
	}
}

sendmsg(bp, type, cp, len)
	struct buffer *bp;
	int type, len;
	char *cp;
{
	struct amesg {
		struct mesg m;
		char body[128];
	} am;

	am.m.magic = MSGMAGIC;
	am.m.type = type;
	setmsgl(&am.m, len);
	if (cp!=NULL && len>0)
		memcpy(am.body, cp, len);
	writemsg(bp, (struct mesg *)&am);
}
