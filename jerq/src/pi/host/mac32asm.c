#include "asm.pri"
#include "mac32asm.h"
#include "core.pub"
#include "format.pub"
SRCFILE("mac32asm.c")

Mac32Instr.Mac32Instr(Asm*a,long l):(a,l) { display(); }
char *Mac32Asm.literaldelimiter()	{ return "&"; }
Instr *Mac32Asm.newInstr(long a)	{ return (Instr*) new Mac32Instr(this,a); }
Mac32Asm.Mac32Asm(Core *c):(c)		{ }

char *Mac32Instr.extend(int tttt)
{
	switch( tttt ){
		case 0: return "uword";
		case 2: return "uhalf";
		case 3: return "ubyte";
		case 4: return "sword";
		case 6: return "shalf";
		case 7: return "sbyte";
	}
	return sf( "tttt=%d", tttt );
}

char *Mac32Instr.macro()
{
	switch((unsigned char) m.chr){
		case 0x09:  return "mverno";
		case 0x0D:  return "enbvjmp";
		case 0x13:  return "disvjmp";
		case 0x19:  return "movblw";
		case 0x1F:  return "strend";
		case 0x2F:  return "intack";
		case 0x35:  return "strcpy";
		case 0x3C:  return "slftst";
		case 0x45:  return "retg";
		case 0x61:  return "gate";
		case 0xAC:  return "callps";
		case 0xC8:  return "retps";
	}
	return sf( "<macro 0x%X>", (unsigned char) m.chr );
}

int Mac32Instr.argtype(int)
{
	return Mac32OpTab[opcode&0xFF].type;
}

int Mac32Instr.nargs()
{
	switch( argtype(0) ){
		case BSBB:
		case BSBH:
		case EXT:
		case JUMP1:
		case JUMP2:
		case JUMP:
		case JUMPSB:
		case MACRO:
		case NOOP16:
		case NOOP8:
		case OPRNDS1:
				return 1;
		case OPRNDS2:	return 2;
		case OPRNDS3:	return 3;
		case OPRNDS4:	return 4;

		case CALL:			/* these don't matter? */
		case AD1OP3:
		case SFPOPS2:
		case SFPOPS3:
		case DFPOPS2:
		case DFPOPS3:
		case SPRTOP0:
		case SPRTOP1:
		case SPRTOP2:
				break;
	}
	return 0;
}

char *Mac32Instr.mnemonic()
{
	char *p = Mac32OpTab[opcode&0xFF].mnem, *q;
	if( !p ) return Instr::mnemonic();
	for( q = p; *q; *q++ |= ' ' ) {}
	return p;
}

