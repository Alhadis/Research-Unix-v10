static char *ID_addr2 = "@(#) addr2.c: 1.5 12/10/83";
#include <stdio.h>
#include <reloc.h>
#include <storclass.h>
#include <syms.h>
#include "systems.h"
#include "gendefs.h"
#include "symbols.h"
#include "codeout.h"
#include "instab.h"

extern	unsigned short relent;
extern	relpc();
extern long
	newdot;
extern FILE
#if !ONEPROC
	*fdstring,
#endif
	*fdrel,
	*fdcode;
extern upsymins
	*lookup();
extern symbol
	symtab[],
	*dot;
extern BYTE
	*longsdi;
extern short
	filedef;
extern char
	cfile[];

long
swapb2(val)
long val;
{
	long b1,b2;

	b1 = (val >> 8) & 0x00FFL;	/* 2nd low goes to low */
	b2 = (val << 8) & 0xFF00L;	/* low goes to high */
	return (b2|b1);
} /* swapb2 */

long
swapb4(val)
long val;
{
	long b1,b2,b3,b4;

	b1 = (val >> 24) & 0x000000FFL;	/* high goes to low */
	b2 = (val >>  8) & 0x0000FF00L;	/* 2nd high goes to 2nd low */
	b3 = (val <<  8) & 0x00FF0000L;	/* 2nd low goes to 2nd high */
	b4 = (val << 24) & 0xFF000000L;	/* low goes to high */
	return (b1|b2|b3|b4);
} /* swapb4 */

relocat(sym,code,rtype)
	register symbol *sym;
	codebuf *code;
	register short rtype;
{
	char *rsym;
	prelent trelent;
	register short stype;

	if (sym != NULLSYM) {
		code->cvalue += sym->value;
		stype = sym->styp & TYPE;
	}
	else
		return;
	switch (stype) {

	case ABS:
		return;
	case TXT:
		rsym = ".text";
		break;
	case DAT:
		rsym = ".data";
		break;
	case BSS:
		rsym = ".bss";
		break;
	case UNDEF:
		sym->styp |= EXTERN; /* make sure in gets in symbol table */
		rsym = sym->_name.name;
		break;
	default:
		aerror("Invalid type");
	}

	trelent.relval = newdot;
	trelent.relname = rsym;
	trelent.reltype = rtype;
	fwrite(&trelent,sizeof(prelent),1,fdrel);
	++relent;
} /* relocat */

reldir32(sym,code)
	symbol *sym;
	codebuf *code;
{
	relocat(sym,code,R_DIR32S);
	code->cvalue = swapb4(code->cvalue);
} /* reldir32 */

reldat32(sym,code)
	symbol *sym;
	codebuf *code;
{
	relocat(sym,code,R_DIR32);
} /* reldat32 */

savtrans(sym,code)
	symbol *sym;
	register codebuf *code;
{
	if (sym != NULLSYM)
		resabs(sym,code);
	if (code->cvalue < 0 || code->cvalue > 6)
		werror("Invalid number of registers in `sav/ret'");
	code->cvalue = (CREGMD << 4) + (FPREG - (int)(code->cvalue));
} /* savtrans */


resabs(sym,code)
	register symbol *sym;
	codebuf *code;
{
	switch (sym->styp & TYPE) {

	case ABS:
		code->cvalue += sym->value; /* sym must be non-null */
		return;
	case UNDEF:
		yyerror("Undefined symbol in absolute expression");
		return;
	default:
		yyerror("Relocatable symbol in absolute expression");
		return;
	}
} /* resabs */

extern SYMENT
	sment;
extern AUXENT
	axent;

relpc8(sym,code)
	register symbol *sym;
	register codebuf *code;
{
	long val;
	short stype;

	val = code->cvalue;
	if (sym != NULLSYM)
		if ((stype = sym->styp & TYPE) != dot->styp && stype != ABS)
			aerror("relpc8: reference to symbol in another section");
		else
			val += sym->value;
	if ((val -= dot->value) >= (1L << 7) || val < -(1L << 7))
		aerror("relpc8: offset out of range");
	code->cvalue = val;
} /* relpc8 */

relpc16(sym,code)
	register symbol *sym;
	register codebuf *code;
{
	long val;
	short stype;

	val = code->cvalue;
	if (sym != NULLSYM)
		if ((stype = sym->styp & TYPE) != dot->styp && stype != ABS)
			aerror("relpc16: reference to symbol in another section");
		else
			val += sym->value;
	if ((val -= dot->value) >= (1L << 15) || val < -(1L << 15))
		aerror("relpc16: offset out of range");
	code->cvalue = swapb2(val);
} /* relpc16 */

