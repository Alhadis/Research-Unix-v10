%{
static char *ID_parsey = "@(#) parse.y: 1.23 5/8/84";
#include <stdio.h>
#include "systems.h"
#include "symbols.h"
#include "instab.h"
#include "gendefs.h"

#if	ER21FIX
static char * SWA_21 = "@(#) ER21FIX";
#endif

#if	ER16FIX
static char * SWA_16 = "@(#) ER16FIX";
#endif

#if	EXPFIX
static char * SWA_EXP = "@(#) EXPFIX";
#endif

#if	MLDVFIX
static char * SWA_BLDV = "@(#) BLDVFIX";
#endif

#if	M32RSTFIX
static char * SWA_RST = "@(#) M32RSTFIX";
#endif

#ifdef	CHIPFIX
static char * SWA_CHIP = "@(#) CHIPFIX";
#endif

static short
	expand,		/* expand byte to be generated */
	nbits,		/* number of bits of code to be generated */
	spctype,	/* distinguishes between .byte, .half, .word, .float, .double */
	width;		/* bit field width */
static short memtoreg = 0;	/* Indicator of first instruction in the
				 * 2 instruction sequence that manifests
				 * the Interrupt After TSTW chip bug.
				 */
static unsigned short
	action; /* action routine to be called */
static instr
	*newins; /* for looking up a new instruction name */
static long
	opcd;	/* opcode */
extern unsigned short
	line,	/* current line number */
	cline;	/* line number from `.ln' */
extern short
	workaround, /* software work around disable flag */
	opt,	/* optimize */
	bitpos;

#if	M32RSTFIX
extern short rstflag;
#endif	/* M32RSTFIX */

extern long
	dotbss,
	newdot; /* up-to-date value of "." */
extern symbol
	*dot;
extern FILE
#if !ONEPROC
	*fdstring,
#endif
	*fdin;
extern char
	cfile[];
extern upsymins *lookup();
extern int deflab();
extern int compare();
%}
%union {
	long uulong;
	rexpr uuexpr;
	symbol *uusymptr;
	instr *uuinsptr;
	addrmode uumode;
	char *uustrptr;
}
%token <uuinsptr> MONOP1 MONOP2 MONOP3
%token <uuinsptr> PUSHOP1
%token <uuinsptr> DYAOP1 DYAOP2 DYAOP3 DYAOP4 DYAOP5
%token <uuinsptr> DYAOP6 DYAOP7 DYAOP8
%token <uuinsptr> CMPOP1 CMPOP2
%token <uuinsptr> TRIOP1 TRIOP2 TRIOP3
%token <uuinsptr> FPDYAOP1 FPDYAOP2 FPTRIOP1 FPCMPOP1
%token <uuinsptr> SPRTOP1  SPRTOP2  SPRTOP3
%token <uuinsptr> SHFTOP1
%token <uuinsptr> JMPOP1 JSBOP1 JSBOP2 BSBOP1
%token <uuinsptr> JMBOP1
%token <uuinsptr> LOOPOP1 LOOPOP2
%token <uuinsptr> BLOCKOP
%token <uuinsptr> FLDOP1 FLDOP2 FLDOP4
%token <uuinsptr> ZEROP1
%token <uuinsptr> TESTOP1
%token <uuinsptr> CALOP1 CALOP2 SAVOP1 SAVOP2 RETOP1
%token <uuinsptr> PSGLOBAL PSSET PSALIGN PSCOMM PSIDENT
%token <uuinsptr> PSBSS PSZERO PSFILE
%token <uuinsptr> PSBYTE PSHALF PSWORD PSFLOAT PSDOUBLE PSDEF PSVAL PSSCL
%token <uuinsptr> PSTYPE PSTAG PSLINE PSSIZE PSDIM PSENDEF PSLN PSIL
%token <uuinsptr> SECTIONN
%token <uuinsptr> SIMOP
%token <uulong> SP QUOTE SHARP DOLLAR REG AMP SQ LP RP MUL PLUS
%token <uulong> COMMA MINUS ALPH DIV DIG COLON SEMI QUEST LB RB LC RC
%token <uulong> LT GT OR NL NOCHAR
%token <uusymptr> ID
%token <uuinsptr> REGISTER
%token <uuinsptr> OPERSPEC
%token <uulong> ERR
%token <uulong> NUMBER
%token <uustrptr> STRING
%token <uuexpr> FPNUMBER
%token <uuinsptr> EXPTYPE
%type <uulong> attrib
%type <uulong> optoperspec
%type <uuexpr> expr term fpnumber
%type <uumode> gen dest mem immode regmode regdefmode optimmode 
%type <uumode> fpgen fpimmode	/* general and immed. modes for fp numbers */
%type <uumode> extaddrmode absaddrmode
%type <uumode> extaddrdefmode absaddrdefmode dispmode dispdefmode
%%
wholeprog:	program
	;

program:	/* empty */
	|	program label instruction endstmt = {
			dot->value = newdot; /* syncronize */
		}
	|	program error endstmt = {
			yyerrok;
			dot->value = newdot; /* syncronize */
		}
	;
endstmt	:	NL = {
			++line;
			generate(0,NEWSTMT,(long)line,NULLSYM);
		}
	|	SEMI = {
			generate(0,NEWSTMT,(long)line,NULLSYM);
		}
	;
label	:	/* empty */
	|	ID COLON = {
			if (($1->styp & TYPE) != UNDEF)
				yyerror("error: multiply defined label");
			$1->value = newdot;
			$1->styp |= dot->styp;
			if (dot->styp == TXT)
				deflab($1);
		}
	;
