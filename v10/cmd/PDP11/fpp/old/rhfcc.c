#define MARK (-1)
#define SYMTABSZ 4096
#define OUTFILE "a_out.c"
#define OUTFILE2 "a_out.o"
#define N 200
#define ON 77
#define OB 200

char *symtout "a.out"; /* symbol table filename */
extern int unixfmt; /* if set, use unix format floating point */
extern int unsign; /* 1 for unsigned allowed, 0 for not */
static struct obj {
	int type,size;
	char text[2];
	};
static char ib[N],ugb[N];
static int ibp {N}, ugbp {0};
static int ifp,ofp;
static char ob[OB];
static int obp,lpos;
static inputno;
static popt,nopt,vopt,qopt,uopt;
static eopt; /* use -E option */
static char *infile;
#define SHARPBUFL 256	/* chars in # lines max */
#define HEADLEN 4	/* chars in Q object header */
#define SHARPLEN 80	/* max characters in filename from # lines */
static char sharptext[SHARPLEN];
static int sharpn0,sharpn1; /* n1 = value from line n0 = inputno */

main(ac,av)
int ac;
char **av;
{
int i;
char *s;
if(ac<2) {
	printf("usage: fcc progname.c -options\n");
	printf("   options: 'P' halt after text conversion\n");
	printf("	    'n' norgen-format floating point constants\n");
	printf("            'v' verbose mode: lots of commentary\n");
	printf("            'q' quiet mode: warnings do not print\n");
	printf("            'u' no unsigned declaration generated\n");
	printf("            'E' use source line numbers if possible\n");
	exit(0);
	}
if(ac>2) {
	s=av[2];
	if(*s!='-') {
		if(av[1][0]=='-') {
			av[2]=av[1];
			av[1]=s;
			s=av[2];
			}
		else abt("bad option, expect -nPvqu");
		}
	while(*++s) {
		nopt=| *s=='n';
		popt=| *s=='P';
		vopt=| *s=='v';
		qopt=| *s=='q';
		uopt=| *s=='u';
		eopt=| *s=='E';
		}
	}
/* change to "!eopt" to reverse default meaning of E option */
if(eopt) {
	system4("cc -E ",av[1]," >",OUTFILE); /* .i output */
	for(i=0; av[1][i]; i++); av[1][--i]='i';
	if(av[1][i-1]!='.')abt("need '.c' name");
	system4("mv ",OUTFILE," ",av[1]);
	}
else	system4("cc -P ",av[1],"","");
for(i=0; av[1][i]; i++);
av[1][--i]='i';
if(--i<=0||av[1][i]!='.')abt("need '.c' name");
infile=av[1];
if((ifp=open(av[1],0))<0)abt("can't open .i for read");
if((ofp=creat(OUTFILE,0666))<0)abt("can't open fcc temp file");
symtab(SYMTABSZ);

if(nopt) {
	unixfmt=0;
	if(vopt)printf("using -n: norgen-format float\n");
	}
else unixfmt=1;

if(uopt)unsign=0; /* no unsigned declaration in output */
else unsign=1; /* declare _ftou() and _ntou() unsigned */

if(vopt)printf("begin %s-float conversion: file %s to %s\n",
	unixfmt?"unix":"norgen",av[1],OUTFILE);
file();
flushout();
close(ifp);
close(ofp);
if(popt==0) system4("rm ",av[1],"","");
else {
	if(vopt)printf("conversion input is %s\n",av[1]);
	if(vopt)printf("conversion output is %s\n",OUTFILE);
	if(vopt)printf("-P option: quit.\n");
	exit(0);
	}
system4("cc -c -O ",OUTFILE,"","");
system4("rm ",OUTFILE,"","");
for(i=0; av[1][i]; i++);
av[1][--i]='o';
system4("mv ",OUTFILE2," ",av[1]);
exit(0);
}
system4(s1,s2,s3,s4)
char *s1,*s2,*s3,*s4;
{
char s[100];
int i;
i=0; while(*s1)s[i++]= *s1++;
while(*s2)s[i++]= *s2++;
while(*s3)s[i++]= *s3++;
while(*s4)s[i++]= *s4++;
s[i]=0;
if(vopt)printf("%s\n",s);
if(fork()==0) execl("/bin/sh","sh","-c",s,0);
wait(&i);
if(i) {
	printf("bad termination status from %s\n",s);
	exit(1);
	}
return ;
}

abt(s)
char *s;
{
printif();
printf("%s line %d abort in fcc\n",infile,inputno+1);
printf("   %s\n",s);
flushout();
exit(1);
}

warn(s)
char *s;
{
if(qopt)return;
printif();
printf("%s line %d warn: %s\n",infile,inputno+1,s);
}

