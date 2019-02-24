# 1 "mux.c"
# 3 "mux.c"


# 1 "/usr/include/sys/param.h"











# 14 "/usr/include/sys/param.h"



































# 1 "/usr/include/signal.h"






































typedef int	(*SIG_TYP)();
SIG_TYP signal();





# 48 "/usr/include/signal.h"











# 50 "/usr/include/sys/param.h"














































# 106 "/usr/include/sys/param.h"




















# 135 "/usr/include/sys/param.h"



































# 1 "/usr/include/sys/types.h"
















typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	struct	_physadr { int r[1]; } *physadr;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_short	ino_t;
typedef	long	swblk_t;
typedef	long	size_t;
typedef	long	time_t;
typedef	long	label_t[14];
typedef	u_short	dev_t;
typedef	long	off_t;
typedef long	portid_t;

# 40 "/usr/include/sys/types.h"






typedef struct		fd_set { unsigned long fds_bits[(128+sizeof(int)*8-1)/(sizeof(int)*8)]; } fd_set;






# 171 "/usr/include/sys/param.h"
# 173 "/usr/include/sys/param.h"














# 6 "mux.c"
# 1 "/usr/include/sys/types.h"




# 52 "/usr/include/sys/types.h"

# 7 "mux.c"
# 1 "/usr/include/sys/stream.h"







struct	queue {
	struct	qinit	*qinfo;		
	struct	block	*first;		
	struct	block	*last;		
	struct	queue	*next;		
	struct	queue	*link;		
	caddr_t	ptr;			
	short	count;			
	u_short	flag;
};















struct	qinit {
	int	(*putp)();		
	int	(*srvp)();		
	long	(*qopen)();		
	int	(*qclose)();		
	short	limit;			
	short	lolimit;		
};








struct	block {
	struct	block	*next;
	u_char	*rptr;
	u_char	*wptr;
	u_char	*lim;
	u_char	*base;
	char	type;
	char	class;
};




struct stdata {
	struct	queue *wrq;		
	struct	block *iocblk;		
	struct	inode *inode;		
	struct	proc *wsel;		
	struct	proc *rsel;		
	short	pgrp;			
	short	flag;
	char	count;			
};









struct	block	*getq();
int	putq();
struct	block	*allocb();
struct	queue	*backq();
struct	queue	*allocq();
struct	inode	*stopen();



































struct	kpassfd {
	union  {
		struct	file *fp;
		int	fd;
	} f;
	short	uid;
	short	gid;
	short	nice;
	char	logname[8];
};





struct mesg {
	char	type;
	u_char	magic;
	u_char	losize, hisize;
};



# 8 "mux.c"
# 1 "/usr/include/sys/ttyio.h"




struct sgttyb {
	char	sg_ispeed;		
	char	sg_ospeed;		
	char	sg_erase;		
	char	sg_kill;		
	short	sg_flags;		
};




struct ttydevb {
	char	ispeed;			
	char	ospeed;			
	short	flags;			
};




struct tchars {
	char	t_intrc;	
	char	t_quitc;	
	char	t_startc;	
	char	t_stopc;	
	char	t_eofc;		
	char	t_brkc;		
};




















































































# 9 "mux.c"
# 1 "/usr/include/sys/filio.h"







struct	insld {
	short	ld;
	short	level;
};




struct	passfd {
	int	fd;
	short	uid;
	short	gid;
	short	nice;
	char	logname[8];
};















# 10 "mux.c"
# 1 "/usr/include/signal.h"
# 58 "/usr/include/signal.h"

# 11 "mux.c"
# 1 "/usr/include/errno.h"



















































extern int errno;

# 12 "mux.c"
# 1 "/usr/jerq/include/jioctl.h"
















struct winsize
{
	char	bytesx, bytesy;
	short	bitsx, bitsy;
};
# 13 "mux.c"
# 1 "/usr/jerq/include/tty.h"





struct ttychars {
	char	flags0;		
	char	flags1;		
	char	erase;		
	char	kill;		
	char	intrc;		
	char	quitc;		
	char	startc;		
	char	stopc;		
	char	eofc;		
	char	brkc;		
};



struct ttycmesg {
	char		cmd;		
	char		chan;		
	struct ttychars	ttychars;
};







# 14 "mux.c"

# 1 "./msgs.h"

















# 16 "mux.c"




struct layer {
	char		chan;		
	char		busy;
	char		dx, dy;		
	short		bitsx, bitsy;	
	int		more;
	struct ttychars	ttychars;
	char		bchan;		
	char		bcount;
	char		bbuf[MAXPKTDSIZE];
};






