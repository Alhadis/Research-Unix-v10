#include <stdio.h>
#include <errno.h>
#include <dk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/filio.h>
#include <sys/ttyio.h>
#include <sys/stream.h>
#include <signal.h>
#include <ipc.h>
#include <libc.h>

/*
 * program to run a command
 * on another cpu on Datakit w/ transparent ioctls,
 * sometimes.
 */

#define	msglen(mp)	((mp)->losize + ((mp)->hisize<<8))

int     rem;            /* remote file descriptor */
extern	int mesg_ld;
int	nomesg;

#define MAXCHARS 8192
char args[MAXCHARS];

char *bldargs(argv)
	register char *argv[];
{
	register char *s=args, *t;
	while(t= *argv++){	/* assignment = */
		while(*s = *t++)
			if(s++ >= &args[MAXCHARS-1]){
				fprintf(stderr, "rx: arg list too long\n");
				exit(1);
			}
		*s++=' ';
	}
	s[-1]='\0';
	return(args);
}

char *
basename(s)
	register char *s;
{
	register char *t;
	extern char *strrchr();
	t=strrchr(s, '/');
	return(t? t+1 : s);
}

setmsgl(mp, n)
register struct mesg *mp;
int n;
{
	mp->losize = n;
	mp->hisize = n >> 8;
}

finish()
{

	if(ioctl(0, FIOLOOKLD, 0) == mesg_ld) {
		ioctl(0, FIOPOPLD, 0);
		ioctl(0, TIOCFLUSH, 0);
	}
	exit(0);
}

main(argc, argv)
char **argv;
{
	char *host, *cmd;

#ifdef LINT
	printf("", argc);
#endif LINT
        host = cmd = basename(argv[0]);
	if(strcmp(host, "rx")==0 || strcmp(host, "rexec")==0
	   || strcmp(host, "nrx")==0){
		host=argv[1];
		argv++;
	}
	if(host==0){
		fprintf(stderr, "usage: %s host [command]\n", cmd);
		exit(1);
	}
	if(argv[1]==0){
		execl("/usr/ipc/bin/ndcon", argv[1], argv[1], (char *)0);
		perror("ndcon");
		exit(1);
	}
        rem = ipcmesgexec(host, "heavy", bldargs(&argv[1]));
        if (rem<0) {
		fprintf(stderr, "%s: call to %s failed: %s\n", cmd , host, errstr);
                exit(1);
	}
	setmesg();
	signal(SIGHUP, finish);
	signal(SIGPIPE, SIG_IGN);
	go(rem);
	finish();
	/* NOTREACHED */
}

/*
 * if standard input and output are the same,
 * try to push the message line discipline
 * otherwise we're in a pipeline and it's not safe
 */
setmesg()
{
	struct stat st0, st1;

	if (fstat(0, &st0) < 0 || fstat(1, &st1) < 0
	||  st0.st_dev != st1.st_dev || st0.st_ino != st1.st_ino) {
		nomesg = 1;
		return;
	}
	if(ioctl(0, FIOPUSHLD, &mesg_ld) < 0)
		nomesg = 1;
}

/*
 * hack:
 * until some message comes back from the remote,
 * ignore local data
 */
go(fd)
{
	int rbits, wbits, srbits;
	int n;
	char buf[4096+MSGHLEN];
	struct mesg *mp;

	mp = (struct mesg *) buf;
	wbits = 0;
	srbits = (1<<0)|(1<<fd);
	while(1){
		rbits = srbits;
		if(select(20, (struct fd_set*)&rbits, (struct fd_set*)&wbits, 20000) < 0){
			if(errno != EINTR)
				return;
			continue;
		}
		if(rbits & 1){
			n = rdmesg(0, buf, sizeof(buf));
			if(n <= 0) {
				srbits &=~ 1;
				continue;
			}
			mp->magic = MSGMAGIC;	/* temporary safety */
			if(write(fd, buf, n) != n)
				return;
			if(mp->type == M_FLUSH)
				remflush();
		}
		if(rbits & (1<<fd)){
			n = read(fd, buf, sizeof(buf));
			if(n <= 0)
				return;
			if(mp->type == M_HANGUP)
				return;
			if(mp->type == M_IOCTL){
				doioctl(buf, n);
			} else {
				mp->magic = MSGMAGIC;	/* temp safety */
				if(wrmesg(1, buf, n) != n)
					return;
			}
		}
	}
}

#define	MAXEOF	10

rdmesg(f, buf, n)
char *buf;
{
	register struct mesg *mp;
	static int eof = 0;

	if (nomesg == 0)
		return (read(f, buf, n));
	if (eof > MAXEOF)
		return (0);
	n = read(f, buf + MSGHLEN, n - (2 * MSGHLEN));
	if (n < 0)
		return (n);
	mp = (struct mesg *)buf;
	if (n == 0) {
		eof++;
		mp->type = M_DELIM;
		mp->magic = MSGMAGIC;
		setmsgl(mp, 0);
		return (MSGHLEN);
	}
	eof = 0;
	setmsgl((struct mesg *)buf, n);
	mp->type = M_DATA;
	mp->magic = MSGMAGIC;
	mp = (struct mesg *)(buf + MSGHLEN + n);
	mp->type = M_DELIM;
	mp->magic = MSGMAGIC;
	setmsgl((struct mesg *)buf, 0);
	return (n + 2*MSGHLEN);
}

wrmesg(f, buf, n)
char *buf;
{
	if (nomesg == 0)
		return (write(f, buf, n));
	if (((struct mesg *)buf)->type != M_DATA)
		return (n);	/* toss it */
	if (write(f, buf + MSGHLEN, n - MSGHLEN) != n - MSGHLEN)
		return (-1);
	return (n);
}

doioctl(buf, n)
char *buf;
{
	struct mesg *mp;
	struct iofoo{
		int cmd;
		union{
			int i;
			char errno;
			struct insld insld;
		} u;
	} *iop;
	int cmd, ld;

	iop = (struct iofoo *)(buf + MSGHLEN);
	mp = (struct mesg *) buf;
	if (nomesg) {
		errno = ENOTTY;
		goto bad;
	}
	cmd = iop->cmd;
	n -= MSGHLEN;
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
		write(1, buf, MSGHLEN + msglen(mp));
		return;
	}
	/* locally successful */
	mp->type = M_IOCACK;
	mp->magic = MSGMAGIC;
	setmsgl(mp, n);
	write(rem, buf, MSGHLEN + msglen(mp));
	return;
bad:
	mp->type = M_IOCNAK;
	mp->magic = MSGMAGIC;
	setmsgl(mp, sizeof(struct iofoo));
	iop->u.errno = errno;
	write(rem, buf, MSGHLEN + msglen(mp));
}

remflush()
{
	char buf[5000];
	struct mesg *mp;

	mp = (struct mesg *) buf;
	mp->type = M_IOCTL;
	mp->magic = MSGMAGIC;
	setmsgl(mp, sizeof(int));
	write(rem, buf, MSGHLEN + msglen(mp));

	while(read(rem, buf, sizeof(buf)) > 0){
		if(mp->type == M_IOCNAK || mp->type == M_IOCACK){
			return;
		}
	}
}
