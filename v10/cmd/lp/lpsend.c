#include <fio.h>
#include <errno.h>
#include <time.h>
#include <ipc.h>

#define MIN(a,b)	((a<b)?a:b)

#define	ACK(a)	write(a, "", 1)
#define NAK(a)	write(a, "\001", 1)

#define LPDAEMONLOG	"/tmp/lpdaemonl"

#define LNBFSZ	4096
char lnbuf[LNBFSZ];

void
error(char *s1, ...)
{
	time_t thetime;
	char *chartime;

	time(&thetime);
	chartime = ctime(&thetime);
	fprint(2, "%.15s ", &(chartime[4]));
	fprint(2, s1, (&s1+1));
	return;
}

/* get a line from inpfd using nonbuffered input.  The line is truncated if it is too
 * long for the buffer.  The result is left in lnbuf and the number of characters
 * read in is returned.
 */
int
readline(int inpfd)
{
	register char *ap;
	register int i;

	ap = lnbuf;
	i = 0;
	do {
		if (read(inpfd, ap, 1) != 1) {
			error("read error\n");
			break;
		}
	} while ((++i < LNBFSZ - 2) && *ap++ != '\n');
	if (i == LNBFSZ - 2) {
		*ap = '\n';
		i++;
	}
	*ap = '\0';
	return(i);
}

#define	RDSIZE 512
char jobbuf[RDSIZE];

int
pass(int inpfd, int outfd, int bsize)
{
	int rv;
	for(; bsize > 0; bsize -= rv)
		if((rv=read(inpfd, jobbuf, MIN(bsize,RDSIZE))) < 0) {
			error("read error\n");
			return(bsize);
		} else if((write(outfd, jobbuf, rv)) != rv) {
			error("write error\n");
			return(bsize);
		}
	return(bsize);
}

void
recvdata(int inpfd, int outfd)
{
	int i;

	while ((readline(inpfd),i = atoi(lnbuf)) != 0) {
		if (pass(inpfd, outfd, i) != 0) {
			error("failed to receive response\n");
			exit(6);
		}
	}
	return; 
}
	
/* get whatever stdin has and put it into the temporary file.
 * return the file size.
 */
int
prereadfile(int inpfd)
{
	int rv, bsize;

	bsize = 0;
	do {
		if((rv=read(0, jobbuf, RDSIZE))<0) {
			error("read error\n");
			exit(4);
		} else if((write(inpfd, jobbuf, rv)) != rv) {
			error("write error\n");
			exit(5);
		}
		bsize += rv;
	} while (rv!=0);
	return(bsize);
}

int
tmpfile(void)
{
	static tindx = 0;
	char tmpf[20];
	int crtfd, tmpfd;

	sprint(tmpf, "/tmp/lp%d.%d", getpid(), tindx++);
	if((crtfd=creat(tmpf, 0666)) < 0) {
		error("cannot create temp file %s\n", tmpf);
		exit(3);
	}
	if((tmpfd=open(tmpf, 2)) < 0) {
		error("cannot open temp file %s\n", tmpf);
		exit(3);
	}
	close(crtfd);
	unlink(tmpf);
	return(tmpfd);
}

int
recvACK(int netfd)
{
	if (read(netfd, jobbuf, 1)!=1 || *jobbuf!='\0') {
		error("failed to receive ACK\n");
		if (*jobbuf == '\0')
			error("read failed\n");
		else
			error("received <0x%x> instead\n", *jobbuf);
		return(0);
	}
	return(1);
}

main(int argc, char *argv[])
{
	char *ap, *cp;
	int i, rv, netfd, bsize, datafd;

	/* make connection */
	if (argc != 4) {
		fprint(2, "usage: %s destination network service\n", argv[0]);
		exit(1);
	}
	if ((netfd = ipcopen((cp = ipcpath(argv[1], argv[2], argv[3])), "heavy tcphup hup")) < 0){
		fprint(2, "ipcopen(%s):%s\n", cp, errstr);
		exit(2);
	}

	/* read options line from stdin and send it */
	ap = lnbuf;
	i = readline(0);
	if (write(netfd, lnbuf, i) != i) {
		error("write error\n");
		exit(1);
	}

	/* read stdin into tmpfile to get size */
	datafd = tmpfile();
	bsize = prereadfile(datafd);

	i = sprint(lnbuf, "%d\n", bsize);
	if (write(netfd, lnbuf, i) != i) {
		error("write error\n");
		exit(1);
	}

	if (lseek(datafd, 0L, 0) < 0) {
		error("error seeking temp file\n");
		exit(4);
	}
	/* mirror performance in readfile() in lpdaemon */
	recvACK(netfd);
	if (pass(datafd, netfd, bsize) != 0) {
		NAK(netfd);
		error("failed to send data\n");
		exit(5);
	}
	ACK(netfd);
	recvACK(netfd);

	/* get response, as from lp -q */
	while((rv=read(netfd, jobbuf, RDSIZE)) > 0) {
		if((write(1, jobbuf, rv)) != rv) {
			error("write error\n");
			exit(6);
		}
	}
	/* close down network connections and go away */
	exit(0);
}
