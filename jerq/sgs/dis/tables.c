/*
 *	static	char	*tables_ID = "@(#) tables.c: 1.2 12/14/83";
 */
#include	"dis.h"

#define		INVALID	{"",UNKNOWN}


/*
 *	Address modes table for operand processing.  The first two
 *	nibbles will be used as an index into the table.  The first
 *	nibble usually determines the address mod and the second
 *	nibble usually determines the register.  The immediate and
 *	extended modes are exceptions.
 *	The first sub-field is the part or all of the string to
 *	be printed for the address.  The second sub-field names
 *	the address mode type (for use in case statement).
 */

struct formtable adrmodtbl[256] = {

/* [0,0] */	{"&0x0",IM6},		{"&0x1",IM6},		{"&0x2",IM6},		{"&0x3",IM6},
/* [0,4] */	{"&0x4",IM6},		{"&0x5",IM6},		{"&0x6",IM6},		{"&0x7",IM6},
/* [0,8] */	{"&0x8",IM6},		{"&0x9",IM6},		{"&0xa",IM6},		{"&0xb",IM6},
/* [0,C] */	{"&0xc",IM6},		{"&0xd",IM6},		{"&0xe",IM6},		{"&0xf",IM6},
 
/* [1,0] */	{"&0x10",IM6},		{"&0x11",IM6},		{"&0x12",IM6},		{"&0x13",IM6},
/* [1,4] */	{"&0x14",IM6},		{"&0x15",IM6},		{"&0x16",IM6},		{"&0x17",IM6},
/* [1,8] */	{"&0x18",IM6},		{"&0x19",IM6},		{"&0x1a",IM6},		{"&0x1b",IM6},
/* [1,C] */	{"&0x1c",IM6},		{"&0x1d",IM6},		{"&0x1e",IM6},		{"&0x1f",IM6},
 
/* [2,0] */	{"&0x20",IM6},		{"&0x21",IM6},		{"&0x22",IM6},		{"&0x23",IM6},
/* [2,4] */	{"&0x24",IM6},		{"&0x25",IM6},		{"&0x26",IM6},		{"&0x27",IM6},
/* [2,8] */	{"&0x28",IM6},		{"&0x29",IM6},		{"&0x2a",IM6},		{"&0x2b",IM6},
/* [2,C] */	{"&0x2c",IM6},		{"&0x2d",IM6},		{"&0x2e",IM6},		{"&0x2f",IM6},
 
/* [3,0] */	{"&0x30",IM6},		{"&0x31",IM6},		{"&0x32",IM6},		{"&0x33",IM6},
/* [3,4] */	{"&0x34",IM6},		{"&0x35",IM6},		{"&0x36",IM6},		{"&0x37",IM6},
/* [3,8] */	{"&0x38",IM6},		{"&0x39",IM6},		{"&0x3a",IM6},		{"&0x3b",IM6},
/* [3,C] */	{"&0x3c",IM6},		{"&0x3d",IM6},		{"&0x3e",IM6},		{"&0x3f",IM6},

/* [4,0] */	{"%r0",R},		{"%r1",R},		{"%r2",R},		{"%r3",R},
/* [4,4] */	{"%r4",R},		{"%r5",R},		{"%r6",R},		{"%r7",R},
/* [4,8] */	{"%r8",R},		{"%fp",R},		{"%ap",R},		{"%psw",R},
/* [4,C] */	{"%sp",R},		{"%pcbp",R},		{"%isp",R},		{"",IMW},
 
/* [5,0] */	{"0(%r0)",R},		{"0(%r1)",R},		{"0(%r2)",R},		{"0(%r3)",R},
/* [5,4] */	{"0(%r4)",R},		{"0(%r5)",R},		{"0(%r6)",R},		{"0(%r7)",R},
/* [5,8] */	{"0(%r8)",R},		{"0(%fp)",R},		{"0(%ap)",R},		{"0(%psw)",R},
/* [5,C] */	{"0(%sp)",R},		{"0(%pcbp)",R},		{"0(%isp)",R},		{"",IMH},
 
/* [6,0] */	{"0x0(%fp)",R4},	{"0x1(%fp)",R4},	{"0x2(%fp)",R4},	{"0x3(%fp)",R4},
/* [6,4] */	{"0x4(%fp)",R4},	{"0x5(%fp)",R4},	{"0x6(%fp)",R4},	{"0x7(%fp)",R4},
/* [6,8] */	{"0x8(%fp)",R4},	{"0x9(%fp)",R4},	{"0xa(%fp)",R4},	{"0xb(%fp)",R4},
/* [6,C] */	{"0xc(%fp)",R4},	{"0xd(%fp)",R4},	{"0xe(%fp)",R4},	{"",IMB},

/* [7,0] */	{"0x0(%ap)",R4},	{"0x1(%ap)",R4},	{"0x2(%ap)",R4},	{"0x3(%ap)",R4},
/* [7,4] */	{"0x4(%ap)",R4},	{"0x5(%ap)",R4},	{"0x6(%ap)",R4},	{"0x7(%ap)",R4},
/* [7,8] */	{"0x8(%ap)",R4},	{"0x9(%ap)",R4},	{"0xa(%ap)",R4},	{"0xb(%ap)",R4},
/* [7,C] */	{"0xc(%ap)",R4},	{"0xd(%ap)",R4},	{"0xe(%ap)",R4},	{"",ABAD},
 
/* [8,0] */	{"(%r0)",BDW},		{"(%r1)",BDW},		{"(%r2)",BDW},		{"(%r3)",BDW},
/* [8,4] */	{"(%r4)",BDW},		{"(%r5)",BDW},		{"(%r6)",BDW},		{"(%r7)",BDW},
/* [8,8] */	{"(%r8)",BDW},		{"(%fp)",BDW},		{"(%ap)",BDW},		{"(%psw)",BDW},
/* [8,C] */	{"(%sp)",BDW},		{"(%pcbp)",BDW},	{"(%isp)",BDW},		{"(%pc)",BDW},
 
/* [9,0] */	{"(%r0)",BDWDF},	{"(%r1)",BDWDF},	{"(%r2)",BDWDF},	{"(%r3)",BDWDF},
/* [9,4] */	{"(%r4)",BDWDF},	{"(%r5)",BDWDF},	{"(%r6)",BDWDF},	{"(%r7)",BDWDF},
/* [9,8] */	{"(%r8)",BDWDF},	{"(%fp)",BDWDF},	{"(%ap)",BDWDF},	{"(%psw)",BDWDF},
/* [9,C] */	{"(%sp)",BDWDF},	{"(%pcbp)",BDWDF},	{"(%isp)",BDWDF},	{"(%pc)",BDWDF},
 
/* [A,0] */	{"(%r0)",BDH},		{"(%r1)",BDH},		{"(%r2)",BDH},		{"(%r3)",BDH},
/* [A,4] */	{"(%r4)",BDH},		{"(%r5)",BDH},		{"(%r6)",BDH},		{"(%r7)",BDH},
/* [A,8] */	{"(%r8)",BDH},		{"(%fp)",BDH},		{"(%ap)",BDH},		{"(%psw)",BDH},
/* [A,C] */	{"(%sp)",BDH},		{"(%pcbp)",BDH},	{"(%isp)",BDH},		{"(%pc)",BDH},
 
/* [B,0] */	{"(%r0)",BDHDF},	{"(%r1)",BDHDF},	{"(%r2)",BDHDF},	{"(%r3)",BDHDF},
/* [B,4] */	{"(%r4)",BDHDF},	{"(%r5)",BDHDF},	{"(%r6)",BDHDF},	{"(%r7)",BDHDF},
/* [B,8] */	{"(%r8)",BDHDF},	{"(%fp)",BDHDF},	{"(%ap)",BDHDF},	{"(%psw)",BDHDF},
/* [B,C] */	{"(%sp)",BDHDF},	{"(%pcbp)",BDHDF},	{"(%isp)",BDHDF},	{"(%pc)",BDHDF},
 
/* [C,0] */	{"(%r0)",BDB},		{"(%r1)",BDB},		{"(%r2)",BDB},		{"(%r3)",BDB},
/* [C,4] */	{"(%r4)",BDB},		{"(%r5)",BDB},		{"(%r6)",BDB},		{"(%r7)",BDB},
/* [C,8] */	{"(%r8)",BDB},		{"(%fp)",BDB},		{"(%ap)",BDB},		{"(%psw)",BDB},
/* [C,C] */	{"(%sp)",BDB},		{"(%pcbp)",BDB},	{"(%isp)",BDB},		{"(%pc)",IMD},
 
/* [D,0] */	{"(%r0)",BDBDF},	{"(%r1)",BDBDF},	{"(%r2)",BDBDF},	{"(%r3)",BDBDF},
/* [D,4] */	{"(%r4)",BDBDF},	{"(%r5)",BDBDF},	{"(%r6)",BDBDF},	{"(%r7)",BDBDF},
/* [D,8] */	{"(%r8)",BDBDF},	{"(%fp)",BDBDF},	{"(%ap)",BDBDF},	{"(%psw)",BDBDF},
/* [D,C] */	{"(%sp)",BDBDF},	{"(%pcbp)",BDBDF},	{"(%isp)",BDBDF},	{"(%pc)",BDBDF},
 
/* [E,0] */	{"{uword}",EXPUW},	INVALID,		{"{uhalf}",EXPUH},	{"{ubyte}",EXPUB},
/* [E,4] */	{"{sword}",EXPSW},	INVALID,		{"{shalf}",EXPSH},	{"{sbyte}",EXPSB},
/* [E,8] */	INVALID,		INVALID,		INVALID,		INVALID,
/* [E,C] */	INVALID,		INVALID,		INVALID,		{"",ABADDF},
 
/* [F,0] */	{"&-0x10",IM4},		{"&-0xf",IM4},		{"&-0xe",IM4},		{"&-0xd",IM4},
/* [F,4] */	{"&-0xc",IM4},		{"&-0xb",IM4},		{"&-0xa",IM4},		{"&-0x9",IM4},
/* [F,8] */	{"&-0x8",IM4},		{"&-0x7",IM4},		{"&-0x6",IM4},		{"&-0x5",IM4},
/* [F,C] */	{"&-0x4",IM4},		{"&-0x3",IM4},		{"&-0x2",IM4},		{"&-0x1",IM4},
};
/*
 *	Main decode table for the op codes.  The first two nibbles
 *	will be used as an index into the table.  All op codes
 *	are exactly two nibbles.  The first sub-field is the op code
 *	mnemonic name.  The second sub-field is the op code class.
 */

