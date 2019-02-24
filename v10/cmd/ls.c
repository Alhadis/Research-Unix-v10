/*
 * ls
 */

#include <sys/param.h>
#include <sys/stat.h>
#include <ndir.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

#define NUID	500
#define NGID	50
#define	NMAX	8

struct id {
	int id;
	char name[NMAX+1];
};

struct id *names, *lastname;
struct id *groups, *lastgroup;

#define	kbytes(size)	((size + 1023) / 1024)

struct afile {
	char	ftype;		/* file type, e.g. 'd', 'c', 'f' */
	char	flink;		/* 'L' for symlinks, ' ' otherwise */
	ino_t	fnum;		/* inode number of file */
	short	fflags;		/* mode&~S_IFMT, perhaps ISARG */
	short	fnl;		/* number of links */
	short	fuid;		/* owner id */
	short	fgid;		/* group id */
	long	fsize;		/* file size */
	time_t	fmtime;		/* time (modify or access or create) */
	char	*fname;		/* file name */
	char	*flinkto;	/* symbolic link value */
};

#define ISARG	0x8000		/* extra ``mode'' */

struct subdirs {
	char	*sd_name;
	struct	subdirs *sd_next;
} *subdirs;

int	aflg, dflg, lflg, sflg, tflg, uflg, iflg, fflg, cflg, rflg = 1;
int	Fflg, Lflg, Rflg;

time_t	now, sixmonthsago;

char	*dotp = ".";

struct	afile *gstat();
int	fcmp();
char	*cat(), *savestr();
char	*fmtentry();
char	*getname(), *getgroup();

char	*ctime();
char	*malloc(), *realloc();
char	*strcpy(), *strcat();

main(argc, argv)
	int argc;
	char *argv[];
{
	int i;
	struct afile *fp0, *fplast;
	register struct afile *fp;
	char ebuf[BUFSIZ];
	char obuf[BUFSIZ];

	setbuf(stderr, ebuf);
	setbuf(stdout, obuf);
	argc--, argv++;
	(void) time(&now); sixmonthsago = now - 6L*30L*24L*60L*60L; now += 60;
	while (argc > 0 && **argv == '-') {
		(*argv)++;
		lsacct(*argv);
		while (**argv) switch (*(*argv)++) {

		case 'a':
			aflg++; break;
		case 'c':
			cflg++; break;
		case 's':
			sflg++; break;
		case 'd':
			dflg++; break;
		case 'l':
			lflg++; break;
		case 'r':
			rflg = -1; break;
		case 't':
			tflg++; break;
		case 'u':
			uflg++; break;
		case 'i':
			iflg++; break;
		case 'f':
			fflg++; break;
		case 'L':
			Lflg++; break;
		case 'F':
		        Fflg++; break;
		case 'R':
		        Rflg++; break;
		}
		argc--, argv++;
	}
	if (fflg) { 
		aflg++; lflg = 0; sflg = 0; tflg = 0;
	}
	if (argc == 0) {
		argc++;
		argv = &dotp;
	}
	fp = (struct afile *)malloc(argc * sizeof (struct afile));
	if (fp == 0) {
		dprintf("ls: out of memory\n");
		exit(1);
	}
	fp0 = fp;
	for (i = 0; i < argc; i++) {
		if (gstat(fp, *argv, 1)) {
			fp->fname = *argv;
			fp->fflags |= ISARG;
			fp++;
		}
		argv++;
	}
	fplast = fp;
	qsort((char *)fp0, fplast - fp0, sizeof (struct afile), fcmp);
	for (fp = fp0; fp < fplast; fp++) {
		if ((fflg == 0) && (dflg || fp->ftype != 'd')) {
			printf("%s\n", fmtentry(fp));
			continue;
		}
		formatd(fp->fname, argc > 1);
		while (subdirs) {
			struct subdirs *t;

			t = subdirs;
			subdirs = t->sd_next;
			formatd(t->sd_name, 1);
			cfree(t->sd_name);
			cfree((char *)t);
		}
	}
	exit(0);
}

formatd(name, title)
	char *name;
	int title;
{
	register struct afile *fp;
	register struct subdirs *dp;
	struct afile *dfp0, *dfplast;

	getdir(name, &dfp0, &dfplast);
	if (dfp0 == 0)
		return;
	if (fflg == 0)
		qsort((char *)dfp0, dfplast - dfp0, sizeof (struct afile), fcmp);
	if (title)
		printf("%s:\n", name);
	for (fp = dfp0; fp < dfplast; fp++)
		printf("%s\n", fmtentry(fp));
	if (Rflg)
		for (fp = dfplast-1; fp >= dfp0; fp--) {
			if (fp->ftype != 'd' ||
			    (fp->flink=='L' && (fp->fflags&ISARG)==0) ||
			    !strcmp(fp->fname, ".") ||
			    !strcmp(fp->fname, ".."))
				continue;
			dp = (struct subdirs *)malloc(sizeof (struct subdirs));
			dp->sd_name = savestr(cat(name, fp->fname));
			dp->sd_next = subdirs; subdirs = dp;
		}
	for (fp = dfp0; fp < dfplast; fp++) {
		if ((fp->fflags&ISARG) == 0 && fp->fname)
			free(fp->fname);
		if (fp->flinkto)
			free(fp->flinkto);
	}
	free((char *)dfp0);
}

