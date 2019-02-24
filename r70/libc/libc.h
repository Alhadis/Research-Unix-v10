/*
 * mem routines
 */
void*	memset(void*, int, long);
int	memcmp(void*, void*, long);
void*	memcpy(void*, void*, long);
void*	memmove(void*, void*, long);
void*	memchr(void*, int, long);
void*	memccpy(void*, void*, int, long);

/*
 * string routines
 */
char*	strcat(char*, char*);
char*	strchr(char*, char);
int	strcmp(char*, char*);
char*	strcpy(char*, char*);
char*	strncat(char*, char*, long);
char*	strncpy(char*, char*, long);
int	strncmp(char*, char*, long);
char*	strrchr(char*, char);
char*	strtok(char*, char*);
long	strlen(char*);
long	strspn(char*, char*);
long	strcspn(char*, char*);
char*	strpbrk(char*, char*);
char*	strdup(char*);

/*
 * malloc
 */
void*	malloc(long);
void	free(void*);
void*	calloc(long, long);
void*	realloc(void*, long);
void	cfree(void*);
void*	galloc(unsigned int);
void	gfree(void*);
void	garbage(void);

/*
 * print routines
 */
void	strconv(char*, int, int);
int	fmtinstall(char, int (*)(void*, int, int, int));
char*	doprint(char*, char*, void*);
char*	donprint(char*, char*, char*, void*);
void	flt_init(void);
int	print(char*, ...);
int	sprint(char*, char*, ...);
int	fprint(int, char*, ...);

/*
 * random number
 */
void	srand(long);
int	rand(void);
int	nrand(int);
long	lrand(void);
long	lnrand(long);
double	frand(void);

/*
 * math
 */
double	pow(double, double);
double	atan2(double, double);
double	fabs(double);
double	atan(double);
double	log(double);
double	log10(double);
double	exp(double);
double	erf(double);
double	erfc(double);
double	floor(double);
double	ceil(double);
double	gamma(double);
double	hypot(double, double);
double	sin(double);
double	cos(double);
double	tan(double);
double	asin(double);
double	acos(double);
double	sinh(double);
double	cosh(double);
double	tanh(double);
double	sqrt(double);

#define	HUGE	3.4028234e38
#define	PIO2	1.570796326794896619231e0
#define	PI	(PIO2+PIO2)

/*
 * one-of-a-kind
 */
int	abs(int);
long	labs(long);
int	gcd(int, int);
int	lcm(int, int);
int	min(int, int);
int	max(int, int);
double	pow10(int);
double	frexp(double, int*);
double	ldexp(double, int);
double	modf(double, double*);
char*	mktemp(char*);
void	monitor(int(*)(void), int(*)(void), void*, long, ...);
char*	crypt(char*, char*);
void	setkey(char*);
void	encrypt(char*, int);
int	getfields(char*, char**, int);
int	getmfields(char*, char**, int);
char	*setfields(char*);
char*	getlogin(void);
int	getopt(int, char**, char*);
char*	getpass(char*);
char*	getwd(char*);
char*	getcwd(char*, int);
int	isatty(int);
int	atexit(int (*)(void));
int	onexit(int (*)(void));
int	ptopen(char*);
void	sleep(unsigned int);
void	swab(void*, void*, int);
char*	tmpnam(char*);
char*	ttyname(int);
int	ttyslot(int);
void	abort(void);
void	perror(char*);
double	atof(char*);
long	atol(char*);
int	atoi(char*);
int	exit(int);
char*	getenv(char*);
void	qsort(void*, long, long, int (*)(void*, void*));

/*
 * Time-of-day
 */

struct	tm
{
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
} ;

struct tm*	gmtime(long*);
struct tm*	localtime(long*);
char*	asctime(struct tm*);
char*	ctime(long*);
char*	timezone(int, int);

#define MAXNAMLEN 255
#define DIRBLKSIZ 512

typedef
struct	direct
{
	unsigned long d_ino;
	short	d_reclen;
	short	d_namlen;
	char	d_name[MAXNAMLEN+1];
} Direct;

typedef
struct	_dirdesc
{
	int	dd_fd;
	long	dd_loc;		/* where we left off in dd_buf */
	long	dd_size;	/* bytes back from system */
	long	dd_offset;	/* lseek at beginning of dd_buf */
	char	dd_buf[DIRBLKSIZ];
} DIR;

