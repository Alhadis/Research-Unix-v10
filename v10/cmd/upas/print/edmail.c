/*
 *	print mail
 */
#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#include <signal.h>
#include <regexp.h>
#include "mail.h"
#include "string.h"
#include "message.h"
#include "aux.h"
#include <sys/stat.h>

/* global to this file */
static int reverse=0; 		/* ordering of mail messages */
static jmp_buf	sjbuf;
int fflg;
int mflg;
int pflg;
int eflg;
SIG_TYP fint;			/* what to do in case of int */
SIG_TYP fquit;			/* what to do in case of quit */
SIG_TYP fpipe;			/* what to do in case of sigpipe */

/* predeclared */
message *addr();
message *grange();
message *range();
message *research();
message *bresearch();
message *incraddr();
char *getre();
char *header();
int edmail();
char *doargs();
void dumpmail();

/* imported */
char *getlog();
FILE *lockopen();
FILE *lockreopen();

SIGRETURN
done(s)
	int s;
{
	cleanlocks();
	V();
	exit(1);
}

main(ac, av)
	int ac;
	char *av[];
{
	char outbuf[BUFSIZ];
	int writeable;
	string *user = s_new();
	char *mailfile;
	char *logname;

	/* set signals - make sure we don't override some previous setting */
	if ((fint=signal(SIGINT, SIG_IGN))!=SIG_DFL)
		signal(SIGINT, fint);
	else
		signal(SIGINT, fint = (SIG_TYP)done);
	if ((fquit=signal(SIGQUIT, SIG_IGN))!=SIG_DFL)
		signal(SIGQUIT, fquit);
	else
		signal(SIGQUIT, fquit = (SIG_TYP)done);
	if ((fpipe=signal(SIGPIPE, SIG_IGN))!=SIG_DFL)
		signal(SIGPIPE, fpipe);
	else
		signal(SIGPIPE, fpipe = (SIG_TYP)done);

	logname = getlog();
	if (logname == NULL) {
		printf ("mail: cannot determine login name\n");
		return 0;
	}
	s_append(user, logname);
	/* setbuf(stdout, outbuf); */
	mailfile = doargs(ac, av, s_to_c(user));
	if (!fflg)
		writeable = P()==0;
	else
		writeable = 1;
	if (eflg) {
		int r = check_mbox(mailfile);
		V();
		return r;
	}
	if (read_mbox(mailfile, reverse)<0 || mlist == NULL) {
		printf("No mail\n");
		V();
		return 0;
	}
	printf("%d messages\n", mlast->pos);
	if (pflg)
		dumpmail();
	else{
		while (edmail(mailfile, reverse) && writeable){
			if(!write_mbox(mailfile, reverse))
				break;
		}
	}
	V();
	return 0;
}

/* parse arguments */
char *
doargs(ac, av, user)
	int ac;
	char *av[];
	char *user;
{
	int i;
	string *mailfile;
	char *cp;

	/* process args */
	mailfile = s_new();
	abspath(user, MAILROOT, mailfile);
	for (i = 1; i<ac; i++) {
		if (av[i][0]!='-') {
			fprintf(stderr, "usage: mail [-mpre] [-f mbox]\n");
			exit(1);
		}
		for (cp = av[i]+1; *cp; cp++) {
			switch(*cp) {
			case 'r':
				reverse = 1;
				break;
			case 'm':
				mflg = 1;
				break;
			case 'p':
				pflg = 1;
				break;
			case 'e':
				eflg = 1;
				break;
			case 'f':
				fflg = 1;
				if (i+1 >= ac) {
					fprintf(stderr,"mail: missing filename\n");
					exit(1);
				}
				s_append(s_restart(mailfile), av[++i]);
				break;
			default:
				fprintf(stderr, "usage: mail [-mpre] [-f mbox]\n");
				exit(1);
			}
		}
	}
	return s_to_c(mailfile);
}

/* is the mailbox empty?  1 if yes, 0 if no */
int
check_mbox (mf)
	char *mf;
{
	int n, r;
	char mbuf[1000];
	string *line;
	FILE *fp;

	/* if file doesn't exist, no mail */
	if ((fp = fopen(mf, "r")) == NULL)
		return 1;

	/* if we can't read it, no mail */
	if (fgets(mbuf, sizeof(mbuf)-2, fp)==NULL){
		fclose(fp);
		return 1;
	}
	fclose(fp);

	/* it is empty iff delivery status is abnormal */
	n = strlen(mbuf);
	mbuf[n++] = '\n';
	mbuf[n++] = '\0';
	line = s_array(mbuf, n);
	r = delivery_status(s_restart(line)) != MF_NORMAL;

	s_free (line);
	return r;
}

