static char *ID_gencode="@(#) gencode.c: 1.22 5/8/84";
#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "instab.h"
#include "parse.h"
#include "gendefs.h"
#include "expand.h"
#include "expand2.h"

long
	swap_b2(),
	swap_b4();
extern upsymins
	*lookup();
extern short
	opt;	/* no optimize flag */
extern unsigned short
	line;	/* current line number */

addrgen(insptr,addr,expand,opnum)
register instr
	*insptr;
register addrmode
	*addr;
short	expand, opnum;
{
	long	val;
	register short reg;

		/* check if USER specified an expand byte */
		/* check if USER specified an operand specifier */
	if (insptr->tag & IS25) {
		if (addr->newtype != NOTYPE)
			yyerror("Expand byte invalid in IS25 instruction");
		if (addr->expspec != NULLSPEC)
			yyerror("Operand specifier invalid in IS25 instruction");
	} else
		expand = addr->newtype;
	if (expand != NOTYPE)
		generate(8,NOACTION,(long)((CEXPAND<<4)|expand),NULLSYM);

	switch(addr->admode) {

	case REGDFMD: /* register deferred mode */
		reg = addr->adreg;
		if (reg != PCREG) {
			generate(8,NOACTION,(long)((CREGDFMD<<4)|reg),NULLSYM);
		} else {
			yyerror("Register deferred mode illegal with register 15 (%pc).");
		}
		return;

	case DSPMD: /* displacement mode */
		val = addr->adexpr.expval;
		reg = addr->adreg;
		if ((addr->adexpr.symptr ==  NULLSYM)
		    && (addr->expspec == NULLSPEC)
		    && opt) {
		     /* No operand specifier, and optimization allowed, and
			symbol ptr is null */
			if ((val == 0) && (reg != PCREG)) {
				/* register deferred mode */
				/* not legal for register 15 or PC */
				generate(8,NOACTION,(long)((CREGDFMD<<4)|reg),
					NULLSYM);
				return;
			} /* register deferred mode */
 
 			if ((reg == APREG) && (0 <= val && val <= 14)) {
 				 /* AP displacement mode */
 				generate(8,NOACTION,(CAPDSPMD<<4)|val,NULLSYM);
 				return;
 			} /* AP displacement mode */

 			if ((reg == FPREG) && (0 <= val && val <= 14)) {
 				 /* FP displacement mode */
 				generate(8,NOACTION,(CFPDSPMD<<4)|val,NULLSYM);
 				return;
 			} /* FP displacement mode */
 
			if (-(1<<7) <= val && val < 1<<7) {
				/* byte displacement mode */
				generate(8,NOACTION,(long)((CBDSPMD<<4)|reg),
					NULLSYM);
				generate(8,NOACTION,val,NULLSYM);
				return;
			} /* byte displacement mode */
			if (-(1L<<15) <= val && val < 1L<<15) {
				/* halfword displacement mode */
				generate(8,NOACTION,(long)((CHDSPMD<<4)|reg),
					NULLSYM);
				generate(16,NOACTION,swap_b2(val),NULLSYM);
				return;
			} /* halfword displacement mode */
			/* Otherwise use word displacement mode */
			generate(8,NOACTION,(long)((CDSPMD<<4)|reg),NULLSYM);
			generate(32,NOACTION,swap_b4(val),NULLSYM);
			return;
		} /* addr->adexpr.symptr == NULLSYM */

		if ( (addr->adexpr.symptr != NULLSYM) 
		     || ((!opt) && (addr->expspec == NULLSPEC))) {
			/* don't know displacement;
				 must use word displacement mode */
			generate(8,NOACTION,(long)((CDSPMD<<4)|reg),NULLSYM);
			generate(32,RELDIR32,val,addr->adexpr.symptr);
			return;
		} 

		/* At this point the user must be specifying the operand 
		   expression size through the use of an operand specifier.
		   Generate the correct operand. */

		switch ( addr->expspec ) {

		case BYTESPEC:
			if ( (unsigned) val >= (1<<8)) {
				yyerror("Expression larger than byte.");
			} else {
				/* byte displacement mode */
				generate(8,NOACTION,(long)((CBDSPMD<<4)|reg),
					 NULLSYM);
				generate(8,NOACTION,val,NULLSYM);
			}
			return;

		case HALFSPEC:
			if ( (unsigned) val >= (1<<16)) {
				yyerror("Expression larger than halfword.");
			} else {
				/* halfword displacement mode */
				generate(8,NOACTION,(long)((CHDSPMD<<4)|reg),
					 NULLSYM);
				generate(16,NOACTION,swap_b2(val),NULLSYM);
			}
			return;

		case WORDSPEC:
			generate(8,NOACTION,(long)((CDSPMD<<4)|reg),NULLSYM);
			generate(32,NOACTION,swap_b4(val),NULLSYM);
			return;

		case SHORTSPEC:
 			if (0 > val || val > 14) {
				yyerror("Expression outside short offset range.");
 				return;
			}
			if (reg == APREG) {
 				 /* AP displacement mode */
 				generate(8,NOACTION,(CAPDSPMD<<4)|val,NULLSYM);
 				return;
 			} /* AP short offset */

 			if (reg == FPREG) {
 				 /* FP displacement mode */
 				generate(8,NOACTION,(CFPDSPMD<<4)|val,NULLSYM);
 				return;
 			} /* FP displacement mode */

			yyerror("Illegal register for short offset address mode.");
 			return;

		default:
			yyerror("Illegal operand specifier for displacement address mode.");
			return;

		} /* switch */

	case DSPDFMD: /* displacement deferred mode */
		val = addr->adexpr.expval;
		reg = addr->adreg;
		if ( (addr->adexpr.symptr ==  NULLSYM)
		     && (addr->expspec == NULLSPEC)
		     &&  opt) {
			if (-(1<<7) <= val && val < 1<<7) {
				/* byte displacement deferred mode */
				generate(8,NOACTION,(long)((CBDSPDFMD<<4)|reg),
					NULLSYM);
				generate(8,NOACTION,val,NULLSYM);
				return;
			} /* byte displacement deferred mode */
			if (-(1L<<15) <= val && val < 1L<<15) {
				/* halfword displacement deferred mode */
				generate(8,NOACTION,(long)((CHDSPDFMD<<4)|reg),
					NULLSYM);
				generate(16,NOACTION,swap_b2(val),NULLSYM);
				return;
			} /* halfword displacement deferred mode */
			/* Otherwise use word displacement deferred mode */
			generate(8,NOACTION,(long)((CDSPDFMD<<4)|reg),NULLSYM);
			generate(32,NOACTION,swap_b4(val),NULLSYM);
			return;
		} /* addr->adexpr.symptr == NULLSYM */

		if ( (addr->adexpr.symptr != NULLSYM)
		     || ((!opt) && (addr->expspec == NULLSPEC))) {

			/* don't know displacement;
				must use word displacement deferred mode */
			generate(8,NOACTION,(long)((CDSPDFMD<<4)|reg),NULLSYM);
			generate(32,RELDIR32,val,addr->adexpr.symptr);
			return;
		}

		/* At this point the user must be specifying the operand
		   expression size through the use of an operand specifier.
		   Generate the correct operand. */

		switch ( addr->expspec ) {

		case BYTESPEC:
			if ( (unsigned) val >= (1<<8)) {
				yyerror("Expression larger than byte.");
			} else {
				/* byte displacement deferred mode */
				generate(8,NOACTION,(long)((CBDSPDFMD<<4)|reg),
					NULLSYM);
				generate(8,NOACTION,val,NULLSYM);
			}
			return;

		case HALFSPEC:
			if ( (unsigned) val >= (1<<16)) {
				yyerror("Expression larger than halfword.");
			} else {
				/* halfword displacement deferred mode */
				generate(8,NOACTION,(long)((CHDSPDFMD<<4)|reg),
					NULLSYM);
				generate(16,NOACTION,swap_b2(val),NULLSYM);
			}
			return;

		case WORDSPEC:
			generate(8,NOACTION,(long)((CDSPDFMD<<4)|reg),NULLSYM);
			generate(32,NOACTION,swap_b4(val),NULLSYM);
			return;

		default:
			yyerror("Illegal operand specifier for displacement deferred address mode.");
			return;

		} /* switch */

	case EXADMD: /* external address mode */
	case ABSMD: /* absolute address mode */
		generate(8,NOACTION,(long)(CABSMD),NULLSYM);
		generate(32,RELDIR32,addr->adexpr.expval,addr->adexpr.symptr);
		return;

	case EXADDFMD: /* external address deferred mode (PC relative deferred) */
	case ABSDFMD: /* absolute address deferred mode */
		generate(8,NOACTION,(long)(CABSDFMD),NULLSYM);
		generate(32,RELDIR32,addr->adexpr.expval,addr->adexpr.symptr);
		return;

	case IMMD: /* immediate mode */
		val = addr->adexpr.expval;
		if ( (addr->adexpr.symptr ==  NULLSYM)
		     && (addr->expspec == NULLSPEC)
		     &&  opt) {
 			if ( val < 0 ) {
				if ( (insptr->tag & IS25) &&
				     (insptr->name[0] == 'u') )
				{
				   generate(8,NOACTION,(long)CIMMD,NULLSYM);
				   generate(32,NOACTION,swap_b4(val),NULLSYM);
				   return;
				}
 			}
 
 			if (-(1<<4) <= val && val < 1<<6) {
 				generate(8,NOACTION,val,NULLSYM);
 				return;
 			}
 
			if (-(1<<7) <= val && val < 1<<7) {
				/* byte immediate mode */
				generate(8,NOACTION,(long)CBIMMD,NULLSYM);
				generate(8,NOACTION,val,NULLSYM);
				return;
			} /* byte immediate mode */
			if (-(1L<<15) <= val && val < 1L<<15) {
				/* halfword immediate mode */
				generate(8,NOACTION,(long)CHIMMD,NULLSYM);
				generate(16,NOACTION,swap_b2(val),NULLSYM);
				return;
			} /* halfword immediate mode */
			/* Otherwise use fullword immediate mode */
			generate(8,NOACTION,(long)CIMMD,NULLSYM);
			generate(32,NOACTION,swap_b4(val),NULLSYM);
			return;
		} /* addr->adexpr.symptr == NULLSYM */

		if ( (addr->adexpr.symptr != NULLSYM)
		     || ((!opt) && (addr->expspec == NULLSPEC))) {

			/* don't know value; 
				must use fullword immediate mode */
			generate(8,NOACTION,(long)CIMMD,NULLSYM);
			generate(32,RELDIR32,val,addr->adexpr.symptr);
			return;
		}

		/* At this point the user must be specifying the operand
		   expression size through the use of an operand specifier.
		   Generate the correct operand. */

		switch ( addr->expspec ) {

		case BYTESPEC:
			if ( (unsigned) val >= (1<<8)) {
				yyerror("Expression larger than byte.");
			} else {
				/* byte immediate mode */
				generate(8,NOACTION,(long)CBIMMD,NULLSYM);
				generate(8,NOACTION,val,NULLSYM);
			}
			return;

		case HALFSPEC:
			if ( (unsigned) val >= (1<<16)) {
				yyerror("Expression larger than halfword.");
			} else {
				/* halfword immediate mode */
				generate(8,NOACTION,(long)CHIMMD,NULLSYM);
				generate(16,NOACTION,swap_b2(val),NULLSYM);
			}
			return;

		case WORDSPEC:
			/* fullword immediate mode */
			generate(8,NOACTION,(long)CIMMD,NULLSYM);
			generate(32,NOACTION,swap_b4(val),NULLSYM);
			return;

		case LITERALSPEC:
 			if (-(1<<4) <= val && val < 1<<6) {
 				generate(8,NOACTION,val,NULLSYM);
			} else {
				yyerror("Expression larger than literal.");
			}
			return;

		default:
			yyerror("Illegal operand specifier for immediate address mode.");
			return;

		} /* switch */

#if	FLOAT
	case FPIMMD: /* fp immediate, same as IMMD but w/o optimization */
		if ((val=optype(insptr->tag,opnum)) == 3)  /* 64 bit (double) */
			generate(8,NOACTION,(long)CDIMMD,NULLSYM);
		else		/* 32 bit (single) */
			generate(8,NOACTION,(long)CIMMD,NULLSYM);
		generate(32,NOACTION,swap_b4(addr->adexpr.expval),addr->adexpr.symptr);
		if (val == 3)		/* generate second 32 bits */
			generate(32,NOACTION,swap_b4(addr->adexpr.fdexpval2),addr->adexpr.symptr);
		break;
#endif

	case REGMD: /* register mode */
		reg = addr->adreg;
		if ( reg == PCREG ) {
			yyerror("Register mode illegal with register 15 (%pc).");
		} else {
			generate(8,NOACTION,(long)((CREGMD<<4)|reg),NULLSYM);
		}
		return;

	default:
		yyerror("Assembler error: invalid addressing mode");
		return;
	} /* switch */
} /* addrgen */

