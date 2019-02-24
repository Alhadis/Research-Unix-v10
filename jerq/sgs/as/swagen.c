static char *ID_swagen="@(#) swagen.c: 1.3 5/4/84";
#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "instab.h"
#include "parse.h"
#include "gendefs.h"
#include "expand.h"
#include "expand2.h"

#define MULOP 0
#define DIVOP 1
#define MODOP 2

#define SIGNED 0
#define UNSIGNED 1

extern upsymins
	*lookup();
extern short
	workaround, /* no software workaround flag */
	opt;	/* no optimize flag */
extern unsigned short
	line;	/* current line number */

/*
 * chip fix routines:
 * The following three routines (mtoregcheck, intaft1check, and intaft2check)
 * are chip fix routines for the Interrupt After TSTW bug.
 *
 * If an interupt is responded to after a certain sequence of instructions,
 * (described below) the last instruction in the sequence may not set flags
 * at all.
 *
 * Instruction Sequences which can cause problems:
 * First Instruction:
 *	Type 1) Instruction which does a store to a register and whose
 *		last source operand comes from memory. This does not
 *		include any discontinuities, SWAPs, RESTORE, or instructions
 *		without destinations (ie. BIT, TST, CMP).
 *	Type 2) Instruction which does a store to a register and is a
 *		multiple cycle ALU operation (ie. multiply, divide, modulo,
 *		insert field, move negate).
 * Second Instruction:
 *	Any instruction which is executed in 3 cycles. These are:
 *	TSTW %reg	MOVW %reg,%reg	MCOMW %reg %reg
 *	INCW %reg	DECW %reg	CLRW %reg
 * The fix for thisproblem is to insert a NOP before the second instruction
 * in the sequence.
 *
 */

/* This routine will return a 1 so that an indicator may be set if
 * the first instruction in the dangerous sequence is assembled.
 */
mtoregcheck(src, dest)
register addrmode *src, *dest;
{
#ifdef	CHIPFIX
	if ((dest->admode == REGMD) && (src->admode == REGDFMD ||
	     src->admode == DSPMD || src->admode == DSPDFMD ||
	     src->admode == EXADMD || src->admode == ABSMD ||
	     src->admode == EXADDFMD || src->admode == ABSDFMD)
	     && workaround )
		return(1);
	else
		return(0);
#else
		return(0);
#endif
} /* mtoregcheck */


/* This routine will generate a NOP if the indicator is set, and
 * the operand is a register operand.
 */
intaft1check(opnd, mtoreg)
register addrmode *opnd;
short	mtoreg;
{
#ifdef	CHIPFIX
	instr	*newins;
	if (mtoreg != 1)
		return;
	if ( (opnd->admode == REGMD) && workaround ) {
		newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
		generate(newins->nbits, NOACTION, newins->opcode, NULLSYM);
	}
#endif
} /* intaft1check */


/* This routine will generate a NOP if the indicator is set, and
 * both of the operands are register operands.
 */
intaft2check(opnd1, opnd2, mtoreg)
register addrmode *opnd1, *opnd2;
short	mtoreg;
{
#ifdef	CHIPFIX
	instr	*newins;
	if (mtoreg != 1)
		return;
	if ((opnd1->admode == REGMD) && (opnd2->admode == REGMD &&
		workaround)) {
		newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
		generate(newins->nbits, NOACTION, newins->opcode, NULLSYM);
	}
#endif
} /* intaft2check */


/*
 *	chip fix routine: Due to pipelining on the cpu chip
 *	it is possible for the chip to have the "next" instruction
 *	almost completely executed while it is waiting for the memory
 *	write of the "current" instr. to be completed, even executed
 *	to the point where the PC is incremented past the "next" instr.
 *	If an interrupt occurs in a situation like this then the PC
 *	will be restored to one past the "next" instr, thereby, in effect,
 *	skipping one instr. The fix for this is to place a NOP after
 *	every instr. that does a memory write, except for:
 *		CALL, call, SAVE, save, PUSHW, pushw, PUSHAW, pushaw
 *	These don't need the NOP padding.
 */
