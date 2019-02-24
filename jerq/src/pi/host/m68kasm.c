#include "asm.pri"
#include "m68kasm.h"
#include "format.pub"
#include "core.pub"
SRCFILE("m68kasm.c")

M68kInstr.M68kInstr(Asm*a,long l):(a,l&1?l+1:l) { tab = 0; display(); }
Instr *M68kAsm.newInstr(long a)		{ return (Instr*) new M68kInstr(this,a); }
M68kAsm.M68kAsm(Core *c):(c)		{ }
char *M68kAsm.literaldelimiter()	{ return "&"; }

const short   SZ_MASK = 00300, SZ_SHIFT = 6;	// bits  (7-6)
const short  DEA_MASK = 00077;			// bits  (5-0)
const short  DRG_MASK = 00007;			// bits  (2-0)
const short DRGL_MASK = 07000;			// bits (11-9)
const short  DBR_MASK = 00377;			// bits  (7-0)
const short  DAQ_MASK = 07000;			// bits (11-9)
const short DEAM_MASK = 07700;			// bits (11-6)
const short  DMQ_MASK = 00377;			// bits  (7-0)

short OpMask[] = {
	0,		// DIG	0	/* ignore this address */
	DEA_MASK,	// DEA	1	/* E.A. to low order 6 bits */
	DRG_MASK,	// DRG	2	/* register to low order 3 bits */
	DRGL_MASK,	// DRGL	3	/* register to bits 11-9 */
	DBR_MASK,	// DBR	4	/* branch offset (short) */
	DMQ_MASK,	// DMQ	5	/* move-quick 8-bit value */
	DAQ_MASK,	// DAQ	6	/* add-quick 3-bit value in 11-9 */
	0,		// DIM	7	/* Immediate value, according to size */
	DEAM_MASK,	// DEAM	8	/* E.A. to bits 11-6 as in move */
	0,		// DBCC	9	/* branch address as in "dbcc" */
	0,		// DIMH	10	/* Immediate forced to be 1 word */
	0,		// D2L	11	/* register to bits 0-2 of next word */
	0,		// D2H	12	/* register to bits 12-14 of next word */
	0,		// DBL	13	/* qty in bits 0-5 of next word */
	0,		// DBH	14	/* qty in bits 6-11 of next word */
	0,		// DCR	15	/* control reg a bit combination in 0-11 */
};

char *M68kInstr.dotbwl()
{
	if( !strcmp(tab->opname, tab[1].opname)		// must be +/- 1?
	 || !strcmp(tab->opname, tab[-1].opname) 
	 || tab->size == BWL )
		switch( size ){
		case B : return ".b";
		case W : return ".w";
		case L : return ".l";
		}
	return "";
}

int M68kInstr.imm_bytes(short addr, short disp)
{
	int bytes = 0;
	switch( disp ){
	case DIMH: bytes = 2; break;
	case DIM:  switch(size){
		   case B:
		   case W: bytes = 2; break;	
		   case L: bytes = 4; break;
		   }
		   if( addr&H ) bytes = 2; break;
	}
	switch(bytes){
	case 2: immval = _asm->core->peekcode(next)->sht; break;
	case 4: immval = _asm->core->peekcode(next)->lng; break;
	}
	return bytes;
}

short M68kInstr.sizemask()
{
	static char bwl[4] = { B, W, L, 0 };
	size = tab->size;
	if( size!=BWL ) return 0;
	size = bwl[(opword0>>SZ_SHIFT)&3];
	return size ? SZ_MASK : 0;
}

