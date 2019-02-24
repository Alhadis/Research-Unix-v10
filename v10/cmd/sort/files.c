/* Copyright 1990, AT&T Bell Labs */
#include "fsort.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGPIPE 13
#define SIGTERM 15

extern int getpid(void);
extern int access(char*, int);
extern int unlink(char*);
extern int stat(const char*, struct stat *);

FILE *
fileopen(char *name, char *mode)
{
	FILE *f;
	if(strcmp(name,"-") == 0)
		if(strcmp(mode, "r") == 0)
			f = stdin;
		else {
			setbuf(stdout,malloc(BUFSIZ));
			f = stdout;
		}
	else {
		if(strcmp(mode, "w") == 0 &&
		   strcmp(name, oname) == 0 &&
		   overwrite(0))
			setsigs(SIG_IGN);
		f = fopen(name, mode);
	}
	if(f == 0)
		fatal("can't open", name, 0);
	return f;
}

void
fileclose(FILE *f, char *name)
{
	if(fclose(f)==EOF && name!=0)
		fatal("error on", name, 0);
}

/* file name strings accumulate as garbage */

char *
filename(int number)
{
	char name[50];
	char *s;
	int i;
	for(i=0; (s=tname[i])!=0; i++)
		if(access(s, 03) != -1)
			break;
	if(s == 0)
		fatal("no accessible temp directory", "", 0);
	sprintf(name, "%s/stm%.5d.%.4d", s, getpid(), number);
	s = malloc(strlen(name) + 1);
	if(s == 0)
		fatal("out of space", "", 0);
	strcpy(s, name);
	return s;
}

/* if there is enough room in record r, getline puts
   a line of data there and returns 0; otherwise it
   returns a pointer to a new record.  The record
   may be grown in stages; intermediate stages can
   be discarded, but the original cannot. */

static struct rec *
newrec(struct rec *r, struct rec *retval)
{
	int n = (uchar*)r->next - data(r);
	int len = (uchar*)r->next - (uchar*)r;
	struct rec *new = (struct rec*)malloc(len + n);
	if(new == 0)
		fatal("no space for record", "", 0);
	memmove(new, r, len);
	new->next = (struct rec*)((uchar*)new + len + n);
	if(retval)
		free(retval);
	return new;
}

struct rec*
getline(struct rec *r, FILE *f)
{
	int n;
	int c;
	uchar *cp;
	uchar *ep = (uchar*)r->next;
	uchar *dp;
	struct rec *retval = 0;

	if(feof(f))		/* in case newline was appended */
		return ENDFILE;
	for(dp=cp=data(r); ; *cp++ = c) {
		if((c=getc(f)) == '\n') 
			break;
		else if(c == EOF) {
			if(cp == dp)
				return ENDFILE;
			warn("newline appended", "", 0);
			break;
		} else if(cp >= ep) {
			n = cp - dp;
			retval = r = newrec(r, retval);
		 	dp = data(r);
			cp = dp + n;
			ep = (uchar*)r->next;
		}
	}

	r->dlen = n = cp - dp;
	if(n > MAXREC)
		fatal("monster record", "", 0);
	if(!keyed) {
		r->klen = 0;	/* hygiene */
		return retval;
	}
	while((n = fieldcode(data(r),key(r), r->dlen,
		(uchar*)r->next, fields, nfields)) < 0)
		retval = r = newrec(r, retval);	/* rare event */
	if(n > MAXREC)
		fatal("monster key", "", 0);
	r->klen = n;
	return retval;
}

static char *level = "warning";

void
warn(char *m, char *s, int l)
{
	fprintf(stderr, "sort: %s: %s %.*s\n",
		level, m, l==0?strlen(s):l, s);
}

void
fatal(char *m, char *s, int l)
{
	level = "error";
	warn(m, s, l);
	if(errno)
		perror("");
	cleanup(1);
}

int
overwrite(int j)
{
	struct stat sb1, sb2;
	if(strcmp(oname, "-") == 0)
		return 0;
	if(stat(oname, &sb1) == -1)
		return 0;
	for( ; j<nfiles; j++) {
		if(strcmp(files[j], "-") == 0)
			continue;
		if(stat(files[j], &sb2) == -1)
			fatal("cannot locate", files[j], 0);
		if(sb1.st_dev==sb2.st_dev && sb1.st_ino==sb2.st_ino)
			return 1;
	}
	return 0;
}

static int siglist[] = { SIGHUP, SIGINT, SIGQUIT, SIGPIPE, SIGTERM };

void
setsigs(void(*f)(int))
{
	int i;
	for(i=0; i<sizeof(siglist)/sizeof(*siglist); i++)
		if(signal(siglist[i], f) == SIG_IGN)
			signal(siglist[i], SIG_IGN);
}

void
cleanup(int i)
{
	char *name;
	setsigs(SIG_IGN);
	while(--nextfile >= 0) {
		name = filename(nextfile);
		unlink(name);
		free(name);
	}
	exit(i);
}
