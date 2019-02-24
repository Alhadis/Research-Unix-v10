#include <pads.pri>
#include <sys/types.h>
#include <sgtty.h>

void Remote.checkproto(int p)		{ if( get()!=p ) err();			}
void Remote.put(char c)			{ writebuffer[pktbase + pktsize++] = c; }
void Remote.proto(int p)		{ put( p ); }

long Remote.rcvlong()			{ return (long)  shiftin( P_LONG  );	}
short Remote.rcvshort()			{ return (short) shiftin( P_SHORT );	}
unsigned char Remote.rcvuchar()		{ return (unsigned char) shiftin(P_UCHAR); }

void Remote.sendlong(long  x)		{ shiftout( P_LONG, x );	 }
void Remote.sendshort(short x)		{ shiftout( P_SHORT, (long) x ); }
void Remote.senduchar(unsigned char x)	{ shiftout( P_UCHAR, (long) x ); }

void Remote.pktstart(char c)		{ if( pktsize ) err(); put(c);	}
void Remote.pktflush()			{ writesize = 0; pktend(); 	}

#define ADDRBITS  24
#define ALIGNBITS 2
#define OIDBITS   (32-(ADDRBITS-ALIGNBITS))
#define MASK(n)	 ((1<<n)-1)
void Remote.sendobj(PadRcv *o)
{
	if( !o ){
		sendlong(0);
		return;
	}
	long l = (long) o;
	if( l & (~MASK(ADDRBITS)^MASK(ALIGNBITS)) ) abort();
	sendlong( (l<<(OIDBITS-ALIGNBITS)) | (o->oid&MASK(OIDBITS)) );
}

PadRcv *Remote.rcvobj()
{
	register long o = rcvlong(), a;
	a = o >> (OIDBITS-ALIGNBITS);
	a &= MASK(ADDRBITS)&~MASK(ALIGNBITS);
	PadRcv *obj = (PadRcv*) a;
	if( obj && (obj->oid&MASK(OIDBITS)) != (o&MASK(OIDBITS)) )
		obj = 0;
	return obj;
}

void Remote.err(char *e)
{
	if( !e ) e = "host/term protocol error";
	write(fd, e, strlen(e));
}

Remote.Remote(int opened)
{
	fd = opened;
	pktsize = writesize = pktbase = 0;
}

Remote.Remote(char *dev)
{
	struct sgttyb tty;

	fd = open(dev, 2);
	pktsize = writesize = pktbase = 0;

	trace( "%d.Remote(%d,%d)", this, in, out );
	if( ioctl(fd, TIOCGETP, &tty) ) err("tty ioctl: GETP failed");
	tty.sg_flags = (tty.sg_flags|CBREAK|RAW) & ~ECHO;
	if( ioctl(fd, TIOCSETP, &tty) ) err("tty ioctl: SETP failed");
	trace( "sg_flags=0x%X", tty.sg_flags );
	if( ioctl(fd, TIOCEXCL, 0)  ) err("tty ioctl: EXCL failed");
}

void Remote.share()
{
	trace( "%d.share()", this, );
	if( ioctl(fd, TIOCNXCL, 0)  ) err("tty ioctl: NXCL failed");
}	

long Remote.shiftin(register int bytes)
{
	register long shifter = 0;

	checkproto( bytes );
	while( bytes-- ) shifter = (shifter<<8) + (get()&0xFF);
	return shifter;
}

void Remote.shiftout( register bytes, register long shifter )
{
	proto( bytes );
	do { put( (char)(shifter>>( (--bytes)*8 )) ); } while( bytes );
}

long BytesToTerm;
void Remote.pktend()
{
	if( pktsize<=0 ) err();
	pktbase += pktsize;
	if( pktbase > writesize ){
		if( write(fd, writebuffer, pktbase) != pktbase ) abort();
		BytesToTerm += pktbase;
		pktbase = 0;
		writesize = 50;
	}
	pktsize = 0;
}

char *Remote.rcvstring( char *s0 )
{
	register char *s = s0;
	register unsigned char len;

	checkproto( P_STRING );
	len = rcvuchar();
	if( !s0 ) s = s0 = new char [len+1];
	while( len-->0 ) *s++ = get();
	*s = '\0';
	return s0;
}

void Remote.sendstring(register char *s)
{
	int len;

	proto( P_STRING );
	len =  strlen(s);
	if( len > 255 ) len = 255;
	senduchar( len );
	while( len-- ) put(*s++); 
}

long BytesFromTerm;
int Remote.get()
{
	char c;
	if( pktsize!=0 || read(fd, &c, 1)!=1 ) err();
	++BytesFromTerm;
	return c;
}
