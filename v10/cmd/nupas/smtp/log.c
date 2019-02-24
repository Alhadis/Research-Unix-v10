#include <stdio.h>
#include <ctype.h>
#include "sys.h"

struct loglevellookup {
	char *name;
	int level;
} loglevellookup[] = {
	{ "emerg",	LOG_EMERG },
	{ "alert",	LOG_ALERT },
	{ "crit",	LOG_CRIT },
	{ "err",	LOG_ERR },
	{ "warning",	LOG_WARNING },
	{ "notice",	LOG_NOTICE },
	{ "info",	LOG_INFO },
	{ "debug",	LOG_DEBUG },
	{ NULL,		-1 },
};


extern int debug;

/*
 * This is a poor substitute for varargs, but
 * syslog doesn't have the equivalent of vfprintf, so this
 * will have to do.
 */
Syslog(level, fmt, p0, p1, p2, p3, p4)
	int level;
	char *fmt;
{
	syslog(level, fmt, p0, p1, p2, p3, p4);
	if (debug)
		fprintf(stderr, fmt, p0, p1, p2, p3, p4);
}

Openlog(label, flags, log)
char *label;
int flags;
int log;
{
	openlog(label, flags, log);
}

setloglevel(arg)
	char *arg;
{
	int newlevel = -1, i;
	char *cp;

	for (cp=arg; *cp; cp++)
		if (isupper(*cp))
			*cp = tolower(*cp);

	if (isdigit(*arg)) {
		newlevel = atoi(arg);
	} else {
		for (i=0; loglevellookup[i].name; i++)
			if (strcmp(loglevellookup[i].name, arg) == 0) {
				newlevel = loglevellookup[i].level;
				break;
			}
	}
	if (newlevel < 0)
		Syslog(LOG_WARNING, "Illegal log parameter - %s\n", arg);
	else
		setlogmask(LOG_UPTO(newlevel));
}