pcintcheck(insptr, dest)
register instr	*insptr;
register addrmode *dest;
{
#ifdef	CHIPFIX
	instr *newins;

	if ( (dest->admode == DSPMD || dest->admode == DSPDFMD ||
		dest->admode == EXADMD || dest->admode == ABSMD ||
		dest->admode == EXADDFMD || dest->admode == ABSDFMD ||
		dest->admode == REGDFMD) && (workaround) ) {

			newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
			generate(newins->nbits,NOACTION,newins->opcode,NULLSYM);
	}
#endif
} /* pcintcheck */

#if	0
/*	The following routine is ifdef'ed out because it is not
 *	needed in DBO Mask 2 chips.
 */
/*
 *	chip fix routine:	if one of the following instructions:
 *		MUL[BHW]3, DIV[BHW]3, MOD[BHW]3
 *	occurs and, during execution, the result from the first two
 *	operands is computed before the third (destination) operand
 *	has been latched and decoded then the CPU will hang.
 *	This problem is fixed with the following mapping:
 *
 *		MULX3, DIVX3, MODX3	is mapped into
 *
 *			pushYX		arg1
 *			if arg2 is %sp
 *				subw3	&4, %sp, marg3
 *			else
 *				movX	marg2, marg3
 *			"offending instr"2	{Z}spoff(%sp), marg3
 *			SUBW2		&4, %sp
 *			TSTX		{Z}arg3
 *
 *			where:	X = B, H, or W
 *				Y = z (zero extend) for bytes
 *				    b (bit extend) for halfs
 *				Z = user specified expand byte
 *				marg[2|3] indicates arg[2|3] has been
 *				   modified if it references the stack
 *				   pointer. (The first instruction in
 *				   the mapping is a push which changes
 *				   the stack pointer.)
 *			        spoff is -4L  if  X = W
 *				      is -2L  if  X = H
 *				      is -1L  if  X = B
 */