struct instable opcodetbl[256] = {

/* [0,0] */	{"halt",OPRNDS0},	{"getsm",OPRNDS0},	{"SPOPRD",SPRTOP1},	{"SPOPD2",SPRTOP2},
/* [0,4] */	{"MOVAW",OPRNDS2},	INVALID,		{"SPOPRT",SPRTOP1},	{"SPOPT2",SPRTOP2},
/* [0,8] */	{"RET",OPRNDS0},	INVALID,		INVALID,		INVALID,
/* [0,C] */	{"MOVTRW",OPRNDS2},	INVALID,		INVALID,		INVALID,

/* [1,0] */	{"SAVE",OPRNDS1},	{"putsm",OPRNDS0},	INVALID,		{"SPOPWD",SPRTOP1},
/* [1,4] */	{"EXTOP",EXT},		INVALID,		INVALID,		{"SPOPWT",SPRTOP1},
/* [1,8] */	{"RESTORE",OPRNDS1},	INVALID,		INVALID,		INVALID,
/* [1,C] */	{"SWAPWI",OPRNDS1},	INVALID,		{"SWAPHI",OPRNDS1},	{"SWAPBI",OPRNDS1},

/* [2,0] */	{"POPW",OPRNDS1},	{"ungetsm",OPRNDS0},	{"SPOPRS",SPRTOP1},	{"SPOPS2",SPRTOP2},
/* [2,4] */	{"JMP",JUMP},		INVALID,		INVALID,		{"CFLUSH",OPRNDS0},
/* [2,8] */	{"TSTW",OPRNDS1},	INVALID,		{"TSTH",OPRNDS1},	{"TSTB",OPRNDS1},
/* [2,C] */	{"CALL",OPRNDS2},	INVALID,		{"BPT",OPRNDS0},	{"WAIT",OPRNDS0},

/* [3,0] */	{"",MACRO},		{"FADDS2",SFPOPS2},	{"SPOP",SPRTOP0},	{"SPOPWS",SPRTOP1},
/* [3,4] */	{"JSB",JUMP},		{"FADDD2",DFPOPS2},	{"BSBH",JUMP2},		{"BSBB",JUMP1},
/* [3,8] */	{"BITW",OPRNDS2},	{"FADDS3",SFPOPS3},	{"BITH",OPRNDS2},	{"BITB",OPRNDS2},
/* [3,C] */	{"CMPW",OPRNDS2},	INVALID,		{"CMPH",OPRNDS2},	{"CMPB",OPRNDS2},

/* [4,0] */	{"RGEQ",OPRNDS0},	{"FSUBS2",SFPOPS2},	{"BGEH",JUMP2},		{"BGEB",JUMP1},
/* [4,4] */	{"RGTR",OPRNDS0},	{"FSUBD2",DFPOPS2},	{"BGH",JUMP2},		{"BGB",JUMP1},
/* [4,8] */	{"RLSS",OPRNDS0},	{"FSUBS3",SFPOPS3},	{"BLH",JUMP2},		{"BLB",JUMP1},
/* [4,C] */	{"RLEQ",OPRNDS0},	INVALID,	    	{"BLEH",JUMP2},		{"BLEB",JUMP1},

/* [5,0] */	{"RCC",OPRNDS0},	{"FMULS2",SFPOPS2},	{"BGEUH",JUMP2},	{"BGEUB",JUMP1},
/* [5,4] */	{"RGTRU",OPRNDS0},	{"FMULD2",DFPOPS2},	{"BGUH",JUMP2},		{"BGUB",JUMP1},
/* [5,8] */	{"RCS",OPRNDS0},	{"FMULS3",SFPOPS3},	{"BLUH",JUMP2},		{"BLUB",JUMP1},
/* [5,C] */	{"RLEQU",OPRNDS0},	INVALID,		{"BLEUH",JUMP2},	{"BLEUB",JUMP1},

/* [6,0] */	{"RVC",OPRNDS0},	{"FDIVS2",SFPOPS2},	{"BVCH",JUMP2},		{"BVCB",JUMP1},
/* [6,4] */	{"RNEQ",OPRNDS0},	{"FDIVD2",DFPOPS2},	{"BNEH",JUMP2},		{"BNEB",JUMP1},
/* [6,8] */	{"RVS",OPRNDS0},	{"FDIVS3",SFPOPS3},	{"BVSH",JUMP2},		{"BVSB",JUMP1},
/* [6,C] */	{"REQL",OPRNDS0},	INVALID,		{"BEH",JUMP2},		{"BEB",JUMP1},

/* [7,0] */	{"NOP",OPRNDS0},	{"MOVHS",SFPOPS2},	{"NOP3",NOOP16},	{"NOP2",NOOP8},
/* [7,4] */	{"RNEQ",OPRNDS0},	{"MOVHD",DFPOPS2},	{"BNEH",JUMP2},		{"BNEB",JUMP1},
/* [7,8] */	{"RSB",OPRNDS0},	{"FADDD3",DFPOPS3},	{"BRH",JUMP2},		{"BRB",JUMP1},
/* [7,C] */	{"REQL",OPRNDS0},	INVALID,		{"BEH",JUMP2},		{"BEB",JUMP1},

/* [8,0] */	{"CLRW",OPRNDS1},	{"MOVWS",SFPOPS2},	{"CLRH",OPRNDS1},	{"CLRB",OPRNDS1},
/* [8,4] */	{"MOVW",OPRNDS2},	{"MOVWD",DFPOPS2},	{"MOVH",OPRNDS2},	{"MOVB",OPRNDS2},
/* [8,8] */	{"MCOMW",OPRNDS2},	{"FSUBD3",DFPOPS3},	{"MCOMH",OPRNDS2},	{"MCOMB",OPRNDS2},
/* [8,C] */	{"MNEGW",OPRNDS2},	INVALID,		{"MNEGH",OPRNDS2},	{"MNEGB",OPRNDS2},

/* [9,0] */	{"INCW",OPRNDS1},	{"MOVSS",SFPOPS2},	{"INCH",OPRNDS1},	{"INCB",OPRNDS1},
/* [9,4] */	{"DECW",OPRNDS1},	{"MOVDS",DFPOPS2},	{"DECH",OPRNDS1},	{"DECB",OPRNDS1},
/* [9,8] */	INVALID,		{"FMULD3",DFPOPS3},	INVALID,		INVALID,
/* [9,C] */	{"ADDW2",OPRNDS2},	INVALID,		{"ADDH2",OPRNDS2},	{"ADDB2",OPRNDS2},

/* [A,0] */	{"PUSHW",OPRNDS1},	{"MOVSD",SFPOPS2},	INVALID,		INVALID,
/* [A,4] */	{"MODW2",OPRNDS2},	{"MOVDD",DFPOPS2},	{"MODH2",OPRNDS2},	{"MODB2",OPRNDS2},
/* [A,8] */	{"MULW2",OPRNDS2},	{"FDIVD3",DFPOPS3},	{"MULH2",OPRNDS2},	{"MULB2",OPRNDS2},
/* [A,C] */	{"DIVW2",OPRNDS2},	INVALID,		{"DIVH2",OPRNDS2},	{"DIVB2",OPRNDS2},

/* [B,0] */	{"ORW2",OPRNDS2},	{"MOVSH",SFPOPS2},	{"ORH2",OPRNDS2},	{"ORB2",OPRNDS2},
/* [B,4] */	{"XORW2",OPRNDS2},	{"MOVDH",DFPOPS2},	{"XORH2",OPRNDS2},	{"XORB2",OPRNDS2},
/* [B,8] */	{"ANDW2",OPRNDS2},	INVALID,		{"ANDH2",OPRNDS2},	{"ANDB2",OPRNDS2},
/* [B,C] */	{"SUBW2",OPRNDS2},	INVALID,		{"SUBH2",OPRNDS2},	{"SUBB2",OPRNDS2},

/* [C,0] */	{"ALSW3",OPRNDS3},	{"MOVTSH",SFPOPS2},	INVALID,		INVALID,
/* [C,4] */	{"ARSW3",OPRNDS3},	{"MOVTDH",DFPOPS2},	{"ARSH3",OPRNDS3},	{"ARSB3",OPRNDS3},
/* [C,8] */	{"INSFW",OPRNDS4},	INVALID,		{"INSFH",OPRNDS4},	{"INSFB",OPRNDS4},
/* [C,C] */	{"EXTFW",OPRNDS4},	INVALID,		{"EXTFH",OPRNDS4},	{"EXTFB",OPRNDS4},

/* [D,0] */	{"LLSW3",OPRNDS3},	{"MOVSW",SFPOPS2},	{"LLSH3",OPRNDS3},	{"LLSB3",OPRNDS3},
/* [D,4] */	{"LRSW3",OPRNDS3},	{"MOVDW",DFPOPS2},	INVALID,		INVALID,
/* [D,8] */	{"ROTW",OPRNDS3},	INVALID,		INVALID,		INVALID,
/* [D,C] */	{"ADDW3",OPRNDS3},	INVALID,		{"ADDH3",OPRNDS3},	{"ADDB3",OPRNDS3},

/* [E,0] */	{"PUSHAW",OPRNDS1},	{"MOVTSW",SFPOPS2},	INVALID,		INVALID,
/* [E,4] */	{"MODW3",OPRNDS3},	{"MOVTDW",DFPOPS2},	{"MODH3",OPRNDS3},	{"MODB3",OPRNDS3},
/* [E,8] */	{"MULW3",OPRNDS3},	INVALID,		{"MULH3",OPRNDS3},	{"MULB3",OPRNDS3},
/* [E,C] */	{"DIVW3",OPRNDS3},	INVALID,		{"DIVH3",OPRNDS3},	{"DIVB3",OPRNDS3},

/* [F,0] */	{"ORW3",OPRNDS3},	{"FCMPS",SFPOPS2},	{"ORH3",OPRNDS3},	{"ORB3",OPRNDS3},
/* [F,4] */	{"XORW3",OPRNDS3},	{"FCMPD",DFPOPS2},	{"XORH3",OPRNDS3},	{"XORB3",OPRNDS3},
/* [F,8] */	{"ANDW3",OPRNDS3},	INVALID,		{"ANDH3",OPRNDS3},	{"ANDB3",OPRNDS3},
/* [F,C] */	{"SUBW3",OPRNDS3},	INVALID,		{"SUBH3",OPRNDS3},	{"SUBB3",OPRNDS3},
};