char *Mac32Instr.arg(int i)
{
	unsigned char modereg, mode;

	trace( "%d.Arg(0x%X)", this ); OK("Mac32Instr.arg");
	m = *_asm->core->peekcode(next++);
	switch(argtype(i)){
	case EXT:	return literal(F_MASKEXT8|fmt);
	case NOOP8:	return "";
	case NOOP16:	next += 1; return "";
	case MACRO:	return macro();
	case JUMP1: 	m = addr+m.chr; return symbolic();
	case JUMP2: 	next += 1; m = addr+m.sht; return symbolic();
	}
	modereg = m.chr;
	mode = modereg>>4;
	reg = modereg&0xF;
	m = *_asm->core->peekcode(next);
	switch( modereg ){
/*abs*/
	case 0x7F:	next += 4; return symbolic();
	case 0xEF:	next += 4; return symbolic("*");
/*imm*/
	case 0x4F:	next += 4; return literal(fmt);
	case 0x5F:	next += 2; return literal(F_MASKEXT16|fmt);
	case 0x6F:	next += 1; return literal(F_MASKEXT8|fmt);
	}

	switch( mode ){	
/*pos*/
	case 0x0:
	case 0x1:
	case 0x2:
	case 0x3:	m = modereg;        return literal(F_MASKEXT8|fmt);
/*neg*/
	case 0xF:	m = (char) modereg; return literal(F_MASKEXT8|fmt);
/*reg*/
	case 0x4:	 return regarg( "%s%s",	  fmt&F_SYMBOLIC|F_NONE );
	case 0x5: m = 0; return regarg( "%s(%s)", fmt&F_SYMBOLIC	);
/*disp*/
	case 0xC: next+=1; m = m.chr; return regarg("%s(%s)",	fmt|F_MASKEXT8 );
	case 0xD: next+=1; m = m.chr; return regarg("*%s(%s)",	fmt|F_MASKEXT8 );
	case 0xA: next+=2; m = m.sht; return regarg("%s(%s)",	fmt|F_MASKEXT16);
	case 0xB: next+=2; m = m.sht; return regarg("*%s(%s)",	fmt|F_MASKEXT16);
	case 0x8: next+=4;	      return regarg("%s(%s)",	fmt	       );
	case 0x9: next+=4;	      return regarg("*%s(%s)",	fmt	       );
/*offset*/
	case 0x6:
	case 0x7:
		  m = reg; reg = mode+3; return regarg("%s(%s)", fmt|F_MASKEXT8);
/*expanded*/
	case 0xE: char *e = extend(reg); return sf( "{%s}%s", e, arg(i) );
	}
	return sf( "modereg=0x%X", modereg );
} 

#define INVALID {"???",UNKNOWN}

/*
 *Main decode table for the op codes.  The first two nibbles
 *will be used as an index into the table.  All op codes
 *are exactly two nibbles.  The first sub-field is the op code
 *mnemonic name.  The second sub-field is the op code class.
 */

