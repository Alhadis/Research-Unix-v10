#include "univ.h"
#include "format.pri"
#include <ctype.h>
#include "symtab.pub"
#include "symbol.h"
SRCFILE("format.c")

char *Bls.af(PRINTF_ARGS)
{
	char x[1024], *q = x;

	trace( "%d.af(%s ...) %s", this, fmt, text );
	IF_LIVE(!this) return "Bls.af";
	if( p<text+TBLS && fmt ){
		sprintf(x, PRINTF_COPY);
		while( *q && p<text+TBLS ) *p++ = *q++;
		*p = 0;
	}
	return text;
}

Bls.Bls(PRINTF_ARGS)
{
	clear();
	af(PRINTF_COPY);
}

Bls.Bls(Bls &b)
{
	clear();
	af("%s", b.text);
}

Format.Format(long f, SymTab *s)	{ format = f; stab = s; }

char *FmtByte(int c)
{	static char buf[8];

	switch( c &= 0xFF ){
	case '\0'	: return "\\0";	
	case '\b'	: return "\\b";	
	case '\f'	: return "\\f";	
	case '\n'	: return "\\n";	
	case '\r'	: return "\\r";	
	case '\t'	: return "\\t";	
	case '\v'	: return "\\v";	
	case ' '	: return " ";		/* see ikeya!<ctype.h> */	
	case '\''	: return "\\\'";
	case '\"'	: return "\\\"";
	case '\\'	: return "\\\\";
	}
	sprintf( buf, isascii(c)&&isprint(c)?"%c":"\\%03o", c );
 	return buf;
}

char *FmtAscii(unsigned long c)
{
	static char buf[32];

	if( c == 0 ) return "0";
	strcpy(buf, "'");
	int lead = 1; int byte = 4;			// cfront bug
	for( ; byte; --byte, c<<=8 ){
		if( (c&0xFF000000) || !lead ){
			strcat(buf, FmtByte(c>>24));
			lead = 0;
		}
	}
	return strcat(buf, "'");
}

void Format.grow(char *b)
{
	accum.af("%s%s", sep, b);
	sep = "=";
}

void Format.growtime(long t)
{
	char *time(long*);
	grow(ctime(&t));
}
	
void Format.grow(char *fmt, long l)
{
	char buf[32];

	sprintf(buf, fmt, l);
	grow(buf);
}

void Format.grow(char *fmt, double d)
{
	char buf[32];

	trace( "%d.grow(%s,%g)", this, fmt, d );
	sprintf(buf, fmt, d);
	if( fmt[1]=='g' && !strcmp(buf, "0") )
		grow("0.0");
	else
		grow(buf);
}

char *Format.f(long lng, double dbl)
{
	Symbol *s;

	if( !this ) return "Format.f";

	accum.clear();
	sep = "";

	if( !format ) format = F_HEX;

	if( format&F_MASK8  ) lng &= 0xFF;
	if( format&F_MASK16 ) lng &= 0xFFFF;

	if( format&F_FLOAT  ) grow("%g",      dbl);
	if( format&F_DOUBLE ) grow("%g",      dbl);
	if( format&F_DBLHEX ) grow("0x%X_%X", dbl);

	if( format&F_SYMBOLIC
	 && lng
	 && stab
	 && (s = stab->loctosym(SSet(U_FUNC,U_GLB,U_STA,U_STMT), lng)) ){
		lng -= s->range.lo;
		grow(s->text());
		if( !lng )
			format = 0;
		else {
			sep =  "+";
			if( !(format&F_HOAD) ) format |= F_HEX;
		}
	}

	if( format&F_TIME ) growtime(lng);
	if( (format&F_HOAD) && lng>=0 && lng<=7 ) grow("%d", lng);
	else {
		if( format&F_OCTAL   ) grow("0%o",  lng);
		if( format&F_DECIMAL ) grow("%u",   lng);
		if( format&F_HEX     ) grow("0x%X", lng);
		if( format&F_ASCII   ) grow(FmtAscii(lng));

		if( format&F_EXT8 ) lng = (char ) lng;
		if( format&F_EXT16) lng = (short) lng;

		if( format&F_SIGNED
		 && (lng<0 || !(format&F_DECIMAL)) ) grow("%d", lng);
	}
	return accum.text;
}