long
swap_b2(val)
long val;
{
	long b1, b2;

	b1 = (val >> 8) & 0x00FFL;	/* 2nd low goes to low */
	b2 = (val << 8) & 0xFF00L;	/* low goes to high */
	return (b2|b1);
} /* swap_b2 */

long
swap_b4(val)
long val;
{
	long b1, b2, b3, b4;

	b1 = (val >> 24) & 0x000000FFL;	/* high goes to low */
	b2 = (val >>  8) & 0x0000FF00L;	/* 2nd high goes to 2nd low */
	b3 = (val <<  8) & 0x00FF0000L;	/* 2nd low goes to 2nd high */
	b4 = (val << 24) & 0xFF000000L;	/* low goes to high */
	return (b1|b2|b3|b4);
} /* swap_b4 */

optype(instype,opnum)
BYTE instype;
short opnum;
{
/*
 * figure out operand type according to instruction type and operand number
 * return:	0=byte	1=half	2=word	3=double
 */

	switch (instype & (~IS25)) {

	case INSTRB:
		return 0;

	case INSTRH:
		return 1;

	case INSTRW:
		return 2;

#if	FLOAT
	case INSTRD:
		return 3;
#endif

	case INSTRBH:
		return (opnum==1) ? 0 : 1;

	case INSTRBW:
		return (opnum==1) ? 0 : 2;

	case INSTRHW:
		return (opnum==1) ? 1 : 2;

	case INSTRWH:
		return (opnum==1) ? 2 : 1;

	case INSTRWB:
		return (opnum==1) ? 2 : 0;

	case INSTRHB:
		return (opnum==1) ? 1 : 0;

	default:		/* should not happen, but just in case */
		return 2;	/* treat as word operand */
	}
} /* optype */

