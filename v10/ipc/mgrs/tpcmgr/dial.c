#include <sgtty.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <ipc.h>
#include "defs.h"

#define	ACUFILE1 "/usr/lib/aculist"
#define ACUFILE	"/etc/aculist"

extern int logevent();
extern char *malloc();
static char *word();
static struct aculine *readacu();
static int sigalrm();
static int getspeed();
static int recordit();

struct	aculine {
	char	*class;
	char	*line;
	char	*acu;
	char	*speed;
	char	*prefix;
	char	*postfix;
};

#define	MONITOR
#define	eq(x, y)	(strcmp(x, y)==0)

extern	int	dt_ld;
extern	int	tty_ld;
extern	int	buf_ld;

/* dummy in-dial routines */
int
net_announce(ip)
	ipcinfo *ip;
{
	USE(ip);
	return ABORT(EACCES, "cannot announce to tpc", NULLINFO);
}
ipcinfo *
net_listen(fd)
	int fd;
{
	USE(fd);
	return NULL;
}
void
net_accept(ip)
	ipcinfo *ip;
{
	USE(ip);
	return;
}
void
net_reject(ip, no, str)
	ipcinfo *ip;
	int no;
	char *str;
{
	USE((ip, no, str));
	return;
}

/* dial out on an acu */
int
net_dial(ip)
	ipcinfo *ip;
{
	char digits[30];
	char *params[10];
	int i, np;
	register struct aculine *ap;
	int pid, dh, retval, r, acu;
	int (*sal)();
	time_t starttime, time();

	sal = signal(SIGALRM, sigalrm);
	alarm(0);
	errno = -9;
	errstr = "unknown service class";
	ap = readacu();
	if (ap == NULL)
		return -1;
	setfields(" \t");
	np = getmfields(ip->param, params, 10);
	for (; ap->class; ap++) {
#ifdef DEBUG
printf("class %s, line %s, acu %s, speed %s, prefix %s, postfix %s\n",
ap->class, ap->line, ap->acu, ap->speed, ap->prefix, ap->postfix);
#endif
		for (i=0; i<np; i++) {
			fprintf(stderr, "%s==%s?\n", ap->class, params[i]);
			if(eq(ap->class, params[i]))
				break;
		}
		if (i==np)
			continue;
		acu = -1;
		if (eq(ap->acu, "none"))
			goto noACU;
		retval = -1;			/* acu busy */
		if (eq(ap->line, "DKVENTEL") || eq(ap->line, "DKACU")){
			retval = tdkdialout(ap->acu, ip->name, ap->class);
			if(retval >= 0)
				break;
			continue;
		}
		for (r=0; r<60; r += 3) {
			acu = open(ap->acu, 1);
			if (acu<0 && !eq(ap->prefix, "-")) {
				sleep(3);
				continue;
			}
			break;
		}
		if (acu < 0)
			continue;
		digits[0] = '\0';
		if (!eq(ap->prefix, "-"))
			strcpy(digits, ap->prefix);
		strcat(digits, ip->name);
		strcat(digits, ap->postfix);
		if ((pid=fork())==0) {
			alarm(60);
			close(acu);
			open(ap->line, 2);
			exit(0);
		}
		alarm(20);
#ifdef DEBUG
printf("dial %s\n", digits);
#endif
		r = write(acu, digits, strlen(digits));
#ifdef DEBUG
printf("wrote %d\n", r);
#endif
		alarm(0);
		close(acu);
		if (r!=strlen(digits)) {
			kill(pid, 9);
			wait(0);
			continue;
		}
	noACU:
		alarm(60);
		starttime = time(NULL);
		retval = -3;		/* no carrier */
		dh = open(ap->line, 2);
#ifdef DEBUG
		printf("open %s\n", ap->line);
#endif
		alarm(0);
		kill(pid, 9);
		wait(0);
		if (dh>=0) {
#ifdef DEBUG
			printf("open succeeds\n");
#endif
			retval = dh;
			break;
		}
		close(acu);	/* This compensates for a system bug. */
#ifdef DEBUG
		printf("open failed\n");
#endif
		if (time(NULL) > starttime+45)  /* quit if timeout */
			break;
	}
ret:
	errstr = "no carrier";
	if (retval > 0) {
		struct sgttyb vec;
		struct ttydevb dvec;
		int ld;

		ioctl(retval, FIOPUSHLD, &buf_ld);
		ld = ioctl(retval, FIOLOOKLD, (char *)0);
		if (ld != tty_ld)
			ioctl(retval, FIOPUSHLD, &tty_ld);
		ioctl(retval, TIOCGETP, &vec);
		ioctl(retval, TIOCGDEV, &dvec);
		vec.sg_flags &= ~ECHO;
		dvec.flags &= ~ECHO;
		vec.sg_flags |= RAW|EVENP|ODDP;
		dvec.flags |= RAW|EVENP|ODDP;
		dvec.ispeed = dvec.ospeed =
		 vec.sg_ispeed = vec.sg_ospeed = getspeed(ap->speed);
		ioctl(retval, TIOCSETP, &vec);
		ioctl(retval, TIOCHPCL, 0);
		ioctl(retval, TIOCEXCL, 0);
		if (vec.sg_ispeed == 0) {
			close(retval);
			retval = -3;
		}
	}
	signal(SIGALRM, sal);
	recordit(params[i], retval, ip);
	return(retval);
}