instruction:
	|	MONOP1 gen = {
			/*
			 * PUSHW pushw SWAP[BHW]I
			 * (TST|CLR|INC|DEC)[BHW]
			 *	instructions
			 */
#if	EXPFIX
			if (compare($1->name,"PUSHW") == EQUAL && workaround)
				if ($2.newtype != NOTYPE)
					yyerror
					("Expand byte invalid in PUSHW instruction");
#endif
			pswcheck($1, &($2));
			if (compare($1->name,"CLRW") == EQUAL ||
			    compare($1->name,"DECW") == EQUAL ||
			    compare($1->name,"INCW") == EQUAL ||
			    compare($1->name,"TSTW") == EQUAL)
				intaft1check(&($2), memtoreg);

			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);

			if (compare($1->name,"PUSHW") != EQUAL &&
			    compare($1->name,"pushw") != EQUAL) {
				pcintcheck($1,&($2));	/* don't fix PUSHW */
			}
			memtoreg = 0;
	}
	|	MONOP2 mem = {
			/* PUSHAW pushaw instruction */
#if	EXPFIX
			if (compare($1->name,"pushaw") != EQUAL && workaround)
				if ($2.newtype != NOTYPE)
					yyerror
					("Expand byte invalid in PUSHAW,SWAPWI,SWAPHI, and SWAPBI instructions");
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			memtoreg = 0;
		}
	|	MONOP3 dest = {
			/* POPW instruction */
#if	EXPFIX
			if( ( $2.newtype != NOTYPE ) && workaround )
				yyerror
				("Expand byte illegal in POPW Instruction.");
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			memtoreg = 0;
		}
	|	PUSHOP1 gen = {
			/* push[zb][bh] instructions */
			pushopgen($1,&($2));
			pcintcheck($1,&($2));
			memtoreg = 0;
		}
	|	DYAOP1 gen COMMA dest = {
			/*
			 * (AND|and)[BHW]2 (OR|or)[BHW]2 (XOR|xor)[BHW]2
			 * MOD[BHW]2 MUL[BHW]2 DIV[BHW]2 modw2 mulw2 divw2
			 * (MCOM|mcom)[BHW] MNEG[BHW]
			 *	instructions
			 */
#if	MLDVFIX
			mulchk3($1,&($2),&($4),&($4));
#endif

			/* Interrupt After TSTW Chip Bug
			 * Check if instruction does a store to
			 * a register and is a multiple cycle
			 * ALU instruction (ie. multiply, divide,
			 * modulo, move negate). If so, then set
			 * memtoreg so that a NOP will be generated
			 * after this instruction if necessary.
			 */

			if ( pswcheck($1, &($2)) )
				pswcheck($1, &($4));

#ifdef	CHIPFIX
			if (($4.admode == REGMD) &&
			    ((($1->name[0] == 'M') && ($1->name[1] != 'C')) ||
			     (($1->name[0] == 'm') && ($1->name[1] != 'c')) ||
			     ($1->name[0] == 'D') ||
			     ($1->name[0] == 'd')))
				memtoreg = 1;
			else if (compare($1->name, "MCOMW") == EQUAL ||
				 compare($1->name, "mcomw") == EQUAL) {
					intaft2check(&($2), &($4), memtoreg);
					memtoreg = mtoregcheck(&($2), &($4));
			}
			else
				memtoreg = mtoregcheck(&($2), &($4));
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			pcintcheck($1,&($4));
		}
	|	DYAOP2 mem COMMA dest = {
			/* MOVAW movaw MOVTRW instructions */
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			pcintcheck($1,&($4));
			memtoreg = mtoregcheck(&($2), &($4));
		}
	|	DYAOP3 gen COMMA dest = {
			/*
			 * [al][lr]sw2 instructions
			 *
			 * generate code using triadic opcode,
			 * repeating 2nd operand
			 */
			pswcheck($1, &($2));
			shftopgen($1,&($2),&($4),&($4));
			pcintcheck($1,&($4));
			memtoreg = mtoregcheck(&($2), &($4));
		}
	|	DYAOP4 gen COMMA dest = {
			/*
			 * (MOV|mov)[bhw] instructions
			 *
			 * if first operand is &0, use CLR opcodes
			 * stored in 2nd low byte of opcode
			 */
			/* The following call to opt21gen
			 * is not always executed out because of the
			 * Interrupt After TSTW chip bug
			 */
			pswcheck($1, &($2));
			if (compare($1->name, "MOVW") == EQUAL ||
			    compare($1->name, "movw") == EQUAL) {
				if ($4.admode != REGMD || memtoreg == 0)
					opt21gen($1,&($2),&($4),0);
				else {
					intaft2check(&($2), &($4), memtoreg);
					generate(8,NOACTION,$1->opcode & 0xFFL,NULLSYM);
					addrgen($1, &($2), NOTYPE, 1);
					addrgen($1, &($4), NOTYPE, 2);
				}
			}
			else
				opt21gen($1,&($2),&($4),0);
		memtoreg = mtoregcheck(&($2), &($4));
		pcintcheck($1, &($4));
		}
	|	DYAOP5 gen COMMA dest = {
			/* movb[bh|bw|hw]  movt[wb|hb|wh] instructions */

			pswcheck($1, &($2));
			convgen($1,&($2),&($4));
			pcintcheck($1,&($4));
			memtoreg = mtoregcheck(&($2), &($4));
		}
	|	DYAOP6 gen COMMA dest = {
			/*
			 * (ADD|SUB|add|sub)[bhw]2 instructions
			 *
			 * if first operand is &1, use INC (DEC) opcodes
			 * stored in 2nd low byte of opcode
			 */
			/* The following call to opt21gen
			 * is not always executed because of the
			 * Interrupt After TSTW chip bug
			 */
			pswcheck($1, &($2));
			if ($4.admode != REGMD || memtoreg == 0)
				opt21gen($1,&($2),&($4),1);
			else {
				generate(8,NOACTION,$1->opcode & 0xFFL,NULLSYM);
				addrgen($1, &($2), NOTYPE, 1);
				addrgen($1, &($4), NOTYPE, 2);
			}
			memtoreg = mtoregcheck(&($2), &($4));
			pcintcheck($1,&($4));
		}
	|	DYAOP7 gen COMMA dest = {
			/* movz[bh|bw|hw] instructions */
			pswcheck($1, &($2));
			generate(8,NOACTION,$1->opcode,NULLSYM);
			expand = (optype($1->tag,1) == 0) ? NOTYPE : UHALF;
			addrgen($1,&($2),expand,1);
			/* 2nd operand is signed (halfword/word) */
			expand = (optype($1->tag,2) == 1) ? UHALF : UWORD;
			addrgen($1,&($4),expand,2);
			pcintcheck($1,&($4));
			memtoreg = mtoregcheck(&($2), &($4));
		}
	|	DYAOP8 gen COMMA dest = {
			/* u(mul|div|mod)w2 instructions */
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),UWORD,1);
			addrgen($1,&($4),NOTYPE,2);
			pcintcheck($1,&($4));
			memtoreg = 1;
		}
	|	CMPOP1 gen COMMA gen = {
			/*
			 * (cmp|CMP)[bhw] instructions
			 *
			 * For IS25 versions, interchange operands and
			 * if first operand is &0, use TST opcodes
			 * stored in 2nd low byte of opcode
			 */

			if ( pswcheck($1, &($2)) )
				pswcheck($1, &($4));
			if ($1->tag & IS25) {
				/* The following call to opt21gen
				 * is not always executed because of the
				 * Interrupt After TSTW chip bug
				 */
				if ($2.admode != REGMD || memtoreg == 0 ||
				    compare($1->name,"cmpw") != EQUAL)
					opt21gen($1,&($4),&($2),0);
				else {
					generate(8,NOACTION,$1->opcode,NULLSYM);
					addrgen($1,&($4),NOTYPE,1);
					addrgen($1,&($2),NOTYPE,2);
				}
			}
			else {
				/* generate normal sequence */
				generate(8,NOACTION,$1->opcode,NULLSYM);
				addrgen($1,&($2),NOTYPE,1);
				addrgen($1,&($4),NOTYPE,2);
			}
			memtoreg = 0;
		}
	|	CMPOP2 gen COMMA gen = {
			/* (BIT|bit)[bhw] instructions */
			if ( pswcheck($1, &($2)) )
				pswcheck($1, &($4));
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			memtoreg = 0;
		}
	|	TRIOP1 gen COMMA gen COMMA dest = {
			/*
			 * (AND|and)[bhw]3 (OR|or)[bhw]3 (XOR|xor)[bhw]3
			 * (ADD|add)[bhw]3 (SUB|sub)[bhw]3
			 * (MOD|mod)[BHW]3 (MUL|mul)[BHW]3 (DIV|div)[BHW]3
			 * (AR|LL)S[BHW]3 ALSW3 ROTW
			 *	instructions
			 */
			if ( pswcheck($1, &($2)) )
				pswcheck($1, &($4));
#if	MLDVFIX
			mulchk3($1,&($2),&($4),&($6));
#endif
#if	ER21FIX
			er21($1,&($2),&($4),&($6));
#endif
#if	0	/* trimul software workaround */
			if (($1->name[0] == 'M') || ($1->name[0] == 'm') ||
			    ($1->name[0] == 'D') || ($1->name[0] == 'd')) {
				trimulcheck($1, &($2), &($4), &($6));
				memtoreg = 0;
			}
			else {
#endif
#if	ER16FIX
			if ( er16fix($1, &($2), &($4), &($6)) )
			{
				memtoreg = 0;
			}
			else {
#endif
				generate(8,NOACTION,$1->opcode,NULLSYM);
				addrgen($1,&($2),NOTYPE,1);
				addrgen($1,&($4),NOTYPE,2);
				addrgen($1,&($6),NOTYPE,3);
				pcintcheck($1,&($6));
				memtoreg = mtoregcheck(&($4), &($6));
#if	ER16FIX
			}
#endif
#if	0	/* trimul software workaround */
			}
#endif
		}
	|	TRIOP2 gen COMMA gen COMMA dest = {
			/* u(mul|div|mod)w3 instructions */
#if	0	/* trimul software workaround */
			if ($1->name[0] == 'u')
				trimulcheck($1, &($2), &($4), &($6));
			else {
#endif
#if	ER16FIX
			if ( er16fix($1, &($2), &($4), &($6)) )
				;
			else {
#endif
				generate(8,NOACTION,$1->opcode,NULLSYM);
				addrgen($1,&($2),UWORD,1);
				addrgen($1,&($4),NOTYPE,2);
				addrgen($1,&($6),NOTYPE,3);
				pcintcheck($1,&($6));
#if	ER16FIX
			}
#endif
#if	0	/* trimul software workaround */
			}
#endif
			memtoreg = 0;
		}
	|	SHFTOP1 gen COMMA gen COMMA dest = {
			/* [al][lr]sw3 instructions */
			if ( pswcheck($1, &($2)) )
				pswcheck($1, &($4));
			shftopgen($1,&($2),&($4),&($6));
			pcintcheck($1,&($6));
			memtoreg = mtoregcheck(&($4), &($6));
		}
	|	JMPOP1 mem = {
			/*
			 * jmp j(n)e j(n)neg j(n)pos j(n)z 
			 * j[lg][eu] j[lg] j[lg]eu
			 * JMP B(N)E[BH] BR[BH]
			 * B[LG][EU][BH] B[LG]EU[BH] B[LG][BH]
			 * BVC[BH] BVS[BH] BCC[BH] BCS[BH]
			 * BEQL[BH]X BNEQ[BH]X
			 *	instructions
			 */

			jmpopgen($1,&($2));
			memtoreg = 0;
		}
	|	JSBOP1 mem = {
			/* jsb instruction */
			jsbopgen($1,&($2));
			memtoreg = 0;
		}
	|	JSBOP2 mem = {
			/* JSB instruction */
#if	EXPFIX
			if( ( $2.newtype != NOTYPE ) && workaround )
				yyerror
				("Expand byte illegal in JSB Instruction.");
#endif
#if	INTEXPFIX
			expandc($1, &($2), destmem);
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,2);
			memtoreg = 0;
		}
	|	BSBOP1 extaddrmode = {
			/* BSB[BH] instructions */
			generate(8,NOACTION,$1->opcode,NULLSYM);
			switch ($1->tag) {
			case INSTRB:
				generate(8,RELPC8,$2.adexpr.expval,$2.adexpr.symptr);
				break;
			case INSTRH:
				generate(16,RELPC16,$2.adexpr.expval,$2.adexpr.symptr);
				break;
			default:
				yyerror("assembler error (BSBOP1)");
				break;
			} /* switch */
			memtoreg = 0;
		}
	|	JMBOP1 immode COMMA gen COMMA dest = {
			/*
			 * jb[cs] instructions
			 *
			 * jb(c|s)	&a,op1,op2
			 *
			 *	is encoded as
			 *
			 * bitw		&(1<<a),op1
			 * j(z|nz)	op2
			 *
			 */
			pswcheck($1, &($4));
			generate(8,NOACTION,$1->opcode,NULLSYM); /* bitw opcode */
			if ($2.adexpr.symptr == NULLSYM) {
				if ($2.adexpr.expval<0L || $2.adexpr.expval>31L)
					yyerror("Bit position out of range");
				$2.adexpr.expval = 1L << $2.adexpr.expval;
				addrgen($1,&($2),NOTYPE,1);
			}
			else {
				/* generate long immediate with action routine */
				generate(8,NOACTION,(long)(CIMMD),NULLSYM);
				generate(32,SHIFTVAL,$2.adexpr.expval,
					$2.adexpr.symptr);
			}
			addrgen($1,&($4),NOTYPE,2);

			generate(0,NEWSTMT,(long)line,NULLSYM);
			newins = (*lookup(
					(($1->name[2] == 'c')
						? "jz"	/* for jbc */
						: "jnz"	/* for jbs */),
					N_INSTALL,MNEMON)).itp;
			jmpopgen(newins,&($6));
			memtoreg = 0;
		}
	|	LOOPOP1 dest COMMA gen COMMA gen COMMA mem = {
			/* acj[l|lu|le|leu] instructions */
			loop1gen($1,&($2),&($4),&($6),&($8));
			pcintcheck($1,&($8));
			memtoreg = 0;
		}
	|	LOOPOP2 dest COMMA gen COMMA mem = {
			/* atjnz[bhw] instructions */
			loop2gen($1,&($2),&($4),&($6));
			pcintcheck($1,&($6));
			memtoreg = 0;
		}
	|	BLOCKOP = {
			/* movbl[bh] */
			if (memtoreg != 0) {
				newins = (*lookup("NOP",N_INSTALL,MNEMON)).itp;
				generate(8, NOACTION, newins->opcode, NULLSYM);
				memtoreg = 0;
			}
			blockgen($1);
		}
	|	SPRTOP1 immode = {
			/* SPOP */
			if( $2.newtype != NOTYPE) {
				yyerror("Expand byte illegal in id word for Support Processor Instructions.");
				return;
			}
			if( $2.expspec != NULLSPEC ) {
				yyerror("Operand specifier illegal in id word for Support Processor Instructions.");
				return;
			}
			generate(8,NOACTION,$1->opcode,NULLSYM);
			generate(32,NOACTION,swap_b4($2.adexpr.expval),NULLSYM);
		}
	|	SPRTOP2 immode COMMA mem  = {
			/* SPOPRS, SPOPRD, SPOPRT, SPOPWS, SPOPWD, SPOPWT */
			if( $2.newtype != NOTYPE) {
				yyerror("Expand byte illegal in id word for Support Processor Instructions.");
				return;
			}
			if( $2.expspec != NULLSPEC ) {
				yyerror("Operand specifier illegal in id word for Support Processor Instructions.");
				return;
			}
#if	EXPFIX
			if( ( $4.newtype != NOTYPE ) && workaround ) {
				yyerror("Expand byte illegal in Support Processor Instructions.");
			}
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			generate(32,NOACTION,swap_b4($2.adexpr.expval),NULLSYM);
			addrgen( $1, &($4), NOTYPE, 2 );
		}
	|	SPRTOP3 immode COMMA mem COMMA mem  = {
			/* SPOPS2, SPOPD2, SPOPT2 */
			if( $2.newtype != NOTYPE) {
				yyerror("Expand byte illegal in id word for Support Processor Instructions.");
				return;
			}
#if	EXPFIX
			if( ( $4.newtype != NOTYPE ||
			    $4.newtype != NOTYPE ) && workaround )
			{
				yyerror("Expand byte illegal in Support Processor Instructions.");
			}
#endif
			if( $2.expspec != NULLSPEC ) {
				yyerror("Operand specifier illegal in id word for Support Processor Instructions.");
				return;
			}
			generate(8,NOACTION,$1->opcode,NULLSYM);
			generate(32,NOACTION,swap_b4($2.adexpr.expval),NULLSYM);
			addrgen( $1, &($4), NOTYPE, 2 );
			addrgen( $1, &($6), NOTYPE, 3 );
		}
	|	FLDOP1	gen COMMA immode COMMA immode COMMA dest = {
			/* insv instruction */
			pswcheck($1, &($2));
			generate(8,NOACTION,$1->opcode,NULLSYM);
			--($6.adexpr.expval); /* decrement field width */
			addrgen($1,&($6),NOTYPE,3);
			addrgen($1,&($4),NOTYPE,2);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($8),NOTYPE,4);
			pcintcheck($1,&($8));
