#define M32LD   "/bin/32reloc"
#define	BINARY_LOAD	0
#define	HEX_LOAD	1
int Loadtype = BINARY_LOAD;

/*
 *      MAC-32 (down)loader
 */

char    Usage[]         = "Usage: 32ld [-d] [-p] [-z] objectfile";
		/* also a [-l] flag, but the user never sees this */
char	*name;

/*
 * swapw        words must be swapped between host and MAC-32
 * swapb        bytes must be swapped between host and MAC-32
 */
#ifdef  pdp11
#define swapb   1
#define swapw   0
#endif
#ifdef  vax
#define swapb   1
#define swapw   1
#endif
#ifdef  u3b
#define swapb   0
#define swapw   0
#endif
#ifdef  u3b2
#define swapb   0
#define swapw   0
#endif
#ifdef  u3b5
#define swapb   0
#define swapw   0
#endif

#include <sgtty.h>
#include "a.out.h"
#include "aouthdr.h"
#include "filehdr.h"
#include "scnhdr.h"
#include <stdio.h>
#include <errno.h>
#include <jioctl.h>
#include "proto.h"
#include <sys/types.h>
#include <sys/stat.h>

#define MAXRETRIES      10
#define SLEEPTIME       7
#define DATASIZE        512

#define NSECTS  12

#define MPX_VER 0x5620
#define FBOMAGIC 0560
#define SENDTERMID "\033[c"
#define TERM_1_0 "\033[?8;7;1c"
#define TERMB_1_0 "\033[?8;7;2c"
#define TERM_DMD "\033[?8;"
#define TERMIDSIZE 9
#define STR_EQUAL 0

struct filehdr fileheader;
struct aouthdr aoutheader;
struct scnhdr secthdrs[NSECTS];
char    *errname;       /* name of error file for m32ld */

#if vax | u3b | u3b2
struct sgttyb   ttysave,        /* save the state of tty */
		ttyraw;
struct ttydevb ttydsave, ttydraw;
#else
struct termio   ttysave,        /* save the state of tty */
		ttyraw;
#endif
int     obj;            /* File descriptor for object file */
int     mpx;            /* Running under mpx */
long    location;
char    file[128];      /* Name of file */
char    m32ld[128];     /* path name to m32ld command */
int     nargchars;      /* Number of characters, including nulls, in args */
long    longbuf[3];
int     debug;          /* Show sizes etc. */
int     psflag;         /* Print error detection statistics */
short   maxpktdsize;
int     rflag;          /* relocate? */
int     zflag;          /* Do a JZOMBOOT */
int     booted;
int     errfile;
int     retries;
int     open();
int     access();
char    *malloc();

char    Load_str[] = "\033[0;0v";               /* default download (binary & standalone*/
int     Layerflag = 0;
struct stat Statbuf, *Statptr;

short speeds[16]={
	 1,     5,      7,      10,     13,     15,     20,     30,
	60,     120,    180,    240,    480,    960,    1920,   1
};

unsigned char sizes[16]={
	 16,    16,     16,     16,     16,     16,     16,     16,
	 16,    32,     32,     56,     56,     120,    60,     16
};

void    Psend();
void    Precv();
void	Write();

extern int      errno;
extern char *getenv();

timeout_id()
{
	error(0, "Error: can't identify terminal:\n\t not a 5620 terminal or 5620 failed to respond",
		(char *) 0);
}