char		*jerqprog;
char		*jerqstart;
char		*progname;
char		*shell;
int		quitflag;
fd_set		rdfd;
int		enabled=1;
int		blocked=0;
struct layer	layer[20];
char		buf[MAXPKTDSIZE+4];
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
	14, 1920,
	13, 960,
	12, 480,
	9, 120,
	7, 30,
	0, 960,		
};



struct Pchannel	pconvs[16];
struct Pconfig	pconfig	= {

	write,
# 96 "mux.c"

	receive,
	(void(*)())creceive,
};

# 107 "mux.c"






# 116 "mux.c"


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
			write(fd, "#!/bin/sh\n", 10);
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
	ioctl(0, (('t'<<8)|8), &sttymodes);
	sttysave=sttymodes;
	if(ioctl(0, (('t'<<8)|23), &devmodes)>=0)	
		devsave=devmodes;
	else {
		devsave.ispeed=devmodes.ispeed=sttymodes.sg_ispeed;
		devsave.ospeed=devmodes.ospeed=sttymodes.sg_ospeed;
	}
	ioctl(0, (('t'<<8)|18), &tcharssave);
	setmodes(&ttychars, &sttymodes);
	settchars(&ttychars, &tcharssave);
	sttymodes.sg_flags|=040;
	sttymodes.sg_flags&=~010;
	ioctl(0, (('t'<<8)|9), &sttymodes);
	devmodes.flags|=040;
	ioctl(0, (('t'<<8)|24), &devmodes);
	signal(13, (int (*)())1);
# 168 "mux.c"