#ifdef	CHIPFIX
			if ($8.admode == REGMD)
				memtoreg = 1;
			else
				memtoreg = 0;
#endif
		}
	|	FLDOP2	immode COMMA immode COMMA gen COMMA dest = {
			/* extzv instruction */
			pswcheck($1, &($6));
			generate(8,NOACTION,$1->opcode,NULLSYM);
			--($4.adexpr.expval); /* decrement field width */
			addrgen($1,&($4),NOTYPE,1);	/* xchange order of */
			addrgen($1,&($2),NOTYPE,2);	/* oprnds 1 and 2 */
			addrgen($1,&($6),NOTYPE,3);
			addrgen($1,&($8),NOTYPE,4);
			pcintcheck($1,&($8));
			memtoreg = mtoregcheck(&($6), &($8));
		}
	|	FLDOP4	gen COMMA gen COMMA gen COMMA dest = {
			/* (INS|EXT)F[BHW] instructions */
			/* Interrupt After TSTW Chip Bug
			 * Check if instruction does a store to
			 * a register and is a multiple cycle
			 * ALU instruction (ie. insert field).
			 * If so, then set memtoreg so that a NOP
			 * will be generated after this instruction
			 * if necessary.
			 */
			if ( pswcheck($1, &($2)) )
				if ( pswcheck($1, &($4)) )
					pswcheck($1, &($6));
#ifdef	CHIPFIX
			if (($8.admode == REGMD) &&
			    ($1->name[0] == 'I'))
				memtoreg = 1;
			else
				memtoreg = mtoregcheck(&($6), &($8));
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			addrgen($1,&($6),NOTYPE,3);
			addrgen($1,&($8),NOTYPE,4);
			pcintcheck($1,&($8));
		}
	|	ZEROP1 = {
			/*
			 * movblw rsb
			 * RET GATE RETG [CALL|RET]PS [DIS|ENB]VJMP
			 * R[LGN][EQ|EQU]
			 * REQ[L|LU] R[LSS|GTR](U)
			 * RC[CS] RV[CS] RSB
			 * MOVBLW MVERNO WAIT SLFTST
			 * BPT NOP STR(CPY|END)
			 * CFLUSH
			 *	instructions
			 */
			generate($1->nbits,NOACTION,$1->opcode,NULLSYM);
			memtoreg = 0;
		}
	|	TESTOP1 optimmode = {
			testgen( $1, &($2));
			memtoreg = 0;
		}
	|	CALOP1 immode COMMA mem = {
			/* call instruction */
			callgen($1,&($2),&($4));
			memtoreg = 0;
		}
	|	CALOP2 mem COMMA mem = {
			/* CALL instruction */
#if	EXPFIX
			if( ( $2.newtype != NOTYPE ||
			    $4.newtype != NOTYPE ) && workaround )
				yyerror
				("Expand byte illegal in CALL Instruction.");
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			memtoreg = 0;
		}
	|	SAVOP1 immode = {
			/* save instruction */
			generate(8,NOACTION,$1->opcode,NULLSYM);
			/* let the action routine do the translation */
			generate(8,SAVTRANS,$2.adexpr.expval,$2.adexpr.symptr);
			memtoreg = 0;
		}
	|	RETOP1 immode = {
			/* ret instruction */
#if M32RSTFIX
			if (rstflag == YES) {
				if ($2.adexpr.expval < 0 || $2.adexpr.expval > 6)
					werror("Invalid number of registers in `ret'");
				restorefix(&($2));
			}
			else {
#endif
			generate(8,NOACTION,($1->opcode & 0xFFL),NULLSYM);
			/* let the action routine do the translation */
			generate(8,SAVTRANS,$2.adexpr.expval,$2.adexpr.symptr);
#if M32RSTFIX
			}
#endif
			/* generate the CRET instruction */
			generate(8,NOACTION,(long)($1->opcode >> 8),NULLSYM);
			memtoreg = 0;
		}
	|	SAVOP2 regmode = {
			/* SAVE RESTORE instructions */
#if	EXPFIX
			if( $2.newtype != NOTYPE && workaround )
				yyerror
				("Expand byte illegal in SAVE/RESTORE Instructions.");
#endif
#if M32RSTFIX
			if (($1->name[0] == 'R' ) && (rstflag == YES))
				restorefix(&($2));
			else {
#endif
			generate(8,NOACTION,$1->opcode,NULLSYM);
			generate(8,NOACTION,(long)((CREGMD<<4)|$2.adreg),NULLSYM);
#if M32RSTFIX
			}
#endif
			memtoreg = 0;
		}
	|	SIMOP extaddrmode = {
			/* EXTOP instuction for the m32a simulator i/o routines */
			generate(8,NOACTION,$1->opcode,NULLSYM);
			generate(8,NOACTION,$2.adexpr.expval,NULLSYM);
			memtoreg = 0;
		}
	|	FPDYAOP1 fpgen COMMA dest = {
#if	FLOAT
			/*
			 * fadd[sd]2 fsub[sd]2 fmul[sd]2 fdiv[sd]2
			 * mov[sd]w mov[sd]h mov[sd][sd]
			 * movt[sd]w movt[sd]h
			 *	instructions
			 */
			generate($1->nbits,NOACTION,$1->opcode,NULLSYM);
			if ($2.admode == FPIMMD) {
				if ($1->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&($2.adexpr));
			}
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			memtoreg = 0;
#endif
		}
	|	FPDYAOP2 gen COMMA dest = {
#if	FLOAT
			/* mov[hw][sd] instructions */
			generate($1->nbits,NOACTION,$1->opcode,NULLSYM);
			addrgen($1,&($2),NOTYPE,1);
			addrgen($1,&($4),NOTYPE,2);
			memtoreg = 0;
#endif
		}
	|	FPTRIOP1 fpgen COMMA fpgen COMMA dest = {
#if	FLOAT
			/* fadd[sd]3 fsub[sd]3 fmul[sd]3 fdiv[sd]3 */
			if ($1->tag == INSTRD) {	/* double operands */
				if (($2.admode==REGMD) && ($4.admode==REGMD))
				/* see if register pairs are used correctly */
					if (abs($4.adreg - $2.adreg) == 1)
					yyerror("invalid double register usage");
			}
			generate($1->nbits,NOACTION,$1->opcode,NULLSYM);
			if ($2.admode == FPIMMD) {
				if ($1->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&($2.adexpr));
			}
			addrgen($1,&($2),NOTYPE,1);
			if ($4.admode == FPIMMD) {
				if ($1->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&($4.adexpr));
			}
			addrgen($1,&($4),NOTYPE,2);
			addrgen($1,&($6),NOTYPE,3);
			memtoreg = 0;
#endif
		}
	|	FPCMPOP1 fpgen COMMA fpgen = {
#if	FLOAT
			/* fcmp[sd] instructions */
			if ($1->tag == INSTRD) {	/* fcmpd opcode */
				if (($2.admode==REGMD) && ($4.admode==REGMD))
				/* see if register pairs are used correctly */
					if (abs($4.adreg - $2.adreg) == 1)
					yyerror("invalid double register usage");
			}
			generate($1->nbits,NOACTION,$1->opcode,NULLSYM);
			if ($2.admode == FPIMMD) {
				if ($1->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&($2.adexpr));
			}
			addrgen($1,&($2),NOTYPE,1);
			if ($4.admode == FPIMMD) {
				if ($1->tag != INSTRD)	/* change dbl -> single */
					fpdtos(&($4.adexpr));
			}
			addrgen($1,&($4),NOTYPE,2);
			memtoreg = 0;
#endif
		}
	|	asmdir
	;