main(argc, argv)
int argc;
register char *argv[];
{
	char    *dwnldflag;

#ifndef M32LD
	M32LD;          /* force a compile time error if undefined */
#endif
			/* Start out by checking that download is going */
			/* to a DMD with at least 1.1 firmware (not 1.0) */
	char termid[TERMIDSIZE+1];
	int lpindex;
	int count;
	char *strcpy(), *strcat();

	for (lpindex=0; lpindex<=TERMIDSIZE; lpindex++)
		termid[lpindex] = 0;

	(void)strcpy(m32ld, getenv("DMDSGS")? getenv("DMDSGS") : "/usr/jerq");
	(void)strcat(m32ld, M32LD);   /* attach tail of path to head */

#if vax | u3b | u3b2
	(void)ioctl(1, TIOCGETP, &ttysave); /* get the current state */
	if (ioctl(1, TIOCGDEV, &ttydsave) < 0) {
		ttydsave.ospeed = ttysave.sg_ospeed;
		ttydsave.ispeed = ttysave.sg_ispeed;
	}
#else
	(void)ioctl(1, TCGETA, &ttysave); /* get the current state */
#endif

	name = *argv;

	while(argc>1 && argv[1][0]=='-'){
		switch(argv[1][1]){
		case 'd':
			debug++;
			break;
		case 'e':
			printf("-e flag no longer used: always assumed\n");
			break;
		case 'p':
			psflag++;
			break;
		case 'z':
			zflag++;
			break;
		case 'l':
			Layerflag++;
			break;
		case '\0':
			break;
		default:
			error(0, Usage, (char *)0);
			return 1;
		}
		argv++; argc--;
	}
	if(argc<2){
		error(0, Usage, (char *)0);
		return 2;
	}

	/*
	 * see what type of download is expected
	 */
	if(((dwnldflag = getenv("DMDLOAD")) != NULL) && (dwnldflag[0] != NULL)) {
		if(strcmp(dwnldflag, "hex") == 0)
			Loadtype = HEX_LOAD;
		else
			Loadtype = BINARY_LOAD;
	}
	Load_str[4] = Loadtype + '0';


#if vax | u3b | u3b2
	ttyraw = ttysave;
	ttydraw = ttydsave;
	ttyraw.sg_flags |= RAW;
	ttydraw.flags |= F8BIT;
	(void)ioctl(1, TIOCSETP, &ttyraw);
	(void)ioctl(1, TIOCSDEV, &ttydraw);
#else
	ttyraw.c_iflag = IGNBRK;
	ttyraw.c_cflag = (ttysave.c_cflag & CBAUD) | (ttysave.c_cflag & CLOCAL) | CS8 | CREAD;
	ttyraw.c_cc[VMIN] = 1;
	(void)ioctl(1, TCSETAW, &ttyraw);
#endif
	mpx = (ioctl(1, JMUX, 0) > -1);
	if ( !mpx )
	{

		/* make sure we've got the correct rom */
		write(1,SENDTERMID,strlen(SENDTERMID));
		count = 0;
		while(count < TERMIDSIZE){
			lpindex = read(0,&termid[count],TERMIDSIZE); 
			if(lpindex <= 0)
				error(1, "read error", (char *)0);
			count += lpindex;
		}
		if ((strcmp(termid,TERM_1_0) == STR_EQUAL) ||   /* equal strings */
			(strcmp(termid,TERMB_1_0) == STR_EQUAL))
			error(0,"Error: Firmware not updated to 1.1 or greater\n",
				(char *) 0);
		if (strncmp(termid,TERM_DMD,strlen(TERM_DMD)) != STR_EQUAL)
			error(0, "Error: 32ld must be run on a 5620 terminal\n",
				(char *) 0);
	} else {

		/* load sequence for mux is shorter */
		Load_str[3] = '\0';
	}

	if(jpath(argv[1], access, 4)!=0)
		error(1, "no such file '%s'", argv[1]);

	Statptr= &Statbuf;
	stat(argv[1], Statptr);
	if(Layerflag && (Statbuf.st_size == 0)) {
		Load_str[2] = '2';
		realwrite(Load_str, 6);
		goto cleanup;
	}

	obj=jpath(argv[1], open, 0);
	if(obj<0)
		error(1, "cannot open '%s'", file);

	Read (&fileheader, sizeof(struct filehdr));
	if(fileheader.f_magic!=FBOMAGIC)        /* FBOMAGIC is 0560 */
		error(0, "'%s' is not a MAC-32 a.out", file);
	Read (&aoutheader, fileheader.f_opthdr);

	if (fileheader.f_nscns > NSECTS)
		error(0,"32ld: exceeded max number of sections -- see system administrator");
	if((aoutheader.vstamp==MPX_VER) ^ mpx)  /* MPX_VER is 0x5620 */
		error(0, mpx? "'%s' compiled stand-alone": "'%s' compiled for mpx", file);

	if(((aoutheader.tsize + aoutheader.dsize + aoutheader.bsize) == 0) && Layerflag) {
		Load_str[2] = '2';
		realwrite(Load_str, mpx ? 3 : 6);
		goto cleanup;
	}
	if(Layerflag) {
		/*
		* if it got here then we want to download layers with a patch
		*/
		Load_str[2] = '1';
	}

	if(boot() && rflag==0)
		rflag++;
	if(!mpx){
#if vax | u3b | u3b2
		maxpktdsize = min(sizes[ttydsave.ospeed & 017], (long)MAXPKTDSIZE);
		pinit(speeds[ttydsave.ospeed & 017], maxpktdsize, ACKON);
#else
		maxpktdsize = min(sizes[ttysave.c_cflag&CBAUD], (long)MAXPKTDSIZE);
		pinit(speeds[ttysave.c_cflag&CBAUD], maxpktdsize, ACKON);
#endif
	}

	load(argv[1], argc-1, argv+1);
	if(!mpx){       /* ACKON is always true */
		buzz();
		(void)ioctl(0, TIOCFLUSH, (struct termio *)0);
	}
	(void)ioctl(1, TIOCNXCL, 0);
cleanup:
#if vax | u3b | u3b2
	(void)ioctl(1, TIOCSETP, &ttysave);
	(void)ioctl(1, TIOCSDEV, &ttydsave);
#else
	(void)ioctl(1, TCSETAW, &ttysave);
#endif

	if(psflag)
		pstats(stderr);
	return(0);
}


