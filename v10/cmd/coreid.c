/*%cc -o coreid -s -n -O %
 *
 * coreid [corefile] - display arguments of program that dumped core
 *
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/user.h>
#define	U(x)	(int)(&(((struct user *)0)->x))
#define	BYTESPERCLICK	512
#define	HSIZE	(UPAGES*512L)
#define	FPOFFSET	(HSIZE-0120)
#define	APOFFSET	(HSIZE-0124)
int coref;
char *core="core";
int value;
char cvalue;
unsigned enddata, startdata;
int nchars;
long lseek();
main(argc, argv)
	char *argv[];
{
	register fp, av, ap;
	if(argc > 1)
		core=argv[1];
	coref=open(core, 0);
	if(coref < 0){
		fprintf(stderr, "coreid: can't open %s\n", core);
		exit(1);
	}
	Hseek(U(u_tsize));
	Read();
	startdata=value*BYTESPERCLICK;	/* address after text */
	Read();
	enddata=startdata+value*BYTESPERCLICK;	/* after data & bss */
	Hseek(APOFFSET);
	Read();
	ap=value;
	Hseek(FPOFFSET);
	Read();
	fp=value;
	for(;;){
		Seek(fp+12);
		Read();
		if(value<fp)
			break;
		Seek(fp+8);
		fp=value;
		Read();
		ap=value;
	}
	av=ap+16;
	for(;;){
		Seek(av);
		Read();
		if(value==0)
			break;
		string();
		av+=sizeof(char *);
	}
	if(nchars==0)
		badcore();
	putchar('\n');
}
Hseek(loc)
	register loc;
{
	if(lseek(coref, (long)loc, 0)==-1L)
		badcore();
}
Seek(loc)
	register loc;
{
	if(loc > enddata){
		if(lseek(coref, (loc|0x80000000)+HSIZE, 2)==-1L)
			badcore();
	}else if(lseek(coref, (unsigned)loc-startdata+HSIZE, 0)==-1L)
		badcore();
}
Read()
{
	if(read(coref, &value, sizeof (int))!=sizeof (int))
		badcore();
}
Readc()
{
	if(read(coref, &cvalue, 1)!=1)
		badcore();
}
string()
{
	Seek(value);
	do{
		Readc();
		if(cvalue){
			putchar(cvalue);
			nchars++;
		}
	}while(cvalue);
	putchar(' ');
}
badcore()
{
	char buf[DIRSIZ+1];
	if(lseek(coref, U(u_comm[0]), 0)!=-1 && read(coref, buf, DIRSIZ)==DIRSIZ){
		buf[DIRSIZ+1]=0;
		printf("(%s)\n", buf);
		exit(0);
	}
	fprintf(stderr, "coreid: bad core file `%s'\n", core);
	exit(1);
}