# 171 "mux.c"

	if(boot(jerqprog))
		quit("can't boot terminal program");
	booted++;
	ioctl(0, (('t'<<8)|13), 0);
	;
	;
	for(n=0; n<((sizeof speeds)/(sizeof speeds[0])); n++)
		if(speeds[n].speed<=devmodes.ospeed)
			break;
	n=speeds[n].bytes;
	Pxtimeout=((((((16-2)*sizeof(struct Packet)*NPCBUFS+n-1)/n))>( 3))?((((16-2)*sizeof(struct Packet)*NPCBUFS+n-1)/n)):( 3));
	Prtimeout=(((((sizeof(struct Packet)+n-1)/n))>( 2))?(((sizeof(struct Packet)+n-1)/n)):( 2));
	Pscanrate=1;
	;
	;
	;
	Pxfdesc=1;
	if(pinit(16)==-1)
		quit("bad protocol initialization");
	buf[0]=(('j'<<8)|4);
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
			quit(0);
		;
	}
	;
	quit("select");
}
scan()
{
	register fd, bit, n, ret=0;

	;
	;
	if(blocked){		
		for(fd=0, bit=1; fd<20; fd++, bit<<=1)
			if((bit&blocked) &&
			   psend(layer[fd].bchan, layer[fd].bbuf, layer[fd].bcount)!=-1){
				blocked&=~bit;
				return 0;
			}
	}
	rdfd.fds_bits[0]=enabled&~blocked;
	while(select(20, &rdfd, (fd_set *)0, (1000*3))==-1){
		if(errno!=4)
			return -1;
		ret++;
	}
	;
	;
	if(rdfd.fds_bits[0]==0) {
		if(Ptflag)
			ptimeout(14);
	}else for(fd=0, bit=1; fd<20; fd++, bit<<=1)
		if(bit&rdfd.fds_bits[0]){
			while((n=read(fd, buf, sizeof buf))==-1)
				if(errno!=4)
					return -1;
				else{
					;
					return 0;
				}
			;
			if(fd==0){
				if(n==0)
					quit("EOF on jerq");
				;
				precv(buf, n);
			} else if(unpack(fd, buf, n))
				enabled&=~bit;
		}
	return ret;	
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
		;
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

	;
	if(booted){
		for(i=0; i<20; i++)
			if(layer[i].busy)
				(void)close(i);
		layer[0].chan=0;
		sendioctl(0, (('j'<<8)|2));	
		for(i=Pxtimeout+1; Ptflag && i>0;){
			enabled=1;
			if((l=scan())==-1)
				break;
			i-=l;
		}
		alarm(0);
	}
	ioctl(0, (('t'<<8)|24), &devsave);
	ioctl(0, (('t'<<8)|9), &sttysave);
	ioctl(0, (('t'<<8)|14), 0);
	sleep(2);
	if (s) {
		write(2, progname, strlen(progname));
		write(2, ": ", 2);
		write(2, s, strlen(s));
		write(2, "\n", 1);
	}
# 318 "mux.c"

# 321 "mux.c"

	;
# 325 "mux.c"

	_exit(0);
}



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
	;
	;
	;
	if(n<=0)
		mp->type=02;
	else if(layer[fd].more>0){
		layer[fd].more-=n;
		return sendchars(fd, bp, n);
	}
	switch (mp->type) {
	case 02:
		;
		wait((int *)0);
		if(layer[fd].busy){
			sendioctl(fd, (('j'<<8)|9));
			
		}
		layer[fd].busy = 0;
		close(fd);
		enabled &= ~(1<<fd);
		return 1;
	case 07:
	default:
		;
		return 0;
	case 03:
	case 0:
		if(size==0){
			;
			return 0;
		}
		break;
	case 06:
		mp->type = 0105;
		switch (*(int *)(bp+4)) {
		case (('t'<<8)|9):
		case (('t'<<8)|10):
			tempchars=layer[fd].ttychars;
			setmodes(&tempchars, (struct sgttyb *)(bp+4+sizeof(int)));
			ttyset(fd, &tempchars);
			size = 0;
			break;
		case (('t'<<8)|8):
			tempchars=layer[fd].ttychars;
			getmodes(&tempchars, (struct sgttyb *)(bp+4+sizeof(int)));
			size=sizeof(struct sgttyb)+sizeof(int);
			ttyset(fd, &tempchars);
			break;
		case (('t'<<8)|17):
			tempchars=layer[fd].ttychars;
			settchars(&tempchars, (struct tchars *)(bp+4+sizeof(int)));
			ttyset(fd, &tempchars);
			size=0;
			break;
		case (('t'<<8)|18):
			gettchars(&layer[fd].ttychars, (struct tchars *)(bp+4+sizeof(int)));
			size=sizeof (struct tchars) + sizeof (int);
			break;
		case (('t'<<8)|24):
			size=0;
			break;
		case (('t'<<8)|23):
			size=sizeof(devsave)+sizeof(int);
			*(struct ttydevb *)(bp+4+sizeof(int))=devsave;
			break;
		case (('j'<<8)|3):
			size=0;
			break;
		case (('j'<<8)|5):
			*((int *)(bp+4))=(('j'<<8)|5);	

			((struct winsize *)(bp+4+sizeof(int)))->bytesx=layer[fd].dx;
			((struct winsize *)(bp+4+sizeof(int)))->bytesy=layer[fd].dy;
			((struct winsize *)(bp+4+sizeof(int)))->bitsx=layer[fd].bitsx;
			((struct winsize *)(bp+4+sizeof(int)))->bitsy=layer[fd].bitsy;
			size=sizeof(struct winsize)+sizeof(int);
			break;
		case (('j'<<8)|2):
		case (('j'<<8)|1):
		case (('j'<<8)|7):
		case (('j'<<8)|8):
			sendioctl(fd, *(int *)(bp+4));
			size=0;
			break;
		case (('j'<<8)|10):
			s=bp+4+sizeof(int);
			if(*s==0){
				if(chdir(cdbuf)!=0) 
					mp->type = 0106;
				cdbuf[0]=0;
			}else
				strcat(cdbuf, s);
			size = 0;
			break;
		default:
			mp->type = 0106;
			size = 0;
		}
		mp->magic = 0345;		
		mp->losize = size;
		mp->hisize = size>>8;
		write(fd, bp, 4+size);
		;
		;
		return 0;
	}
	if(size>MAXPKTDSIZE){
		layer[fd].more=size-MAXPKTDSIZE;
		size=MAXPKTDSIZE;
	}
	return sendchars(fd, bp+4, size);
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
	static struct ttycmesg m={128};
	for(i=0, p=(char *)tp, q=(char *)&layer[fd].ttychars; *p++==*q++; i++)
		if(i>=sizeof(struct ttychars))
			return;	
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

