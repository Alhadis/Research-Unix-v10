#ifndef generic
#define generic(op) ((op)&~15)
#define opindex(op) ((op)>>4)
#define optype(op) ((op)&15)
#define isaddrop(op) ((op)==ADDRG+P||(op)==ADDRL+P||(op)==ADDRF+P)

#define F	1	/* float */
#define D	2	/* double */
#define C	3	/* char */
#define S	4	/* short */
#define I	5	/* int */
#define U	6	/* unsigned */
#define P	7	/* void* */
#define V	8	/* void */
#define B	9	/* struct */
#define TYPENAMES " FDCSIUPVB"

#ifdef __STDC__
typedef enum opcode {
	CNST=1<<4,
	CNSTC=(1<<4)+C,
	CNSTD=(1<<4)+D,
	CNSTF=(1<<4)+F,
	CNSTI=(1<<4)+I,
	CNSTP=(1<<4)+P,
	CNSTS=(1<<4)+S,
	CNSTU=(1<<4)+U,
	ARG=2<<4,
	ARGB=(2<<4)+B,
	ARGD=(2<<4)+D,
	ARGF=(2<<4)+F,
	ARGI=(2<<4)+I,
	ARGP=(2<<4)+P,
	ASGN=3<<4,
	ASGNB=(3<<4)+B,
	ASGNC=(3<<4)+C,
	ASGND=(3<<4)+D,
	ASGNF=(3<<4)+F,
	ASGNI=(3<<4)+I,
	ASGNS=(3<<4)+S,
	ASGNP=(3<<4)+P,
	INDIR=4<<4,
	INDIRB=(4<<4)+B,
	INDIRC=(4<<4)+C,
	INDIRD=(4<<4)+D,
	INDIRF=(4<<4)+F,
	INDIRI=(4<<4)+I,
	INDIRS=(4<<4)+S,
	INDIRP=(4<<4)+P,
	CVC=5<<4,
	CVCI=(5<<4)+I,
	CVCU=(5<<4)+U,
	CVD=6<<4,
	CVDF=(6<<4)+F,
	CVDI=(6<<4)+I,
	CVF=7<<4,
	CVFD=(7<<4)+D,
	CVI=8<<4,
	CVIC=(8<<4)+C,
	CVID=(8<<4)+D,
	CVIS=(8<<4)+S,
	CVIU=(8<<4)+U,
	CVP=9<<4,
	CVPU=(9<<4)+U,
	CVS=10<<4,
	CVSI=(10<<4)+I,
	CVSU=(10<<4)+U,
	CVU=11<<4,
	CVUC=(11<<4)+C,
	CVUI=(11<<4)+I,
	CVUP=(11<<4)+P,
	CVUS=(11<<4)+S,
	NEG=12<<4,
	NEGD=(12<<4)+D,
	NEGF=(12<<4)+F,
	NEGI=(12<<4)+I,
	CALL=13<<4,
	CALLB=(13<<4)+B,
	CALLD=(13<<4)+D,
	CALLF=(13<<4)+F,
	CALLI=(13<<4)+I,
	CALLV=(13<<4)+V,
	LOAD=14<<4,
	LOADB=(14<<4)+B,
	LOADC=(14<<4)+C,
	LOADD=(14<<4)+D,
	LOADF=(14<<4)+F,
	LOADI=(14<<4)+I,
	LOADP=(14<<4)+P,
	LOADS=(14<<4)+S,
	LOADU=(14<<4)+U,
	RET=15<<4,
	RETD=(15<<4)+D,
	RETF=(15<<4)+F,
	RETI=(15<<4)+I,
	RETV=(15<<4)+V,
	ADDRG=16<<4,
	ADDRGP=(16<<4)+P,
	ADDRF=17<<4,
	ADDRFP=(17<<4)+P,
	ADDRL=18<<4,
	ADDRLP=(18<<4)+P,
	ADD=19<<4,
	ADDD=(19<<4)+D,
	ADDF=(19<<4)+F,
	ADDI=(19<<4)+I,
	ADDP=(19<<4)+P,
	ADDU=(19<<4)+U,
	SUB=20<<4,
	SUBD=(20<<4)+D,
	SUBF=(20<<4)+F,
	SUBI=(20<<4)+I,
	SUBP=(20<<4)+P,
	SUBU=(20<<4)+U,
	LSH=21<<4,
	LSHI=(21<<4)+I,
	LSHU=(21<<4)+U,
	MOD=22<<4,
	MODI=(22<<4)+I,
	MODU=(22<<4)+U,
	RSH=23<<4,
	RSHI=(23<<4)+I,
	RSHU=(23<<4)+U,
	BAND=24<<4,
	BANDU=(24<<4)+U,
	BCOM=25<<4,
	BCOMU=(25<<4)+U,
	BOR=26<<4,
	BORU=(26<<4)+U,
	BXOR=27<<4,
	BXORU=(27<<4)+U,
	DIV=28<<4,
	DIVD=(28<<4)+D,
	DIVF=(28<<4)+F,
	DIVI=(28<<4)+I,
	DIVU=(28<<4)+U,
	MUL=29<<4,
	MULD=(29<<4)+D,
	MULF=(29<<4)+F,
	MULI=(29<<4)+I,
	MULU=(29<<4)+U,
	EQ=30<<4,
	EQD=(30<<4)+D,
	EQF=(30<<4)+F,
	EQI=(30<<4)+I,
	GE=31<<4,
	GED=(31<<4)+D,
	GEF=(31<<4)+F,
	GEI=(31<<4)+I,
	GEU=(31<<4)+U,
	GT=32<<4,
	GTD=(32<<4)+D,
	GTF=(32<<4)+F,
	GTI=(32<<4)+I,
	GTU=(32<<4)+U,
	LE=33<<4,
	LED=(33<<4)+D,
	LEF=(33<<4)+F,
	LEI=(33<<4)+I,
	LEU=(33<<4)+U,
	LT=34<<4,
	LTD=(34<<4)+D,
	LTF=(34<<4)+F,
	LTI=(34<<4)+I,
	LTU=(34<<4)+U,
	NE=35<<4,
	NED=(35<<4)+D,
	NEF=(35<<4)+F,
	NEI=(35<<4)+I,
	JUMP=36<<4,
	JUMPV=(36<<4)+V,
	LABEL=37<<4,
	LABELV=(37<<4)+V,
	MAXOP=38<<4,
	/* additional tree operators: */
	AND=MAXOP,
	NOT=MAXOP+1*16,
	OR=MAXOP+2*16,
	COND=MAXOP+3*16,
	RIGHT=MAXOP+4*16,
	FIELD=MAXOP+5*16
} Opcode;
#else
typedef int Opcode;
#define CNST	(1<<4)
#define CNSTC	((1<<4)+C)
#define CNSTD	((1<<4)+D)
#define CNSTF	((1<<4)+F)
#define CNSTI	((1<<4)+I)
#define CNSTP	((1<<4)+P)
#define CNSTS	((1<<4)+S)
#define CNSTU	((1<<4)+U)
#define ARG	(2<<4)
#define ARGB	((2<<4)+B)
#define ARGD	((2<<4)+D)
#define ARGF	((2<<4)+F)
#define ARGI	((2<<4)+I)
#define ARGP	((2<<4)+P)
#define ASGN	(3<<4)
#define ASGNB	((3<<4)+B)
#define ASGNC	((3<<4)+C)
#define ASGND	((3<<4)+D)
#define ASGNF	((3<<4)+F)
#define ASGNI	((3<<4)+I)
#define ASGNS	((3<<4)+S)
#define ASGNP	((3<<4)+P)
#define INDIR	(4<<4)
#define INDIRB	((4<<4)+B)
#define INDIRC	((4<<4)+C)
#define INDIRD	((4<<4)+D)
#define INDIRF	((4<<4)+F)
#define INDIRI	((4<<4)+I)
#define INDIRS	((4<<4)+S)
#define INDIRP	((4<<4)+P)
#define CVC	(5<<4)
#define CVCI	((5<<4)+I)
#define CVCU	((5<<4)+U)
#define CVD	(6<<4)
#define CVDF	((6<<4)+F)
#define CVDI	((6<<4)+I)
#define CVF	(7<<4)
#define CVFD	((7<<4)+D)
#define CVI	(8<<4)
#define CVIC	((8<<4)+C)
#define CVID	((8<<4)+D)
#define CVIS	((8<<4)+S)
#define CVIU	((8<<4)+U)
#define CVP	(9<<4)
#define CVPU	((9<<4)+U)
#define CVS	(10<<4)
#define CVSI	((10<<4)+I)
#define CVSU	((10<<4)+U)
#define CVU	(11<<4)
#define CVUC	((11<<4)+C)
#define CVUI	((11<<4)+I)
#define CVUP	((11<<4)+P)
#define CVUS	((11<<4)+S)
#define NEG	(12<<4)
#define NEGD	((12<<4)+D)
#define NEGF	((12<<4)+F)
#define NEGI	((12<<4)+I)
#define CALL	(13<<4)
#define CALLB	((13<<4)+B)
#define CALLD	((13<<4)+D)
#define CALLF	((13<<4)+F)
#define CALLI	((13<<4)+I)
#define CALLV	((13<<4)+V)
#define LOAD	(14<<4)
#define LOADB	((14<<4)+B)
#define LOADC	((14<<4)+C)
#define LOADD	((14<<4)+D)
#define LOADF	((14<<4)+F)
#define LOADI	((14<<4)+I)
#define LOADP	((14<<4)+P)
#define LOADS	((14<<4)+S)
#define LOADU	((14<<4)+U)
#define RET	(15<<4)
#define RETD	((15<<4)+D)
#define RETF	((15<<4)+F)
#define RETI	((15<<4)+I)
#define RETV	((15<<4)+V)
#define ADDRG	(16<<4)
#define ADDRGP	((16<<4)+P)
#define ADDRF	(17<<4)
#define ADDRFP	((17<<4)+P)
#define ADDRL	(18<<4)
#define ADDRLP	((18<<4)+P)
#define ADD	(19<<4)
#define ADDD	((19<<4)+D)
#define ADDF	((19<<4)+F)
#define ADDI	((19<<4)+I)
#define ADDP	((19<<4)+P)
#define ADDU	((19<<4)+U)
#define SUB	(20<<4)
#define SUBD	((20<<4)+D)
#define SUBF	((20<<4)+F)
#define SUBI	((20<<4)+I)
#define SUBP	((20<<4)+P)
#define SUBU	((20<<4)+U)
#define LSH	(21<<4)
#define LSHI	((21<<4)+I)
#define LSHU	((21<<4)+U)
#define MOD	(22<<4)
#define MODI	((22<<4)+I)
#define MODU	((22<<4)+U)
#define RSH	(23<<4)
#define RSHI	((23<<4)+I)
#define RSHU	((23<<4)+U)
#define BAND	(24<<4)
#define BANDU	((24<<4)+U)
#define BCOM	(25<<4)
#define BCOMU	((25<<4)+U)
#define BOR	(26<<4)
#define BORU	((26<<4)+U)
#define BXOR	(27<<4)
#define BXORU	((27<<4)+U)
#define DIV	(28<<4)
#define DIVD	((28<<4)+D)
#define DIVF	((28<<4)+F)
#define DIVI	((28<<4)+I)
#define DIVU	((28<<4)+U)
#define MUL	(29<<4)
#define MULD	((29<<4)+D)
#define MULF	((29<<4)+F)
#define MULI	((29<<4)+I)
#define MULU	((29<<4)+U)
#define EQ	(30<<4)
#define EQD	((30<<4)+D)
#define EQF	((30<<4)+F)
#define EQI	((30<<4)+I)
#define GE	(31<<4)
#define GED	((31<<4)+D)
#define GEF	((31<<4)+F)
#define GEI	((31<<4)+I)
#define GEU	((31<<4)+U)
#define GT	(32<<4)
#define GTD	((32<<4)+D)
#define GTF	((32<<4)+F)
#define GTI	((32<<4)+I)
#define GTU	((32<<4)+U)
#define LE	(33<<4)
#define LED	((33<<4)+D)
#define LEF	((33<<4)+F)
#define LEI	((33<<4)+I)
#define LEU	((33<<4)+U)
#define LT	(34<<4)
#define LTD	((34<<4)+D)
#define LTF	((34<<4)+F)
#define LTI	((34<<4)+I)
#define LTU	((34<<4)+U)
#define NE	(35<<4)
#define NED	((35<<4)+D)
#define NEF	((35<<4)+F)
#define NEI	((35<<4)+I)
#define JUMP	(36<<4)
#define JUMPV	((36<<4)+V)
#define LABEL	(37<<4)
#define LABELV	((37<<4)+V)
#define MAXOP	(38<<4)
#define AND	MAXOP
#define	NOT	(MAXOP+1*16)
#define	OR	(MAXOP+2*16)
#define	COND	(MAXOP+3*16)
#define	RIGHT	(MAXOP+4*16)
#define	FIELD	(MAXOP+5*16)
#endif /* __STDC__ */
#endif /* generic */

#ifdef NEEDNAMES
"CNST",
"ARG",
"ASGN",
"INDIR",
"CVC",
"CVD",
"CVF",
"CVI",
"CVP",
"CVS",
"CVU",
"NEG",
"CALL",
"LOAD",
"RET",
"ADDRG",
"ADDRF",
"ADDRL",
"ADD",
"SUB",
"LSH",
"MOD",
"RSH",
"BAND",
"BCOM",
"BOR",
"BXOR",
"DIV",
"MUL",
"EQ",
"GE",
"GT",
"LE",
"LT",
"NE",
"JUMP",
"LABEL",
#undef NEEDNAMES
#endif /* NEEDNAMES */
