#include <stdio.h>
#include <regexp.h>
#include <signal.h>
#include "mail.h"
#include "string.h"
#include "message.h"
#include "dest.h"
#include "aux.h"
#include "process.h"

/* globals to all files */
int rmail = 0;
int onatty = 0;
char *thissys;
int nflg = 0;
int debug = 0;

/* global to this file */
static string *errstr;
static message *mp;
static int interrupt;
static char stderrbuf[BUFSIZ];
static int savemail;

/* imports (other than in .h's) */
extern int nsysfile;
extern void exit();
extern FILE* lockopen();

/* interrupt handling */
SIGRETURN
catch_int(s)
	int s;
{
	signal(SIGINT, catch_int);
	interrupt = 1;
}

/* save the mail and go away */
SIGRETURN
save_on_int(s)
	int s;
{
	cleanlocks();
	save_mail(mp);
	exit(1);
}

/* save the mail and go away */
SIGRETURN
die_on_quit(s)
	int s;
{
	cleanlocks();
	exit(1);
}

main(ac, av)
	int ac;
	char *av[];
{
	int i,j;
	dest *dp=NULL;
	int checkforward;
	SIG_TYP fint, fquit, fhup;
	char *base;
	int rv;

	/* process args */
	setbuf(stderr, stderrbuf);
	for (i = 1; i < ac; i++) {
		if (av[i][0] == '-') {
			for(j=1; av[i][j]; j++)
				/* option */
				switch (av[i][1]) {
				case '#':
					nflg = 1;
					break;
				case 'd':
					debug = 1;
					break;
				default:
					fprintf(stderr, "usage: mail [-#] list-of-addresses\n");
					exit(1);
				}
		} else
			/* destination */
			d_insert(&dp, d_new(s_copy(av[i])));
	}
	if (dp == NULL) {
		fprintf(stderr, "usage: mail [-#] address-list\n");
		exit(1);
	}

	/*
	 * get context:
	 *	- whether we're rmail or mail
	 *	- whether on a tty
	 */
	base = basename(av[0]);
	checkforward = rmail = strcmp(base, "rmail")==0;
	onatty = (!rmail) && isatty(0);
	thissys = sysname_read();
	if (!nflg) {
		if ((fint=signal(SIGINT, SIG_IGN))!=SIG_DFL)
			signal(SIGINT, fint);
		else
			signal(SIGINT, fint = (SIG_TYP)catch_int);
		if ((fquit=signal(SIGQUIT, SIG_IGN))!=SIG_DFL)
			signal(SIGQUIT, fquit);
		else
			signal(SIGQUIT, die_on_quit);
		if ((fhup=signal(SIGHUP, SIG_IGN))!=SIG_DFL)
			signal(SIGHUP, fhup);
		else
			signal(SIGHUP, fhup = (SIG_TYP)catch_int);

		mp = m_read(stdin, rmail, onatty);
		if (mp == NULL)
			exit(0);
		if (interrupt != 0) {
			save_mail(mp);
			exit(1);
		}

		if (fint==(SIG_TYP)catch_int)
			signal(SIGINT, save_on_int);
		if (fhup==(SIG_TYP)catch_int)
			signal(SIGHUP, save_on_int);
	} else {
		mp = m_new();
		default_from(mp);
	}
	errstr = s_new();
	(void)getrules();

	/*
	 *  If this is a gateway, translate the sender address into a local
	 *  address.  This only happens if mail to the local address is 
	 *  forwarded to the sender.
	 */
	gateway(mp);

	/*
	 *  Protect against shell characters in the sender name for
	 *  security reasons.
	 */
	s_restart(mp->sender);
	if (shellchars(s_to_c(mp->sender)))
		mp->replyaddr = s_copy("postmaster");
	else
		mp->replyaddr = s_clone(mp->sender);
	s_restart(mp->replyaddr);

	/*
	 *  reject messages that are too long.  We don't do it earlier
	 *  in m_read since we haven't set up enough things yet.
	 */
	if(mp->size < 0)
		return refuse(dp, mp, "message too long", 0);

	rv = send(dp, mp, checkforward);
	if(savemail)
		save_mail(mp);
	return rv;
}


/* send a message to a list of sites */
send(destp, mp, checkforward)
	dest *destp;
	message *mp;
{
	dest *dp;		/* destination being acted upon */
	dest *bound;		/* bound destinations */
	int errors=0;
	static int forked;
	extern dest *up_bind();

	/* bind the destinations to actions */
	bound = up_bind(destp, mp, checkforward);

	/* loop through and execute commands */
	for (dp = d_rm(&bound); dp != NULL; dp = d_rm(&bound)) {
		switch (dp->status) {
		case d_cat:
			errors += cat_mail(dp, mp);
			break;
		case d_pipeto:
		case d_pipe:
			if (!rmail && !nflg && !forked) {
				forked = 1;
				lesstedious();
			}
			errors += pipe_mail(dp, mp, dp->status==d_pipeto);
			break;
		default:
			errors += complain_mail(dp, mp);
			break;
		}
	}

	return errors;
}

