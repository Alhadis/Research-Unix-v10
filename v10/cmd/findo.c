#include "sys/param.h"
#include "sys/ino.h"
#include "sys/inode.h"
#include "sys/dir.h"
#include "sys/filsys.h"
#include "stdio.h"
#include "sys/stat.h"
#include <pwd.h>
#include <ctype.h>

#define MONTH	3600*24L*7L*4L
#define	DAY	(3600*24L)
#define	WEEK	3600*24L*7L
#define	DAY3	3600*24L*3L
#define	NUID	20000

int	uid = -2;
int	nflag = 0;

#define biton(x, n)	x[(n)>>3] |= 1 << ((n)&7)
#define isdir(p)	(((p)->di_mode & S_IFMT) == S_IFDIR)
#define isold(p)	(p->di_mtime + WEEK < nowtime)
#define isvold(p)	(p->di_mtime + MONTH < nowtime)
#define	isuser(p)	((p->di_uid>=NUID||isbad[p->di_uid]) && (nflag==0 || p->di_mtime + DAY3 >= nowtime))
#define isnt(p)		!(p->di_mode & S_IFMT)
#define istrue(x, n)	x[(n)>>3] & (1<<((n)&7))

extern int screen(), consider();
char dlist[8192], olist[8192], slist[8192], tlist[8192];
int szlist[65536];
int days[65536];
char curdir[256];
long nowtime;
char	isbad[NUID];
struct stat origstat;

main(argc, argv) char **argv;
{
	int i;

	(void) time(&nowtime);
	isbad[uid] = 1;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-n")==0) {
			nflag++;
			continue;
		}
		if (i < argc-2 && strcmp(argv[i], "-u")==0) {
			isbad[uid] = 0;
			uid = atoi(argv[i+1]);
			isbad[uid] = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i], "-f")==0) {
			getbad();
			continue;
		}
		break;
	}
	if (i>argc-2) {
		fprintf(stderr, "findo /dev/device /place-it-is-mounted\n");
		exit(1);
	}
	finit();
	eachinode(argv[i], screen);
	if (chdir(argv[++i])) {
		perror(argv[i]);
		exit(1);
	}
	strcpy(curdir, argv[i]);
	stat(curdir, &origstat);	/* where we started */
	alldir(0, consider);
	exit(0);
}

finit()
{	int i;
	for (i = 0; i < sizeof(olist); i++)
		olist[i] = 0;
	for (i = 0; i < sizeof(dlist); i++)
		dlist[i] = 0;
	for (i = 0; i < sizeof(slist); i++)
		slist[i] = 0;
	for (i = 0; i<sizeof(tlist); i++)
		tlist[i] = 0;
	for (i = 0; i < sizeof(szlist)/sizeof(szlist[0]); i++)
		szlist[i] = days[i] = 0;
}

screen(n, p) struct dinode *p; unsigned int n;
{
	if(isnt(p)) return;
	szlist[n] = p->di_size;
	days[n] = (nowtime - p->di_mtime) / DAY;
	if(isdir(p)) {
		biton(dlist, n);
	}
	if(isuser(p)) {
		biton(slist, n);
		return;
	}
	if(isitroff(p)) {
		biton(tlist, n);
		return;
	}
	if(isold(p)) {
		biton(olist, n);
		if (isvold(p))
			biton(tlist, n);
		return;
	}
}

newsys()
{
	struct stat dot;

	stat(".", &dot);
	return dot.st_dev != origstat.st_dev;
}

consider(n, p) struct direct *p;
{	int j;
	char buf[DIRSIZ+1];
	if(istrue(dlist, p->d_ino)) {
		for(j = 0; j < DIRSIZ; j++)
			buf[j] = p->d_name[j];
		buf[j] = 0;
		j = strlen(curdir);
		if(strcmp(".", buf) == 0 || strcmp("..", buf) == 0)
			return;	/* of course this can't happen (ha) */
		strcat(curdir, "/");
		strcat(curdir, buf);
		if(chdir(buf) == -1)
			perror(curdir);
		else {
			if(!newsys())
				alldir(n+1, consider);
			if (chdir("..") < 0) {
				fprintf(stderr, "bad '..' in %s\n", curdir);
				curdir[j] = '\0';
				chdir(curdir);
			}
		}
		curdir[j] = 0;
	}
	if (istrue(slist, p->d_ino)) {
		printf("user\t%8d %8d\t", szlist[p->d_ino], days[p->d_ino]);
		file(p->d_name);
	}
	if (istrue(olist, p->d_ino)) {
		if(badname(p->d_name, istrue(tlist, p->d_ino))) {
			printf("old\t%8d %8d\t", szlist[p->d_ino], days[p->d_ino]);
			file(p->d_name);
		}
		return;
	}
	if (istrue(tlist, p->d_ino)) {
		printf("troff\t%8d %8d\t", szlist[p->d_ino], days[p->d_ino]);
		file(p->d_name);
	}
}

