/*
 *	cron - clock daemon
 *
 *	modified from Berkeley version 4.3 (7/5/81):
 *
 *		place crontab in /etc/crontab
 *		new mandatory first field gives uid for command
 */

#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>

#define	LISTS	1024

#define	NAMESIZE 9

#define	EXACT	100
#define	ANY	101
#define	LIST	102
#define	RANGE	103
#define	EOS	104
#define	BOL	105
char	crontab[]	= "/etc/crontab";
time_t	itime;
struct	tm *loct;
struct	tm *localtime();
char	*malloc();
char	*realloc();
int	flag;
char	*list;
unsigned listsize;
struct passwd *getpwnam();

main()
{
	register char *cp;
	char *cmp(), *getid(), *getname();
	time_t filetime = 0;

	if (fork())
		exit(0);
	chdir("/");
	freopen(crontab, "r", stdin);
	freopen("/", "r", stdout);
	freopen("/", "r", stderr);
	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	time(&itime);
	itime -= localtime(&itime)->tm_sec;
	fclose(stdin);

	for (;; itime+=60, slp()) {
		struct stat cstat;

		if (stat(crontab, &cstat) == -1)
			continue;
		if (cstat.st_mtime > filetime) {
			filetime = cstat.st_mtime;
			init();
		}
		loct = localtime(&itime);
		loct->tm_mon++;		 /* 1-12 for month */
		for(cp = list; *cp != EOS;) {
			int uid, gid;
			char *np;

			cp++;	/* skip start of line */
			cp = getid (cp, &uid);
			cp = getid (cp, &gid);
			cp = getname(cp, &np);
			flag = 0;
			cp = cmp(cp, loct->tm_min);
			cp = cmp(cp, loct->tm_hour);
			cp = cmp(cp, loct->tm_mday);
			cp = cmp(cp, loct->tm_mon);
			cp = cmp(cp, loct->tm_wday);
			if(flag == 0)
				ex (cp, uid, gid, np);
			while(*cp++ != 0)
				;
		}
	}
}

/*
 *	getid and putid are somewhat machine-dependent.
 *	putid stores a uid or gid as some number of characters;
 *	getid retrieves the uid or gid.  The number of characters
 *	and number of bits per character might be different
 *	on other machines.  Each function returns a pointer to
 *	the char just past the last one it dealt with.
 */
char *
getid (p, v)
	register char *p;
	int *v;
{
	*v = ((p[0] & 0xff) + (p[1] << 8)) & 0xffff;
	return p + 2;
}

char *
putid (p, v)
	register char *p;
	int v;
{
	p[0] = v & 0xff;
	p[1] = (v >> 8) & 0xff;
	return p + 2;
}

char *
getname(p, vp)
char *p;
char **vp;
{

	*vp = p;
	return (p + NAMESIZE);
}

char *
putname(p, v)
register char *p;
register char *v;
{
	register int i;

	for (i = 0; i < NAMESIZE-1; i++) {
		*p++ = *v;
		if (*v)
			v++;
	}
	*p++ = '\0';
	return (p);
}

char *
cmp(p, v)
char *p;
{
	register char *cp;

	cp = p;
	switch(*cp++) {

	case EXACT:
		if (*cp++ != v)
			flag++;
		return(cp);

	case ANY:
		return(cp);

	case LIST:
		while(*cp != LIST)
			if(*cp++ == v) {
				while(*cp++ != LIST)
					;
				return(cp);
			}
		flag++;
		return(cp+1);

	case RANGE:
		if(*cp > v || cp[1] < v)
			flag++;
		return(cp+2);
	}
	if(cp[-1] != v)
		flag++;
	return(cp);
}

slp()
{
	register i;
	time_t t;

	time(&t);
	i = itime - t;
	if(i < -60 * 60 || i > 60 * 60) {
		itime = t;
		i = 60 - localtime(&itime)->tm_sec;
		itime += i;
	}
	if(i > 0)
		sleep((unsigned) i);
}