gen	:	immode
	|	regmode
	|	regdefmode
	|	extaddrmode
	|	absaddrmode
	|	extaddrdefmode
	|	absaddrdefmode
	|	dispmode
	|	dispdefmode
	;
fpgen	:	fpimmode
	|	dest
	;
dest	:	regmode
	|	mem
	;
mem	:	extaddrmode
	|	absaddrmode
	|	extaddrdefmode
	|	absaddrdefmode
	|	dispmode
	|	dispdefmode
	|	regdefmode
	;
optoperspec:	OPERSPEC = { 			/* optional operand specifier */
		/* operand specifier type is found in opcode field of instab */
		$$ = ($1->opcode);
		}
	|	/* empty */ = {
		$$ = NULLSPEC;
		}
	;
optimmode :	immode 		/* optional immediate operand addressing mode */
	|	/* empty */ = {
			$$.admode = IMMD;
			$$.newtype = NOTYPE;
			$$.adexpr.exptype = ABS;
			$$.adexpr.expval = 0;
			$$.adexpr.symptr = NULL;
			$$.expspec = NULLSPEC;
		}
	;
immode	:	AMP expr optoperspec = {
			$$.admode = IMMD;
			$$.newtype = NOTYPE;
			$$.adexpr = $2;
			$$.expspec = $3;
		}
	|	LC EXPTYPE RC immode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = IMMD;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