convgen(insptr,addr1,addr2)
instr	*insptr;
register addrmode
	*addr1,
	*addr2;
{
	/*
	 *
	 * movb[bh|bw|hw] and movt[wb|hb|wh] instructions;
	 *
	 * All are encoded as expand mode mappings except
	 * for movtXY with a register destination.
	 * These are encoded as follows:
	 *
	 *	movthb	X,%ry	-> andh3 &0xFF,X,%ry
	 *	movtwb	X,%ry	-> andw3 &0xFF,X,%ry
	 *	movtwh	X,%ry	-> movw  X,%ry ; movh %ry,%ry
	 *
	 */

	static addrmode maskop = {NOTYPE,IMMD,0x01,{ABS,NULLSYM,0xFFL}};
	register short	expand;
	instr	*newins;		/* set up dummy operands for movtwb */
	char	*maskinst = "andw3";

	/* check for movtXY with a register dest. */
	if (addr2->admode == REGMD)
		switch (insptr->tag & (~IS25)) {
		case INSTRHB:
			/* change instruction for movthb */
			maskinst = "andh3";
			/* fall through to ... */
		case INSTRWB:
			newins = (*lookup(maskinst,N_INSTALL,MNEMON)).itp;
			generate(8,NOACTION,newins->opcode,NULLSYM);
			addrgen(newins,&maskop,NOTYPE,1); /* mask operand */
			addrgen(insptr,addr1,NOTYPE,2);
			addrgen(insptr,addr2,UBYTE,3);
			return;
		case INSTRWH:
			generate(8,NOACTION,insptr->opcode,NULLSYM);  /* movw */
			addrgen(insptr,addr1,NOTYPE,1);
			addrgen(insptr,addr2,NOTYPE,2);
			generate(0,NEWSTMT,(long)line,NULLSYM);
			newins = (*lookup("movh",N_INSTALL,MNEMON)).itp;
			generate(8,NOACTION,newins->opcode,NULLSYM);
			addrgen(insptr,addr2,NOTYPE,1);
			addrgen(insptr,addr2,NOTYPE,2);
			return;
		} /* switch */

	generate(insptr->nbits,NOACTION,insptr->opcode,NULLSYM);
	addrgen(insptr,addr1,NOTYPE,1);
	switch (optype(insptr->tag,2)) {
		case 0: /* byte */
			expand = SBYTE;
			break;
		case 1: /* half */
			expand = SHALF;
			break;
		case 2: /* word */
			expand = SWORD;
			break;
	} /* switch */

	addrgen(insptr,addr2,expand,2);
} /* convgen */

