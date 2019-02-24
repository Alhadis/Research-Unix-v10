#include <stdio.h>
#include <regexp.h>
#include <signal.h>
#include "mail.h"
#include "string.h"
#include "message.h"
#include "aux.h"
#include <sys/stat.h>

/* imported */
extern char *malloc();
extern long time();
extern fseek();
FILE *lockopen();
FILE *lockreopen();

/* definition of UNIX message headers */

#define MSGALLOC 32

typedef struct {
	message m[MSGALLOC];
	int o;
} msgalloc;
static msgalloc *freep=NULL;

/* read in a message, interpret the 'From' header */
extern message *
m_get(sp)
	string *sp;
{
	message *mp;
	register char *cp;

	if (freep==NULL || freep->o >= MSGALLOC) {
		freep = (msgalloc *)malloc(sizeof(msgalloc));
		if (freep==NULL) {
			perror("allocating message");
			exit(1);
		}
		freep->o = 0;
	}
	mp = &(freep->m[freep->o++]);
	mp->body = NULL;
	mp->sender = s_new();
	mp->date = s_new();
	mp->extent = mp->prev = mp->next = NULL;
	mp->status = 0;
	mp->pos = 0;

	if (sp->ptr >= sp->end) {
		mzero = mp;
		return NULL;
	}

	/* parse From lines */
	for(cp=sp->ptr; *cp=='\n'||*cp==' '||*cp=='\t'; cp++);
	for(sp->ptr=cp; *cp != '\n' && *cp; cp++);
	*cp = '\0';
	if (parse_header(sp->ptr, mp->sender, mp->date)<0) {
		fprintf(stderr, "!mailbox format incorrect\n");
		mzero = mp;
		return NULL;
	}
	*cp++ = '\n';
	sp->ptr = cp;

	/* get body */
	while (cp < sp->end && !IS_HEADER(cp)) {
		while (*cp++ != '\n')
			;
	}
	cp[-1] = '\0';

	mp->size = cp - sp->ptr;
	mp->body = s_array(sp->ptr, mp->size);
	sp->ptr = cp;
	return mp;
}

/* output a message, return 0 if ok -1 otherwise */
extern int
m_print(mp, fp, nl, header)
	message *mp;	/* the message */
	FILE *fp;	/* where to print it */
{
	SIG_TYP pstat;
	int rv = 0;

	pstat = signal(SIGPIPE, SIG_IGN);
	if (header)
		print_header(fp, s_to_c(mp->sender), s_to_c(mp->date));
	fflush(fp);

	/*
	 *  the following are writes instead of an fwrites because,
	 *  in the case of a broken pipe, fwrite will continuously
	 *  generate SIGPIPE's.  Enough SIGPIPE's will terminate the
	 *  process despite the SIG_IGN (at least on v9).
	 */
	if (mp->size > 0)
		if(write(fileno(fp), s_to_c(mp->body), mp->size-1) != mp->size-1)
			rv = -1;
	if (rv==0 && nl)
		if(write(fileno(fp), "\n", 1) != 1)
			rv = -1;

	signal(SIGPIPE, pstat);
	return ferror(fp) ? -1 : rv;
}

/* lists of mail messages */
message *mzero;		/* zeroth message */
message *mlist;		/* first mail message */
message *mlast;		/* last mail message */

long mbsize;		/* last size of mail box */


/* 
 *  read in the mail file.  mbsize is the spot to start reading from.
 *  if fpp is non-zero then don't close the locked fmailbox, just
 *  return the file pointer to it.
 */
static FILE *
rd_mbox(file, reverse, newmail, fp)
	char *file;
	int reverse;
	FILE *fp;
{
	message *mp;
	int pos = 0;
	struct stat sbuf;
	string *line;
	char *corefile;
	int len;

	if(fp==NULL)
		fp = lockopen(file, "r", 0, -1, -1);
	if(fp==NULL)
		return NULL;
	if (stat(file, &sbuf) < 0){
		lockclose(fp);
		return NULL;
	}
	if (sbuf.st_size <= mbsize)
		return fp;
	fseek(fp, mbsize, 0);
	len = sbuf.st_size-mbsize;
	corefile = malloc(len+1);
	if (corefile == NULL || fread(corefile, len, 1, fp)!=1) {
		perror("reading mbox");
		exit(1);
	}
	corefile[len-1] = '\n';		/* ensure last mbox char is \n */
	corefile[len] = '\0';		/* ensure null termination */
	line = s_array(corefile, len);
	if (mbsize==0) {
		switch(delivery_status(s_restart(line))) {
		case MF_PIPE:
			printf("mail: mail is being piped to %s\n", line->ptr);
			lockclose(fp);
			return NULL;
		case MF_FORWARD:
			printf("mail: mail is being forwarded to %s\n", line->ptr);
			lockclose(fp);
			return NULL;
		}
	}
	s_restart(line);
	while((mp = m_get(line)) != NULL) {
		if (mlist == NULL)
			mlist = mlast = mp;
		else if (reverse) {
			mlast->next = mp;
			mp->prev = mlast;
			mlast = mp;
		} else {
			mp->next = mlist;
			mlist->prev = mp;
			mlist = mp;
		}
	}
	if (mlist==NULL)
		return fp;
	mzero->next = mlist;
	mzero->prev = mlast;
	for (mp=mzero; mp!=NULL; mp=mp->next)
		mp->pos=pos++;
	mbsize = sbuf.st_size;
	if (newmail)
		printf("mail: new message arrived\n");
	return fp;
}

