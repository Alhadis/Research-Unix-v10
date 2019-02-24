/*
 * print an instruction
 */

#include "defs.h"

typedef	unsigned char uchar;
typedef	unsigned short ushort;
typedef	unsigned long ulong;
static	ADDR	p;
static	int	isp;
long	ddisp();

#define	IHEX	0	/* give up */
#define	IDISP	1	/* 1 displacement op */
#define	IFF	2	/* float, float */
#define	IIF	3	/* int, float */
#define	IFI	4	/* float, int */
#define	III	5	/* int, int */
#define	III3	6	/* int, int; 3 byte instr */
#define	II	7	/* int */
#define	IMD	8	/* enter: reg mask, disp */
#define	IN	9	/* exit, restore: rev reg mask */
#define	IQI	10	/* quick, int */
#define	IIID3	11	/* int, int, disp; 3 byte instr */
#define	INONE	12	/* no operands */
#define	IDPC	13	/* pc-relative displacement */
#define	II3	14	/* int; 3 byte */
#define	IRII	15	/* reg, int, int */
#define	IRIID	16	/* reg, int, int, disp */
#define	IM	17	/* reg mask (save) */
#define	IDMOD	18	/* disp which is really a module number */
#define	INONE3	19	/* 3-byte no-operand (string stuff) */
#define	II3X	20	/* 3 byte 1 operand sort of (sfsr) */
#define	IMR	21	/* mem mgt instructions */
#define	IPR	22	/* preg instructions */
#define	ICFG	23	/* setcfg */