/* mesage dump */
void
dumpmail()
{
	message *mp;

	for (mp=mlist; mp!=NULL; mp=mp->next) {
		m_print(mp, stdout, 1, 1);
	}
}

SIGRETURN
catchint(s)
	int s;
{
	signal(SIGINT, catchint);
	clearerr(stdin);
	clearerr(stdout);
	longjmp(sjbuf, 1);
}

notatnl(cp)
	char *cp;
{
	if (*cp=='\n') {
		fprintf(stderr, "!argument expected\n");
		return -1;
	}
	return 0;
}

int
atblank(cp)
char *cp;
{
	if (*cp!='\n' && *(cp-1)!=' ' && *(cp-1)!='\t') {
		fprintf(stderr, "newline or space expected\n");
		return -1;
	}
	return 0;
}

atnl(cp)
	char *cp;
{
	if (*cp!='\n') {
		fprintf(stderr, "!newline expected\n");
		return -1;
	}
	return 0;
}

zero(mp)
	message *mp;
{
	if (mp==mzero) {
		fprintf(stderr, "!message 0\n");
		return -1;
	}
	return 0;
}

#define s_skipwhite(s) for (; *s->ptr==' ' || *s->ptr=='\t'; s->ptr++);

/* ed style interface */
edmail(mailfile, reverse)
	char *mailfile;
	int reverse;
{
	message *dot;
	message *extent;
	string *cmd=s_new();
	char *cp;
	int cmdc;
	int i, abort=0, nopr, change=1;
	int del;

	extent = dot = mzero;
	nopr = mflg;
	for(;;) {
		extent = dot;	/* in case of interrupt */
		if (!nopr) {
			/* advance only if we want to print next message */
			if(dot->next!=NULL)
				dot = dot->next;
			else
				nopr = 1;
		}
		if (setjmp(sjbuf)) {
			/* come here after interrupt */
			if (extent!=NULL)
				dot = extent;
			nopr = 1;
			printf("\n");
		}
		if (fint==(SIG_TYP)done)
			signal(SIGINT, catchint);
		if (!nopr&&dot!=mzero) {
			/* print next message */
			extent = dot;	/* in case of interrupt */
			print(dot);
			nopr = 1;
		}
		abort = 0;
		nopr = 1;
		change = 1;
		printf("?");
		fflush(stdout);
		if(s_read_line(stdin, s_restart(cmd)) == NULL) {
			signal(SIGINT, fint);	/* bacause of the setjmp */
			return 1;
		}
		s_restart(cmd);
		s_skipwhite(cmd);
		if (!mflg && *cmd->ptr=='\n' && dot==mlast) {
			signal(SIGINT, fint);	/* bacause of the setjmp */
			return 1;
		}
		extent = range(dot, cmd);
		s_skipwhite(cmd);
		del = 0;
compoundcmd:
		/* hack to catch a common mistake */
		if (strncmp(cmd->ptr, "mail", 4)==0)
			cmdc = -1;
		else
			cmdc = *cmd->ptr++;
		s_skipwhite(cmd);
		cp = cmd->ptr;
		for(; extent!=NULL && !abort; extent=extent->extent) {
			switch(cmdc){
			case 'b':
				abort = atnl(cp)||(del&&delete(extent));
				if (!abort)
					if (extent!=mzero)
						prheader(extent);
				for(i=0; extent->next!=NULL&&i<9; i++){
					extent = extent->next;
					prheader(extent);
				}
				break;
			case 'h':
				abort = atnl(cp)||zero(extent)
					||(del&&delete(extent))
					||prheader(extent);
				break;
			case 'd':
				if(*cp=='\n' || *cp==0){
					abort = zero(extent)||delete(extent);
					nopr = abort||mflg;
					break;
				}
				del = 1;
				goto compoundcmd;
			case 's':
				abort = atblank(cp)||zero(extent)||store(extent, cp, 1)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case 'w':
				abort = atblank(cp)||zero(extent)||notatnl(cp)
					||store(extent, cp, 0)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case 'm':
				abort = atblank(cp)||zero(extent)||notatnl(cp)
					||remail(extent, cp, 0)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case 'M':
				abort = atblank(cp)||zero(extent)||notatnl(cp)
					||remail(extent, cp, 1)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case '\n':
				abort = zero(extent)||print(extent);
				break;
			case 'p':
				if(del){
					nopr = abort = atnl(cp)||zero(extent)
						||(del&&delete(extent));
					break;
				}
				abort = atnl(cp)||zero(extent)||print(extent);
				break;
			case 'r':
				abort = zero(extent)||atnl(cp)||reply(extent, 0)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case 'R':
				abort = zero(extent)||atnl(cp)||reply(extent, 1)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case 'q':
				abort=atnl(cp)
				    ||(del&&(zero(extent)||delete(extent)));
				if(abort)
					break;
				signal(SIGINT, fint);	/* bacause of the setjmp */
				return 1;
			case '|':
				abort = zero(extent)||notatnl(cp)
					||pipemail(extent, cp, 0)
					||(del&&delete(extent));
				nopr = abort||mflg||!del;
				break;
			case '=':
				abort = atnl(cp)||(del&&delete(extent))
					||whereis(extent);
				change = 0;
				break;
			case '!':
				abort=del&&(zero(extent)||delete(extent));
				if(abort)
					break;
				escape(cp);
				break;
			case 'u':
				abort = zero(extent)||atnl(cp)||undelete(extent);
				break;
			case 'x':
				signal(SIGINT, fint);	/* bacause of the setjmp */
				return 0;
			case '?':
				abort = atnl(cp)
					||del&&(zero(extent)||delete(extent));
				if(abort)
					break;
				help();
				nopr = abort = 1;
				break;
			case 'i':
				abort = atnl(cp)
					||(del&&(zero(extent)||delete(extent)));
				if(abort)
					break;
				reread_mbox(mailfile, reverse);
				nopr = abort = 1;
				break;
			default:
				printf("!unknown command (type ? for help)\n");
				nopr = abort = 1;
				break;
			}
			if (!abort&&change&&extent!=NULL)
				dot = extent;
		}
	}
}

