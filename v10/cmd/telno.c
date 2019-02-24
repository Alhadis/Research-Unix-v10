#include <ipc.h>
#include <fio.h>
/*
name<tab>company<tab>dialcode<tab>phone<tab>location<tab>room<tab>org<tab>mail<\n>
*/
int	fd;

timeout(){
	fprint(2, "telno: timeout\n");
	exit(1);
}
main(argc, argv)
	char *argv[];
{
	register i;

	if(argc==1){
		fprint(2, "usage: telno datum...\n");
		return 1;
	}
	fd=ipcopen(ipcpath("nj/homer/dq", "dk", "dq1"), "light");
	if(fd==-1){
		fprint(2, "telno: call failed: %s\n", errstr);
		return 1;
	}
	copy("-\n", 0);
	nap(20);
	for(i=1; i<argc; i++)
		lookup(argv[i]);
	return 0;
}
char	peekbuf[20];
int	npeek=0;
#define	unget(c)	(peekbuf[npeek++]=(c))
lookup(s)
	char *s;
{
	char buf[256];
	sprint(buf, "%s\n", s);
	write(fd, buf, strlen(buf));
	copy("\n", 0);
	unget('\n');
	copy("\n.\n", 1);
	return 0;
}
get(){
	register n, c;
	char buf[sizeof peekbuf];
	if(npeek==0){
		n=read(fd, buf, sizeof peekbuf);
		if(n<=0){
			fprint(2, "telno: read error\n");
			exit(1);
		}
		while(--n>=0)
			unget(buf[n]);
	}
	c=peekbuf[--npeek];
	c&=0x7F;
	if(c<' ' && c!='\n')
		return get();
	return c;
}
copy(end, output)
	register char *end;
{
	register n=0;
	register char *e;
	char c;
	for(;;){
		c=get();
		if(output)
			Fputc(1, c);
		e=end;
		while(c==*e && *++e)
			c=get();
		if(*e==0)
			break;
		if(e>end){
			unget(c);
			while(e>end+1)
				unget(*--e);
		}
	}
}
