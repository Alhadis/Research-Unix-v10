#include "sys/types.h"
#include "sys/stat.h"
#include "f2c.h"
#include "fio.h"
#include "string.h"
#include "fcntl.h"
#include "rawio.h"
#ifndef O_WRONLY
#define O_RDONLY 0
#define O_WRONLY 1
#endif

#ifdef KR_headers
extern char *malloc(), *mktemp();
extern FILE *fdopen();
extern integer f_clos();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
extern int f__canseek(FILE*);
extern integer f_clos(cllist*);
#endif

#ifdef NON_ANSI_RW_MODES
char *f__r_mode[2] = {"r", "r"};
char *f__w_mode[2] = {"w", "w"};
#else
char *f__r_mode[2] = {"rb", "r"};
char *f__w_mode[2] = {"wb", "w"};
#endif

#ifdef KR_headers
f__isdev(s) char *s;
#else
f__isdev(char *s)
#endif
{
#ifdef MSDOS
	int i, j;

	i = open(s,O_RDONLY);
	if (i == -1)
		return 0;
	j = isatty(i);
	close(i);
	return j;
#else
	struct stat x;

	if(stat(s, &x) == -1) return(0);
#ifdef S_IFMT
	switch(x.st_mode&S_IFMT) {
		case S_IFREG:
		case S_IFDIR:
			return(0);
		}
#else
#ifdef S_ISREG
	/* POSIX version */
	if(S_ISREG(x.st_mode) || S_ISDIR(x.st_mode))
		return(0);
	else
#else
	Help! How does stat work on this system?
#endif
#endif
		return(1);
#endif
}
#ifdef KR_headers
integer f_open(a) olist *a;
#else
integer f_open(olist *a)
#endif
{	unit *b;
	int n;
	char buf[256];
	cllist x;
#ifndef MSDOS
	struct stat stb;
#endif
	if(a->ounit>=MXUNIT || a->ounit<0)
		err(a->oerr,101,"open")
	f__curunit = b = &f__units[a->ounit];
	if(b->ufd) {
		if(a->ofnm==0)
		{
		same:	if (a->oblnk)
				b->ublnk = *a->oblnk == 'z' || *a->oblnk == 'Z';
			return(0);
		}
#ifdef MSDOS
		if (b->ufnm
		 && strlen(b->ufnm) == a->ofnmlen
		 && !strncmp(b->ufnm, b->ufnm, (unsigned)a->ofnmlen))
			goto same;
#else
		g_char(a->ofnm,a->ofnmlen,buf);
		if (f__inode(buf,&n) == b->uinode && n == b->udev)
			goto same;
#endif
		x.cunit=a->ounit;
		x.csta=0;
		x.cerr=a->oerr;
		if((n=f_clos(&x))!=0) return(n);
		}
	b->url=a->orl;
	b->ublnk = a->oblnk && (*a->oblnk == 'z' || *a->oblnk == 'Z');
	if(a->ofm==0)
	{	if(b->url>0) b->ufmt=0;
		else b->ufmt=1;
	}
	else if(*a->ofm=='f' || *a->ofm == 'F') b->ufmt=1;
	else b->ufmt=0;
#ifdef url_Adjust
	if (b->url && !b->ufmt)
		url_Adjust(b->url);
#endif
	if (a->ofnm) {
		g_char(a->ofnm,a->ofnmlen,buf);
		if (!buf[0])
			err(a->oerr,107,"open")
		}
	else
		sprintf(buf, "fort.%ld", a->ounit);
	b->uscrtch = 0;
	switch(a->osta ? *a->osta : 'u')
	{
	case 'o':
	case 'O':
#ifdef MSDOS
		if(access(buf,0))
#else
		if(stat(buf,&stb))
#endif
			err(a->oerr,errno,"open")
		break;
	 case 's':
	 case 'S':
		b->uscrtch=1;
#ifdef _POSIX_SOURCE
		tmpnam(buf);
#else
		(void) strcpy(buf,"tmp.FXXXXXX");
		(void) mktemp(buf);
#endif
		(void) close(creat(buf, 0666));
		break;
	case 'n':
	case 'N':
#ifdef MSDOS
		if(!access(buf,0))
#else
		if(!stat(buf,&stb))
#endif
			err(a->oerr,128,"open")
		/* no break */
	case 'r':	/* Fortran 90 replace option */
	case 'R':
		(void) close(creat(buf, 0666));
		break;
	}

	b->ufnm=(char *) malloc((unsigned int)(strlen(buf)+1));
	if(b->ufnm==NULL) err(a->oerr,113,"no space");
	(void) strcpy(b->ufnm,buf);
	b->uend=0;
	b->uwrt = 0;
	if(f__isdev(buf))
	{	b->ufd = fopen(buf,f__r_mode[b->ufmt]);
		if(b->ufd==NULL) err(a->oerr,errno,buf)
	}
	else {
		if((b->ufd = fopen(buf, f__r_mode[b->ufmt])) == NULL) {
			if ((n = open(buf,O_WRONLY)) >= 0) {
				b->uwrt = 2;
				}
			else {
				n = creat(buf, 0666);
				b->uwrt = 1;
				}
			if (n < 0
			|| (b->ufd = fdopen(n, f__w_mode[b->ufmt])) == NULL)
				err(a->oerr, errno, "open");
			}
	}
	b->useek=f__canseek(b->ufd);
#ifndef MSDOS
	if((b->uinode=f__inode(buf,&b->udev))==-1)
		err(a->oerr,108,"open")
#endif
	if(a->orl && b->useek) rewind(b->ufd);
	return(0);
}
#ifdef KR_headers
fk_open(seq,fmt,n) ftnint n;
#else
fk_open(int seq, int fmt, ftnint n)
#endif
{	char nbuf[10];
	olist a;
	(void) sprintf(nbuf,"fort.%ld",n);
	a.oerr=1;
	a.ounit=n;
	a.ofnm=nbuf;
	a.ofnmlen=strlen(nbuf);
	a.osta=NULL;
	a.oacc= seq==SEQ?"s":"d";
	a.ofm = fmt==FMT?"f":"u";
	a.orl = seq==DIR?1:0;
	a.oblnk=NULL;
	return(f_open(&a));
}