/* parse an address range */
message *
range(dot, cmd)
	message *dot;
	string *cmd;
{
	message *first, *last;

	s_skipwhite(cmd);

	/* get first address in range */
	switch(*cmd->ptr) {
	case 'g':
		cmd->ptr++;
		return grange(cmd);
	case ',':
		first = mlist;
		break;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		first = addr((message *)NULL, cmd);
		break;
	case '?':
		if (*(cmd->ptr+1)=='\n') {
			if(dot!=NULL)
				dot->extent = NULL;
			return dot;
		}
	case '\\': case '/': case '+': case '-': case '$': case '.':
		first = addr(dot, cmd);
		break;
	case '\n':
		first = dot->next;
		if (first==NULL)
			fprintf(stderr, "!address\n");
		break;
	default:
		if(dot!=NULL)
			dot->extent = NULL;
		return dot;
	}
	if (first == NULL)
		return NULL;
	while(*cmd->ptr == ' ' || *cmd->ptr == '\t')
		cmd->ptr++;
	if (*cmd->ptr != ',') {
		first->extent = NULL;
		return first;
	}

	/* get second address in range */
	cmd->ptr++;
	while(*cmd->ptr == ' ' || *cmd->ptr == '\t')
		cmd->ptr++;
	switch(*cmd->ptr) {
	case '\\': case '/': case '?': case '+': case '-': case '$':
		last = addr(first, cmd);
		break;
	case '.':
		last = addr(dot, cmd);
		break;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		last = addr((message *)NULL, cmd);
		break;
	default:
		last = mlast;
		break;
	}
	if (last==NULL)
		return NULL;

	/* fill in the range */
	for(dot=first; dot!=last && dot!=NULL; dot=dot->next)
		dot->extent = dot->next;
	if (dot==NULL) {
		fprintf(stderr, "!addresses out of order\n");
		return NULL;
	}
	last->extent = NULL;
	return first;
}

/* parse a global range */
message *
grange(cmd)
	string *cmd;
{
	char *re;
	message *first, *last, *next;
	regexp *pp;

	if (*cmd->ptr == '/') {
		re = getre(cmd);
		if (re == NULL)
			return NULL;
		if ((pp = regcomp(re))==NULL)
			return NULL;
		first = NULL;
		for(next=mlist; next!=NULL; next=next->next)
			if (regexec(pp, header(next), 0, 0)) {
				if (first==NULL)
					first = last = next;
				else {
					last->extent = next;
					last = next;
				}
				last->extent = NULL;
			}
		if (first==NULL)
			fprintf(stderr, "!match\n");
		free((char *)pp);
		return first;
	} else if (*cmd->ptr == '\\') {
		re = getre(cmd);
		if (re == NULL)
			return NULL;
		if ((pp = regcomp(re))==NULL)
			return NULL;
		first = NULL;
		for(next=mlist; next!=NULL; next=next->next){
			s_to_c(next->body)[next->size-1]='\0';
			if (regexec(pp, s_to_c(next->body), 0, 0)) {
				if (first==NULL)
					first = last = next;
				else {
					last->extent = next;
					last = next;
				}
				last->extent = NULL;
			}
		}
		if (first==NULL)
			fprintf(stderr, "!match\n");
		free((char *)pp);
		return first;
	} else {
		/* fill in the range */
		for(next=mlist; next!=NULL; next=next->next)
			next->extent = next->next;
		mlast->extent = NULL;
		return mlist;
	}
}

