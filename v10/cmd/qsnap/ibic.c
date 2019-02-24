#include	<stdio.h>

/*
 * This software is provided solely for use with
 * the National Instruments GPIB series interfaces.
 *
 * Copyright 1980, 1983 National Instruments
 *
 * Jeffrey Kodosky
 * REV D:  10/09/83
 */

extern int ibup(), gpib(), slp(), echo();
extern int errno;

#define EINTR	4
#define	NTYPES	(sizeof fctn / sizeof fctn[0])

char *fns[]= { "ibup", "gpib", "sleep", "echo" };

int (*fctn[])()= { ibup, gpib, slp, echo };

int maxcode[]= { 10, 16, 0 };

struct argdef {
	char *name, *args, *ans; };

struct argdef echoargs[]= {
	{"str", "S", "B"},
};

struct argdef slpargs[]= {
	{"sec", "I", "B"},
};

struct argdef ibupargs[]= {
	{"write", "ISL", "I" },
	{"read", "IBN", "IB" },
	{"clear", "I", "I" },
	{"trigger", "I", "I" },
	{"remote", "I", "I" },
	{"local", "I", "I" },
	{"poll", "I", "O" },
	{"configure", "III", "I" },
	{"passcontrol", "I", "I" },
	{"define", "IIIIIII", "I" },
	{"finish", "", "I" }  };

struct argdef gpibargs[]= {
	{"command", "SL", "I" },
	{"write", "SIL", "I" },
	{"read", "BNII", "IB" },
	{"transfer", "", "I" },
	{"clear", "", "I" },
	{"remote", "", "I" },
	{"local", "", "I" },
	{"parallelpoll", "", "O" },
	{"passcontrol", "", "I" },
	{"setstatus", "I", "I" },
	{"monitor", "I", "I" },
	{"readcommand", "", "O" },
	{"setparameters", "I", "I" },
	{"testsrq", "I", "I" },
	{"finish", "", "I" },
	{"status", "BN", "IT" },
	{"spbyte", "I", "I" }	};

struct argdef *fnargs[]= { ibupargs, gpibargs, slpargs, echoargs };

#define SIGSRQ 2
#define MAXLIN 256
#define MAXBUF 2048
#define MAXINT 0100000

char lbuf[MAXBUF], ansbuf[MAXBUF], *cp;
int strleng, fn;
int argc;
int argv[9];
int code;

