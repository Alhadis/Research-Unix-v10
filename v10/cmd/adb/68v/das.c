/*
 * disassemble 68020 opcodes
 */

#include "defs.h"
#include "optab.h"
#include "space.h"

#define	SZ_MASK		00300
#define	SZ_SHIFT	6		/* bits  (7-6) */

extern	long	A6base;
static	int	dorand();
static	int	ea();
static	int	doindex();
static	int	dcr();
static	char	*fcr();

static struct opmask {
	long mask;
	int shift;
} opmask[] = {
	0,		0,	/* DIG	0   ignore this address */
	0x0000003f,	0,	/* DEA	1   E.A. to low order 6 bits */
	0x00000007,	0,	/* DRG	2   register to low order 3 bits */
	0x00000e00,	9,	/* DRGL	3   register to bits 11-9 */
	0x000000ff,	0,	/* DBR	4   branch offset (short) */
	0x000000ff,	0,	/* DMQ	5   move-quick 8-bit value */
	0x00000e00,	9,	/* DAQ	6   add-quick 3-bit value in 11-9 */
	0,		0,	/* DIM	7   Immediate value, according to size */
	0x00000fc0,	6,	/* DEAM	8   E.A. to bits 11-6 as in move */
	0,		0,	/* DBCC	9   branch address as in "dbcc" */
	0x0000000f,	0,	/* DTRAP 10 immediate in low 4 bits */
	0x00070000,	16,	/* D2L	11  register to bits 0-2 of next word */
	0x70000000,	16+12,	/* D2H	12  register to bits 12-14 of next word */
	0x001f0000,	16,	/* DBL	13  qty in bits 0-5 of next word */
	0x07c00000,	16+6,	/* DBH	14  qty in bits 6-11 of next word */
	0x0fff0000,	16,	/* DCR	15  control reg a bit combination in 0-11 */
	0x00000007,	0,	/* DBKPT 16 immediate in low 3 bits */
	0x1c000000,	16+10,	/* DFSRC 17 floating source specifier */
	0x03800000,	16+7,	/* DFDRG 18 floating destination register */
	0x1c000000,	16+10,	/* DFSRG 19 floating source register */
	0x007f0000,	16,	/* DFCR 20 floating constant register */
	0x00000040,	0,	/* DFBR 21 floating branch offset */
	0x00ff0000,	16,	/* DFMRGM 22 FMOVE register mask */
	0x1c000000,	16+10,	/* DFMRGM 23 FMOVE register mask */
};

static int dsp;
extern struct optab optab[];

printins(isp)
int isp;
{
	register struct optab *op;
	register int i;
	register WORD w, w1;
	int w1f = 0;

	dsp = isp;
	w = stow(sget(dot, isp));
	chkerr();
	for (op = optab; op->opname; op++) {
		if ((w & op->mask) != op->opcode)
			continue;
		if ((op->flags & I2W) == 0)
			break;		/* 1-word match */
		if (w1f == 0) {
			w1 = stow(sget(dot+2, isp));
			w1f++;
		}
		if ((w1 & op->mk2) == op->op2)
			break;		/* 2-word match */
	}
	if (op->opname == NULL) {
		printf("\tnumber\t%R", w);
		dotinc = 2;
		return;
	}
	w &= 0xffff;
	if ((op->flags & I2W) == 0)
		dotinc = 2;
	else {
		w |= w1 << 16;
		dotinc = 4;
	}
	printf("\t%s", op->opname);
	for (i = 0; i < op->nrand; i++) {
		if (i == 0)
			printf("\t");
		else
			printf(",");
		dorand(w, op->rand[i], op->flags & SZ);
	}
}

#define	ENSIGN(x)	((WORD)(short)(x))