gc()	/* get a character */
{
int i,j;
if((i=gc2())!='/')return(i); /* not a comment */
if((j=gc2())!='*') {
	ug(j); /* not a comment */
	return(i);
	}
/*	skip till end of comment */
while(1) {
	while((i=gc2())!='*') if(i==0)abt("unclosed comment");
	if((i=gc2())=='/') return(' '); /*replace comment by blank*/
	ug(i);
	}
}

gc2() /* used to be gc(): this entry doesn't strip comments */
{
int i,j,k;
char *temp; /* buffer for # line */
char outtemp[16]; /* buffer for # number on line numbers */
static int newline 1; /* if 1, next char starts a new line. check for # */
if(ugbp==0) {
gc2top:	if(ibp>=N) {
		j=read(ifp,&ib,N);
		if(j<0)j=0;
		for(i=j; i<N; i++) ib[i]=0;
		ibp=0;
		}
	if(newline) {	/* check for # line to skip */
		newline=0;
		if(sharptext[0]) { /* toss out # n file if ever seen # */
			for(k=0; outtemp[k]="# "[k]; k++);
			outtemp[i=k+5]=0; /* convert 5 digits */
			j=inputno-sharpn0+sharpn1;
			while(j) {
				outtemp[--i]=j%10+'0';
				j=/10;
				}
			while(outtemp[k++]=outtemp[i++]);
			outtemp[k-1]='\n';
			write(ofp,outtemp,k);
			}
		if(ib[ibp]=='#') {
			temp=getq(SHARPBUFL-HEADLEN); /* get buffer for line */
			for(i=0; i<SHARPBUFL;) {
				if((temp[i++]=gc2())=='\n')break;
				}
			write(ofp,temp,i);
			sharpn0=inputno;
			j=1; while(temp[j]==' ')j++;
			if(temp[j]=='\n')goto skipx;
			sharpn1=0;
			while(temp[j]>='0'&&temp[j]<='9')sharpn1=sharpn1*10
							+temp[j++]-'0';
			while(temp[j]==' ')j++;
			if(temp[j]=='\n')goto skipx;
			i=0;
			while(temp[j]!='\n'&&i<SHARPLEN-1)
				sharptext[i++]=temp[j++];
			sharptext[i]=0;
		skipx:
			free(temp);
			goto gc2top;
			}
		}
	inputno=+newline=ib[ibp]=='\n'; /* ha! */
	while((j=ib[ibp++])=='\\') {
		if((i=gc())!='\n') {
			ug(i);
			return('\\');
			}
		}
	return(j);
	}
return(ugb[ugbp--]);
}

ug(c)	/* unget a character */
int c;
{
ugb[++ugbp]=c;
if(ugbp>=N-1)abt("unget buffer overflow");
return(c);
}


outq(op)
struct obj *op;
{
int i,j,k;
int c;
char *s;
k=op->size;
s=op->text;
	for(i=0; i<k; i++) {
		c= *s++;
		ob[obp++]=c;
		if(c=='\n') flushout();
		lpos=+ c!=MARK;
		if(lpos>ON||obp>=OB) partial();
		}
}

flushout()
{
int i,j,k;
if(obp<=0)return(0);
for(i=j=0; i<obp; i++) {
	if(ob[i]!=MARK)ob[j++]=ob[i];
	}
if(j==0)ob[j++]='\n';
if(ob[j-1]!='\n')ob[j++]='\n';
if(write(ofp,ob,j)<=0) {
	obp=0;
	abt("write error");
	}
lpos=obp=0;
}

partial()
{
int i,j,k,l;
for(i=j=k=0; i<obp; i++) {
	if(ob[i]==MARK) {
		if(k<ON) j=i;
		}
	else k++;
	}
if(j==0) {
	l=obp;
	for(i=ON; i>0; --i)if(ob[i-1]!='\\')break;
	if(i==0)abt("funny line in 'partial()'");
	k=ob[i];
	j=ob[i+1];
	ob[i]='\\';
	ob[i+1]='\n';
	obp=i+2;
	flushout();
	obp=l;
	ob[i]=k;
	ob[i+1]=j;
	for(j=lpos=0; i<l; i++) {
		lpos=+ (ob[j++]=ob[i])!=MARK;
		}
	obp=j;
	return(0);
	}
else {
	ob[j++]='\n';
	l=obp;
	obp=j;
	flushout();
	for(i=lpos=0; j<l; j++) {
		lpos=+ (ob[i++]=ob[j])!=MARK;
		}
	obp=i;
	return(0);
	}
}
static printif() {
if(sharptext[0]==0)return;
printf("%s line %d & ",sharptext,inputno-sharpn0+sharpn1);
}
