/*% cyntax -DTEST % && cc -DTEST -go # %
 */
#include <libc.h>
#include <getflags.h>
char **flag[NFLAG];
char cmdline[NCMDLINE+1];
char *cmdname;
char *flagset[];
char *flagset[]={"<flag>"};
static char *flagarg="";
static reverse(), scanflag();
static int reason;
#define	RESET	1
#define	ARGC	2
#define	FLAGSYN	3
#define	BADFLAG	4
static int badflag;
char *getflagsargv[NGETFLAGSARGV+2];	/* original argv stored here for people who need it */
getflags(argc, argv, flags)
char *argv[], *flags;
{
	register char *s, *t;
	register i, j, c, count;
	flagarg=flags;
	if(cmdname==0){
		cmdname=argv[0];
		for(i=0;i!=argc && i!=NGETFLAGSARGV;i++) getflagsargv[i]=argv[i];
		if(argc>NGETFLAGSARGV) getflagsargv[i++]="...";
		getflagsargv[i]=0;
	}
	s=cmdline;
	for(i=0;i!=argc;i++){
		for(t=argv[i];*t;)
			if(s!=&cmdline[NCMDLINE])
				*s++=*t++;
		if(i!=argc-1 && s!=&cmdline[NCMDLINE])
			*s++=' ';
	}
	*s='\0';
	i=1;
	while(i!=argc && argv[i][0]=='-'){
		s=argv[i]+1;
		if(*s=='\0'){	/* if argument is "-", stop scanning and delete it */
			for(j=i+1;j<=argc;j++)
				argv[j-1]=argv[j];
			return argc-1;
		}
		while(*s){
			c=*s++;
			count=scanflag(c, flags);
			if(count==-1) return -1;
			if(flag[c]){ reason=RESET; badflag=c; return -1; }
			if(count==0){
				flag[c]=flagset;
				if(*s=='\0'){
					for(j=i+1;j<=argc;j++)
						argv[j-1]=argv[j];
					--argc;
				}
			}
			else{
				if(*s=='\0'){
					for(j=i+1;j<=argc;j++)
						argv[j-1]=argv[j];
					--argc;
					s=argv[i];
				}
				if(argc-i<count){
					reason=ARGC;
					badflag=c;
					return -1;
				}
				reverse(argv+i, argv+argc);
				reverse(argv+i, argv+argc-count);
				reverse(argv+argc-count+1, argv+argc);
				argc-=count;
				flag[c]=argv+argc+1;
				flag[c][0]=s;
				s="";
			}
		}
	}
	return argc;
}
static reverse(p, q)
register char **p, **q;
{
	register char *t;
	for(;p<q;p++,--q){ t=*p; *p=*q; *q=t; }
}
static scanflag(c, f)
register char *f;
{
	register fc, count;
	if(0<=c && c<NFLAG) while(*f){
		if(*f==' '){
			f++;
			continue;
		}
		fc=*f++;
		if(*f==':'){
			f++;
			if(*f<'0' || '9'<*f){ reason=FLAGSYN; return -1; }
			count=0;
			while('0'<=*f && *f<='9') count=count*10+*f++-'0';
		}
		else
			count=0;
		if(*f=='['){
			do{
				f++;
				if(*f=='\0'){ reason=FLAGSYN; return -1; }
			}while(*f!=']');
			f++;
		}
		if(c==fc) return count;
	}
	reason=BADFLAG;
	badflag=c;
	return -1;
}
static errn(), errs(), errc();
usage(tail)
char *tail;
{
	register char *s, *t, c;
	register count, nflag=0;
	switch(reason){
	case RESET:
		errs("Flag -");
		errc(badflag);
		errs(": set twice\n");
		break;
	case ARGC:
		errs("Flag -");
		errc(badflag);
		errs(": too few arguments\n");
		break;
	case FLAGSYN:
		errs("Bad argument to getflags!\n");
		break;
	case BADFLAG:
		errs("Illegal flag -");
		errc(badflag);
		errc('\n');
		break;
	}
	errs("Usage: ");
	errs(cmdname);
	for(s=flagarg;*s;){
		c=*s;
		if(*s++==' ') continue;
		if(*s==':'){
			s++;
			count=0;
			while('0'<=*s && *s<='9') count=count*10+*s++-'0';
		}
		else count=0;
		if(count==0){
			if(nflag==0) errs(" [-");
			nflag++;
			errc(c);
		}
		if(*s=='['){
			s++;
			while(*s!=']' && *s!='\0') s++;
			if(*s==']') s++;
		}
	}
	if(nflag) errs("]");
	for(s=flagarg;*s;){
		c=*s;
		if(*s++==' ') continue;
		if(*s==':'){
			s++;
			count=0;
			while('0'<=*s && *s<='9') count=count*10+*s++-'0';
		}
		else count=0;
		if(count!=0){
			errs(" [-");
			errc(c);
			if(*s=='['){
				s++;
				t=s;
				while(*s!=']' && *s!='\0') s++;
				errs(" ");
				errn(t, s-t);
				if(*s==']') s++;
			}
			else
				while(count--) errs(" arg");
			errs("]");
		}
		else if(*s=='['){
			s++;
			while(*s!=']' && *s!='\0') s++;
			if(*s==']') s++;
		}
	}
	if(tail){
		errs(" ");
		errs(tail);
	}
	errs("\n");
	exit(1);
}
static errn(s, count)
register char *s;
register count;
{
	while(count){ errc(*s++); --count; }
}
static errs(s)
register char *s;
{
	while(*s) errc(*s++);
}
#define	NBUF	80
static char buf[NBUF], *bufp=buf;
static errc(c){
	*bufp++=c;
	if(bufp==&buf[NBUF] || c=='\n'){
		write(2, buf, bufp-buf);
		bufp=buf;
	}
}
#ifdef TEST
#include <stdio.h>
main(argc, argv)
char *argv[];
{
	int c, i, n;
	if(argc<3){
		fprintf(stderr, "Usage: %s flags cmd ...\n", argv[0]);
		exit(1);
	}
	n=getflags(argc-2, argv+2, argv[1], 0);
	if(n<0) usage("...");
	for(i=0;i!=n;i++) printf("%s ", argv[2+i]);
	putchar('\n');
	for(c=0;c!=128;c++) if(flag[c]){
		printf("\t-%c", c);
		n=scanflag(c, argv[1]);
		for(i=0;i!=n;i++) printf(" %s", flag[c][i]);
		putchar('\n');
	}
}
#endif
