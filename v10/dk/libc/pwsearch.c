/*
 * pwsearch(name, uid, pwline)
 *   search for name (if non null) or uid in password.
     return ptr to passwd structure.
     if pwline is non null return whole password line.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

#define PASSWD "/etc/passwd"
#define PWLINE 500
#define CHUNK 64	/* allocation of hint table -- must be a power of two */

static struct hint {
	long	hint;
	char	*name;
	short	uid;
} *htab;

static int htabsize = 0;
char	buf[PWLINE];
static	FILE *pwf;
static	time_t pwtime;

struct passwd *pwdecode();
char *malloc(), *realloc(), *strcpy();

struct passwd *
pwsearch(name, uid, pwline)
char *name, *pwline;
{
	register int min, max, mid;
	register struct passwd *pw;
	int ntries = 0;
	struct stat sbuf;

again:
	if (ntries>1)
		return(NULL);
	if (pwf)
		fstat(fileno(pwf), &sbuf);
	if (pwf==NULL || ntries || pwtime < sbuf.st_ctime) {
		buildtable();
		pwtime = sbuf.st_ctime;
	}
	ntries++;
	if (name) {
		/* binary search for name */
		min = 0;
		max = htabsize - 1;
		while (min <= max) {
			register n;
			mid = (min + max) >> 1;
			n = strcmp (name, htab[mid].name);
			if (n == 0)
				break;
			if (n < 0)
				max = mid - 1;
			else
				min = mid + 1;
		}
		if (min > max)
			goto again;
	} else {
		/* linear search for uid */
		for (mid = 0; mid <htabsize; mid++)
			if (htab[mid].uid == uid)
				break;
		if (mid == htabsize)
			goto again;
	}
	/*
	 * We have a hint.  Seek to the given point in the file.
	 * If it's not the very beginning, ensure it is the start
	 * of a line by peeking at the preceding character.
	 */
	if (htab[mid].hint) {
		fseek(pwf, htab[mid].hint-1, 0);
		if (getc(pwf) != '\n')
			goto again;
	} else
		fseek(pwf, htab[mid].hint, 0);
	if (fgets(buf, PWLINE, pwf) == NULL)
		goto again;
	if (pwline)
		strcpy(pwline, buf);
	pw = pwdecode(buf);
	if (name) {
		if (strcmp(name, pw->pw_name))
			goto again;
	} else if (uid != pw->pw_uid)
		goto again;
	return(pw);
}

/* comparison function for call to qsort */
static int
comp (p, q)
struct hint *p, *q;
{
	return(strcmp (p->name, q->name));
}

static int
buildtable()
{
	long where;
	register int i;

	/* free the old hint table */
	for (i = 0; i < htabsize; i++) {
		if (htab[i].name) {
			(void)free((char *)htab[i].name);
			htab[i].name = NULL;
		}
	}
	if (htab) {
		(void)free((char *)htab);
		htab = NULL;
	}
	htabsize = 0;
	if (pwf)
		fclose(pwf);
	pwf = fopen(PASSWD, "r");
	if (pwf == NULL)
		return;
	while (where = ftell(pwf), fgets(buf, PWLINE, pwf) != NULL) {
		register struct passwd *pw = pwdecode(buf);
		register char *p = malloc(strlen(pw->pw_name) + 1);

		if (p == NULL)
			return;
		/* time to expand the hint table? */
		if ((htabsize & (CHUNK - 1)) == 0) {
			unsigned s = (htabsize + CHUNK) * sizeof(struct hint);
			if (htab)
				htab = (struct hint *) realloc(htab, s);
			else
				htab = (struct hint *) malloc(s);
			if (htab == NULL) {
				htabsize = 0;
				return;
			}
		}
		htab[htabsize].name = strcpy(p, pw->pw_name);
		htab[htabsize].uid = pw->pw_uid;
		htab[htabsize].hint = where;
		htabsize++;
	}
	qsort((char *) htab, htabsize, sizeof (struct hint), comp);
}
