#include <ctype.h>
#include <regexp.h>	/* FIXME: use Posix interface. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"
#include "dist.h"

static char *
gettok(char *tok, char *src)
{
	while (!isspace(*src)) {
		if (!*src)
			break;
		*tok++ = *src++;
	}
	while (isspace(*src))
		++src;
	return src;
}

/* return the next line of the config file matching the given args.
   and argument can be NULL and will be assumed to match.  the return
   value is in a static data area.  */
struct conf *
confread(FILE *fp, char *host, char *user, char *file)
{
	static char confline[MAXLINE];
	static char confhost[MAXLINE], confuser[MAXLINE], conffile[MAXLINE];
	char *next;
	static struct conf conf = { confhost, confuser, conffile };
	int len, flag;
	regexp *re;
	regsubexp match[1];

	for (;;) {
		if (!fgets(confline, MAXLINE, fp))
			return NULL;
		if (confline[0] == '#')
			continue;
		len = strlen(confline);
		if (confline[len - 1] != '\n') {
			if (feof(fp))
				eprintf("readconf: incomplete last line!");
			else
				eprintf("readconf: line too long or embedded NUL!");
			exit(1);
		}
		next = gettok(confhost, confline);
		next = gettok(confuser, next);
		conf.cmds = gettok(conffile, next);
		if (host) {
			re = regcomp(confhost);
			flag = regexec(re, host, match, 1);
			free(re);
			if (!flag || match[0].ep - match[0].sp != strlen(host))
				continue;
		}
		if (user) {
			re = regcomp(confuser);
			flag = regexec(re, user, match, 1);
			free(re);
			if (!flag || match[0].ep - match[0].sp != strlen(user))
				continue;
		}
		if (file) {
			re = regcomp(conffile);
			flag = regexec(re, file, match, 1);
			free(re);
			if (!flag || match[0].ep - match[0].sp != strlen(file))
				continue;
		}
		return &conf;
	}
}

/* call functions associated with the commands in the given string
   return the total of the return values */
int
confexec(char *cmdstr, struct confcmds cmdtab[])
{
	char cmd[MAXLINE], arg[MAXLINE];
	int i, np, ret;

	ret = 0;
	while (*cmdstr) {
		for (i = 0; isalnum(*cmdstr); ++cmdstr)
			cmd[i++] = *cmdstr;
		cmd[i] = 0;
		if (*cmdstr == '(')
			for (i = 0, np = 1, ++cmdstr; *cmdstr && np; ++cmdstr) {
				if (*cmdstr == '(')
					++np;
				if (*cmdstr == ')' && !--np)
					continue;
				arg[i++] = *cmdstr;
			}
		for (i = 0; cmdtab[i].name; ++i)
			if (!strcmp(cmd, cmdtab[i].name)) {
				ret += (*cmdtab[i].func)(arg);
				break;
			}
		while (*cmdstr && !isalnum(*cmdstr))
			++cmdstr;
	}
	return ret;
}
