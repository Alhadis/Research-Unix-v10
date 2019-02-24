#include <stdio.h>

#include "Xoptab.h"

#include "Xoptab.c"

#define	swab

main()
{
	register struct optab *op;

	for (op = optab; op->opname; op++) {
		if (op->size == BWL)
			putbwl(op);
		else
			putopt(op);
	}
}

putbwl(op)
register struct optab *op;
{
	char *orig;
	char new[20];

	orig = op->opname;
	op->opname = new;
	sprintf(new, "%s.b", orig);
	/* size field in opcode == 0 */
	op->size = B;
	putopt(op);
	sprintf(new, "%s.w", orig);
	op->opcode |= 0100;	/* size == 01 */
	op->size = W;
	putopt(op);
	sprintf(new, "%s.l", orig);
	op->opcode &=~ 0100;
	op->opcode |= 0200;	/* size == 03 */
	op->size = L;
	putopt(op);
}

/*
 * struct optab {
 * 	unsigned short opcode;
 * 	unsigned short mask;
 * 	unsigned short op2;
 * 	unsigned short mk2;
 * 	char	*opname;
 * 	char	flags;
 * 	char	nrand;
 * 	short	rand[MAXRAND];
 * };
 */

putopt(op)
register struct optab *op;
{
	register int n;
	long mask;
	char *mkrand();
	long mkmask();

	printf("{ ");
	printf("0x%04x, ", swab(op->opcode));	/* opcode */
	mask = mkmask(op);
	printf("0x%04x, ", swab((int)mask&0xffff));	/* mask */
	if ((((int)mask & 0xffff) & op->opcode) != op->opcode)
		fprintf(stderr, "awful mask %s\n", op->opname);
	if ((op->iflag & I2W) == 0)
		printf("0x0000, 0x0000, ");	/* op2 mk2 */
	else {
		mask = (mask >> 16) & 0xffff;
		printf("0x%04x, ", swab(op->op2code));	/* op2 */
		printf("0x%04x, ", swab((int)mask));	/* mk2 */
		if (((int)mask & op->op2code) != op->op2code)
			fprintf(stderr, "awful 2mask %s\n", op->opname);
	}
	printf("\"%s\", ", op->opname);	/* name */
	switch (op->size) {
	case B:
		printf("B");
		break;

	case W:
		printf("W");
		break;

	case L:
		printf("L");
		break;

	default:
		fprintf(stderr, "bad size %s\n", op->opname);
		printf("GOK ");
	case 0:
		printf("NZ");
		break;
	}
	if (op->iflag & I2W)
		printf("|I2W");
	printf(", ");			/* flags */
	n = nrand(op);
	printf("%d", n);		/* nrand */
	if (n > 0)
		printf(", %s", mkrand(op->addr1, op->a1disp));
	if (n > 1)
		printf(", %s", mkrand(op->addr2, op->a2disp));
	if (n > 2)
		printf(", %s", mkrand(op->addr3, op->a3disp));
	if (n > 3)
		printf(", %s", mkrand(op->addr4, op->a4disp));
	printf(" },\n");
}

static long opmask[] = {
	0,		/* DIG	0   ignore this address */
	0x0000003f,	/* DEA	1   E.A. to low order 6 bits */
	0x00000007,	/* DRG	2   register to low order 3 bits */
	0x00000e00,	/* DRGL	3   register to bits 11-9 */
	0x000000ff,	/* DBR	4   branch offset (short) */
	0x000000ff,	/* DMQ	5   move-quick 8-bit value */
	0x00000e00,	/* DAQ	6   add-quick 3-bit value in 11-9 */
	0,		/* DIM	7   Immediate value, according to size */
	0x00000fc0,	/* DEAM	8   E.A. to bits 11-6 as in move */
	0,		/* DBCC	9   branch address as in "dbcc" */
	0,		/* DIMH	10  Immediate forced to be 1 word */
	0x00070000,	/* D2L	11  register to bits 0-2 of next word */
	0x70000000,	/* D2H	12  register to bits 12-14 of next word */
	0x001f0000,	/* DBL	13  qty in bits 0-5 of next word */
	0x07c00000,	/* DBH	14  qty in bits 6-11 of next word */
	0,		/* DCR	15  control reg a bit combination in 0-11 */
};

long
mkmask(op)
register struct optab *op;
{
	register long x;

	x = opmask[op->a1disp]|opmask[op->a2disp]|opmask[op->a3disp]|opmask[op->a4disp];
	return (~x);
}

int
nrand(op)
register struct optab *op;
{

	if (op->addr1 == 0)
		return (0);
	if (op->addr2 == 0)
		return (1);
	if (op->addr3 == 0)
		return (2);
	if (op->addr4 == 0)
		return (3);
	return (4);
}

char *
mkrand(addr, disp)
int addr, disp;
{
	char *regmode();

	switch (addr) {
	case AIMM+O:
		return ("DIM|AONE");

	case AREG+C:
		return ("DSREG|C");

	case AREG+SR:
		return ("DSREG|SR");

	case AREG+U:
		return ("DSREG|U");
	}
	switch (disp) {
	case DIG:
		fprintf(stderr, "unaccounted DIG\n");
		return ("DIG");

	case DEA:
		return ("DEA");

	case DRG:
		return (regmode(addr, "DRG"));

	case DRGL:
		return (regmode(addr, "DRGL"));

	case DBR:
		return ("DBR");

	case DMQ:
		return ("DMQ");

	case DAQ:
		return ("DAQ");

	case DIM:
		switch (addr) {
		case AIMM:
			return ("DIM");

		case AIMM+O:
			return ("DIM|AONE");

		case AIMM+H:
			return ("DIM|AWORD");
		}
		fprintf(stderr, "unknown DIM\n");            
		return ("GOK DIM");

	case DEAM:
		return ("DEAM");

	case DBCC:
		return ("DBCC");

	case DIMH:
		return ("DIM|AWORD");

	case D2L:
		return (regmode(addr, "D2L"));

	case D2H:
		return (regmode(addr, "D2H"));

	case DBL:
		return ("DBL");

	case DBH:
		return ("DBH");

	case DCR:
		return ("DCR");

	default:
		fprintf(stderr, "GOK rand\n");
		return ("GOK");
	}
}

#ifndef swab
int
swab(s)
int s;
{

	return (((s >> 8) & 0xff) | ((s << 8) & 0xff00));
}
#endif

char *
regmode(addr, rb)
int addr;
char *rb;
{
	static char buf[10];
	char *mod;

	if (addr == ADEC)
		mod = "ADEC";
	else if (addr == AINC)
		mod = "AINC";
	else if (addr == AREG + A)
		mod = "AAREG";
	else if (addr == AREG + D)
		mod = "ADREG";
	else {
		fprintf(stderr, "awful regmode\n");
		mod = "GOK";
	}
	sprintf(buf, "%s|%s", rb, mod);
	return (buf);
}
