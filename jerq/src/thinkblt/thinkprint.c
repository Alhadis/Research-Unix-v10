#include <jerq.h>
#include "think.h"

#define ESC	033
#define DEL	0177

thinkprint(getc)
register int (*getc)();
{
	register linepos = 0, nspaces;
	register c, iquit = 0;

	while (!(iquit = (*thquitf)()) && (c = (*getc)()) >= 0) switch (c) {
	case '\b':
		thinkchar('\b');
		if (linepos > 0)
			linepos--;
		break;
	case '\t':
		linepos += nspaces = 8 - linepos%8;
		thinknchars(nspaces, "        ");
		break;
	case '\n':
		thinkchar('\n');
	case '\r':
		thinkchar('\r');
		linepos = 0;
		break;
	case ESC:
		thinkchar(ESC);
		if ((c = (*getc)()) < 0)
			break;
		thinkchar(c);
		if (c == 'z' || c == '=')
			break;
		while ((c < '@' || c > 'Z') && (c = (*getc)()) >= 0)
			thinkchar(c);
		break;
	default:
		thinkchar(c);
		if ((c &= 127) >= ' ' && c < DEL)
			++linepos;
	}
	return iquit ? thinkabort() : thinkflush();
}
