#if	TRACING | PSTATISTICS
#include <stdio.h>
#endif

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stream.h>
#include <sys/ttyio.h>
#include <sys/filio.h>
#include <signal.h>
#include <errno.h>
#include <jioctl.h>
#include <tty.h>

#include "msgs.h"
#include "pconfig.h"
#include "proto.h"
#include "packets.h"
#include "pstats.h"

/*
 *	One layer structure per file-descriptor
 */
struct layer {
	char		chan;		/* jerq protocol channel */
	char		busy;
	char		dx, dy;		/* Window size in characters */
	short		bitsx, bitsy;	/* Window size in bits */
	int		more;
	struct ttychars	ttychars;
	char		ptfile[14];	/* file name of slave */
	char		bchan;		/* holding area for blocked data */
	char		bcount;
	char		bbuf[MAXPKTDSIZE];
};

#define	NLAYERS	16		/* Same as in jerq itself */
#define	NSELFD	20		/* Maximum file descriptors for 'select' */
#define	SELTIMO	(1000*3)	/* 'select' timeout in millisecs */
#define	CDSIZE	(sizeof(struct sgttyb)-1)

char		*jerqprog;
char		*jerqstart;
char		umesgf[]=	"/dev/pt/pt109";
char		*progname;
char		*shell;
int		quitflag;
fd_set		rdfd;
int		enabled=1;
int		blocked=0;
struct layer	layer[NSELFD];
char		buf[MAXPKTDSIZE+MSGHLEN];
struct sgttyb	sttymodes;
struct sgttyb	sttysave;
struct ttydevb	devmodes;
struct ttydevb	devsave;
struct tchars	tcharssave;
struct ttychars	ttychars;
struct ttychars zerochars;
short		booted;
extern int	receive();
extern int	creceive();
void		dosig();
int		twrite();
void		wrmesgb();
extern char	*sys_errlist[];
extern char	*getenv();
extern char	*itoa();
extern char	*strcpy();
extern char	*strcat();
extern int	tty_ld;
extern int	mesg_ld;
extern int	errno;
extern int	sys_nerr;
extern int	strlen();
extern int	write();

struct{
	short	speed;
	short	bytes;
} speeds[] = {
	EXTA, 1920,
	B9600, 960,
	B4800, 480,
	B1200, 120,
	B300, 30,
	0, 960,		/* default */
};
#define	NSPEEDS	((sizeof speeds)/(sizeof speeds[0]))
#define	max(A,B)	(((A)>(B))?(A):(B))

struct Pchannel	pconvs[NLAYERS];
struct Pconfig	pconfig	= {
#	ifndef	TRACING
	write,
#	else
	twrite,
#	endif
	receive,
	(void(*)())creceive,
};

#ifdef	TRACING
int	twrite();
FILE	*tracefd;
void	trace();
void	tread();
#define	ifdeftracing(a)		a
#else
#define trace(a, b)
#define	tread(a, b)
#define	ifdeftracing(a)
#endif

#if	TRACING==1 || PDEBUG==1
char 	tracefile[]="traces";
#define	_exit	exit
#endif