fpimmode:	AMP fpnumber = {	/* immed. mode for fp numbers only */
#if	FLOAT
			$$.admode = FPIMMD;
			$$.newtype = NOTYPE;
			$$.adexpr = $2;
#endif
		}
	;
regmode	:	REGISTER = {
			$$.admode = REGMD;
			$$.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			$$.adreg = (short)($1->opcode);
			$$.expspec = NULLSPEC;
		}
	|	LC EXPTYPE RC regmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = REGMD;
			$$.adreg = $4.adreg;
			$$.expspec = $4.expspec;
		}
	;
regdefmode :	LP  REGISTER  RP = {
			$$.admode = REGDFMD;
			$$.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			$$.adreg = (short)($2->opcode);
			$$.expspec = NULLSPEC;
		}
	|	MUL  REGISTER = {
			$$.admode = REGDFMD;
			$$.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			$$.adreg = (short)($2->opcode);
			$$.expspec = NULLSPEC;
		}
	|	LC EXPTYPE RC regdefmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = REGDFMD;
			$$.adreg = $4.adreg;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
extaddrmode:	expr = {
			$$.admode = EXADMD;
			$$.newtype = NOTYPE;
			$$.adexpr = $1;
			$$.expspec = NULLSPEC;
		}
	|	LC EXPTYPE RC extaddrmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = EXADMD;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