/* read the mailbox */
extern int
read_mbox(file, reverse)
	char *file;
	int reverse;
{
	FILE *fp;
	SIG_TYP fhup, fint, fquit;

	fhup = signal(SIGHUP, SIG_IGN);
	fint = signal(SIGINT, SIG_IGN);
	fquit = signal(SIGQUIT, SIG_IGN);
	fp = rd_mbox(file, reverse, 0, NULL);
	if(fp!=NULL)
		lockclose(fp);
	signal(SIGHUP, fhup);
	signal(SIGINT, fint);
	signal(SIGQUIT, fquit);
	return fp==NULL ? -1 : 0;
}

/* read the mailbox looking for new messages */
extern int
reread_mbox(file, reverse)
	char *file;
	int reverse;
{
	FILE *fp;
	SIG_TYP fhup, fint, fquit;

	fhup = signal(SIGHUP, SIG_IGN);
	fint = signal(SIGINT, SIG_IGN);
	fquit = signal(SIGQUIT, SIG_IGN);
	fp = rd_mbox(file, reverse, 1, NULL);
	if(fp!=NULL)
		lockclose(fp);
	signal(SIGHUP, fhup);
	signal(SIGINT, fint);
	signal(SIGQUIT, fquit);
	return fp==NULL ? -1 : 0;
}

/* read any changes from the mailbox and write out the result */
rdwr_mbox(file, reverse)
	char *file;
{
	int rv, err;
	FILE *fp;
	message *mp;

	/*
	 *  if nothing has changed, just return
	 */
	for (mp=mlist; mp!=NULL; mp=mp->next)
		if (mp->status&DELETED)
			break;
	if (mp==NULL)
		return 0;

	/*
	 *  read mailbox to pick up any changes
	 */
	fp = NULL;
	if((fp=rd_mbox(file, reverse, 1, NULL)) == NULL){
		fprintf(stderr, "mail: can't write mail file %s\n", file);
		return 1;
	}

	/*
	 *  now rewrite it.  use the fp passed back from read_mbox to
	 *  maintain the lock across both operations.
	 */
	if(fp==NULL)
		fp = lockopen(file, "w", 0, -1, -1);
	else
		fp = lockreopen(file, "w", fp);
	if (fp == NULL) {
		fprintf(stderr, "mail: can't write mail file %s\n", file);
		return 1;
	} else {
		rv = 0;
		mlist->prev = NULL;	/* ignore mzero */
		err = 0;
		for(mp=reverse?mlist:mlast; mp!=NULL; mp=reverse?mp->next:mp->prev)
			if ((mp->status&DELETED)==0)
				err += m_print(mp, fp, 1, 1);
		if (err < 0) {
			fprintf(stderr,"mail: error writing mail file %s\n", file);
			rv = 1;
		}
		lockclose(fp);
	}
	return rv;
}

/* write out the mail file */
extern int
write_mbox(file, reverse)
	char *file;
	int reverse;
{
	SIG_TYP fhup, fint, fquit;
	int rv;

	fhup = signal(SIGHUP, SIG_IGN);
	fint = signal(SIGINT, SIG_IGN);
	fquit = signal(SIGQUIT, SIG_IGN);
	rv = rdwr_mbox(file, reverse);
	signal(SIGHUP, fhup);
	signal(SIGINT, fint);
	signal(SIGQUIT, fquit);
	return rv;
}

/* imported */
extern char *getenv();
extern int getpid();

/* global to semaphores */
static char semaphore[128];

extern void
V()
{
	unlink(semaphore);
}

/* return name of tty if file is already being read, NULL otherwise */
extern int
P()
{
	char file[128];
	struct stat sbuf;
	FILE *fp;
	int pid;
	char *home = getenv("HOME");

	if (home == NULL)
		return 0;
	(void)strcpy(file, home);
	(void)strcat(file, "/.Maillock");
	if (stat(file, &sbuf) >= 0) {

		/* lock file exists */
		fp = fopen(file, "r");
		if (fp != NULL) {
			fscanf(fp, "%d", &pid);
			fclose(fp);
		}
		if (fp == NULL || kill(pid, 0)==0) {
			fprintf(stderr,"WARNING: You are already reading mail.\n");
			fprintf(stderr, "\tThis instance of mail is read only.\n");
			return -1;
		}
	}

	/* create a semaphore file */
	strcpy(semaphore, file);
	V();
	fp = fopen(semaphore, "w");
	if (fp == NULL)
		return 0;		/* nothing else we can do */
	fprintf(fp, "%d somewhere", getpid());
	fclose(fp);
	return 0;
}
