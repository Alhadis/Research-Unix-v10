#include <sys/ttyio.h>
#include <sys/filio.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#define	ACUFILE1 "/usr/lib/aculist"
#define ACUFILE	"/etc/aculist"
#define DKACU

char	*malloc();
static char	*word();
static struct	aculine *readacu();
static sigalrm(), getspeed(), recordit(), tdkdialout();

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

extern	int	tty_ld;
extern	int	buf_ld;

dialout(telno, type)
char *telno, *type;
{
	char digits[30];
	register struct aculine *ap;
	int pid, dh, retval, r, acu;
	int (*sal)();
	time_t starttime, time();
	struct sgttyb vec;
	struct ttydevb dvec;

	sal = signal(SIGALRM, sigalrm);
	alarm(0);
	retval = -9;		/* unknown service class */
	ap = readacu();
	if (ap == NULL)
		return(retval);
	for (; ap->class; ap++) {
#ifdef DEBUG
printf("class %s, line %s, acu %s, speed %s, prefix %s, postfix %s\n",
ap->class, ap->line, ap->acu, ap->speed, ap->prefix, ap->postfix);
#endif
		if(!eq(ap->class, type))
			continue;
		acu = -1;
		if (eq(ap->acu, "none"))
			goto noACU;
		retval = -1;			/* acu busy */
		if (eq(ap->line, "DKVENTEL") || eq(ap->line, "DKACU")){
			retval = tdkdialout(ap, telno);
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
		strcat(digits, telno);
		strcat(digits, ap->postfix);
		if ((pid=fork())==0) {
			alarm(60);
			close(acu);
			(void) open(ap->line, 2);
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
		if (time((long *)NULL) > starttime+45)  /* quit if timeout */
			break;
	}
ret:
	if (retval > 0) {
		int ld;

		ioctl(retval, FIOPUSHLD, &buf_ld);
		ld = ioctl(retval, FIOLOOKLD, (char *)0);
		if (ld != tty_ld)
			ioctl(retval, FIOPUSHLD, &tty_ld);
		ioctl(retval, TIOCGETP, &vec);
		vec.sg_flags &= ~ECHO;
		vec.sg_flags |= RAW|EVENP|ODDP;
		vec.sg_ispeed = vec.sg_ospeed = getspeed(ap->speed);
		ioctl(retval, TIOCSETP, &vec);
		ioctl(retval, TIOCGDEV, &dvec);
		dvec.ispeed = dvec.ospeed = vec.sg_ispeed;
		dvec.flags |= F8BIT|EVENP|ODDP;
		ioctl(retval, TIOCSDEV, &dvec);
		ioctl(retval, TIOCHPCL, 0);
		ioctl(retval, TIOCEXCL, 0);
		if (vec.sg_ispeed == 0) {
			close(retval);
			retval = -3;
		}
	}
	signal(SIGALRM, sal);
	recordit(type, telno, retval, ap);
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
			return(NULL);
		}
	}
	fstat(f, &sb);
	ap = malloc(2*(unsigned short)sb.st_size);
	if (ap == NULL)
		return(NULL);
	i = read(f, ap, (unsigned short)sb.st_size);
	close(f);
	if (i < 0)
		i = 0;
	ap[i] = '\0';
	for (nline=0, i=0; ap[i]; i++)
		if (ap[i] == '\n')
			nline++;
	acp = (struct aculine *)malloc(2*(nline+1)*sizeof(struct aculine));
	if (acp == NULL)
		return(NULL);
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
recordit(class, telno, retval, ap)
char *class, *telno;
struct aculine *ap;
{
	FILE *fp;
	long time();
	long tm = time(0);
	char *line;
	struct passwd *pwp, *getpwuid();

	fp = fopen("/usr/adm/smdr", "a");
	if (fp == NULL)
		return;
	pwp = getpwuid(geteuid());
	if (ap->class)
		line = ap->line;
	else
		line = "-";	/* couldn't find a line */
	if (retval >= 0)
		retval = 0;
	fprintf(fp, "%s %s %d %s %.12s %s\n", telno, class, retval,
	   pwp?pwp->pw_name:"?", ctime(&tm)+4, line);
	fclose(fp);
}

#ifdef DKACU
static
tdkdialout(ap, telno)
register struct aculine *ap; char *telno;
{
	char atelno[100], *p, *ipcpath();
	int fd;
	strcpy(atelno, ap->acu);
	strcat(atelno, ".");
	p = atelno + strlen(atelno);

	while (*telno) switch (*telno) {
	case '-':
		*p++ = '%'; telno++; break;
	case '=':
		*p++ = '%'; telno++; break;
	case '+':
		*p++ = '&'; telno++; break;
	default:
		*p++ = *telno++; break;
	}
	*p = 0;
	if (eq(ap->speed, "300"))
		strcat(atelno, "/300");
	p = ipcpath(atelno, "dk", "");
	fd = ipcopen(p, "light");
	if(fd < 0)
		return(-3);
	return(fd);
}
#else
static
tdkdialout(addr, telno, dt)
char *addr;
char *telno, *dt;
{
	return(-1);
}
#endif DKACU