badname(s, voflg)
register char *s;
{
	register char *p;
	register int k;

	if(strncmp("core", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("a.out", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("junk", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("ed.hup", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("jim.recover", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("sam.save", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("sam.err", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("~~sam~~", s, DIRSIZ) == 0)
		return(1);
	if(strncmp(".pilog", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("mon.out", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("qed.hup:", s, 8) == 0)
		return(1);
	if(strncmp("x", s, DIRSIZ) == 0)
		return(1);
	if(strncmp("dead.letter", s, DIRSIZ) == 0)
		return(1);
	if (strend(".dvi", s))
		return(1);
	if (strncmp("junk", s, 4)==0 || strncmp("temp", s, 4)==0)
		return 1;
	if (strlen(s) == 1 && islower(s[0]))
		return 1;
	if (strncmp("foo", s, 3)==0 ||
	   (strncmp("rst", s, 3)==0 && strlen(s)>5)) {
		p = s+3;
		while (p < s+DIRSIZ) {
			if (*p == 0)
				return(1);
			if (!isdigit(*p))
				break;
			p++;
		}
	}
	if (strncmp(".jx", s, 3) == 0)
		return(1);
	if (s[5]=='.' && isdigit(s[3]) && isdigit(s[4]) && isdigit(s[7])
	 && isdigit(s[8]) && s[6]=='a')
		return(1);
	if (voflg==0)
		return(0);
	p = s+DIRSIZ;
	if (strend(".o", s))
		return(1);
	return(0);
}

strend(e, s)	/* is e the end of s? */
	register char *e, *s;
{
	register char *t=s;
	while(*t)
		t++;
	t-=strlen(e);
	return(strcmp(e, t)==0);
}

file(s) char *s;
{	int i;
	char buf[DIRSIZ+1];
	for(i = 0; i < DIRSIZ; i++)
		buf[i] = *s++;
	buf[i] = 0;
	printf("%s/%s\n", curdir, buf);
}

isitroff(ip)
register struct dinode *ip;
{
	long dska[13];
	char tbuf[BSIZE(0)];
	if ((ip->di_mode&IFMT) != IFREG)
		return(0);
	if (ip->di_mode&IEXEC)
		return(0);
/*	if (ip->di_size < 512)
/*		return(0);
*/
	if (ip->di_mtime + DAY > nowtime)
		return(0);
/*	if (ip->di_mtime +(200*DAY) < nowtime)
/*		return(0);
*/
	l3tol(dska, ip->di_addr, 13);
	bread(dska[0], tbuf, BSIZE(0));
	if (troffint(tbuf))
		return(1);
	return(0);
}

char *troff[] = {	/* troff intermediate lang */
	"x T ", "x res ", "x init", "V0", "x font ", 0 };
int ntroff[] = {
	4,	 6,	   6,	     2,    7 };

troffint(bp)
register char *bp;
{
	register char **tp;
	register i, j, k = 0, oi, n;

	for (i = oi = n = 0; i < BSIZE(0) && k < 15; i++) {
		if (bp[i] == '\n') {
			k++;
			bp[i] = '\0';
			for (j = 0; troff[j]; j++)
				if (strncmp(bp+oi, troff[j], ntroff[j]) == 0)
					n++;
			oi = i+1;
		}
	}
	return n > 5;
}

word(p1, p2)
register char *p1, *p2;
{
	while (*p1++ == *p2++)
		;
	if (*--p1 != '\0')
		return(0);
	if (*--p2 !=' ' && *p2!='\n')
		return(0);
	return(1);
}

#define	ITABSZ	BUFSIZE/sizeof(long)
#define	ISIZ	(BUFSIZE/sizeof(struct dinode))
static	struct	filsys	sblock;
static	struct	dinode	itab[ITABSZ];
static	fi;
struct stat stb;
int dev;


eachinode(file, func)
char *file;
int (*func)();
{	unsigned ino;
	int nfiles = 0;
	register unsigned i, j;

	fi = open(file, 0);
	if(fi < 0 || fstat(fi, &stb) != 0) {
		fprintf(stderr, "cannot open %s\n", file);
		return;
	}
	dev = stb.st_rdev;
	bread((long)1, (char *)&sblock, sizeof sblock);
	nfiles = (sblock.s_isize-2)*(BSIZE(dev)/sizeof(struct dinode));
	ino = 1;
	for(i=2+((ino-1)/INOPB(dev)); ino<nfiles; i ++) {
		bread((long)i, (char *)itab, BSIZE(dev));
		for (j=0; j<INOPB(dev) && ino<nfiles; j++) {
			(*func)(ino, &itab[j]);
			ino++;
		}
	}
}

bread(bno, buf, cnt)
long bno;
char *buf;
{	int i;

	lseek(fi, bno*BSIZE(dev), 0);
	if (read(fi, buf, cnt) != cnt) {
		printf("read error %u\n", bno);
		for(i=0; i<cnt;i++)
			buf[i] = 0;
	}
}

alldir(n, f) int (*f)();
{	struct direct p;
	FILE *fd;
	fd = fopen(".", "r");
	if(fd == NULL) {
		perror(".");
		return(0);
	}
	(void) fseek(fd, (long)2 * sizeof(p), 0);	/* skip . and .. */
	for(;;) {
		(void) fread((char *)&p, 1, sizeof(p), fd);
		if(feof(fd)) break;
		if(p.d_name[0] != 0 && p.d_ino != 0)
			(*f)(n, &p);
	}
	(void) fclose(fd);
	return(1);
}

getbad()
{
	register struct passwd *p;
	extern struct passwd *getpwent();
	register i;

	for (i=0; i<NUID; i++)
		isbad[i] = 1;
	while (p = getpwent()) {
		if (p->pw_uid>=0 && p->pw_uid<NUID)
			isbad[p->pw_uid] = 0;
	}
}
