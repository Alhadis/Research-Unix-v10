#include <pads.pri>
SRCFILE("padrcv.c")

void PadRcv.invalidate()
{
	static short uniq;

	magic = 0;
	oid = ++uniq;
}
	
#define MAGIC (('P'<<8)|'R')
PadRcv.PadRcv()
{
	invalidate();
	magic = MAGIC;
}

int PadRcv.isvalid()		{ return magic == MAGIC; }

PadRcv.~PadRcv()		{ invalidate(); }

char *PadRcv.help()
{
	trace("%d.help()", this);
	return "Sorry - no help. Complain to application programmer.";
}

void MissingVirtual(char *s, char *f)
{
	char t[128];
	trace("MissingVirtual(%s,%s)", s, f);
	sprintf(t, "%s object does not define %s()", s, f);
	PadsWarn(t);
}

char *PadRcv.kbd(char*s)	{ return "no kbd() function"; }

void PadRcv.numeric(long n)	{ MissingVirtual("NumericRange()", "numeric"); }

void PadRcv.userclose()		{ MissingVirtual("USERCLOSE", "userclose"); }

void PadRcv.usercut()		{ MissingVirtual("USERCUT", "usercut"); }

void PadRcv.cycle()		{ MissingVirtual("alarm()", "cycle"); }

void PadRcv.linereq(long, Attrib) { MissingVirtual("lines()", "linereq"); }

int PadRcv.accept(Action a)	{ trace("%d.accept(%d)", this, a);  return 0;}

int PadRcv.disc()		{ trace("%d.disc()", this); return 0;	}
