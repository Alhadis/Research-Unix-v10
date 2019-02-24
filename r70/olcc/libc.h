/*
 * mem routines
 */
void*	memccpy(void*, void*, int, long);
void*	memset(void*, int, long);
int	memcmp(void*, void*, long);
void*	memcpy(void*, void*, long);
void*	memchr(void*, int, long);

/*
 * string routines
 */
char*	strcat(char*, char*);
char*	strchr(char*, char);
int	strcmp(char*, char*);
char*	strcpy(char*, char*);
char*	strdup(char*);
char*	strncat(char*, char*, long);
char*	strncpy(char*, char*, long);
int	strncmp(char*, char*, long);
char*	strpbrk(char*, char*);
char*	strrchr(char*, char);
char*	strtok(char*, char*);
long	strlen(char*);
long	strspn(char*, char*);
long	strcspn(char*, char*);

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
double	atof(char*);
long	atol(char*);
int	atoi(char*);
double	pow10(int);
double	frexp(double, int*);
double	ldexp(double, int);
double	modf(double, double*);
char*	getenv(char*);
char*	mktemp(char*);
int	exit(int);
void	qsort(void*, long, long, int (*)(void*, void*));
void	monitor(int(*)(), int(*)(), void*, long, ...);
void	perror(char*);
char*	crypt(char*, char*);
void	setkey(char*);
void	encrypt(char*, int);
int	getfields(char*, char**, int);
int	getmfields(char*, char**, int);
void	setfields(char*);
char*	getlogin(void);
int	getopt(int, char**, char*);
char*	getpass(char*);
char*	getwd(char*);
char*	getcwd(char*, int);
int	isatty(int);
int	onexit(int (*)(void));
int	ptopen(char*);
void	sleep(unsigned int);
void	swab(void*, void*, int);
char*	tmpnam(char*);
char*	ttyname(int);
int	ttyslot(int);
void	abort(void);

/*
 * Time-of-day
 */

typedef
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
} Tm;

Tm*	gmtime(long*);
Tm*	localtime(long*);
char*	asctime(Tm*);
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

DIR*	opendir();
Direct*	readdir();
long	telldir();
void	seekdir();
void	closedir();

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
	short		st_uid;
	short		st_gid;
	unsigned short	st_rdev;
	long		st_size;
	long		st_atime;
	long		st_mtime;
	long		st_ctime;
} Stat;

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
int	fchown(char*, int, int);
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
int	mount(char*, char*);
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
long	settod(long);
void	(*signal(int, void (*)(int)))(int);
int	stat(char*, Stat*);
int	stime(long*);
void	sync(void);
int	symlink(char*, char*);
int	syscall(int, ...);
int	times(struct tms*);
int	unmount(char*, char*);
int	umask(int);
int	unlink(char*);
int	utime(char*, long[2]);
int	wait(int*);
long	write(int, void*, int);