static
struct
{
	uchar	f;
	ushort	m;
	ushort	v;
	char	*s;
} t[] = {
/* begin */
	III3, 0x3fff, 0x304e, "absb",
	III3, 0x3fff, 0x334e, "absd",
	IFF, 0x3fff, 0x35be, "absf",
	IFF, 0x3fff, 0x34be, "absl",
	III3, 0x3fff, 0x314e, "absw",
	IQI, 0x3f, 0x4c, "acbb",
	IQI, 0x3f, 0x4d, "acbw",
	IQI, 0x3f, 0x4f, "acbd",
	III, 0x3f, 0x00, "addb",
	III, 0x3f, 0x01, "addw",
	III, 0x3f, 0x03, "addd",
	III, 0x3f, 0x10, "addcb",
	III, 0x3f, 0x13, "addcd",
	III, 0x3f, 0x11, "addcw",
	IFF, 0x3fff, 0x01be, "addf",
	IFF, 0x3fff, 0x00be, "addl",
	III3, 0x3fff, 0x3c4e, "addpb",
	III3, 0x3fff, 0x3f4e, "addpd",
	III3, 0x3fff, 0x3d4e, "addpw",
	IQI, 0x3f, 0x0c, "addqb",
	IQI, 0x3f, 0x0d, "addqw",
	IQI, 0x3f, 0x0f, "addqd",
	III, 0x3f, 0x24, "addrb",
	III, 0x3f, 0x27, "addrd",
	III, 0x3f, 0x25, "addrw",
	II, 0x7ff, 0x57c, "adjspb",
	II, 0x7ff, 0x57f, "adjspd",
	II, 0x7ff, 0x57d, "adjspw",
	III, 0x3f, 0x28, "andb",
	III, 0x3f, 0x2b, "andd",
	III, 0x3f, 0x29, "andw",
	III3, 0x3fff, 0x044e, "ashb",
	III3, 0x3fff, 0x074e, "ashd",
	III3, 0x3fff, 0x054e, "ashw",
	IDPC, 0xff, 0x3a, "bcc",
	IDPC, 0xff, 0x2a, "bcs",
	IDPC, 0xff, 0x0a, "beq",
	IDPC, 0xff, 0x1a, "bne",
	IDPC, 0xff, 0x9a, "bfc",
	IDPC, 0xff, 0x8a, "bfs",
	IDPC, 0xff, 0xda, "bge",
	IDPC, 0xff, 0x6a, "bgt",
	IDPC, 0xff, 0x4a, "bhi",
	IDPC, 0xff, 0xba, "bhs",
	IDPC, 0xff, 0x5a, "bls",
	IDPC, 0xff, 0x7a, "ble",
	IDPC, 0xff, 0xaa, "blo",
	IDPC, 0xff, 0xca, "blt",
	III, 0x3f, 0x08, "bicb",
	III, 0x3f, 0x09, "bicw",
	III, 0x3f, 0x0b, "bicd",
	II, 0x3ff, 0x17c, "bicpsrb",
	II, 0x3ff, 0x17d, "bicpsrw",
	II, 0x3ff, 0x37c, "bispsrb",
	II, 0x3ff, 0x37d, "bispsrw",
	INONE, 0xff, 0xf2, "bpt",
	IDPC, 0xff, 0xea, "br",
	IDPC, 0xff, 0x02, "bsr",
	II, 0x7ff, 0x77c, "caseb",
	II, 0x7ff, 0x77d, "casew",
	II, 0x7ff, 0x77f, "cased",
	III3, 0x3fff, 0x084e, "cbitb",
	III3, 0x3fff, 0x0b4e, "cbitd",
	III3, 0x3fff, 0x0c4e, "cbitib",
	III3, 0x3fff, 0x0f4e, "cbitid",
	III3, 0x3fff, 0x0d4e, "cbitiw",
	III3, 0x3fff, 0x094e, "cbitw",
	IRII, 0x7ff, 0x0ee, "checkb",
	IRII, 0x7ff, 0x1ee, "checkw",
	IRII, 0x7ff, 0x3ee, "checkd",
	III, 0x3f, 0x04, "cmpb",
	III, 0x3f, 0x05, "cmpw",
	III, 0x3f, 0x07, "cmpd",
	IFF, 0x3fff, 0x09be, "cmpf",
	IFF, 0x3fff, 0x08be, "cmpl",
	IIID3, 0x3fff, 0x04ce, "cmpmb",
	IIID3, 0x3fff, 0x05ce, "cmpmw",
	IIID3, 0x3fff, 0x07ce, "cmpmd",
	IQI, 0x7f, 0x1c, "cmpqb",
	IQI, 0x7f, 0x1d, "cmpqw",
	IQI, 0x7f, 0x1f, "cmpqd",
	INONE3, 0xffffff, 0x00040e, "cmpsb",
	INONE3, 0xffffff, 0x00050e, "cmpsw",
	INONE3, 0xffffff, 0x00070e, "cmpsd",
	INONE3, 0xffffff, 0x00840e, "cmpst",
	INONE3, 0xffffff, 0x01040e, "cmpsbb",
	INONE3, 0xffffff, 0x01050e, "cmpswb",
	INONE3, 0xffffff, 0x01070e, "cmpsdb",
	INONE3, 0xffffff, 0x01840e, "cmpstb",
	INONE3, 0xffffff, 0x02040e, "cmpsbw",
	INONE3, 0xffffff, 0x02050e, "cmpsww",
	INONE3, 0xffffff, 0x02070e, "cmpsdw",
	INONE3, 0xffffff, 0x02840e, "cmpstw",
	INONE3, 0xffffff, 0x03040e, "cmpsbbw",
	INONE3, 0xffffff, 0x03050e, "cmpswbw",
	INONE3, 0xffffff, 0x03070e, "cmpsdbw",
	INONE3, 0xffffff, 0x03840e, "cmpstbw",
	INONE3, 0xffffff, 0x06040e, "cmpsbu",
	INONE3, 0xffffff, 0x06050e, "cmpswu",
	INONE3, 0xffffff, 0x06070e, "cmpsdu",
	INONE3, 0xffffff, 0x06840e, "cmpstu",
	INONE3, 0xffffff, 0x07040e, "cmpsbbu",
	INONE3, 0xffffff, 0x07050e, "cmpswbu",
	INONE3, 0xffffff, 0x07070e, "cmpsdbu",
	INONE3, 0xffffff, 0x07840e, "cmpstbu",
	III3, 0x3fff, 0x344e, "comb",
	III3, 0x3fff, 0x374e, "comd",
	III3, 0x3fff, 0x354e, "comw",
	IRII, 0x7ff, 0x36e, "cvtp",
	IDMOD, 0xff, 0x22, "cxp",
	II, 0x3ff, 0x07f, "cxpd",
	III3, 0x3fff, 0x2cce, "deib",
	III3, 0x3fff, 0x2dce, "deiw",
	III3, 0x3fff, 0x2dce, "deid",
	INONE, 0xff, 0xc2, "dia",
	III3, 0x3fff, 0x3cce, "divb",
	III3, 0x3fff, 0x3fce, "divd",
	IFF, 0x3fff, 0x21be, "divf",
	IFF, 0x3fff, 0x20be, "divl",
	III3, 0x3fff, 0x3dce, "divw",
	IMD, 0xff, 0x82, "enter",
	IN, 0xff, 0x92, "exit",
	IRIID, 0x7ff, 0x02e, "extb",
	IRIID, 0x7ff, 0x12e, "extw",
	IRIID, 0x7ff, 0x32e, "extd",
	III3, 0x3fff, 0x0cce, "extsb",
	III3, 0x3fff, 0x0dce, "extsw",
	III3, 0x3fff, 0x0fce, "extsd",
	III3, 0x3fff, 0x046e, "ffsb",
	III3, 0x3fff, 0x056e, "ffsw",
	III3, 0x3fff, 0x076e, "ffsd",
	INONE, 0xff, 0xd2, "flag",
	IFI, 0x3fff, 0x3c3e, "floorfb",
	IFI, 0x3fff, 0x3f3e, "floorfd",
	IFI, 0x3fff, 0x3d3e, "floorfw",
	IFI, 0x3fff, 0x383e, "floorlb",
	IFI, 0x3fff, 0x3b3e, "floorld",
	IFI, 0x3fff, 0x393e, "floorlw",
	III3, 0x3fff, 0x384e, "ibitb",
	III3, 0x3fff, 0x3b4e, "ibitd",
	III3, 0x3fff, 0x394e, "ibitw",
	IRII, 0x7ff, 0x42e, "indexb",
	IRII, 0x7ff, 0x52e, "indexw",
	IRII, 0x7ff, 0x72e, "indexd",
	IRIID, 0x7ff, 0x0ae, "insb",
	IRIID, 0x7ff, 0x1ae, "insw",
	IRIID, 0x7ff, 0x3ae, "insd",
	III3, 0x3fff, 0x08ce, "inssb",
	III3, 0x3fff, 0x09ce, "inssw",
	III3, 0x3fff, 0x0bce, "inssd",
	II, 0x7ff, 0x67f, "jsr",
	II, 0x3ff, 0x27f, "jump",
	II3, 0x7ffff, 0x00f3e, "lfsr",
	IMR, 0x7fff, 0x0b1e, "lmr",
	IPR, 0x7f, 0x6c, "lprb",
	IPR, 0x7f, 0x6d, "lprw",
	IPR, 0x7f, 0x6f, "lprd",
	III3, 0x3fff, 0x144e, "lshb",
	III3, 0x3fff, 0x174e, "lshd",
	III3, 0x3fff, 0x154e, "lshw",
	III3, 0x3ff, 0x24ce, "meib",
	III3, 0x3ff, 0x25ce, "meiw",
	III3, 0x3ff, 0x27ce, "meid",
	III3, 0x3fff, 0x38ce, "modb",
	III3, 0x3fff, 0x3bce, "modd",
	III3, 0x3fff, 0x39ce, "modw",
	III, 0x3f, 0x14, "movb",
	IIF, 0x3fff, 0x043e, "movbf",
	IIF, 0x3fff, 0x003e, "movbl",
	III, 0x3f, 0x17, "movd",
	IIF, 0x3fff, 0x073e, "movdf",
	IIF, 0x3fff, 0x033e, "movdl",
	IFF, 0x3fff, 0x05be, "movf",
	IIF, 0x3fff, 0x1b3e, "movfl",
	IFF, 0x3fff, 0x04be, "movl",
	IIF, 0x3fff, 0x163e, "movlf",
	III, 0x3f, 0x15, "movw",
	IIF, 0x3fff, 0x053e, "movwf",
	IIF, 0x3fff, 0x013e, "movwl",
	III3, 0x3fff, 0x1cce, "movxbd",
	III3, 0x3fff, 0x10ce, "movxbw",
	III3, 0x3fff, 0x1dce, "movxwd",
	III3, 0x3fff, 0x18ce, "movzbd",
	III3, 0x3fff, 0x14ce, "movzbw",
	III3, 0x3fff, 0x19ce, "movzwd",
	III3, 0x3fff, 0x00ce, "movmb",
	III3, 0x3fff, 0x01ce, "movmw",
	III3, 0x3fff, 0x03ce, "movmd",
	IQI, 0x7f, 0x5c, "movqb",
	IQI, 0x7f, 0x5d, "movqw",
	IQI, 0x7f, 0x5f, "movqd",
	INONE3, 0xffffff, 0x00000e, "movsb",
	INONE3, 0xffffff, 0x00010e, "movsw",
	INONE3, 0xffffff, 0x00030e, "movsd",
	INONE3, 0xffffff, 0x00800e, "movst",
	INONE3, 0xffffff, 0x01000e, "movsbb",
	INONE3, 0xffffff, 0x01010e, "movswb",
	INONE3, 0xffffff, 0x01030e, "movsdb",
	INONE3, 0xffffff, 0x01800e, "movstb",
	INONE3, 0xffffff, 0x02000e, "movsbw",
	INONE3, 0xffffff, 0x02010e, "movsww",
	INONE3, 0xffffff, 0x02030e, "movsdw",
	INONE3, 0xffffff, 0x02800e, "movstw",
	INONE3, 0xffffff, 0x03000e, "movsbbw",
	INONE3, 0xffffff, 0x03010e, "movswbw",
	INONE3, 0xffffff, 0x03030e, "movsdbw",
	INONE3, 0xffffff, 0x03800e, "movstbw",
	INONE3, 0xffffff, 0x06000e, "movsbu",
	INONE3, 0xffffff, 0x06010e, "movswu",
	INONE3, 0xffffff, 0x06030e, "movsdu",
	INONE3, 0xffffff, 0x06800e, "movstu",
	INONE3, 0xffffff, 0x07000e, "movsbbu",
	INONE3, 0xffffff, 0x07010e, "movswbu",
	INONE3, 0xffffff, 0x07030e, "movsdbu",
	INONE3, 0xffffff, 0x07800e, "movstbu",
	III3, 0x3fff, 0x0cae, "movsub",
	III3, 0x3fff, 0x0dae, "movsuw",
	III3, 0x3fff, 0x0fae, "movsud",
	III3, 0x3fff, 0x1cae, "movusb",
	III3, 0x3fff, 0x1dae, "movusw",
	III3, 0x3fff, 0x1fae, "movusd",
	III3, 0x3fff, 0x20ce, "mulb",
	III3, 0x3fff, 0x23ce, "muld",
	IFF, 0x3fff, 0x31be, "mulf",
	IFF, 0x3fff, 0x30be, "mull",
	III3, 0x3fff, 0x21ce, "mulw",
	III3, 0x3fff, 0x204e, "negb",
	III3, 0x3fff, 0x234e, "negd",
	IFF, 0x3fff, 0x15be, "negf",
	IFF, 0x3fff, 0x14be, "negl",
	III3, 0x3fff, 0x214e, "negw",
	INONE, 0xff, 0xa2, "nop",
	III3, 0x3fff, 0x244e, "notb",
	III3, 0x3fff, 0x274e, "notd",
	III3, 0x3fff, 0x254e, "notw",
	III, 0x3f, 0x18, "orb",
	III, 0x3f, 0x1b, "ord",
	III, 0x3f, 0x19, "orw",
	III3, 0x3fff, 0x30ce, "quob",
	III3, 0x3fff, 0x33ce, "quod",
	III3, 0x3fff, 0x31ce, "quow",
	II3, 0x7ffff, 0x0031e, "rdval",
	III3, 0x3fff, 0x34ce, "remb",
	III3, 0x3fff, 0x37ce, "remd",
	III3, 0x3fff, 0x35ce, "remw",
	IN, 0xff, 0x72, "restore",
	IDISP, 0xff, 0x12, "ret",
	INONE, 0xff, 0x52, "reti",
	INONE, 0xff, 0x42, "rett",
	III3, 0x3fff, 0x004e, "rotb",
	III3, 0x3fff, 0x034e, "rotd",
	III3, 0x3fff, 0x014e, "rotw",
	IFI, 0x3fff, 0x243e, "roundfb",
	IFI, 0x3fff, 0x273e, "roundfd",
	IFI, 0x3fff, 0x253e, "roundfw",
	IFI, 0x3fff, 0x203e, "roundlb",
	IFI, 0x3fff, 0x233e, "roundld",
	IFI, 0x3fff, 0x213e, "roundlw",
	IDISP, 0xff, 0x32, "rxp",
	II, 0x7ff, 0x1bc, "sccb",
	II, 0x7ff, 0x1bf, "sccd",
	II, 0x7ff, 0x1bd, "sccw",
	II, 0x7ff, 0x13c, "scsb",
	II, 0x7ff, 0x13f, "scsd",
	II, 0x7ff, 0x13d, "scsw",
	II, 0x7ff, 0x03c, "seqb",
	II, 0x7ff, 0x03f, "seqd",
	II, 0x7ff, 0x03d, "seqw",
	II, 0x7ff, 0x4bc, "sfcb",
	II, 0x7ff, 0x4bf, "sfcd",
	II, 0x7ff, 0x4bd, "sfcw",
	II, 0x7ff, 0x43c, "sfsb",
	II, 0x7ff, 0x43f, "sfsd",
	II, 0x7ff, 0x43d, "sfsw",
	II, 0x7ff, 0x6bc, "sgeb",
	II, 0x7ff, 0x6bf, "sged",
	II, 0x7ff, 0x6bd, "sgew",
	II, 0x7ff, 0x33c, "sgtb",
	II, 0x7ff, 0x33f, "sgtd",
	II, 0x7ff, 0x33d, "sgtw",
	II, 0x7ff, 0x23c, "shib",
	II, 0x7ff, 0x23f, "shid",
	II, 0x7ff, 0x23d, "shiw",
	II, 0x7ff, 0x5bc, "shsb",
	II, 0x7ff, 0x5bf, "shsd",
	II, 0x7ff, 0x5bd, "shsw",
	II, 0x7ff, 0x3bc, "sleb",
	II, 0x7ff, 0x3bf, "sled",
	II, 0x7ff, 0x3bd, "slew",
	II, 0x7ff, 0x53c, "slob",
	II, 0x7ff, 0x53f, "slod",
	II, 0x7ff, 0x53d, "slow",
	II, 0x7ff, 0x2bc, "slsb",
	II, 0x7ff, 0x2bf, "slsd",
	II, 0x7ff, 0x2bd, "slsw",
	II, 0x7ff, 0x63c, "sltb",
	II, 0x7ff, 0x63f, "sltd",
	II, 0x7ff, 0x63d, "sltw",
	II, 0x7ff, 0x0bc, "sneb",
	II, 0x7ff, 0x0bf, "sned",
	II, 0x7ff, 0x0bd, "snew",
	IM, 0xff, 0x62, "save",
	III3, 0x3fff, 0x184e, "sbitb",
	III3, 0x3fff, 0x1b4e, "sbitd",
	III3, 0x3fff, 0x1c4e, "sbitib",
	III3, 0x3fff, 0x1f4e, "sbitid",
	III3, 0x3fff, 0x1d4e, "sbitiw",
	III3, 0x3fff, 0x194e, "sbitw",
	ICFG, 0xf87fff, 0x000b0e, "setcfg",
	II3X, 0xf83fff, 0x00373e, "sfsr",
	INONE3, 0xffffff, 0x000c0e, "skpsb",
	INONE3, 0xffffff, 0x000d0e, "skpsw",
	INONE3, 0xffffff, 0x000f0e, "skpsd",
	INONE3, 0xffffff, 0x008c0e, "skpst",
	INONE3, 0xffffff, 0x010c0e, "skpsbb",
	INONE3, 0xffffff, 0x010d0e, "skpswb",
	INONE3, 0xffffff, 0x010f0e, "skpsdb",
	INONE3, 0xffffff, 0x018c0e, "skpstb",
	INONE3, 0xffffff, 0x020c0e, "skpsbw",
	INONE3, 0xffffff, 0x020d0e, "skpsww",
	INONE3, 0xffffff, 0x020f0e, "skpsdw",
	INONE3, 0xffffff, 0x028c0e, "skpstw",
	INONE3, 0xffffff, 0x030c0e, "skpsbbw",
	INONE3, 0xffffff, 0x030d0e, "skpswbw",
	INONE3, 0xffffff, 0x030f0e, "skpsdbw",
	INONE3, 0xffffff, 0x038c0e, "skpstbw",
	INONE3, 0xffffff, 0x060c0e, "skpsbu",
	INONE3, 0xffffff, 0x060d0e, "skpswu",
	INONE3, 0xffffff, 0x060f0e, "skpsdu",
	INONE3, 0xffffff, 0x068c0e, "skpstu",
	INONE3, 0xffffff, 0x070c0e, "skpsbbu",
	INONE3, 0xffffff, 0x070d0e, "skpswbu",
	INONE3, 0xffffff, 0x070f0e, "skpsdbu",
	INONE3, 0xffffff, 0x078c0e, "skpstbu",
	IMR, 0x7fff, 0x0f1e, "smr",
	IPR, 0x7f, 0x2c, "sprb",
	IPR, 0x7f, 0x2d, "sprw",
	IPR, 0x7f, 0x2f, "sprd",
	III, 0x3f, 0x20, "subb",
	III, 0x3f, 0x30, "subcb",
	III, 0x3f, 0x33, "subcd",
	III, 0x3f, 0x31, "subcw",
	III, 0x3f, 0x23, "subd",
	IFF, 0x3fff, 0x11be, "subf",
	IFF, 0x3fff, 0x10be, "subl",
	III3, 0x3fff, 0x2c4e, "subpb",
	III3, 0x3fff, 0x2f4e, "subpd",
	III3, 0x3fff, 0x2d4e, "subpw",
	III, 0x3f, 0x21, "subw",
	INONE, 0xff, 0xe2, "svc",
	III, 0x3f, 0x34, "tbitb",
	III, 0x3f, 0x37, "tbitd",
	III, 0x3f, 0x35, "tbitw",
	IFI, 0x3fff, 0x2c3e, "truncfb",
	IFI, 0x3fff, 0x2f3e, "truncfd",
	IFI, 0x3fff, 0x2d3e, "truncfw",
	IFI, 0x3fff, 0x283e, "trunclb",
	IFI, 0x3fff, 0x2b3e, "truncld",
	IFI, 0x3fff, 0x293e, "trunclw",
	INONE, 0xff, 0xb2, "wait",
	II3, 0x7ffff, 0x0071e, "wrval",
	III, 0x3f, 0x38, "xorb",
	III, 0x3f, 0x3b, "xord",
	III, 0x3f, 0x39, "xorw",
/* end */
	IHEX, 0x0, 0x0, "hex",
};

