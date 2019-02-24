/*
 *	WARNING! the mail log does not get locked.
 *
 *	Vismon users beware!
 */
#include "mail.h"
#include "string.h"
#include "message.h"
#include "aux.h"
#include "dest.h"
#include <sys/stat.h>

/* configuration */
#define LOGFILE "mail.log"

/* imports */
extern struct tm *localtime();
extern char *asctime();
extern long time();

/* open log file for appending */
static int
logopen()
{
	int out;
	string *file=s_new();
	long lseek();

	umask(0);
	abspath(LOGFILE, MAILROOT, file);
	out = open(s_to_c(file), 2);
	if(out<0)
		out = creat(s_to_c(file), 0666);
	s_free(file);
	return out;
}

/* log mail delivery */
logdelivery(list, rcvr, mp)
	dest *list;	/* receiver(s) */
	char *rcvr;	/* mailbox */
	message *mp;	/* message */
{
	dest *next;
	int out;
	char buf[2048];
	dest *parent;
	char *dp;
	struct tm *bp;
	long thetime;

	out = logopen();

	thetime = time((long *)0);
	bp = localtime(&thetime);
	dp = asctime(bp);

	for(parent=list; parent->parent!=NULL; parent=parent->parent)
		;
	if(parent!=list && strcmp(s_to_c(parent->addr), rcvr)!=0)
		sprintf(buf,"delivered %s From %.256s %.256s (%.256s) [%.5s] %d\n",
			rcvr,
			s_to_c(mp->sender), s_to_c(mp->date),
			s_to_c(parent->addr), dp+11, mp->size);
	else
		sprintf(buf, "delivered %s From %.256s %.256s [%.5s] %d\n", rcvr,
			s_to_c(mp->sender), s_to_c(mp->date), dp+11, mp->size);
	lseek(out, 0L, 2);
	write(out, buf, strlen(buf));

	close(out);
}

/* log mail forwarding */
loglist(list, mp, tag)
	dest *list;	/* receiver(s) */
	message *mp;	/* message */
	char *tag;	/* type of mail */
{
	dest *next;
	int out;
	char buf[2048];
	dest *parent;
	char *dp;
	struct tm *bp;
	long thetime;

	out = logopen();

	thetime = time((long *)0);
	bp = localtime(&thetime);
	dp = asctime(bp);

	for(next=d_rm(&list); next != NULL; next = d_rm(&list)) {
		for(parent=next; parent->parent!=NULL; parent=parent->parent)
			;
		if(parent!=next)
			sprintf(buf, "%s %.256s From %.256s %.256s (%.256s) [%.5s] %d\n",
				tag,
				s_to_c(next->addr), s_to_c(mp->sender),
				s_to_c(mp->date), s_to_c(parent->addr), dp+11,
				mp->size);
		else
			sprintf(buf, "%s %.256s From %.256s %.256s [%.5s] %d\n", tag,
				s_to_c(next->addr), s_to_c(mp->sender),
				s_to_c(mp->date), dp+11, mp->size);
		lseek(out, 0L, 2);
		write(out, buf, strlen(buf));
	}
	close(out);
}

/* log a mail refusal */
logrefusal(dp, mp, msg)
	dest *dp;	/* receiver */
	message *mp;	/* message */
	char *msg;	/* error message */
{
	int out;
	char buf[2048];
	char *cp, *ep;

	out = logopen();
	sprintf(buf, "error %.256s From %.256s %.256s\nerror+ ", s_to_c(dp->addr),
		s_to_c(mp->sender), s_to_c(mp->date));
	cp = buf + strlen(buf);
	ep = buf + sizeof(buf) - sizeof("error + ");
	while(*msg && cp<ep) {
		*cp++ = *msg;
		if (*msg++ == '\n') {
			strcpy(cp, "error+ ");
			cp += sizeof("error+ ") - 1;
		}
	}
	*cp++ = '\n';
	lseek(out, 0L, 2);
	write(out, buf, cp - buf);
	close(out);
}
