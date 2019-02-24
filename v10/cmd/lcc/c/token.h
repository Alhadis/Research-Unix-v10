/* C compiler: token data */

#ifndef yy
#define yy xx
#endif
#ifndef zz
#define zz xx
#endif
/*
xx - non-keyword symbols; yy - tokens w/o names; zz - keywords
xx(symbol,	value,	prec,	op,	opnode,	kind,	string)
*/
yy(0,		0,	0,	0,	0,	0,	0)
zz(FLOAT,	1,	0,	0,	0,	CHAR,	"float")
zz(DOUBLE,	2,	0,	0,	0,	CHAR,	"double")
zz(CHAR,	3,	0,	0,	0,	CHAR,	"char")
zz(SHORT,	4,	0,	0,	0,	CHAR,	"short")
zz(INT,		5,	0,	0,	0,	CHAR,	"int")
zz(UNSIGNED,	6,	0,	0,	0,	CHAR,	"unsigned")
xx(POINTER,	7,	0,	0,	0,	0,	0)
zz(VOID,	8,	0,	0,	0,	CHAR,	"void")
zz(STRUCT,	9,	0,	0,	0,	CHAR,	"struct")
zz(UNION,	10,	0,	0,	0,	CHAR,	"union")
xx(FUNCTION,	11,	0,	0,	0,	0,	0)
xx(ARRAY,	12,	0,	0,	0,	0,	0)
zz(ENUM,	13,	0,	0,	0,	CHAR,	"enum")
zz(LONG,	14,	0,	0,	0,	CHAR,	"long")
zz(CONST,	15,	0,	0,	0,	CHAR,	"const")
zz(VOLATILE,	16,	0,	0,	0,	CHAR,	"volatile")
yy(0,		17,	0,	0,	0,	0,	0)
yy(0,		18,	0,	0,	0,	0,	0)
yy(0,		19,	0,	0,	0,	0,	0)
yy(0,		20,	0,	0,	0,	0,	0)
yy(0,		21,	0,	0,	0,	0,	0)
yy(0,		22,	0,	0,	0,	0,	0)
yy(0,		23,	0,	0,	0,	0,	0)
yy(0,		24,	0,	0,	0,	0,	0)
yy(0,		25,	0,	0,	0,	0,	0)
yy(0,		26,	0,	0,	0,	0,	0)
yy(0,		27,	0,	0,	0,	0,	0)
yy(0,		28,	0,	0,	0,	0,	0)
yy(0,		29,	0,	0,	0,	0,	0)
yy(0,		30,	0,	0,	0,	0,	0)
yy(0,		31,	0,	0,	0,	0,	0)
xx(ID,		32,	0,	0,	0,	ID,	"identifier")
yy(0,		33,	0,	0,	0,	ID,	"!")
xx(FCON,	34,	0,	0,	0,	ID,	"floating constant")
xx(ICON,	35,	0,	0,	0,	ID,	"integer constant")
xx(SCON,	36,	0,	0,	0,	ID,	"string constant")
yy(0,		37,	13,	MOD,	bitnode,'%',	"%")
yy(0,		38,	8,	BAND,	bitnode,ID,	"&")
xx(INCR,	39,	0,	ADD,	addnode,ID,	"++")
yy(0,		40,	0,	0,	0,	ID,	"(")
yy(0,		41,	0,	0,	0,	')',	")")
yy(0,		42,	13,	MUL,	mulnode,ID,	"*")
yy(0,		43,	12,	ADD,	addnode,ID,	"+")
yy(0,		44,	1,	0,	0,	',',	",")
yy(0,		45,	12,	SUB,	subnode,ID,	"-")
yy(0,		46,	0,	0,	0,	'.',	".")
yy(0,		47,	13,	DIV,	mulnode,'/',	"/")
xx(DECR,	48,	0,	SUB,	subnode,ID,	"--")
xx(DEREF,	49,	0,	0,	0,	DEREF,	"->")
xx(ANDAND,	50,	5,	AND,	andnode,ANDAND,	"&&")
xx(OROR,	51,	4,	OR,	andnode,OROR,	"||")
xx(LEQ,		52,	10,	LE,	cmpnode,LEQ,	"<=")
xx(EQL,		53,	9,	EQ,	eqnode,	EQL,	"==")
xx(NEQ,		54,	9,	NE,	eqnode,	NEQ,	"!=")
xx(GEQ,		55,	10,	GE,	cmpnode,GEQ,	">=")
xx(RSHIFT,	56,	11,	RSH,	shnode,	RSHIFT,	">>")
xx(LSHIFT,	57,	11,	LSH,	shnode,	LSHIFT,	"<<")
yy(0,		58,	0,	0,	0,	':',	":")
yy(0,		59,	0,	0,	0,	IF,	";")
yy(0,		60,	10,	LT,	cmpnode,'<',	"<")
yy(0,		61,	2,	ASGN,	asgnnode,'=',	"=")
yy(0,		62,	10,	GT,	cmpnode,'>',	">")
yy(0,		63,	0,	0,	0,	'?',	"?")
xx(ELLIPSIS,	64,	0,	0,	0,	ELLIPSIS,"...")
zz(SIZEOF,	65,	0,	0,	0,	ID,	"sizeof")
yy(0,		66,	0,	0,	0,	0,	0)
zz(AUTO,	67,	0,	0,	0,	STATIC,	"auto")
zz(BREAK,	68,	0,	0,	0,	IF,	"break")
zz(CASE,	69,	0,	0,	0,	IF,	"case")
zz(CONTINUE,	70,	0,	0,	0,	IF,	"continue")
zz(DEFAULT,	71,	0,	0,	0,	IF,	"default")
zz(DO,		72,	0,	0,	0,	IF,	"do")
zz(ELSE,	73,	0,	0,	0,	IF,	"else")
zz(EXTERN,	74,	0,	0,	0,	STATIC,	"extern")
zz(FOR,		75,	0,	0,	0,	IF,	"for")
zz(GOTO,	76,	0,	0,	0,	IF,	"goto")
zz(IF,		77,	0,	0,	0,	IF,	"if")
zz(REGISTER,	78,	0,	0,	0,	STATIC,	"register")
zz(RETURN,	79,	0,	0,	0,	IF,	"return")
zz(SIGNED,	80,	0,	0,	0,	CHAR,	"signed")
zz(STATIC,	81,	0,	0,	0,	STATIC,	"static")
zz(SWITCH,	82,	0,	0,	0,	IF,	"switch")
zz(TYPEDEF,	83,	0,	0,	0,	STATIC,	"typedef")
zz(WHILE,	84,	0,	0,	0,	IF,	"while")
yy(0,		85,	0,	0,	0,	0,	0)
yy(0,		86,	0,	0,	0,	0,	0)
yy(0,		87,	0,	0,	0,	0,	0)
yy(0,		88,	0,	0,	0,	0,	0)
yy(0,		89,	0,	0,	0,	0,	0)
yy(0,		90,	0,	0,	0,	0,	0)
yy(0,		91,	0,	0,	0,	'[',	"[")
yy(0,		92,	0,	0,	0,	0,	0)
yy(0,		93,	0,	0,	0,	']',	"]")
yy(0,		94,	7,	BXOR,	bitnode,'^',	"^")
yy(0,		95,	0,	0,	0,	0,	0)
yy(0,		96,	0,	0,	0,	0,	0)
yy(0,		97,	0,	0,	0,	0,	0)
yy(0,		98,	0,	0,	0,	0,	0)
yy(0,		99,	0,	0,	0,	0,	0)
yy(0,		100,	0,	0,	0,	0,	0)
yy(0,		101,	0,	0,	0,	0,	0)
yy(0,		102,	0,	0,	0,	0,	0)
yy(0,		103,	0,	0,	0,	0,	0)
yy(0,		104,	0,	0,	0,	0,	0)
yy(0,		105,	0,	0,	0,	0,	0)
yy(0,		106,	0,	0,	0,	0,	0)
yy(0,		107,	0,	0,	0,	0,	0)
yy(0,		108,	0,	0,	0,	0,	0)
yy(0,		109,	0,	0,	0,	0,	0)
yy(0,		110,	0,	0,	0,	0,	0)
yy(0,		111,	0,	0,	0,	0,	0)
yy(0,		112,	0,	0,	0,	0,	0)
yy(0,		113,	0,	0,	0,	0,	0)
yy(0,		114,	0,	0,	0,	0,	0)
yy(0,		115,	0,	0,	0,	0,	0)
yy(0,		116,	0,	0,	0,	0,	0)
yy(0,		117,	0,	0,	0,	0,	0)
yy(0,		118,	0,	0,	0,	0,	0)
yy(0,		119,	0,	0,	0,	0,	0)
yy(0,		120,	0,	0,	0,	0,	0)
yy(0,		121,	0,	0,	0,	0,	0)
yy(0,		122,	0,	0,	0,	0,	0)
yy(0,		123,	0,	0,	0,	IF,	"{")
yy(0,		124,	6,	BOR,	bitnode,'|',	"|")
yy(0,		125,	0,	0,	0,	'}',	"}")
yy(0,		126,	0,	BCOM,	0,	ID,	"~")
xx(EOI,		127,	0,	0,	0,	EOI,	"end of input")
#undef xx
#undef yy
#undef zz