trimulcheck(insptr, arg1, arg2, arg3)
register instr	*insptr;
register addrmode	*arg1, *arg2, *arg3;
{
#ifdef	CHIPFIX
	instr	*newins, *movins, *tstins;
	short	usrexp1, usrexp2, usrexp3, adjarg3, otype;
	long	spoffset;

	/* Save user specified expand bytes (if there are any)
	 * so that we can push arg1 on the stack using our own
	 * expand byte
	 */
	usrexp1 = arg1->newtype;
	usrexp2 = arg2->newtype;
	usrexp3 = arg3->newtype;
	/* check if USER specified an expand byte in an IS25 instruction */
	if (insptr->tag & IS25) {
		if (( usrexp1 != NOTYPE) || (usrexp2 != NOTYPE) || (usrexp3 != NOTYPE))
			yyerror("Expand byte invalid in IS25 instruction");
	}
	arg1->newtype = NOTYPE;

	/* PUSHYX  arg1	
	 *
	 * Generate push instruction. Also look up the correct forms
	 * of the MOV and TST instructions.
	 */
	otype = optype(insptr->tag,1);
	switch (otype) {
		case 0:	newins = (*lookup("pushzb",N_INSTALL,MNEMON)).itp; /* BYTE */
			pushopgen(newins, arg1);
			movins = (*lookup("MOVB",N_INSTALL,MNEMON)).itp;
			tstins = (*lookup("TSTB",N_INSTALL,MNEMON)).itp;
			spoffset = -1L;
			break;
		case 1:	newins = (*lookup("pushbh",N_INSTALL,MNEMON)).itp; /* HALF */
			pushopgen(newins, arg1);
			movins = (*lookup("MOVH",N_INSTALL,MNEMON)).itp;
			tstins = (*lookup("TSTH",N_INSTALL,MNEMON)).itp;
			spoffset = -2L;
			break;
		case 2:	newins = (*lookup("pushw",N_INSTALL,MNEMON)).itp; /* WORD */
			generate (newins->nbits, NOACTION, newins->opcode, NULLSYM);
			addrgen (newins, arg1, NOTYPE, 1);
			movins = (*lookup("MOVW",N_INSTALL,MNEMON)).itp;
			tstins = (*lookup("TSTW",N_INSTALL,MNEMON)).itp;
			spoffset = -4L;
			break;
	}

	/* adjust arg3 if it references the stack pointer */
	adjarg3 = 0;
	if (((arg3->admode == DSPMD) || (arg3->admode == DSPDFMD)
	   || (arg3->admode == REGDFMD)) && (arg3->adreg == SPREG)) {
		adjarg3 = 1;
		arg3->adexpr.expval -= 4L;
	}
	if ((arg2->admode == REGMD) && (arg2->adreg == SPREG)) {
		/* SUBW3  &4,%sp,marg3	*/
		movins = (*lookup("SUBW3",N_INSTALL,MNEMON)).itp;
		generate(movins->nbits, NOACTION, movins->opcode, NULLSYM);
		generate(8, NOACTION, 0x4L, NULLSYM);
		generate(8, NOACTION, (long)((CREGMD<<4)|SPREG), NULLSYM);
		addrgen(movins, arg3, NOTYPE, 3);
	}
	else {
		/* adjust arg2 if it references the stack pointer */
		if (((arg2->admode == DSPMD) || (arg2->admode == DSPDFMD)
		   || (arg3->admode == REGDFMD)) && (arg2->adreg == SPREG))
			arg2->adexpr.expval -= 4L;

		/* MOVX  marg2,marg3	*/
		generate(movins->nbits, NOACTION, movins->opcode, NULLSYM);
		if (insptr->name[0] == 'u')
			generate(8, NOACTION, (long)((CEXPAND<<4)|UWORD), NULLSYM);
		else if (usrexp2 == NOTYPE)
			arg2->newtype = usrexp1;
		addrgen(movins, arg2, NOTYPE, 1);
		addrgen(movins, arg3, NOTYPE, 2);
	}
	pcintcheck(movins, arg3);

	/* <instr>X2  {expand}spoff(%sp),marg3	*/
	switch ((int)insptr->opcode) {
		case 0xe4:	newins = (*lookup("MODW2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xe6:	newins = (*lookup("MODH2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xe7:	newins = (*lookup("MODB2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xe8:	newins = (*lookup("MULW2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xea:	newins = (*lookup("MULH2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xeb:	newins = (*lookup("MULB2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xec:	newins = (*lookup("DIVW2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xee:	newins = (*lookup("DIVH2",N_INSTALL,MNEMON)).itp;
				break;
		case 0xef:	newins = (*lookup("DIVB2",N_INSTALL,MNEMON)).itp;
				break;
	}
	generate(newins->nbits, NOACTION, newins->opcode, NULLSYM);
	if (insptr->name[0] == 'u')
		generate(8,NOACTION,(long)((CEXPAND<<4)|UWORD),NULLSYM);
	else
		if (usrexp1 != NOTYPE)
			generate(8,NOACTION,(long)((CEXPAND<<4)|usrexp1),NULLSYM);

	generate(8, NOACTION, (long)((CBDSPMD<<4)|SPREG), NULLSYM);
	generate(8, NOACTION, spoffset, NULLSYM);
	arg3->newtype = usrexp2;
	addrgen(newins, arg3, NOTYPE, 2);
	pcintcheck(newins, arg3);

	/* SUBW2  &4,%sp	*/
	newins = (*lookup("SUBW2",N_INSTALL,MNEMON)).itp;
	generate(newins->nbits, NOACTION, newins->opcode, NULLSYM);
	generate(8, NOACTION, 0x4L, NULLSYM);
	generate(8, NOACTION, (long)((CREGMD<<4)|SPREG), NULLSYM);

	/* TSTX	arg3	
	 * This instruction is generated in order to restore the flag settings
	 * of the mul, div, or mod instruction.
	 *
	 * If arg3 was adjusted because of a stack reference it must now
	 * be readjusted.
	 */
	if (adjarg3 != 0)
		arg3->adexpr.expval += 4L;
	generate(tstins->nbits, NOACTION, tstins->opcode, NULLSYM);
	if (insptr->name[0] == 'u')
		arg3->newtype = UWORD;
	else if (usrexp3 != NOTYPE)
		arg3->newtype = usrexp3;
	else if (usrexp2 != NOTYPE)
		arg3->newtype = usrexp2;
	else
		arg3->newtype = usrexp1;
	addrgen(tstins, arg3, NOTYPE, 1);
#endif
} /* trimulcheck */
#endif

/*
 *	chip fix routine: if the psw register is used as the source of
 *	an instruction, the flag values may not be set yet from the 
 *	previous instruction due to a timing problem.  If the instruction
 *	is preceded by a NOP then the bits have time to settle and all
 *	is well.
 *	This chip fix routine will only fix IS25 instructions, BELLMAC-32B
 *	instructions are left untouched.
 */
pswcheck( insptr, addr)
register	instr	*insptr;
register	addrmode	*addr;
{
	instr	*newins;
	
	if ( (addr->admode == REGMD) && (addr->adreg == PSWREG) ) {
		newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
		generate(newins->nbits,NOACTION,newins->opcode,NULLSYM);
		return(0);	/* indicate that NOP was generated */
	} else {
		return(1);	/* NOP not needed */
	}
}

#if	M32RSTFIX
/*
 *  The following routine is a workaround for the
 *  RESTORE chip bug.
 *
 *  The RESTORE instruction will be mapped into the
 *  following sequence of instructions.
 *
 *	RESTORE %rX  ===>	MOVAW  -Y(%fp), %sp
 *				POPW	%r8
 *				POPW	%rZ
 *				  .
 *				  .
 *				POPW	%rX
 *				POPW	%fp
 *
 *	where:
 *		Y is  4 * (3 - X)  (ie. 12-4X )
 *		Z is  Y - 1
 *
 *	and if %rW is %fp then the sequence is the following:
 *	RESTORE %fp ===>	MOVAW	-24(%fp),%sp
 *				POPW	%fp
 */
restorefix(num)
register addrmode *num;
{
	register int numregs, rnum;
	instr *newins;
	static addrmode dispfp = {NOTYPE, DSPMD,FPREG,{ABS,NULLSYM,1L}};

	if (num->admode == REGMD) 
			numregs = 9 - num->adreg;
	if (num->admode == IMMD) 
			numregs = num->adexpr.expval;
	newins = (*lookup("MOVAW",N_INSTALL,MNEMON)).itp;
	generate(newins->nbits,NOACTION,newins->opcode,NULLSYM);
	dispfp.adexpr.expval = -24 + (4 * numregs);
	addrgen(newins,&dispfp,NOTYPE,1);
	generate(8,NOACTION,(long)(CREGMD<<4|SPREG),NULLSYM);
	/* generate MOVAW (12-4X)(%fp),%sp */
	newins = (*lookup("POPW",N_INSTALL,MNEMON)).itp;
	for (rnum=8; numregs > 0; numregs--,rnum--) {
		/* generate POPW with the register being 9-numreg */
		generate(newins->nbits,NOACTION,newins->opcode,NULLSYM);
		generate(8,NOACTION,(long)(CREGMD<<4|rnum),NULLSYM);
	}
	generate(newins->nbits,NOACTION,newins->opcode,NULLSYM);
	generate(8,NOACTION,(long)(CREGMD<<4|FPREG),NULLSYM);
}
#endif	/* M32RSTFIX */

flags(flag)
char flag;
{
	char errmsg[28];

	switch(flag) {

	default:
		sprintf(errmsg,"Illegal flag (%c) - ignored",flag);
		werror(errmsg);
		break;
	} /* switch */
} /* flags */

#if MLDVFIX
/* 
 * Number:	 er 1
 * 
 * Name: Flag Setting for Multiply/Divide
 * 
 * Description: There are two situations where the CPU sets the N & V
 * 	     flags but not the Z flag, even though the result is zero.
 * 	     The source operands must be of opposite signs, with
 * 	     an unsigned destination.  For multiply one source must be 0.
 * 	     For divide the absolute value of the numerator must be 
 * 	     non-zero and greater than the absolute value of the denominator.
 * 
 * Resolution: generate a warning diagnostic
 * 
 */

mulchk3(insptr,addr1,addr2,addr3)
	    instr *
insptr;
	    addrmode *
addr1;
	    addrmode *
addr2;
	    addrmode *
addr3;
{
   if (compare(insptr->name, "MULB3") == EQUAL ||
       compare(insptr->name, "MULH3") == EQUAL ||
       compare(insptr->name, "MULW3") == EQUAL ||
       compare(insptr->name, "DIVB3") == EQUAL ||
       compare(insptr->name, "DIVH3") == EQUAL ||
       compare(insptr->name, "DIVW3") == EQUAL ||
       compare(insptr->name, "MULB2") == EQUAL ||
       compare(insptr->name, "MULH2") == EQUAL ||
       compare(insptr->name, "MULW2") == EQUAL ||
       compare(insptr->name, "DIVB2") == EQUAL ||
       compare(insptr->name, "DIVH2") == EQUAL ||
       compare(insptr->name, "DIVW2") == EQUAL)
   {
	       int 
      type1, type2, type3;

      switch (addr1->newtype)
      {
      case SBYTE:
      case SHALF:
      case SWORD:
	 type1 = SIGNED;
	 break;
      case UBYTE:
      case UHALF:
      case UWORD:
	 type1 = UNSIGNED;
	 break;
      case NOTYPE:
         switch(optype(insptr->tag,1))
         {
         case 0:	/* byte */
	    type1 = UNSIGNED;
	    break;
         case 1:	/* half */
	    type1 = SIGNED;
	    break;
         case 2:	/* word */
	    type1 = SIGNED;
	    break;
         }
      }
      switch (addr2->newtype)
      {
      case SBYTE:
      case SHALF:
      case SWORD:
	 type2 = SIGNED;
	 break;
      case UBYTE:
      case UHALF:
      case UWORD:
	 type2 = UNSIGNED;
	 break;
      case NOTYPE:
         type2 = type1;
      }
      switch (addr3->newtype)
      {
      case SBYTE:
      case SHALF:
      case SWORD:
	 type3 = SIGNED;
	 break;
      case UBYTE:
      case UHALF:
      case UWORD:
	 type3 = UNSIGNED;
	 break;
      case NOTYPE:
         type3 = type2;
      }
      if ( (addr1->admode == IMMD) && (addr1->adexpr.expval < 0 ) )
	 type1 = SIGNED;
      if ( (addr2->admode == IMMD) && (addr2->adexpr.expval < 0 ) )
	 type2 = SIGNED;

      if ( ( ( type1 == SIGNED ) || ( type2 == SIGNED ) ) &&
	     ( type3 == UNSIGNED ) )

      {
	 werror("Mixed signs with unsignd destination can cause erroneous flags");
      }
   }
} /* mulchk3 */

#endif

#if	ER21FIX

er21(insptr,addr1,addr2,addr3)
	    instr *
insptr;
	    addrmode *
addr1;
	    addrmode *
addr2;
	    addrmode *
addr3;
{
   if (compare(insptr->name, "MODB3") == EQUAL ||
       compare(insptr->name, "MODH3") == EQUAL ||
       compare(insptr->name, "MODW3") == EQUAL )
   {
	       int 
      type1, type2, type3;

      switch (addr1->newtype)
      {
      case SBYTE:
      case SHALF:
      case SWORD:
	 type1 = SIGNED;
	 break;
      case UBYTE:
      case UHALF:
      case UWORD:
	 type1 = UNSIGNED;
	 break;
      case NOTYPE:
         switch(optype(insptr->tag,1))
         {
         case 0:	/* byte */
	    type1 = UNSIGNED;
	    break;
         case 1:	/* half */
	    type1 = SIGNED;
	    break;
         case 2:	/* word */
	    type1 = SIGNED;
	    break;
         }
      }
      switch (addr2->newtype)
      {
      case SBYTE:
      case SHALF:
      case SWORD:
	 type2 = SIGNED;
	 break;
      case UBYTE:
      case UHALF:
      case UWORD:
	 type2 = UNSIGNED;
	 break;
      case NOTYPE:
         type2 = type1;
      }
      switch (addr3->newtype)
      {
      case SBYTE:
      case SHALF:
      case SWORD:
	 type3 = SIGNED;
	 break;
      case UBYTE:
      case UHALF:
      case UWORD:
	 type3 = UNSIGNED;
	 break;
      case NOTYPE:
         type3 = type2;
      }
      if ( (addr1->admode == IMMD) && (addr1->adexpr.expval < 0 ) )
	 type1 = SIGNED;
      if ( (addr2->admode == IMMD) && (addr2->adexpr.expval < 0 ) )
	 type2 = SIGNED;

      if ( ( type2 == SIGNED ) && ( type3 == UNSIGNED ) )
      {
	 werror
	 ("Mixed signs with unsignd destination can cause erroneous flags");
      }
   }
} /* er21 */

#endif

#if ER16FIX

decrstk(i)
	    int
i;
{ /* decrstk */
	    instr *
   tmpins;

   tmpins = (*lookup("SUBW2",N_INSTALL,MNEMON)).itp;
   generate(8, NOACTION, tmpins->opcode, NULLSYM);
   generate(8, NOACTION, i, NULLSYM);
   generate(8, NOACTION, (long)((CREGMD<<4)|SPREG), NULLSYM);
} /* decrstk */

incrstk(i)
	    int
i;
{ /* incrstk */
	    instr *
   tmpins;

   tmpins = (*lookup("ADDW2",N_INSTALL,MNEMON)).itp;
   generate(8, NOACTION, tmpins->opcode, NULLSYM);
   generate(8, NOACTION, i, NULLSYM);
   generate(8, NOACTION, (long)((CREGMD<<4)|SPREG), NULLSYM);
} /* incrstk */

copyadd(addr1,addr2)
	    addrmode *
addr1;
	    addrmode *
addr2;
{ /* copyadd */
   addr2->newtype = addr1->newtype;
   addr2->admode = addr1->admode;
   addr2->adreg = addr1->adreg;
   addr2->adexpr.exptype = addr1->adexpr.exptype;
   addr2->adexpr.symptr = addr1->adexpr.symptr;
   addr2->adexpr.expval = addr1->adexpr.expval;
   addr2->adexpr.fdexpval2 = addr1->adexpr.fdexpval2;
   addr2->expspec = addr1->expspec;
} /* copyadd */

gen2(op,addr1,addr2)
	    char *
op;
	    addrmode *
addr1;
	    addrmode *
addr2;
{ /* gen2 */
	    instr *
   tmpins;

   tmpins = (*lookup(op,N_INSTALL,MNEMON)).itp;
   generate (tmpins->nbits, NOACTION, tmpins->opcode, NULLSYM);
   addrgen(tmpins,addr1,NOTYPE,1);
   addrgen(tmpins,addr2,NOTYPE,2);
} /* gen2 */

er16fix(insptr, arg1, arg2, arg3)
	 instr *
insptr;
	 addrmode *
arg1;
	 addrmode *
arg2;
	 addrmode *
arg3;
{ /* er16fix */
	    addrmode
   c_tmp1;
	 addrmode *
   c_arg1 = &c_tmp1;
	    addrmode
   c_tmp2;
	 addrmode *
   c_arg2 = &c_tmp2;
	    addrmode
   c_tmp3;
	 addrmode *
   c_arg3 = &c_tmp3;
	    instr *
   tmpins;

   if ( ! ( (compare(insptr->name, "MULH3") == EQUAL ) ||
	    (compare(insptr->name, "MULB3") == EQUAL ) ||
	    (compare(insptr->name, "MULW3") == EQUAL ) ||
	    (compare(insptr->name, "DIVH3") == EQUAL ) ||
	    (compare(insptr->name, "DIVB3") == EQUAL ) ||
	    (compare(insptr->name, "DIVW3") == EQUAL ) ||
	    (compare(insptr->name, "MODH3") == EQUAL ) ||
	    (compare(insptr->name, "MODB3") == EQUAL ) ||
	    (compare(insptr->name, "MODW3") == EQUAL ) ||
	    (compare(insptr->name, "mulw3") == EQUAL ) ||
	    (compare(insptr->name, "umulw3") == EQUAL ) ||
	    (compare(insptr->name, "divw3") == EQUAL ) ||
	    (compare(insptr->name, "udivw3") == EQUAL ) ||
	    (compare(insptr->name, "modw3") == EQUAL ) ||
	    (compare(insptr->name, "umodw3") == EQUAL ) ) )
      return(0); 

   if( ! ( (er16chk(arg1,arg3)) || (er16chk(arg2,arg3)) ) )
      return(0); 
   if( ! workaround )
      return(0); 

   if ( compare(insptr->name, "mulw3") == EQUAL)
      insptr = (*lookup("MULW3",N_INSTALL,MNEMON)).itp;
   if (compare(insptr->name, "umulw3") == EQUAL )
   {
      insptr = (*lookup("MULW3",N_INSTALL,MNEMON)).itp;
      arg1->newtype = UWORD;
   }

   if ( compare(insptr->name, "divw3") == EQUAL)
      insptr = (*lookup("DIVW3",N_INSTALL,MNEMON)).itp;
   if (compare(insptr->name, "udivw3") == EQUAL )
   {
      insptr = (*lookup("DIVW3",N_INSTALL,MNEMON)).itp;
      arg1->newtype = UWORD;
   }

   if ( compare(insptr->name, "modw3") == EQUAL)
      insptr = (*lookup("MODW3",N_INSTALL,MNEMON)).itp;
   if (compare(insptr->name, "umodw3") == EQUAL )
   {
      insptr = (*lookup("MODW3",N_INSTALL,MNEMON)).itp;
      arg1->newtype = UWORD;
   }

   incrstk(12);

   if (arg1->newtype == NOTYPE)
      switch(optype(insptr->tag,1))
      {
      case 0:	/* byte */
         arg1->newtype = UBYTE;
	 break;
      case 1:	/* half */
	 arg1->newtype = SHALF;
	 break;
      case 2:	/* word */
	 arg1->newtype = SWORD;
	 break;
      }
   if (arg2->newtype == NOTYPE)
      arg2->newtype = arg1->newtype;
   if (arg3->newtype == NOTYPE)
      arg3->newtype = arg2->newtype;

   copyadd(arg1,c_arg1);
   copyadd(arg2,c_arg2);
   copyadd(arg3,c_arg3);

   if ( ( (c_arg1->admode == DSPMD  ) ||
          (c_arg1->admode == DSPDFMD) ||
	  (c_arg1->admode == REGDFMD) ) &&
	( c_arg1->adreg == SPREG      ) )
      c_arg1->adexpr.expval -= 12;
   if ( (c_arg1->admode == REGMD) && (c_arg1->adreg == SPREG) )
   {
      /* SUBW3  &12,%sp,c_arg1	*/
      c_arg1->admode = DSPMD;
      c_arg1->adexpr.exptype = ABS;
      c_arg1->adexpr.symptr = NULLSYM;
      c_arg1->adexpr.expval = -12;

      tmpins = (*lookup("SUBW3",N_INSTALL,MNEMON)).itp;
      generate(tmpins->nbits, NOACTION, tmpins->opcode, NULLSYM);
      generate(8, NOACTION, 0x12L, NULLSYM);
      generate(8, NOACTION, (long)((CREGMD<<4)|SPREG), NULLSYM);
      addrgen(tmpins, c_arg1, NOTYPE, 3);
   }

   if ( ( (c_arg2->admode == DSPMD  ) ||
          (c_arg2->admode == DSPDFMD) ||
	  (c_arg2->admode == REGDFMD) ) &&
	( c_arg2->adreg == SPREG      ) )
      c_arg2->adexpr.expval -= 12;
   if ( (c_arg2->admode == REGMD) && (c_arg2->adreg == SPREG) )
   {
      /* SUBW3  &12,%sp,c_arg2	*/
      c_arg2->admode = DSPMD;
      c_arg2->adexpr.exptype = ABS;
      c_arg2->adexpr.symptr = NULLSYM;
      c_arg2->adexpr.expval = -8;


      tmpins = (*lookup("SUBW3",N_INSTALL,MNEMON)).itp;
      generate(tmpins->nbits, NOACTION, tmpins->opcode, NULLSYM);
      generate(8, NOACTION, 0x12L, NULLSYM);
      generate(8, NOACTION, (long)((CREGMD<<4)|SPREG), NULLSYM);
      addrgen(tmpins, c_arg2, NOTYPE, 3);
   }

   c_arg3->admode = DSPMD;
   c_arg3->adreg = SPREG;
   c_arg3->adexpr.exptype = ABS;
   c_arg3->adexpr.symptr = NULLSYM;
   c_arg3->adexpr.expval = -4;

   generate(8,NOACTION,insptr->opcode,NULLSYM);
   addrgen(insptr,c_arg1,NOTYPE,1);
   addrgen(insptr,c_arg2,NOTYPE,2);
   addrgen(insptr,c_arg3,NOTYPE,3);

   if ( ( (arg3->admode == DSPMD  ) ||
          (arg3->admode == DSPDFMD) ||
	  (arg3->admode == REGDFMD) ) &&
	( arg3->adreg == SPREG      ) )
      arg3->adexpr.expval -= 12;

   gen2("MOVW",c_arg3,arg3);

   decrstk(12);

   c_arg3->adexpr.expval = 0;
   generate(8,NOACTION,insptr->opcode,NULLSYM);
   addrgen(insptr,arg1,NOTYPE,1);
   addrgen(insptr,arg2,NOTYPE,2);
   addrgen(insptr,c_arg3,NOTYPE,3);

   return(1);

} /* er16fix */


er16chk(addr1,addr2)
addrmode
	*addr1;
addrmode
	*addr2;
{ /* er16chk */
   switch(addr2->admode)
   { /* switch(addr2->admode) */
   case REGDFMD: /* register deferred mode */
      switch(addr1->admode)
      {
      case DSPMD: /* displacement mode */
	 if ( ( addr1->adreg == addr2->adreg ) &&
	      ( abs(addr1->adexpr.expval - 0) >= 4 ) )
	       return(NO);
         break;
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case DSPMD: /* displacement mode */
      switch(addr1->admode)
      {
      case DSPMD: /* displacement mode */
	 if ( ( addr1->adreg == addr2->adreg ) &&
	      ( abs(addr1->adexpr.expval - addr2->adexpr.expval) >=4 ) )
	       return(NO);
         break;
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case DSPDFMD: /* displacement deferred mode */
      switch(addr1->admode)
      {
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case EXADMD: /* external address mode */
      switch(addr1->admode)
      {
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case ABSMD: /* absolute address mode */
      switch(addr1->admode)
      {
      case ABSMD: /* absolute address mode */
	 if ( abs(addr1->adexpr.expval - addr2->adexpr.expval) >= 4 )
	    return(NO);
         break;
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case EXADDFMD: /* external address deferred mode (PC relative deferred) */
      switch(addr1->admode)
      {
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case ABSDFMD: /* absolute address deferred mode */
      switch(addr1->admode)
      {
      case REGMD: /* register mode */
	 return(NO);
      }
      break;
   case IMMD: /* immediate mode */
      return(NO);
   case REGMD: /* register mode */
      return(NO);
   } /* switch(addr2->admode) */
   return(YES);
} /* er16chk */

#endif