main(argc, argv)
	char *argv[];
{
	register int n;
	char cmdline[64];
	progname=argv[0];
	for(n=1; n<argc; n++) {
		if(strcmp(argv[n], "-L")==0)
			jerqstart=argv[++n];
		else if(strcmp(argv[n], "-l")==0){
			register fd; char jfd[32];
			fd = creat(mktemp(strcpy(jfd, "/tmp/.muxXXXXXX")), 0744);
			++n, write(fd, argv[n], strlen(argv[n]));
			close(fd);
			strcpy(cmdline, jfd);
			strcat(strcat(strcat(cmdline, "; rm "), jfd), "; exit");
			jerqstart = cmdline;
		}else{
			n=argc+1; break;
		}
	}
	if(n!=argc)
		return service(argc-1, argv+1);
	if((jerqprog=getenv("MUXTERM")) == 0)
		jerqprog="/usr/jerq/lib/muxterm";
	if((shell=getenv("SHELL")) == 0)
		shell="sh";
	ioctl(0, TIOCGETP, &sttymodes);
	sttysave=sttymodes;
	if(ioctl(0, TIOCGDEV, &devmodes)>=0)	/* band-aid for old systems */
		devsave=devmodes;
	else {
		devsave.ispeed=devmodes.ispeed=sttymodes.sg_ispeed;
		devsave.ospeed=devmodes.ospeed=sttymodes.sg_ospeed;
	}
	ioctl(0, TIOCGETC, &tcharssave);
	setmodes(&ttychars, &sttymodes);
	settchars(&ttychars, &tcharssave);
	sttymodes.sg_flags|=RAW;
	sttymodes.sg_flags&=~ECHO;
	ioctl(0, TIOCSETP, &sttymodes);
	devmodes.flags|=F8BIT;
	ioctl(0, TIOCSDEV, &devmodes);
	signal(SIGPIPE, (int (*)())1);
#ifdef	TRACING
	tracefd=fopen(tracefile, "w");
#ifdef	PDEBUG
	ptracefd = tracefd;
#endif
#endif
#if	TRACING!=1 && PDEBUG==1
	ptracefd=fopen(tracefile, "w");
#endif
	if(boot(jerqprog))
		quit("can't boot terminal program");
	booted++;
	ioctl(0, TIOCEXCL, 0);
	trace(0, 0);
	trace("start\n", 0);
	for(n=0; n<NSPEEDS; n++)
		if(speeds[n].speed<=devmodes.ospeed)
			break;
	n=speeds[n].bytes;
	Pxtimeout=max((((NLAYERS-2)*sizeof(struct Packet)*NPCBUFS+n-1)/n), 3);
	Prtimeout=max(((sizeof(struct Packet)+n-1)/n), 2);
	Pscanrate=1;
	trace("speed = %d", n);
	trace(" xtimo = %d", Pxtimeout);
	trace(" rtimo = %d\n", Prtimeout);
	Pxfdesc=1;
	if(pinit(NLAYERS)==-1)
		quit("bad protocol initialization");
	buf[0]=JTIMO;
	buf[1]=Prtimeout;
	buf[2]=Pxtimeout;
	if (jerqstart && (n = strlen(jerqstart)) <= MAXPKTDSIZE-3) {
		strncpy(&buf[3], jerqstart, n);
		n += 3;
	} else
		n = 3;
	(void)psend(0, buf, n);
	while(scan()!=-1){
		if(quitflag)
			quit(NULL);
		ifdeftracing(fflush(tracefd));
	}
	trace("errno = %d\n", errno);
	quit("select");
}
scan()
{
	register fd, bit, n, ret=0;

	trace(0, 0);
	trace("enabled %o\n", enabled);
	if(blocked){		/* try to clear blocked channels */
		for(fd=0, bit=1; fd<NSELFD; fd++, bit<<=1)
			if((bit&blocked) &&
			   psend(layer[fd].bchan, layer[fd].bbuf, layer[fd].bcount)!=-1){
				blocked&=~bit;
				return 0;
			}
	}
	rdfd.fds_bits[0]=enabled&~blocked;
	while(select(NSELFD, &rdfd, (fd_set *)0, SELTIMO)==-1){
		if(errno!=EINTR)
			return -1;
		ret++;
	}
	trace(0, 0);
	trace("selected %o\n", rdfd.fds_bits[0]);
	if(rdfd.fds_bits[0]==0) {
		if(Ptflag)
			ptimeout(SIGALRM);
	}else for(fd=0, bit=1; fd<NSELFD; fd++, bit<<=1)
		if(bit&rdfd.fds_bits[0]){
			while((n=read(fd, buf, sizeof buf))==-1)
				if(errno!=EINTR)
					return -1;
				else{
					trace("read error, errno=%d\n", errno);
					return 0;
				}
			ifdeftracing(if(n==0) trace("0 byte read", 0));
			if(fd==0){
				if(n==0)
					quit("EOF on jerq");
				tread(buf, n);
				precv(buf, n);
			} else if(unpack(fd, buf, n))
				enabled&=~bit;
		}
	return ret;	/* used in quit */
}
psend_hold(chan, bufp, count, fd)
	int chan;
	register char *bufp;
	int count;
	register fd;
{
	int ret;
	register i;
	if((ret=psend(chan, bufp, count))==-1){
		trace("psend hold on fd %d\n", fd);
		layer[fd].bchan=chan;
		layer[fd].bcount=count;
		for(i=0; i<count; i++)
			layer[fd].bbuf[i]=bufp[i];
		blocked|=1<<fd;
	}
	return ret;
}
quit(s)
	register char *s;
{
	register l, i;

	ifdeftracing(trace("\nmux: %s\n", s); trace(0, 0); fflush(tracefd));
	if(booted){
		for(i=0; i<NSELFD; i++)
			if(layer[i].busy)
				(void)close(i);
		layer[0].chan=0;
		sendioctl(0, JTERM);	/* kill demux ==> boot terminal */
		for(i=Pxtimeout+1; Ptflag && i>0;){
			enabled=1;
			if((l=scan())==-1)
				break;
			i-=l;
		}
		alarm(0);
	}
	ioctl(0, TIOCSDEV, &devsave);
	ioctl(0, TIOCSETP, &sttysave);
	ioctl(0, TIOCNXCL, 0);
	sleep(2);
	if (s) {
		write(2, progname, strlen(progname));
		write(2, ": ", 2);
		write(2, s, strlen(s));
		write(2, "\n", 1);
	}
#ifdef	PSTATISTICS
	for(i=0, l=0; i<PS_NSTATS; i++)
		if (pstats[i].count) {
			if(l++==0)
				fprintf(stderr, "\nPacket protocol statistics:\n");
			fprintf(stderr, "%6ld %s\n"
				,pstats[i].count
#ifdef	PSTATSDESC
				,pstats[i].descp
#else
				,""
#endif
				);
			trace("%6ld ", pstats[i].count);
			trace("%s\n", pstats[i].descp);
		}
	fflush(stderr);
#endif
#if	TRACING == 1 || PDEBUG == 1
	fprintf(stderr, "\nThere are traces in '%s'\n", tracefile);
#endif
	ifdeftracing(fflush(tracefd); abort());
#ifdef	MONITOR
	monitor(0);
#endif
	_exit(0);
}
/*
 *	Unpack a message buffer bp of length n.
 */
