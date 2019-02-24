
#define I_ADDF2 	0x40
#define I_ADDF3 	0x41
#define I_DIVF2 	0x46
#define I_DIVF3 	0x47
#define I_MULF2 	0x44
#define I_MULF3 	0x45
#define I_SUBF2 	0x42
#define I_SUBF3 	0x43

#define PC 15

#include <signal.h>

void setsigs();

static void
ovfltrap (signo, param, xx, pc, psl)
	unsigned char *pc;
{
	register int operands = 2;
	register unsigned char *rpc = pc;

	setsigs();		/* trap overflow again */
	psl |= 02;		/* set the overflow bit */

	/* Adjust the PC if this is an op code we know about */
	switch (*rpc) {

	case I_ADDF3:
	case I_SUBF3:
	case I_MULF3:
	case I_DIVF3:
		operands++;

	case I_ADDF2:
	case I_SUBF2:
	case I_MULF2:
	case I_DIVF2:

		rpc++;

		do {

			/* Fetch and step past the first operand byte */
			register int mode = (*rpc & 0xf0) >> 4;
			register int reg = *rpc++ & 0xf;

			switch (mode) {

			case 0: 	/* literal */
			case 1: 	/* literal */
			case 2: 	/* literal */
			case 3: 	/* literal */
				break;

			case 4: 	/* [reg] */
				operands++;
				break;

			case 5: 	/* reg */
			case 6: 	/* (reg) */
			case 7: 	/* -(reg) */
				break;

			case 8: 	/* (reg)+ */
				if (reg == PC)
					rpc += sizeof (float);
				break;

			case 9: 	/* autoincrement deferred or absolute */

				if (reg == PC)
					rpc += 4;
				break;

			case 10:	/* byte displacement */
			case 11:	/* byte displacement deferred */
				rpc++;
				break;

			case 12:	/* word displacement */
			case 13:	/* word displacement deferred */
				rpc += 2;
				break;

			case 14:	/* longword displacement */
			case 15:	/* longword displacement deferred */
				rpc += 4;
				break;
			}
		} while (--operands);

	}
	pc = rpc;
}

void
setsigs()
{
	signal (SIGFPE, ovfltrap);
}

