#include <pads.pri>
SRCFILE("pad.c")
#include <ctype.h>

void Pad.makecurrent()	{ termop(P_MAKECURRENT); }
void Pad.clear()	{ termop(P_CLEAR); }

void Pad.alarm(short n)
{
	R->pktstart(P_ALARM);
	R->sendobj( _object );
	R->sendshort( n );
	R->pktend();
}

Pad.~Pad()		{ trace("%d.~Pad()", this); VOK; termop(P_DELETE); }
int Pad.ok()		{ return this!=0; }
int Line.ok()		{ return this!=0; }

Attrib Implicits(PadRcv *obj)		/* can't inline - C++ bug */
{
	Attrib accum = 0;
	PadRcv padrcv;

	if( !obj ) return 0;
	if( &obj->kbd	    != &padrcv.kbd	 ) accum |= ACCEPT_KBD; // warning
	if( &obj->userclose != &padrcv.userclose ) accum |= USERCLOSE;  // warning
	trace( "Implicits(%d,0x%X)", obj, accum );
	return accum;
}

Pad.Pad(PadRcv *o)
{
	trace( "%d.Pad(%d)", this, o );		VOK;
	if( o && !o->isvalid() )
		PadsWarn( "Pad.Pad: object is not a PadRcv" );
	_name = "<name>";
	_banner = "<banner>";
	_object = o;
	_attributes = 0;
	R->pktstart( P_PADDEF );
	R->sendobj( _object );
	R->pktend();
	options( Implicits(_object) );
}

void Pad.nameorbanner(Protocol p, PRINTF_ARGS)
{
	char *t, **_born = (p==P_BANNER ? &_banner : &_name);
	trace( "%d.nameorbanner(0x%X,%s) %s", this, p, fmt, *_born );	VOK;
	t = sf( PRINTF_COPY );
	if( strcmp( t, *_born ) ){
		R->pktstart( p );
		R->sendobj( _object );
		R->sendstring( *_born = t );
		R->pktend();
	}
}
	
void Pad.banner(PRINTF_ARGS)	{ nameorbanner(P_BANNER, PRINTF_COPY); }
	
void Pad.name(PRINTF_ARGS)	{ nameorbanner(P_NAME, PRINTF_COPY); }

void Pad.tabs(short n)
{
	short lo = 1, hi = 127;

	trace( "%d.tabs(%d)", this, n );	VOK;
	if( n<lo || n>hi )
		PadsWarn( "tabs(%d) should be >=%d and <=%d", n, lo, hi );
	else {
		R->pktstart( P_TABS );
		R->sendobj( _object );
		R->sendshort( n );
		R->pktend();
	}
}

void Pad.removeline(long k)
{
	trace( "%d.removeline(%d)", this, n );		VOK;
	R->pktstart( P_REMOVELINE );
	R->sendobj( _object );
	R->sendlong( k );
	R->pktend();
}
	
void Pad.createline(long lo, long hi)
{
	trace( "%d.createline(%d,%d)", this, lo, hi );		VOK;
	if( lo>hi ) return;
	R->pktstart( P_CREATELINE );
	R->sendobj( _object );
	R->sendlong( lo );
	R->sendlong( hi );
	R->pktend();
}
	
void Pad.createline(long k)
{
	createline(k, k);
}
	
void Pad.menu(Index ix)
{
	trace( "%d.menu(0x%X)", this, ix.sht() );	VOK;
	IF_LIVE( !_object ) return;
	R->pktstart( P_CARTE );
	R->sendobj( _object );
	R->sendshort( ix.sht() );
	R->pktend();
}

void Pad.menu(Menu &m)
{
	trace( "%d.menu(%d)", this, &m );	VOK;
	IF_LIVE( !&m ) return;
	menu(m.index());
}

void Pad.options(Attrib on, Attrib off)
{
	trace( "%d.options(0x%X,0x%X)", this, on, off );	VOK;
	_attributes |= on;
	_attributes &= ~off;
	R->pktstart( P_ATTRIBUTE );
	R->sendobj( _object );
	R->sendshort( _attributes );
	R->pktend();
}

