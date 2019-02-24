#include <stdio.h>
#include "mail.h"
#include "string.h"

/* format of REMOTE FROM lines */
char *REMFROMRE =
	"^>?From[ \t]+([^ \t]+)[ \t]+(.+)[ \t]+remote[ \t]+from[ \t]+(.*)\n$";
int REMSENDERMATCH = 1;
int REMDATEMATCH = 2;
int REMSYSMATCH = 3;

/* format of LOCAL FROM lines */
char *FROMRE =
	"^>?From[ \t]+([^ \t]+)[ \t]+(.+)\n$";
int SENDERMATCH = 1;
int DATEMATCH = 2;

/* output a unix style local header */
void
print_header(fp, sender, date)
	FILE *fp;
	char *sender, *date;
{
	fprintf(fp, "From %s %s\n", sender, date);
}

/* output a unix style remote header */
void
print_remote_header(fp, sender, date, system)
	FILE *fp;
	char *sender, *date, *system;
{
	fprintf(fp, "From %s %s remote from %s\n", sender, date, system);
}

/* parse a mailbox style header */
int
parse_header(line, sender, date)
	register char *line;
	register string *sender, *date;
{
	if (!IS_HEADER(line))
		return -1;
	line += sizeof("From ") - 1;
	s_restart(sender);
	while(*line==' '||*line=='\t')
		line++;
	while(*line&&*line!=' '&&*line!='\t')
		s_putc(sender, *line++);
	s_terminate(sender);
	s_restart(date);
	while(*line==' '||*line=='\t')
		line++;
	while(*line)
		s_putc(date, *line++);
	s_terminate(date);
	return 0;
}
