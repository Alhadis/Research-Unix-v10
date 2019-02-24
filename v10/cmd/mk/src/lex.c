#include	"mk.h"

lex(fd, buf)
	char *buf;
{
	char buf1[BIGBLOCK];

	quote1(fd, buf, buf1);	/* '..' */
	bquote(fd, buf1, buf);	/* `cmd` */
	quote2(fd, buf, buf1);	/* ".." and variable expansion */
	strcpy(buf, buf1);
}
