#include <stddef.h>
#include <stdarg.h>
#include <signal.h>
#include <fio.h>
#include <errno.h>
#include <time.h>

/* for Tenth Edition systems */
#define LP	"/usr/bin/lp"
/* for System V or BSD systems */
/* #define LP	"/v/bin/lp" */

#define LPDAEMONLOG	"/tmp/lpdaemonl"

#define ARGSIZ 4096
#define NAMELEN 11

char argvstr[ARGSIZ];		/* arguments after parsing */
char *argvals[ARGSIZ/2+1];	/* pointers to arguments after parsing */
int ascnt = 0, argcnt = 0;	/* number of arguments parsed */
/* for 'stuff' gleened from lpr cntrl file */
struct jobinfo {
	char user[NAMELEN+1];
	char host[NAMELEN+1];
} *getjobinfo();

#define MIN(a,b)	((a<b)?a:b)

#define	CPYFIELD(src, dst)	{ while (*(src)!=' ' && *(src)!='\t' && *(src)!='\r' && *(src)!='\n' && *(src)!='\0') *(dst)++ = *(src)++; }

#define	ACK()	write(1, "", 1)
#define NAK()	write(1, "\001", 1)

#define LNBFSZ	4096
char lnbuf[LNBFSZ];
int readline();

#define	RDSIZE 512
char jobbuf[RDSIZE];

int datafd[400], cntrlfd;