printins(asp)
int asp;
{
	register long o;
	register i, f;
	ADDR orig;

	isp = asp;
	o = ltow(lget(dot, asp));
	p = dot;
	orig = dot;
	for(i=0; t[i].f; i++)
		if((o&t[i].m) == t[i].v)
			break;
	printf("%s	", t[i].s);
	switch(t[i].f) {
	case IHEX:
		p += 1;
		printf("%02x", o & 0377);
		break;

	case INONE:
		p += 1;
		break;

	case INONE3:
		p += 3;
		break;

	case IDISP:
		p += 1;
		psymoff((WORD)ddisp(), isp, "");
		break;

	case IDMOD:
		p += 1;
		psymoff((WORD)mtotext(ddisp(), isp), isp, "");
		break;

	case IDPC:
		p += 1;
		psymoff((WORD)(ddisp()+dot), isp, "");
		break;

	case IFF:
		p += 3;
		f = (o>>8)|2;
		addr2((int)(o>>14), f, f);
		break;

	case IIF:
		p += 3;
		i = (o>>8);
		f = (o>>10)|2;
		addr2((int)(o>>14), i, f);
		break;

	case IFI:
		p += 3;
		i = (o>>8);
		f = (o>>10)|2;
		addr2((int)(o>>14), f, i);
		break;

	case III:
		p += 2;
		i = o;
		addr2((int)(o>>6), i, i);
		break;

	case III3:
		p += 3;
		i = (o>>8);
		addr2((int)(o>>14), i, i);
		break;

	case IIID3:
		p += 3;
		i = (o>>8);
		addr2((int)(o>>14), i, i);
		printf(",%r", ddisp());
		break;

	case II:
		p += 2;
		i = o;
		dgen((int)(o>>11), i, -1);
		break;

	case II3:
		p += 3;
		dgen((int)(o>>19), 3, -1);
		break;

	case II3X:
		p += 3;
		dgen((int)(o>>14), 3, -1);
		break;

	case IMD:
		p += 2;
		mask((int)(o>>8)&0xff);
		printf(",%R", ddisp());
		break;

	case IM:
		p += 2;
		mask((int)(o>>8)&0xff);
		break;

	case IN:
		p += 2;
		rmask((int)(o>>8)&0xff);
		break;

	case IQI:
		p += 2;
		i = o;
		printf("$");
		quick((int)(o>>7));
		printf(",");
		dgen((int)(o>>11), o, -1);
		break;

	case IRII:
		p += 3;
		printf("r%d,", (int)(o>>11)&0x7);
		i = o;
		addr2((int)(o>>14), i, i);
		break;

	case IRIID:
		p += 3;
		printf("r%d,", (int)(o>>11)&0x7);
		i = o;
		addr2((int)(o>>14), i, i);
		printf(",");
		psymoff((WORD)ddisp(), isp, "");
		break;

	case ICFG:
		p += 3;
		printf("[");
		if (o & 0x8000)
			printf("I");
		if (o & 0x10000)
			printf("F");
		if (o & 0x20000)
			printf("M");
		if (o & 0x40000)
			printf("C");
		printf("]");
		break;

	case IPR:
		p += 2;
		prreg((o >> 7) & 0xf);
		printf(",");
		dgen(o >> 11, o, -1);
		break;

	case IMR:
		p += 1;
		prmr((o >> 15) & 0xf);
		printf(",");
		dgen(o >> 19, 3, -1);
		break;
	}
	dotinc = p - orig;
}

