#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/reg.h>
#include <sys/pioctl.h>
#include <sys/pte.h>
#include <sys/vm.h>

#define	UBASE	(long)USRSTACK
#define NSTACK	8192

char *malloc();
#define Malloc(type,n)		(type *)malloc((n)*sizeof(type))

#define Kread(addr, destp)	((addr) && kread(addr, (char *)destp, sizeof(*(destp))))

#define min(a,b)	((a) <= (b) ? (a) : (b))
#define max(a,b)	((a) >= (b) ? (a) : (b))
#define minmax(x,a,b)	min(b,max(a,x))

#define ENTRYSIZ	DIRSIZ

typedef struct Entry {
	struct Entry *next;
	unsigned int e_val;
	char e_name[ENTRYSIZ];
} Entry;

typedef struct List {
	struct List *next;
	int nentry;
	struct Entry *begin;
} List;

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

int Pslcomp();
char *printp(), *fdprint(), *iprint();
char *gettty(), *getfs(), *getuname(), *getargs(), *memcpy();
char *lookdir();
List *devlist, *prlist, *getdir();
Select *selbeg, *selend;
int Fflag, fflag, hflag, lflag, Nflag, nflag, Tflag, uflag;
int aflag, rflag, tflag, xflag;
int mypid, myuid;
int drum, memory;
char *drumf, *memf, *nlistf;

char *progname;

long lseek();

/*
 * data-fetching functions
 */
extern struct ftab {
	int (*f_proc)();	/* fetch a particular proc entry */
	List *(*f_prlist)();	/* fetch list of all processes */
	char *(*f_uarea)();	/* fetch user block */
	int (*f_stack)();	/* fetch top of stack seg, for arguments */
	int (*f_kread)();		/* read kernel memory */
	int (*f_init)();		/* init, open files */
} ftab[];
int mode;		/* index into ftab */

#define	getproc(s, pp)	(*ftab[mode].f_proc)(s, pp)
#define	getprlist()	(*ftab[mode].f_prlist)()
#define	getuarea(f, p, u)	(*ftab[mode].f_uarea)(f, p, u)
#define	getstack(f, p, u, s)	(*ftab[mode].f_stack)(f, p, u, s)
#define	kread(a, b, s)	(*ftab[mode].f_kread)(a, b, s)
#define	openfiles()	(*ftab[mode].f_init)();