/* avoid user tedium (as Mike Lesk said in a previous version) */
lesstedious()
{
	int i;

	switch(fork()){
	case -1:
		onatty = 0;
		break;
	case 0:
		signal(SIGHUP, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTERM, SIG_IGN);
		setpgrp(getpid());
		for(i=0; i<nsysfile; i++)
			close(i);
		onatty = 0;
		savemail=0;
		break;
	default:
		exit(0);
	}
}


/* save the mail */
save_mail(mp)
	message *mp;
{
	FILE *fp;
	string *file=s_new();
	char *home;
	static saved = 0;
	int uid, gid;
	extern char *getenv();

	setuid(uid=getuid());
	setgid(gid=getgid());
	if ((home = getenv("HOME")) == NULL)
		return;
	s_append(file, home);
	s_append(file, "/dead.letter");
	if ((fp = lockopen(s_to_c(file), "w", MBOXMODE, uid, gid)) == NULL)
		return;
	m_bprint(mp, fp);
	lockclose(fp);
	fprintf(stderr, "saved in %s\n", s_to_c(file));
}

/* remember the interrupt happened */
/* dispose of incorrect addresses */
complain_mail(dp, mp)
	dest *dp;
	message *mp;
{
	char *msg;

	switch (dp->status) {
	case d_undefined:
		msg = "Invalid address"; /* a little different, for debugging */
		break;
	case d_syntax:
		msg = "invalid address";
		break;
	case d_unknown:
		msg = "unknown user";
		break;
	case d_eloop:
	case d_loop:
		msg = "forwarding loop";
		break;
	case d_noforward:
		if(dp->pstat && *s_to_c(dp->repl2))
			return refuse(dp, mp, s_to_c(dp->repl2), dp->pstat);
		else
			msg = "destination unknown or forwarding disallowed";
		break;
	case d_pipe:
		msg = "broken pipe";
		break;
	case d_cat:
		msg = "broken cat";
		break;
	case d_translate:
		if(dp->pstat && *s_to_c(dp->repl2))
			return refuse(dp, mp, s_to_c(dp->repl2), dp->pstat);
		else
			msg = "name translation failed";
		break;
	case d_alias:
		msg = "broken alias";
		break;
	case d_badmbox:
		msg = "corrupted mailbox";
		break;
	case d_resource:
		msg = "out of some resource.  Try again later.";
		break;
	}
	if (nflg) {
		printf("%s: %s\n", msg, s_to_c(dp->addr));
		return 0;
	}
	return refuse(dp, mp, msg, 0);
}

/* dispose of remote addresses */
pipe_mail(dp, mp, dolock)
	dest *dp;
	message *mp;
	int dolock;
{
	string *file;
	FILE *fp;
	dest *next, *list=NULL;
	string *cmd;
	process *pp;
	int status;
	string *errstr=s_new();

	/*
	 *  collect the arguments
	 */
	file = s_new();
	abspath(s_to_c(dp->addr), MAILROOT, file);
	next = d_rm_same(&dp);
	cmd = s_clone(s_restart(next->repl1));
	for(; next != NULL; next = d_rm_same(&dp)) {
		if ((next->uid!=-1 || next->gid !=-1)
		&& shellchars(s_to_c(next->addr))){
			/* this could be a serious security violation */
			next->status=d_syntax;
			complain_mail(next, mp);
			continue;
		}
		if (next->repl2 != NULL) {
			s_append(cmd, " ");
			s_append(cmd, s_to_c(next->repl2));
		}
		d_insert(&list, next);
	}

	if (nflg) {
		puts(s_to_c(cmd));
		s_free(cmd);
		s_free(file);
		return 0;
	}

	/*
	 *  lock the mailbox (so that `Pipe to's run sequentially)
	 */
	if(dolock)
		fp = lockopen(s_to_c(file), "r", 0, 0, 0);
	else
		fp = NULL;
	s_free(file);

	/*
	 *  run the process
	 */
	pp = proc_start(s_to_c(cmd), instream(), (stream *)NULL,
			outstream(), list->uid);
	if(pp==NULL || pp->std[0]==NULL || pp->std[2]==NULL)
		return refuse(list, mp, "out of processes, pipes, or memory", 0);
	m_print(mp, pp->std[0]->fp, thissys, 0);
	stream_free(pp->std[0]); pp->std[0] = NULL;
	while(s_read_line(pp->std[2]->fp, errstr) != NULL)
		;
	status = proc_wait(pp);
	proc_free(pp);
	s_free(cmd);

	/*
	 *  unlock the mailbox (if it was locked)
	 */
	if(fp!=NULL)
		lockclose(fp);

	/*
	 *  return status
	 */
	if (status != 0)
		return refuse(list, mp, s_to_c(errstr), status);
	loglist(list, mp, "remote");
	return 0;
}

