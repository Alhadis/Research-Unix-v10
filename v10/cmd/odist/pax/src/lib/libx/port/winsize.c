/*
 * AT&T Bell Laboratories
 * return terminal rows and cols
 */

#include <ttyinfo.h>

#include "FEATURE/jioctl"
#ifdef _hdr_jioctl
#define winsize	jwinsize
#include <jioctl.h>
#undef	winsize
#else
#ifdef _sys_jioctl
#define winsize	jwinsize
#include <sys/jioctl.h>
#undef	winsize
#endif
#endif

static int	ttctl();

void
winsize(rows, cols)
register int*	rows;
register int*	cols;
{
#ifdef TIOCGSIZE
	struct ttysize	ts;

	if (!ttctl(TIOCGSIZE, &ts) && ts.ts_lines > 0 && ts.ts_cols > 0)
	{
		if (rows) *rows = ts.ts_lines;
		if (cols) *cols = ts.ts_cols;
	}
	else
#else
#ifdef TIOCGWINSZ
	struct winsize	ws;

	if (!ttctl(TIOCGWINSZ, &ws) && ws.ws_col > 0 && ws.ws_row > 0)
	{
		if (rows) *rows = ws.ws_row;
		if (cols) *cols = ws.ws_col;
	}
	else
#else
#ifdef JWINSIZE
	struct jwinsize	ws;

	if (!ttctl(JWINSIZE, &ws) && ws.bytesx > 0 && ws.bytesy > 0)
	{
		if (rows) *rows = ws.bytesy;
		if (cols) *cols = ws.bytesx;
	}
	else
#endif
#endif
#endif
	{
		char*		s;

		extern int	atoi();
		extern char*	getenv();

		if (rows) *rows = (s = getenv("LINES")) ? atoi(s) : 0;
		if (cols) *cols = (s = getenv("COLUMNS")) ? atoi(s) : 0;
	}
}

/*
 * tty ioctl() -- no cache
 */

static int
ttctl(op, tt)
int	op;
char*	tt;
{
	register int	fd;
	register int	v;

	extern int	ioctl();

	for (fd = 0; fd <= 2; fd++)
		if (!ioctl(fd, op, tt)) return(0);
	if ((fd = open("/dev/tty", 0)) >= 0)
	{
		v = ioctl(fd, op, tt);
		(void)close(fd);
		return(v);
	}
	return(-1);
}
