#include	<fio.h>
#include	"backup.h"
#include	"sym.h"
#include	<utsname.h>
#include	<ndir.h>

#define		S_SYS		1
#define		S_REC		2
#define		S_USER		3
#define		S_SYSTEM	4

#define		LOGSCALE	30
#define		SCALE		0x3FFFFFFF
#define		DAY		(24L*3600)

#define		T0		1
#define		T1		7
#define		T2		30

typedef struct Stat
{
	long date;
	long nfiles;
	long nb0;		/* total bytes>>LOGSCALE */
	long nb1;		/* total bytes&SCALE */
	char user[16];
} Stat;

typedef struct sstat
{
	Stat s;
	int fd;
	struct sstat *next;
} sstat;
sstat *root = 0;
int sc;

typedef struct ssstat
{
	Stat s[3];
	char key[32];
	long t0, t1;
} ssstat;

char **fakeargv();
int iflag = 0;
int dflag = 0;
int sall = 0;
int uall = 0;
int (*fmtfn)();
int sufn(), sfn(), ufn(), fn();
long t0, t1, t2;
char bdir[FNAMELEN];

main(argc, argv)
	char **argv;
{
	int fd;
	char buf[FNAMELEN];
	char *uflag = 0;
	char *s;
	char *sflag = 0;
	int cflag = 0;
	int c, i;
	int server;
	DIR *dirp;
	struct direct *dp;
	char *ptrs[4096];
	extern char *optarg;

	backupdir(bdir);
	if(server = (strcmp(argv[1], "-S") == 0))
		argv = fakeargv(Frdline(0), &argc);
	else {
		struct utsname name;

		uname(&name);
		for(s = bdir; *s++;)
			;
		if(strrchr(s, '/'))
			s = strrchr(s, '/')+1;
		if(strcmp(s, name.sysname)){
			dup2(open("/dev/null", 0), 0);
			relay(bdir, argv+1, "bkpstats", "light");	/* it doesn't return */
		}
	}
	while((c = getopt(argc, argv, "cdiu:s:")) != -1)
		switch(c)
		{
		case 'i':	iflag = 1; break;
		case 'c':	cflag = 1; break;
		case 'd':	dflag = 1; break;
		case 's':	sflag = optarg; break; 
		case 'u':	uflag = optarg; break; 
		case '?':	usage();
		}

	if(cflag){
		sprint(buf, "%s/%s", bdir, STATLOG);
		if((fd = open(buf, 0)) < 0){
			perror(buf);
			exit(1);
		}
		lock(bdir);
		readraw(fd);
		close(fd);
		dumpraw();
		if(creat(buf, 0644) < 0)
			perror(buf);
		unlock();
		exit(0);
	}
	if(dflag){
		(void)time(&t0);
		t0 = (t0/DAY)*DAY+DAY;	/* origin one */
		t1 = t0-T1*DAY;
		t2 = t0-T2*DAY;
		t0 -= T0*DAY;
	} else
		t0 = t1 = t2 = 0;
	if(uflag)
		fmtfn = sflag? sufn : ufn;
	else
		fmtfn = sflag? sfn : fn;
	setfields(" ,");
	if(uflag && (strcmp(uflag, "*") == 0))
		uall = 1;
	else if(uflag){
		uall = 0;
		getmfields(uflag, ptrs, sizeof ptrs/sizeof ptrs[0]);
		for(i = 0; ptrs[i]; i++)
			(void)symlook(ptrs[i], S_USER, (void *)1);
	} else
		uall = 1;
	if(sflag && (strcmp(sflag, "*") == 0))
		sall = 1;
	else if(sflag){
		sall = 0;
		getmfields(sflag, ptrs, sizeof ptrs/sizeof ptrs[0]);
		for(i = 0; ptrs[i]; i++)
			(void)symlook(ptrs[i], S_SYSTEM, (void *)1);
	} else
		sall = 1;
	sprint(buf, "%s/%s", bdir, STATDIR);
	dirp = opendir(buf);
	if(dirp == 0){
		perror(buf);
		exit(1);
	}
	for(dp = readdir(dirp); dp != 0; dp = readdir(dirp)){
		if(dp->d_name[0] == '.')
			continue;
		if(sall || (int)symlook(dp->d_name, S_SYSTEM, (void *)0)){
			sprint(buf, "%s/%s/%s", bdir, STATDIR, dp->d_name);
			if((fd = open(buf, 0)) < 0){
				perror(buf);
				exit(1);
			}
			readcooked(fd, dp->d_name);
			close(fd);
		}
	}
	dumpcooked();
	if(server){
		Fputc(1, 0);
		Fputc(1, '\n');
	}
	exit(0);
}

char *
f_malloc(n)
{
	register char *s;

	if(s = malloc(n))
		return(s);
	fprint(2, "malloc(%d) failed\n", n);
	exit(1);
	return("");	/* cyntax */
}

