#include <sys/param.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/reg.h>
#include <sys/pioctl.h>

#define	SYSADR	0x80000000L

#define	UBASE	(SYSADR-UPAGES*NBPG)

#define Malloc(type,n)		(type *)malloc((n)*sizeof(type))
#define Calloc(type,n)		(type *)calloc((n),sizeof(type))
#define Realloc(type,ptr,n)	(type *)realloc(ptr,(n)*sizeof(type))
#define Nalloc(type,pp,n)	(type *)nalloc(pp, sizeof(type), (n)*sizeof(type))

char *malloc(), *calloc(), *realloc(), *nalloc(), *balloc();

#define	Ioctl(fd,c,buf)		(ioctl(fd, c, buf) >= 0)
#define	Seek(fd,n)		(lseek(fd, (int)(n), 0) == (int)(n))
#define	Read(fd,buf,n)		(read(fd, (char *)buf, (n)) == (n))

#define Kread(addr, destp)	(((long)(addr) & SYSADR) && Seek(kernel, addr) && \
				Read(kernel, (char *)(destp), sizeof(*(destp))))

#define Sread(fd, addr, destp)	(Seek(fd, addr) && \
				Read(fd, (char *)(destp), sizeof(*(destp))))

#define min(a,b)	((a) <= (b) ? (a) : (b))
#define max(a,b)	((a) >= (b) ? (a) : (b))
#define minmax(x,a,b)	min(b,max(a,x))

typedef struct Dirnode {
	struct Dirnode *next;
	struct direct *begin;
	struct direct *end;
} Dirnode;

typedef struct Select {
	long flag;
	char *id;
	dev_t dev; ino_t ino;
} Select;

#define	SELTTY	1
#define SELXFL	2
#define SELFIL	4

typedef struct Psline {
	int weight;
	char *string;
} Psline;

char *printp(), *fdprint(), *iprint();

char *gettty(), *getfs(), *getuname(), *getargs(), *memcpy();

Dirnode *devlist, *prlist, *getdir();

Select *selbeg, *selend;

int Fflag, fflag, hflag, lflag, Nflag, nflag, Tflag, uflag;

int aflag, rflag, tflag, xflag;

int mypid, myuid, drum, kernel;

char *progname;
