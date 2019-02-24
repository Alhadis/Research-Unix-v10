#include <stdio.h>
#include <regexp.h>
#include <signal.h>
#include <pwd.h>
#include "mail.h"
#include "string.h"
#include "message.h"
#include "aux.h"

/* imported */
extern char *malloc();
extern long time();
extern char *ctime();
extern char *getlog();
extern void exit();
extern char *thedate();

/* global to this file */
static regexp *rfprog;
static regexp *fprog;
static pipe_err=0;

#define VMLIMIT (64*1024)
#define MSGLIMIT (5*1024*1024)

extern void
default_from(mp)
	message *mp;
{
	char *logname;
	struct passwd *pw;
	extern char *getenv();

	/* add in date and sender */
	if ((logname=getenv("upasname")) == NULL) {
		if ((logname=getlog()) == NULL)
			logname = "Liz.Bimmler";
	}
	s_append(mp->sender, logname);
	s_append(mp->date, thedate());
}

extern message *
m_new()
{
	message *mp;

	mp = (message *)malloc(sizeof(message));
	if (mp == NULL) {
		perror("message:");
		exit(1);
	}
	mp->sender = s_new();
	mp->replyaddr = s_new();
	mp->date = s_new();
	mp->body = s_new();
	mp->size = 0;
	mp->fd = -1;
	return mp;
}

extern int
m_free(mp)
	message *mp;
{
	if(mp->fd >= 0)
		close(mp->fd);
	s_free(mp->sender);
	s_free(mp->date);
	s_free(mp->body);
	free((char *)mp);
}

/* read a message into a temp file , return an open fd to it */
static int
m_read_to_file(fp, mp)
	FILE *fp;
	message *mp;
{
	int cfd, fd;
	int n;
	char buf[4*1024];

	/*
	 *  create and unlink temp file
	 */
	strcpy(buf, MAILROOT);
	strcat(buf, "mtXXXXXX");
	mktemp(buf);
	if((cfd = creat(buf, 0600))<0)
		return -1;
	if((fd = open(buf, 2))<0){
		unlink(buf);
		close(cfd);
		return -1;
	}
	close(cfd);
	unlink(buf);

	/*
	 *  read the rest into the temp file
	 */
	while((n = fread(buf, 1, sizeof buf, fp)) > 0){
		if(write(fd, buf, n) != n){
			close(fd);
			return -1;
		}
		mp->size += n;
		if(mp->size > MSGLIMIT){
			mp->size = -1;
			break;
		}
	}

	mp->fd = fd;
	return 0;
}

/* read in a message, interpret the 'From' header */
extern message *
m_read(fp, rmail, onatty)
	FILE *fp;
	int rmail;	/* true if invoked as /bin/rmail */
	int onatty;	/* true if input from a terminal */
{
	message *mp;
	regsubexp subexp[10];
	int first;

	mp = m_new();

	/* parse From lines */
	if (rmail) {
		/* get remote address */
		string *sender=s_new();

		if (rfprog == NULL)
			rfprog = regcomp(REMFROMRE);
		first = 1;
		while(s_read_line(fp, s_restart(mp->body)) != NULL) {
			if (regexec(rfprog, s_to_c(mp->body), subexp, 10) == 0){
				if(first == 0)
					break;
				if (fprog == NULL)
					fprog = regcomp(FROMRE);
				if(regexec(fprog, s_to_c(mp->body), subexp,10) == 0)
					break;
				s_restart(mp->body);
				append_match(subexp, s_restart(sender), SENDERMATCH);
				append_match(subexp, s_restart(mp->date), DATEMATCH);
				break;
			}
			append_match(subexp, s_restart(sender), REMSENDERMATCH);
			append_match(subexp, s_restart(mp->date), REMDATEMATCH);
			if(subexp[REMSYSMATCH].sp!=subexp[REMSYSMATCH].ep){
				append_match(subexp, mp->sender, REMSYSMATCH);
				s_append(mp->sender, "!");
			}
			first = 0;
		}
		s_append(mp->sender, s_to_c(sender));
		s_free(sender);
	}
	if (*s_to_c(mp->sender)=='\0')
		default_from(mp);

	/* get body */
	if(onatty){
		/* user typing on terminal: terminator == '.' or EOF */
		for(;;) {
			char *line;

			if ((line=s_read_line(fp, mp->body))==NULL)
				break;
			if (strcmp(".\n", line)==0) {
				*line = '\0';
				mp->body->ptr = line;
				break;
			}
		}
		mp->size = mp->body->ptr - mp->body->base;
	} else {
		/*
		 *  read up to VMLIMIT bytes (more or less).
		 *  if message is longer use temp file
		 */
		mp->size = s_read_to_lim(fp, mp->body, VMLIMIT);
		if(mp->size < 0){
			mp->size = -mp->size;
			if(m_read_to_file(fp, mp) < 0){
				perror("m_read");
				exit(1);
			}
		}
	}

	/*
	 *  ignore 0 length messages from a terminal
	 */
	if (!rmail && *s_to_c(mp->body) == '\0')
		return NULL;

	return mp;
}