DIR*	opendir(char*);
Direct*	readdir(DIR*);
long	telldir(DIR*);
void	seekdir(DIR*, long);
void	closedir(DIR*);

/*
 * manual section 2
 *
 */
typedef
struct	stat
{
	unsigned short	st_dev;
	unsigned short	st_ino;
	unsigned short	st_mode;
	short		st_nlink;
	short		st_uid;
	short		st_gid;
	unsigned short	st_rdev;
	long		st_size;
	long		st_atime;
	long		st_mtime;
	long		st_ctime;
} Stat;

#define	S_IFMT	0170000		/* type of file */
#define		S_IFDIR	0040000	/* directory */
#define		S_IFCHR	0020000	/* character special */
#define		S_IFBLK	0060000	/* block special */
#define		S_IFREG	0100000	/* regular */
#define		S_IFLNK 0120000	/* symbolic link */
#define	S_ISUID	0004000		/* set user id on execution */
#define	S_ISGID	0002000		/* set group id on execution */
#define	S_IREAD	0000400		/* read permission, owner */
#define	S_IWRITE 0000200	/* write permission, owner */
#define	S_IEXEC	0000100		/* execute/search permission, owner */
#define	S_ICCTYP 0007000	/* type of concurrent access */
#define		S_ISYNC	0001000	/* 1 writer and n readers (synchronized access) */
#define		S_IEXCL	0003000	/* 1 writer or n readers (exclusive access) */

#define	NSIG	32
#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt */
#define	SIGQUIT	3	/* quit */
#define	SIGILL	4	/* illegal instruction (not reset when caught) */
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#define	SIGIOT	6	/* IOT instruction */
#define	SIGEMT	7	/* EMT instruction */
#define	SIGFPE	8	/* floating point exception */
#define	SIGKILL	9	/* kill (cannot be caugt or ignored) */
#define	SIGBUS	10	/* bus error */
#define	SIGSEGV	11	/* segmentation violation */
#define	SIGSYS	12	/* bad argument to system call */
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#define	SIGSTOP	17	/* sendable stop signal not from tty */
#define	SIGCONT	19	/* continue a stopped process */
#define	SIGCHLD	20	/* to parent on child stop or exit */
#define	SIGLAB	26	/* file label changed; secure unix only (not reset) */

typedef
struct	fd_set
{
	unsigned int fds_bits[5];
} Fd_set;

struct	tms
{
	long	tms_utime;
	long	tms_stime;
	long	tms_cutime;
	long	tms_cstime;
};

int	access(char*, int);
int	acct(char*);
unsigned alarm(int);
int	brk(void*);
int	chdir(char*);
int	chmod(char*, int);
int	chroot(char*);
int	chown(char*, int, int);
int	close(int);
int	creat(char*, int);
int	dup(int);
int	dup2(int, int);
int	execl(char*, ...);
int	execle(char*, ...);
int	execlp(char*, ...);
int	exect(char*, char**, char**);
int	execv(char*, char**);
int	execve(char*, char**, char**);
int	execvp(char*, char**);
void	_exit(int);
int	fmount(int, int, char*, int);
int	fchmod(int, int);
int	fchown(int, int, int);
int	funmount(char*);
int	fork(void);
int	fstat(int, Stat*);
int	getegid(void);
int	geteuid(void);
int	getgid(void);
int	getlogname(char*);
int	getpid(void);
int	getppid(void);
int	getuid(void);
int	ioctl(int, int, void*);
int	kill(int, int);
int	link(char*, char*);
long	lseek(int, long, int);
int	lstat(char*, Stat*);
int	mkdir(char*, int);
int	mknod(char*, int, int);
int	nap(int);
int	nice(int);
int	newpgrp(void);
int	open(char*, int);
int	pipe(int[2]);
int	profil(void*, int, int, int);
long	read(int, void*, int);
int	readlink(char*, char*, int);
int	reboot(int);
int	rmdir(char*);
void*	sbrk(int);
int	select(int, Fd_set*, Fd_set*, int);
void	(*signal(int, void (*)(int)))(int);
int	stat(char*, Stat*);
int	stime(long*);
void	sync(void);
int	symlink(char*, char*);
int	syscall(int, ...);
long	time(long*);
int	times(struct tms*);
int	umask(int);
int	unlink(char*);
int	utime(char*, long[2]);
int	wait(int*);
long	write(int, void*, int);

