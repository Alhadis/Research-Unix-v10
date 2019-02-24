#
/*
 * dpd -- data-phone daemon dispatcher using spider
 *
 */

#include	<errno.h>

#define	SPIDER	1
#define	PHONE	0
#define	LPD	0
#define	FGET	0

#define	TIMEOUT	100

char	dname[] = "DPDS";
char	dpd[]	= "/usr/spool/dpd";
char	dfname[30] = "/usr/spool/dpd/";
char	lock[]	= "/usr/spool/dpd/lock";
char	error[]	= "/usr/spool/dpd/errors";

#include	"daemon.c"
#include	"gcos.c"

/* nak codes --
 * i illegal
 * b dest busy
 * s illegal gcos command
 * g gcos down/no resp
 */
#define	SETSIG	0
#define	GETSIG	1
#define	GETCHN	2
#define	GETTRB	3
#define	SETEOF	4
#define	CLREOF	5

int	COMM[]	= {3};
int	DATA[]	= {1};
int	SPEOF[]	= {2};
int	TERM[]	= {4};

#define	LIDMES	9
char	idmes[LIDMES+1]	= "gXXrxxxxx";
char	sndev[]	= "/dev/tiu/d2";

FILE	*cfb = NULL;		/*data file*/
char	ibuf[64];
int	snf = -1;	/*spider device*/
int	rathole[1];
int	errno;

dem_con()
{
	return(0);
}

dem_dis()
{
	FCLOSE(dfb);
	FCLOSE(cfb);
	if(snf >= 0){
		close(snf);
		snf = -1;
	}
}

dem_open(file)
char	*file;
{
	static lastbad, badcount;
	int badexit();

	signal(SIGALRM, badexit);
/*
 * Only allow 30 seconds to open file.
 */
	alarm(30);
	if ((snf = open(sndev, 2)) < 0) {
		if (errno == EBUSY)
			trouble("NG: Channel busy          ");
		else {
			retcode = TIMEOUT;
			trouble("NG: Can't find TIU file   ");
		}
	}
	snstat(snf, rathole, GETTRB);
	snstat(snf, COMM, SETSIG);
	idmes[1] = spider_id[0];
	idmes[2] = spider_id[1];
	snwrite(idmes, LIDMES);
	if (read(snf, ibuf, 10) <= 0) {
		snstat(snf, rathole, GETTRB);
		trouble("read err %d %d, pid %8s", errno, rathole[0]&0377,
			file+LPID);
	}
	if (ibuf[0]!='+') {
		if (lastbad=='B' && ibuf[0]=='B' && badcount>=4){
			/*reboot PDP8 on 4th consecutive
			  busy response (~1+2+4+8=15 minutes)
			*/
			lastbad=badcount=0; /*forget we've had busies*/
			waittm=60;	/*setup for retry after one minute*/
			snstat(snf, COMM, SETSIG);
			snwrite("b", 1);
			trouble("Another NAK:B, reboot     ");
		}
		lastbad = ibuf[0];
		badcount = lastbad=='B'?++badcount:0;
		trouble("NAK:%-22c", ibuf[0]);
	}
	lastbad=badcount=0;
	snumb = &ibuf[1];
	snumb[5] = 0;
}

get_snumb()
{
}

lwrite()
{
	setcom('h');
	line[linel] = '\n';
	snwrite(&line[1], linel);
}

dem_close()
{
	snstat(snf, COMM, SETSIG);
	snwrite("t\001", 2);
	snstat(snf, TERM, SETSIG);
	snwrite((char *)0, 0);
/*
	read(snf, ibuf, 10);
*/
}

sascii(fff)
{
	register n;
	static char buf[BUFSIZ];

	if((cfb = fopen(&line[1], "r")) == NULL){
		logerr("Can't open %s", &line[1]);
		return(0);
	}
	setcom('a');
	snstat(snf, DATA, SETSIG);
	if (fff)
		snwrite(&ff, 1);
	while ((n=fread(buf, 1, BUFSIZ, cfb))==BUFSIZ) {
		snwrite(buf, BUFSIZ);
	}
	if (n<0)
		n = 0;
	snstat(snf, SPEOF, SETSIG);
	snwrite(buf, n);
	FCLOSE(cfb);
	return(0);
}

setcom(c)
char	c;
{
	snstat(snf, COMM, SETSIG);
	snwrite(&c, 1);
	snstat(snf, SPEOF, SETSIG);
}

snwrite(b, n)
char	*b;
int	n;
{
	int ercode;
	register nw;

	alarm(30);
	nw = write(snf, b, n);
	ercode = 0;
	snstat(snf, &ercode, GETTRB);
	if (nw!=n || ercode) {
		if (ercode >= 0100)
			retcode = TIMEOUT;
		trouble("wrote %d sent %d err %o, %s",
			n, nw, ercode&0377, dfname+LPID);
	}
	snsum += n;
}

/* VARARGS */
trouble(s, a1, a2, a3, a4)
char	*s;
int	a1, a2, a3, a4;
{
	alarm(0);
	sleep(8); /* temporary to overcome timing problem in pdp8 ADH */
	dem_dis();
	logerr(s, a1, a2, a3, a4);
	sleep(5);
	if (retcode==TIMEOUT) {
		/*
		 * Give up.
		 */
		unlink(lock);
		execl("/usr/lib/odpd", "odpd", (char *)0);
		execl("/etc/odpd", "odpd", (char *)0);
		execl("/usr/lib/dpd", "dpd", (char *)0);
		execl("/etc/dpd", "dpd", (char *)0);
		logerr("Can't find dpd.");
		exit(1);
	}
	longjmp(env, 1);
}

badexit()
{
	if (waittm >= 2*60) {	/* Second try */
		retcode = TIMEOUT;
		trouble("Another timeout, I give up");
	}
	trouble("Timed out.                ");
}
