#include	"hdr.h"

int debug;
int vfd;
int incmd;
int logfd;
char **inptr, *inpptr;
extern int yydebug;
extern tfn(), Tfn();

main(int argc, char **argv)
{
#pragma ref argc
	int quiet = 0;
	char *remote = "/cs/dk!2500rem";

	fmtinstall('t', tfn);
	fmtinstall('T', Tfn);
	logfd = open("/dev/null", 1);
	for(argv++; argv[0][0] == '-'; argv++)
		switch(argv[0][1])
		{
		case 'l':
			logfd = creat("2500.log", 0666); break;
		case 'D':
			++debug; break;
		case 'q':
			quiet = 1; break;
		}
	if(*argv){
		incmd = 1;
		inptr = argv;
		inpptr = *inptr;
	} else
		incmd = 0;
	if(debug){
		vfd = open("/dev/null", 2);
	} else {
		if((vfd = ipcopen(remote, "light")) < 0){
			Fprint(2, "can't open %s\n", remote);
			exit(1);
		}
/*		ioctl(vfd, TIOCFLUSH, (char *)0);*/
	}
	if(!quiet){
		locenable();
		status(0);
		frame(0xFF);
	}
	while(yyparse())
		fprint(2, "parse errors!\n");
	exit(0);
}

cmd(int cnt, char *str, char *res, char *fmt, int args)
{
	static char buf[64];
	char *out;
	register i, sum;
	int cm, n;
	int rn;
	extern char *doprint();

	out = doprint(buf, fmt, (char *)&args);
	n = out-buf;
	buf[0] &= 0xf0;
	buf[0] |= n-2;
	cm = buf[0];
	for(i = sum = 0; i < n; i++)
		sum += buf[i];
	if(n < 2){
		fprint(2, "%ux: cmd too small[%d]\n", cm, n);
		return(-1);
	}
	if(n > 17){
		fprint(2, "%ux: cmd too big[%d]\n", cm, n);
		return(-1);
	}
	buf[n++] = sum;
	if(write(vfd, buf, n) != n){
		perror("write");
		return(-1);
	}
	if(res == 0)
		res = buf;
	if(!debug){
		i = read(vfd, res, 64);
		if(i < 0){
			perror("XTTY read");
			return(-1);
		}
		rn = (res[0]&0xF)+3;
		while(i < rn){
			int j;
			j = read(vfd, &res[i], rn-i);
			if(j < 0) break;
			i += j;
		}
		if((res[0] == 0x10) && (res[1] == 0x01) && (res[2] == 0x11)){	/* ack */
			if(cnt){
				fprint(2, "%s: got ack, expected %d data bytes\n", cnt);
				return(-1);
			}
		} else if((res[0] == 0x11) && (res[1] == 0x12)){
			fprint(2, "%s: NAK on cmd %2.2ux:", str, cm);
			if(res[2]&0x80) fprint(2, " timeout");
			if(res[2]&0x40) fprint(2, " framing");
			if(res[2]&0x20) fprint(2, " overrun");
			if(res[2]&0x10) fprint(2, " parity");
			if(res[2]&0x04) fprint(2, " checksum");
			if(res[2]&0x01) fprint(2, " undefined command");
			fprint(2, "\n");
			return(-1);
		} else if(i != (cnt+3)){
			fprint(2, "%s: got %d data bytes, expected %d\n", str, i-3, cnt);
			return(-1);
		}
		if(i != (res[0]&0xF)+3){
			fprint(2, "%s: urk! got %d bytes, pkt says %d:", str,
				i, (res[0]&0xF)+3);
			for(n = 0; n < i; n++) fprint(2, " 0x%2.2x", res[n]&0xFF);
			fprint(2, "\n");
			return(-1);
		}
	}
	return(0);
}

getc()
{
	register c;

	if(incmd){
		if(inpptr == 0)
			return(-1);
		if((c = *inpptr++) == 0){
			c = '\n';
			inpptr = *++inptr;
		}
	} else
		c = Fgetc(0);
	if(c >= 0)
		Fputc(logfd, c);
	else
		Fprint(logfd, "<<EOF>>\n");
	return(c);
}

tfn(t, f1, f2, f3)
	Time *t;
{
#pragma ref f2
#pragma ref f3
#define	T(x)	BYTE(((x)&0xFF)/10, ((x)&0xFF)%10)

	chconv(T(t->f), f1);
	chconv(T(t->s), f1);
	chconv(T(t->m), f1);
	chconv(T(t->h), f1);
	return(sizeof *t);
}

Tfn(t, f1, f2, f3)
	Time *t;
{
#pragma ref f3
	char buf[20];

	sprint(buf, "%d.%d.%d.%d", t->h, t->m, t->s, t->f);
	strconv(buf, f1, f2);
	return(sizeof *t);
}

help()
{
	extern char *helpstrs[];
	char **h;

	for(h = helpstrs; *h; h++)
		Fprint(1, "%s\n", *h);
	Fprint(1, "!unix cmd\n");
	Fprint(1, "\ttimes are s|s.f|m.s.f|h.m.s.f\n");
	Fprint(1, "\tchannels are one or more of audio[123]|video\n");
	Fprint(1, "\tall input is lower case; comments start with #; cmds sep by ;|\\n\n");
	Fflush(1);
}
