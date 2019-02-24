#ifndef lint
static char *sccsid = "@(#)netio.c	1.7 87/07/31";
#endif lint

#include <stdio.h>
#include <setjmp.h>
#include <sysexits.h>

#include "smtp.h"

char *strcpy(), *strncat();

int
tgets(line, size, fi)		/* fgets from TCP */
char *line;
int size;
FILE *fi;
{
	register char *cr, *rp;

	*line = 0;
	rp = fgets(line, size, fi);
	if (ferror(fi)) {
		perror("error reading from smtp");
		bomb(EX_IOERR);
	}
	if (feof(fi)) {
		perror("eof from smtp");
		bomb(EX_IOERR);
	}
	if (rp==NULL) {
		perror("Eof reading from smtp");
		bomb(EX_IOERR);
	}

	/* convert \r\n -> \n */
	cr = line + strlen(line) - 2;
	if (cr >= line && *cr == '\r' && *(cr+1) == '\n') {	/* CRLF there? */
		*cr++ = '\n';
		*cr = 0;
	} else				/* no CRLF present */
		cr += 2;		/* point at NUL byte */

	if (feof(fi)) {
		perror("read eof from smtp");
		bomb(EX_IOERR);
	}
	return cr - line;
}

int
tputs(line, fo)			/* fputs to TCP */
char *line;
FILE *fo;
{
	char buf[MAXSTR];
	register char *nl;

	(void) strcpy(buf, line);
	/* replace terminating \n by \r\n */
	nl = buf + strlen(buf) - 1;		/* presumably \n */
	if (nl >= buf && *nl=='\n') {		/* if it is ... */
		*nl++ = '\r';
		*nl++ = '\n';
		*nl = 0;
	} else
		printf("unterminated line: <%s>\n", buf);

	(void) fputs(buf, fo);
	(void) fflush(fo);
	if (ferror(fo)) {
		perror("error writing to smtp");
		bomb(EX_IOERR);
	}
	return 0;
}
