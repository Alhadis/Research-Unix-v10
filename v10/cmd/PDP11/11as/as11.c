/* PDP-11 assembler pass 0 */
#include "as1.h"
#include <signal.h>

#ifndef PASS2PGM
#define PASS2PGM "/lib/as2"		/*god: is tweakable */
#endif
#ifndef A_DOT_OUT
#define A_DOT_OUT "a.out"		/*god: " */
#endif

char *atmp1 = "/tmp/atm1XXXXXX";
char *atmp2 = "/tmp/atm2XXXXXX";
char *atmp3 = "/tmp/atm3XXXXXX";
char *fpass2 = PASS2PGM;		/*god: use -DPASS2PGM=/xxxx... */
char unglob[3] = "-";
char *outfile = A_DOT_OUT;		/*god:6/17/80 made for porting*/
int	debug;

main(argc, argv)
char **argv;
{
	extern char *sbrk();
	extern aexit();
	FILE *t;
	char *buf1,*buf2;

	if (signal(SIGINT,SIG_IGN)!=SIG_IGN) signal(SIGINT,aexit);
	while (--argc>=0 && (++argv)[0][0]=='-') {
		if (argv[0][1]=='u' || argv[0][1]=='\0') unglob[1]='g';
		else if (argv[0][1]=='o' && --argc>=0) outfile= *++argv;
		else if (argv[0][1]=='d') {
			atmp1 = "T1"; atmp2 = "T2", atmp3 = "T3";
			debug++;
		}
	}
	nargs=argc+1; curarg=argv-1;
	if (NULL==(pof=fopen(mktemp(atmp1),"w"))) wrterr();
	setbuf(pof,buf1=sbrk(BUFSIZ));
	if (NULL==(fbfil=fopen(mktemp(atmp2),"w"))) wrterr();
	setbuf(fbfil,buf2=sbrk(BUFSIZ));
	setup();
	fclose(stdin);
	assem();
	if (EOF==fclose(pof)) wrterr(); if (EOF==fclose(fbfil)) wrterr();
	if (errflg) aexit();
	if (NULL==(t=fopen(mktemp(atmp3),"w"))) wrterr();
	setbuf(t,buf1);
	if ((symend-usymtab)!=fwrite(usymtab,sizeof(*usymtab),symend-usymtab,t)) wrterr();
	if (EOF==fclose(t)) wrterr();
	if (debug)
		exit(1);
	execl(fpass2,fpass2,unglob,"-o",outfile,atmp1,atmp2,atmp3,0);
	filerr(fpass2,"?\n");
	aexit();
}

aexit()
{
	unlink(atmp1); unlink(atmp2); unlink(atmp3);
	exit(3);
}

filerr(file,str)
char *file,*str;
{
	fprintf(stderr,"%s%s",file,str);
}

wrterr()
{
	fprintf(stderr,"as: Write error on temp file.\n");
	++errflg; aexit();
}