void Pad.lines(long l)
{
	trace( "%d.lines(%d)", this, l );		VOK;
	IF_LIVE( l<0 ) return;
	R->pktstart( P_LINES );
	R->sendobj( _object );
	R->sendlong( _lines = l );
	R->pktend();
}	

void Pad.termop(Protocol p)
{
	trace( "%d.termop(%d)", this, p ); VOK;
	R->pktstart( p );
	R->sendobj( _object );
	R->pktend();
}

void Pad.insert(long k, PRINTF_ARGS)
	{ insert(k, (Attrib)0, (PadRcv*)0, *(Menu*)0, PRINTF_COPY); }

void Pad.insert(long k, Attrib a, PRINTF_ARGS)
	{ insert(k, a, (PadRcv*)0, *(Menu*)0, PRINTF_COPY); }

void Pad.insert(long k, Attrib a, PadRcv *o, Menu &m, PRINTF_ARGS)
	{ insert(k, a, o, &m ? m.index() : ZIndex, PRINTF_COPY); }

void Pad.insert(long k, Attrib a, PadRcv *o, Index ix, PRINTF_ARGS)
{
	Line l;
	char t[1024];

	trace( "%d.insert(%d,0x%X,%d,0x%X,%s)", this, k, a, o, ix.sht(), fmt );	VOK;
	sprintf( l.text = t, PRINTF_COPY );
	l.key = k ? k : UniqueKey();
	if( !o ) a &= ~ACCEPT_KBD;
	l.attributes = a;
	l.object = o;
	l.carte = o ? ix : ZIndex;
	insert(l);
}

static Line prev; /* = { 0, 0, 0, 0, {0,0} } - cfront bug */
void Pad.insert(Line &l)
{
	char buf[256];
	register char *from;
	register int to;
	static Pad *prevpad;

	trace("%d.insert(%d,%s,%d,%X)",this,l.key,l.text,l.object,l.attributes);VOK;
	if( l.object && !l.object->isvalid() )
		PadsWarn("Pad.insert: object is not a PadRcv");
	if( _lines>0 && (l.key<1 || l.key>_lines) ){
		PadsWarn("Pad.insert: key out of range: %d %s", l.key, l.text);
		return;
	}
	if( l.attributes&FLUSHLINE || l.key==prev.key )
		R->writesize = 0;
	for( from = l.text, to = 0; *from && to<250; ++from )
		buf[to++] = isprint(*from) || *from=='\t'  ? *from : ' ';
	buf[to] = '\0';
	if(         this == prevpad
	&&         l.key == prev.key+1
	&& l.carte.sht() == prev.carte.sht()
	&&  l.attributes == prev.attributes ){
		trace( "P_NEXTLINE %d", l.key );
		R->pktstart( P_NEXTLINE );
		R->sendobj( l.object );
	} else {
		R->pktstart( P_LINE );
		R->sendobj( _object );
		R->sendobj( l.object );
		R->sendlong( l.key );
		R->sendshort( l.carte.sht() );
		R->sendshort( l.attributes );
	}
	R->sendstring( buf );
	R->pktend();
	prev = l;
	prevpad = this;
}

Line.Line()
{
	trace( "%d.Line()", this ); VOK;
	object = 0;
	text = "";
	key = 0;
	attributes = 0;
	carte = 0;
}

long UniqueKey() { static long u; return u += 1024; }

void Pad.error( PRINTF_ARGS )
{
	trace( "%d.error(%s)", this, fmt );	VOK;
	if( errorkey ) removeline( errorkey );
	errorkey = 0;
	if( !fmt || !*fmt ) return;
	insert(errorkey = UniqueKey(), SELECTLINE, PRINTF_COPY);
}

void Pad.makegap(long k, long gap)
{
	trace( "%d.makegap(%d,%d)", this, k, gap ); VOK;
	R->pktstart( P_MAKEGAP );
	R->sendobj( _object );
	R->sendlong( k );
	R->sendlong( gap );
	R->pktend();
}