static
dorand(w, rand, size)
register WORD w;
register short rand;
int size;
{
	struct opmask *om;
	WORD lval;
	register int val;

	om = &opmask[rand & DMASK];
	if (om->mask)
		val = (w & om->mask) >> om->shift;
	switch(rand & DMASK) {
	case DEA:	/* effective address spec */
		ea(val >> 3, val & 07, size);
		return;

	case DRG:	/* abs register */
	case DRGL:
	case D2H:
	case D2L:
		if (rand & ADEC)
			printf("-(A%d)", val);
		else if (rand & AINC)
			printf("(A%d)+", val);
		else if (rand & AAREG)
			printf("A%d", val);
		else if (rand & ADREG)
			printf("R%d", val);
		else
			printf("DRGgok");
		return;

	case DBR:	/* branch displacement */
		lval = val;
		if (val == 0) {
			lval = stow(sget(dot+dotinc, dsp));
			if (lval & 0x8000)
				lval |= ~0xffff;
			dotinc += 2;
		}
		else if (val == 0xff) {
			lval = ltow(lget(dot+dotinc, dsp));
			dotinc += 4;
		}
		else if (val & 0x80)
			lval |= ~0xff;
		lval += dot + 2;
		psymoff(lval, dsp, "");
		return;

	case DBH:	/* 6-bit strange quick */
	case DBL:	/* other 6-bit strange quick */
		if (val & 040) {
			printf("%%d%d", val & 07);
			return;
		}
		/* fall through */
	case DMQ:	/* 8-bit quick */
	case DTRAP:	/* 4-bit quick */
		printf("$Q");
		psymoff((WORD)val, dsp, "");
		return;

	case DBKPT:	/* 3-bit quick */
		printf("$Q");
		psymoff((WORD)val, dsp, "");
		return;

	case DAQ:	/* silly 3-bit immediate */
		if (val == 0)
			val = 8;
		printf("$Q");
		psymoff((WORD)val, dsp, "");
		return;

	case DIM:	/* immediate */
		if (rand & AONE) {
			printf("$Q1");
			return;
		}
		if (rand & AWORD)
			size = W;
		switch ((int)size) {
		case B:
			lval = stow(sget(dot+dotinc, dsp)) & 0377;
			if (val & 0200)
				val |= ~0377;	/* sign extend */
			dotinc += 2;	/* sic */
			break;

		case W:
			lval = ENSIGN(stow(sget(dot+dotinc, dsp)));
			dotinc += 2;
			break;

		case L:
			lval = ltow(lget(dot+dotinc, dsp));
			dotinc += 4;
			break;

		default:
			lval = 0;
		}
		printf("$");
		psymoff(lval, dsp, "");
		return;

	case DEAM:	/* assinine backwards ea */
		ea(val & 07, val >> 3, size);
		return;

	case DBCC:	/* branch displacement a la dbcc */
		lval = stow(sget(dot+dotinc, dsp));
		dotinc += 2;
		lval += dot + 2;
		psymoff(lval, dsp, "");
		return;

	case DCR:
		dcr(val);
		return;

	case DSREG:
		if (rand & C)
			printf("CCR");
		else if (rand & SR)
			printf("SR");
		else if (rand & U)
			printf("USP");
		else
			printf("GOKdsreg");
		return;

	case DFSRC:
		printf("F%d", val);
		return;

	case DFDRG:
	case DFSRG:
		printf("F%d", val);
		return;

	case DFCR:
		printf("$%s", fcr((int)val));
		return;

	case DFBR:	/* floating branch displacement */
		if (val == 0) {
			lval = stow(sget(dot+dotinc, dsp));
			if (lval & 0x8000)
				lval |= ~0xffff;
			dotinc += 2;
		}
		else {
			lval = ltow(lget(dot+dotinc, dsp));
			dotinc += 4;
		}
		lval += dot + 2;
		psymoff(lval, dsp, "");
		return;

	case DFMRGM:
	case DFMCRGM:
		printf("$%x", val);
		return;
	}
	printf("GOK");
}

static
ea(mode, reg, size)
int mode, reg;
{
	WORD disp;
	float sval;
	double dval;
	switch((int)mode){
	case 0:			/* data reg */
		printf("R%d", reg);
		return;

	case 1:			/* addr reg */
		printf("A%d", reg);
		return;

	case 2:			/* addr reg indir */
		printf("(A%d)", reg);
		return;

	case 3:			/* addr reg indir incr */
		printf("(A%d)+", reg);
		return;

	case 4:			/* addr reg indir decr */
		printf("-(A%d)", reg);
		return;

	case 5:			/* addr reg indir with displ */
		disp = ENSIGN(stow(sget(dot+dotinc, dsp)));
		dotinc += 2;
		psymoff(disp, dsp, "");
		printf("(A%d)", reg);
		if (reg == 6) {
			printf(".");
			psymoff(disp+A6base, DATASP, "");
		}
		return;

	case 6:			/* wretched indexing */
		doindex(reg);	/* ugh */
		return;

	case 7:			/* non-register stuff: */
		switch ((int)reg) {
		case 0:		/* absolute short */
			disp = ENSIGN(stow(sget(dot+dotinc, dsp)));
			dotinc += 2;
			psymoff(disp, dsp, "");
			printf("($0)");
			return;

		case 1:		/* absolute long */
			disp = ltow(lget(dot+dotinc, dsp));
			dotinc += 4;
			psymoff(disp, dsp, "");
			printf("($0)");
			return;

		case 4:		/* immediate */
			switch((int)size) {
			case B:
				disp = ENSIGN(ctow(cget(dot+dotinc, dsp)));
				dotinc += 2;	/* sic */
				printf("$");
				psymoff(disp, dsp, "");
				return;

			case W:
				disp = ENSIGN(stow(sget(dot+dotinc, dsp)));
				dotinc += 2;
				printf("$");
				psymoff(disp, dsp, "");
				return;

			case L:
				disp = ltow(lget(dot+dotinc, dsp));
				dotinc += 4;
				printf("$");
				psymoff(disp, dsp, "");
				return;

			case D:
				fget(dot+dotinc, dsp, (char *)&dval, sizeof dval);
				printf("$");
				fpout('f', &dval);
				dotinc += sizeof dval;
				return;

			case F:
				fget(dot+dotinc, dsp, (char *)&sval, sizeof sval);
				printf("$");
				dval = sval;
				fpout('f', &dval);
				dotinc += sizeof sval;
				return;
			}
		}
	}
	printf("gok%d:%d", mode, reg);
}

