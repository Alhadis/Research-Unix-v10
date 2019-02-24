#include "univ.h"
#define REMOTEBUFFER 64
unsigned char RemoteBuffer[REMOTEBUFFER];
int RemoteIndex;

FlushRemote()
{
	if( RemoteIndex ){
		sendnchars(RemoteIndex, RemoteBuffer);
		RemoteIndex = 0;
	}
}

GetRemote()
{
	register int c;

	while( (c = rcvchar() ) == -1 ){
		cursswitch(&Coffee);
		wait(RCV);
		cursswitch(Cursor);
	}
	return c;
}

void PutRemote( c )
char c;
{
	if( RemoteIndex >= REMOTEBUFFER ) FlushRemote();
	RemoteBuffer[RemoteIndex++] = c;
}

ToHost( p, s )
Protocol p;
short s;
{
	extern Pad *Current;

	PutRemote( p );
	SendLong( HostParent );
	SendLong( HostObject );
	SendShort( s );
}
	
HostAction( i )
Index *i;			/* <text:action:opand> stored at host	*/
{
	ToHost( P_ACTION, *i );
}

HostNumeric( n )
long n;				/* always in short range ! */
{
	ToHost( P_NUMERIC, (short) n );
}

RCVServe()	/* poll the host */ 
{
	register Texture *t = Cursor;
	register int op;

	FlushRemote();
Again:
	if( P->nchars ){
		op = GetRemote();
		switch( op&0xF0 ){
		case (int)P_VERSION&0xF0:
			if( RcvLong() != PADS_VERSION )
			   ProtoErr( "host/term versions differ - regenerate. " );
			break;
		case (int)P_PICK&0xF0:
			PickOp();
			break;
		case (int)P_HOSTSTATE&0xF0:
			t = (op==(int)P_BUSY) ? &HostBusy : (Texture*)0;
			goto Again;
		case (int)P_PADDEF&0xF0:
		case (int)P_PADOP&0xF0:	
			PadOp(op);
			break;
		case (int)P_CACHEOP&0xF0:
			CacheOp(op);
			break;
		case (int)P_HELPSTR&0xF0:
			HelpString();
			break;
		default:
			ProtoErr( "RCVServe(): " );
		}
	}
	if( t != Cursor ) cursswitch( Cursor = t );
}

HelpString()
{
	char s[256];

	RcvString(s);
	InvertKBDrect(s,"");
}

ProtoErr(s)
register char *s;
{
	extern char KBDStr[];
	register char *k = KBDStr, *p = "protocol: ";

	while( *s ) *k++ = *s++;
	while( *p ) *k++ = *p++;
	for(;;){
		PaintKBD();
		if( k<&KBDStr[64] ) *k++ = GetRemote()&0x7F;
		sleep(30);
	}
}