char *M68kInstr.ea()
{
	if( mode>=1 && mode<=6 ) reg += 8;
	switch( mode ){
	case 0:	return regarg("%s%s",    fmt&F_SYMBOLIC|F_NONE);
	case 1: return regarg("%s%s",    fmt&F_SYMBOLIC|F_NONE);
	case 2:	return regarg("%s(%s)",  fmt&F_SYMBOLIC|F_NONE);
	case 3:	return regarg("%s(%s)+", fmt&F_SYMBOLIC|F_NONE);
	case 4:	return regarg("%s-(%s)", fmt&F_SYMBOLIC|F_NONE);
	case 5:	m = m.sht; next += 2;
		return regarg("%s(%s)", fmt|F_MASKEXT16);
	case 6: short brief = m.sht; next+= 2;
		if( brief & 0400 ) return "full";
		char ireg = (brief>>12)&15;
		m = (char)brief;
		int scale = (brief>>9)&3;
		scale = 1<<scale;
		char *wl = brief&04000 ? "l" : "w";
		char *pre = regarg("%s(%s,", fmt&F_SYMBOLIC|F_NONE);
		return sf("%s%s.%s*%d)", pre, _asm->core->regname(ireg), wl, scale);
	case 7:
	    switch( reg ){
	    case 0: m = m.sht; next += 2; return symbolic();
	    case 1: m = m.lng; next += 4; return symbolic();
	    case 4: switch( size ){
		    case B: m = m.sht; next += 2; return literal(F_MASKEXT8|fmt);
		    case W: m = m.sht; next += 2; return literal(F_MASKEXT16|fmt);
		    case L: m = m.lng; next += 4; return literal(fmt);
		    }
		    return "<imm?>";
	    }
	}
	return sf("mode=%d:reg=%d", mode, reg);
}

char *M68kInstr.dbr(char x)
{
	switch( x ){
	case  0: m = next + m.sht; next += 2; break;
	case -1: m = next + m.lng; next += 4; break;
	default: m = next + x;
	}
	return symbolic();
}

char *M68kInstr.dbh()
{
	if( opword1&0004000 ){
		reg = (opword1>>6)&7;
		return regarg("%s%s", fmt&F_SYMBOLIC|F_NONE);
	}
	m = (opword1>>6)&31;
	return literal(fmt);
}

char *M68kInstr.dbl()
{
	if( opword1&0000040 ){
		reg = opword1&7;
		return regarg("%s%s", fmt&F_SYMBOLIC|F_NONE);
	}
	m = opword1&31;
	return literal(fmt);
}

char *M68kInstr.dcr()
{
	next += 2;
	switch( opword1&0xFFF ){
	case 0x000: return "sfc";
	case 0x001: return "dfc";
	case 0x002: return "cacr";
	case 0x800: return "usp";
	case 0x801: return "vbr";
	case 0x802: return "caar";
	case 0x803: return "msp";
	case 0x804: return "vbr";
	}
	return "<dcr>";
}

char *M68kInstr.arg(int i)
{
	short disp;
	short addr;
	switch( i ){						// !!
	case 0: disp = tab->a1disp; addr = tab->addr1; break;
	case 1: disp = tab->a2disp; addr = tab->addr2; break;
	case 2: disp = tab->a3disp; addr = tab->addr3; break;
	case 3: disp = tab->a4disp; addr = tab->addr4; break;
	}

	disp &= 15;
	short mask = OpMask[disp];
	short x = mask ? (opword0&mask)/(((((~mask)+1)^(~mask))+1)>>1) : 0;
	m = *_asm->core->peekcode(next);
	switch( addr ){
	case AREG+C:  return "ccr";
	case AREG+SR: return "sr";
	}
	switch( disp ){
	case DEA: mode = x>>3; reg = x&7; return ea();
	case DRG:						// fall thru
	case DRGL:reg = x;
		  if( addr&A ) reg += 8;
		  return regarg("%s%s",fmt&F_SYMBOLIC|F_NONE);
	case DBR: return dbr(x);
	case DMQ: m = (long)(char)x; return literal(F_MASKEXT16|fmt);
	case DAQ: if( !x ) x = 8; m = x; return literal(fmt);
	case DIMH:						// fall thru
	case DIM: if( (addr&H)
		   || addr==AIMM ) { m = immval; return literal(F_MASKEXT16|fmt);}
		  return "DIM";
	case DEAM:mode = x&7; reg = x>>3; return ea();
	case DBCC:m = m.sht+next; next += 2; return symbolic();
	case D2L: return "D2L";
	case D2H: reg = (opword1>>12)&15;
		  return regarg("%s%s", fmt&F_SYMBOLIC|F_NONE);
	case DBH: return dbh();
	case DBL: return dbl();
	case DCR: return dcr();
	}
	return Instr::arg(i);
}

int M68kInstr.nargs()
{
	if( !tab->addr1 ) return 0;
	if( !tab->addr2 ) return 1;
	if( !tab->addr3 ) return 2;
	if( !tab->addr4 ) return 3;
	return 4;
}

int M68kInstr.argtype(int)
{
	return 0;
}

