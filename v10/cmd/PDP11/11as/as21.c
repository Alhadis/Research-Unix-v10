/* PDP-11 assembler pass 2 */
#include "as2.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef A_DOT_OUT
#define A_DOT_OUT "a.out"		/*god: is tweakable*/
#endif

struct stat statbuf;

struct {
	char name[NCPS];
	short dtyp;
	short dval;
} dsksym;

struct expr *usymtab;
struct expr *usymend;
struct expr *fbbuf;
long tseek[2];
long rseek[2];
FILE *txtf,*relf;
char *aoutp = A_DOT_OUT;		/*god:6/17/80 put in for porting*/
char *atmp1,*atmp2,*atmp3;
int outmod = 0777;
extern int brtabp;

struct hdr hdr = {0407,0,0,0,0,0,0,0};

main(argc, argv)
char **argv;
{
	extern char *sbrk();
	extern saexit();
	FILE *intfil,*fbfil,*symf;
	int datbase,bssbase;
	int nsym;
	int defund = 0;

	if (signal(SIGINT,SIG_IGN)!=SIG_IGN) signal(SIGINT,saexit);
	while (--argc>=0 && (++argv)[0][0]=='-') {
		if (argv[0][1]=='g') defund=T_EXTERN;
		if (argv[0][1]=='o' && --argc>=0) aoutp= *++argv;
	}
	if (argc!=3) {
		fprintf(stderr, "as2: bad arguments\n");
		saexit();
	}
	intfil=fopen(atmp1= *argv++,"r");
	   fbfil=fopen(atmp2= *argv++,"r");
	    symf=fopen(atmp3= *argv++,"r");
	if (intfil==NULL || fbfil==NULL || symf==NULL) {
		fprintf(stderr, "as2: can't reopen assembler temp file\n");
		saexit();
	}
	if (NULL==(  txtf=fopen(  aoutp,"w"))) wrterr();
	if (NULL==(  relf=fopen(  aoutp,"a"))) wrterr();
	setbuf(intfil,sbrk(BUFSIZ));
	setbuf( fbfil,sbrk(BUFSIZ));
	setbuf(  symf,sbrk(BUFSIZ));
	setbuf(  txtf,sbrk(BUFSIZ));
	setbuf(  relf,sbrk(BUFSIZ));

	/* read symbol table */
	{
		register struct expr *sp;
		register int n,t;

		fstat(fileno(symf),&statbuf); hdr.symsiz=statbuf.st_size;
		nsym=n=statbuf.st_size/sizeof(dsksym);
		if ((struct expr *)-1==(usymtab=sp=(struct expr *)sbrk(n*sizeof(*sp)))) saexit();
		while (--n>=0) {
			if (1!=fread(&dsksym,sizeof(dsksym),1,symf)) wrterr();
			if ((t=dsksym.dtyp&037)==T_TEXT || t==T_DATA) {
				sp->typ=dsksym.dtyp+T_ESTTXT-T_TEXT; sp->val=dsksym.dval;
			} else {sp->typ=0; sp->val=0;}
			sp++;
		}
	}

	/* read f-b definitions */
	{
		register struct expr *fbp;
		register int nfb;

		fstat(fileno(fbfil),&statbuf);
		nfb=statbuf.st_size/sizeof(*fbp);
		if ((struct expr *)-1==(fbbuf=fbp=(struct expr *)sbrk((nfb+1)*sizeof(*fbp)))) saexit();
		if (nfb!=fread(fbp,sizeof(*fbp),nfb,fbfil)) wrterr();
		while (--nfb>=0) fbp++->typ+=T_ESTTXT-T_TEXT; 	/* mark "estimated" */
		usymend=fbp; fbp->typ=0100000;
	}

	setup();
	assem();

	/* prepare for pass2 */
	if (outmod!=0777) aexit();
	*dot=0; *dotrel=T_TEXT; *dotdot=0; brtabp=0;
	if (EOF==fseek(intfil,0L,0)) wrterr();
	setup();
	passno=1;
	hdr.txtsiz=(hdr.txtsiz+1)&~1;
	hdr.datsiz=(hdr.datsiz+1)&~1;
	hdr.bsssiz=(hdr.bsssiz+1)&~1; 
	savdot[T_DATA-T_TEXT]=datbase=hdr.txtsiz;
	savdot[ T_BSS-T_TEXT]=bssbase=datbase+hdr.datsiz;
	tseek[T_TEXT-T_TEXT]=sizeof(hdr);
	tseek[T_DATA-T_TEXT]=sizeof(hdr)+hdr.txtsiz;
	rseek[T_TEXT-T_TEXT]=sizeof(hdr)+hdr.txtsiz+hdr.datsiz;
	rseek[T_DATA-T_TEXT]=sizeof(hdr)+hdr.txtsiz+hdr.datsiz+hdr.txtsiz;

	{/* doreloc */
		register struct expr *sp = usymtab;
		register int i = usymend-sp;
		register int t;

		while (--i>=0) {
			if ((t=sp->typ&0377)==T_UNDEF) sp->typ |= defund;
			t&=037;
			if (t>=T_DATA && t<R_EXTERN)
				if (t==T_BSS) sp->val+=bssbase;
				else sp->val+=datbase;
			sp++;
		}
	}

	if (1!=fwrite(&hdr,sizeof(hdr),1,txtf)) wrterr();
	fseek(txtf,tseek[T_TEXT-T_TEXT],0);
	fseek(relf,rseek[T_TEXT-T_TEXT],0);
	assem();
	
	{/* append full symbol table */
		register struct expr *sp = usymtab;
		register int n = nsym;

		fseek(symf,0L,0); fseek(relf,rseek[T_DATA-T_TEXT],0);
		while (--n>=0) {
			if (1!=fread(&dsksym,sizeof(dsksym),1,symf)) wrterr();
			dsksym.dtyp=sp->typ; dsksym.dval=sp->val;
			if (1!=fwrite(&dsksym,sizeof(dsksym),1,relf)) wrterr();
			sp++;
		}
	}
	aexit();
}

aexit()
{
	unlink(atmp1); unlink(atmp2); unlink(atmp3);
	if (errflg==0) chmod(aoutp,outmod&~umask(0));
	exit(errflg);
}

saexit()
{
	++errflg; aexit();
}

filerr(file,str)
char *file,*str;
{
	fprintf(stderr,"%s%s",file,str);
}

setup()
{
	register int i;

	for (i=20; --i>=0; ) curfb[i]=0;
	for (i=10; --i>=0; ) fbadv(i);
}