/* parse an address */
message *
addr(base, cmd)
	message *base;		/* where to compute +/- from */
	string *cmd;
{
	int forward = -1;

	/* get direction */
	switch(*cmd->ptr) {
	case '+':
		forward = 1;
		cmd->ptr++;
		break;
	case '-':
		forward = 0;
		cmd->ptr++;
		break;
	}
	while(*cmd->ptr == ' ' || *cmd->ptr == '\t')
		cmd->ptr++;
	switch(*cmd->ptr) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		base = incraddr(base, getnumb(cmd), forward);
		if (base==NULL)
			break;
		return addr(base, cmd);
	case '?':
		forward = !forward;
	case '/':
		base = research(base, getre(cmd), forward);
		if (base==NULL) {
			fprintf(stderr, "!search\n");
			return NULL;
		}
		return addr(base, cmd);
	case '\\':
		base = bresearch(base, getre(cmd), forward);
		if (base==NULL) {
			fprintf(stderr, "!search\n");
			return NULL;
		}
		return addr(base, cmd);
	case '.':
		cmd->ptr++;
		return addr(base, cmd);;
	case '$':
		cmd->ptr++;
		return addr(mlast, cmd);
	default:
		/* default increment is 1 */
		if (forward != -1) {
			base = incraddr(base, 1, forward);
			return addr(base, cmd);
		}
		break;
	}
	if (base==NULL)
		fprintf(stderr, "!address\n");
	return base;
}

/* increment the base address by offset */
message *
incraddr(base, offset, forward)
	message *base;
	int offset;
	int forward;
{
	if (base==NULL) {
		base = mzero;
	}
	for(; offset > 0 && base != NULL; offset--)
		base = forward?(base->next):(base->prev);
	return base;
}

/* look for the message header after base that matches the reg exp */
message *
research(base, rp, forward)
	message *base;
	char *rp;
	int forward;
{
	regexp *pp;
	message *mp;

	if (rp == NULL) 
		return NULL;
	if ((pp = regcomp(rp))==NULL)
		return NULL;
	mp = base;
	base = (base==mzero)?(forward?(mp->prev):(mp->next)):base;
	do {
		mp = forward?(mp->next):(mp->prev);
		if (mp==NULL)
			mp = forward?mlist:mlast;
		if (regexec(pp, header(mp), 0, 0)) {
			free((char *)pp);
			return mp;
		}
	} while (mp!=base);
	free((char *)pp);
	return NULL;
}

/* look for the message after base that matches the reg exp */
message *
bresearch(base, rp, forward)
	message *base;
	char *rp;
	int forward;
{
	regexp *pp;
	message *mp;

	if (rp == NULL) 
		return NULL;
	if ((pp = regcomp(rp))==NULL)
		return NULL;
	mp = base = (base==mzero)?mzero->next:base;
	do {
		mp = forward?(mp->next):(mp->prev);
		if (mp==NULL)
			mp = forward?mlist:mlast;
		s_to_c(mp->body)[mp->size-1]='\0';
		if (regexec(pp, s_to_c(mp->body), 0, 0)) {
			free((char *)pp);
			return mp;
		}
	} while (mp!=base);
	free((char *)pp);
	return NULL;
}

/* get a number out of the command */
int
getnumb(cmd)
	string *cmd;
{
	int offset=0;
	while (isdigit(*cmd->ptr))
		offset = offset*10 + *cmd->ptr++ - '0';
	return offset;
}

/* get a regular exression out of the command */
char *
getre(cmd)
	string *cmd;
{
	static char re[80];
	char *cp=re;
	char term = *cmd->ptr++;

	while (*cmd->ptr!='\0' && *cmd->ptr!='\n' && *cmd->ptr!=term)
		*cp++ = *cmd->ptr++;
	if (*cmd->ptr == term)
		cmd->ptr++;
	if (cp == re) {
		if (*re == '\0') {
			fprintf(stderr, "!no previous regular expression\n");
			return NULL;
		}
	} else
		*cp = '\0';
	return re;
}

regerror(msg)	
	char *msg;
{
	fprintf(stderr, "!illegal address: %s\n", msg);
}