struct mac32optab Mac32OpTab[256] = {

/* [0,0] */{"halt",OPRNDS0},{"getsm",OPRNDS0},{"SPOPRD",SPRTOP1},{"SPOPD2",SPRTOP2},
/* [0,4] */{"MOVAW",OPRNDS2},INVALID,{"SPOPRT",SPRTOP1},{"SPOPT2",SPRTOP2},
/* [0,8] */{"RET",OPRNDS0},INVALID,INVALID,INVALID,
/* [0,C] */{"MOVTRW",OPRNDS2},INVALID,INVALID,INVALID,

/* [1,0] */{"SAVE",OPRNDS1},{"putsm",OPRNDS0},INVALID,{"SPOPWD",SPRTOP1},
/* [1,4] */{"EXTOP",EXT},INVALID,INVALID,{"SPOPWT",SPRTOP1},
/* [1,8] */{"RESTORE",OPRNDS1},INVALID,INVALID,INVALID,
/* [1,C] */{"SWAPWI",OPRNDS1},INVALID,{"SWAPHI",OPRNDS1},{"SWAPBI",OPRNDS1},

/* [2,0] */{"POPW",OPRNDS1},{"ungetsm",OPRNDS0},{"SPOPRS",SPRTOP1},{"SPOPS2",SPRTOP2},
/* [2,4] */{"JMP",JUMP},INVALID,INVALID,{"CFLUSH",OPRNDS0},
/* [2,8] */{"TSTW",OPRNDS1},INVALID,{"TSTH",OPRNDS1},{"TSTB",OPRNDS1},
/* [2,C] */{"CALL",OPRNDS2},INVALID,{"BPT",OPRNDS0},{"WAIT",OPRNDS0},

/* [3,0] */{"",MACRO},{"FADDS2",SFPOPS2},{"SPOP",SPRTOP0},{"SPOPWS",SPRTOP1},
/* [3,4] */{"JSB",JUMP},{"FADDD2",DFPOPS2},{"BSBH",JUMP2},{"BSBB",JUMP1},
/* [3,8] */{"BITW",OPRNDS2},{"FADDS3",SFPOPS3},{"BITH",OPRNDS2},{"BITB",OPRNDS2},
/* [3,C] */{"CMPW",OPRNDS2},INVALID,{"CMPH",OPRNDS2},{"CMPB",OPRNDS2},

/* [4,0] */{"RGEQ",OPRNDS0},{"FSUBS2",SFPOPS2},{"BGEH",JUMP2},{"BGEB",JUMP1},
/* [4,4] */{"RGTR",OPRNDS0},{"FSUBD2",DFPOPS2},{"BGH",JUMP2},{"BGB",JUMP1},
/* [4,8] */{"RLSS",OPRNDS0},{"FSUBS3",SFPOPS3},{"BLH",JUMP2},{"BLB",JUMP1},
/* [4,C] */{"RLEQ",OPRNDS0},INVALID,    {"BLEH",JUMP2},{"BLEB",JUMP1},

/* [5,0] */{"RCC",OPRNDS0},{"FMULS2",SFPOPS2},{"BGEUH",JUMP2},{"BGEUB",JUMP1},
/* [5,4] */{"RGTRU",OPRNDS0},{"FMULD2",DFPOPS2},{"BGUH",JUMP2},{"BGUB",JUMP1},
/* [5,8] */{"RCS",OPRNDS0},{"FMULS3",SFPOPS3},{"BLUH",JUMP2},{"BLUB",JUMP1},
/* [5,C] */{"RLEQU",OPRNDS0},INVALID,{"BLEUH",JUMP2},{"BLEUB",JUMP1},

/* [6,0] */{"RVC",OPRNDS0},{"FDIVS2",SFPOPS2},{"BVCH",JUMP2},{"BVCB",JUMP1},
/* [6,4] */{"RNEQ",OPRNDS0},{"FDIVD2",DFPOPS2},{"BNEH",JUMP2},{"BNEB",JUMP1},
/* [6,8] */{"RVS",OPRNDS0},{"FDIVS3",SFPOPS3},{"BVSH",JUMP2},{"BVSB",JUMP1},
/* [6,C] */{"REQL",OPRNDS0},INVALID,{"BEH",JUMP2},{"BEB",JUMP1},

/* [7,0] */{"NOP",OPRNDS0},{"MOVHS",SFPOPS2},{"NOP3",NOOP16},{"NOP2",NOOP8},
/* [7,4] */{"RNEQ",OPRNDS0},{"MOVHD",DFPOPS2},{"BNEH",JUMP2},{"BNEB",JUMP1},
/* [7,8] */{"RSB",OPRNDS0},{"FADDD3",DFPOPS3},{"BRH",JUMP2},{"BRB",JUMP1},
/* [7,C] */{"REQL",OPRNDS0},INVALID,{"BEH",JUMP2},{"BEB",JUMP1},

/* [8,0] */{"CLRW",OPRNDS1},{"MOVWS",SFPOPS2},{"CLRH",OPRNDS1},{"CLRB",OPRNDS1},
/* [8,4] */{"MOVW",OPRNDS2},{"MOVWD",DFPOPS2},{"MOVH",OPRNDS2},{"MOVB",OPRNDS2},
/* [8,8] */{"MCOMW",OPRNDS2},{"FSUBD3",DFPOPS3},{"MCOMH",OPRNDS2},{"MCOMB",OPRNDS2},
/* [8,C] */{"MNEGW",OPRNDS2},INVALID,{"MNEGH",OPRNDS2},{"MNEGB",OPRNDS2},

/* [9,0] */{"INCW",OPRNDS1},{"MOVSS",SFPOPS2},{"INCH",OPRNDS1},{"INCB",OPRNDS1},
/* [9,4] */{"DECW",OPRNDS1},{"MOVDS",DFPOPS2},{"DECH",OPRNDS1},{"DECB",OPRNDS1},
/* [9,8] */INVALID,{"FMULD3",DFPOPS3},INVALID,INVALID,
/* [9,C] */{"ADDW2",OPRNDS2},INVALID,{"ADDH2",OPRNDS2},{"ADDB2",OPRNDS2},

/* [A,0] */{"PUSHW",OPRNDS1},{"MOVSD",SFPOPS2},INVALID,INVALID,
/* [A,4] */{"MODW2",OPRNDS2},{"MOVDD",DFPOPS2},{"MODH2",OPRNDS2},{"MODB2",OPRNDS2},
/* [A,8] */{"MULW2",OPRNDS2},{"FDIVD3",DFPOPS3},{"MULH2",OPRNDS2},{"MULB2",OPRNDS2},
/* [A,C] */{"DIVW2",OPRNDS2},INVALID,{"DIVH2",OPRNDS2},{"DIVB2",OPRNDS2},

/* [B,0] */{"ORW2",OPRNDS2},{"MOVSH",SFPOPS2},{"ORH2",OPRNDS2},{"ORB2",OPRNDS2},
/* [B,4] */{"XORW2",OPRNDS2},{"MOVDH",DFPOPS2},{"XORH2",OPRNDS2},{"XORB2",OPRNDS2},
/* [B,8] */{"ANDW2",OPRNDS2},INVALID,{"ANDH2",OPRNDS2},{"ANDB2",OPRNDS2},
/* [B,C] */{"SUBW2",OPRNDS2},INVALID,{"SUBH2",OPRNDS2},{"SUBB2",OPRNDS2},

/* [C,0] */{"ALSW3",OPRNDS3},{"MOVTSH",SFPOPS2},INVALID,INVALID,
/* [C,4] */{"ARSW3",OPRNDS3},{"MOVTDH",DFPOPS2},{"ARSH3",OPRNDS3},{"ARSB3",OPRNDS3},
/* [C,8] */{"INSFW",OPRNDS4},INVALID,{"INSFH",OPRNDS4},{"INSFB",OPRNDS4},
/* [C,C] */{"EXTFW",OPRNDS4},INVALID,{"EXTFH",OPRNDS4},{"EXTFB",OPRNDS4},

/* [D,0] */{"LLSW3",OPRNDS3},{"MOVSW",SFPOPS2},{"LLSH3",OPRNDS3},{"LLSB3",OPRNDS3},
/* [D,4] */{"LRSW3",OPRNDS3},{"MOVDW",DFPOPS2},INVALID,INVALID,
/* [D,8] */{"ROTW",OPRNDS3},INVALID,INVALID,INVALID,
/* [D,C] */{"ADDW3",OPRNDS3},INVALID,{"ADDH3",OPRNDS3},{"ADDB3",OPRNDS3},

/* [E,0] */{"PUSHAW",OPRNDS1},{"MOVTSW",SFPOPS2},INVALID,INVALID,
/* [E,4] */{"MODW3",OPRNDS3},{"MOVTDW",DFPOPS2},{"MODH3",OPRNDS3},{"MODB3",OPRNDS3},
/* [E,8] */{"MULW3",OPRNDS3},INVALID,{"MULH3",OPRNDS3},{"MULB3",OPRNDS3},
/* [E,C] */{"DIVW3",OPRNDS3},INVALID,{"DIVH3",OPRNDS3},{"DIVB3",OPRNDS3},

/* [F,0] */{"ORW3",OPRNDS3},{"FCMPS",SFPOPS2},{"ORH3",OPRNDS3},{"ORB3",OPRNDS3},
/* [F,4] */{"XORW3",OPRNDS3},{"FCMPD",DFPOPS2},{"XORH3",OPRNDS3},{"XORB3",OPRNDS3},
/* [F,8] */{"ANDW3",OPRNDS3},INVALID,{"ANDH3",OPRNDS3},{"ANDB3",OPRNDS3},
/* [F,C] */{"SUBW3",OPRNDS3},INVALID,{"SUBH3",OPRNDS3},{"SUBB3",OPRNDS3},
};
