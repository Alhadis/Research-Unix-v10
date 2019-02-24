/* mjm: started with rhfcc.c	(OCT 78, JAN 79)
 *      Made version to be called from "cc -F[qunl]"; /lib/fpp
 *	When SMALL DEFINEd,
 *	SYMTABSZ, OB, N changed to fit into 16k
 */
/* #define SMALL	/* mjm: so will fit on LSX with 16K */
#define MARK (-1)
#ifndef SMALL
#define SYMTABSZ 4096
#define N 200
#define OB 200
#endif
#ifdef SMALL
#define SYMTABSZ 1024	/* mjm: was 4096 (11/20 only 16K) */
#define N 100	/* mjm: was 200 */
#define OB 100	/* mjm: was 200 */
#endif
#define ON 77

#define SHARPBUFL 128	/* chars in # lines max */
#define HEADLEN 4	/* chars in Q object header */
#define SHARPLEN 60	/* max chars in filename from # lines */
static char sharptext[SHARPLEN];
static int sharpn0,sharpn1; /* n1 = value from line n0 = inputno */

extern int unixfmt; /* if set, use unix format floating point */
extern int unsign; /* 1 for unsigned allowed, 0 for not */
static struct obj {
	int type,size;
	char text[2];
	};
static char ib[N],ugb[N];
static int ibp = {N}, ugbp = {0};
static int ifp,ofp;
static char ob[OB];
static int obp,lpos;
static inputno;
static nopt,qopt,uopt, lopt;	/* mjm: no popt,vopt,eopt; but lopt */
static char *infile;
char *symtout;	/* mjm: file for rhsymtab.c (command arg) */

main(ac,av)
int ac;
char **av;
{
int i;
char *s;
if(ac < 4) {	/* mjm */
	printf("fpp arg count error\n");	/* mjm */
	exit(1);
}
if(ac == 5) {	/* mjm */
	s = av[4];
	while(*s)
		switch(*s++) {
			case 'q': qopt=1; break;
			case 'n': nopt=1; break;
			case 'u': uopt=1; break;
			case 'l': lopt=1; break;
		}
}
if(nopt) unixfmt=0;
else unixfmt=1;

if(uopt) unsign=0; /* no unsigned declaration in output */
else unsign=1; /* declare _ftou() and _ntou() unsigned */

infile=av[1];
if((ifp=open(infile,0))<0) abt("can't open .i for read");
if((ofp=creat(av[2],0666))<0) abt("can't open fcc write file");
symtout = av[3];	/* mjm */
symtab(SYMTABSZ);

file();
flushout();
close(ifp);
close(ofp);
unlink(symtout);	/* mjm: rm symbol table file */
exit(0);
}

abt(s)
char *s;
{
pdiag("abt",s);	/* mjm */
flushout();
exit(1);
}

warn(s)
char *s;
{
if(qopt==0)
	pdiag("warn",s);
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
register char *temp; /* buffer for # line */
char outtemp[16+SHARPLEN]; /* buffer for # number on line numbers */
char *s;	/* mjm */
static int newline = 1; /* if 1, next char starts a new line. check for # */
if(ugbp==0) {
gc2top:	if(ibp>=N) {
		j=read(ifp,ib,N);
		if(j<0)j=0;
		for(i=j; i<N; i++) ib[i]=0;
		ibp=0;
		}
	if(newline) {	/* check for # line to skip */
		newline=0;
		if(lopt)	/* mjm: only to get extra line numbers */
		if(sharptext[0]) { /* toss out # n file if ever seen # */
			for(k=0; outtemp[k]="# "[k]; k++);
			outtemp[i=k+5]=0; /* convert 5 digits */
			j=inputno-sharpn0+sharpn1;
			while(j) {
				outtemp[--i]=j%10+'0';
				j/=10;
				}
			while(outtemp[k++]=outtemp[i++]);
			outtemp[k-1] = ' ';	/* mjm */
			s = sharptext; 	/* mjm: filename also */
			while(outtemp[k++] = *s++) ; /* mjm */
			outtemp[k-1]='\n';
			write(ofp,outtemp,k);
			}
		if(ib[ibp]=='#') {
			temp=(char *)getq(SHARPBUFL-HEADLEN); /* get buffer for line */
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
	inputno+=newline=ib[ibp]=='\n'; /* ha! */
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
		lpos+= c!=MARK;
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
		lpos+= (ob[j++]=ob[i])!=MARK;
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
		lpos+= (ob[i++]=ob[j])!=MARK;
		}
	obp=i;
	return(0);
	}
}
static pdiag(name,s)	/* mjm: replaces printif() and printf's in abt and warn */
char *name, *s;
{
	printf("%s line %d:", sharptext,inputno-sharpn0+sharpn1);
	printf("  fpp %s: %s\n", name,s);
}