brnopt(sym,code)
	register symbol *sym;
	register codebuf *code;
{
	long opcd;

	if (*++longsdi) {
		if ((opcd = code->cvalue >> 8) != 0) {
			/* generate complementary branch */
			codgen(8,opcd);
			codgen(8,8L); /* length of branch */
			dot->value = newdot; /* resynchronize */
		}
		codgen(8,JMPOPCD);
		codgen(8,(long)CABSMD); /* descriptor for operand */
		if (fread(code,sizeof(*code),1,fdcode) != 1)
			aerror("Unexpeced EOF on temporary (code) file");
		sym = code->cindex ? (symtab + (code->cindex-1))
				: (symbol *)NULL;
		code->cnbits = 32;
		reldir32(sym,code);
	} /* if (*++longsdi) */
} /* brnopt */

bsbnopt(sym,code)
	register symbol *sym;
	register codebuf *code;
{
	if (*++longsdi) {
		codgen(8,code->cvalue >> 8); /* generate long form */
		codgen(8,(long)CABSMD); /* descriptor for operand */
		if (fread(code,sizeof(*code),1,fdcode) != 1)
			aerror("Unexpeced EOF on temporary (code) file");
		sym = code->cindex ? (symtab + (code->cindex-1))
				: (symbol *)NULL;
		code->cnbits = 32;
		reldir32(sym,code);
	} /* if (*++longsdi) */
} /* bsbnopt */

#ifdef	CALLPCREL
callnopt(sym,code)
	register symbol *sym;
	register codebuf *code;
{
	if (*++longsdi) {
		codgen(8,(long)CABSMD); /* descriptor for operand */
		if (fread(code,sizeof(*code),1,fdcode) != 1)
			aerror("Unexpeced EOF on temporary (code) file");
		sym = code->cindex ? (symtab + (code->cindex-1))
				: (symbol *)NULL;
		code->cnbits = 32;
		reldir32(sym,code);
	} /* if (*++longsdi) */
} /* callnopt */
#endif

fndbrlen(sym,code)
	symbol *sym;
	register codebuf *code;
{
	long opcd;

	if (fread(code,sizeof(*code),1,fdcode) != 1)
		aerror("Unexpected EOF on temporary (code) file");
	opcd = code->cvalue;
	codgen(8,(long)(3 + oplen(code)));
	dot->value = newdot; /* resynchronize */
	codgen(8,opcd);
	/* now return with descriptor of operand in "code" */
} /* fndbrlen */

oplen(code)
	register codebuf *code;
{
	static int opndlen[2][16] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 3, 3, 2, 2, 0, 1}, /*  < 15 */
		{1, 1, 1, 1, 5, 3, 2, 5, 5, 5, 3, 3, 2, 2, 5, 1}  /* == 15 */
	};
	register int desc;

	if (fread(code,sizeof(*code),1,fdcode) != 1)
		aerror("Unexpected EOF on temporary (code) file");
	desc = (int)(code->cvalue);
	return(opndlen[(desc & 0xF) == 0xF][desc >> 4]);
} /* opndlen */

shiftval(sym,code)
	symbol	*sym;
	register codebuf *code;
{
	resabs(sym,code); /* symbol must be absolute */
	if (code->cvalue < 1L || code->cvalue > 31L)
		yyerror("Bit position out of range");
	code->cvalue = swapb4(1L << code->cvalue); /* shift and swap */
} /* shiftval */

extern
	define(),
	setval(),
	settyp(),
	setscl(),
	settag(),
	setlno(),
	setsiz(),
	inline(),
	setdim1(),
	setdim2(),
	endef(),
	lineno(),
	linenum(),
	lineval(),
	newstmt(),
	setfile();

int (*(modes[NACTION+2]))() = {
/*0*/	0,
/*1*/	define,
/*2*/	setval,
/*3*/	setscl,
/*4*/	settyp,
/*5*/	settag,
/*6*/	setlno,
/*7*/	setsiz,
/*8*/	setdim1,
/*9*/	setdim2,
/*10*/	endef,
/*11*/	lineno,
/*12*/	linenum,
/*13*/	lineval,
/*14*/	newstmt,
/*15*/	setfile,
/*16*/	reldir32,
/*17*/	resabs,
/*18*/	relpc8,
/*19*/	relpc16,
/*20*/	brnopt,
/*21*/	bsbnopt,
/*22*/	fndbrlen,
/*23*/	shiftval,
/*24*/	reldat32,
/*25*/	savtrans,
/*26*/	inline,
#ifdef	CALLPCREL
/*27*/  callnopt,
#endif
	0 };

#if !ONEPROC
flags(flag)
char flag;
{
	switch(flag) {

	default:
		yyerror("Illegal flag (ignored)");
		break;
	} /* switch */
} /* flags */
#endif