#ifndef __STDC__
#ifndef FLOAT
#define FLOAT	1 
#define DOUBLE	2 
#define CHAR	3 
#define SHORT	4 
#define INT	5 
#define UNSIGNED	6 
#define POINTER	7 
#define VOID	8 
#define STRUCT	9 
#define UNION	10 
#define FUNCTION	11 
#define ARRAY	12 
#define ENUM	13 
#define LONG	14 
#define CONST	15 
#define VOLATILE	16 
#define ID	32 
#define FCON	34 
#define ICON	35 
#define SCON	36 
#define INCR	39 
#define DECR	48 
#define DEREF	49 
#define ANDAND	50 
#define OROR	51 
#define LEQ	52 
#define EQL	53 
#define NEQ	54 
#define GEQ	55 
#define RSHIFT	56 
#define LSHIFT	57 
#define ELLIPSIS	64 
#define SIZEOF	65 
#define ASM	66 
#define AUTO	67 
#define BREAK	68 
#define CASE	69 
#define CONTINUE	70 
#define DEFAULT	71 
#define DO	72 
#define ELSE	73 
#define EXTERN	74 
#define FOR	75 
#define GOTO	76 
#define IF	77 
#define REGISTER	78 
#define RETURN	79 
#define SIGNED	80 
#define STATIC	81 
#define SWITCH	82 
#define TYPEDEF	83 
#define WHILE	84 
#define EOI	127 
#endif
#endif
