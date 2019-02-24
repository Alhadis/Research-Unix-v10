#include "dtype.pri"
#include <a.out.h>
#include "symbol.h"
#include "format.pub"
#include "mip.h"
#include "phrase.pub"
#include "expr.pub"
SRCFILE("dtype.c")

char *BTName(int t)
{
	switch( BTYPE(t) ){
	case FARG:	return "farg";
	case CHAR:	return "char";
	case SHORT:	return "short";
	case INT:	return "int";
	case LONG:	return "long";
	case FLOAT:	return "float";
	case DOUBLE:	return "double";
	case STRTY:	return "struct";
	case UNIONTY:	return "union";
	case ENUMTY:	return "enum";
	case UCHAR:	return "uchar";
	case USHORT:	return "ushort";
	case UNSIGNED:	return "unsigned";
	case ULONG:	return "ulong";
	case VOID:	return "void";
	case UBITS:	return "ubits";
	case BITS:	return "bits";
	default:	return Name("BTName=%d",BTYPE(t));
	}
}

char *PccName(int t)
{
	static char name[64];

	strcpy( name, "" );
	for( ;; t = DECREF(t) ){
		if( ISPTR(t) ) { strcat( name, "*"  ); continue; }
		if( ISARY(t) ) { strcat( name, "[]" ); continue; }
		if( ISFTN(t) ) { strcat( name, "()" ); continue; }
		break;
	}
	return strcat( name, BTName(t) );
}

char *FmtName(int f)
{
	if( f&F_TURNOFF )
		switch( f&~F_TURNOFF ){
		case F_HEX:
		case F_DBLHEX:		return "hex      off";
		case F_DECIMAL:		return "unsd_dec off";
		case F_SIGNED:		return "sign_dec off";
		case F_OCTAL:		return "octal    off";
		case F_ASCII:		return "ascii    off";
		case F_FLOAT:		return "float    off";
		case F_DOUBLE:		return "double   off";
		case F_SHORTSTRING:	return "\"ascii\"  off";
		case F_LONGSTRING:	return "\"wider\"  off";
		case F_ENUM:
		case F_SYMBOLIC:	return "symbolic off";
		case F_TIME:		return "time     off";
	}
	switch(f){
	case F_HEX:
	case F_DBLHEX:		return "hex      on ";
	case F_DECIMAL:		return "unsd_dec on ";
	case F_SIGNED:		return "sign_dec on ";
	case F_OCTAL:		return "octal    on ";
	case F_ASCII:		return "ascii    on ";
	case F_FLOAT:		return "float    on ";
	case F_DOUBLE:		return "double   on ";
	case F_SHORTSTRING:	return "\"ascii\"  on ";
	case F_LONGSTRING:	return "\"wider\"  on ";
	case F_ENUM:
	case F_SYMBOLIC:	return "symbolic on ";
	case F_TIME:		return "time     on ";

	default:		return " ";	// ???
	}
}

DType.DType()
{
	univ = 0;
	pcc = UNDEF;
	dim = 0;
	over = 0;
}

void DType.free()
{
	if( isary() || isftn() || isptr() ){
		ref()->free();
		delete ref();
	}
}

int DType.isary() 		{ return ISARY(pcc); }
int DType.isptr()		{ return ISPTR(pcc); }
int DType.isaryorptr() 		{ return ISARY(pcc) || ISPTR(pcc); }
int DType.isftn() 		{ return ISFTN(pcc); }
int DType.isstrun() 		{ return pcc==STRTY || pcc==UNIONTY; }
UType *DType.utype()		{ return (UType*) univ; }
DType *DType.ref()		{ return (DType* )univ; }

int DType.isintegral()
{
	switch( pcc ){
		case UCHAR:
		case USHORT:
		case UNSIGNED:
		case ULONG:
		case CHAR:
		case SHORT:
		case INT:
		case LONG:
		case BITS:
		case UBITS:
		case ENUMTY:	return 1;
	}
	return 0;
}

int DType.isreal()		{ return pcc==FLOAT || pcc==DOUBLE; }

int DType.isscalar()	{ return isptr() || isintegral() || isreal(); }

DType DType.incref()
{
	DType d;

	trace( "%d.incref()", this );
	d.univ = this;
	d.pcc = PTR;
	return d;
}

DType *DType.decref()
{
	trace( "%d.decref()", this );	OK(0);
	return this ? ref(): 0;
}

char *DType.text()
{
	OK("DType.text");
	if( isftn() ) return sf( "()%s", ref()->text() );
	if( isptr() ) return sf( "*%s", ref()->text() );
	if( isary() ) return sf( "[%d]%s", dim, ref()->text() );
	if( !utype() ) return BTName(pcc);
	return sf( "%s %s", BTName(pcc), utype()->_text );
}