char *
bldargs(argc, argv)
	char *argv[];
{
	register i;
	register char *argp, *p, *q;
	for(nargchars=0, i=0; i<argc; i++)
		nargchars+=strlen(argv[i])+1;
	if((argp=malloc(nargchars))==0)
		error("can't allocate argument chars", "");
	/* this loop is probably not necessary, but it's safe */
	for(i=0, q=argp; i<argc; i++){
		p=argv[i];
		do; while(*q++ = *p++);
	}
	return argp;
}


load(f, argc, argv)
	char *f;
	char *argv[];
{
	char *argp;
	long largc;
	int i;
	if(mpx){
		argp=bldargs(argc, argv);
		largc=argc;
		writeswap((char *)&largc, 4);   /* number of arguments */
		largc=nargchars;
		writeswap((char *)&largc, 4);   /* number of chars in arguments */
		writeswap((char *)&aoutheader.tsize, 12);
	}

	if(rflag)
		relocate();
	else
		location = aoutheader.entry;
	if(mpx)
		Write(argp, nargchars);
	for (i = 0; i < fileheader.f_nscns; ++i)     /* read section header array */
		Read (&secthdrs[i], sizeof(struct scnhdr));

	if(debug){
		fprintf(stderr, "%s:\nSection:\taddr:\tsize:\n", file);
		for ( i = 0; i < fileheader.f_nscns; ++i)
			fprintf(stderr,"%s\t\t0x%lx\t0x%lx\n",
			secthdrs[i].s_name,secthdrs[i].s_paddr,secthdrs[i].s_size);
		buzz();
	}
	sendfile();
	if(!mpx){
		long    startaddr;

		retries = 0;
		while(freepkts != NPBUFS)
			Precv();
		location = aoutheader.entry;
		swaw(&location, &startaddr, PKTASIZE);
		psend((char *)&startaddr, PKTASIZE);
		retries = 0;
		while(freepkts != NPBUFS)
			Precv();
	}
}

jpath(f, fn, a)
	register char *f;
	register int (*fn)();
{
	char *getenv(), *strcpy();
	register char *jp, *p;
	register o;
	if (*f != '/' && strncmp(f, "./", 2) && strncmp(f, "../", 3) && 
	    (jp=getenv("JPATH"))!=0){
		while(*jp){
			for(p=file; *jp && *jp!=':'; p++,jp++)
				*p= *jp;
			if(p!=file)
				*p++='/';
			if(*jp)
				jp++;
			(void)strcpy(p, f);
			if((o=(*fn)(file, a))!=-1)
				return o;
		}
	}
	return((*fn)(strcpy(file, f), a));
}