static
doindex(reg)
int reg;
{
	register WORD w;
	register WORD base, outer;

	base = outer = 0;
	w = stow(sget(dot+dotinc, dsp));
	dotinc += 2;
	if ((w & 0x100) == 0) {		/* brief format */
		base = w & 0x7f;
		if (base & 0x40)
			base |= ~0x7f;
	}
	else {				/* full format */
		switch ((int)(w & 0x30)) {
		case 0:			/* ugh */
		case 0x10:		/* null displacement */
			break;

		case 0x20:
			base = stow(sget(dot+dotinc, dsp));
			if (base & 0x8000)
				base |= 0xFFFF0000;	/* sign extend */
			outer = stow(sget(dot+dotinc+2, dsp));
			if (!(w&0x80))
				dotinc += 2;	/* for base */
			if ((w&7)==2 || (w&7)==3)
				dotinc += 2;	/* for outer */
			break;

		case 0x30:
			base = ltow(lget(dot+dotinc, dsp));
			if (base & 0x8000)
				base |= 0xFFFF0000;	/* sign extend */
			outer = ltow(lget(dot+dotinc+4, dsp));
			if (!(w&0x80))
				dotinc += 4;	/* for base */
			if ((w&7)==2 || (w&7)==3)
				dotinc += 2;	/* for outer */
			break;
		}
	}
	if ((w & 0x100) && (w & 0x47)) {
		if ((w&7)==2 || (w&7)==3 && outer)
			psymoff(outer, dsp, "");
		printf("(");
	}
	if (base)
		psymoff(base, dsp, "");
	printf("(A%d)", reg);
	if ((w & 0x100) && (w & 0x4))
		printf(")");
	if (reg == 6) {
		printf(".");
		psymoff(base+A6base, DATASP, "");
	}
	if (!(w & 0x40)) {
		printf("(%c%d.%c", w&0100000 ? 'A' : 'R', (int)(w>>12)&07,
			w&04000 ? 'L' : 'W');
		printf("*%D)", (WORD)1<<((w>>9)&03));
	}
	if ((w & 0x100) && (w & 0x43) && !(w & 0x4))
		printf(")");
}

static
dcr(reg)
int reg;
{

	switch (reg) {
	case 0x000:
		printf("SFC");
		return;

	case 0x001:
		printf("DFC");
		return;

	case 0x002:
		printf("CACR");
		return;

	case 0x800:
		printf("USP");
		return;

	case 0x801:
		printf("VBR");
		return;

	case 0x802:
		printf("CAAR");
		return;

	case 0x803:
		printf("MSP");
		return;

	case 0x804:
		printf("ISP");
		return;

	default:
		printf("CR%x", reg);
		return;
	}
}

static struct{
	int	c;
	char	*name;
}fcrtab[]={
	0x00,	"C_PI",
	0x0b,	"C_LOG10(2)",
	0x0c,	"C_E",
	0x0d,	"C_LOG2(E)",
	0x0e,	"C_LOG10(E)",
	0x0f,	"C_0.0",
	0x30,	"C_LOGN(2)",
	0x31,	"C_LOGN(10)",
	0x32,	"C_TENTO0",
	0x33,	"C_TENTO1",
	0x34,	"C_TENTO2",
	0x35,	"C_TENTO4",
	0x36,	"C_TENTO8",
	0x37,	"C_TENTO16",
	0x38,	"C_TENTO32",
	0x39,	"C_TENTO64",
	0x3a,	"C_TENTO128",
	0x3b,	"C_TENTO256",
	0x3c,	"C_TENTO512",
	0x3d,	"C_TENTO1024",
	0x3e,	"C_TENTO2048",
	0x3f,	"C_TENTO4096",
	0x00,	(char *)0,
};

static
char *
fcr(c)
{
	int i;

	for (i=0; fcrtab[i].name; i++)
		if (c == fcrtab[i].c)
			return fcrtab[i].name;
	return "strangeconstant";
}