getdir(dir, pfp0, pfplast)
	char *dir;
	struct afile **pfp0, **pfplast;
{
	register struct afile *fp;
	DIR *dirp;
	register struct direct *dp;
	int nent = 20;

	dirp = opendir(dir);
	if (dirp == NULL) {
		*pfp0 = *pfplast = NULL;
		printf("%s unreadable\n", dir);		/* not stderr! */
		return;
	}
	fp = *pfp0 = (struct afile *)malloc(nent * sizeof (struct afile));
	*pfplast = *pfp0 + nent;
	while (dp = readdir(dirp)) {
		if (dp->d_ino == 0)
			continue;
		if (aflg == 0 && dp->d_name[0]=='.' &&
		   (dp->d_name[1]==0 || dp->d_name[1]=='.' && dp->d_name[2]==0))
			continue;
		if (gstat(fp, cat(dir, dp->d_name), 0) == 0)
			continue;
		fp->fnum = dp->d_ino;
		fp->fname = savestr(dp->d_name);
		fp++;
		if (fp == *pfplast) {
			*pfp0 = (struct afile *)realloc((char *)*pfp0,
			    2 * nent * sizeof (struct afile));
			if (*pfp0 == 0) {
				dprintf("ls: out of memory\n");
				exit(1);
			}
			fp = *pfp0 + nent;
			*pfplast = fp + nent;
			nent *= 2;
		}
	}
	closedir(dirp);
	*pfplast = fp;
}

int	stat(), lstat();

struct afile *
gstat(fp, file, statarg)
	register struct afile *fp;
	char *file;
	int statarg;
{
	char buf[BUFSIZ]; int cc;
	static struct afile azerofile = { 0, ' ' };
	static struct afile azerolink = { 0, 'L' };

	*fp = azerofile;
	fp->fflags = 0;
	fp->fnum = 0;
	fp->ftype = '-';
	if (statarg || sflg || lflg || tflg || Fflg || Rflg) {
		struct stat stb;

		if (lstat(file, &stb) < 0) {
			if (statarg || errno!=ENOENT) {
				dprintf("ls: ");
				perror(file);
				return (0);
			}
			return (&azerofile);
		}
		fp->flink = ' ';
		if (Lflg==0 && (stb.st_mode&S_IFMT)==S_IFLNK) {
			fp->flink = 'L';
			if (stat(file, &stb) < 0)
				return (&azerolink);
		}
		fp->fsize = stb.st_size;
		switch (stb.st_mode & S_IFMT) {

		case S_IFDIR:
			fp->ftype = 'd'; break;
		case S_IFBLK:
			fp->ftype = 'b'; fp->fsize = stb.st_rdev; break;
		case S_IFCHR:
			fp->ftype = 'c'; fp->fsize = stb.st_rdev; break;
		case S_IFLNK:
			fp->ftype = 'l';
			if (lflg) {
				cc = readlink(file, buf, BUFSIZ);
				if (cc >= 0) {
					buf[cc] = 0;
					fp->flinkto = savestr(buf);
				}
			}
			break;
		}
		fp->fnum = stb.st_ino;
		fp->fflags = stb.st_mode & ~S_IFMT;
		fp->fnl = stb.st_nlink;
		fp->fuid = stb.st_uid;
		fp->fgid = stb.st_gid;
		if (uflg)
			fp->fmtime = stb.st_atime;
		else if (cflg)
			fp->fmtime = stb.st_ctime;
		else
			fp->fmtime = stb.st_mtime;
	}
	return (fp);
}

fcmp(cf1, cf2)
char *cf1, *cf2;
{
	register struct afile *f1, *f2;

	f1 = (struct afile *)cf1;
	f2 = (struct afile *)cf2;
	if (dflg == 0 && fflg == 0) {
		if ((f1->fflags&ISARG) && f1->ftype == 'd') {
			if ((f2->fflags&ISARG) == 0 || f2->ftype != 'd')
				return (1);
		} else {
			if ((f2->fflags&ISARG) && f2->ftype == 'd')
				return (-1);
		}
	}
	if (tflg) {
		if (f2->fmtime == f1->fmtime)
			return (0);
		if (f2->fmtime > f1->fmtime)
			return (rflg);
		return (-rflg);
	}
	return (rflg * strcmp(f1->fname, f2->fname));
}

