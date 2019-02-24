#include <pads.pri>

static void Proto(p) { PutRemote(p); }

static void CheckProto( p ) { if( GetRemote() != p ) ProtoErr(); }

static long ShiftIn( bytes )
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

static void ShiftOut( bytes, shifter )
register bytes;
register long shifter;
{
	Proto( bytes );
	do { PutRemote( (char)(shifter>>( (--bytes)*8 )) ); } while( bytes );
}

void SendLong(x)  long  x; { ShiftOut( P_LONG,  (long) x ); }
void SendShort(x) short x; { ShiftOut( P_SHORT, (long) x ); }
void SendUChar(x) unsigned char x; { ShiftOut( P_UCHAR, (long) x ); }

char *RcvString( s0 )
char *s0;
{
	register char *s = s0;
	register unsigned char len;

	CheckProto( P_STRING );
	len = RcvUChar();
	if( !s0 ) s = s0 = new char [len+1];
	while( len-->0 ) *s++ = GetRemote();
	*s = '\0';
	return s0;
}

void SendString(s)
register char *s;
{
	int len;

	Proto( P_STRING );
	len =  strlen(s);
	if( len > 255 ) len = 255;
	SendUChar( len );
	while( len-- ) PutRemote(*s++); 
}
