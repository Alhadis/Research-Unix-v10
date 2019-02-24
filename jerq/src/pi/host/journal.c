#include "journal.pri"
#include "core.pub"
#include "format.h"
SRCFILE("journal.c")

Journal.Journal(char *p)
{
	proc = p;
	key = 0;
	length = 50;
	lastreq = 0;
}

void Journal.open()
{
	trace("%d.open()", this);		VOK;
	if( !pad ){
		pad = new Pad((PadRcv*) this);
		banner();
	}
	pad->makecurrent();
}

void Journal.hostclose()
{
	trace( "%d.hostclose()", this );	VOK;
	if( pad ){
		delete pad;
		pad = 0;
	}
	if( bls ){
		delete bls;
		delete ct;
		bls = 0;
		ct = 0;
	}
	invalidate();
}

void Journal.banner()
{
	trace("%d.banner()", this);	VOK;
	if( pad ){
		pad->banner("Journal: %s", proc);
		pad->name("Journal %s", proc);
		pad->tabs(2);
		pad->options(TRUNCATE|NO_TILDE);
		if( !bls ){
			bls = new Bls[length];
			ct = new int[length];
		}
	}
}

void Journal.insert(PRINTF_ARGS)
{
	trace("%.insert(%s)", this, fmt);
	if( !this ) return;	VOK;
	if( !this->pad || !bls ) return;			// can that be?
	Bls t(PRINTF_COPY);
	if( !t.text[0] ) return;
	long i = key%length;
	if( key==0 || strcmp(bls[i].text, t.text) ){
		i = (++key)%length;
		ct[i] = 0;
		bls[i].clear();
		bls[i].af("%s", t.text);
	}
	if( lastreq == key )
		linereq(key);
	else
		pad->createline(key);
	++ct[i];
	if( key-length > 0 ) pad->removeline(key-length);
}

void Journal.linereq(long i, Attrib a)
{
	trace("%.linereq(%d,%x)", this, i, a); VOK;
	if( i <= key-length || !bls ) return;			// too late!
	int mod = i%length;
	if( ct[mod] > 1 )
		pad->insert(i, a, "%s (%d)", bls[mod].text, ct[mod]);
	else
		pad->insert(i, a, "%s", bls[mod].text);
	lastreq = i;
}
