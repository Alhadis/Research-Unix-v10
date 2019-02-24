#include "sam.h"

static char *emsg[]={
	/* error_s */
	"can't open",
	"can't create",
	"not in menu:",
	"changes to",
	"I/O error:",
	/* error_c */
	"unknown command",
	"no operand for",
	"bad delimiter",
	/* error */
	"can't fork",
	"out of memory",
	"interrupt",
	"address",
	"search",
	"pattern",
	"newline expected",
	"blank expected",
	"pattern expected",
	"can't nest X or Y",
	"unmatched `}'",
	"command takes no address",
	"addresses overlap",
	"substitution",
	"substitution too long",
	"& match too long",
	"bad \\ in rhs",
	"address range",
	"changes not in sequence",
	"file name too long",
	"addresses out of order",
	"no file name",
	"unmatched `('",
	"unmatched `)'",
	"too many char classes",
	"malformed `[]'",
	"reg. exp. list overflow",
	"unix command",
	"can't pipe",
	"no current file",
	"string too long",
	"changed files",
	"empty string",
	"file search",
	"non-unique match for \"\"",
	"tag match too long",
	"too many subexpressions",
};
static char *wmsg[]={
	/* warn_s */
	"duplicate file name",
	"no such file",
	"write might change good version of",
	/* warn */
	"non-ascii chars elided",
	"can't run pwd",
	"last char not newline",
	"exit status not 0",
};
error(s)
	Error s;
{
	char buf[512];
	sprint(buf, "?%s", emsg[s]);
	hiccough(buf);
}
error_s(s, a)
	Error s;
	char *a;
{
	char buf[512];
	sprint(buf, "?%s \"%s\"", emsg[s], a);
	hiccough(buf);
}
error_c(s, c)
	Error s;
{
	char buf[512];
	sprint(buf, "?%s `%c'", emsg[s], c);
	hiccough(buf);
}
warn(s)
	Warning s;
{
	dprint("?warning: %s\n", wmsg[s]);
}
warn_s(s, a)
	Warning s;
	char *a;
{
	dprint("?warning: %s `%s'\n", wmsg[s], a);
}
dprint(z, a, b, c, d, e)
	char *z;
{
	uchar buf[BLOCKSIZE];
	sprint(buf, z, a, b, c, d, e);
	termwrite(buf, strlen(buf));
}
termwrite(s, n)
	uchar *s;
{
	static String p;	/* avoid tempstr: we might be using it now */
	if(downloaded){
		p.n=p.size=n;
		p.s=s;
		if(cmd)
			Finsert(cmd, &p, cmdpt);
		else
			strinsert(&cmdstr, &p, (long)cmdstr.n);
		cmdptadv+=n;
	}else
		Write(2, s, n);
}