void
error(int level, char *s1, ...)
{
	int fd;
	long thetime;
	char *chartime;
	va_list ap;
	char *args[8];
	int argno = 0;

	if((fd=open(LPDAEMONLOG, 1))<0) {
		if(errno==ENOENT) {
			if((fd=creat(LPDAEMONLOG, 0664))<0) {
				return;	/* hopeless, just go away mad */
			}
		} else return;
	}
	
	lseek(fd, 0, 2);
	if (level == 0) {
		time(&thetime);
		chartime = ctime(&thetime);
		fprint(fd, "%.15s ", &(chartime[4]));
	}
	va_start(ap, s1);
	while(args[argno++] = va_arg(ap, char*));
	va_end(ap);
	fprint(fd, s1, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
	close(fd);
	return;
}

void
forklp(int inputfd)
{
	int i, cpid;
	char *bp, *cp;
	char logent[LNBFSZ];

	/* log this call to lp */
	cp = logent;
	for (i=1; i<argcnt; i++) {
		bp = argvals[i];
		if (cp+strlen(bp)+1 < logent+LNBFSZ-1) {
			CPYFIELD(bp, cp);
			*cp++ = ' ';
		}
	}
	*--cp = '\n';
	*++cp = '\0';
	error(0, logent);
	switch((cpid=fork())){
	case -1:
		error(0, "fork error\n");
		exit(2);
	case 0:
		if (inputfd != 0)
			dup2(inputfd, 0);
		dup2(1, 2);
		lseek(0, 0L, 0);
		execvp(LP, argvals);
		error(0, "exec failed\n");
		exit(3);
	default:
		while(wait((int *)0) != cpid);
	}
}

int
getfiles(void)
{
	char *ap;
	int filecnt, bsize, rv;

	filecnt = 0;
	/* get a line, hopefully containing a ctrl char, size, and name */
	for(;;) {
		ap = lnbuf;
		do {
			if ((rv = read(1, ap, 1)) != 1) {
				if (rv < 0) {
					error(0, "Lost connection\n");
					NAK();
				}
				return(filecnt);
			}
		} while (*ap != '\n' && (ap++ - lnbuf < LNBFSZ - 1));
		*ap = '\0';
		ap = lnbuf;
		switch(*ap++) {
		case '\1':		/* cleanup - data sent was bad (whatever that means) */
			break;
		case '\2':		/* read control file */
			bsize = atoi(ap);
			cntrlfd = tmpfile();
			if (readfile(cntrlfd, bsize) < 0) {
				close(cntrlfd);
				NAK();
				return(0);
			}
			return(filecnt);
		case '\3':		/* read data file */
			bsize = atoi(ap);
			datafd[filecnt] = tmpfile();
			readfile(datafd[filecnt++], bsize);
			break;
		default:
			error(0, "protocol error <%d>\n", *(ap-1));
			NAK();
		}
	}
}

int
readfile(int outfd, int bsize)
{
	int bcnt, rv;

	ACK();
	for(bcnt=bsize ; bcnt > 0; bcnt -= rv) {
		if((rv=read(0, jobbuf, MIN(bcnt,RDSIZE))) < 0) {
			error(0, "error reading input\n");
			exit(4);
		} else if((write(outfd, jobbuf, rv)) != rv) {
			error(0, "error writing temp file\n");
			exit(5);
		}
	}
	if (((rv=read(0, jobbuf, 1))==1) && (*jobbuf=='\0')) {
		ACK();
		return(outfd);
	}
	error(0, "received bad status <%d> from sender ", *jobbuf);
	error(1, "rv=%d\n", rv);
	NAK();
	return(-1);
}

struct jobinfo *
getjobinfo(int fd)
{
	register char *ap;
	int rv;
	static struct jobinfo info;

	if (lseek(fd, 0L, 0) < 0) {
		error(0, "error seeking in temp file\n");
		exit(7);
	}
	/* the following strings should be < NAMELEN or else they will not
	 * be null terminated.
	 */
	strncpy(info.user, "daemon", NAMELEN);
	strncpy(info.host, "nowhere", NAMELEN);
	/* there may be a space after the name and host.  It will be filtered out
	 * by CPYFIELD.
	 */
	while ((rv=readline(fd)) != 0) {
		ap = lnbuf;
		ap[rv-1] = '\0';	/* remove newline from string */
		switch (*ap) {
		case 'H':
			strncpy(info.host, &ap[1], NAMELEN);
			break;
		case 'P':
			strncpy(info.user, &ap[1], NAMELEN);
			break;
		}
	}
	return(&info);
}

int
tmpfile(void)
{
	static tindx = 0;
	char tmpf[20];
	int crtfd, tmpfd;

	sprint(tmpf, "/tmp/lp%d.%d", getpid(), tindx++);
	if((crtfd=creat(tmpf, 0666)) < 0) {
		error(0, "cannot create temp file %s\n", tmpf);
		NAK();
		exit(3);
	}
	if((tmpfd=open(tmpf, 2)) < 0) {
		error(0, "cannot open temp file %s\n", tmpf);
		NAK();
		exit(3);
	}
	close(crtfd);
/*	unlink(tmpf);	/* comment out for debugging */
	return(tmpfd);
}

int
readline(int inpfd)
{
	register char *ap;
	register int i;

	ap = lnbuf;
	i = 0;
	do {
		if (read(inpfd, ap, 1) != 1) {
			error(0, "read error\n");
			break;
		}
		i++;
	} while (*ap++ != '\n' && (i < LNBFSZ - 2));
	if (*(ap-1) != '\n') {
		*(ap-1) = '\n';
		i++;
	}
	*ap = '\0';
	return(i);
}

main()
{
	char *ap, *bp, *cp, *savbufpnt;
	int i, rv, saveflg, savargcnt;
	struct jobinfo *jinfop;

	signal(1, SIG_IGN);
	cp = argvstr;
	/* setup argv[0] for exec */
	argvals[argcnt++] = cp;
	for (bp = LP, i = 0; (*bp != '\0') && (i < ARGSIZ-1); *cp++ = *bp++, i++);
	*cp++ = '\0';
	/* get the first line sent and parse it as arguments for lp */
	ap = lnbuf;
	i = 0;
	do {
		if ((rv = read(0, ap, 1)) != 1) {
			if (rv < 0) {
				error(0, "Lost connection\n");
			}
			exit(1);
		}
		i++;
	} while (*ap++ != '\n' && (i < LNBFSZ - 2));
	if (*(ap-1) != '\n') {
		*(ap-1) = '\n';
		i++;
	}
	*ap = '\0';
	ap = lnbuf;
	if (ap == (char *)0) {
		error(0, "cannot read arg line\n");
		NAK();
		exit(1);
	}
	bp = ap;
	/* setup the remaining arguments */
	/* check for BSD style request */
	/* ^A, ^B, ^C, ^D, ^E (for BSD lpr) */
	switch (*bp) {
	case '\001':
	case '\003':
	case '\004':
		bp++;	/* drop the ctrl character from the input */
		argvals[argcnt++] = cp;
		*cp++ = '-'; *cp++ = 'q'; *cp++ = '\0';		/* -q */
		argvals[argcnt++] = cp;
		*cp++ = '-'; *cp++ = 'd'; 			/* -d */
		CPYFIELD(bp, cp);				/* printer */
		*cp++ = '\0';
		break;
	case '\002':
		bp++;	/* drop the ctrl character from the input */
		argvals[argcnt++] = cp;
		*cp++ = '-'; *cp++ = 'd'; 			/* -d */
		CPYFIELD(bp, cp);				/* printer */
		*cp++ = '\0';
		ACK();
		savargcnt = argcnt;
		savbufpnt = cp;
		while ((rv=getfiles())) {
			jinfop = getjobinfo(cntrlfd);
			close(cntrlfd);
			argcnt = savargcnt;
			cp = savbufpnt;
			argvals[argcnt++] = cp;
			*cp++ = '-'; *cp++ = 'M'; 			/* -M */
			bp = jinfop->host;
			CPYFIELD(bp, cp);				/* host name */
			*cp++ = '\0';
			argvals[argcnt++] = cp;
			*cp++ = '-'; *cp++ = 'u'; 			/* -u */
			bp = jinfop->user;
			CPYFIELD(bp, cp);				/* user name */
			*cp++ = '\0';
			for(i=0;i<rv;i++)
				forklp(datafd[i]);
		}
		exit(0);
	case '\005':
		bp++;	/* drop the ctrl character from the input */
		argvals[argcnt++] = cp;
		*cp++ = '-'; *cp++ = 'k'; *cp++ = '\0';		/* -k */
		argvals[argcnt++] = cp;
		*cp++ = '-'; *cp++ = 'd'; 			/* -d */
		CPYFIELD(bp, cp);				/* printer */
		*cp++ = '\0';
		argvals[argcnt++] = cp;
		*cp++ = '-'; *cp++ = 'u'; 			/* -u */
		CPYFIELD(bp, cp);				/* username */
		*cp++ = '\0';
		datafd[0] = tmpfile();
		fprint(datafd[0], "%s\n", bp);
		break;
	default:
		/* otherwise get my lp arguments */
		do {
			/* move to next non-white space */
			while (*bp==' '||*bp=='\t')
				++bp;
			if (*bp=='\n') continue;
			/* only accept arguments beginning with -
			 * this is done to prevent the printing of
			 * local files from the destination host
			 */
			if (*bp=='-') {
				argvals[argcnt++] = cp;
				saveflg = 1;
			} else
				saveflg = 0;
			/* move to next white space copying text to argument buffer */
			while (*bp!=' ' && *bp!='\t' && *bp!='\n'
			    && *bp!='\0') {
				*cp = *bp++;
				cp += saveflg;
			}
			*cp = '\0';
			cp += saveflg;
		} while (*bp!='\n');
		readline(0);
		datafd[0] = tmpfile();
		error(0, "reading in %d bytes <%s>\n", atoi(lnbuf), lnbuf);
		if(readfile(datafd[0], atoi(lnbuf)) < 0) {
			error(0, "readfile failed\n");
			exit(7);
		}
	}
	forklp(datafd[0]);
	exit(0);
}