char *
cat(dir, file)
	char *dir, *file;
{
	static char dfile[BUFSIZ];

	if (strlen(dir)+1+strlen(file)+1 > BUFSIZ) {
		dprintf("ls: filename too long\n");
		exit(1);
	}
	if (!strcmp(dir, "") || !strcmp(dir, ".")) {
		(void) strcpy(dfile, file);
		return (dfile);
	}
	(void) strcpy(dfile, dir);
	if (dir[strlen(dir) - 1] != '/' && *file != '/')
		(void) strcat(dfile, "/");
	(void) strcat(dfile, file);
	return (dfile);
}

char *
savestr(str)
	char *str;
{
	char *cp = malloc(strlen(str) + 1);

	if (cp == NULL) {
		dprintf("ls: out of memory\n");
		exit(1);
	}
	(void) strcpy(cp, str);
	return (cp);
}

char	*fmtinum(), *fmtsize(), *fmtlstuff(), *fmtmode();

dprintf(a)
	char *a;
{
	fprintf(stderr, a);
	fflush(stderr);
}

char *
fmtentry(fp)
	register struct afile *fp;
{
	static char fmtres[BUFSIZ];
	register char *cp, *dp;

	(void) sprintf(fmtres, "%s%s%s",
	    iflg ? fmtinum(fp) : "",
	    sflg ? fmtsize(fp) : "",
	    lflg ? fmtlstuff(fp) : "");
	dp = &fmtres[strlen(fmtres)];
	for (cp = fp->fname; *cp; cp++)
		*dp++ = *cp;
	if (Fflg) {
		if (fp->ftype == 'd')
			*dp++ = '/';
		else if (fp->ftype == 'l')
			*dp++ = '@';
		else if (fp->fflags & 0111)
			*dp++ = '*';
	}
	if (lflg && fp->flinkto) {
		(void) strcpy(dp, " -> "); dp += 4;
		for (cp = fp->flinkto; *cp; cp++)
			*dp++ = *cp;
	}
	*dp = 0;
	return (fmtres);
}

char *
fmtinum(p)
	register struct afile *p;
{
	static char inumbuf[8];

	(void) sprintf(inumbuf, "%5d ", p->fnum);
	return (inumbuf);
}

char *
fmtsize(p)
	register struct afile *p;
{
	static char sizebuf[32];

	switch (p->ftype) {

	case 'b':
	case 'c':
		(void) sprintf(sizebuf, "%4ld ", 0);
		break;

	default:
		(void) sprintf(sizebuf, "%4ld ", kbytes(p->fsize));
		break;
	}
	return (sizebuf);
}

char *
fmtlstuff(p)
	register struct afile *p;
{
	static char lstuffbuf[256];
	char gname[32], uname[32], fsize[32], ftime[32];
	register char *lp = lstuffbuf;
	extern char *ngs();

	/* type mode uname gname fsize ftime */
/* get uname */
	{ char *cp = getname(p->fuid);
	  if (cp)
		(void) sprintf(uname, "%.*s", NMAX, cp);
	  else
		(void) sprintf(uname, "%d", p->fuid);
	}
/* get gname */
	{
	  char *cp = getgroup(p->fgid);
	  if (cp)
		(void) sprintf(gname, "%.*s", NMAX, cp);
	  else
		(void) sprintf(gname, "%d", p->fgid);
	}
/* get fsize */
	if (p->ftype == 'b' || p->ftype == 'c')
		(void) sprintf(fsize, "%d,%3d",
		    major(p->fsize), minor(p->fsize));
	else
		(void) sprintf(fsize, "%ld", p->fsize);
/* get ftime */
	{ char *cp = ctime(&p->fmtime);
	  if ((p->fmtime < sixmonthsago) || (p->fmtime > now))
		(void) sprintf(ftime, " %-7.7s %-4.4s ", cp+4, cp+20);
	  else
		(void) sprintf(ftime, " %-12.12s ", cp+4);
	}
/* splat */
	*lp++ = p->ftype;
	lp = fmtmode(lp, p->fflags);
	(void) sprintf(lp, "%2d%c %s%s",
	    p->fnl, p->flink, ngs(uname, gname, fsize), ftime);
	return (lstuffbuf);
}

/* format username, group, size in small space */
char *
ngs(name, group, size)
char *name, *group, *size;
{
	static char r[64];
	register s1, s2;
	register sn=strlen(name), sg=strlen(group), ss=strlen(size);

	s1 = 8-sn;	
	s2 = 12-(sg+ss);
	if (s2<1) {
		s1 -= 1-s2;
		s2 = 1;
	}
	if (s1<1) {
		s2 -= 1-s1;
		s1 = 1;
	}
	if (s2<1)
		s2 = 1;
	sprintf(r, "%s%*s%s%*s%s", name, s1, "", group, s2, "", size);
	return(r);
}