unpack(fd, bp, n)
	register int fd;
	char *bp;
	int n;
{
	register struct mesg *mp;
	struct ttychars tempchars;
	static char cdbuf[256];
	char *s;
	register int size;
	mp = (struct mesg *)bp;
	size = mp->losize + (mp->hisize<<8);
	trace("unpack fd %d", fd);
	trace(" size %d", n);
	trace(" count %d\n", size);
	if(n<=0)
		mp->type=M_HANGUP;
	else if(layer[fd].more>0){
		layer[fd].more-=n;
		return sendchars(fd, bp, n);
	}
	switch (mp->type) {
	case M_HANGUP:
		trace("shell died\n", 0);
		wait((int *)0);
		if(layer[fd].busy){
			sendioctl(fd, JDELETE);
			/*(void)psend(layer[fd].chan, "Shell died.\n", 12);*/
		}
		layer[fd].busy = 0;
		close(fd);
		enabled &= ~(1<<fd);
		return 1;
	case M_DELAY:
	default:
		trace("ignore type 0%o\n", mp->type);
		return 0;
	case M_DELIM:
	case M_DATA:
		if(size==0){
			trace("size 0 %s ignored\n", mp->type==M_DELIM? "delim" : "data");
			return 0;
		}
		break;
	case M_IOCTL:
		mp->type = M_IOCACK;
		switch (*(int *)(bp+MSGHLEN)) {
		case TIOCSETP:
		case TIOCSETN:
			tempchars=layer[fd].ttychars;
			setmodes(&tempchars, (struct sgttyb *)(bp+MSGHLEN+sizeof(int)));
			ttyset(fd, &tempchars);
			size = 0;
			break;
		case TIOCGETP:
			tempchars=layer[fd].ttychars;
			getmodes(&tempchars, (struct sgttyb *)(bp+MSGHLEN+sizeof(int)));
			size=sizeof(struct sgttyb)+sizeof(int);
			ttyset(fd, &tempchars);
			break;
		case TIOCSETC:
			tempchars=layer[fd].ttychars;
			settchars(&tempchars, (struct tchars *)(bp+MSGHLEN+sizeof(int)));
			ttyset(fd, &tempchars);
			size=0;
			break;
		case TIOCGETC:
			gettchars(&layer[fd].ttychars, (struct tchars *)(bp+MSGHLEN+sizeof(int)));
			size=sizeof (struct tchars) + sizeof (int);
			break;
		case TIOCSDEV:
			size=0;
			break;
		case TIOCGDEV:
			size=sizeof(devsave)+sizeof(int);
			*(struct ttydevb *)(bp+MSGHLEN+sizeof(int))=devsave;
			break;
		case JMUX:
			size=0;
			break;
		case JWINSIZE:
			*((int *)(bp+MSGHLEN))=JWINSIZE;	/* answering JWINSIZE ioctl */
#define	BP	((struct winsize *)(bp+MSGHLEN+sizeof(int)))
			BP->bytesx=layer[fd].dx;
			BP->bytesy=layer[fd].dy;
			BP->bitsx=layer[fd].bitsx;
			BP->bitsy=layer[fd].bitsy;
			size=sizeof(struct winsize)+sizeof(int);
			break;
		case JTERM:
		case JBOOT:
		case JZOMBOOT:
			sendioctl(fd, *(int *)(bp+MSGHLEN));
			size=0;
			break;
		case JEXIT:
			sendioctl(fd, *(int *)(bp+MSGHLEN));
			size=0;
			break;
		case JCHDIR:
			s=bp+MSGHLEN+sizeof(int);
			if(*s==0){
				if(chdir(cdbuf)!=0) 
					mp->type = M_IOCNAK;
				cdbuf[0]=0;
			}else
				strcat(cdbuf, s);
			size = 0;
			break;
		default:
			mp->type = M_IOCNAK;
			size = 0;
		}
		mp->magic = MSGMAGIC;		/* safety net */
		mp->losize = size;
		mp->hisize = size>>8;
		write(fd, bp, MSGHLEN+size);
		trace("unpack ioctl type '%c'", *(int *)(bp+MSGHLEN)>>8);
		trace(" %d\n", *(int *)(bp+MSGHLEN)&0xff);
		return 0;
	}
	if(size>MAXPKTDSIZE){
		layer[fd].more=size-MAXPKTDSIZE;
		size=MAXPKTDSIZE;
	}
	return sendchars(fd, bp+MSGHLEN, size);
}
getmodes(tp, bp)
	register struct ttychars *tp;
	register struct sgttyb *bp;
{
	bp->sg_ispeed=sttysave.sg_ispeed;
	bp->sg_ospeed=sttysave.sg_ospeed;
	bp->sg_flags=(tp->flags1<<8)|(tp->flags0&0xFF);
	bp->sg_erase=tp->erase;
	bp->sg_kill=tp->kill;
}
setmodes(tp, bp)
	register struct ttychars *tp;
	register struct sgttyb *bp;
{
	tp->flags0=bp->sg_flags;
	tp->flags1=bp->sg_flags>>8;
	tp->erase=bp->sg_erase;
	tp->kill=bp->sg_kill;
}
gettchars(tp, bp)
	register struct ttychars *tp;
	register struct tchars *bp;
{
	bp->t_intrc=tp->intrc;
	bp->t_quitc=tp->quitc;
	bp->t_startc=tp->startc;
	bp->t_stopc=tp->stopc;
	bp->t_eofc=tp->eofc;
	bp->t_brkc=tp->brkc;
}
settchars(tp, bp)
	register struct ttychars *tp;
	register struct tchars *bp;
{
	tp->intrc=bp->t_intrc;
	tp->quitc=bp->t_quitc;
	tp->startc=bp->t_startc;
	tp->stopc=bp->t_stopc;
	tp->eofc=bp->t_eofc;
	tp->brkc=bp->t_brkc;
}
ttyset(fd, tp)
	struct ttychars *tp;
{
	register char *p, *q;
	register i;
	static struct ttycmesg m={JTTYC};
	for(i=0, p=(char *)tp, q=(char *)&layer[fd].ttychars; *p++==*q++; i++)
		if(i>=sizeof(struct ttychars))
			return;	/* no need to send; they're identical */
	m.chan=layer[fd].chan;
	layer[fd].ttychars=*tp;
	m.ttychars=*tp;
	(void)psend_hold(0, (char *)&m, sizeof m, fd);
}
sendioctl(fd, cmd)
{
	char ioctlvec[2];
	ioctlvec[0]=cmd;
	ioctlvec[1]=layer[fd].chan;
	if(psend_hold(0, ioctlvec, sizeof ioctlvec, fd)!=-1)
		unblock(fd);
}
int
sendchars(fd, s, cc)
	char *s;
	int cc;
{
	register int	l=layer[fd].chan;
	register int	n;

#	ifdef TRACING
	char buf[256];
	trace("write %d chars ", cc);
	trace("to layer %d\n", l);
	strncpy(buf, s, cc);
	buf[cc]=0;
	trace("<%s>\n", buf);
#	endif
	if(fd!=0 && layer[fd].busy==0)
		return 0;		/* layer was deleted, but there's still data */
	if(cc>0)
		do{
			if((n=cc)>MAXPKTDSIZE)
				n=MAXPKTDSIZE;
			if(psend(l, s, n)==-1){
				trace("layer %d blocked\n", l);
				return fd;	/* BUG */
			}
		}while(s+=n, (cc-=n)>0);
	unblock(fd);
	return 0;
}
unblock(fd)
	int fd;
{
	register Pch_p	pcp=&pconvs[layer[fd].chan];

	trace("unblock for layer %d", layer[fd].chan);
	trace(" freepkts=%d\n", pcp->freepkts);
	if(fd==0)
		return;
	if(pcp->freepkts>=1)
		enabled|=1<<fd;
	else
		enabled&=~(1<<fd);
}
void
lerror(l, s, t)
	int l;
	char *s, t;
{
	char	ebuf[128];
	int	busy;
	strcpy(ebuf, s);
	if(errno){
		strcat(ebuf, ": ");
		if(errno < sys_nerr)
			strcat(ebuf, sys_errlist[errno]);
		else{
			strcat(ebuf, "error ");
			strcat(ebuf, itoa(errno));
		}
		errno=0;
	}
	strcat(ebuf, "\n");
	trace("lerror type %d", t);
	trace(" for layer %d", l);
	trace(" %s\n", ebuf);
	layer[0].chan=l;
	sendchars(0, ebuf, strlen(ebuf));
}
int
creceive(l, s, n)
	char *s;
{
	if(s[0]!=C_UNBLK || n!=1)
		quit("bad control type");
	(void)receive(l, s, n);
}
int
receive(l, s, cc)
	int l;
	register char *s;
	register int cc;
{
	register int i;
	struct mesg hupmsg;
	if((i=ltofd(l))==-1)
		switch(*s){
		case C_NEW:
		case C_EXIT:
			break;
		default:
			errno = 0;
			lerror(l, "inactive layer", *s);
		case C_UNBLK:
			return 0;
		}
	while(cc--){
		trace("receive C type %d", *s);
		trace(" for layer %d", l);
		trace(" fd %d\n", i);
		switch(*s++){
		case C_SENDCHAR:	/* send layer char */
			wrmesgb(i, s++, 1);
			delim(i);
			cc--;
			break;
		case C_DELIM:		/* send delimiter */
			delim(i);
			break;
		case C_NEW:		/* make layer */
			if((i=doshell())==-1){
				lerror(l, umesgf, C_NEW);
				trace("can't open %s\n", umesgf);
				cc-=6;
				break;
			}
			layer[i].busy=1;
			layer[i].chan=l;
			ttyset(i, &ttychars);
			trace("new fd %d ", i);
			trace("layer %d ", l);
			enabled |= (1<<i);
		case C_RESHAPE:
			layer[i].dx=*s++;
			trace("x wid %d ", layer[i].dx);
			layer[i].dy=*s++;
			trace("y wid %d\n", layer[i].dy);
			layer[i].bitsx=(unsigned char)*s++;
			layer[i].bitsx|=(*s++)<<8;
			layer[i].bitsy=(unsigned char)*s++;
			layer[i].bitsy|=(*s++)<<8;
			cc-=6;
			break;
		case C_UNBLK:		/* unblock layer */
			unblock(i);
			break;
		case C_PUSHLD:		/* push ld onto stream */
			pushld(i);
			break;
		case C_POPLD:		/* pop ld from stream */
			popld(i);
			break;
		case C_DELETE:		/* delete layer */
			hupmsg.losize=0;
			hupmsg.hisize=0;
			hupmsg.magic=MSGMAGIC;
			hupmsg.type=M_HANGUP;
			write(i, (char *)&hupmsg, MSGHLEN);
			layer[i].busy=0;
			pconvs[layer[i].chan].freepkts=1;	/* hack */
			unblock(i);
			layer[i].ttychars=zerochars;
			break;
		case C_EXIT:		/* exit */
			quitflag++;
			return 0;
		case C_SENDNCHARS:	/* send cc characters */
			wrmesgb(i, s, cc);
			return 0;
		case C_KILL:	/* send layer signal */
			dosig(i, *s++);
			cc--;
			break;
		default:
			quit("unknown state incase 0");
		}
		ifdeftracing(if(cc<0) quit("bad count in receive"));
	}
	return 0;
}
int
ltofd(l)
{
	register i;
	if(l==0)
		return 0;
	for(i=1; i<NSELFD; i++)
		if(layer[i].busy && layer[i].chan==l)
			return i;
	trace("unknown layer %d\n", l);
	return -1;
}
void
dosig(fd, sig)		/* Interrupt shell */
{
	char sigbuf[MSGHLEN+1];
	register struct mesg *mp;

	mp = (struct mesg *)sigbuf;
	mp->type=M_SIGNAL;
	mp->magic=MSGMAGIC;
	mp->losize=sizeof(char);
	mp->hisize=0;
	sigbuf[MSGHLEN]=sig;
	write(fd, sigbuf, MSGHLEN+1);
	mp->type=M_FLUSH;
	mp->magic=MSGMAGIC;
	mp->losize=0;
	mp->hisize=0;
	write(fd, sigbuf, MSGHLEN);
}
void
wrmesgb(fd, cp, n)
	register char *cp;
	int n;
{
	char wrbuf[MAXPKTDSIZE+MSGHLEN];
	register char *bp;
	register struct mesg *mp;
	register int i;

	ifdeftracing(fprintf(tracefd, "mesg to fd %d: <%.*s>\n", fd, n, cp));
	mp = (struct mesg *)wrbuf;
	mp->type=M_DATA;
	mp->magic=MSGMAGIC;
	mp->losize=n;
	mp->hisize=n>>8;
	bp=wrbuf+MSGHLEN;
	i=n;
	while(i--)
		*bp++=*cp++;
	write(fd, wrbuf, MSGHLEN+n);
}
delim(fd)
{
	struct mesg delbuf;

	ifdeftracing(fprintf(tracefd, "delim fd %d\n", fd));
	delbuf.type=M_DELIM;
	delbuf.magic=MSGMAGIC;
	delbuf.losize=0;
	delbuf.hisize=0;
	write(fd, (char *)&delbuf, MSGHLEN);
}
popld(fd)
{
	register f, i;
	struct ttychars tempchars;
	struct sgttyb tb;
	struct tchars tc;
	errno=0;
	if((f=open(layer[fd].ptfile, 2))>0){
		if((i=ioctl(f, FIOLOOKLD, 0))==tty_ld) {
			i=0;
			if(ioctl(f, TIOCGETP, &tb)<0
			|| ioctl(f, TIOCGETC, &tc)<0)
				i=1;	/* hack for old systems */
			ioctl(f, FIOPOPLD, (struct sgttyb *)0);
			if(i==0){
				setmodes(&tempchars, &tb);
				settchars(&tempchars, &tc);
				ttyset(fd, &tempchars);
			}
		}
		else if(i>=0)
			lerror(layer[fd].chan, "mux warning: unknown line discipline", 0);
	}
	close(f);
	trace("popld file %s\n", layer[fd].ptfile);
	trace("popld file descriptor %d\n", f);
}
pushld(fd)
{
	register f, i;
	int tty;
	struct tchars tc;
	struct sgttyb tb;
	if((f=open(layer[fd].ptfile, 2))>0){
		if((i=ioctl(f, FIOLOOKLD, 0))==-1){
			ioctl(f, FIOPUSHLD, (struct sgttyb *)&tty_ld);
			getmodes(&layer[fd].ttychars, &tb);
			gettchars(&layer[fd].ttychars, &tc);
			ioctl(f, TIOCSETP, &tb);
			ioctl(f, TIOCSETC, &tc);
		}else
			; /* can't warn; program could be e.g. jim! */
	}
	trace("pushld errno  file %d\n", errno);
	close(f);
	trace("pushld file descriptor  file %s\n", layer[fd].ptfile);
}
int
doshell()
{
	register fd, slave;
	trace("do shell\n", 0);
	if((fd=ptopen(umesgf))<0){
		trace("can't open %s\n", umesgf);
		return -1;
	}
	if((slave=open(umesgf, 2))==-1){
		trace("can't open %s\n", umesgf);
		close(fd);
		return -1;
	}
	trace("opened %s\n", umesgf);
	strcpy(layer[fd].ptfile, umesgf);
	if(ioctl(fd, FIOPUSHLD, &mesg_ld) == -1){
		trace("FIOPUSHLD fails, errno=%d\n", errno);
		close(fd);
		return -1;
	}
	switch(fork()){
	case 0:
		/* close every file descriptor in sight, and then some */
		for(fd=0; fd<5+NLAYERS; fd++)
			if(fd!=slave)
				close(fd);
		dup(slave); dup(slave); dup(slave); dup(slave);
		close(slave);
		ioctl(0, TIOCSPGRP, 0);
		signal(SIGPIPE, (int (*)())0);
		execlp(shell, shell, 0);
		perror(shell);
		exit(1);
		break;
	case -1:
		close(fd);
		return -1;
	}
	trace("doshell succeeds\n", 0);
	close(slave);
	return fd;
}
int
boot(s)
	char *s;
{
	if(system("/usr/jerq/bin/32ld", "32ld", s))
		return 1;
	sleep(2);
	return 0;
}
int
system(s, t, u)
char *s, *t, *u;
{
	int status, pid, l;

	if ((pid=fork())==0){
		execl(s, t, u, 0);
		_exit(127);
	}
	while ((l = wait(&status)) != pid && l != -1)
		;
	if (l == -1)
		status = -1;
	return(status);
}
char *
itoa(i)
	register int i;
{
	static char str[11];
	register char *	sp = &str[sizeof str];

	*--sp = '\0';
	if(i>0){
		do
			*--sp=i%10+'0';
		while((i/=10)>0);
	}else
		*--sp='0';
	return sp;
}
service(argc, argv)
	char *argv[];
{
	if(strcmp(argv[0], "cd")==0){
		char *where=argv[1];
		char buf[CDSIZE+1];
		buf[CDSIZE]=0;
		if(where==0 && (where=getenv("HOME"))==0){
			write(2, "cd: no HOME set\n", 16);
			return 1;
		}
		while(*where){
			strncpy(buf, where, CDSIZE);
			ioctl(0, JCHDIR, buf);
			where+=strlen(buf);
		}
		if(ioctl(0, JCHDIR, where)!=0){
			write(2, "cd: bad directory\n", 18);
			return 1;
		}
		return 0;
	}
	if(strcmp(argv[0], "exit")==0)
		return ioctl(0, JEXIT, 0);
	write(2, "mux: no such command ", 21);
	write(2, argv[0], strlen(argv[0]));
	write(2, "\n", 1);
	return 1;
}
#ifdef	TRACING
/*VARARGS1*/
void
trace(s, a)
	char *s, *a;
{
	long t;
	extern long time();
	extern char *ctime();

	if(s)
		fprintf(tracefd, s, a);
	else{
		(void)time(&t);
		fprintf(tracefd, "%.9s", ctime(&t)+11);
	}
}
int
twrite(fd, s, n)
	unsigned char *	s;
{
	register i;
	fprintf(tracefd, "to jerq: ");
	for(i=0; i<n; i++)
		fprintf(tracefd, "<%o>", s[i]);
	fprintf(tracefd, "\n");
	return write(fd, s, n);
}
void
tread(s, n)
	unsigned char *	s;
{
	register i;
	fprintf(tracefd, "from jerq: ");
	for(i=0; i<n; i++)
		fprintf(tracefd, "<%o>", s[i]);
	fprintf(tracefd, "\n");
}
#endif