main(){ int n, srqint();
	char *atp, c, *getstr();

	signal(SIGSRQ,srqint);
loop:	while((n=getline())>=0){
		if(n==0) continue;
		cp= lbuf;
		skpspc();
		if(*cp == 0) continue;
		if((fn=getfn())<0) continue;
		if((code=getcode(fn))<0) continue;
		for(argc=0, atp=fnargs[fn][code].args; c= *atp++; )
			switch(c){
				case 'N':
					if((n=getint(0))==MAXINT) goto loop;
					if(n>MAXBUF) n= MAXBUF;
					argv[argc++]= n;
					break;
				case 'I':
					if((argv[argc++]= getint(0))==MAXINT)
						goto loop;
					break;
				case 'S':
					if((argv[argc++]= (int)getstr())==0)
						goto loop;
					break;
				case 'L':
					n = getint(1);
					if(n == MAXINT) n = strleng;
					argv[argc++]= n;
					break;
				case 'B':
					argv[argc++]= (int)ansbuf;
					break;
				}
		n= (*fctn[fn])(code,argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
		for(atp= fnargs[fn][code].ans; c= *atp++; )
			switch(c){
				case 'I': if(n<0) iberr(n, "IBIC");
					  else putd(n);
					  putchar(' '); break;
				case 'O': if(n<0) putd(n);
					  else puto(n);
					  putchar(' '); break;
				case 'B': put(ansbuf,n); putchar(' '); break;
				case 'T': putt(ansbuf,n);putchar(' '); break;
				}
		putchar('\n');
	}	}

srqint(){
	puts("SRQ interrupt\n");
	signal(SIGSRQ,srqint); }

echo(n, a)
	char *a;
{
	return(strlen(ansbuf));
}

slp(n, a)
{
	printf("sleeping %d secs\n", a); fflush(stdout);
	sleep(a);
	return(0);
}

getfn(){ int fn;char *s, **a;
	skpspc();
	for(fn=0, a=fns; fn<NTYPES; fn++, a++)
		if(match(cp,*a)) break;
	if(fn>=NTYPES){ putl("unrecognized function"); return -1; }
	skpnspc();
	return fn; }

getcode(fn){ int f; struct argdef *a;
	skpspc();
	if(*cp>='0' && *cp<='9') f= atoi(cp);
	else for(f=0, a=fnargs[fn]; f<=maxcode[fn]; a++, f++)
		if(match(cp,a->name)) break;
	if(f<0 || f>maxcode[fn]){ putl("illegal code"); return -1; }
	skpnspc();
	return f; }

getint(opt){ register int i;
	skpspc();
	if(*cp>='0' && *cp<='9' || *cp=='-'){
		i= atoi(cp);
		skpnspc();
		return i; }
	if(!opt)putl("illegal integer argument");
	return MAXINT; }

char *getstr(){ char *s;
	skpspc();
	if(*cp++=='"'){
		for(strleng=0, s=ansbuf; *cp && *cp!='"'; strleng++)
			*s++= cscan();
		skpnspc();
		return ansbuf; }
	putl("illegal string argument");
	return 0; }

skpspc()
{
	while(*cp)
	{
		if(*cp == '#')
			while(*++cp);
		else if(*cp > ' ') break;
		else cp++;
	}
}

skpnspc()
{
	while(*cp>' ') cp++;
}

putl(s) char *s;{
	puts(s); putchar('\n'); }

puts(s) char *s;{
	while(*s) putchar(*s++); }

put(s,i) char *s;{
	register int c;
	while(i-->0)
		if((c= *s++&0377)>=' ' && c<0177)
			putchar(c);
		else {	putchar('\\');
			switch(c){
				case '\r': putchar('r'); break;
				case '\n': putchar('n'); break;
				case '\t': putchar('t'); break;
				case '\b': putchar('b'); break;
				case '\014': putchar('p'); break;
				default: putchar(((c>>6)&7) +'0');
					putchar(((c>>3)&7) +'0');
					putchar((c&7) +'0');
					break;
	}		}	}

putd(n){
	if(n<0){ putchar('-'); n= -n; }
	putu(n); }

putu(u) unsigned u;{
	if(u>=10) putu(u/10);
	putchar(u%10 + '0'); }

puto(o){
	if(o&0177770) puto((o>>3)&017777);
	putchar((o&7) +'0'); }

putt(buf,ct) char *buf;{ register int i;
	if(ct<=0) return;
	i= 0;
	while(ct--){
		if((i%8)==0) putchar('\n');
		puto(buf[i++] & 0377);
		putchar(' ');
	}	}

atoi(s) char *s;{ int sign, base, n;
	sign= 0;
	base= 10;
	if(*s=='-'){ sign++; s++; }
	else if(*s=='0'){ base= 8; s++; }
	for(n=0; *s>='0' && *s<='9'; ) n= n*base + *s++ - '0';
	return sign? -n:n; }

getline(){ char *s;
	putchar(':');
	for(s=lbuf; (*s=getchar())!='\n'; )
		if(*s<=0)
			if(errno==EINTR) errno= 0;
			else return s>lbuf? s-lbuf: -1;
		else s++;
	*s= 0;
	return s-lbuf; }

match(st,s) char *st, *s;{ register char *t;
	for(t=st; *t==*s || *t==(*s&~040); t++)
		if(*s++==0) return 1;
	if(t>st && *t<=' ') return 1;
	return 0; }

cscan(){ register int n, i;
	if(*cp!='\\') return *cp++;
	cp++;
	if(*cp>='0' && *cp<='7'){
		for(i=3, n=0; i-->0 && *cp>='0' && *cp<='7'; )
			n= (n<<3) + *cp++ -'0';
		return n; }
	switch(*cp){
		case 0: return 0;
		case 'n': cp++; return '\n';
		case 't': cp++; return '\t';
		case 'r': cp++; return '\r';
		case 'b': cp++; return '\b';
		case 'p': cp++; return '\014';
		default: return *cp++;
	}	}