/*
 * hack: setlogname wants exactly 8 characters
 * we happen to know that there are at least 8 in name, always,
 * because NAMESIZE > 8
 */
ex (s, uid, gid, name)
	char *s;
	int uid, gid;
	char *name;
{
	int st;

	if(fork()) {
		wait(&st);
		return;
	}
	if(fork())
		exit(0);
	setlogname(name);
/*
	(void)setupshares(uid, (void (*)())0);
	setupgroups(name, gid);
*/
	setgid (gid);
	setuid (uid);
	freopen("/dev/null", "r", stdin);
	execl("/bin/sh", "sh", "-c", s, 0);
	exit(0);
}

init()
{
	register i, c;
	register char *cp;
	register char *ocp;
	register int n;
	char username[NAMESIZE];
	struct passwd *pw;

	freopen(crontab, "r", stdin);
	if (list) {
		list = realloc(list, LISTS);
	} else
		list = malloc(LISTS);
	listsize = LISTS;
	cp = list;

loop:
	if(cp > list+listsize-500) {
		char *olist;
		listsize += LISTS;
		olist = list;
		list = realloc(list, listsize);
		cp = list + (cp - olist);
	}
	ocp = cp;

	/* skip leading white space on the line */
	do	c = getchar();
	while (c == ' ' || c == '\t');

	/* accumulate the user name into "username" */
	n = 0;
	while (c != EOF && c != '\n' && c != ' ' && c != '\t') {
		if (n < sizeof (username) - 1)
			username[n++] = c;
		c = getchar();
	}
	username[n] = '\0';

	/* look up the user name and store it */
	pw = getpwnam (username);
	if (pw == NULL)
		goto ignore;
	*cp++ = BOL;
	cp = putid (cp, pw->pw_uid);
	cp = putid (cp, pw->pw_gid);
	cp = putname(cp, username);

	ungetc (c, stdin);

	/* scan the time fields */
	for(i=0;; i++) {
		do	c = getchar();
		while(c == ' ' || c == '\t');
		if(c == EOF || c == '\n')
			goto ignore;
		if(i == 5)
			break;
		if(c == '*') {
			*cp++ = ANY;
			continue;
		}
		if ((n = number(c)) < 0)
			goto ignore;
		c = getchar();
		if(c == ',')
			goto mlist;
		if(c == '-')
			goto mrange;
		if(c != '\t' && c != ' ')
			goto ignore;
		*cp++ = EXACT;
		*cp++ = n;
		continue;

	mlist:
		*cp++ = LIST;
		*cp++ = n;
		do {
			if ((n = number(getchar())) < 0)
				goto ignore;
			*cp++ = n;
			c = getchar();
		} while (c==',');
		if(c != '\t' && c != ' ')
			goto ignore;
		*cp++ = LIST;
		continue;

	mrange:
		*cp++ = RANGE;
		*cp++ = n;
		if ((n = number(getchar())) < 0)
			goto ignore;
		c = getchar();
		if(c != '\t' && c != ' ')
			goto ignore;
		*cp++ = n;
	}
	while(c != '\n') {
		if(c == EOF)
			goto ignore;
		if(c == '%')
			c = '\n';
		*cp++ = c;
		c = getchar();
	}
	*cp++ = '\n';
	*cp++ = 0;
	goto loop;

ignore:
	cp = ocp;
	while(c != '\n') {
		if(c == EOF) {
			*cp++ = EOS;
			*cp++ = EOS;
			fclose(stdin);
			return;
		}
		c = getchar();
	}
	goto loop;
}

number(c)
register c;
{
	register n = 0;

	while (isdigit(c)) {
		n = n*10 + c - '0';
		c = getchar();
	}
	ungetc(c, stdin);
	if (n>100)
		return(-1);
	return(n);
}