static
sigalrm()
{
	signal(SIGALRM, sigalrm);
	return;
}

static struct aculine *
readacu()
{
	char *ap;
	int nline, i, f;
	struct stat sb;
	struct aculine *acp;

	if ((f = open(ACUFILE, 0)) < 0){
		if((f = open(ACUFILE1, 0)) < 0){
			errstr = "can't open acu";
			return(NULL);
		}
	}
	fstat(f, &sb);
	ap = malloc(2*(unsigned short)sb.st_size);
	if (ap == NULL) {
		errstr = "out of memory";
		return(NULL);
	}
	i = read(f, ap, (unsigned short)sb.st_size);
	close(f);
	if (i < 0)
		i = 0;
	ap[i] = '\0';
	for (nline=0, i=0; ap[i]; i++)
		if (ap[i] == '\n')
			nline++;
	acp = (struct aculine *)malloc(2*(nline+1)*sizeof(struct aculine));
	if (acp == NULL) {
		errstr = "out of memory";
		return(NULL);
	}
	for (i=0; i<nline; i++) {
		ap = word(ap, &acp[i].class, 1);
		ap = word(ap, &acp[i].line, 0);
		ap = word(ap, &acp[i].acu, 0);
		ap = word(ap, &acp[i].speed, 0);
		ap = word(ap, &acp[i].prefix, 0);
		ap = word(ap, &acp[i].postfix, 0);
	}
	acp[nline].class = NULL;
	*ap = '\0';

	return(acp);
}

static char *
word(cp, loc, f)
register char *cp, **loc;
{
	register char *bw;

	while (*cp == ' ' || *cp == '\t' || (*cp == '\n' && f))
		*cp++ = '\0';
	bw = cp;
	while (*cp > ' ')
		cp++;
	*loc = bw;
	return(cp);
}

static
getspeed(sp)
char *sp;
{
	switch(atoi(sp)) {

	case 50:
		return(B50);
	case 75:
		return(B75);
	case 110:
		return(B110);
	case 134:
		return(B134);
	case 150:
		return(B150);
	case 300:
		return(B300);
	case 600:
		return(B600);
	case 1200:
		return(B1200);
	case 1800:
		return(B1800);
	case 2400:
		return(B2400);
	case 4800:
		return(B4800);
	case 9600:
		return(B9600);
	default:
		if (eq(sp, "exta"))
			return(EXTA);
		if (eq(sp, "extb"))
			return(EXTB);
		return(0);
	}
}

static
recordit(class, retval, ip)
char *class;
ipcinfo *ip;
{
	FILE *fp;
	long time();
	long tm = time(0);

	fp = fopen("/usr/adm/smdr", "a");
	if (fp == NULL)
		return(0);
	fprintf(fp, "%s %s %s %s!%s %.12s\n", ip->name, class, retval>=0?"OK":"NG",
	   ip->machine, ip->user, ctime(&tm)+4);
	fclose(fp);
}

#include <ctype.h>

tdkdialout(addr, telno, dt)
char *addr;
char *telno, *dt;
{
	char atelno[100];
	char buf[100];
	int fd, i;

	fprintf(stderr, "tdkdialout %s %s\n", addr, telno);
	for(i = 0; telno[i]; i++){
		if(telno[i] == '-'){
			atelno[i] = '%';
		} else if(telno[i] == '='){
			atelno[i] = '%';
		} else if(telno[i] == '+'){
			atelno[i] = '&';
		} else {
			atelno[i] = telno[i];
		}
	}
	atelno[i] = '\0';
	if(dt[0] == 'C' && atelno[0] != '9' && atelno[0] != '8'){
		/* C1200 or C300; ventel is on dimension, not centrex */
		return(-9);
	}
	sprintf(buf, "/cs/dk!%s!%s", addr, atelno);
	fd = ipcopen(buf, "");
	if(fd < 0)
		return(-3);
	return(fd);
}