int	m1[] = { 1, S_IREAD>>0, 'r', '-' };
int	m2[] = { 1, S_IWRITE>>0, 'w', '-' };
int	m3[] = { 2, S_ISUID, 's', S_IEXEC>>0, 'x', '-' };
int	m4[] = { 1, S_IREAD>>3, 'r', '-' };
int	m5[] = { 1, S_IWRITE>>3, 'w', '-' };
int	m6[] = { 2, S_ISGID, 's', S_IEXEC>>3, 'x', '-' };
int	m7[] = { 1, S_IREAD>>6, 'r', '-' };
int	m8[] = { 1, S_IWRITE>>6, 'w', '-' };
int	m9[] = { 1, S_IEXEC>>6, 'x', '-' };

int	*m[] = { m1, m2, m3, m4, m5, m6, m7, m8, m9};

char *
fmtmode(lp, flags)
	char *lp;
	int flags;
{
	int **mp;
	char cc;

	if ((flags&S_ICCTYP)==S_ISYNC) {
		flags &= ~S_ICCTYP;
		cc = 'y';
	} else if ((flags&S_ICCTYP)==S_IEXCL) {
		flags &= ~S_ICCTYP;
		cc = 'e';
	}
#	ifdef S_IAPPEND		/* for secure unix */
	else if ((flags&S_ICCTYP)==S_IAPPEND) {
		flags &= ~S_ICCTYP;
		cc = 'a';
	}
#	endif
#	ifdef S_IBLIND		/* for secure unix */
	else if ((flags&S_ICCTYP)==S_IBLIND) {
		flags &= ~S_ICCTYP;
		cc = 'b';
	}
#	endif
	else
		cc = ' ';

	for (mp = &m[0]; mp < &m[sizeof(m)/sizeof(m[0])]; ) {
		register int *pairp = *mp++;
		register int n = *pairp++;

		while (--n >= 0 && (flags&*pairp++) == 0)
			pairp++;
		*lp++ = *pairp;
	}
	*lp++ = cc;
	return (lp);
}

/* rest should be done with nameserver or database */

char *
getname(uid)
{
	register struct id *ip;
	static struct id *lastip;

	if (names == NULL)
		initnames();
	if (names == NULL)
		return (NULL);
	if (lastip != NULL && lastip->id == uid)
		return (lastip->name);
	for (ip = names; ip < lastname; ip++)
		if (ip->id == uid) {
			lastip = ip;
			return (ip->name);
		}
	return (NULL);
}

initnames()
{
	register struct passwd *pw;
	register struct id *ip;
	register int nnames;
	struct passwd *getpwent();
	char *malloc(), *realloc();

	if ((names = (struct id *)malloc(sizeof(*names) * NUID)) == NULL)
		return;
	nnames = NUID;
	ip = names;
	setpwent();
	while (pw = getpwent()) {
		if (ip >= names + nnames) {
			if ((names = (struct id *)realloc((char *)names,
				sizeof(*names) * (NUID+nnames))) == NULL)
				return;
			ip = names + nnames;
			nnames += NUID;
		}
		ip->id = pw->pw_uid;
		strncpy(ip->name, pw->pw_name, NMAX);
		ip++;
	}
	lastname = ip;
	endpwent();
}

char *
getgroup(gid)
{
	register struct id *ip;
	static struct id *lastip;

	if (groups == NULL)
		initgroups();
	if (groups == NULL)
		return (NULL);
	if (lastip != NULL && lastip->id == gid)
		return (lastip->name);
	for (ip = groups; ip < lastgroup; ip++)
		if (ip->id == gid) {
			lastip = ip;
			return (ip->name);
		}
	return (NULL);
}

initgroups()
{
	register struct group *gr;
	register struct id *ip;
	register int ngroups;
	struct group *getgrent();
	char *malloc(), *realloc();

	if ((groups = (struct id *)malloc(sizeof(*groups) * NGID)) == NULL)
		return;
	ngroups = NGID;
	ip = groups;
	setgrent();
	while (gr = getgrent()) {
		if (ip >= groups + ngroups) {
			if ((groups = (struct id *)realloc((char *)groups,
				sizeof(*groups) * (NGID+ngroups))) == NULL)
				return;
			ip = groups + ngroups;
			ngroups += NGID;
		}
		ip->id = gr->gr_gid;
		strncpy(ip->name, gr->gr_name, NMAX);
		ip++;
	}
	lastgroup = ip;
	endgrent();
}

/*
 * temp hack
 */

lsacct(s)
char *s;
{
	int f;

	if ((f = open("/tmp/lsacct", 1)) < 0)
		return;
	write(f, s, strlen(s));
	write(f, "\n", 1);
}