readraw(fd)
{
	struct { long t, l; char u[12]; char s[12];} rec;
	sstat *s;
	char buf[512];
	long null = 0;

	while(Fread(fd, (char *)&rec, (long)sizeof rec) == sizeof rec){
		if(rec.s[0] == 0){
			null++;
			continue;
		}
		talign(&rec.t);
		sprint(buf, "%s.%ld.%s", rec.s, rec.t, rec.u);
		if((s = (sstat *)symlook(buf, S_REC, (void *)0)) == 0){
			s = (sstat *)f_malloc(sizeof(sstat));
			s->s.date = rec.t;
			s->s.nfiles = 0;
			s->s.nb0 = s->s.nb1 = 0;
			strcpy(s->s.user, rec.u);
			s->next = root;
			root = s;
			(void)symlook(strdup(buf), S_REC, (void *)s);
			if((s->fd = (int)symlook(rec.s, S_SYS, (void *)0)) == 0)
				(void)symlook(strdup(rec.s), S_SYS, s->fd = sopen(rec.s));
		}
		s->s.nfiles++;
		s->s.nb1 += rec.l;
		s->s.nb0 += s->s.nb1>>LOGSCALE;
		s->s.nb1 &= SCALE;
	}
	if(null)
		fprint(2, "%ld records discarded\n", null);
}

talign(t)
	long *t;
{
	*t = (*t/DAY)*DAY + 1;
}

sopen(s)
	char *s;
{
	int fd;
	char buf[FNAMELEN];

	sprint(buf, "%s/%s/%s", bdir, STATDIR, s);
	if((fd = open(buf, 1)) < 0){
		if((fd = creat(buf, 0644)) < 0){
			perror(buf);
			unlock();
			exit(1);
		}
	}
	lseek(fd, 0L, 2);
	return(fd);
}

dumpraw()
{
	while(root){
		Fwrite(root->fd, (char *)&root->s, (long)sizeof(Stat));
		root = root->next;
	}
}

sufn(buf, s, u)
	char *buf, *s, *u;
{
	if(iflag)
		sprint(buf, "%s %s", s, u);
	else
		strcpy(buf, "* *");
}

ufn(buf, s, u)
	char *buf, *s, *u;
{
	if(iflag)
		sprint(buf, "* %s", u);
	else
		strcpy(buf, "* *");
}

sfn(buf, s, u)
	char *buf, *s, *u;
{
	if(iflag)
		sprint(buf, "%s *", s);
	else
		strcpy(buf, "* *");
}

fn(buf, s, u)
	char *buf, *s, *u;
{
	strcpy(buf, "* *");
}

readcooked(fd, sys)
	char *sys;
{
	Stat s;
	ssstat *ss;
	char buf[512];

	Finit(fd, (char *)0);
	while(Fread(fd, (char *)&s, (long)sizeof s) == sizeof s){
		if(s.date < t2)
			continue;
		if((uall == 0) && ((int)symlook(s.user, S_USER, (void *)0) == 0))
			continue;
		(*fmtfn)(buf, sys, s.user);
		if((ss = (ssstat *)symlook(buf, S_REC, (void *)0)) == 0){
			ss = (ssstat *)f_malloc(sizeof *ss);
			strcpy(ss->key, buf);
			ss->s[2].nfiles = 0;
			ss->s[2].nb0 = 0;
			ss->s[2].nb1 = 0;
			ss->s[0] = ss->s[1] = ss->s[2];
			ss->t0 = ss->t1 = s.date;
			(void)symlook(strdup(buf), S_REC, (void *)ss);
			sc++;
		}
		if(s.date >= t0)
			upd(&ss->s[0], s.nfiles, s.nb0, s.nb1);
		if(s.date >= t1)
			upd(&ss->s[1], s.nfiles, s.nb0, s.nb1);
		if(s.date >= t2)
			upd(&ss->s[2], s.nfiles, s.nb0, s.nb1);
		if(ss->t0 > s.date)
			ss->t0 = s.date;
		if(ss->t1 < s.date)
			ss->t1 = s.date;
	}
}

upd(s, nfiles, nb0, nb1)
	Stat *s;
	long nfiles, nb0, nb1;
{
	s->nfiles += nfiles;
	s->nb1 += nb1;
	s->nb0 += s->nb1>>LOGSCALE;
	s->nb1 &= SCALE;
	s->nb0 += nb0;
}

prfn(s)
	ssstat *s;
{
	double nb;
	double days;

	nb = s->s[0].nb0*(SCALE+1.0);
	nb += s->s[0].nb1;
	nb /= 1e6;
	days = (s->t1-s->t0)/DAY + 1;
	Fprint(1, "%s[%lg]: %ld(%.1f) files, %.3f(%.3f) MB; %.1f KB\n",
		s->key, days, s->s[0].nfiles, s->s[0].nfiles/days, nb, nb/days,
		nb*1000/s->s[0].nfiles);
}

dprfn(s)
	ssstat *s;
{
	double days, d;
	char buf[128];

#define	PR(x)	Fprint(1, "  %4.0f,%4.1fMB", s->s[x].nfiles/d, (s->s[x].nb0*(SCALE+1.0) + s->s[x].nb1)*1e-6/d)

	days = (s->t1-s->t0)/DAY + 1;
	sprint(buf, "%s:", s->key);
	Fprint(1, "%-12s", buf);
	if((d = days) > T0)
		d = T0;
	PR(0);
	if((d = days) > T1)
		d = T1;
	PR(1);
	if((d = days) > T2)
		d = T2;
	PR(2);
	Fprint(1, "\n");
}

dumpcooked()
{
	if(dflag){
		Fprint(1, "%20s%dd%11s%dd%10s%dd\n", "", T0, "", T1, "", T2);
		symtraverse(S_REC, dprfn);
	} else
		symtraverse(S_REC, prfn);
}

usage()
{
	fprint(2, "Usage: stats [-id] [-ssys1,sys2,...] [-uusr1,usr2,...]\n");
	exit(1);
}