testgen(insptr, addr1)
register	instr		*insptr;
register	addrmode	*addr1;
{
	/*
	 *	NOP2 NOP3
	 *
	 *		This routine will generate the filler bytes
	 *	for the above two nops.
	 */
	
	int	val;

	if ( addr1->newtype != NOTYPE ) {
		yyerror("Expand byte illegal with NOP2 or NOP3 instructions.");
		return;
	}

	if ( addr1->expspec != NULLSPEC ) {
		yyerror("Operand specifier illegal with NOP2 or NOP3 instructions.");
		return;
	}

	generate( insptr->nbits, NOACTION, insptr->opcode, NULLSYM);
	val = addr1->adexpr.expval;

	if ( insptr->tag ==  INSTRB ) {
		if ((unsigned) val >= (1L << 8)) {
		      yyerror("Optional immediate for NOP2 larger than byte.");
		} else {
			generate( 8, NOACTION, val, NULLSYM);
		}
	} else {
		if ((unsigned) val >= (1L << 16)) {
		      yyerror("Optional immediate for NOP3 larger than halfword.");
		} else {
			generate(16, NOACTION, val, NULLSYM);
		}
	}
} /* testgen */

opt21gen(insptr,addr1,addr2,constant)
instr	*insptr;
register addrmode
	*addr1,
	*addr2;
short	constant;
{
	/*
	 * This function attempts to optimize certain dyadic instructions
	 * into monadics when the first operand is a particular
	 * immediate constant.
	 *
	 * The possible optimizations are:
	 *
	 * Dyadic	Constant	Monadic
	 *
	 * mov[bhw]	0		CLR[BHW]
	 * add[bhw]2	1		INC[BHW]
	 * sub[bhw]2	1		DEC[BHW]
	 * cmp[bhw]	0		TST[BHW]
	 *
	 *
	 * In all cases the dyadic opcode is stored in the low byte
	 * of the opcode field of the instruction table (pointed to by insptr)
	 * and the monadic opcode is stored in the 2nd low byte
	 *
	 */

	if (addr1->admode==IMMD && addr1->adexpr.exptype==ABS &&
		addr1->adexpr.expval==(long)constant &&
		(insptr->tag & IS25) && opt)
	{
		/* generate monadic opcode */
		generate(8,NOACTION,(insptr->opcode >> 8),NULLSYM);
		addrgen(insptr,addr2,NOTYPE,2);
	}
	else {
		/* just do normal dyadic instruction */
		/* mask off 2nd low byte, but don't really have to */
		generate(8,NOACTION,insptr->opcode & 0xFFL,NULLSYM);
		addrgen(insptr,addr1,NOTYPE,1);
		addrgen(insptr,addr2,NOTYPE,2);
	} /* else */
} /* opt21gen */