static
addr2(o, i1, i2)
{
	register x1, x2, t;

	x1 = -1;
	x2 = -1;
	t = (o >> 5) & 0x1f;
	if (28 <= t && t <= 31)		/* index mode */
		x1 = cget(p++, isp);
	t = o & 0x1f;
	if (28 <= t && t <= 31)
		x2 = cget(p++, isp);
	dgen(o>>5, i1, x1);
	printf(",");
	dgen(o, i2, x2);
}

static
dgen(g, i, t)
{
	register long v, v1;
	register uchar pp;

	switch(g &= 0x1f) {
	default:
		printf("gok%d", g & 0x1f);
		break;

	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		printf("r%d", g);
		break;

	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		v = ddisp();
		psymoff((WORD)v, isp, "");
		printf("(r%d)", g-8);
		break;

	case 16:
	case 17:
	case 18:
		v = ddisp();
		v1 = ddisp();
		psymoff((WORD)v1, isp, "(");
		psymoff((WORD)v, isp, "");
		printf("(%c%c))", "fss"[g-16], "ppb"[g-16]);
		break;

	case 20:
		v = 0;
		pp = cget(p, isp);
		if(pp & 0200)
		switch(i & 3) {
		case 2:	break;
		case 3:	break;
		case 1:	v = 0xffff0000L; break;
		case 0:	v = 0xffffff00L; break;
		}
		switch(i & 3) {
		case 2:	p += 4;
		case 3:	v |= (long)cget(p++, isp) << 24;
			v |= (long)cget(p++, isp) << 16;
		case 1:	v |= (long)cget(p++, isp) << 8;
		case 0:	v |= (long)cget(p++, isp);
		}
		printf("$");
		psymoff((WORD)v, isp, "");
		break;

	case 21:
		v = ddisp();
		printf("*");
		psymoff((WORD)v, isp, "");
		break;

	case 22:
		v = ddisp();
		v1 = ddisp();
		psymoff((WORD)(mtodata(v, isp)+v1), isp, "");
		break;

	case 23:
		printf("tos");
		break;

	case 24:
	case 25:
	case 26:
	case 27:
		v = ddisp();
		psymoff((WORD)v, isp, "");
		printf("(%c%c)", "fssp"[g-24], "ppbp"[g-24]);
		break;

	case 28:
	case 29:
	case 30:
	case 31:
		if (t == -1)
			t = cget(p++, isp);
		dgen(t>>3, i, -1);
		printf("[r%d:%c]", t&07, "bwdq"[g&03]);
		break;
	}
}