/* dispose of local addresses */
cat_mail(dp, mp)
	dest *dp;
	message *mp;
{
	FILE *fp;
	char *rcvr;

	if (nflg) {
		printf("cat >> %s\n", s_to_c(dp->repl1));
		return 0;
	}
	fp=lockopen(s_to_c(dp->repl1), "a", MBOXMODE, dp->uid, dp->gid);
	if (fp == NULL)
		return refuse(dp, mp, "mail file cannot be opened", 0);
	m_print(mp, fp, (char *)NULL, 1);
	fputs("\n", fp);
	fflush(fp);
	if (ferror(fp)) {
		lockclose(fp);
		return refuse(dp, mp, "error writing mail file", 0);
	}
	lockclose(fp);
	rcvr = basename(s_to_c(dp->repl1));
	logdelivery(dp, rcvr, mp);
	notify(rcvr, mp);
	return 0;
}

static void
appaddr(sp, dp)
	string *sp;
	dest *dp;
{
	dest *parent;

	if (dp->parent != NULL) {
		for(parent=dp->parent; parent->parent!=NULL; parent=parent->parent)
			;
		s_append(sp, s_to_c(parent->addr));
		s_append(sp, "' alias `");
	}
	s_append(sp, s_to_c(dp->addr));
}

/* reject delivery */
refuse(list, mp, cp, status)
	dest *list;
	message *mp;
	char *cp;
	int status;
{
	string *errstr=s_new();
	dest *dp;
	int rv=0;

	dp = d_rm(&list);
	mkerrstr(errstr, mp, dp, list, cp, status);
	/*
	 * if on a tty just report the error.  Otherwise send mail
	 * reporting the error.  N.B. To avoid mail loops, don't
	 * send mail reporting a failure of mail to reach the postmaster.
	 */
	if (onatty) {
		fputs(s_to_c(errstr), stderr);
		savemail = 1;
		rv = 1;
	} else {
		if (strcmp(s_to_c(mp->replyaddr), "postmaster")!=0)
			rv = replymsg(errstr, mp, dp);
		else
			rv = 1;
	}
	logrefusal(dp, mp, s_to_c(errstr));
	s_free(errstr);
	return rv;
}

/* make the error message */
mkerrstr(errstr, mp, dp, list, cp, status)
	string *errstr;
	message *mp;
	dest *dp;
	dest *list;
	char *cp;
{
	dest *next;
	char smsg[64];

	/* list all aliases */
	s_append(errstr, "Mail to `");
	appaddr(errstr, dp);
	for(next = d_rm(&list); next != NULL; next = d_rm(&list)) {
		s_append(errstr, "', '");
		appaddr(errstr, next);
		d_insert(&dp, next);
	}
	s_append(errstr, "' from '");
	s_append(errstr, s_to_c(mp->sender));
	s_append(errstr, "' failed.\n");

	/* >> and | deserve different flavored messages */
	switch(dp->status) {
	case d_pipe:
		s_append(errstr, "The mailer `");
		s_append(errstr, s_to_c(dp->repl1));
		sprintf(smsg, "' returned error status %x.\n", status);
		s_append(errstr, smsg);
		s_append(errstr, "The error message was:\n");
		s_append(errstr, cp);
		break;
	default:
		s_append(errstr, "The error message was:\n");
		s_append(errstr, cp);
		break;
	}
}

/*
 *  reply with up to 1024 characters of the
 *  original message
 */
replymsg(errstr, mp, dp)
	string *errstr;
	message *mp;
	dest *dp;
{
	message *refp = m_new();
	dest *ndp;
	char *rcvr;
	int rv;

	rcvr = dp->status==d_eloop ? "postmaster" : s_to_c(mp->replyaddr);
	ndp = d_new(s_copy(rcvr));
	s_append(refp->sender, "postmaster");
	s_append(refp->replyaddr, "postmaster");
	s_append(refp->date, thedate());
	s_append(refp->body, s_to_c(errstr));
	s_append(refp->body, "\nThe message began:\n");
	s_nappend(refp->body, s_to_c(mp->body), 8*1024);
	refp->size = strlen(s_to_c(refp->body));
	rv = send(ndp, refp, 0);
	m_free(refp);
	d_free(ndp);
	return rv;
}