error(pflag, s1, s2)
	char *s1, *s2;
{
	long flushval = 0L;
	register int    n;
	register int    saverrno;
	char            buf[BUFSIZ];
	extern int      errno;

	saverrno = errno;
	if(booted){
		if (mpx)                /* tell dmd side to give up */
			(void)ioctl(1, JTERM, 0);
		else
		{
			psend((char *)(&flushval),sizeof(long));        
		}
		if(errfile>0){
			buzz();
			while((n=read(errfile, buf, sizeof buf))>0)
				write(2, buf, n);
			unlink(errname);
		}
	}
	(void)ioctl(1, TIOCNXCL, 0);
#if vax | u3b | u3b2
	(void)ioctl(1, TIOCSETP, &ttysave);
	(void)ioctl(1, TIOCSDEV, &ttydsave);
#else
	(void)ioctl(1, TCSETAW, &ttysave);
#endif
	if(pflag){
		errno=saverrno;
		perror(s2);
	}
	fprintf(stderr, "%s: ", name);
	fprintf(stderr, s1, s2);
	fprintf(stderr, "\n");
	if(psflag)
		pstats(stderr);
	exit(1);
}
int
Read(a, n)
	char *a;
{
	register i;
	i=read(obj, a, n);
	if(i<0)
		error(1, "read error on '%s'", file);
	return(i);
}
void
Write(a, n)
	char *a;
{
	if(realwrite(a, n)!=n)
		error(1, "write error to jerq", (char *)0);
	if(psflag && !mpx)
		trace(a);
}
writeswap(a, n)
	char *a;
{
	char buf1[DATASIZE+PKTASIZE], buf2[DATASIZE+PKTASIZE];
	swaw(a, buf1, n);
	swab(buf1, buf2, n);
	Write(buf2, n);
}
trace(a)
	char *a;
{
	register int    i;

	for(i=0; i<(PKTHDRSIZE+PKTASIZE); i++)
		fprintf(stderr, "<%o>", a[i]&0xff);
	fprintf(stderr, "\n");
}

sendfile()
{
	register int i;
	for ( i = 0; i<fileheader.f_nscns; ++i) {
		if(secthdrs[i].s_scnptr > 0)  {
			if ((secthdrs[i].s_flags & STYP_NOLOAD) ||
			    (secthdrs[i].s_flags & STYP_DSECT))
				continue;
			lseek(obj,secthdrs[i].s_scnptr,0);
			sendseg(secthdrs[i].s_paddr,secthdrs[i].s_paddr+secthdrs[i].s_size);
		}
	}
}

sendseg(strloc,endloc)
long strloc;
long endloc;
{
	char buf[DATASIZE+PKTASIZE], buf2[DATASIZE];
	char tmpbuf[DATASIZE+PKTASIZE];
	register n;
	while((n=Read(&buf[PKTASIZE], min(!mpx?maxpktdsize:DATASIZE, endloc-strloc)))>0){
		if(mpx){
	/*              swab(&buf[PKTASIZE], buf2, n); NOT RITE with m32 sgs*/
			Write(&buf[PKTASIZE], n);
		}else{
			swab(&buf[PKTASIZE], &tmpbuf[PKTASIZE], n);
			swaw((short *)&strloc, (short *)&tmpbuf[0], PKTASIZE);
			Psend(tmpbuf, n+PKTASIZE);
		}
		strloc+=n;
	}
}
void
Psend(bufp, count)
	char *bufp;
	int count;
{
	retries = 0;
	while(freepkts == 0)
		Precv();
	psend(bufp, count);
}
void
Precv()
{
	char c;

	alarm(3);               /* sleep at least 2 seconds */
	if(realread(&c, 1) == 1){
		alarm(0);
		if(psflag)
			fprintf(stderr, "recv <%o>\n", c&0xff);
		precv(c);
	}else if(errno != EINTR )
		error(1, "read error", (char *)0);
	else if(++retries >= MAXRETRIES)
		error(0, "load protocol failed", (char *)0);
	else if(psflag)
			fprintf(stderr, "recv timeout.. retries=%d\n",retries);
}

min(a, b)
	long b; /* not your average min() */
{
	return(a<b? a : (int)b);
}

swab(a, b, n)
	register char *a, *b;
	register n;
{
#       if(swapb)
	register char *s, *t;
	n/=2;   /* n in bytes */
	s=b+1;
	t=b;
	while(n--){
		*s= *a++;
		*t= *a++;
		s+=2;
		t+=2;
	}
#       else
	while(n--)
		*b++= *a++;
#       endif
}

