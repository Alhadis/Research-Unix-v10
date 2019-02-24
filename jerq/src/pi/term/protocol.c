#include "term.h"
#include "../protocol.h"

#define REMOTEBUFFER 64
unsigned char RemoteBuffer[REMOTEBUFFER];
int RemoteIndex;

void ProtoErr() { assert(0); }

FlushRemote()
{
	if( RemoteIndex ){
		sendnchars( RemoteIndex, RemoteBuffer );
		RemoteIndex = 0;
	}
}

GetRemote()
{
	register int c;

	while( (c = rcvchar() ) == -1 ){
		cursswitch( &Coffee );
		wait( RCV );
		cursswitch( Cursor );
	}
	return c;
}

void PutRemote( c )
char c;
{
	if( RemoteIndex >= REMOTEBUFFER ) FlushRemote();
	RemoteBuffer[RemoteIndex++] = c;
}

void Proto(p)
{
	PutRemote(p);
}

void CheckProto( p )
{
	if( GetRemote() != p ) ProtoErr("");
}

long ShiftIn( bytes )
register bytes;
{
	register long shifter = 0;

	CheckProto( bytes );
	while( bytes-- ) shifter = (shifter<<8) + (GetRemote()&0xFF);
	return shifter;
}

long  RcvLong()  { return (long)  ShiftIn( P_LONG  ); }
short RcvShort() { return (short) ShiftIn( P_SHORT ); }
unsigned char RcvUChar() { return (unsigned char) ShiftIn( P_UCHAR ); }

void ShiftOut( bytes, shifter )
register bytes;
register long shifter;
{
	Proto( bytes );
	do PutRemote( (char)(shifter>>( (--bytes)*8 )) ); while( bytes );
}

void SendLong(x)  long  x; { ShiftOut( P_LONG,  (long) x ); }
void SendShort(x) short x; { ShiftOut( P_SHORT, (long) x ); }
void SendUChar(x) unsigned char x; { ShiftOut( P_UCHAR, (long) x ); }

char *RcvString( s )
register char *s;
{
	register unsigned char len;

	assert( (long) s );
	CheckProto( P_STRING );
	len = RcvUChar();
	while( len-->0 ) *s++ = GetRemote();
	*s = '\0';
	return s;
}

void SendString(s)
register char *s;
{
	register len = strlen(s);
	if( len>250) len = 250;
	Proto( P_STRING );
	SendUChar( len );
	while( len-->0 ) PutRemote(*s++); 
}

char *SendError(e)
char *e;
{
	return SendString( e ? e : "" );
}