absaddrmode:	DOLLAR  expr = {
			$$.admode = ABSMD;
			$$.newtype = NOTYPE;
			$$.adexpr = $2;
			$$.expspec = NULLSPEC;
		}
	|	LC EXPTYPE RC absaddrmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = ABSMD;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
extaddrdefmode:	MUL expr = {
			$$.admode = EXADDFMD;
			$$.newtype = NOTYPE;
			$$.adexpr = $2;
			$$.expspec = NULLSPEC;
		}
	|	LC EXPTYPE RC extaddrdefmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = EXADDFMD;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
absaddrdefmode:	MUL  DOLLAR  expr = {
			$$.admode = ABSDFMD;
			$$.newtype = NOTYPE;
			$$.adexpr = $3;
			$$.expspec = NULLSPEC;
		}
	|	LC EXPTYPE RC absaddrdefmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = ABSDFMD;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
dispmode:	expr  LP  REGISTER  RP optoperspec = {
			$$.admode = DSPMD;
			$$.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			$$.adreg = (short)($3->opcode);
			$$.adexpr = $1;
			$$.expspec = $5;
		}
	|	LC EXPTYPE RC dispmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = DSPMD;
			$$.adreg = $4.adreg;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
dispdefmode:	MUL  expr  LP  REGISTER  RP optoperspec = {
			$$.admode = DSPDFMD;
			$$.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			$$.adreg = (short)($4->opcode);
			$$.adexpr = $2;
			$$.expspec = $6;
		}
	|	MUL  LP  REGISTER  RP optoperspec = {
			$$.admode = DSPDFMD;
			$$.newtype = NOTYPE;
			/* register number is in opcode field of instab */
			$$.adreg = (short)($3->opcode);
			$$.adexpr.exptype = ABS;
			$$.adexpr.expval = 0;
			$$.adexpr.symptr = NULL;
			$$.expspec = $5;
		}
	|	LC EXPTYPE RC dispdefmode = {
			if ($4.newtype != NOTYPE)
				yyerror("Multiple expanded type operand");
			$$.newtype = (short)($2->opcode);
			$$.admode = DSPDFMD;
			$$.adreg = $4.adreg;
			$$.adexpr = $4.adexpr;
			$$.expspec = $4.expspec;
		}
	;
expr	:	term
	|	expr PLUS term = {
			if ($1.symptr == NULL) {
				$$.symptr = $3.symptr;
				$$.exptype = $3.exptype;
			}
			else if ($3.symptr == NULL) {
				$$.symptr = $1.symptr;
				$$.exptype = $1.exptype;
			}
			else {
				yyerror("Illegal addition");
			}

			$$.expval = $1.expval + $3.expval;
		}
	|	expr MINUS term = {
			if ($1.symptr == NULL) {
				if ($3.symptr != NULL) {
					yyerror("Illegal subtraction");
				}
				$$.symptr = NULL;
				$$.exptype = ABS;
				$$.expval = $1.expval - $3.expval;
			}
			else if ($3.symptr == NULL) {
				$$.symptr = $1.symptr;
				$$.exptype = $1.exptype;
				$$.expval = $1.expval - $3.expval;
			}
			else if ($1.exptype == $3.exptype) {
				$$.symptr = NULL;
				$$.exptype = ABS;
				$$.expval
				= ($1.symptr->value - $3.symptr->value)
				+ ($1.expval - $3.expval);
			}
			else {
				yyerror("Illegal subtraction");
				$$.symptr = NULL;
				$$.exptype = ABS;
				$$.expval = 0;
			}
		}
	|	expr MUL term = {
			if ($1.symptr != NULL || $3.symptr != NULL) {
				yyerror("Illegal multiplication");
			}
			$$.symptr = NULL;
			$$.exptype = ABS;
			$$.expval = $1.expval * $3.expval;
		}
	|	expr DIV term = {
			if ($1.symptr != NULL || $3.symptr != NULL) {
				yyerror("Illegal division");
			}
			$$.symptr = NULL;
			$$.exptype = ABS;
			$$.expval = $1.expval / $3.expval;
		}
	;
term	:	ID = {
			if (($$.exptype = $1->styp & TYPE) == ABS) {
				$$.expval = $1->value;
				$$.symptr = NULL;
			}
			else {
				$$.symptr = $1;
				$$.expval = 0;
			}
		}
	|	NUMBER = {
			$$.exptype = ABS;
			$$.expval = $1;
			$$.symptr = NULL;
		}
	|	LP expr RP = {
			$$ = $2;
		}
	|	MINUS term = {
		$$.exptype = $2.exptype;
		$$.symptr = $2.symptr;
		$$.expval = - $2.expval;
		}
	;
asmdir	:	SECTIONN = {
			cgsect((short)($1->tag));
		}
	|	PSIDENT STRING = {
			comment($2);
		}
	|	PSIL = {
			generate(0,SETEXPAND,NULLVAL,NULLSYM);
		}
	|	PSGLOBAL ID = {
			$2->styp |= EXTERN;
		}
	|	PSSET ID COMMA expr = {
			if ($2 == NULL) /* in case of error */
				aerror("Unable to define identifier");
			else {
				/* reset all but EXTERN bit */
				if ($2==dot)
					if (($2->styp != $4.exptype))
						yyerror("Incompatible types");
					else {
						long incr;

						if ((incr = $4.symptr->value + $4.expval - $2->value) < 0)
							yyerror("Cannot decrement '.'");
						else
							fill(incr);
					} /* $2->styp == $4.expval */
				else { /* $2!=dot */
					$2->styp &= EXTERN;
					if (($2->styp |= $4.exptype & TYPE) == UNDEF)
						yyerror("Illegal `.set' expression");
					$2->value = $4.expval;
					if ($4.symptr != NULL) {
						$2->value += $4.symptr->value;
						$2->maxval = $4.symptr->maxval;
					}
				} /* else */
			} /* else */
		}
	|	PSZERO expr = {
			long val;

			if ($2.exptype != ABS)
				yyerror("`.zero' size not absolute");
			if ((val = $2.expval) < 0)
				yyerror("Invalid `.zero' size");
			if (dot->styp != DAT)
				yyerror("`.zero' valid only in .data");
			fill(val);
		}
	|	PSBSS ID COMMA expr COMMA expr = {
			if ($6.exptype != ABS)
				aerror("`.bss' alignment not absolute");
			bss($2,$4.expval,$6.expval,$4.exptype);
		}
	|	PSALIGN expr = {
			if ($2.exptype != ABS)
				yyerror("'.align' field not absolute");
			ckalign($2.expval);
		}
	|	PSCOMM ID COMMA expr = {
			if (($2->styp & TYPE) != UNDEF)
				yyerror("Illegal attempt to redefine symbol");
			else {
				$2->styp = (EXTERN | UNDEF);
				$2->value = $4.expval;
			};
		}
	|	dotbyte exprlst = {
			fillbyte();
		}
	|	dothalf exprlst = {
			fillbyte();
			ckalign(2L);
		}
	|	dotword exprlst = {
			fillbyte();
			ckalign(4L);
		}
	|	dotfloat fpvalst = {
			ckalign(4L);
		}
	|	PSFILE STRING = {
			if (cfile[0] != '\0')
				yyerror("Only 1 `.file' allowed");
			if (strlen($2) > 14)
				yyerror("`.file' name too long (14 character limit)");
			else
				strcpy(cfile,$2);
			generate(0,SETFILE,NULLVAL,NULLSYM);
		}
	|	PSDEF ID = {
			generate(0,DEFINE,NULLVAL,$2);
		}
	|	PSENDEF = {
			generate(0,ENDEF,NULLVAL,NULLSYM);
		}
	|	attrib expr = {
			generate(0,(unsigned short)$1,$2.expval,$2.symptr);
		}
	|	PSTAG ID = {
			generate(0,SETTAG,NULLVAL,$2);
		}
	|	dotdim
	|	PSLN expr = {
			cline = (short)$2.expval;
			generate(0,LLINENO,$2.expval,$2.symptr);
		}
	|	PSLN expr COMMA expr = {
			cline = (short)$2.expval;
			generate(0,LLINENUM,$2.expval,$2.symptr);
			generate(0,LLINEVAL,$4.expval,$4.symptr);
		}
	;
