#include	<libc.h>
#include	<fio.h>
#include	<sysent.h>
#include	<string.h>
#include	<errno.h>
#include	"crossbar.h"

extern int verbose;
static ntilt;
#define		MAXTILT		5

Dynasty::Dynasty(char *dkname, int n, int depth) : (dkname, n, depth)
{
}

void
Dynasty::startup()
{
	char buf[256];
	extern int opendk(char *);

	fd = opendk(dial);
	tocontrol();
}

void
Dynasty::tocontrol()
{
	for(;;){
		put1(0xAC);
		if(get1(0xFB) == 0)
			break;
		sleep(1);
	}
}

void
Dynasty::config()
{
	int i, j;

	imap = new int[nouts];
	ntilt = 0;
	for(i = 0; i < nouts; i++){
	again:
		if(ntilt > MAXTILT){
			fprint(2, "vsw: too many (%d) tilts, exiting\n", ntilt);
			exit(1);
		}
		put1(0xDA);
		putn(i);
		if(get1(0xAB)){
			sleep(1);
			tocontrol();
			goto again;
		}
		j = getn(8);
		put1(0xFB);
		imap[i] = j;
	}
}

int
Dynasty::connect(int input, Crossbar *c, int output, int nw)
{
	char buf[256];

	if(fd < 0)
		startup();
	if(c != this){
		if(nextc->connect(nextl, c, output, nw) == 0)
			return(0);
		output = froml;
	}
	nw = (nw+nwires-1)/nwires;
	while(nw-- > 0){
		put1(0xCF);
		putn(output);
		putn(input);
		if(get1(0xFB)){
			fprint(2, "%s.con %d->%d failed\n", name(), input, output);
			return(0);
		}
		input++, output++;
	}
	return(1);
}

void
Dynasty::put1(unsigned char x)
{
	if(write(fd, (char *)&x, 1) != 1)
		perror("WRITE");
}

void
Dynasty::putn(int i)
{
	unsigned char wbuf[2];

	wbuf[0] = i/100;
	wbuf[1] = i%10 | (((i/10)%10)<<4);
	if(write(fd, (char *)wbuf, 2) != 2)
		perror("putn");
}

int
Dynasty::get1(unsigned char x)
{
	unsigned char r;
	char buf[60];

	alarm(2);
	if(read(fd, (char *)&r, 1) != 1){
		if(errno != EINTR)
			perror("READ");
		else
			fprint(2, "vsw: timeout, retrying\n"), tocontrol();
		return(1);
	}
	alarm(0);
	if(r != x){
		switch(r)
		{
		case 0xF0:
			ntilt++;
			break;
		case 0xAB:
			alarm(2);
			read(fd, buf, 8);
			alarm(0);
			print("continue(#%ux#%ux#%ux#%ux#%ux#%ux#%ux#%ux)\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
			break;
		case 0xAD:
			alarm(2);
			read(fd, buf, 10);
			alarm(0);
			print("autostatus(#%ux#%ux#%ux#%ux#%ux#%ux#%ux#%ux#%ux#%ux)\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9]);
			break;
		default:
			fprint(2, "READ(#%2.2x): got #%2.2x\n", x, r);
			break;
		}
		return(1);
	}
	return(0);
}

int
Dynasty::getn(int n)
{
	unsigned char r[64];

	if(read(fd, (char *)r, n) != n)
		perror("getn");
	return((r[0]&0xF)*100 + ((r[1]>>4)&0xF)*10 + (r[1]&0xF));
}

#ifdef	CRAP

int verbose;
int rfs;

#define	WRITE(x)	{ char wbuf[1]; wbuf[0] = x; if(write(rfs, wbuf, 1) != 1) perror("WRITE"); }

WRITEN(i)
{
	unsigned char wbuf[2];

	wbuf[0] = i/100;
	wbuf[1] = i%10 | (((i/10)%10)<<4);
	if(write(rfs, wbuf, 2) != 2)
		perror("WRITEN");
}

READ(x)
{
	unsigned char rbuf[1];
	if(read(rfs, rbuf, 1) != 1)
		perror("READ");
	if(rbuf[0] != x){
		fprint(2, "READ(#%2.2x): got #%2.2x\n", x, rbuf[0]);
		return(1);
	}
	return(0);
}

READN(buf, n)
	unsigned char buf[];
{
	int i;

	for(i = 0; i < n; i++)
		if(read(rfs, &buf[i], 1) != 1){
			perror("READ");
			return(1);
		}
	return(0);
}

main(argc, argv)
	char **argv;
{
	int i, n, in, out;
	char buf[256];
	struct sgttyb vvec;
	extern int tty_ld;

	if((argc = getflags(argc, argv, "dcrvpPi", 0)) < 0)
		usage("[ iopair ... ]");
	verbose = flag['v'] != 0;
	sprint(buf, "/cs/dk!%s", "vswrgb2");
	rfs = ipcopen(buf, "light");
	if(rfs < 0){
		perror(buf);
		exit(1);
	}
	if(ioctl(rfs, FIOLOOKLD, 0) != tty_ld){
		if(ioctl(rfs, FIOPUSHLD, &tty_ld) < 0){
			perror("tty_ld");
			exit(1);
		}
	}
	vvec.sg_flags = RAW;
	vvec.sg_erase = 0;
	vvec.sg_kill = 0;
	if(ioctl(rfs, TIOCSETP, &vvec) < 0){
		perror("ioctl");
		exit(1);
	}
	tocontrol();
	print("in control:\n");
	status();
	exit(0);
}

tocontrol()
{
	WRITE(0xAC);
	return(READ(0xFB));
}

status()
{
	int i, n;
	unsigned char buf[8];

	for(i = 0; i < 5; i++){
		n = status1(i, buf);
		print("%d: status1 ret=%d; %x %x %x %x %x %x %x %x\n", i, n, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
	}
}

status1(i, buf)
	unsigned char *buf;
{
	WRITE(0xDA);
	WRITEN(i);
	if(i = READ(0xAB))
		return(i);
	if(i = READN(buf, 8))
		return(i);
	WRITE(0xFB);
	sleep(1);
	return(0);
}
#endif