jmpopgen(insptr,opnd1)
instr	*insptr;
addrmode	*opnd1;
{
	instr	*newins;
	long	opcd, newopcd;
	BYTE	nbits;
	unsigned short	action;
	short	jmptype;

	opcd = insptr->opcode;

	switch (insptr->tag) {
	case INSTRB: /* byte displacement */
		nbits = 8;
		if (opnd1->admode != EXADMD)
			yyerror("Invalid operand");
		action = RELPC8;
		break;
	case INSTRH: /* halfword displacement */
		if (opnd1->admode != EXADMD)
			yyerror("Invalid operand");
		nbits = 16;
		action = RELPC16;
		break;
	case IS25|INSTRW: /* IS25, word displacement */
		if (opnd1->admode != EXADMD) {
			/* can't generate PC relative */
			if ((newopcd = opcd >> 8) != 0) {
				/* generate complementary branch */
				generate(8,NOACTION,newopcd,NULLSYM);
				generate(8,FNDBRLEN,0L,NULLSYM);
			} /* (newopcd = opcd >> 8) != 0 */
			newins = (*lookup("JMP",N_INSTALL,MNEMON)).itp;
			generate(8,NOACTION,newins->opcode,NULLSYM);
			addrgen(insptr,opnd1,NOTYPE,1);
			return;
		} /* opnd1->admode != EXADMD */
		jmptype = (opcd >> 8) != 0;
		/* 1 means conditional; 0 unconditional */
		if (opt) switch(shortsdi(opnd1->adexpr.symptr,
				opnd1->adexpr.expval,
				jmptype ? CBRB : UBRB))
		{
		case S_SDI: /* definitely 8-bit short */
			generate(8,NOACTION,opcd & 0xFFL,NULLSYM);
			generate(8,RELPC8,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		case U_SDI: /* don't know; generate short and flag */
			generate(8,BRNOPT,opcd,NULLSYM);
			generate(8,RELPC8,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		/* case L_SDI: fall through */
		} /* if (opt) switch (...) */

		/* try 16-bit branch offset */
		switch(shortsdi(opnd1->adexpr.symptr,
				opnd1->adexpr.expval,
				jmptype ? CBRH : UBRH))
		{
		case S_SDI: /* definitely 16-bit short */
			generate(8,NOACTION,(opcd & 0xFFL) - 1L,
				NULLSYM);
			generate(16,RELPC16,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		case U_SDI: /* unknown; generate 16-bit short (interm.) and flag */
			/* convert opcode(s) from short to intermed. */
			if ((opcd >> 8) != 0)
				/* subtract 1 from both opcodes */
				opcd -= 0x0101L;
			else
				/* subtract 1 from opcode */
				--opcd;
			generate(8,BRNOPT,opcd,NULLSYM);
			generate(16,RELPC16,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		case L_SDI:
			/* test for unconditional jump */
			if ((newopcd = opcd >> 8) != 0) {
				/* generate complementary branch */
				generate(8,NOACTION,newopcd,NULLSYM);
				generate(8,FNDBRLEN,0L,NULLSYM);
				generate(0,NEWSTMT,(long)line,NULLSYM);
			}
			newins = (*lookup("JMP",N_INSTALL,MNEMON)).itp;
			generate(8,NOACTION,newins->opcode,NULLSYM);
			addrgen(insptr,opnd1,NOTYPE,1);
			return;
		} /* switch */
	case INSTRW: /* word displacement */
			generate(8,NOACTION,opcd,NULLSYM);
			addrgen(insptr,opnd1,NOTYPE,1);
			return;
	default:
		aerror("assembler error; invalid operator type");
	} /* switch */

	generate(8,NOACTION,opcd & 0xFFL,NULLSYM); /* not necessary to mask */
	generate(nbits,action,opnd1->adexpr.expval,opnd1->adexpr.symptr);
} /* jmpopgen */

jsbopgen(insptr,opnd1)
instr	*insptr;
addrmode	*opnd1;
{
	long	opcd;

	opcd = insptr->opcode;

	if (opnd1->admode != EXADMD) {
		/* can't generate PC relative */
		generate(8,NOACTION,opcd >> 8,NULLSYM);
		addrgen(insptr,opnd1,NOTYPE,1);
	}
	else {
		if (opt) switch(shortsdi(opnd1->adexpr.symptr,
				opnd1->adexpr.expval,BSBB))
		{
		case S_SDI: /* definitely 8-bit short */
			generate(8,NOACTION,opcd & 0xFFL,NULLSYM);
			generate(8,RELPC8,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		case U_SDI: /* don't know; generate short and flag */
			generate(8,BSBNOPT,opcd,NULLSYM);
			generate(8,RELPC8,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		/* case L_SDI: fall through */
		} /* if (opt) switch (...) */

		/* try 16-bit branch offset */
		switch(shortsdi(opnd1->adexpr.symptr,
				opnd1->adexpr.expval,BSBH))
		{
		case S_SDI: /* definitely 16-bit short */
			generate(8,NOACTION,(opcd & 0xFFL) - 1L,NULLSYM);
			generate(16,RELPC16,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			return;
		case U_SDI: /* don't know; generate 16-bit short and flag */
			/* convert opcode from short to intermed. */
			--opcd;
			generate(8,BSBNOPT,opcd,NULLSYM);
			generate(16,RELPC16,opnd1->adexpr.expval,
				opnd1->adexpr.symptr);
			generate(8,NOACTION,opcd >> 8,NULLSYM);
			addrgen(insptr,opnd1,NOTYPE,1);
			return;
		case L_SDI:
			generate(8,NOACTION,opcd,NULLSYM);
			addrgen(insptr,opnd1,NOTYPE,2);
			return;
		} /* switch */
	} /* else */
} /* jsbopgen */

callgen(insptr,imm,dest)
instr	*insptr;
addrmode	*imm, *dest;
{
	/*
	 *
	 * call	  imm,dest
	 *
	 *	is encoded as
	 *
	 * CALL	-(4*imm)(%sp),dest
	 *
	 */

	static addrmode dispsp = {NOTYPE,DSPMD,SPREG,{ABS,NULLSYM,1L}};
	int	span;

	dispsp.adexpr.expval = - (4L * imm->adexpr.expval );
	generate(8,NOACTION,insptr->opcode,NULLSYM); /* CALL opcode */
	addrgen(insptr,&dispsp,NOTYPE,1);

#ifndef CALLPCREL
	addrgen(insptr,dest,NOTYPE,2);
#else
	if (dest->admode != EXADMD) {
		/* can't generate PC relative */
		addrgen(insptr,dest,NOTYPE,2);
	} else {
		if (opt) {
			if (sdi(dest->adexpr.symptr, dest->adexpr.expval,
				PCBRELCALL) == S_SDI) {
				span = shortsdi(dest->adexpr.symptr,
					dest->adexpr.expval,PCBRELCALL);
				if ( span == S_SDI) {
					/* byte displacement from PC */
					generate(8,NOACTION,0xCFL,NULLSYM);
					generate(8,RELPC8,dest->adexpr.expval,
						dest->adexpr.symptr);
				} else {
					yyerror("Assembler error with short span dependent procedure call.");
				}
				return;
			}
		}

/*		if (opt) switch(shortsdi(dest->adexpr.symptr,
/*				dest->adexpr.expval,PCBRELCALL))
/*		{
/*		case S_SDI: /* definitely 8-bit short */
/*			/* byte displacement from PC */
/*			generate(8,NOACTION,0xCFL,NULLSYM);
/*			generate(8,RELPC8,dest->adexpr.expval,
/*				dest->adexpr.symptr);
/*			return;
/*		case U_SDI: /* don't know; generate short and flag */
/*			/* byte displacement from PC */
/*			generate(8,CALLNOPT,0xCFL,NULLSYM);
/*			generate(8,RELPC8,dest->adexpr.expval,
/*				dest->adexpr.symptr);
/*			return;
/*		/* case L_SDI: fall through */
/*		} /* if (opt) switch (...) */

		/* try 16-bit branch offset */
		switch(shortsdi(dest->adexpr.symptr,
				dest->adexpr.expval,PCHRELCALL))
		{
		case S_SDI: /* definitely 16-bit short */
			/* halfword relative from PC */
			generate(8,NOACTION,0xAFL,NULLSYM);
			generate(16,RELPC16,dest->adexpr.expval,
				dest->adexpr.symptr);
			return;
		case U_SDI: /* don't know; generate 16-bit short and flag */
			/* halfword relative from PC */
			generate(8,CALLNOPT,0xAFL,NULLSYM);
			generate(16,RELPC16,dest->adexpr.expval,
				dest->adexpr.symptr);
			return;
		case L_SDI:
			/* CALL opcode */
			addrgen(insptr,dest,NOTYPE,2);
			return;
		} /* switch */
	} /* else */
#endif
} /* callgen */

loop1gen(insptr,index,incr,limit,dest)
instr	*insptr;
addrmode	*index, *incr, *limit, *dest;
{
	/*
	 *
	 * acX	index,incr,limit,dest
	 *	where X can be [jl|jlu|jle|jleu]
	 *
	 *	is encoded as
	 *
	 * addw2	incr,index
	 * cmpw 	index,limit
	 * X		dest
	 *
	 */

	instr	*newins;

		/* addw2 */
	newins = (*lookup("addw2",N_INSTALL,MNEMON)).itp;
	opt21gen(newins,incr,index,1);	/* optimized "add" */
	pcintcheck(newins,index);

		/* cmpw */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup("cmpw",N_INSTALL,MNEMON)).itp;
#if 0
	/* generate optimized "cmpw" and interchange operands */
	opt21gen(newins,limit,index,0);
#endif
	/* DONT generate optimized "cmpw" because of Interrupt
	 * After TSTW Chip Bug.
	 */
	generate(8,NOACTION,newins->opcode & 0xFFL,NULLSYM);
	addrgen(newins,limit,NOTYPE,1);
	addrgen(newins,index,NOTYPE,2);

		/* jump instr. */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup((insptr->name)+2,N_INSTALL,MNEMON)).itp;
	jmpopgen(newins,dest);
} /* loop1gen */

loop2gen(insptr,index,incr,dest)
instr	*insptr;
addrmode	*index, *incr, *dest;
{
	/*
	 *
	 * atjnz[bhw]	index,incr,dest
	 *
	 *	is encoded as
	 *
	 * addw2	incr,index
	 * TST[BHW]	*index
	 * jnz		dest
	 *
	 */

	instr	*newins;

		/* addw2 */
	newins = (*lookup("addw2",N_INSTALL,MNEMON)).itp;
	opt21gen(newins,incr,index,1);	/* optimized "add" */

		/* TST[BHW] */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	generate(8,NOACTION,insptr->opcode,NULLSYM); /* TST opcode */

	/*
	 * To generate TST *index,
	 * must add a level of indirection to "index"
	 *
	 */

	switch(index->admode) {

		case DSPMD:
		case EXADMD:
		case ABSMD:
			++(index->admode); /* adds a level of indirection */
			break;
		case REGMD:
			index->admode = DSPMD;
			index->adexpr.exptype = ABS;
			index->adexpr.expval = 0L;
			index->adexpr.symptr = NULLSYM;
			break;
		default:
			yyerror("Invalid `index' operand of `atjnz'");
			break;
	} /* switch */

	addrgen(insptr,index,NOTYPE,1);

		/* jnz */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup("jnz",N_INSTALL,MNEMON)).itp;
	jmpopgen(newins,dest);
} /* loop2gen */

blockgen(insptr)
instr	*insptr;
{
	/*
	 *
	 * movbl[bh]
	 *	is encoded as
	 *
	 * TSTW 	%r2
	 * BRB  	+12 (for half) or +14 (for byte)
	 * mov[bh]	(%r0),(%r1)
	 * NOP 
	 * addw2	&2,%r0 (for half) or INCW %r0 (for byte)
	 * addw2	&2,%r1 (for half) or INCW %r1 (for byte)
	 * DECW 	%r2
	 * BNEB 	-12 (for half) or -10 (for byte)
	 *
	 */

	instr	*newins;
	short	byte;

	if( (insptr->tag & SHFT_MSK) == INSTRB)
		byte = YES;
	else
		byte = NO;

		/* TSTW */
	newins = (*lookup("TSTW",N_INSTALL,MNEMON)).itp;
	generate(8,NOACTION,newins->opcode,NULLSYM);	/* TSTW opcode */
	generate(8,NOACTION,(long)(CREGMD<<4|0x02),NULLSYM); /* %r2 */

		/* BRB */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup("BRB",N_INSTALL,MNEMON)).itp;
	generate(8,NOACTION,newins->opcode,NULLSYM);	/* unc. branch */
	if (byte == YES)
		generate(8,NOACTION,12L,NULLSYM);
	else
		generate(8,NOACTION,14L,NULLSYM);

		/* mov[bh] */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	generate(8,NOACTION,insptr->opcode,NULLSYM);	/* mov[bh] */
	generate(8,NOACTION,(long)(CREGDFMD<<4|0x00),NULLSYM); /* (%r0) */
	generate(8,NOACTION,(long)(CREGDFMD<<4|0x01),NULLSYM); /* (%r1) */

	/*
	 *  A NOP must ne inserted here because a write to memory 
	 *  was just generated.
	 */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
	generate(newins->nbits, NOACTION, newins->opcode, NULLSYM);

	generate(0,NEWSTMT,(long)line,NULLSYM);
	if (byte == YES) {
		/* INCW - increment register 0 */
		/* INCW - increment register 1 */
	
		newins = (*lookup("INCW",N_INSTALL,MNEMON)).itp;
		generate(8,NOACTION,newins->opcode,NULLSYM);	/* INCW */
		generate(8,NOACTION,(long)(CREGMD<<4|0x00),NULLSYM); /* %r0 */
		generate(0,NEWSTMT,(long)line,NULLSYM);
		generate(8,NOACTION,newins->opcode,NULLSYM);	/* INCW */
		generate(8,NOACTION,(long)(CREGMD<<4|0x01),NULLSYM); /* %r1 */
	}
	else {

		/* addw2 - increment register 0 */
		/* addw2 - increment register 1 */
		newins = (*lookup("addw2",N_INSTALL,MNEMON)).itp;
		generate(8,NOACTION,newins->opcode,NULLSYM);	/* addw2 */
		generate(8,NOACTION,2L,NULLSYM);
		generate(8,NOACTION,(long)(CREGMD<<4|0x00),NULLSYM); /* %r0 */
		generate(0,NEWSTMT,(long)line,NULLSYM);
		generate(8,NOACTION,newins->opcode,NULLSYM);	/* addw2 */
		generate(8,NOACTION,2L,NULLSYM);
		generate(8,NOACTION,(long)(CREGMD<<4|0x01),NULLSYM); /* %r1 */
	}

		/* DECW */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup("DECW",N_INSTALL,MNEMON)).itp;
	generate(8,NOACTION,newins->opcode,NULLSYM);	/* DECW */
	generate(8,NOACTION,(long)(CREGMD<<4|0x02),NULLSYM); /* %r2 */

		/* BNEB */
	generate(0,NEWSTMT,(long)line,NULLSYM);
	newins = (*lookup("BNEB",N_INSTALL,MNEMON)).itp;
	generate(8,NOACTION,newins->opcode,NULLSYM);	/* BNEB opcode */
	if (byte == YES)
		generate(8,NOACTION,-10L,NULLSYM);
	else
		generate(8,NOACTION,-12L,NULLSYM);
} /* blockgen */

pushopgen(insptr,opnd1)
instr	*insptr;
addrmode	*opnd1;
{
	/*
	 *
	 * push[zb][bh]	opnd1
	 *
	 *	is encoded as
	 *
	 * addw2	&4,%sp
	 * followed by one of the following:
	 *
	 *	if opnd1 does not use %sp
	 * mov[bh]	{X}opnd1,{sword}-4(%sp)
	 *
	 *	if opnd1=%sp
	 * subw3	&4,{X}%sp,{sword}-4(%sp)
	 *
	 *	if opnd1=expr(%sp)
	 * mov[bh]	{X}expr-4(%sp),{sword}-4(%sp)
	 *
	 *	if opnd1=*expr(%sp)
	 * mov[bh]	{X}*expr-4(%sp),{sword}-4(%sp)
	 *
	 *	where X = signed for push.b
	 *		  unsigned for push.h
	 */

	static addrmode dispsp = {NOTYPE,DSPMD,SPREG,{ABS,NULLSYM,-4L}};
	static addrmode immdop = {NOTYPE,IMMD,0x04,{ABS,NULLSYM,4L}};
	instr	*newins;
	short	expand;

		/* addw2 */
	newins = (*lookup("addw2",N_INSTALL,MNEMON)).itp;
	generate(8,NOACTION,newins->opcode,NULLSYM);
	addrgen(insptr,&immdop,NOTYPE,1);
	generate(8,NOACTION,(long)((CREGMD<<4)|SPREG),NULLSYM); /* operand %sp */

		/* decide on which second instruction to generate */
	switch (opnd1->admode) {
	case REGMD:
		if (opnd1->adreg == SPREG) {	/* if opnd1 is %sp then */
						/* generate subw3 instr */
			newins = (*lookup("subw3",N_INSTALL,MNEMON)).itp;
			generate(8,NOACTION,newins->opcode,NULLSYM);
			addrgen(insptr,&immdop,NOTYPE,1);
			generate(8,NOACTION,(long)((CREGMD<<4)|SPREG),NULLSYM);	/* oprnd %sp */
			addrgen(insptr,&dispsp,SWORD,3); /* oprnd {sword}-4(%sp) */
			pcintcheck(insptr,&dispsp);
			return;
		} else
			break;
	case REGDFMD:
	case DSPMD:
	case DSPDFMD:
		/* if register is %sp, subtract 4 from displacement */
		if (opnd1->adreg == SPREG)
			opnd1->adexpr.expval -= 4;
		break;
	} /* switch */

		/* mov[bh] opcode */
	generate(8,NOACTION,insptr->opcode,NULLSYM);
	switch (insptr->tag & (~IS25)) {
		case INSTRBW:
			expand = SBYTE;
			break;
		case INSTRHW:
			expand = UHALF;
			break;
		default:
			expand = NOTYPE;
	} /* switch */

	addrgen(insptr,opnd1,expand,1);
	addrgen(insptr,&dispsp,SWORD,2);
	pcintcheck(insptr,&dispsp);
} /* pushopgen */

shftopgen(insptr,opnd1,opnd2,opnd3)
instr	*insptr;
addrmode	*opnd1, *opnd2, *opnd3;
{
	short	expand1, expand2;
	long	opcd, opcd2;

	opcd = insptr->opcode;
	if ((insptr->tag & IS25) &&
		(opnd1->admode != REGMD) && (opnd1->admode != IMMD))
	{
		expand2 = SWORD;
		/*
		 * The opcode field contains 2 8-bit opcodes.
		 * The lower opcode corresponds to the shift
		 * word opcode.  The higher opcode corresponds
		 * either to a shift word or a shift byte opcode.
		 * Since the shift length (opnd1) is a byte operand
		 * in IS25, the shift byte opcode will be used if
		 * there is one on the MAC32.  If there is no shift
		 * byte instruction, then the shift word will be used
		 * with an expand byte of signed-byte specified.
		 * Thus, test to see if the 2 opcodes are the same.
		 * If so, then the expand byte must be used.
		 */
		opcd2 = opcd & 0x000000FFL; /* lower 8 bits */
		if ((opcd >>= 8) == opcd2)
			expand1 = SBYTE;
		else
			expand1 = NOTYPE;
	}
	else
		expand1 = expand2 = NOTYPE;

	generate(8,NOACTION,opcd,NULLSYM);
	addrgen(insptr,opnd1,expand1,1);
	addrgen(insptr,opnd2,expand2,2);
	addrgen(insptr,opnd3,NOTYPE,3);
} /* shftopgen */