attrib	:	PSVAL = {
			$$ = SETVAL;
		}
	|	PSSCL = {
			$$ = SETSCL;
		}
	|	PSTYPE = {
			$$ = SETTYP;
		}
	|	PSLINE = {
			$$ = SETLNO;
		}
	|	PSSIZE = {
			$$ = SETSIZ;
		}
	;
dotdim	:	PSDIM expr = {
			generate(0,SETDIM1,$2.expval,$2.symptr);
		}
	|	dotdim COMMA expr = {
			generate(0,SETDIM2,$3.expval,$3.symptr);
		}
	;
dotbyte	:	PSBYTE = {
			spctype = NBPW / 4;
		}
	;
dothalf	:	PSHALF = {
			spctype = NBPW / 2;
			ckalign(2L);
		}
	;
dotword	:	PSWORD = {
			spctype = NBPW;
			ckalign(4L);
		}
	;
dotfloat:	PSFLOAT = {
			spctype = NBPW;
			ckalign(4L);
		}
	|	PSDOUBLE = {
			spctype = 2 * NBPW;
			ckalign(4L);
		}
	;
exprlst	:	dgexpr = {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		}
	|	exprlst COMMA dgexpr = {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		}
	;
dgexpr	:	expr = {
			width = spctype;
			if (bitpos + width > spctype)
				yyerror("Expression crosses field boundary");
			/*
			 * Figure out which action routine to use
			 * in case there is an unresolved symbol.
			 * If a full word is being used, then
			 * a relocatable may be specified.
			 * Otherwise it is restricted to being an
			 * absolute (forward reference).
			 */
			action = (width == NBPW) ? RELDAT32 : RESABS;
			generate(width,($1.symptr != NULLSYM)*action,
				$1.expval,$1.symptr);
		}
	|	NUMBER COLON expr = {
			width = $1;
			if (bitpos + width > spctype)
				yyerror("Expression crosses field boundary");
			action = (width == NBPW) ? RELDAT32 : RESABS;
			generate(width,($3.symptr != NULLSYM)*action,
				$3.expval,$3.symptr);
		}
	;
fpvalst	:	fpdgexpr = {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		}
	|	fpvalst COMMA fpdgexpr = {
			dot->value = newdot; /* syncronize */
			generate(0,NEWSTMT,(long)line,NULLSYM);
		}
	;
fpdgexpr:	fpnumber = {
#if	FLOAT
			width = spctype;
			if (bitpos + width > spctype)
				yyerror("Expression crosses field boundary");
			if (spctype != NBPW) {	/* double fp number */
				generate(32,NOACTION,$1.expval,$1.symptr);
				generate(32,NOACTION,$1.fdexpval2,$1.symptr);
			} else {	/* single fp number */
				fpdtos(&($1));	/* convert dbl to single */
				generate(32,NOACTION,$1.expval,$1.symptr);
			}
#endif
		}
	;
fpnumber:	FPNUMBER = {
#if	FLOAT
			$$.exptype = ABS;
			$$.symptr = NULL;
			$$.expval = $1.expval;
			$$.fdexpval2 = $1.fdexpval2;
#endif
		}
	|	MINUS FPNUMBER = {
#if	FLOAT
			$$.exptype = ABS;
			$$.symptr = NULL;
			$$.expval = $2.expval | (1L<<31);   /* set sign bit */
			$$.fdexpval2 = $2.fdexpval2;
#endif
		}
	;
%%

#if	FLEXNAMES
char yytext[BUFSIZ];
#else
char yytext[27];
#endif

short	type[] = {
	EOF,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	SP,	NL,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	SP,	0,	QUOTE,	SHARP,	DOLLAR,	REG,	AMP,	SQ,
	LP,	RP,	MUL,	PLUS,	COMMA,	MINUS,	ALPH,	DIV,
	DIG,	DIG,	DIG,	DIG,	DIG,	DIG,	DIG,	DIG,
	DIG,	DIG,	COLON,	SEMI,	LT,	0,	GT,	QUEST,
	0,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	LB,	0,	RB,	0,	ALPH,
	0,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,	ALPH,
	ALPH,	ALPH,	ALPH,	LC,	OR,	RC,	0,	0,
};