struct m68koptab M68kOpTab[] = {

{0140400,"abcd",B,	SIG,	AREG+D,	DRG,	AREG+D,	DRGL,	0},
{0140410,"abcd",B,	SIG,	ADEC,	DRG,	ADEC,	DRGL,	0},

{0050000,"add",	BWL,	SD,	AIMM+Q,	DAQ,	AGEN+AL,DEA,	0},
{0003000,"add",	BWL,	SD,	AIMM,	DIM,	AGEN+AL+DA,DEA,	0},
{0150000,"add",	BWL,	SD,	AGEN,	DEA,	AREG+D,	DRGL,	0},
{0150400,"add",	BWL,	SD,	AREG+D,	DRGL,	AGEN+AM,DEA,	0},
{0150300,"add",	W,	SIG,	AGEN,	DEA,	AREG+A,	DRGL,	0},
{0150700,"add",	L,	SIG,	AGEN,	DEA,	AREG+A,	DRGL,	ISL},

{0150400,"addx",BWL,	SD,	AREG+D,	DRG,	AREG+D,	DRGL,	0},
{0150410,"addx",BWL,	SD,	ADEC,	DRG,	ADEC,	DRGL,	0},

{0001174,"and",	W,	SIG,	AIMM,	DIM,	AREG+SR,DIG,	0},
{0001074,"and",	B,	SIG,	AIMM,	DIM,	AREG+C,	DIG,	0},
{0001000,"and",	BWL,	SD,	AIMM,	DIM,	AGEN+AL+DA,DEA,	0},
{0140000,"and",	BWL,	SD,	AGEN+DA,DEA,	AREG+D,	DRGL,	0},
{0140400,"and",	BWL,	SD,	AREG+D,	DRGL,	AGEN+AM,DEA,	0},

{0160700,"asl",	W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160400,"asl",	BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160440,"asl",	BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0160300,"asr",	W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160000,"asr",	BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160040,"asr",	BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0165300,"bfchg",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL},
{0166300,"bfclr",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL},
{0165700,"bfexts",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL,	AREG+D,	D2H},
{0164700,"bfextu",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL,	AREG+D,	D2H},
{0166700,"bfffo",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL,	AREG+D,	D2H},
{0167700,"bfins",0,	0,	AREG+D,	D2H,	AGEN+BF,DEA,	I2W,	0,	ABIT,	DBH,	ABIT,	DBL},
{0167300,"bfset",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL},
{0164300,"bftst",0,	0,	AGEN+BF,DEA,	ABIT,	DBH,	I2W,	0,	ABIT,	DBL},

//{0060000,"br",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0060000,"bra",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0061000,"bhi",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
//{0062000,"bhs",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0062000,"bcc",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0063000,"bne",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0064000,"bvc",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0065000,"bpl",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0066000,"bge",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0067000,"bgt",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0061400,"bls",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
//{0062400,"blo",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0062400,"bcs",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0063400,"beq",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0064400,"bvs",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0065400,"bmi",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0066400,"blt",	B,	SIG,	AEXP,	DBR,	0,	0,	0},
{0067400,"ble",	B,	SIG,	AEXP,	DBR,	0,	0,	0},

{0000500,"bchg",0,	0,	AREG+D,	DRGL,	AGEN+AL+DA,DEA,	0},
{0004100,"bchg",0,	0,	AIMM+H,	DIM,	AGEN+AL+DA,DEA,	0},

{0000600,"bclr",0,	0,	AREG+D,	DRGL,	AGEN+AL+DA,DEA,	0},
{0004200,"bclr",0,	0,	AIMM+H,	DIM,	AGEN+AL+DA,DEA,	0},

{0000700,"bset",0,	0,	AREG+D,	DRGL,	AGEN+AL+DA,DEA,	0},
{0004300,"bset",0,	0,	AIMM+H,	DIM,	AGEN+AL+DA,DEA,	0},

{0060400,"bsr",	B,	SIG,	AEXP,	DBR,	0,	0,	0},

{0000400,"btst",0,	0,	AREG+D,	DRGL,	AGEN+DA,DEA,	0},
{0004000,"btst",0,	0,	AIMM+H,	DIM,	AGEN+DA,DEA,	0},

{0040600,"chk",	W,	SIG,	AGEN,	DEA,	AREG+D,	DRGL,	0},

{0041000,"clr",	BWL,	SD,	AGEN+AL+DA,DEA,	0,	0,	0},

{0006000,"cmp",	BWL,	SD,	AGEN+AL+DA,DEA,	AIMM,	DIM,	0},
{0130000,"cmp",	BWL,	SD,	AREG+D,	DRGL,	AGEN,	DEA,	0},
{0130300,"cmp",	W,	SIG,	AREG+A,	DRGL,	AGEN,	DEA,	0},
{0130700,"cmp",	L,	SIG,	AREG+A,	DRGL,	AGEN,	DEA,	ISL},
{0130410,"cmp",	BWL,	SD,	AINC,	DRGL,	AINC,	DRG,	0},

{0050310,"dbt",	0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0051310,"dbhi",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0052310,"dbhs",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0053310,"dbne",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0052310,"dbcc",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0054310,"dbvc",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0055310,"dbpl",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0056310,"dbge",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0057310,"dbgt",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0051710,"dbls",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0050710,"dbr",	0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0050710,"dbf",	0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0050710,"dbra",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0052710,"dblo",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0053710,"dbeq",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0052710,"dbcs",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0054710,"dbvs",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0055710,"dbmi",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0056710,"dblt",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},
{0057710,"dble",0,	0,	AREG+D,	DRG,	AEXP,	DBCC,	0},

{0100700,"divs",W,	SIG,	AGEN+DA,DEA,	AREG+D,	DRGL,	0},
/*
 * tricky stuff here.  divs.l is really a 3 operand instruction but it can be
 * parsed as a two operand one, in which case the second argument is copied
 * in outins() and the remainder is discarded.  honest, read the 68020 manual.
 */
{0046100,"divs",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0004000,	AREG+D,	D2H},
{0046100,"divs3",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0004000,	AREG+D,	D2H},

{0046100,"divsl",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0006000,	AREG+D,	D2H},

{0100300,"divu",W,	SIG,	AGEN+DA,DEA,	AREG+D,	DRGL,	0},
{0046100,"divu",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0000000,	AREG+D,	D2H},
{0046100,"divu3",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0000000,	AREG+D,	D2H},

{0046100,"divul",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0002000,	AREG+D,	D2H},

{0005000,"eor",	BWL,	SD,	AIMM,	DIM,	AGEN+AL+DA,DEA,	0},
{0005074,"eor",	B,	SIG,	AIMM,	DIM,	AREG+C,	DIG,	0},
{0005174,"eor",	W,	SIG,	AIMM,	DIM,	AREG+SR,DIG,	0},
{0130400,"eor",	BWL,	SD,	AREG+D,	DRGL,	AGEN+AL+DA,DEA,	0},

{0140500,"exg",	0,	0,	AREG+D,	DRG,	AREG+D,	DRGL,	0},
{0140510,"exg",	0,	0,	AREG+A,	DRG,	AREG+A,	DRGL,	0},
{0140610,"exg",	0,	0,	AREG+A,	DRG,	AREG+D,	DRGL,	0},
{0140610,"exg",	0,	0,	AREG+D,	DRGL,	AREG+A,	DRG,	0},

{0044200,"ext",	W,	SIG,	AREG+D,	DRG,	0,	0,	0},
{0044300,"ext",	L,	SIG,	AREG+D,	DRG,	0,	0,	0},
{0044700,"extb",L,	SIG,	AREG+D,	DRG,	0,	0,	0},

{0047300,"jmp",	0,	0,	AGEN+CT,DEA,	0,	0,	0},

{0047200,"jsr",	0,	0,	AGEN+CT,DEA,	0,	0,	0},

{0040700,"lea",	L,	SIG,	AGEN+CT,DEA,	AREG+A,	DRGL,	0},

{0047120,"link",0,	0,	AREG+A,	DRG,	AIMM+H,	DIM,	0},

{0161700,"lsl",	W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160410,"lsl",	BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160450,"lsl",	BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0161300,"lsr",	W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160010,"lsr",	BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160050,"lsr",	BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0070000,"mov",	L,	SIG,	AIMM+M,	DMQ,	AREG+D,	DRGL,	0},
{0030000,"mov",	W,	SIG,	AGEN,	DEA,	AGEN+AL+DA,DEAM,0},
{0020000,"mov",	L,	SIG,	AGEN,	DEA,	AGEN+AL+DA,DEAM,0},
{0010000,"mov",	B,	SIG,	AGEN,	DEA,	AGEN+AL+DA,DEAM,0},
{0030100,"mov",	W,	SIG,	AGEN,	DEA,	AREG+A,	DRGL,	0},
{0020100,"mov",	L,	SIG,	AGEN,	DEA,	AREG+A,	DRGL,	ISL},
{0040300,"mov",	W,	SIG,	AREG+SR,DIG,	AGEN+AL+DA,DEA,	0},
{0043300,"mov",	W,	SIG,	AGEN+DA,DEA,	AREG+SR,DIG,	0},
{0042300,"mov",	W,	SIG,	AGEN+DA,DEA,	AREG+C,	DIG,	0},
{0041300,"mov",	W,	SIG,	AREG+C,	DIG,	AGEN+AL+DA,DEA,	0},
{0047140,"mov",	L,	SIG,	AREG+A,	DRG,	AREG+U,	DIG,	0},
{0047150,"mov",	L,	SIG,	AREG+U,	DIG,	AREG+A,	DRG,	0},
{0047172,"mov",	L,	SIG,	AREG+CR,DCR,	AREG+D,	D2H,	I2W,	0000000},
{0047172,"mov",	L,	SIG,	AREG+CR,DCR,	AREG+A,	D2H,	I2W,	0100000},
{0047173,"mov",	L,	SIG,	AREG+D,	D2H,	AREG+CR,DCR,	I2W,	0000000},
{0047173,"mov",	L,	SIG,	AREG+A,	D2H,	AREG+CR,DCR,	I2W,	0100000},

{0044200,"movm",W,	SIG,	AIMM+H,	DIMH,	AGEN+AM,DEA,	0},
{0044300,"movm",L,	SIG,	AIMM+H,	DIMH,	AGEN+AM,DEA,	0},
{0046200,"movm",W,	SIG,	AGEN+AM,DEA,	AIMM+H,	DIMH,	0},
{0046300,"movm",L,	SIG,	AGEN+AM,DEA,	AIMM+H,	DIMH,	0},

{0007000,"movs",BWL,	SD,	AGEN+AM,DEA,	AREG+D,	D2H,	I2W,	0000000},
{0007000,"movs",BWL,	SD,	AGEN+AM,DEA,	AREG+A,	D2H,	I2W,	0100000},
{0007000,"movs",BWL,	SD,	AREG+D,	D2H,	AGEN+AM,DEA,	I2W,	0004000},
{0007000,"movs",BWL,	SD,	AREG+A,	D2H,	AGEN+AM,DEA,	I2W,	0104000},

/*
 * more tricky stuff.  D2L and D2H mean low and high register fields of
 * second instruction word, NOT low and high components of product.
 * read the manual.
 */
{0140700,"muls",W,	SIG,	AGEN+DA,DEA,	AREG+D,	DRGL,	0},
{0046000,"muls",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2H,	I2W,	0004000},
{0046000,"mulsl",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0006000,	AREG+D,	D2H},

{0140300,"mulu",W,	SIG,	AGEN+DA,DEA,	AREG+D,	DRGL,	0},
{0046000,"mulu",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2H,	I2W,	0000000},
{0046000,"mulul",L,	SIG,	AGEN+DA,DEA,	AREG+D,	D2L,	I2W,	0002000,	AREG+D,	D2H},

{0044000,"nbcd",B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},

{0042000,"neg",	BWL,	SD,	AGEN+AL+DA,DEA,	0,	0,	0},

{0040000,"negx",BWL,	SD,	AGEN+AL+DA,DEA,	0,	0,	0},

{0047161,"nop",	0,	0,	0,	0,	0,	0,	0},

{0043000,"not",	BWL,	SD,	AGEN+AL+DA,DEA,	0,	0,	0},

{0000174,"or",	W,	SIG,	AIMM,	DIM,	AREG+SR,DIG,	0},
{0000074,"or",	B,	SIG,	AIMM,	DIM,	AREG+C,	DIG,	0},
{0000000,"or",	BWL,	SD,	AIMM,	DIM,	AGEN+AL+DA,DEA,	0},
{0100000,"or",	BWL,	SD,	AGEN+DA,DEA,	AREG+D,	DRGL,	0},
{0100400,"or",	BWL,	SD,	AREG+D,	DRGL,	AGEN+AM,DEA,	0},

{0100500,"pack",0,	0,	AREG+D,	DRG,	AREG+D,	DRGL,	0,	0,	AIMM+H,	DIMH},
{0100510,"pack",0,	0,	ADEC,	DRG,	ADEC,	DRGL,	0,	0,	AIMM+H,	DIMH},
{0100600,"unpk",0,	0,	AREG+D,	DRG,	AREG+D,	DRGL,	0,	0,	AIMM+H,	DIMH},
{0100610,"unpk",0,	0,	ADEC,	DRG,	ADEC,	DRGL,	0,	0,	AIMM+H,	DIMH},

{0044100,"swap",W,	SIG,	AREG+D,	DRG,	0,	0,	0},
{0044100,"pea",	L,	SIG,	AGEN+CT,DEA,	0,	0,	0},

{0047160,"reset",0,	0,	0,	0,	0,	0,	0},

{0163700,"rol",	W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160430,"rol",	BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160470,"rol",	BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0163300,"ror",	W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160030,"ror",	BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160070,"ror",	BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0162700,"roxl",W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160420,"roxl",BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160460,"roxl",BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0162300,"roxr",W,	SIG,	AIMM+O,	DIG,	AGEN+AM,DEA,	ISH},
{0160020,"roxr",BWL,	SD,	AIMM+Q,	DAQ,	AREG+D,	DRG,	ISH},
{0160060,"roxr",BWL,	SD,	AREG+D,	DRGL,	AREG+D,	DRG,	ISH},

{0047163,"rte",	0,	0,	0,	0,	0,	0,	0},

{0047167,"rtr",	0,	0,	0,	0,	0,	0,	0},

{0047165,"rts",	0,	0,	0,	0,	0,	0,	0},

{0100400,"sbcd",B,	SIG,	AREG+D,	DRG,	AREG+D,	DRGL,	0},
{0100410,"sbcd",B,	SIG,	ADEC,	DRG,	ADEC,	DRGL,	0},

{0050300,"st",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0051300,"shi",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0052300,"shs",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0053300,"sne",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0052300,"scc",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0054300,"svc",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0055300,"spl",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0056300,"sge",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0057300,"sgt",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0050700,"sf",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0051700,"sls",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0052700,"slo",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0053700,"seq",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0052700,"scs",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0054700,"svs",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0055700,"smi",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0056700,"slt",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},
{0057700,"sle",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},

{0047162,"stop",0,	0,	AIMM+H,	DIMH,	0,	0,	0},

{0050400,"sub",	BWL,	SD,	AIMM+Q,	DAQ,	AGEN+AL,DEA,	0},
{0002000,"sub",	BWL,	SD,	AIMM,	DIM,	AGEN+AL+DA,DEA,	0},
{0110000,"sub",	BWL,	SD,	AGEN,	DEA,	AREG+D,	DRGL,	0},
{0110400,"sub",	BWL,	SD,	AREG+D,	DRGL,	AGEN+AM,DEA,	0},
{0110300,"sub",	W,	SIG,	AGEN,	DEA,	AREG+A,	DRGL,	0},
{0110700,"sub",	L,	SIG,	AGEN,	DEA,	AREG+A,	DRGL,	ISL},

{0110400,"subx",BWL,	SD,	AREG+D,	DRG,	AREG+D,	DRGL,	0},
{0110410,"subx",BWL,	SD,	ADEC,	DRG,	ADEC,	DRGL,	0},

{0045300,"tas",	B,	SIG,	AGEN+AL+DA,DEA,	0,	0,	0},

{0047100,"trap",0,	0,	AIMM+V,	DMQ,	0,	0,	0},

{0047166,"trapv",0,	0,	0,	0,	0,	0,	0},

{0045000,"tst",	BWL,	SD,	AGEN+AL+DA,	DEA,	0,	0,	0},

{0047130,"unlk",0,	0,	AREG+A,	DRG,	0,	0,	0},
{0, 0},
};

char *M68kInstr.mnemonic()
{
	opword0 = _asm->core->peekcode(addr)->sht;
	opword1 = _asm->core->peekcode(next = addr+2)->sht;
	for( tab = M68kOpTab; tab->opname; ++tab ){
		short m = sizemask();
		m |= OpMask[tab->a1disp&15] | OpMask[tab->a2disp&15];
		if( (short)(opword0 & ~m) == tab->opcode ){
			next += imm_bytes(tab->addr1, tab->a1disp)
			      + imm_bytes(tab->addr2, tab->a2disp);
			if( tab->iflag == I2W ) next = addr+4;
			return sf("%s%s", tab->opname, dotbwl());
		}
	}
	return sf("0%o", opword0);
}