swaw(a, b, n)
	register short *a, *b;
	register n;
{
#       if(swapw)
	register short *s, *t;
	n/=4;   /* n in bytes */
	s=b+1;
	t=b;
	while(n--){
		*s= *a++;
		*t= *a++;
		s+=2;
		t+=2;
	}
#       else
	n>>=1;
	while(n--)
		*b++= *a++;
#       endif
}
relocate(){
	long address;
	char buf[200];
	char *mktemp();
	long caddress;
	register i;
	register char *p=(char *)&address;
	char	*tmpname;	/* name of temporary file for reloc */
	for(i=0; i<4; i++)
		read(0, p++, 1);
	(void)ioctl(1, TIOCEXCL, 0);	/* must be here so PUSHLD can work in mux */
	swab(&address, &caddress, 4);
	swaw(&caddress, &address, 4);
	location=address;
	if(location==0)
		error(0, "no memory in terminal", (char *)0);
	sprintf(buf, "%s -b %lx %s >%s 2>%s", m32ld, location, file,
		tmpname=mktemp("/tmp/32XXXXXX"),
		errname=mktemp("/tmp/32EXXXXXX"));
	if(system(buf)){
		errfile=open(errname, 0);
		if (!debug)
			unlink(tmpname);
		unlink(errname);
		error(0, "reloc errors", (char *)0);
	}
	close(obj);
	unlink(errname);
	obj=open(tmpname, 0);
	if(obj<0)	/* HELP!! */
		error(1, "tmp file vanished! (%s)", tmpname);
	if (!debug)
		unlink (tmpname);
	Read (&fileheader, sizeof(struct filehdr));
	Read (&aoutheader, fileheader.f_opthdr);
}

boot(){
	char c = 0;

	if(mpx){
		(void)ioctl(0, TIOCFLUSH, 0);      /* throw away type-ahead! */
		(void)ioctl(1, zflag?JZOMBOOT:JBOOT, 0);
	}else{
		write(1, Load_str,6);   /* esc sequence for download */
		while(c != 'a' && c != 1)
			read(0, &c, 1); /* wait for terminal to be ready */
	}
	booted++;
	return mpx;
}

buzz(){
	/* sleep for a time >~0.5 sec; nice if we had nap! */
	sleep(2);       /* sleep(1) not necessarily long enough */
}

/*
* This routine is the lowest level write routine to the dmd.  It provides a 
* simple way to implement a safer download protocol through networks. 
* This requires that a shell varariable will be set if this extra precaution
* is to be taken.
*/

realwrite(a,n)
char *a;
{
	char cbuf[(MAXPKTSIZE + PKTASIZE) * 2], c;
	int i, j, maxsize;

	if((Loadtype == BINARY_LOAD) || mpx){
		return(write(1,a,n));
	}
	else {
		/*
		* do a hex load
		*/
		j = n;
		maxsize = ((MAXPKTSIZE + PKTASIZE) / 2);
		for(i = 0;i < n*2; i++){
			c = *a++;
			cbuf[i]=(c & 0xf) | 0x40;
			cbuf[++i]=((c >> 4) & 0xf) | 0x40;
		}
		i = 0;
		while(n > 0) {
			if(n > maxsize) {
				if(write(1, &cbuf[i*maxsize*2], maxsize*2) != maxsize*2)
					return(-1);
				n -= maxsize;
				i++;
			}
			else {
				if(write(1, &cbuf[i*maxsize*2], n*2) != n*2)
					return(-1);
				n=0;    /* last buffer so don't loop anymore */
			}               
		}
		return(j); /* all correct so return number of actual characters sent */
	}
}

realread(a, n)
char *a;
{
	char    cbuf[2];
	int     i;

	if((Loadtype == BINARY_LOAD) || mpx){
		return(read(0, a, n));
	}
	else {
		for(i = 0; i < n ; i++) {
			if(read(0, cbuf, 2) != 2)
				return(-1);
			*a++ = (cbuf[0] & 0xf) | ((cbuf[1] & 0xf) << 4);
		}
		return(n);
	}
}