yylex()
{
	upsymins istp;
	static short
		labfield = 1, /* indicates if might be in a label field */
		mnemseen = 0; /* indicates if mnemonic seen yet */
	register char *yychrptr;
	register short c;
	long val;
	register short ctype;
	short base;
#if	FLOAT
	rexpr fpval;			/* floating point constant */
	extern short validfp(), ato3bf();	/* fp service routines */
#endif

	while (type[(c = getc(fdin)) + 1] == SP)
		labfield = 0;

	switch (ctype = type[c+1]) {
	case DIG:
#if	FLOAT
		yychrptr = yytext;
		*yychrptr++ = c;	/* save first char */
#endif
		val = c - '0';
		if (c=='0') {
			c = getc(fdin);
			if (c=='x' || c=='X') {
				base = 16;
			} else {
#if	FLOAT
				if (c=='.' || c=='E' || c=='e')
					base = 10;	/* fp number */
				else
#endif
					base = 8;
				ungetc(c,fdin);
			}
		} else
			base = 10;

		while ( (type[(c = getc(fdin))+1] == DIG)
		    || ((base==16) &&
			((('a'<=c) && (c<='f'))||(('A'<=c) && (c<='F')))))
		{
			if (base==8)
				val <<= 3;
			else if (base==10) {
				val *= 10;
#if	FLOAT
				*yychrptr++ = c;	/* save for fp check */
#endif
			} else
				val <<= 4;
			if ('a'<=c && c<='f')
				val += 10 + c - 'a';
			else if ('A'<=c && c<='F')
				val += 10 + c - 'A';
			else	val += c - '0';
		}

#if	FLOAT
			/* check if its a fp number */
		if ((base==10) && (c=='.' || c=='E' || c=='e')) {
			do {
				if (yychrptr < &yytext[sizeof(yytext)-2])
					*yychrptr++ = c;
				if (c == 'E' || c == 'e') {
					/* exp for fp number, next char might
					   be a sign (PLUS/MINUS) so get it here */
					if (((c = getc(fdin))=='+') || (c=='-'))
						*yychrptr++ = c;
					else	ungetc(c,fdin);
				}
			} while (( (ctype = type[(c = getc(fdin))+1]) == DIG)
			     ||	(ctype == ALPH));

			ungetc(c,fdin);
			*yychrptr = '\0';
			if (validfp(yytext)) {
				if (ato3bf(yytext,&fpval))
					yyerror("fp conversion error");
				yylval.uuexpr = fpval;
				return(FPNUMBER);
			} else	return(ERR);

		} else {	/* if( (base==10)... */
#endif
			ungetc(c,fdin);
			yylval.uulong = val;
			return(NUMBER);
#if	FLOAT
		}
#endif

	case REG:
		yychrptr = yytext;
		while (((ctype = type[(c = getc(fdin))+1]) == ALPH)
			     ||	(ctype == DIG))
		{
#if	FLEXNAMES
			if (yychrptr < &yytext[BUFSIZ-1])
#else
			if (yychrptr < &yytext[NCPS])
#endif
				*yychrptr++ = c;
		}
		ungetc(c,fdin);
		*yychrptr = '\0';
		istp = *lookup(yytext,N_INSTALL,MNEMON);
		if ((istp.itp == NULL) || (istp.itp->tag == 0)
			|| (istp.itp->val != REGISTER-256))
			yyerror("Invalid register name");
		yylval.uuinsptr = istp.itp;
		return(REGISTER);
	case ALPH:
		yychrptr = yytext;
		do {
#if	FLEXNAMES
			if (yychrptr < &yytext[BUFSIZ-1])
#else
			if (yychrptr < &yytext[NCPS])
#endif
				*yychrptr++ = c;
		} while (((ctype = type[(c = getc(fdin))+1]) == ALPH)
			     ||	(ctype == DIG));
		ungetc(c,fdin);
		*yychrptr = '\0';
		if (labfield) {
			/* found a label; lookup USRNAME */
			istp = *lookup(yytext,INSTALL,USRNAME);
			labfield = 0;
		}
		else {
			if (mnemseen)
				istp = *lookup(yytext,INSTALL,USRNAME);
			else {
				istp = *lookup(yytext,N_INSTALL,MNEMON);
				if ((istp.stp==NULL) || (istp.stp->tag==0)) {
					yyerror("Invalid instruction name");
					return(ERR);
				}
				yylval.uuinsptr = istp.itp;
				mnemseen = 1; /* set for next time */
				return(istp.itp->val + 256);
			} /* else */
		}
		yylval.uusymptr = istp.stp;
		return(ID);
	case QUOTE:
		yylval.uustrptr = yychrptr = yytext;
		while ((c = getc(fdin)) != '"') {
#if !ONEPROC
			putc(c,fdstring);
#endif
			if (yychrptr < &yytext[sizeof(yytext)] - 1)
				*yychrptr++ = c;
		}
#if !ONEPROC
		putc('\n',fdstring);
#endif
		*yychrptr = '\0';
		return(STRING);
	case LC:
		mnemseen = 0;
		return(LC);
	case 0:
		yyerror("illegal character");
		return(NOCHAR);

	case SEMI:
		/* reinitialize for next statement */
		mnemseen = 0;
		labfield = 1;
		return(SEMI);
	case SHARP:
		/* comment; skip rest of line */
		while (getc(fdin) != '\n');
		/* same as in case NL */
	case NL:
		/* reinitialize for next statement */
		mnemseen = 0;
		labfield = 1;
		return(NL);
	case LT:
		/* start of operand descriptor */
		yychrptr = yytext;
		do {
			if (yychrptr < &yytext[NCPS])
				*yychrptr++ = c;
		} while (((ctype = type[(c = getc(fdin))+1]) == ALPH)
			     ||	(ctype == GT));
		ungetc(c,fdin);
		*yychrptr = '\0';
		istp = *lookup(yytext,N_INSTALL,MNEMON);
		if ((istp.itp == NULL) || (istp.itp->tag == 0)
			|| (istp.itp->val != OPERSPEC-256))
			yyerror("Invalid operand specifier.");
		yylval.uuinsptr = istp.itp;
		return(OPERSPEC);
	default:
		return(ctype);
	} /* switch */
} /* yylex */

fill(nbytes)
long nbytes;
{
	long fillval;

	fillval = (dot->styp == TXT)	? ((TXTFILL<<8)|TXTFILL)
					: ((FILL<<8)|FILL);
	while (nbytes >= 2) {
		generate(2*BITSPBY,NOACTION,fillval,NULLSYM);
		nbytes -= 2;
	}
	if (nbytes)
		generate(BITSPBY,NOACTION,fillval,NULLSYM);
} /* fill */

ckalign(size)
long size;
{
	long mod;
	if ((mod = newdot % size) != 0) {
		fill(size - mod);
	}
} /* ckalign */

fillbyte()
{
	if (bitpos)
		generate(BITSPBY-bitpos,NOACTION,FILL,NULLSYM);
} /* fillbyte */

bss(sym,val,alignment,valtype)
symbol *sym;
long val,alignment;
short valtype;
{
	long mod;

	if (valtype != ABS)
		yyerror("`.bss' size not absolute");
	if (val <= 0)
		yyerror("Invalid `.bss' size");
	if ((sym->styp & TYPE) != UNDEF)
		yyerror("multiply define label in `.bss'");
	if (mod = dotbss % alignment)
		dotbss += alignment - mod;
	sym->value = dotbss;
	dotbss += val;
	sym->styp |= BSS;
} /* bss */