# 533 "mux.c"

	if(fd!=0 && layer[fd].busy==0)
		return 0;		
	if(cc>0)
		do{
			if((n=cc)>MAXPKTDSIZE)
				n=MAXPKTDSIZE;
			if(psend(l, s, n)==-1){
				;
				return fd;	
			}
		}while(s+=n, (cc-=n)>0);
	unblock(fd);
	return 0;
}
unblock(fd)
	int fd;
{
	register Pch_p	pcp=&pconvs[layer[fd].chan];

	;
	;
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
	;
	;
	;
	layer[0].chan=l;
	sendchars(0, ebuf, strlen(ebuf));
}
int
creceive(l, s, n)
	char *s;
{
	if(s[0]!=3 || n!=1)
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
		case 2:
		case 5:
			break;
		default:
			errno = 0;
			lerror(l, "inactive layer", *s);
		case 3:
			return 0;
		}
	while(cc--){
		;
		;
		;
		switch(*s++){
		case 1:	
			wrmesgb(i, s++, 1);
			delim(i);
			cc--;
			break;
		case 6:		
			delim(i);
			break;
		case 2:		
			if((i=doshell())==-1){
				lerror(l, "new layer", 2);
				;
				cc-=6;
				break;
			}
			layer[i].busy=1;
			layer[i].chan=l;
			ttyset(i, &ttychars);
			;
			;
			enabled |= (1<<i);
		case 8:
			layer[i].dx=*s++;
			;
			layer[i].dy=*s++;
			;
			layer[i].bitsx=(unsigned char)*s++;
			layer[i].bitsx|=(*s++)<<8;
			layer[i].bitsy=(unsigned char)*s++;
			layer[i].bitsy|=(*s++)<<8;
			cc-=6;
			break;
		case 3:		
			unblock(i);
			break;
		case 10:		
		case 11:
			break;
		case 4:		
			hupmsg.losize=0;
			hupmsg.hisize=0;
			hupmsg.magic=0345;
			hupmsg.type=02;
			write(i, (char *)&hupmsg, 4);
			layer[i].busy=0;
			pconvs[layer[i].chan].freepkts=1;	
			unblock(i);
			layer[i].ttychars=zerochars;
			break;
		case 5:		
			quitflag++;
			return 0;
		case 7:	
			wrmesgb(i, s, cc);
			return 0;
		case 12:	
			wrmesgb(i, s, cc);
			delim(i);
			return 0;
		case 9:	
			dosig(i, *s++);
			cc--;
			break;
		default:
			quit("unknown state incase 0");
		}
		;
	}
	return 0;
}
int
ltofd(l)
{
	register i;
	if(l==0)
		return 0;
	for(i=1; i<20; i++)
		if(layer[i].busy && layer[i].chan==l)
			return i;
	;
	return -1;
}
void
dosig(fd, sig)		
{
	char sigbuf[4+1];
	register struct mesg *mp;

	mp = (struct mesg *)sigbuf;
	mp->type=0101;
	mp->magic=0345;
	mp->losize=sizeof(char);
	mp->hisize=0;
	sigbuf[4]=sig;
	write(fd, sigbuf, 4+1);
	mp->type=0102;
	mp->magic=0345;
	mp->losize=0;
	mp->hisize=0;
	write(fd, sigbuf, 4);
}
void
wrmesgb(fd, cp, n)
	register char *cp;
	int n;
{
	char wrbuf[MAXPKTDSIZE+4];
	register char *bp;
	register struct mesg *mp;
	register int i;

	;
	mp = (struct mesg *)wrbuf;
	mp->type=0;
	mp->magic=0345;
	mp->losize=n;
	mp->hisize=n>>8;
	bp=wrbuf+4;
	i=n;
	while(i--)
		*bp++=*cp++;
	write(fd, wrbuf, 4+n);
}
delim(fd)
{
	struct mesg delbuf;

	;
	delbuf.type=03;
	delbuf.magic=0345;
	delbuf.losize=0;
	delbuf.hisize=0;
	write(fd, (char *)&delbuf, 4);
}
int
doshell()
{
	register fd, slave;
	int fp[2];
	;
	if(pipe(fp)<0){
		;
		return -1;
	}
	slave=fp[0];
	fd=fp[1];
	;
	if(ioctl(fd, (('f'<<8)|3), &mesg_ld) == -1){
		;
		close(fd);
		return -1;
	}
	setdelim(fd);	
	switch(fork()){
	case 0:
		
		for(fd=0; fd<5+16; fd++)
			if(fd!=slave)
				close(fd);
		dup(slave); dup(slave); dup(slave); dup(slave);
		close(slave);
		ioctl(0, (('t'<<8)|118), 0);
		signal(13, (int (*)())0);
		execlp(shell, shell, 0);
		perror(shell);
		exit(1);
		break;
	case -1:
		close(fd);
		close(slave);
		return -1;
	}
	;
	close(slave);
	return fd;
}
setdelim(fd)
{
	struct mesg m;

	m.type = 012;
	m.magic = 0345;
	m.losize = m.hisize = 0;
	write(fd, (char *)&m, 4);
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
		char buf[(sizeof(struct sgttyb)-1)+1];
		buf[(sizeof(struct sgttyb)-1)]=0;
		if(where==0 && (where=getenv("HOME"))==0){
			write(2, "cd: no HOME set\n", 16);
			return 1;
		}
		while(*where){
			strncpy(buf, where, (sizeof(struct sgttyb)-1));
			ioctl(0, (('j'<<8)|10), buf);
			where+=strlen(buf);
		}
		if(ioctl(0, (('j'<<8)|10), where)!=0){
			write(2, "cd: bad directory\n", 18);
			return 1;
		}
		return 0;
	}
	if(strcmp(argv[0], "exit")==0)
		return ioctl(0, (('j'<<8)|8), 0);
	write(2, "mux: no such command ", 21);
	write(2, argv[0], strlen(argv[0]));
	write(2, "\n", 1);
	return 1;
}
# 912 "mux.c"