SIGRETURN
sigpipe(s)
	int s;
{
	signal(SIGPIPE, sigpipe);
	pipe_err = -1;
}

/* return a piece of message starting at `offset' */
extern int
m_get(mp, offset, pp)
	message *mp;
	long offset;
	char **pp;
{
	static char buf[4*1024];

	/*
	 *  are we past eof?
	 */
	if(offset >= mp->size)
		return 0;

	/*
	 *  are we in the virtual memory portion?
	 */
	if(offset < mp->body->ptr - mp->body->base){
		*pp = mp->body->base + offset;
		return mp->body->ptr - mp->body->base - offset;
	}

	/*
	 *  read it from the temp file
	 */
	offset -= mp->body->ptr - mp->body->base;
	if(mp->fd < 0)
		return -1;
	if(lseek(mp->fd, offset, 0)<0)
		return -1;
	*pp = buf;
	return read(mp->fd, buf, sizeof buf);
}

/* output the message body without ^From escapes */
static int
m_noescape(mp, fp)
	message *mp;	/* the message */
	FILE *fp;	/* where to print it */
{
	long offset;
	int n;
	char *p;

	for(offset = 0; offset < mp->size; offset += n){
		n = m_get(mp, offset, &p);
		if(n < 0){
			fflush(fp);
			return -1;
		}
		fwrite(p, n, 1, fp);
	}
	fflush(fp);
	return 0;
}

/*
 *  output the message body with ^From escapes.  The state machine
 *  ensures that any line starting with a 'From ' gets a '>' stuck
 *  in front of it.
 */
static int
m_escape(mp, fp)
	message *mp;	/* the message */
	FILE *fp;	/* where to print it */
{
	register char *p;
	register char *end;
	register int state;
	long offset;
	int n;
	char *start;

	state = 1;
	for(offset = 0; offset < mp->size; offset += n){
		n = m_get(mp, offset, &start);
		if(n < 0){
			fflush(fp);
			return -1;
		}

		p = start;
		for(end = p+n; p < end; p++){
			switch(state){
			case 1:
				if(*p == 'F'){
					state = 2;
					continue;
				}
				state = 0;
				break;
			case 2:
				if(*p == 'r'){
					state = 3;
					continue;
				}
				state = 0;
				fputc('F', fp);
				break;
			case 3:
				if(*p == 'o'){
					state = 4;
					continue;
				}
				state = 0;
				fputc('F', fp);
				fputc('r', fp);
				break;
			case 4:
				if(*p == 'm'){
					state = 5;
					continue;
				}
				state = 0;
				fputc('F', fp);
				fputc('r', fp);
				fputc('o', fp);
				break;
			case 5:
				if(*p == ' ')
					fputc('>', fp);
				state = 0;
				fputc('F', fp);
				fputc('r', fp);
				fputc('o', fp);
				fputc('m', fp);
				break;
			}
			fputc(*p, fp);
			if(*p == '\n')
				state = 1;
		}
	}
	fflush(fp);
	return 0;
}

/* output a message */
extern int
m_print(mp, fp, remote, mbox)
	message *mp;	/* the message */
	FILE *fp;	/* where to print it */
	char *remote;	/* 'remote from' string */
{
	SIG_TYP oldsig;
	int err;

	pipe_err = 0;
	oldsig = signal(SIGPIPE, sigpipe);
	if (remote != NULL)
		print_remote_header(fp,s_to_c(mp->sender),s_to_c(mp->date),remote);
	else
		print_header(fp, s_to_c(mp->sender), s_to_c(mp->date));

	if (!mbox)
		err = m_noescape(mp, fp);
	else
		err = m_escape(mp, fp);
	signal(SIGPIPE, oldsig);
	if(ferror(fp))
		err = -1;
	return pipe_err|err;
}

/* print just the message body */
extern int
m_bprint(mp, fp)
	message *mp;	/* the message */
	FILE *fp;	/* where to print it */
{
	SIG_TYP oldsig;
	int err;

	pipe_err = 0;
	oldsig = signal(SIGPIPE, sigpipe);
	err = m_noescape(mp, fp);
	signal(SIGPIPE, oldsig);
	if(ferror(fp))
		err = -1;
	return pipe_err|err;
}