static long
ddisp()
{
	register long o;

	o = cget(p++, isp);
	if(!(o & 0x80)) {
		if(o & 0x40)
			o |= 0xffffff80;
		return o;
	}
	o = (o<<8) | cget(p++, isp);
	if(!(o & 0x4000)) {
		o &= 0x3fff;
		if(o & 0x2000)
			o |= 0xffffc000;
		return o;
	}
	o = (o<<8) | cget(p++, isp);
	o = (o<<8) | cget(p++, isp);
	o &= 0x3fffffff;
	if(o & 0x20000000)
		o |= 0xc0000000;
	return o;
}

static
mask(m)
register int m;
{
	register int i;
	int first = 0;

	m &= 0xff;
	printf("[");
	for (i = 0; m; i++) {
		if (m & (1<<i)) {
			if (first++)
				printf(",");
			printf("r%d", i);
		}
		m &=~ (1<<i);
	}
	printf("]");
}

static
rmask(m)
register int m;
{
	register int i;
	int first = 0;

	m &= 0xff;
	printf("[");
	for (i = 0; m; i++) {
		if (m & (0x80>>i)) {
			if (first++)
				printf(",");
			printf("r%d", i);
		}
		m &=~ (0x80>>i);
	}
	printf("]");
}

static
quick(q)
register int q;
{

	q &= 0xf;
	if (q & 0x8)
		q |= ~0x7;
	printf("%r", q);
}

static char *pmrtab[] = {
	"bpr0", "bpr1", "2", "3",
	"pf0", "pf1", "6", "7",
	"sc", "9", "msr", "bcnt",
	"ptb0", "ptb1", "#d", "eia",
};

static
prmr(r)
int r;
{

	printf("%s", pmrtab[r]);
}

static char *pregtab[] = {
	"upsr", "1", "2", "3",
	"4", "5", "6", "7",
	"fp", "sp", "sb", "#b",
	"#c", "psr", "intbase", "mod",
};

static
prreg(r)
int r;
{

	printf("%s", pregtab[r]);
}