int DType.format()
{
	trace( "%d.format() 0x%X %s", this, over, text() );	OK(F_HEX);	
	if( over ) return over;
	if( isptr() && decref()->isftn() ) return over=F_HEX|F_SYMBOLIC;
	if( isptr() && (decref()->pcc==CHAR || decref()->pcc==UCHAR))
		return over=F_SHORTSTRING|F_HEX;
	if( isary() && (decref()->pcc==CHAR || decref()->pcc==UCHAR) )
		return over=F_SHORTSTRING|F_HEX|F_ARY;
	if( isary() ) return over = F_ARY|F_HEX;
	if( isptr() || isftn() ) return over = F_HEX;
	switch( pcc ){
	case CHAR:	return over = F_MASK8|F_ASCII|F_EXT8|F_SIGNED;
	case SHORT:	return over = F_MASK16|F_EXT16|F_SIGNED;
	case BITS:
	case INT:
	case LONG:	return over = F_SIGNED;
	case FLOAT:	return over = F_FLOAT;
	case DOUBLE:	return over = F_DOUBLE;
	case UCHAR:	return over = F_MASK8|F_ASCII|F_DECIMAL;
	case USHORT:	return over = F_MASK16|F_DECIMAL;
	case UBITS:
	case UNSIGNED:
	case ULONG:	return over = F_DECIMAL;

	case ENUMTY:	return over = F_ENUM;
	case STRTY:
	case UNIONTY:	return over = F_UTYPE;
	case VOID:	return over = 0;

	default:	IF_LIVE(1) return F_HEX;
	}
}

void DType.reformat(int o, int stars)
{
	trace("%d.reformat(0x%X,%d) 0x%X %s", this, o, stars, over, text()); VOK;
	if( stars > 0 ) decref()->reformat(o,stars-1);
	else {
		format();
		if( o&F_TURNOFF )
			over &= ~o;
		else
			over |= o;
		if( (over&F_STRING) == F_STRING )
			over = (over&~F_STRING) | o;
	}
}

int DType.size_of()
{
	OK(0);
	if( isptr() ) return 4;					//
	if( isftn() ) return 1;					// &_malloc+2 etc
	if( isary() ) return dim * decref()->size_of();
	switch( BTYPE(pcc) ){
		case UCHAR:
		case CHAR:	return 1;
		case USHORT:
		case SHORT:	return 2;
		case UNSIGNED:
		case ENUMTY:					// wrong
		case BITS:
		case UBITS:					// ???
		case INT:	return 4;			// wrong
		case ULONG:
		case LONG:	return 4;
		case FLOAT:	return 4;			// ???
		case DOUBLE:	return 8;			// ???
		case STRTY:
		case UNIONTY:	return utype() ? utype()->range.lo: 0;
		case VOID:	return 0;
	}
	IF_LIVE(1) {};
	return 0;
}

int DType.formatset()
{
	OK(F_HEX);
	if( isaryorptr() && (decref()->pcc==CHAR || decref()->pcc==UCHAR))
		return F_SYMBOLIC|F_STRING|F_SIGNED|F_HOAD|F_FLOAT;
	if( isptr() || isftn() || isary() )
		return F_SYMBOLIC|F_SIGNED|F_HOAD|F_FLOAT;
	switch( BTYPE(pcc) ){
	case UCHAR:
	case CHAR:
	case USHORT:
	case SHORT:	return F_ASCII|F_HEX|F_OCTAL|F_SIGNED|F_DECIMAL;
	case UNSIGNED:
	case INT:
	case ULONG:
	case LONG:
	case BITS:
	case UBITS:
	case FLOAT: return F_ASCII|F_HEX|F_OCTAL|F_SIGNED|F_DECIMAL|F_FLOAT|F_TIME;
	case DOUBLE:	return F_DOUBLE|F_DBLHEX;

	case STRTY:
	case UNIONTY:
	case VOID:	return 0;

	case ENUMTY: return F_ENUM|F_ASCII|F_HEX|F_OCTAL|F_SIGNED|F_DECIMAL|F_FLOAT;

	default:	IF_LIVE(1) return F_HEX;
	}
}

Index DType.carte()
{
	int f = formatset(), bit;
	Menu m;
 
	OK(ZIndex);
	if( pcc == VOID ) return ZIndex;
	m.last( "typeof $", (Action)&Phrase::applyunary, (long)O_TYPEOF );
	m.last( "sizeof $", (Action)&Phrase::applyunary, (long)O_SIZEOF );
	if( isptr() )
		m.last( "* $", (Action)&Phrase::applyunary, (long)O_DEREF );
	if( isptr() || isary() )
		m.last( "$ []", (Action)&Phrase::applyunary, (long)O_QINDEX );
	if( isptr() && decref() && decref()->utype()
	 && (decref()->pcc==STRTY || decref()->pcc==UNIONTY) )
		m.last( decref()->utype()->carte(O_ARROW) );
	if( (pcc == STRTY || pcc == UNIONTY) && utype() )
		m.last( utype()->carte(O_DOT) );
	if( f ){
		Menu s;
		for( bit = 1; bit; bit <<= 1 ) if( bit & f ){
			long b = bit;
			if( b&over ) b |= F_TURNOFF;
	    		s.last(FmtName(b), (Action)&Phrase::reformat, (long)b);
		}
		m.last(s.index("format"));
	}
	return m.index();
}
